#pragma once

// for iOS build
#include <vector>


namespace PlayEngine
{

	#define  MAX_ACTUAL_SOUND_COUNT 16
#ifndef MAX_PATH
#define MAX_PATH 256
#endif // !MAX_PATH
#ifndef TCHAR
#define TCHAR char
#endif

	typedef struct _FILETIME {
		time_t dwLowDateTime;
		time_t dwHighDateTime;
	} FILETIME, *PFILETIME, *LPFILETIME;

	enum EClipType
	{
		eClipTypeInvalid		,
		eClipTypeFile 		    ,  // �ļ��ز�
		eClipTypeTrack		    ,  // ����
		eClipTypeTimeline       , // ʱ����
		eClipTypeTransition		, // �����ؼ�(�����ز�֮�����)
		eClipTypeEffect     	,	  // ���ؼ�(���ز��ؼ�)
		eClipTypeLast
	};


	enum EFileType
	{
		eFileTypeInvalid ,
		eFileTypeVideo   ,     // ��Ƶ�ļ�
		eFileTypeAudio   ,     // ��Ƶ�ļ�
		eFileTypeCg			   // ��Ļ�ļ�
	};

	// �������
	enum  EDsTrackType
	{
		eDsTrackerTypeInvalid ,
		eDsTrackerTypeVideo   ,  // ��Ƶ���
		eDsTrackerTypeAudio   ,  // ��Ƶ���
		eDsTrackerTypeCG,
	};

	enum EDsTrackPlayListIndex
	{
		eDsTrackPlayListA,
		eDsTrackPlayListB
	};

	enum EDsAudioVolumeOutputType
	{
		eDsAudioVolumeLeftToLeft   = 1,
		eDsAudioVolumeLeftToRight  = 2,
		eDsAudioVolumeRightToLeft  = 4,
		eDsAudioVolumeRightToRight = 8,
		eDsAudioVolumeLeftToStereo = 3,
		eDsAudioVolumeRightToStereo = 12
	};

	// ͬ����
	enum EDsActualSound
	{
		eDsVANone,     // û��ͬ����
		eDsVALink,     // ��ͬ����������һ���ļ�
		eDsVAInterlace // ��ͬ��������ͬһ���ļ�
	};

	struct SClipInfo
	{
		EClipType			 m_eClipType;     // �ز�����
		uint64_t     m_framePosition; // �ڹ���ϵ���ʼλ��
		uint64_t     m_frameDuration; // �ڹ���ϵĳ���
		uint64_t	 m_frameTrimIn;   // ���ز��ϵ���ʼλ��
		uint64_t	 m_frameTrimOut;  // ���ز��ϵĽ���λ��
		uint64_t	 m_frameFxDur;    // ��Ч����
		int					 m_iTrackIndex;   // ���ڹ������ ��δ��
	};

	struct SAudioChannelInfo
	{
		int					m_iAudioStreamIndex;
		int					m_iChannelIndex;
		TCHAR             m_wszFileName[MAX_PATH];
	};

	struct SFileClipInfo
	{
		BOOL			  m_bInvalidFile;			// ���ز�
		EFileType		  m_eFileType;              // �ļ�����
		TCHAR			  m_wszFilename[MAX_PATH];  // �ļ�·��
		uint64_t  m_frameTotal;             // �ļ����ܳ���
		int				  m_iFileSubtype;           // �ļ������ʽ
		int				  m_iStreamIndex;           // ���ڰ��������Ƶ�����ļ�
		EDsActualSound    m_eActualSound;           // �Ƿ���ͬ���� 
		int				  m_iAudioFileCount;        // ͬ�����ļ�����
		int				  m_iAudioChannelsCount;    // ��Ƶ�������
		SAudioChannelInfo m_AudioChannels[MAX_ACTUAL_SOUND_COUNT];
		int				  m_iAudioPlayListCount;    // ��Ƶ�ļ���Ҫ��playlist ����
		TCHAR		      m_wszVideoIndexFile[MAX_PATH];
		int				  m_nFileAFDType;			// ��Ƶ�ļ��򿪷�ʽ
		DSNleLib::EDsSurfaceFormat  m_eSurfaceFormat;
	};





	struct SDsVideoFileInfo
	{
		uint32_t		 m_size;
		int				 m_left;
		int				 m_top;
		int				 m_iWidth;
		int				 m_iHeight;

		DSNleLib::EDsSurfaceFormat m_eSurfaceFormat;
		DSNleLib::EDsAspectRatio   m_eAspectRatio;
		DSNleLib::EDsFrameRate     m_eFrameRate;
		DSNleLib::EDsScanMode      m_eScanMode;
		unsigned long    m_ulComponentBitCount;
		unsigned long    m_ulDataRate; // ��������
	};

