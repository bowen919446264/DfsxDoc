
#pragma once
#include "DsExport.h"
#include "DsObject.h"
#include "DsSmartPtr.h"
#if _MSC_VER >= 1600
#include <intrin.h>
#endif
#ifdef  __GNUC__
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdint.h>
#endif //  __GNUC__

namespace DSNleLib {
	/*************************************************
	 *
	 * Macro Definitions
	 *
	 *************************************************/

	 /**
	  * @summary	decrease references on the object safely.
	  * @param	p - the object to be decreased refereneces.
	  */
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p) { (p)->Release(); (p) = NULL; }
#endif
	  /**
	   * @summary	delete the object safely.
	   * @param	p - the object to be deleted.
	   */
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p) { delete (p); (p) = NULL; }
#endif
	   /**
		* @summary	delete the array safely.
		* @param	p - the array to be deleted.
		*/
#ifndef SAFE_DELETEARRAY
#define SAFE_DELETEARRAY(p) if(p) { delete[] (p); (p) = NULL; }
#endif
		/**
		 * @summary	prompt the message in a message box
		 * @param	msg - the message to be prompted.
		 */

		 // wrapper for whatever critical section we have
		 // Intrinsics available in VC6.0

#ifdef _MSC_VER
/**
* @summary	close the windows handle safely.
* @param	p - the handle to be closed.
*/
#ifndef SAFE_CLOSEHANDLE

#define SAFE_CLOSEHANDLE(p) if(p) { ::CloseHandle(p); (p) = NULL; }
#endif

#ifndef ERRORMSG
#define ERRORMSG(msg) ::MessageBox(NULL, msg, TEXT("Error"), MB_OK);
#endif
	LIBNLE_FUNC HRESULT AppendModulePath(LPTSTR szTarget, LPCTSTR szPath);
	LIBNLE_FUNC HRESULT SetDllDirectory(LPCTSTR szPathName);

#if _MSC_VER < 1600
	extern "C" long __cdecl _InterlockedDecrement(volatile long *p);
	extern "C" long __cdecl _InterlockedIncrement(volatile long *p);
	extern "C" long __cdecl _InterlockedCompareExchange(volatile long *p, long n, long p_compare);
	extern "C" long __cdecl _InterlockedExchange(volatile long *p, long n);
	extern "C" long __cdecl _InterlockedExchangeAdd(volatile long *p, long n);
#else
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#endif

#else

#endif

#ifdef _MSC_VER
// Intrinsics available in VC7.1. Note that the compiler is smart enough to
// use straight LOCK AND/OR/XOR if the return value is not needed; otherwise
// it uses a LOCK CMPXCHG loop.
#if _MSC_VER >= 1310
	extern "C" long __cdecl _InterlockedAnd(volatile long *p, long n);
	extern "C" long __cdecl _InterlockedOr(volatile long *p, long n);
	extern "C" long __cdecl _InterlockedXor(volatile long *p, long n);

#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedXor)
#endif

	// Intrinsics available with AMD64
#ifdef _M_AMD64
	extern "C" void *__cdecl _InterlockedExchangePointer(void *volatile *pp, void *p);
#pragma intrinsic(_InterlockedExchangePointer)
	extern "C" void *__cdecl _InterlockedCompareExchangePointer(void *volatile *pp, void *p, void *compare);
#pragma intrinsic(_InterlockedCompareExchangePointer)
#endif

	inline void *CDsAtomicCompareExchangePointer(void *volatile *pp, void *p, void *compare) {
#ifdef _M_AMD64
		return _InterlockedCompareExchangePointer(pp, p, compare);
#else
		return (void *)(__w64 int32_t)_InterlockedCompareExchange((volatile long *)(volatile __w64 int32_t *)pp, (long)(__w64 int32_t)p, (long)(__w64 int32_t)compare);
#endif

	}
