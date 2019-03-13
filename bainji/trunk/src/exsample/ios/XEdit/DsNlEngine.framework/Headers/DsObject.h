#pragma once
#ifdef _MSC_VER
#include <objbase.h>
#include <InitGuid.h>
#elif __GNUC__
//#define interface               struct FAR

#ifdef COM_STDMETHOD_CAN_THROW
#define COM_DECLSPEC_NOTHROW
#else
#define COM_DECLSPEC_NOTHROW DECLSPEC_NOTHROW
#endif
#define DECLSPEC_NOVTABLE 
#define DECLSPEC_UUID(x)
#define __STRUCT__ struct
#define interface __STRUCT__
#define STDMETHOD(method)        virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)  virtual COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE method
#define STDMETHODV(method)       virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_(type,method) virtual COM_DECLSPEC_NOTHROW type STDMETHODVCALLTYPE method
#define PURE                    = 0
#define THIS_
#define THIS                    void
#define DECLARE_INTERFACE(iface)                        interface DECLSPEC_NOVTABLE iface
#define DECLARE_INTERFACE_(iface, baseiface)            interface DECLSPEC_NOVTABLE iface : public baseiface
#define DECLARE_INTERFACE_IID(iface, iid)               interface DECLSPEC_UUID(iid) DECLSPEC_NOVTABLE iface
#define DECLARE_INTERFACE_IID_(iface, baseiface, iid)   interface DECLSPEC_UUID(iid) DECLSPEC_NOVTABLE iface : public baseiface

#define IFACEMETHOD(method)         __override STDMETHOD(method)
#define IFACEMETHOD_(type,method)   __override STDMETHOD_(type,method)
#define IFACEMETHODV(method)        __override STDMETHODV(method)
#define IFACEMETHODV_(type,method)  __override STDMETHODV_(type,method)

#if !defined(BEGIN_INTERFACE)
#if defined(_MPPC_)  && \
    ( (defined(_MSC_VER) || defined(__SC__) || defined(__MWERKS__)) && \
    !defined(NO_NULL_VTABLE_ENTRY) )
#define BEGIN_INTERFACE virtual void a() {}
#define END_INTERFACE
#else
#define BEGIN_INTERFACE
#define END_INTERFACE
#endif
#endif


#define MIDL_INTERFACE(x)   struct DECLSPEC_UUID(x) DECLSPEC_NOVTABLE
MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
IUnknown
{
public:
	BEGIN_INTERFACE
		virtual HRESULT  QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void  ** ppvObject) = 0;

	virtual unsigned long  AddRef(void) = 0;

	virtual unsigned long  Release(void) = 0;

	END_INTERFACE
};

typedef /* [unique] */ IUnknown *LPUNKNOWN;

DEFINE_GUID(IID_IUnknown,0x000000,0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);

#endif

#include "DsExport.h"
namespace DSNleLib {
#ifndef INONDELEGATINGUNKNOWN_DEFINED
DECLARE_INTERFACE(INonDelegatingUnknown)
{
	virtual HRESULT			__stdcall NonDelegatingQueryInterface(THIS_ REFIID, LPVOID *) PURE;
	virtual unsigned long	__stdcall NonDelegatingAddRef(THIS) PURE;
	virtual unsigned long	__stdcall NonDelegatingRelease(THIS) PURE;
};
#define INONDELEGATINGUNKNOWN_DEFINED
#endif

class LIBNLE_CLASS CDsBaseObject
{
public:

CDsBaseObject()
{
}

~CDsBaseObject()
{
}

private:
	CDsBaseObject(const CDsBaseObject&);  // no implementation
	void operator=(const CDsBaseObject&); // no implementation
};


/* We have to ensure that we DON'T use a max macro, since these will typically   */
/* lead to one of the parameters being evaluated twice.  Since we are worried    */
/* about concurrency, we can't afford to access the m_cRef twice since we can't  */
/* afford to run the risk that its value having changed between accesses.        */

template<class T> inline static T ourmax( const T & a, const T & b )
{
    return a > b ? a : b;
}
class LIBNLE_CLASS CDsUnknown : public INonDelegatingUnknown
							 , public CDsBaseObject
{
public:
#ifdef _MSC_VER
#pragma warning( disable : 4355 4100 )
#endif
	explicit CDsUnknown(IUnknown *pOutter=NULL)
	: m_lRefCount(0)
	, m_pUnknown( pOutter ? pOutter : reinterpret_cast<LPUNKNOWN>( static_cast<INonDelegatingUnknown*>(this) ) )
	{
	};
#ifdef _MSC_VER
#pragma warning( default : 4355 4100 )
#endif
	virtual ~CDsUnknown(){};

	LPUNKNOWN GetOwner() const { return m_pUnknown; }

	virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID riid, void **ppv)
	{
		if(!ppv)return E_POINTER;

		if(riid == IID_IUnknown)
		{
			LPUNKNOWN pUnk = (LPUNKNOWN)(INonDelegatingUnknown*)this;
			pUnk->AddRef();
			*ppv = pUnk;

			return S_OK;
		}
		else
		{
			*ppv = NULL;

			return E_NOINTERFACE;
		}
	};