	struct SDsAudioFileInfo
	{
		uint32_t            m_size;				 // Structure size in bytes. 
		DSNleLib::EDsChannelType	    eChannelType;         // Type of channels supported (mono, stereo, etc.). 
		DSNleLib::EDsAudioDataType    eDataType;            // Type of audio data samples (PCM, float etc.). 
		unsigned long	    ulSamplesPerSec;      // Sample rate, in samples per second (hertz). 
		unsigned long	    ulBitsPerSample;      // Bits per sample for the data type. This is the container 
		// size of the data if the actual data size is different from
		// the container size. This field must be equal to or larger than 
		// the ulValidBitsPerSample. 
		unsigned long	    ulValidBitsPerSample; // Valid bits per sample for the data type. This is the 
		// actual size of the data inside the container. This 
		// field must be equal to or smaller than the 
		// ulBitsPerSample. 
		TCHAR               m_wszFileName[MAX_PATH];
	};


	struct SDsFileInfo
	{
		uint32_t     m_size;                   // �ṹ��С
		TCHAR        m_wszFileName[MAX_PATH]; // �ļ�ȫ·��
		uint64_t     m_ui64Duration;           // �ļ��ܳ���
		uint64_t     m_ui64FileSize;           // �ļ���С
		EFileType    m_eFileType;			   // �ļ�����
		FILETIME     m_lastModifyTime;         // �ļ�����޸�ʱ��
		int			 m_iFileSubType;		   // �ļ������ʽ
		int			 m_iStreamIndex;           // ���ڰ��������Ƶ�����ļ�			

		// ��Ƶ��Ϣ
		SDsVideoFileInfo m_sVideoInfo;
		EDsActualSound   m_eActualSound; // �Ƿ���ͬ����

		// ��Ƶ��Ϣ
		SDsAudioFileInfo m_sAudioInfoList[MAX_ACTUAL_SOUND_COUNT];
		int				 m_AudioFileCount;

		TCHAR	    m_wszVideoIndexFile[MAX_PATH]; // ��Ƶ�����ļ�
	};

	enum EFxSettingType
	{
		eFxSettingTypeFXPAGE = 0,
		eFxSettingTypeTRANSFORM, 
		eFxSettingTypeMASK, 
		eFxSettingTypeTRANSFORM_WITHOUT_CROP, 
		eFxSettingTypeSPECIAL_PAGE,
	};


	struct SDsKFCompositorSettings
	{
		float   m_framePosition; //  �Ե��زĳ��ȵİٷֱȱ�ʾ
		float	m_fAlpha; // 0 - 1 ��ȡֵ��Χ

		SDsKFCompositorSettings()
		{
			m_framePosition = 0;
			m_fAlpha = 1.0;
		}
	};

	typedef std::vector<SDsKFCompositorSettings> SDsKFCompositorSettingsList;


	struct SDsKFAudioFxVolumeSettings
	{
		float m_fPercentOfDuration;  
		float m_fVolume;

		SDsKFAudioFxVolumeSettings()
		{
			m_fPercentOfDuration = 0;
			m_fVolume = 1;
		}
	};
	typedef std::vector<SDsKFAudioFxVolumeSettings> SDsKFAudioVolumeSettingsList;

	struct SDsKFAudioOutputSettings
	{
		float m_fPercentOfDuration;
		EDsAudioVolumeOutputType m_eAudioOutputType;
	};
	typedef std::vector<SDsKFAudioOutputSettings> SDsKFAudioOutputList;

	struct SDsKFAudioFxSettings
	{
		SDsKFAudioFxSettings()
		{
			m_dVolumeGrain = 0.0;
		}
		SDsKFAudioVolumeSettingsList m_AudioVolumeList;
		SDsKFAudioOutputList         m_AudioOutputList;
		double						 m_dVolumeGrain;

		void GetKFAudioVolumeFxSetting(float fPercentOfDuration, SDsKFAudioFxVolumeSettings &audioVolumeSettings)
		{
			// get volumne fx settings
			int count = (int)(m_AudioVolumeList.size());
			if (count <= 0)
			{
				return;
			}

			if (fPercentOfDuration <= m_AudioVolumeList[0].m_fPercentOfDuration)
			{
				audioVolumeSettings = m_AudioVolumeList[0];
			}
			else if (fPercentOfDuration >= m_AudioVolumeList[count - 1].m_fPercentOfDuration)
			{
				audioVolumeSettings = m_AudioVolumeList[count - 1];
			}
			else
			{
				SDsKFAudioFxVolumeSettings startFxSettings;
				SDsKFAudioFxVolumeSettings endFxSettings;
				for(int i = 0; i < count; i++)
				{
					endFxSettings = m_AudioVolumeList[i];
					if (endFxSettings.m_fPercentOfDuration >= fPercentOfDuration)
					{
						audioVolumeSettings.m_fPercentOfDuration = fPercentOfDuration;

						float fValue = (endFxSettings.m_fVolume - startFxSettings.m_fVolume) / 
										(endFxSettings.m_fPercentOfDuration - startFxSettings.m_fPercentOfDuration);

						audioVolumeSettings.m_fVolume = startFxSettings.m_fVolume + fValue * (fPercentOfDuration - startFxSettings.m_fPercentOfDuration);
						break;
					}

					startFxSettings = endFxSettings;
				}
			}
		}