#endif

	///////////////////////////////////////////////////////////////////////////
	/// \class CDsAtomicInt
	/// \brief Wrapped integer supporting thread-safe atomic operations.
	///
	/// CDsAtomicInt allows integer values shared between threads to be
	/// modified with several common operations in a lock-less manner and
	/// without the need for explicit barriers. This is particularly useful
	/// for thread-safe reference counting.
	///
	class CDsAtomicInt {
	protected:
		volatile int n;

	public:
		CDsAtomicInt() {}
		CDsAtomicInt(int v) : n(v) {}

		bool operator!() const { return !n; }
		bool operator!=(volatile int v) const { return n != v; }
		bool operator==(volatile int v) const { return n == v; }
		bool operator<=(volatile int v) const { return n <= v; }
		bool operator>=(volatile int v) const { return n >= v; }
		bool operator<(volatile int v) const { return n < v; }
		bool operator>(volatile int v) const { return n > v; }

		///////////////////////////////

		/// Atomically exchanges a value with an integer in memory.
		static inline int staticExchange(volatile int *dst, int v) {
#ifdef _MSC_VER
			return (int)_InterlockedExchange((volatile long *)dst, v);
#else
			return __sync_lock_test_and_set(dst, v);
#endif
		}

		/// Atomically adds one to an integer in memory.
		static inline void staticIncrement(volatile int *dst) {
#ifdef _MSC_VER
			_InterlockedExchangeAdd((volatile long *)dst, 1);
#else
			__sync_fetch_and_add(dst, 1);
#endif
		}

		/// Atomically subtracts one from an integer in memory.
		static inline void staticDecrement(volatile int *dst) {
#ifdef _MSC_VER
			_InterlockedExchangeAdd((volatile long *)dst, -1);
#else
			__sync_fetch_and_sub(dst, 1);
#endif
		}

		/// Atomically subtracts one from an integer in memory and returns
		/// true if the result is zero.
		static inline bool staticDecrementTestZero(volatile int *dst) {
#ifdef _MSC_VER
			return 1 == _InterlockedExchangeAdd((volatile long *)dst, -1);
#else
			return 1 == __sync_fetch_and_add(dst, -1);
#endif
		}

		/// Atomically adds a value to an integer in memory and returns the
		/// result.
		static inline int staticAdd(volatile int *dst, int v) {
#ifdef _MSC_VER
			return (int)_InterlockedExchangeAdd((volatile long *)dst, v) + v;
#else
			return __sync_add_and_fetch(dst, v);
#endif
		}

		/// Atomically adds a value to an integer in memory and returns the
		/// old result (post-add).
		static inline int staticExchangeAdd(volatile int *dst, int v) {
#ifdef _MSC_VER
			return _InterlockedExchangeAdd((volatile long *)dst, v);
#else
			return __sync_fetch_and_add(dst, v);
#endif
		}

		/// Atomically compares an integer in memory to a compare value and
		/// swaps the memory location with a second value if the compare
		/// succeeds. The return value is the memory value prior to the swap.
		static inline int staticCompareExchange(volatile int *dst, int v, int compare) {
#ifdef _MSC_VER
			return _InterlockedCompareExchange((volatile long *)dst, v, compare);
#else
			return __sync_val_compare_and_swap(dst, v, compare);
#endif
		}

		///////////////////////////////

		int operator=(int v) { return n = v; }

		int operator++() { return staticAdd(&n, 1); }
		int operator--() { return staticAdd(&n, -1); }
		int operator++(int) { return staticExchangeAdd(&n, 1); }
		int operator--(int) { return staticExchangeAdd(&n, -1); }
		int operator+=(int v) { return staticAdd(&n, v); }
		int operator-=(int v) { return staticAdd(&n, -v); }

#ifdef _MSC_VER

#if _MSC_VER >= 1310
		void operator&=(int v) { _InterlockedAnd((volatile long *)&n, v); }	///< Atomic bitwise AND.
		void operator|=(int v) { _InterlockedOr((volatile long *)&n, v); }		///< Atomic bitwise OR.
		void operator^=(int v) { _InterlockedXor((volatile long *)&n, v); }	///< Atomic bitwise XOR.
#else
		/// Atomic bitwise AND.
		void operator&=(int v) {
			__asm mov eax, v
			__asm mov ecx, this
			__asm lock and dword ptr[ecx], eax
		}

		/// Atomic bitwise OR.
		void operator|=(int v) {
			__asm mov eax, v
			__asm mov ecx, this
			__asm lock or dword ptr[ecx], eax
		}

		/// Atomic bitwise XOR.
		void operator^=(int v) {
			__asm mov eax, v
			__asm mov ecx, this
			__asm lock xor dword ptr[ecx], eax
		}
#endif
#else
		void operator&=(int v) { __sync_fetch_and_and(&n, v); }	///< Atomic bitwise AND.
		void operator|=(int v) { __sync_fetch_and_or(&n, v); }		///< Atomic bitwise OR.
		void operator^=(int v) { __sync_fetch_and_xor(&n, v); }	///< Atomic bitwise XOR.
#endif
		operator int() const {
			return n;
		}

		/// Atomic exchange.
		int xchg(int v) {
			return staticExchange(&n, v);
		}

		/// Compare/exchange (486+).
		int compareExchange(int newValue, int oldValue) {
			return staticCompareExchange(&n, newValue, oldValue);
		}

		// 486 only, but much nicer.  They return the actual result.

		int inc() { return operator++(); }				///< Atomic increment.
		int dec() { return operator--(); }				///< Atomic decrement.
		int add(int v) { return operator+=(v); }				///< Atomic add.

		// These return the result before the operation, which is more inline with
		// what XADD allows us to do.

		int postinc() { return operator++(0); }				///< Atomic post-increment.
		int postdec() { return operator--(0); }				///< Atomic post-decrement.
		int postadd(int v) { return staticExchangeAdd(&n, v); }	///< Atomic post-add.

	};

	///////////////////////////////////////////////////////////////////////////

	class CDsAtomicFloat {
	protected:
		volatile float n;

	public:
		CDsAtomicFloat() {}
		CDsAtomicFloat(float v) : n(v) {}

		bool operator!=(float v) const { return n != v; }
		bool operator==(float v) const { return n == v; }
		bool operator<=(float v) const { return n <= v; }
		bool operator>=(float v) const { return n >= v; }
		bool operator<(float v) const { return n < v; }
		bool operator>(float v) const { return n > v; }

		float operator=(float v) { return n = v; }

		operator float() const {
			return n;
		}

		/// Atomic exchange.
		float xchg(float v) {
			union { int i; float f; } converter = { CDsAtomicInt::staticExchange((volatile int *)&n, *(const int *)&v) };

			return converter.f;
		}
	};

