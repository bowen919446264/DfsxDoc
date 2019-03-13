#pragma once
#include <list>
#include "DsUtil.h"
#ifdef __GNUC__

#endif // __GNUC__

namespace DSNleLib{
/***********************************************************
 *
 * CDsPool
 *
 ***********************************************************/

/**
 * @summary	template class for pooling objects.
 * @param	T - the class of the object to be pooled.
 */
template<typename T>
class TDsPool
{
public:
	TDsPool(int nMaxElementCount = 0)
	: m_nElementCount(0)
	, m_nElementCountOutsidePool(0)
	, m_nMaxElementCount(nMaxElementCount)
	,m_lWaiting(0)
	{
		if(m_nMaxElementCount > 0)
		{
			//m_hSemaphore = ::CreateSemaphore(NULL, m_nMaxElementCount, m_nMaxElementCount, NULL);
#ifdef WIN32
			m_hSemaphore = ::CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
#else
			//todo for __APPLE__
			//sem_init(&m_hSemaphore, 0, 0);
			m_hSemaphore = new Semaphore(0, 0x7FFFFFFF);
#endif // WIN32

		}
	}

	virtual ~TDsPool()
	{
		//ClearElement can't be called here because ClearElement calls a 
		//virtual function in the derived class's virtual function table  
		// which already destroyed at this time.

		//
		if(m_nMaxElementCount > 0)
		{
#ifdef WIN32
			SAFE_CLOSEHANDLE(m_hSemaphore);
#else
			//sem_destroy(&m_hSemaphore);
			SAFE_DELETE(m_hSemaphore);
#endif
		}

		assert(GetElementCount() == 0);
	}

	/**
	* @summary	clear all objects in the pool
	*/
	void ClearElement()
	{
		m_csElementsLock.Lock();
		for (typename  std::list<T*>::iterator i = m_Elements.begin();
			i != m_Elements.end(); i++)
		{
			DestroyElement(*i);
			m_nElementCount--;
		}
		m_Elements.clear();
		m_csElementsLock.Unlock();
	}

	int GetElementCount() { return m_nElementCount; }

	int GetElementCountOutsidePool() { return m_nElementCountOutsidePool; }

	void PreAlloc()
	{
		if (m_nMaxElementCount == 0)
			return;
		for (int i = 0; i < m_nMaxElementCount; i++)
		{
			T *pObj = CreateElement();
			if (pObj)
			{
				m_nElementCount++;
				m_Elements.push_back(pObj);
			}
		}
	}

	/**
	 * @summary	retrieve a object from the pool
	 * @return	the object retrieved.
	 */
	T* RetrieveElement(DWORD dwWait = INFINITE)
	{
		T* pObj = NULL;
		for(;;)
		{
			{
				CDsAutoLock lck(m_csElementsLock);
				if(!m_nMaxElementCount || m_nElementCountOutsidePool < m_nMaxElementCount )
				{
					if(!m_Elements.empty())
					{
						pObj = m_Elements.front();
						m_Elements.pop_front();
					}
					else
					{
						pObj = CreateElement();
						if(pObj)
						{
							m_nElementCount++;
							assert(m_nMaxElementCount <=0 || 
								m_nElementCount <= m_nMaxElementCount);
						}
					}
					if(pObj)m_nElementCountOutsidePool++;
				}
				else
				{
					m_lWaiting++;
				}
			}
			if(pObj)
				break;
#ifdef WIN32
			if(::WaitForSingleObject(m_hSemaphore, dwWait) != WAIT_OBJECT_0)
				break;
#else
			m_hSemaphore->wait(dwWait);
			//struct timespec ts;
			//if (clock_gettime(CLOCK_REALTIME, &ts) == (DWORD)-1)
			//	sem_wait(&m_hSemaphore);
			//else
			//{
			//	int msec = dwWait % 1000;
			//	int sec = dwWait / 1000;
			//	ts.tv_sec += sec;
			//	ts.tv_nsec += msec * 1000000;
			//	sem_timedwait(&m_hSemaphore, &ts);
			//}
#endif
		}
	
		return pObj;
	}

	/**
	 * @summary	rlease a object into the pool
	 * @param	pObj - the object to be released.
	 */
	BOOL ReleaseElement(T* pObj)
	{
		PreRelease(pObj);
		BOOL bRet = FALSE;
		if(pObj)
		{
			m_csElementsLock.Lock();
//			if(!m_nMaxElementCount || 
//				::ReleaseSemaphore(m_hSemaphore, 1, NULL)) //must be put inside lock operation
			{
				m_Elements.push_back(pObj);
				m_nElementCountOutsidePool--;
				if(m_lWaiting)
				{
#ifdef WIN32
					::ReleaseSemaphore(m_hSemaphore, m_lWaiting, NULL);
#else
					for (int i = 0; i < m_lWaiting; i++)
						//sem_post(&m_hSemaphore);
						m_hSemaphore->post();
#endif
					m_lWaiting = 0;
				}
				bRet = TRUE;
			}
			m_csElementsLock.Unlock();
		}

		return bRet;
	}


protected:
	/**
	 * @summary create a new object. if there are no objects in the pool, 
	 *			this member function will be called.
	 * @return	the new object created.
	 */
	virtual T* CreateElement() = 0;
	/**
	 * @summary	destroy a object. this member function is called on each objects in the
	 *			pool when it's being destroyed.
	 * @param	the object to be destroyed.
	 */
	virtual void DestroyElement(T* pObj) = 0;

