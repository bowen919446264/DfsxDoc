#ifndef __FXEFFECTSETTINGS_H__
#define __FXEFFECTSETTINGS_H__
#pragma once
#include "fxSurface.h"
/*************************************************
 * 
 * IFxEffectSettings Interface
 *
 *************************************************/

// {50CE9946-394E-4aef-9B4A-5ECD123C7D45}
DEFINE_GUID(IID_IFxEffectSettings, 
			0x50ce9946, 0x394e, 0x4aef, 0x9b, 0x4a, 0x5e, 0xcd, 0x12, 0x3c, 0x7d, 0x45);

extern "C" typedef float (*funcFxInterpolate)(float fStart, float fStop, float fDuration, float fOffset);
/**
 * @summary	the interface the class providing fx effect parameter should implement
 */
class IFxEffectSettings : public IUnknown
{
public:
	/**
	 * @summary	the guid of the fx effect this parameter is used for
	 * @param	guid - the guid of the fx effect.
	 * @return	HRESULT code
	 */
	virtual HRESULT __stdcall GetEffectGuid(GUID& guid) = 0;

	virtual HRESULT __stdcall PrepareSettings() = 0;

	virtual HRESULT __stdcall GetResourceURI(EFxPolarity eTargetPolarity, LPWSTR szURI, UINT& uiLen) = 0;

	virtual HRESULT __stdcall LoadFromBuffer(const void* pBuffer, UINT uiSize, UINT *puiRead) = 0;

	virtual HRESULT __stdcall SaveToBuffer(void* pBuffer, UINT uiSize, UINT *puiWritten) = 0;

	virtual HRESULT __stdcall Interpolate(	IFxEffectSettings *pStart, 
											IFxEffectSettings *pStop, 
											float fDuration, 
											float fOffset, 
											funcFxInterpolate func) = 0;


	virtual HRESULT __stdcall Copy(IFxEffectSettings* pSettings) = 0;

};

/*************************************************
 * 
 * IFxEffectSettingsPool Interface
 *
 *************************************************/

// {7218EBBE-F74E-46a1-B273-9FD3E7F0C980}
DEFINE_GUID(IID_IFxEffectSettingsPool, 
			0x7218ebbe, 0xf74e, 0x46a1, 0xb2, 0x73, 0x9f, 0xd3, 0xe7, 0xf0, 0xc9, 0x80);

/**
 * @summary	the interface the class providing fx effect parameter builder should implement
 */
class IFxEffectSettingsPool : public IUnknown
{
public:
	/**
	 * @summary	create a new fx effect parameter.
	 * @param	ppParameter - the created parameter's pointer will stored in this parameter
	 * @return	HRESULT code
	 */
	virtual HRESULT __stdcall GetEffectSettings(IFxEffectSettings** ppSettings) = 0;
};

#endif