#ifdef WIN32
	///////////////////////////////////////////////////////////////////////////
	/// \class CDsAtomicPtr
	/// \brief Wrapped pointer supporting thread-safe atomic operations.
	///
	/// CDsAtomicPtr allows a shared pointer to be safely manipulated by
	/// multiple threads without locks. Note that atomicity is only guaranteed
	/// for the pointer itself, so any operations on the object must be dealt
	/// with in other manners, such as an inner lock or other atomic
	/// operations. An atomic pointer can serve as a single entry queue.
	///
	template<typename T>
	class CDsAtomicPtr {
	protected:
		T *volatile ptr;

	public:
		CDsAtomicPtr() {}
		CDsAtomicPtr(T *p) : ptr(p) { }

		operator T*() const { return ptr; }
		T* operator->() const { return ptr; }

		T* operator=(T* p) {
			return ptr = p;
		}

		/// Atomic pointer exchange.
		T *xchg(T* p) {
#ifdef _M_AMD64
			return ptr == p ? p : (T *)_InterlockedExchangePointer((void *volatile *)&ptr, p);
#else
			return ptr == p ? p : (T *)_InterlockedExchange((volatile long *)&ptr, (long)p);
#endif
		}

		T *compareExchange(T *newValue, T *oldValue) {
#ifdef _M_AMD64
			return (T *)_InterlockedCompareExchangePointer((void *volatile *)&ptr, (void *)newValue, (void *)oldValue);
#else
			return (T *)_InterlockedCompareExchange((volatile long *)&ptr, (long)(size_t)newValue, (long)(size_t)oldValue);
#endif
		}
	};

