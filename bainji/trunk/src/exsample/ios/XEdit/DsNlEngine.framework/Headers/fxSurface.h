#ifndef __FXSURFACE_H__
#define __FXSURFACE_H__

#pragma once
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
	
	EFxMemoryLocation_GPU,				// (inner usage)surface is located in GPU memory
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

/*************************************************
 * 
 * IFxSurface Interface
 *
 *************************************************/

// {BDC09BD8-621E-4378-B94C-27742AD6FBD4}
DEFINE_GUID(IID_IFxSurface, 
			0xbdc09bd8, 0x621e, 0x4378, 0xb9, 0x4c, 0x27, 0x74, 0x2a, 0xd6, 0xfb, 0xd4);

/**
 * @summary	fx surface interface
 */
class IFxEngine;
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



#endif //__FXSURFACE_H__