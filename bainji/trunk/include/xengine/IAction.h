///////////////////////////////////////////////////////////
//  IAction.h
//  Implementation of the Interface IAction
//  Created on:      22-06-2018 16:58:43
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_9A750771_DA47_4913_A0BE_4ABAD649CBEA__INCLUDED_)
#define EA_9A750771_DA47_4913_A0BE_4ABAD649CBEA__INCLUDED_

#include "avpub/Define.h"
#include "xutil/id.h"
#include "xutil/IDictionary.h"

namespace xedit {

    /**
     * 动作类型枚举
     */
    enum EActionType {
    	AT_Unknown,						// 未知动作
		AT_AddMedia,					// 添加媒体
		AT_RemoveMedia,					// 移除媒体
        AT_AddClip,						// 添加切片
        AT_RemoveClip,					// 删除切片
        AT_MoveClip,					// 移动切片
        AT_ChangeClipOffsetOnTrack,		// 改变切片在轨道上的偏移量
		AT_ChangeClipDuration,			// 改变切片时长
		AT_ChangeClipOffsetInMedia,		// 改变切片在媒体内的偏移量
        AT_GroupAction                  // 组合动作
    };

    /**
     * 动作接口
     */
	class IAction {
	public:
        virtual ~IAction(){}

        /**
         * 获取动作类型
         * @return
         */
        virtual EActionType getActionType() const =0;

        /**
         * 获取动作参数
         * @param key 参数key, AP_*
         * @return
         */
        virtual const char* getActionParam(const char* key) const =0;
	};

	// 添加媒体 - 媒体路径 - string
    #define AP_AddMedia_MediaPath "ap.addMedia.mediaPath"

    // 删除媒体 - 媒体路径 - string
    #define AP_RemoveMedia_MediaId "ap.removeMedia.mediaId"

    // 添加切片 - 轨道id - int64_t
    #define AP_AddClip_TrackId "ap.addClip.trackId"

    // 添加切片 - 媒体id - int64_t
    #define AP_AddClip_MediaId "ap.addClip.mediaId"

    // 添加切片 - 切片在轨道上的偏移量，形如(100, 25) - Rational
    #define AP_AddClip_OffsetOnTrack "ap.addClip.offsetOnTrack"

    // 添加切片 - 切片在媒体内的偏移量，形如(100, 25) - Rational
    #define AP_AddClip_OffsetInMedia "ap.addClip.offsetInMedia"

    // 添加切片 - 切片时长，形如(100, 25) - Rational
    #define AP_AddClip_Duration "ap.addClip.duration"

    // 添加切片 - 添加成功后的切片id - int64_t
    #define AP_AddClip_ClipId "ap.addClip.clipId"

    // 删除切片 - 切片id - int64_t
    #define AP_RemoveClip_ClipId "ap.removeClip.clipId"

    // 移动切片 - 切片id - int64_t
    #define AP_MoveClip_ClipId "ap.moveClip.clipId"

    // 移动切片 - 切片原来在轨道上的偏移量 - Rational
    #define AP_MoveClip_OldOffsetOnTrack "ap.moveClip.oldOffsetOnTrack"

    // 移动切片 - 切片现在在轨道上的偏移量 - Rational
    #define AP_MoveClip_NewOffsetOnTrack "ap.moveClip.newOffsetOnTrack"

    // 改变切片在轨道上的偏移量 - 切片id - int64_t
    #define AP_ChangeClipOffsetOnTrack_ClipId "ap.changeClipOffsetOnTrack.clipId"

    // 改变切片在轨道上的偏移量 - 切片原来偏移量 - Rational
    #define AP_ChangeClipOffsetOnTrack_OldOffsetOnTrack "ap.changeClipOffsetOnTrack.oldOffsetOnTrack"

    // 改变切片在轨道上的偏移量 - 切片现在偏移量 - Rational
    #define AP_ChangeClipOffsetOnTrack_NewOffsetOnTrack "ap.changeClipOffsetOnTrack.newOffsetOnTrack"

    // 改变切片时长 - 切片id - int64_t
    #define AP_ChangeClipDuration_ClipId "ap.changeClipDuration.clipId"

    // 改变切片时长 - 切片原来时长 - Rational
    #define AP_ChangeClipDuration_OldDuration "ap.changeClipDuration.oldDuration"

    // 改变切片时长 - 切片现在时长 - Rational
    #define AP_ChangeClipDuration_NewDuration "ap.changeClipDuration.newDuration"

    // 改变切片在文件中的偏移量 - 切片id
    #define AP_ChangeClipOffsetInMedia_ClipId "ap.changeClipOffsetInMedia.clipId"

    // 改变切片在文件中的偏移量 - 切片原来在文件中的偏移量 - Rational
    #define AP_ChangeClipOffsetInMedia_OldOffsetInMedia "ap.changeClipOffsetInMedia.oldOffsetInMedia"

    // 改变切片在文件中的偏移量 - 切片现在在文件中的偏移量 - Rational
    #define AP_ChangeClipOffsetInMedia_NewOffsetInMedia "ap.changeClipOffsetInMedia.newOffsetInMedia"
}
#endif // !defined(EA_9A750771_DA47_4913_A0BE_4ABAD649CBEA__INCLUDED_)
