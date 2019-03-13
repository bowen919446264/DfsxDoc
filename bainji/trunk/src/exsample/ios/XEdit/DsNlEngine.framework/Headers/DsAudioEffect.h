#pragma once
#include "DsUtil.h"
#include "fxEffectSettings.h"
#include "DsPublicApi.h"
namespace DSNleLib {

//////////////////////////////////////////////////////////////////////////
//
// Summary:
//    This interface sets the volume for the audio mixer.
//
//////////////////////////////////////////////////////////////////////////
// {5778C932-3FC9-4EDF-9DB2-AD47F376A4EE}
DEFINE_GUID(IID_IDsAudioMixerSettings, 
0x5778c932, 0x3fc9, 0x4edf, 0x9d, 0xb2, 0xad, 0x47, 0xf3, 0x76, 0xa4, 0xee);

interface  IDsAudioMixerSettings 
: public IUnknown
{
   //
   // Summary:
   //    This method sets the volume of the left channel source that will be mixed to the left 
   //    channel destination.
   // Description:
   //    For both parameters:
   //    Default value: 1.0 (0dB).
   //    Valid range:
   //       - For 16-bit audio data: [0.0, 7.999] multiplication factor (equivalent to 
   //         [-infinite, 18] dB). A value below 1.0 is attenuation, and a value above 1.0 is gain. 
   //       - For 24 bit audio data: Not restricted, full range 32 bit float. However, the usefull 
   //         range is 0.0 to 8.0. A value below 1.0 is attenuation, and a value above 1.0 is gain.
   //         Negative volumes will invert (180 degree phase inversion), which can cause 
   //         unpredictable results (signal cancellation). 
   //    Granularity:
   //       - For 16-bit audio data: Limited to 16 high order bits of the mantissa (approx. 0.0001
   //         steps in multiplication factor or 0.001dB steps around 0dB). 
   //       - For 24-bit audio data: Full precision 32 bit float (approx. 1 x 10 exp -7 steps in
   //         multiplication factor or 1 x 10 exp -6 dB around 0dB ). 
   //    Dependency:
   //       - The volume will be linearly interpolated (from one sample to the other) from the
   //         in_fInitial to the in_fFinal. 
   // Return value:
   //    - DS_NOERROR, if succeeded. 
   //    - DS_E_INVALID_PARAMETER, if one of the parameter is out of range.  
   virtual HRESULT __stdcall SetVolumeLeftToLeft
      (
      float in_fInitial,   // Volume to be applied to the left channel at the beginning of the
                              // segment (first sample). The volume is a multiplying factor. 
      float in_fFinal      // Volume to be applied to the left channel at the end of the segment 
                              // (last sample).
      ) = 0;

   //
   // Summary:
   //    This method sets the volume of right channel source that will be mixed to the right
   //    channel destination. 
   // Description:
   //    For both parameters:
   //    Default value: 1.0 (0dB)
   //    Valid range:
   //       - For 16-bit audio data: [0.0, 7.999] multiplication factor (equivalent to
   //         [-infinite, 18] dB). A value below 1.0 is attenuation, and a value above 1.0 is gain. 
   //       - For 24-bit audio data: Not restricted, full range 32 bit float. However, the usefull 
   //         range is 0.0 to 8.0. A value below 1.0 is attenuation, and a value above 1.0 is gain.
   //         Negative volumes will invert (180 degree phase inversion), this can cause 
   //         unpredictable results (signal cancellation). 
   //    Granularity:
   //       - For 16-bit audio data: Limited to 16 high order bits of the mantissa (approx. 0.0001
   //         steps in multiplication factor or 0.001dB steps around 0dB). 
   //       - For 24-bit audio data: Full precision 32 bit float (approx. 1 x 10 exp -7 steps in 
   //         multiplication factor or 1 x 10 exp -6 dB around 0dB ). 
   //    Dependency:
   //       - The volume will be linearly interpolated (from one sample to the other) from the 
   //         in_fInitial to the in_fFinal.
   // Return value:
   //    - DS_NOERROR, if succeeded. 
   //    - DS_E_INVALID_PARAMETER, if one of the parameter is out of range. 
   virtual HRESULT __stdcall SetVolumeRightToRight
      (
      float in_fInitial,   // Volume to be applied to the right channel at the beginning of the 
                              // segment (first sample). The volume is a multiplying factor.
      float in_fFinal      // Volume to be applied to the right channel at the end of the segment 
                              // (last sample).
      ) = 0;

   //
   // Summary:
   //    This method sets the volume of left channel source that will be mixed to the right channel
   //    destination. 
   // Description:For both parameters:
   //    Default value: 1.0 (0dB)
   //    Valid range:
   //       - For 16-bit audio data: [0.0, 7.999] multiplication factor (equivalent to 
   //         [-infinite, 18] dB). A value below 1.0 is attenuation, and a value above 1.0 is gain. 
   //       - For 24-bit audio data: Not restricted, full range 32 bit float. However, the usefull
   //         range is 0.0 to 8.0. A value below 1.0 is attenuation, and a value above 1.0 is gain. 
   //         Negative volumes will invert (180 degree phase inversion), which can cause 
   //         unpredictable results (signal cancellation). 
   //    Granularity:
   //       - For 16-bit audio data: Limited to 16 high order bits of the mantissa (approx. 0.0001 
   //         steps in multiplication factor or 0.001dB steps around 0dB). 
   //       - For 24 bit audio data: Full precision 32 bit float (approx. 1 x 10 exp -7 steps in 
   //         multiplication factor or 1 x 10 exp -6 dB around 0dB ). 
   //    Dependency:
   //       - The volume will be linearly interpolated (from one sample to the other) from the
   //         in_fInitial to the in_fFinal.
   // Return value:
   //    - DS_NOERROR, if succeeded. 
   //    - DS_E_INVALID_PARAMETER, if one of the parameter is out of range. 
   virtual HRESULT __stdcall SetVolumeLeftToRight
      (
      float in_fInitial,   // Volume to be applied to the left channel at the beginning of the 
                              // segment (first sample). The volume is a multiplying factor. 
      float in_fFinal      // Volume to be applied to the left channel at the end of the segment
                              // (last sample).
      ) = 0;

   //
   // Summary:
   //    Sets the volume of the right channel source that will be mixed to the left channel
   //    destination. 
   // Description:
   //    For both parameters:
   //    Default value: 1.0 (0dB)
   //    Valid range:
   //       - For 16-bit audio data: [0.0, 7.999] multiplication factor (equivalent to
   //         [-infinite, 18] dB). A value below 1.0 is attenuation, and a value above 1.0 is gain. 
   //       - For 24-bit audio data: Not restricted, full range 32 bit float. However, the usefull
   //         range is 0.0 to 8.0. A value below 1.0 is attenuation, and a value above 1.0 is gain.
   //         Negative volumes will invert (180 degree phase inversion), this can cause 
   //         unpredictable results (signal cancellation). 
   //    Granularity:
   //       - For 16-bit audio data: Limited to 16 high order bits of the mantissa (approx. 0.0001 
   //         steps in multiplication factor or 0.001dB steps around 0dB). 
   //       - For 24-bit audio data: Full precision 32 bit float (approx. 1 x 10 exp -7 steps in 
   //         multiplication factor or 1 x 10 exp -6 dB around 0dB ). 
   //    Dependency:
   //       - The volume will be linearly interpolated (from one sample to the other) from the 
   //         in_fInitial to the in_fFinal.
   // Return value:
   //    - DS_NOERROR, if succeeded. 
   //    - DS_E_INVALID_PARAMETER, if one of the parameters is out of range. 
   virtual HRESULT __stdcall SetVolumeRightToLeft
      (
      float in_fInitial,   // Volume to be applied to the right channel at the beginning of the
                              // segment (first sample). The volume is a multiplying factor. 
      float in_fFinal      // Volume to be applied to the right channel at the end of the segment
                              // (last sample).
      ) = 0;
};


// {5778C932-3FC9-4EDF-9DB2-AD47F376A4EF}
DEFINE_GUID(IID_IDsAudioMixer, 
0x5778c932, 0x3fc9, 0x4edf, 0x9d, 0xb2, 0xad, 0x47, 0xf3, 0x76, 0xa4, 0xef);
interface  IDsAudioMixer : public IUnknown
{
	//
	// Summary:
	//    Called to mix input tracks of audio into 1 output track of audio using CPU with host memory 
	//    at a specific thread pool index.
	// Return value:
	//    - DS_NOERROR, if everything went fine.
	//    - HRESULT error code, if something went wrong.
	// Remarks:
	//    - This method calls the Audio Mixer object running at the corresponding pool index. 
	//    - The Mix method's implementation will perform an IDsAudioSamples::SetLength on the 
	//      destination buffer in_pIOutputBuffer once its asynchronous processing is completed. You 
	//      will need to take special precautions if the source buffer in_apIInputBufferarray and the 
	//      destination buffer in_pIOutputBuffer are not the same length. 
	virtual HRESULT __stdcall Mix
		(
		IDsAudioSamples**   in_apIInputBufferArray,  // Array of input buffers.
		unsigned long       in_ulTrackCount,         // Number of input tracks (number of elements in 
		// the Effect Settings and input buffer array). 
		IDsAudioSamples*    io_pIOutputBuffer,       // Output buffer.
		IFxEffectSettings** in_apIMixerSettings     // Array of Effect Settings that are Mixer Effect.
		)=0;
};

#ifdef WIN32
interface __declspec(uuid("5778C932-3FC9-4EDF-9DB2-AD47F376A4EE")) IDsAudioMixerSettings;
#endif // WIN32
// {5778C932-3FC9-4EDF-9DB2-AD47F376A4f0}
DEFINE_GUID(IID_IDsAudioEffect, 
0x5778c932, 0x3fc9, 0x4edf, 0x9d, 0xb2, 0xad, 0x47, 0xf3, 0x76, 0xa4, 0xf0);
interface  IDsAudioEffect : public IUnknown
{
	virtual HRESULT __stdcall DoEffect
		(
		IDsAudioSamples*    in_pIInputBuffer,  // input buffer.
		IDsAudioSamples*    io_pIOutputBuffer,       // Output buffer.
		IFxEffectSettings* in_pISettings     
		)=0;

};

DEFINE_GUID(IID_IDsAudioPitchSettings, 
0x5778c932, 0x3fc9, 0x4edf, 0x9d, 0xb2, 0xad, 0x47, 0xf3, 0x76, 0xa4, 0xf1);

interface  IDsAudioPitchSettings 
: public IUnknown
{
    /// Sets pitch change in semi-tones compared to the original pitch
    /// (-12 .. +12)
	virtual HRESULT __stdcall SetPitchSemiTones(float newPitch) = 0;
};


// cfpeng add for “Ù∆µªÿ…˘Ãÿºº
// {198C8E45-019C-4B7E-9D05-BF670BCAE270}
DEFINE_GUID(IID_IDsAudioEchoSettings, 
	0x198c8e45, 0x19c, 0x4b7e, 0x9d, 0x5, 0xbf, 0x67, 0xb, 0xca, 0xe2, 0x70);

interface IDsAudioEchoSettings : public IUnknown
{
	// 0 ~ 1000 : 150
	virtual HRESULT __stdcall SetDelay(float fdelay) = 0;
	// 0 ~ 1
	virtual HRESULT __stdcall SetDecay(float fdecay) = 0;
	// 0 ~ 1
	virtual HRESULT __stdcall SetGainIn(float fGainIn) = 0;
	// 0 ~ 1
	virtual HRESULT __stdcall SetGainOut(float fGainOut) = 0;
};

typedef struct 
{
	DWORD	dwSize;
	DWORD   dwFlags;
	GUID    fxid;
#ifdef WIN32
	wchar_t szName[128];
#else
	char szName[128];
#endif // WIN32

}SDsAudioFxProps;

// {6778C932-3FC9-4EDF-9DB2-AD47F376A4f0}
DEFINE_GUID(IID_IDsAudioEffectManager, 
0x6778c932, 0x3fc9, 0x4edf, 0x9d, 0xb2, 0xad, 0x47, 0xf3, 0x76, 0xa4, 0xf0);

typedef BOOL (*EnumEffectsCallback)(SDsAudioFxProps sprops, void* lpParam);

interface IDsAudioEffectManager
	:public IUnknown
{
	virtual HRESULT __stdcall EnumEffects(EnumEffectsCallback cb, void* lpParam) = 0;

	virtual HRESULT __stdcall CreateEffect(GUID fxid,IDsAudioEffect **out_ppfx) = 0;
};

};//namespace.