	virtual void PreRelease(T* pObj){};


private:
	std::list<T*> m_Elements;
	CDsCritSec m_csElementsLock;
	int m_nElementCount;
	int m_nElementCountOutsidePool;
	int m_nMaxElementCount;
	long m_lWaiting;
#ifdef WIN32
	HANDLE m_hSemaphore;
#else
	//sem_t m_hSemaphore;
	Semaphore *m_hSemaphore;
#endif

};

/***********************************************************
 *
 * CDsMemoryPool
 *
 ***********************************************************/

 template<typename T>
class TDsMemoryPool : public TDsPool<T>
{
public:
	TDsMemoryPool(int iMax = 0)
	:TDsPool<T>(iMax)
	{
	}

	virtual ~TDsMemoryPool()
	{
		TDsPool<T>::ClearElement();

		assert(TDsPool<T>::GetElementCount() == 0);
	}
protected:
	virtual T* CreateElement()
	{
		BYTE* pBuffer = new BYTE[sizeof(T)];
		memset(pBuffer,0, sizeof(T));
		return reinterpret_cast<T*>(pBuffer);
	}

	virtual void DestroyElement(T* pObj)
	{
		delete[] reinterpret_cast<BYTE*>(pObj);
	}


};

/***********************************************************
 *
 * CDsUnknownMemoryPooled
 *
 ***********************************************************/
template<typename T>
class TDsUnknownMemoryPooled : public CDsUnknown
{
public:
#ifdef _MSC_VER
	typedef typename TDsMemoryPool<T> PoolType;
#else
	typedef  TDsMemoryPool<T> PoolType;
#endif

	TDsUnknownMemoryPooled(PoolType *pPool = NULL)
	: CDsUnknown(NULL)
	,m_pPool(pPool)
	{
	}

	virtual ~TDsUnknownMemoryPooled()
	{
	}

	virtual ULONG __stdcall NonDelegatingRelease()
	{
#ifdef WIN32
		LONG lRef = InterlockedDecrement(&m_lRefCount);
#else
		LONG lRef = __sync_sub_and_fetch(&m_lRefCount, 1);
#endif // WIN32

		assert(lRef >= 0);

		if (lRef == 0)
		{
//			m_lRefCount++;//£¡£¡£¡

			if(m_pPool) //memory used by this object is pooled
			{
				T* pObj			= (T*)this;
				PoolType* pPool	= m_pPool; //for later use. after ZeroMemory operation, m_pPool will no longer exist.
				//call deconstructor explicitly
				pObj->~T();
				//zero memory
				memset((void*)pObj, 0,sizeof(T));
				//return the memory to the pool
				pPool->ReleaseElement(pObj);
			}
			else //not pooled
			{
				delete this;
			}

			return ULONG(0);
		}
		else
		{
			return ULONG(lRef);
		}
	}
private:
	PoolType* m_pPool;
};
template<typename T>
class TDsUnknownPooled : public CDsUnknown
{
public:
#ifdef _MSC_VER
	typedef typename TDsPool<T> PoolType;
#else
	typedef  TDsPool<T> PoolType;
#endif

	TDsUnknownPooled(PoolType *pPool = NULL)
	:CDsUnknown(NULL)
	, m_pPool(pPool)
	{
	}

	virtual ~TDsUnknownPooled()
	{
	}

	virtual ULONG __stdcall NonDelegatingRelease()
	{
#ifdef WIN32
		LONG lRef = InterlockedDecrement(&m_lRefCount);
#else		
		LONG lRef = __sync_sub_and_fetch(&m_lRefCount, 1);
#endif // WIN32

		assert(lRef >= 0);

		if (lRef == 0)
		{

			if(m_pPool) //memory used by this object is pooled
			{
				T* pObj			= (T*)this;
				PoolType* pPool	= m_pPool; //for later use. after ZeroMemory operation, m_pPool will no longer exist.
				//call deconstructor explicitly
//				pObj->~T();
				//zero memory
//				::ZeroMemory(pObj, sizeof(T));
				//return the memory to the pool 
				pPool->ReleaseElement(pObj);
			}
			else //not pooled
			{
				m_lRefCount++;
				delete this;
			}

			return ULONG(0);
		}
		else
		{
			return ULONG(lRef);
		}
	}
private:
	PoolType* m_pPool;
};

#define pnew(oPool) new((void*)(oPool).RetrieveElement())
}
