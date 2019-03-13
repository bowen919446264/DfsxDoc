///////////////////////////////////////////////////////////
//  IProject.h
//  Implementation of the Interface IProject
//  Created on:      13-06-2018 10:24:03
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_C6F89AEA_155D_4120_A926_C94EBCB1491D__INCLUDED_)
#define EA_C6F89AEA_155D_4120_A926_C94EBCB1491D__INCLUDED_

#include "avpub/Define.h"
#include "IInputStream.h"
#include "IOutputStream.h"
#include "xutil/id.h"
#include "xengine/ITrack.h"
#include "xengine/IMedia.h"
#include "xengine/IClip.h"

namespace xedit {
	/**
	 * 工程设置
	 */
	struct ProjectSetting {
		// 采样格式
		libav::ESampleFormat eSampleFormat;

		// 采样大小
		int nBitsPerSample;

		// 采样率
		int nSampleRate;

		// 声道数
		int nChannelCount;

		/** 声道布局
		 * 1ch : front center (mono)
		 * 2ch : L + R (stereo)
		 * 3ch : front center + L + R
		 * 4ch : front center + L + R + back center
		 * 5ch : front center + L + R + back stereo
		 * 6ch : front center + L + R + back stereo + LFE (5.1)
		 * 7ch : front center + L + R + outer front left + outer front right + back stereo + LFE (7.1)
		 */
		int nChannelLayout;

		// 像素宽度
		int nWidth;

		// 像素高度
		int nHeight;

		// 显示高宽比
		libav::Rational rAspectRatio;

		// 是否隔行扫描
		bool  bInterlaced;

		// 是否顶场优先
		bool  bTopFieldFirst;

		// 像素格式（手机硬件可能支持多种格式）
		libav::EPixFormat ePixFormat;

		// 帧率，单位：fps
		libav::Rational rFrameRate;
	};

	/**
	 * 工程接口
	 */
	class IProject {
	public:
		virtual ~IProject() {}

		/**
		 * 获得工程版本号
		 * @return
		 */
		virtual int getVersion() const =0;

		/**
		 * 获得工程id
		 * @return
		 */
		virtual ID getId() const =0;

		/**
		 * 获得工程名称
		 * @return
		 */
		virtual const char* getName() const =0;

		/**
		 * 获得工程设置
		 * @return
		 */
		virtual const ProjectSetting& getSetting() const =0;
	};
}
#endif // !defined(EA_C6F89AEA_155D_4120_A926_C94EBCB1491D__INCLUDED_)