	ULONG  __stdcall NonDelegatingAddRef()
	{
#ifdef _MSC_VER
		LONG lRef = InterlockedIncrement( &m_lRefCount );
#else
		LONG lRef = __sync_add_and_fetch(&m_lRefCount,1);
#endif
		assert(lRef > 0);

		return ourmax(ULONG(m_lRefCount), 1ul);
	};

	ULONG  __stdcall NonDelegatingRelease()
	{
#ifdef _MSC_VER
		LONG lRef = InterlockedDecrement( &m_lRefCount );
#else
		LONG lRef = __sync_sub_and_fetch(&m_lRefCount,1);
#endif
		assert(lRef >= 0);

		if (lRef == 0)
		{
			m_lRefCount++;

			delete this;
			return ULONG(0);
		}
		else
		{
			return ourmax(ULONG(m_lRefCount), 1ul);
		}
	};
protected:
	volatile LONG m_lRefCount;
private:
	const LPUNKNOWN m_pUnknown;
};

#define DS_DECLARE_IUNKNOWN															\
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)				\
	{																				\
		return GetOwner()->QueryInterface(riid,ppv);								\
	}																				\
	virtual ULONG __stdcall AddRef()												\
	{																				\
		return GetOwner()->AddRef();												\
	}																				\
	virtual ULONG __stdcall Release()												\
	{																				\
		return GetOwner()->Release();												\
	}
#define DS_IMPLEMENT_INTERFACE_1(interface1, baseClass)								\
	virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID riid, void ** ppv)	\
	{																				\
		if(!ppv)return E_POINTER;													\
		if(riid == IID_##interface1)												\
		{																			\
			LPUNKNOWN pUnk = (LPUNKNOWN)(interface1*)this;							\
			pUnk->AddRef();															\
			*ppv = pUnk;															\
			return S_OK;															\
		}																			\
		else																		\
		{																			\
			return baseClass::NonDelegatingQueryInterface(riid, ppv);				\
		}																			\
	}

#define DS_IMPLEMENT_INTERFACE_2(interface1, interface2, baseClass)					\
	virtual HRESULT __stdcall NonDelegatingQueryInterface(REFIID riid, void ** ppv)	\
	{																				\
		if(!ppv)return E_POINTER;													\
		if(riid == IID_##interface1)												\
		{																			\
			LPUNKNOWN pUnk = (LPUNKNOWN)(interface1*)this;							\
			pUnk->AddRef();															\
			*ppv = pUnk;															\
			return S_OK;															\
		}																			\
		else if(riid == IID_##interface2)											\
		{																			\
			LPUNKNOWN pUnk = (LPUNKNOWN)(interface2*)this;							\
			pUnk->AddRef();															\
			*ppv = pUnk;															\
			return S_OK;															\
		}																			\
		else																		\
		{																			\
			return baseClass::NonDelegatingQueryInterface(riid, ppv);				\
		}																			\
	}

};