#endif // WIN32

	///////////////////////////////////////////////////////////////////////////

	class CDsCritSec {
	private:
		struct CritSec {				// This is a clone of CRITICAL_SECTION.
			void	*DebugInfo;
			int32_t	LockCount;
			int32_t	RecursionCount;
			void	*OwningThread;
			void	*LockSemaphore;
			uint32_t	SpinCount;
		} csect;

#ifndef WIN32
		pthread_mutex_t _mux;
#endif // !WIN32

		CDsCritSec(const CDsCritSec&);
		const CDsCritSec& operator=(const CDsCritSec&);
		//static void StructCheck() {	ASSERT(sizeof(CritSec) == sizeof(CRITICAL_SECTION))};
	public:

		CDsCritSec() {
#ifdef WIN32
			InitializeCriticalSection((_RTL_CRITICAL_SECTION *)&csect);
#else
			//_mux = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
			pthread_mutexattr_t Attr;

			pthread_mutexattr_init(&Attr);
			pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&_mux, &Attr);
#endif // WIN32

		}

		~CDsCritSec() {
#ifdef WIN32
			DeleteCriticalSection((_RTL_CRITICAL_SECTION *)&csect);
#else
			pthread_mutex_destroy(&_mux);
#endif
		}

		void operator++() {
#ifdef WIN32
			EnterCriticalSection((_RTL_CRITICAL_SECTION *)&csect);
#else
			pthread_mutex_lock(&_mux);
#endif
		}

		void operator--() {
#ifdef WIN32
			LeaveCriticalSection((_RTL_CRITICAL_SECTION *)&csect);
#else
			pthread_mutex_unlock(&_mux);
#endif
		}

		void Lock() {
#ifdef WIN32
			EnterCriticalSection((_RTL_CRITICAL_SECTION *)&csect);
#else
			pthread_mutex_lock(&_mux);
#endif
		}

		void Unlock() {
#ifdef WIN32
			LeaveCriticalSection((_RTL_CRITICAL_SECTION *)&csect);
#else
			pthread_mutex_unlock(&_mux);
#endif
		}
	};

	class CDsAutoLock {
	private:
		CDsCritSec& cs;
	public:
		CDsAutoLock(CDsCritSec& csect) : cs(csect) { cs.Lock(); }
		~CDsAutoLock() { cs.Unlock(); }

		inline operator bool() const { return false; }
	};


	LIBNLE_FUNC void *  ds_malloc(unsigned int size);
	LIBNLE_FUNC void  ds_free(void *p);

	// ----------------------------------------------------------------------------
	// Function name   : long g_ResolutionToAudioSampleCount
	// Description     : 
	// ----------------------------------------------------------------------------
	inline unsigned long g_ResolutionToAudioSampleCount(SDsResolutionInfo & in_rsResolution)
	{
		switch (in_rsResolution.eFrameRate)
		{
		case keDsFrameRate24:   return 2000;   break;
		case keDsFrameRate24M:  return 2002;   break;
		case keDsFrameRate25:   return 1920;   break;
		case keDsFrameRate30:   return 1600;   break;
		case keDsFrameRate30M:  return 1602;   break;
		case keDsFrameRate50:   return 960;    break;
		case keDsFrameRate60:   return 800;    break;
		case keDsFrameRate60M:  return 801;    break;
		default:                               break;
		}

		return 0;
	}

	inline unsigned long g_GetRowPitch(EDsSurfaceFormat eFormat, unsigned long ulWidth, unsigned long ulComponentBits)
	{
		unsigned long ulPitch = ulWidth;
		switch (eFormat)
		{
		case keDsSurfaceFormatA:
			ulPitch = ulWidth;
		case keDsSurfaceFormatYUYV422:
			ulPitch *= 2;
			break;
		case keDsSurfaceFormatRGBGraphic:
		case keDsSurfaceFormatRGBVideo:
		case keDsSurfaceFormatYUAYVA4224:
			ulPitch *= 3;
		case keDsSurfaceFormatARGBVideo:
		case keDsSurfaceFormatARGBGraphic:
			ulPitch *= 4;
			break;
		default:
			assert(false);
		}
		ulPitch = (ulPitch + 63)&(~63);

		return ulPitch;
	}

#ifdef __GNUC__

	class Semaphore {
	protected:
		pthread_mutex_t mutex;
		pthread_cond_t pCond;
		pthread_cond_t cCond;
		unsigned int waiters;
		unsigned int max;
	public:
		Semaphore(unsigned int,unsigned int);
		virtual ~Semaphore();
		int post();
		int wait(uint64_t milliseconds = (DWORD)-1);
	};



	/*

	* WIN32 Events for POSIX

	* Author: Mahmoud Al-Qudsi <mqudsi@neosmart.net>

	* Copyright (C) 2011 - 2015 by NeoSmart Technologies

	* This code is released under the terms of the MIT License

	*/



#pragma once



#if defined(_WIN32) && !defined(CreateEvent)

#error Must include Windows.h prior to including pevents.h!

#endif

#ifndef WAIT_TIMEOUT

#define WAIT_TIMEOUT ETIMEDOUT
#endif
#define WAIT_OBJECT_0 0
#define WFMO
	//Type declarations
	struct g_event_t_;
	typedef g_event_t_ * g_event_t;
	//WIN32-style functions
	g_event_t CreateEvent(bool manualReset = false, bool initialState = false);

	int DestroyEvent(g_event_t event);

	int WaitForEvent(g_event_t event, uint64_t milliseconds = (DWORD)-1);

	int SetEvent(g_event_t event);

	int ResetEvent(g_event_t event);

#ifdef WFMO

	int WaitForMultipleEvents(g_event_t *events, int count, bool waitAll, uint64_t milliseconds);

	int WaitForMultipleEvents(g_event_t *events, int count, bool waitAll, uint64_t milliseconds, int &index);
#endif
#ifdef PULSE

	int PulseEvent(neosmart_event_t event);
#endif

	//POSIX-style functions
	//TBD
#endif // __GNUC__

}