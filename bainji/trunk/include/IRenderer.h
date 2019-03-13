///////////////////////////////////////////////////////////
//  IRenderer.h
//  Implementation of the Interface IRenderer
//  Created on:      22-06-2018 16:58:43
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_F5B7E642_923B_4c15_9407_C5F731C4746E__INCLUDED_)
#define EA_F5B7E642_923B_4c15_9407_C5F731C4746E__INCLUDED_

#include "IBuffer.h"
#include "avpub/Define.h"
#include "avpub/ESampleFormat.h"
#include "avpub/EPixFormat.h"

using namespace libav;

namespace xedit {
    /**
     * renderer接口
     */
	class IRenderer {
	public:
        virtual ~IRenderer(){}

	    /**
	     * 渲染
	     * @param buffer 待渲染的buffer
	     * @return
	     */
		virtual StatusCode render(IBuffer *pBuffer) =0;
	};

	/**
     * 视频渲染接口
     */
	class IVideoRenderer : public IRenderer {
	public:
		virtual ~IVideoRenderer(){}

		/**
         * 初始化renderer
         * @param nWidth 目标宽度
         * @param nHeight 目标高度
         * @param ePixFormat 目标像素格式
         * @return 返回0表示成功；否则返回失败代码
         */
		virtual StatusCode init(int nWidth, int nHeight, EPixFormat ePixFormat) =0;
	};

	/**
     * 声音renderer
     */
	class IAudioRenderer : public IRenderer {
	public:
		virtual ~IAudioRenderer(){}

		/**
         * 初始化renderer
         * @param nChannel 声道数
         * @param nSampleRate 采样率
         * @param nBitsPerSample 采样大小
         * @param eSampleFormat 采样格式
         * @return 返回0表示成功；否则返回失败代码
         */
		virtual StatusCode init(int nChannel, int nSampleRate, int nBitsPerSample, ESampleFormat eSampleFormat) =0;
	};
}

#endif // !defined(EA_F5B7E642_923B_4c15_9407_C5F731C4746E__INCLUDED_)
