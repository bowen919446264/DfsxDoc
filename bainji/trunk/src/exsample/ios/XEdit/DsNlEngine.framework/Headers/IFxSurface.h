#ifndef __FXSURFACE_H__
#define __FXSURFACE_H__
#pragma once
#include "FxExport.h"

struct IMvSurface;
interface IDirect3DTexture9;
interface IDirect3DSurface9;
class IFxEngine;
class IFxSurface;

/**
 * @summary	enumeration type of format kinds of fx surface
 */
typedef enum
{
	EFxSurfaceFormat_Invalid = 0,		// invalid format
	EFxSurfaceFormat_R8G8B8A8,			// R8G8B8A8 format
	EFxSurfaceFormat_R8G8B8A8Compressed,// R8G8B8A8 format. Black is 16, white is 235. Can contain super white and/or super black.
	EFxSurfaceFormat_Y8U8Y8V8,			// Y8U8Y8V8 format
	EFxSurfaceFormat_Y8U8A8Y8V8A8,		// Y8U8A8Y8V8A8 format
	EFxSurfaceFormat_Y8U8V8A8,			// Y8U8V8A8 format
	EFxSurfaceFormat_A8,				// A8 format //can't be put in EFxMemoryLocation_Texture
	EFxSurfaceFormat_B16G16R16A16,		// A16B16G16R16 format
	EFxSurfaceFormat_Count				// total count of format kinds
}EFxSurfaceFormat;

/**
 * @summary	enumeration type of colorimetry kinds of fx surface
 */
typedef enum
{
	EFxColorimetry_Invalid = 0,		// invalid colorimetry
	EFxColorimetry_BT_601,			// BT 601(SD) colorimetry
	EFxColorimetry_BT_709,			// BT 709(HD) colorimetry
	EFxColorimetry_Count			// total count of colorimetry kinds
}EFxColorimetry;

/**
 * @summary	enumeration type of memory kinds of fx surface
 */
typedef enum
{
	EFxMemoryLocation_Invalid = 0,		// invalid member location
	EFxMemoryLocation_Host,				// surface is located in Host memory
	EFxMemoryLocation_Texture,			// surface is a D3D texture of render target type
	EFxMemoryLocation_DynamicTexture,	// surface is a D3D texture of dynamic type
	EFxMemoryLocation_HostTexture,		// surface is a D3D texture of host type
	
	EFxMemoryLocation_Count				// total count of memory kinds
}EFxMemoryLocation;

/**
 * @summary	enumeration type of polarity kinds of fx surface
 */
typedef enum
{
	EFxPolarity_Invalid = 0,		// invalid polarity
	EFxPolarity_FirstField,			// surface is a first field
	EFxPolarity_SecondField,		// surface is a second field
	EFxPolarity_ProgressiveFrame,	// surface is a progress frame
	EFxPolarity_Count				// total count of polarity kinds
}EFxPolarity;

/**
 * @summary	copy flag used with IFxEngine::CopySurface
 */
typedef enum
{
	EFxCopySurface_None			= 0x0000,	// default
	EFxCopySurface_Discard		= 0x0001,	// if this flag is on, destination surface's content will not 
											// be taken care in copy progress.
	EFxCopySurface_InterlaceMode= 0x0002, 	// just support	*host			->dynamic texture
											//				*texture		->host
											//				*dynamic texture->host
											//				host			->host texture
											//				dynamic texture	->host texture
											//				host texture	->host
											//				host texture	->dynamic texture
	EFxCopySurface_ClearTarget	= 0x0004	// clear destination surface before copy surface
											// can't be used with EFxCopySurface_Discard
											// only support R8G8B8A8, R8G8B8A8Compressed, Y8U8V8A8, A8 format
}EFxCopySurfaceFlag;

/**
 * @summary	structure for describing a surface
 */
struct SFxSurfaceDescription
{
	DWORD dwSize;						// structure size. in bytes.
	EFxSurfaceFormat eFormat;			// surface format.
	EFxColorimetry eColorimetry;		// surface colorimetry
	EFxMemoryLocation eMemoryLocation;	// surface memory location.
	EFxPolarity ePolarity;				// surface polarity
	RECT rcSurface;						// rectangle of this surface on the screen. X axis is left to right and 
										// Y axis is up to down. left-right corner of the screen is assumed to be (0, 0)
	BOOL bPremultiplied;				// is this surface's color components premultiplied by alpha component
	int nMipmapCount;					// how many mipmap this surface has. used for Texture surface.
};

/**
 * @summary	structure for describing a locked surface
 */
struct SFxSurfaceLockedInfo
{
	DWORD dwSize;		// structure size. in bytes.
	void* pData;		// pointer of surface's data buffer. may be NULL
	ULONG ulPitch;		// pitch of surface's data buffer. if pData is NULL, ulPitch will be 0.
};

