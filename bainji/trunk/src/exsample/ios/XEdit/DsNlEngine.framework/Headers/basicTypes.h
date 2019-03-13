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
		eClipTypeFile 		    ,  // 文件素材
		eClipTypeTrack		    ,  // 单轨
		eClipTypeTimeline       , // 时间线
		eClipTypeTransition		, // 过度特技(两个素材之间过渡)
		eClipTypeEffect     	,	  // 简单特技(单素材特技)
		eClipTypeLast
	};


	enum EFileType
	{
		eFileTypeInvalid ,
		eFileTypeVideo   ,     // 视频文件
		eFileTypeAudio   ,     // 音频文件
		eFileTypeCg			   // 字幕文件
	};

	// 轨道类型
	enum  EDsTrackType
	{
		eDsTrackerTypeInvalid ,
		eDsTrackerTypeVideo   ,  // 视频轨道
		eDsTrackerTypeAudio   ,  // 音频轨道
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

	// 同期声
	enum EDsActualSound
	{
		eDsVANone,     // 没有同期声
		eDsVALink,     // 有同期声，不在一个文件
		eDsVAInterlace // 有同期声，在同一个文件
	};

	struct SClipInfo
	{
		EClipType			 m_eClipType;     // 素材类型
		uint64_t     m_framePosition; // 在轨道上的起始位置
		uint64_t     m_frameDuration; // 在轨道上的长度
		uint64_t	 m_frameTrimIn;   // 在素材上的起始位置
		uint64_t	 m_frameTrimOut;  // 在素材上的结束位置
		uint64_t	 m_frameFxDur;    // 特效长度
		int					 m_iTrackIndex;   // 所在轨道索引 ：未用
	};

	struct SAudioChannelInfo
	{
		int					m_iAudioStreamIndex;
		int					m_iChannelIndex;
		TCHAR             m_wszFileName[MAX_PATH];
	};

	struct SFileClipInfo
	{
		BOOL			  m_bInvalidFile;			// 坏素材
		EFileType		  m_eFileType;              // 文件类型
		TCHAR			  m_wszFilename[MAX_PATH];  // 文件路径
		uint64_t  m_frameTotal;             // 文件的总长度
		int				  m_iFileSubtype;           // 文件分类格式
		int				  m_iStreamIndex;           // 用于包含多个音频流的文件
		EDsActualSound    m_eActualSound;           // 是否有同期声 
		int				  m_iAudioFileCount;        // 同期声文件个数
		int				  m_iAudioChannelsCount;    // 音频轨道个数
		SAudioChannelInfo m_AudioChannels[MAX_ACTUAL_SOUND_COUNT];
		int				  m_iAudioPlayListCount;    // 音频文件需要的playlist 个数
		TCHAR		      m_wszVideoIndexFile[MAX_PATH];
		int				  m_nFileAFDType;			// 视频文件打开方式
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
		unsigned long    m_ulDataRate; // 编码码率
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
		uint32_t     m_size;                   // 结构大小
		TCHAR        m_wszFileName[MAX_PATH]; // 文件全路径
		uint64_t     m_ui64Duration;           // 文件总长度
		uint64_t     m_ui64FileSize;           // 文件大小
		EFileType    m_eFileType;			   // 文件类型
		FILETIME     m_lastModifyTime;         // 文件最后修改时间
		int			 m_iFileSubType;		   // 文件分类格式
		int			 m_iStreamIndex;           // 用于包含多个音频流的文件			

		// 视频信息
		SDsVideoFileInfo m_sVideoInfo;
		EDsActualSound   m_eActualSound; // 是否有同期声

		// 音频信息
		SDsAudioFileInfo m_sAudioInfoList[MAX_ACTUAL_SOUND_COUNT];
		int				 m_AudioFileCount;

		TCHAR	    m_wszVideoIndexFile[MAX_PATH]; // 视频索引文件
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
		float   m_framePosition; //  以点素材长度的百分比表示
		float	m_fAlpha; // 0 - 1 的取值范围

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
		int m_iBitrate;       // 码率（M）
		int m_iMaxBitrate;    // 最大码率(M)
		int m_iMinBitrate;	  // 最小码率(M)
		int m_iWidth;		  // 视频宽度
		int m_iHeight;        // 视频高度
		int m_iFrameRate;     // 帧率
	};

	struct SDsAudioConfigInfo
	{
		int m_iChannelCount;    // 声道个数
		int m_iSampleDepth;     // 采样位数
		int m_iSampleRate;      // 采样频率
		int m_iBitrate;
	};

	
	// 生成格式信息
	struct SDsCompilerFormatInfo
	{
		DWORD m_dwSize;						// 结构大小
		bool  m_bHDFormat;					// 是否为高清
		uint32_t m_uiFormatID;				// 格式ID
		TCHAR m_wszFormatName[50];			// 格式名称
		bool m_bEs;							// 音视频是否分离

		int   m_iVideoEncapsulate;			// 视频封装
											// 1:MPGES 2:MPGPS 3:MPGTS 4:AVI 5:MXF_blue 6:MXF_P2 
											// 7:MOV 8:MKV 9:REAL 10:FLV 11:WMVA 12:gxf 13:mp4 14:f4v
		CLSID m_clsidVideo;					//视频频编码格式GUID 
		bool m_bVideoConfig;				// 视频信息是否能修改
		TCHAR m_wszVideoExt[50];			// 视频后缀名
		SDsVideoConfigInfo m_sVideoInfo;	// 视频格式描述信息
		
		int	  m_iAudioEncapsulate;			// 音频封装 1:MXF   2:WAVE   3:MPEGAUDIO 4:REAL 5:WMA
		CLSID m_clsidAudio;					//音频编码格式GUID 
		bool m_bAudioConfig;				// 音频信息是否能修改
		TCHAR m_wszAudioExt[50];			// 视频后缀名
		SDsAudioConfigInfo m_sAudioInfo;	// 音频格式描述信息
		int m_nOnlyGenerateDataIndex ;      // 只产生AV裸数据及其索引【不进行AV数据包封装】标志，用于非编后台集群生成素材并合并 
		int m_nProgressive;				    // 逐行
	};

	//typedef std::vector<SDsCompilerFormatInfo> CompilerFormatInfoList;

	// 生成文件结构信息
	struct SDsExportFileInfo
	{
		uint64_t m_frameStart;					// 生成开始位置
		uint64_t m_frameEnd;					// 生成结束位置
		bool	 m_bExportVideo;				// 是否生成视频
		TCHAR    m_wszVideoFile[MAX_PATH];		// 视频文件名
		bool	 m_bExportAudio;				// 是否生成音频
		TCHAR    m_wszAudioFile[MAX_PATH];		// 音频文件名
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
		TCHAR				  *m_pwszFormatInfo;        // 采集生成的文件格式
		SDsExportFileInfo     *m_arCompileFileList;   // 采集生成文件的列表
		uint32_t			  m_uiFileCount;		  // 文件个数

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


	// 采集相关结构
	struct SDsCaptureSignalItem
	{
		int		 m_iValue;
		TCHAR  m_wszName[50];    // 采集信号名称
		BOOL     m_bSignalPresent; // 信号是否可用
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