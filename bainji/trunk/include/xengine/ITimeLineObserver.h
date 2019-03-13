///////////////////////////////////////////////////////////
//  ITimeLineObserver.h
//  Implementation of the Interface ITimeLineObserver
//  Created on:      22-06-2018 16:58:43
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_56EA7F2A_52FB_4147_8493_048539AF7BED__INCLUDED_)
#define EA_56EA7F2A_52FB_4147_8493_048539AF7BED__INCLUDED_

#include "avpub/Rational.h"
#include "xutil/id.h"
#include "ITrack.h"
using namespace libav;

namespace xedit {

    /**
     * 时间线状态
     */
    enum ETimeLineStatus {
        ETimeLineStatus_None,
        ETimeLineStatus_Playing,	// 播放中
        ETimeLineStatus_Pause,      // 暂停
        ETimeLineStatus_PlayEnd,	// 播放结束
        ETimeLineStatus_DropFrame,	// 播放掉帧
        ETimeLineStatus_PlayFailed, // 播放失败
        ETimeLineStatus_Error,      // 错误

        ETimeLineStatus_Generating,     // 正在生成
        ETimeLineStatus_GenerateFinish, // 生成完成
        ETimeLineStatus_GenerateFailed, // 生成失败
    };


    /**
     * 时间线观察者接口
     */
	class ITimeLineObserver {
	public:
        virtual ~ITimeLineObserver(){}

	    /**
	     * 位置改变回调处理
	     * @param rNewPos 新位置
	     */
		virtual void onPosDidChanged(Rational rNewPos) =0;

		/**
		 * 新建轨道事件
		 * @param pTrack
		 */
		virtual void onTrackCreated(ITrack *pTrack) =0;

		/**
		 * 轨道移除事件
		 * @param trackId
		 */
		virtual void onTrackRemoved(ID trackId) =0;

		/**
		 * 时间线状态改变事件
		 * @param newStatus
		 */
		virtual void onTimeLineStatusChanged(ETimeLineStatus newStatus) =0;
	};
}

#endif // !defined(EA_56EA7F2A_52FB_4147_8493_048539AF7BED__INCLUDED_)
