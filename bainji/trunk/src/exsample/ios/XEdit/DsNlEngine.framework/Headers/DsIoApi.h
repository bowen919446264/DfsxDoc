#pragma once

#include "DsNleDef.h"
namespace DSNleLib {
#define 	IOPLAY  1 
#define     IOCAPTURE  2
#define DEVICE_HD2SD_AFD_16_9 9999
#define DEVICE_HD2SD_AFD_4_3  9998

	// {09397D2F-D8C7-4A3D-89BC-45E5B57C8F16}
	DEFINE_GUID(IID_IDsRemoteControl, 
		0x9397d2f, 0xd8c7, 0x4a3d, 0x89, 0xbc, 0x45, 0xe5, 0xb5, 0x7c, 0x8f, 0x16);

	interface IDsRemoteControl: public IUnknown
	{
		virtual HRESULT SetDeviceMode(EDsRemoteMode eDsRemoteMode) = 0;
		virtual HRESULT GetDeviceMode(EDsRemoteMode &eDsRemoteMode) = 0;
		virtual HRESULT SetShuttleRate(double dRate) = 0;
		virtual HRESULT GetRemotePort(EDSRemoteControlPort &eDsPort) = 0;
		virtual uint64_t GetTimeCode() = 0;
		virtual HRESULT SeekPosition(uint64_t framePoisition) = 0;

		virtual HRESULT EnableComRemoteControl(bool bComRemoteControl) = 0;
		// strComaddr COM 接口
		// strRecordType  遥控协议
		// 时码类型
		virtual HRESULT SetComInfo(LPCTSTR strComAddr, LPCTSTR strRecordType, LPCTSTR strTimecodeType) = 0;
	};


	// cfpeng 添加使用Matrox卡硬件上下变换接口
	// {19BB90A0-11B2-4A8D-AAD7-3159D85EE336}
	DEFINE_GUID(IID_ICrossConverterControl, 
		0x19bb90a0, 0x11b2, 0x4a8d, 0xaa, 0xd7, 0x31, 0x59, 0xd8, 0x5e, 0xe3, 0x36);
	interface ICrossConverterControl: public IUnknown
	{
		virtual HRESULT SetCrossConverterMode(bool HDtoSD, int nAFD) = 0;
		virtual HRESULT StopCrossConverter() = 0;
		virtual HRESULT DeInterlaceAndCrossConvert(IDsSurface *in_pCurrentSurface[],
												   IDsSurface *in_pNextSurface[],	
												   IDsSurface *io_pSurface[],
									     		   uint32_t   ulSize) = 0;
	};


	// {3F3E9388-BE65-4BC2-B5C0-23A93BB577C2}
	DEFINE_GUID(IID_IDsIoDevice, 
		0x3f3e9388, 0xbe65, 0x4bc2, 0xb5, 0xc0, 0x23, 0xa9, 0x3b, 0xb5, 0x77, 0xc2);

	interface IDsIoDevice : public IUnknown
	{
		virtual HRESULT __stdcall GetDeviceDescritption(SDsDeviceDescription *pDeviceDesc) = 0;
		virtual HRESULT __stdcall SetIoMode(ULONG in_uIoMode) = 0;
		virtual uint64_t __stdcall GetPreroll() = 0;
		virtual HRESULT __stdcall PlaybackVideo(IDsSurface *in_pSurface[],uint32_t ulSize) = 0;
		virtual HRESULT __stdcall PlaybackAudio(IDsAudioSamples *in_pSample[] ,uint32_t ulSize) = 0;

		virtual HRESULT __stdcall CancelAllPlay() = 0;

		//@ capture.
		virtual HRESULT __stdcall Capture(IDsSurface *io_pSurface[],IDsAudioSamples *io_pAudBuffer[],uint32_t ulSize) = 0;
		virtual HRESULT __stdcall CancelAllCapture() = 0;

		//@ for paramssetup.
		virtual HRESULT __stdcall SetVideoInputSignal(EDsVideoSignal ulInput) = 0;
		virtual EDsVideoSignal __stdcall GetVideoInputSignal() = 0;

		virtual HRESULT __stdcall SetVideoOutputSignal(EDsVideoSignal ulInput) = 0;
		virtual EDsVideoSignal __stdcall GetVideoOutputSignal() = 0;

		virtual HRESULT __stdcall GetVideoParamsRange(ULONG in_index,INT_PTR &out_min,INT_PTR &out_max,INT_PTR &out_default) = 0;
		virtual HRESULT __stdcall SetVideoParamsValue(ULONG in_index,INT_PTR in_value) = 0;
		virtual INT_PTR __stdcall GetVideoParamsValue(ULONG in_index) = 0;

		virtual HRESULT __stdcall ReleaseDevice() = 0; // 释放
		virtual BOOL __stdcall SupportInputSignal(EDsVideoSignal ulInput, BOOL &bPresent) = 0; // 硬件设置是否支持指定的信号输入


		virtual HRESULT __stdcall SetCaptureAudioSource(EDsCaptureAudioSource eAudioSource) = 0;

		virtual HRESULT __stdcall GetCaptureAudioSource(EDsCaptureAudioSource &eAudioSource, EDsCaptureAudioSource &eCurrentAudioSource) = 0;


		//
		// Summary:
		//    Retrieves the status ofvarious I/O items.
		// Return value:
		//    - Returns an error if the command fails.
		//    <table>
		//    EDsIOStatus                              Parameter pointer             
		//    ---------------------------------------  ----------------------------  

		//    keIOStatusEmbeddedAudio                  SDsEmbeddedAudioStatus        
		virtual HRESULT __stdcall GetIOStatus(EDsIOStatus in_eIOSatus,  // Specifies the status of the I/O item. 
			void *      out_pParam,   // Points to a structure or enumeration that will be filled
			// with information. See the table to determine 
			// which pointer to pass here.
			unsigned int in_uiSizeOfParam) = 0;// Specifies the size of the structure passed.


		virtual HRESULT __stdcall SupportRemoteControl(IDsRemoteControl **ppRemoteControl) = 0;
		virtual bool __stdcall IsSupportCrossConverter() = 0;
	    virtual HRESULT __stdcall GetCrossConverter(ICrossConverterControl **ppCrossConverterControl) = 0;
	};
	// {1A599C6C-1D50-435A-A716-FAF2290B87C5}
	DEFINE_GUID(IID_IDsInputOutput, 
		0x1a599c6c, 0x1d50, 0x435a, 0xa7, 0x16, 0xfa, 0xf2, 0x29, 0xb, 0x87, 0xc5);

	interface IDsInputOutput :public IUnknown
	{
		virtual HRESULT __stdcall GetDeviceCount(SDsDeviceCount *io_sDevice) = 0;
		virtual HRESULT __stdcall GetDevice(ULONG in_index,IDsIoDevice **out_ppDevice) = 0;
		virtual HRESULT __stdcall GetMixerDevice(ULONG in_index,IDsIoDevice **out_ppDevice) = 0;
		virtual HRESULT __stdcall GetSoftwareDevice(ULONG in_index,IDsIoDevice **out_ppDevice) = 0;
	};

	interface IDsPresent :public IUnknown
	{
		virtual HRESULT __stdcall Present(IDsSurface *) = 0;
	};


};//namespace