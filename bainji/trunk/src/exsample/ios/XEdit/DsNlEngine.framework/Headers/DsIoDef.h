#ifndef __DSIODEF_H__
#define __DSIODEF_H__
#pragma once
#include "DsNleDef.h"
namespace DSNleLib {

	// eYUV  ===>  component
	// eCVBS ===>  composite
	// eYC   ===>  S-Video
	// eSDI  ===>  ��������
	// eRGB  ===>
	// eEMB_SDI ===>  ��������SDI
	// eHDMI ===>   HDMI
	enum EDsVideoSignal{eVideoSignalValid = -1, eYUV=0, eCVBS, eYC, eSDI, eRGB, eEMB_SDI, eHDMI};

	enum EDsCaptureAudioSource
	{
		eDsCaptureAudioSourceInvalid   = 0x00000,
		eDsCaptureAudioSourceAnalog    = 0x00001,
		eDsCaptureAudioSourceAesEbu    = 0x00002,
		eDsCaptureAudioSourceEmbedded  = 0x00004,
		eDsCaptureAudioSourceHDMI      = 0x00008,
		eDsCaptureAudioSourceLast      = 0xfffff
	};


	enum EDsIOStatus
	{   
		eDsIOStatusInvalid,                        // Invalid value.
		eDsIOStatusEmbeddedAudio,                  // Specifies that the status refers to the embedded audio
												   // group and pair (see the SMvEmbeddedAudioStatus definition
												   // for details).

		eDsIOStatusAesEbuAudio,                    // Specifies that the status refers to the aes/ebu audio signal
												   // (see the SMvAesEbuAudioStatus definition for details).
		eDsIOStatusAesEbuChannelStatus,            // Specifies that the status refers to the channel status bytes of
												   // the aes/ebu audio signal (see SMvDigitalAudioChannelStatus

		eDsIOStatusHdmiAudio,                      // Specifies that the status refers to the Hdmi audio
												   // (see the SMvHdmiAudioStatus definition for details).
		eDsIOChannelCount,						   // ��ǰ��Ƶ֧�ֵĹ����
		eDsIOStatusLast      
	};

	//
	// Summary:
	//    Determines the genlock source.
	//
	enum EDsGenlockSource
	{
		eDsGenlockSourceInvalid                 = 0x00,  // Invalid value.
		eDsGenlockSourceInternal                = 0x01,  // Indicates that the genlock source comes from 
														// the internal reference.
		eDsGenlockSourceBlackBurst              = 0x02,  // Indicates that the genlock source comes from 
														// the analog blackburst reference.
		eDsGenlockSourceSDIVideo                = 0x04,  // Indicates that the genlock source comes from 
														// the digital SDI reference.
		eDsGenlockSourceBlackBurstPoorQuality   = 0x08,   // Indicates that the genlock source comes from 
														 // the analog blackburst reference, and that 
														// this reference is not of professional 
														// quality. 
		eDsGenlockSourceAnalogVideo             = 0x10,  // Indicates that the genlock source comes from 
														// the analog video input reference.
		eDsGenlockSourceDVIVideo                = 0x20,  // Indicates that the genlock source comes from 
														// the digital DVI video input reference.
		eDsGenlockSourceHDMIVideo               = 0x40,  // Indicates that the genlock source comes from 
														// the digital HDMI video input reference.
		eDsGenlockSourceAll                     = eDsGenlockSourceInvalid  // All defined values.
											  |  eDsGenlockSourceInternal
											  |  eDsGenlockSourceBlackBurst
											  |  eDsGenlockSourceSDIVideo
											  |  eDsGenlockSourceBlackBurstPoorQuality
											  |  eDsGenlockSourceAnalogVideo
											  |  eDsGenlockSourceDVIVideo
									     	  |  eDsGenlockSourceHDMIVideo,

		eDsGenlockSourceLast                    = eDsGenlockSourceAll+1 // End of list indicator.
	};

	struct SDsEmbeddedAudioStatus
	{
		uint32_t                size; // Structure size in bytes.
		bool                    bGroup1Pair1Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup1Pair2Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup2Pair1Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup2Pair2Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup3Pair1Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup3Pair2Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup4Pair1Present; // TRUE if there is an audio signal on the group and pair.
		bool                    bGroup4Pair2Present; // TRUE if there is an audio signal on the group and pair.
	};


	enum EDsRemoteMode
	{
		eDsRemoteMode_Invalid = -1,
		eDsRemoteMode_Play,				// ����
		emgRemoteMode_ReversePlay,      // ����
		eDsRemoteMode_Stop,				// ֹͣ
		eDsRemoteMode_Pause,			// ��ͣ
		eDsRemoteMode_Resume,			// 
		eDsRemoteMode_FastForword,		// �������λ��
		eDsRemoteMode_Rewind,           // ������ʼλ��
		eDsRemoteMode_Record,			// ��¼
		eDsRemoteMode_PauseRecord,		// ��ͣ��¼
		eDsRemoteMode_RecordStrobe,		// ��¼��֡
		eDsRemoteMode_StepForword,		// ǰ��һ֡
		eDsRemoteMode_StepRewind,		// ����һ֡
		eDsRemoteMode_Shuttle,			// Shuttle
		eDsRemoteMode_Eject,		    // ����
		eDsRemoteMode_Last
	};


	enum EDSRemoteControlPort
	{
		eDsRemote_Invalid,
		eDsRemote_1394,
		eDsRemote_ARTI,
		eDsRemote_COM1,
		eDsRemote_COM2,
		eDsRemote_COM3,
		eDsRemote_COM4,
		eDsRemote_DIAQ,
		eDsRemote_SIM,
		eDsRemote_USB,
		eDsRemote_Last
	};

	//
	// Summary:
	//    Returns the number of all types of devices.
	//
	//@ params define.
	//@ input.
#define LIVEWINDOW	0xff
#define LIVERECT	0xFE
#define VIDEOCALLBACK	0xFD
#define BRIGHT		0
#define CONTRAST	1
#define HUE			2
#define SATURATION	3
#define CHROMAGAIN	4
#define SUPPORT_GENLOCK		5
#define GENLOCK_VALUE       6

	//@ output
#define HORIZONALPHASE  10
#define VERTICALPHASE   11 	

#define KEYDELAY   12 	
#define CGDELAY   13
#define SUBPHASE   14
	struct SDsDeviceCount
	{
		uint32_t       size;                                  // Structure size in bytes.
		unsigned int   uiNumberOfInputDevices;         
		unsigned int   uiNumberOfOutputDevices;        
		unsigned int   uiNumberOfMixerDevices;  
	};

	struct SDsDeviceDescription
	{
		uint32_t uiSize;
		wchar_t  wszDeviceName[50];   // �豸����
		bool     bSupportInput;   // ֧�ֲɼ�
		bool     bSupportOutput;  // ֧�����
	};

};// namespace 
#endif //__DSIODEF_H__