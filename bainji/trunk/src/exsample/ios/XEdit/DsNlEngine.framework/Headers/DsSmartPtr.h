#pragma once
namespace DSNleLib {
//////////////////////////////////////////////////////////////////////////
//
// Summary:
//    This is the basic smart pointer template.
//
//////////////////////////////////////////////////////////////////////////
template < class T >
class TDsSmartPtr 
{
public:

	// Constructor
	TDsSmartPtr()
	{
		m_ptInterface = NULL ;
	}

	//
	// Summary:
	//    Constructor calls in_ptInterface->AddRef() and keeps interface pointer.
	TDsSmartPtr(T* ptInterface)   // COM pointer to handle.
	{
		m_ptInterface = NULL ; 

		if( ptInterface != NULL )
		{
			ptInterface -> AddRef();
		}

		m_ptInterface = ptInterface ;
	}

	//
	// Summary:
	//    Constructor calls in_ptInterface->AddRef() and keeps interface pointer.
	TDsSmartPtr(const TDsSmartPtr<T>  & reftInterface)   // reference to the COM pointer to handle.
	{
		m_ptInterface = NULL ; 

		if( reftInterface.m_ptInterface != NULL )
		{
			reftInterface.m_ptInterface -> AddRef();
		}

		m_ptInterface = reftInterface.m_ptInterface ;
	}

	//
	// Summary:
	//    Destructor calls Release() on the internal interface that was passed in the constructor.
	//
	virtual ~TDsSmartPtr()
	{
		Release();
	}

	//
	// Summary:
	//    Replaces the current pointer with the new one passed as parameter without doing an 
	//    AddRef().  The old one is released.
	//
	T* AssignNew(T * ptInterface)   // New COM pointer to handle.
	{
		if( m_ptInterface != NULL )
		{
			m_ptInterface -> Release();
			m_ptInterface = NULL ;
		}

		// We assign the newly created interface directly, without AddRef-ing
		// beacause its RefCount is already set to 1, and we are tbe the only
		// ones keeping the pointer on that interface.
		m_ptInterface = ptInterface ;

		return m_ptInterface ;
	}

	//
	// Summary:
	//    Release the internal pointer.
	//
	void Release()
	{
		if( m_ptInterface != NULL )
		{
			m_ptInterface -> Release();
		}

		m_ptInterface = NULL ;
	}

	//
	// Summary:
	//    Returns the internal pointer.
	//      
	virtual operator T*() const 
	{
		return  m_ptInterface;
	}

	//
	// Summary:
	//    Returns the internal pointer as an object.
	virtual T& operator*()
	{
		if( m_ptInterface == NULL )
		{
			assert(false) ;
		}
		return *m_ptInterface;
	}

	//
	// Summary:
	//    Return the address of the internal variable containning the internal pointer.
	//
	virtual T** operator&()
	{
		if( m_ptInterface != NULL )
		{
			assert(false) ;
		}
		return &m_ptInterface ;
	}

	//
	// Summary:
	//    Access the internal pointer
	//
	virtual T* operator->()
	{
		if( m_ptInterface == NULL )
		{
			assert(false) ;
		}
		return m_ptInterface ;
	}

	//
	// Summary:
	//    Assign a new pointer.  The old pointer is released and the new one is "AddRefed".
	//
	virtual T* operator=( T* ptInterface )
	{
		if( ptInterface != NULL )
		{
			ptInterface -> AddRef();
		}

		if( m_ptInterface != NULL )
		{
			m_ptInterface -> Release();
			m_ptInterface = NULL ;
		}

		m_ptInterface = ptInterface ;

		return m_ptInterface ;
	}

	//
	// Summary:
	//    NOT operator to replace (m_pTInterface == NULL)
	//
	virtual bool operator!()
	{
		return (m_ptInterface == NULL);
	}

	//
	// Summary:
	//    Compare internal pointers.
	//
	virtual bool operator==(T* ptInterface)
	{
		return (m_ptInterface == ptInterface);
	}

	//
	// Summary:
	//    Assign a new pointer in a diffent way.
	//
	T* operator=( const TDsSmartPtr<T> &reftInterface )
	{
		return operator=( reftInterface.m_ptInterface );
	}

protected:
	T *	m_ptInterface;  // any interface.   
};


//////////////////////////////////////////////////////////////////////////
//
// Summary:
//    This template is derived from the TDsSmartPtr template, it only adds the possibility to get 
//    another type interface pointer from a received interface.
//
//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
template <class T, const IID* piid = &__uuidof(T)>
class TDsSmartQPtr : public TDsSmartPtr<T>
{
public:

	//
	// Summary:
	//    Default constructor.  Internal pointer is NULL.
	TDsSmartQPtr()
	{
	}

	//
	// Summary:
	//    Constructor calls in_ptInterface->AddRef() and keeps interface pointer.
	TDsSmartQPtr(T* ptInterface)
		: TDsSmartPtr<T>(ptInterface)
	{
	}

	//
	// Summary:
	//    Copy constructor.
	//
	TDsSmartQPtr(const TDsSmartQPtr<T,piid>& reftInterface)
		: TDsSmartPtr<T>(reftInterface.m_ptInterface)
	{
	}

	//
	// Summary:
	//    Constructor queries template interface T on the received IUnknown pointer.  Constructor 
	//    calls in_pIUnk->AddRef() and keeps the template interface pointer T.
	TDsSmartQPtr(IUnknown *pIUnk)
	{
		if (pIUnk != NULL)
		{
			T * pINewInterface=NULL;

			HRESULT hr = pIUnk->QueryInterface(*piid, (void **)&pINewInterface);

			if (SUCCEEDED(hr))
			{
				TDsSmartPtr<T>::m_ptInterface = pINewInterface;
			}
			else
			{
				assert(false) ;
			}
		}
		else
		{
			assert(false) ;
		}
	}

	//
	// Summary:
	//    Constructor queries template interface T on the received IUnknown pointer.  Constructor 
	//    calls in_pIUnk->AddRef() and keeps the template interface pointer T.  Error is returned 
	//    in io_phr.
	TDsSmartQPtr(IUnknown *pIUnk, HRESULT *phr)
	{
		if (pIUnk != NULL)
		{
			T * pINewInterface=NULL;

			*phr = pIUnk->QueryInterface( *piid, (void **)&pINewInterface );

			if (SUCCEEDED(*phr))
			{
				TDsSmartPtr<T>::m_ptInterface = pINewInterface;
			}			
		}
		else
		{
			*phr = E_POINTER;
		}
	}
};
#endif
};