/**
 * @summary	structure for describing a locked Texture surface
 */
struct SFxD3DTexSurfaceLockedInfo : public SFxSurfaceLockedInfo
{
	IDirect3DTexture9* pTexture;	// the pointer of D3D texture for storing this surface
	IDirect3DSurface9* pSurface;	// the pointer of level 0 D3D surface for storing this surface
};

/**
 * @summary	structure for describing a locked DynamicTexture surface
 */
struct SFxD3DDynTexSurfaceLockedInfo : public SFxSurfaceLockedInfo
{
	IDirect3DTexture9* pTexture;	// the pointer of D3D texture for storing this surface
	IDirect3DSurface9* pSurface;	// the pointer of level 0 D3D surface for storing this surface
};

/**
 * @summary	structure for describing a locked HostTexture surface
 */
struct SFxD3DHostTexSurfaceLockedInfo : public SFxSurfaceLockedInfo
{
	IDirect3DTexture9* pTexture;	// the pointer of D3D texture for storing this surface
	IDirect3DSurface9* pSurface;	// the pointer of level 0 D3D surface for storing this surface
};

/*************************************************
 * 
 * IFxSurface Interface
 *
 *************************************************/

// {2E1BBA78-7757-49D5-81FF-58264FB545D2}
DEFINE_GUID(IID_IFxSurfaceEventListener, 
			0x2e1bba78, 0x7757, 0x49d5, 0x81, 0xff, 0x58, 0x26, 0x4f, 0xb5, 0x45, 0xd2);

class IFxSurfaceEventListener : public IUnknown
{
public:
	virtual void __stdcall OnReadCompletion(IFxSurface* pSurface) = 0;

	virtual void __stdcall OnWriteCompletion(IFxSurface* pSurface) = 0;
};

// {BDC09BD8-621E-4378-B94C-27742AD6FBD4}
DEFINE_GUID(IID_IFxSurface, 
			0xbdc09bd8, 0x621e, 0x4378, 0xb9, 0x4c, 0x27, 0x74, 0x2a, 0xd6, 0xfb, 0xd4);

/**
 * @summary	fx surface interface
 */
class IFxSurface : public IUnknown
{
public:
	virtual HRESULT __stdcall GetEngine(IFxEngine** ppEngine) = 0;
	/**
	 * @summary	get the description of this surface
	 * @param	pDes - the structure for holding the returned description.
	 * @return	HRESULT code
	 */
	virtual HRESULT __stdcall GetDescription(SFxSurfaceDescription *pDes) = 0;
	/**
	 * @summary	wait for read completion of this surface
	 * @param	dwWait - how long this wait operation will last.
	 * @return	HRESULT code
	 */
	virtual DWORD __stdcall WaitForReadCompletion(DWORD dwWait = INFINITE) = 0;
	/**
	 * @summary	wait for write completion of this surface
	 * @param	dwWait - how long this wait operation will last.
	 * @return	HRESULT code
	 */
	virtual DWORD __stdcall WaitForWriteCompletion(DWORD dwWait = INFINITE) = 0;
	/**
	 * @summary	increase read count of this surface
	 */
	virtual void __stdcall IncrementReadCount() = 0;
	/**
	 * @summary	increase write count of this surface
	 */
	virtual void __stdcall IncrementWriteCount() = 0;
	/**
	 * @summary	get read count of this surface
	 */
	virtual LONG __stdcall GetReadCount() = 0;
	/**
	 * @summary	get write count of this surface
	 */
	virtual LONG __stdcall GetWriteCount() = 0;
	/**
	 * @summary	signal read completion of this surface. the read count will descrease by 1.
	 */
	virtual void __stdcall SignalReadCompletion() = 0;
	/**
	 * @summary	signal write completion of this surface. the write count will descrease by 1.
	 */
	virtual void __stdcall SignalWriteCompletion() = 0;
	/**
	 * @summary	get the read completion event. user can use this event 
	 *			to wait for read completion of this surface.
	 * @return	the wait handle
	 */
	virtual HANDLE __stdcall GetReadCompletionEvent() = 0;
	/**
	 * @summary	get the write completion event. user can use this event 
	 *			to wait for write completion of this surface.
	 * @return	the wait handle
	 */
	virtual HANDLE __stdcall GetWriteCompletionEvent() = 0;
	/**
	 * @summary	lock this surface for accessing it's content
	 * @param	pLockInfo - the structure for holding the returned inforation.
	 * @return	HRESULT code
	 */
	virtual HRESULT __stdcall Lock(SFxSurfaceLockedInfo *pLockInfo) = 0;
	/**
	 * @summary	unlock this surface. it's not allowed to access this surface anymore.
	 */
	virtual void __stdcall Unlock() = 0;
};

