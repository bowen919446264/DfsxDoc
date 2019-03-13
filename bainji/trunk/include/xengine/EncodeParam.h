///////////////////////////////////////////////////////////
//  EncodeParam.h
//  Implementation of the Class EncodeParam
//  Created on:      22-06-2018 16:58:42
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_3B7C4F25_6250_4caa_9E9C_A63B44CAE5EF__INCLUDED_)
#define EA_3B7C4F25_6250_4caa_9E9C_A63B44CAE5EF__INCLUDED_

#include "avpub/MediaInfo.h"
using namespace libav;

namespace xedit {
	/**
	 * codec 参数
	 */
	struct CodecParam {
	    // 编码ID
		ECodecID 		eCodecID;

		// 编码框架
        ECodecProfile 	eCodecProfile;

		// 编码级别
		ECodecLevel 	eCodecLevel;

		// 比特率模式
        EBitrateMode    eBitrateMode;

        // 比特率
		int             nBitrate;

        // 保留字
        uint8_t         reserved[64];
	};

	/**
	 * 视频codec 参数
	 */
	struct VideoCodecParam: public CodecParam {
	    // 像素格式
        EPixFormat  ePixFormat;

        // 像素宽度
        int         nWidth;

        // 像素高度
        int         nHeight;

        // 是否场交错
        bool        bInterlaced;

        // 是否顶场优先
        bool        bTopFieldFirst;

        // 保留字
        uint8_t     reserved[64];
	};

    /**
     * 音频codec 参数
     */
    struct AudioCodecParam: public CodecParam {
        // 声音采样格式
        ESampleFormat   eSampleFmt;

        // 采样率
        int             nSampleRate;

        // 采样大小
        int             nBitsPerSample;

        /**
         * 声道数
         * 1ch : front center (mono)
         * 2ch : L + R (stereo)
         * 3ch : front center + L + R
         * 4ch : front center + L + R + back center
         * 5ch : front center + L + R + back stereo
         * 6ch : front center + L + R + back stereo + LFE (5.1)
         * 7ch : front center + L + R + outer front left + outer front right + back stereo + LFE (7.1)
         */
        int             nChannels;

        // 保留字
        uint8_t         reserved[64];
    };

	/**
	 * 编码参数
	 */
	struct EncodeParam {
		// 封装类型
		EMuxerType      eMuxerType;

		// 视频codec参数
        VideoCodecParam videoParam;

		// 音频codec参数
        AudioCodecParam audioParam;

		// 保留字
		uint8_t         reserved[512];
	};
}

#endif // !defined(EA_3B7C4F25_6250_4caa_9E9C_A63B44CAE5EF__INCLUDED_)