		void GetKFAudioOutputFxSetting(float fPercentOfDuration, SDsKFAudioOutputSettings &audioOutputSettings)
		{
			int count = (int)(m_AudioOutputList.size());
			if (count <= 0)
			{
				return;
			}

			if (fPercentOfDuration <= m_AudioOutputList[0].m_fPercentOfDuration)
			{
				audioOutputSettings = m_AudioOutputList[0];
			}
			else if (fPercentOfDuration >= m_AudioOutputList[count - 1].m_fPercentOfDuration)
			{
				audioOutputSettings = m_AudioOutputList[count - 1];
			}
			else
			{
				SDsKFAudioOutputSettings startFxSettings;
				SDsKFAudioOutputSettings endFxSettings;

				for (int i = 0; i < count; i++)
				{
					endFxSettings = m_AudioOutputList[i];

					if (endFxSettings.m_fPercentOfDuration > fPercentOfDuration)
					{
						audioOutputSettings.m_fPercentOfDuration = fPercentOfDuration;
						audioOutputSettings.m_eAudioOutputType = startFxSettings.m_eAudioOutputType;

						break;
					}
					startFxSettings = endFxSettings;
				}
			}
		}

		void InsertAudioVolumeSetting(SDsKFAudioFxVolumeSettings &sAudioVolumeSetting)
		{
			BOOL bOK = FALSE;
			for (SDsKFAudioVolumeSettingsList::iterator itor = m_AudioVolumeList.begin(); itor != m_AudioVolumeList.end(); ++itor)
			{
				SDsKFAudioFxVolumeSettings &tempItem = *itor;
				if (tempItem.m_fPercentOfDuration > sAudioVolumeSetting.m_fPercentOfDuration)
				{
					m_AudioVolumeList.insert(itor, sAudioVolumeSetting);
					bOK = TRUE;
					break;
				}
			}

			if (bOK == FALSE)
			{
				m_AudioVolumeList.push_back(sAudioVolumeSetting);
			}
		}

		void InsertAudioOutputSetting(SDsKFAudioOutputSettings &sAudioOutputSettings)
		{
			BOOL bOK = FALSE;
			for (SDsKFAudioOutputList::iterator itor = m_AudioOutputList.begin(); itor != m_AudioOutputList.end(); ++itor)
			{
				SDsKFAudioOutputSettings &tempItem = *itor;
				if (tempItem.m_fPercentOfDuration > sAudioOutputSettings.m_fPercentOfDuration)
				{
					m_AudioOutputList.insert(itor, sAudioOutputSettings);
					bOK = TRUE;
					break;
				}
			}
			if (bOK == FALSE)
			{
				m_AudioOutputList.push_back(sAudioOutputSettings);
			}
		}
	};

	struct SMuteArea
	{
		uint64_t startOffset;
		uint64_t endOffset;
	};

	typedef std::vector<SMuteArea> MuteAreaList;

	struct SDsVideoConfigInfo
	{
		int m_iBitrate;       // ���ʣ�M��
		int m_iMaxBitrate;    // �������(M)
		int m_iMinBitrate;	  // ��С����(M)
		int m_iWidth;		  // ��Ƶ���
		int m_iHeight;        // ��Ƶ�߶�
		int m_iFrameRate;     // ֡��
	};

	struct SDsAudioConfigInfo
	{
		int m_iChannelCount;    // ��������
		int m_iSampleDepth;     // ����λ��
		int m_iSampleRate;      // ����Ƶ��
		int m_iBitrate;
	};

	
	// ���ɸ�ʽ��Ϣ
	struct SDsCompilerFormatInfo
	{
		DWORD m_dwSize;						// �ṹ��С
		bool  m_bHDFormat;					// �Ƿ�Ϊ����
		uint32_t m_uiFormatID;				// ��ʽID
		TCHAR m_wszFormatName[50];			// ��ʽ����
		bool m_bEs;							// ����Ƶ�Ƿ����

		int   m_iVideoEncapsulate;			// ��Ƶ��װ
											// 1:MPGES 2:MPGPS 3:MPGTS 4:AVI 5:MXF_blue 6:MXF_P2 
											// 7:MOV 8:MKV 9:REAL 10:FLV 11:WMVA 12:gxf 13:mp4 14:f4v
		CLSID m_clsidVideo;					//��ƵƵ�����ʽGUID 
		bool m_bVideoConfig;				// ��Ƶ��Ϣ�Ƿ����޸�
		TCHAR m_wszVideoExt[50];			// ��Ƶ��׺��
		SDsVideoConfigInfo m_sVideoInfo;	// ��Ƶ��ʽ������Ϣ
		