// {615F0098-7730-42EF-A5C7-79BF397CB8EE}
DEFINE_GUID(IID_IFxSurface2, 
			0x615f0098, 0x7730, 0x42ef, 0xa5, 0xc7, 0x79, 0xbf, 0x39, 0x7c, 0xb8, 0xee);

class IFxSurface2 : public IFxSurface
{
public:
	virtual HRESULT __stdcall AttachCustomObject(const GUID& guidObject, IUnknown* pObject) = 0;

	virtual HRESULT __stdcall DetachCustomObject(const GUID& guidObject) = 0;

	virtual HRESULT __stdcall GetCustomObject(const GUID& guidObject, IUnknown** ppObject) = 0;

	virtual HRESULT __stdcall SetEventListener(IFxSurfaceEventListener* pEventListener) = 0;

	virtual HRESULT __stdcall GetEventListener(IFxSurfaceEventListener** ppEventListener) = 0;

	virtual HRESULT __stdcall RewriteDescription(const SFxSurfaceDescription& stDesc) = 0;

	virtual HRESULT __stdcall GetSubRegion(const RECT& rcRegion, IFxSurface** ppSubSurface) = 0;
};

/*************************************************
 * 
 * IFxSurfaceInnerPool Interface
 *
 *************************************************/

// {E9F69F52-E644-4b66-AD05-ABE21118B90B}
DEFINE_GUID(IID_IFxSurfaceInnerPool, 
			0xe9f69f52, 0xe644, 0x4b66, 0xad, 0x5, 0xab, 0xe2, 0x11, 0x18, 0xb9, 0xb);

class IFxSurfaceInnerPool : public IUnknown
{
public:
	virtual HRESULT __stdcall GetHostSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		void*				pData = NULL, 
		ULONG				ulPitch = 0) = 0;

	virtual HRESULT __stdcall GetD3DTexSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		int					nMipmapCount, 
		IFxSurface**		ppOutSurface, 
		IDirect3DTexture9*	pTexture = NULL) = 0;

	virtual HRESULT __stdcall GetD3DDynTexSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		IDirect3DTexture9*	pTexture = NULL) = 0;

	virtual HRESULT __stdcall GetD3DHostTexSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		BOOL				bExactSize = FALSE, 
		IDirect3DTexture9*	pTexture = NULL) = 0;
};

/*************************************************
 * 
 * IFxSurfacePool Interface
 *
 *************************************************/

// {BF1D2A7D-D54A-4246-BD05-2AB3AC34F002}
DEFINE_GUID(IID_IFxSurfacePool, 
			0xbf1d2a7d, 0xd54a, 0x4246, 0xbd, 0x5, 0x2a, 0xb3, 0xac, 0x34, 0xf0, 0x2);

typedef void (*FX_FREE_MEMORY)(void* pData, void* pContext);

class IFxSurfacePool : public IUnknown
{
public:
#ifdef MATROX_SUPPORT
	virtual HRESULT __stdcall GetMatroxSurface(
		IMvSurface			*pSurface, 
		IFxSurface**		ppOutSurface, 
		EFxSurfaceFormat*	peFormatOverride = NULL, 
		EFxColorimetry*		peColorimetryOverride = NULL, 
		EFxPolarity*		pePolarityOverride = NULL, 
		RECT*				prcSurfaceOverride = NULL, 
		BOOL*				pbPremultipliedOverride = NULL) = 0;
#endif

	virtual HRESULT __stdcall GetHostSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		void*				pData = NULL, 
		ULONG				ulPitch = 0) = 0;

	virtual HRESULT __stdcall GetD3DTexSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		int					nMipmapCount, 
		IFxSurface**		ppOutSurface, 
		IDirect3DTexture9*	pTexture = NULL) = 0;

	virtual HRESULT __stdcall GetD3DDynTexSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		IDirect3DTexture9*	pTexture = NULL) = 0;

	virtual HRESULT __stdcall GetD3DHostTexSurface(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		BOOL				bExactSize = FALSE, 
		IDirect3DTexture9*	pTexture = NULL) = 0;

	virtual HRESULT __stdcall GetNullSurface(IFxSurface** ppOutSurface) = 0;

	virtual HRESULT __stdcall SetInnerPool(IFxSurfaceInnerPool* pPool) = 0;

	virtual HRESULT __stdcall GetInnerPool(IFxSurfaceInnerPool** ppPool) = 0;

	virtual HRESULT __stdcall GetHostSurfaceEx(
		EFxSurfaceFormat	eFormat, 
		EFxColorimetry		eColorimetry, 
		EFxPolarity			ePolarity, 
		RECT				rcSurface, 
		BOOL				bPremultiplied, 
		IFxSurface**		ppOutSurface, 
		void*				pData, 
		ULONG				ulPitch, 
		FX_FREE_MEMORY		pFreeCallback, 
		void*				pFreeContext) = 0;
};

#endif