		int	  m_iAudioEncapsulate;			// ��Ƶ��װ 1:MXF   2:WAVE   3:MPEGAUDIO 4:REAL 5:WMA
		CLSID m_clsidAudio;					//��Ƶ�����ʽGUID 
		bool m_bAudioConfig;				// ��Ƶ��Ϣ�Ƿ����޸�
		TCHAR m_wszAudioExt[50];			// ��Ƶ��׺��
		SDsAudioConfigInfo m_sAudioInfo;	// ��Ƶ��ʽ������Ϣ
		int m_nOnlyGenerateDataIndex ;      // ֻ����AV�����ݼ���������������AV���ݰ���װ����־�����ڷǱ��̨��Ⱥ�����زĲ��ϲ� 
		int m_nProgressive;				    // ����
	};

	//typedef std::vector<SDsCompilerFormatInfo> CompilerFormatInfoList;

	// �����ļ��ṹ��Ϣ
	struct SDsExportFileInfo
	{
		uint64_t m_frameStart;					// ���ɿ�ʼλ��
		uint64_t m_frameEnd;					// ���ɽ���λ��
		bool	 m_bExportVideo;				// �Ƿ�������Ƶ
		TCHAR    m_wszVideoFile[MAX_PATH];		// ��Ƶ�ļ���
		bool	 m_bExportAudio;				// �Ƿ�������Ƶ
		TCHAR    m_wszAudioFile[MAX_PATH];		// ��Ƶ�ļ���
	};
	//typedef std::vector<SDsExportFileInfo> SDsExportFileList;

	struct SDsEncAFD
	{
		int   m_nAfd;
		TCHAR m_wszName[100];
	};
	typedef std::vector<SDsEncAFD> SDsEncAFDList;


	struct SDsCaptureFileDescription
	{
		TCHAR				  *m_pwszFormatInfo;        // �ɼ����ɵ��ļ���ʽ
		SDsExportFileInfo     *m_arCompileFileList;   // �ɼ������ļ����б�
		uint32_t			  m_uiFileCount;		  // �ļ�����

		SDsCaptureFileDescription()
		{
			m_arCompileFileList = NULL;
			m_uiFileCount = 0;
		}
		~SDsCaptureFileDescription()
		{
			if (m_arCompileFileList)
			{
				delete [] m_arCompileFileList;
			}
			m_arCompileFileList = NULL;
			m_uiFileCount = 0;
		}
	};

	// VU Meter
	// Summary:
	//    Contains the calculated VU meter result and Peak meter result for a mono channel of an audio buffer.
	//    Refer to IMvVuMeter interface.
	//
	struct SDsAudioVuMeterValueMono
	{
		uint32_t size;          // Structure size in bytes. Should be set by the application before calling the Vu-Meter.
		float    fVuResult;     // Vu-Meter result. Calculated according to ANSI S1.4 "Specification for Sound Level 
								// Meters". The unit is dBFS (dB Full Scale). The maximum value is 0 dBFS (full scale).
								// Any value below full scale is negative (-3dBFS means 3 dB below full scale).
		float    fPeakResult;   // Peak result. Returns the amplitude of the loudest sample in the buffer. The unit is
								// dBFS (dB Full Scale). The maximum value is 0 dBFS (full scale). Any value below 
								// full scale is negative (-3dBFS means 3 dB below full scale).
		bool     bClipped;      // If any sample in the buffer has reached the full scale then this flag is set to true.  By default it's false.

		SDsAudioVuMeterValueMono()
		{
			size = sizeof(SDsAudioVuMeterValueMono);
			fVuResult = - 60.0;
			fPeakResult = -60.0;
			bClipped = 0;
		}
	};


	// �ɼ���ؽṹ
	struct SDsCaptureSignalItem
	{
		int		 m_iValue;
		TCHAR  m_wszName[50];    // �ɼ��ź�����
		BOOL     m_bSignalPresent; // �ź��Ƿ����
	};

	typedef std::vector<SDsCaptureSignalItem> SCaptureSignalList;

	enum EDsCaptureSourceItem
	{
		eDsCaptureInvliad	   = -1,
		eDsCaptureVideoSignal  = 0 ,
		eDsCaptureAudioSiganle = 1,
		eDsCaptureGenLock      = 2,
		eDsCpatureDevice	   = 3,
		eDsCaptureLast		   
	};

	enum EDSVideoParameterControl
	{
		eDsVideoControlInvalid = -1,
		eDsVideoControlSet,
		eDsVideoControlGet,
		eDsVideoControlLast
	};

}