//
// Created by wendachuan on 2018/11/30.
//

#ifndef PROJECT_ITRACK_H
#define PROJECT_ITRACK_H

#include "avpub/Rational.h"
#include "xengine/IClip.h"
#include "xutil/id.h"

using namespace libav;

namespace xedit {
    /**
     * 轨道类型
     */
    enum ETrackType {
        ETrackType_Invalid,
        ETrackType_Video,
        ETrackType_Audio
    };

    /**
     * 轨道
     */
    class ITrack {
    public:
        virtual ~ITrack() {}

        /**
         * 获得轨道id
         * @return
         */
        virtual ID getId() const =0;

        /**
         * 获得轨道类型
         * @return
         */
        virtual ETrackType getTrackType() const =0;

        /**
         * 轨道当前是否已禁用
         * @return
         */
        virtual bool isDisabled() const =0;

        /**
         * 获取轨道上切片数量
         * @return
         */
        virtual int getClipCount() const =0;

        /**
         * 获取指定序号的切片
         * @param nIndex
         * @return
         */
        virtual IClip* getClip(int nIndex) const =0;

        /**
         * 获取指定id的切片
         */
        virtual IClip* getClipById(ID clipId) const =0;

        /**
         * 获得指定位置的切片
         * @param rOffsetOnTrack
         * @return
         */
        virtual IClip* getClipByOffset(Rational rOffsetOnTrack) const =0;

        /**
         * 根据时间，查找切片
         * @param rTime 时间
         * @return
         */
        virtual IClip* findClip(Rational rTime) const =0;

        /**
         * 判断在指定偏移量处是否有切片存在
         * @param rOffsetOnTrack
         * @return
         */
        virtual bool hasClipOnOffset(Rational rOffsetOnTrack) const =0;

        /**
         * 添加切片(指定的位置必须没有切片存在)
         * @param mediaId 媒体id
         * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
         * @return
         */
        virtual IClip* addClip(ID mediaId, Rational rOffsetOnTrack) =0;

        /**
         * 添加流切片
         * @param mediaId 媒体id
         * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
         * @param nStreamIndex 视/音频流序号
         * @return
         */
        //virtual IClip* addAVStreamClip(ID mediaId, Rational rOffsetOnTrack, int nStreamIndex) =0;

        /**
         * 拆分切片
         * @param clipId 待拆分的切片id
         * @param rSplitOffsetInClip 拆分位置(在切片中的偏移量)
         * @param ppOutLeftClip 拆分后的左侧切片
         * @param ppOutRightClip 拆分后的右侧切片
         * @return
         */
        virtual StatusCode splitClip(ID clipId, Rational rSplitOffsetInClip, IClip **ppOutLeftClip, IClip **ppOutRightClip) =0;

        /**
         * 移动切片 - 移动后其他切片的位置和长度可能发生改变，需要遍历切片，更新切片UI
         * 如果目标位置处于其它切片中间，则会拆分该切片，然后按下述流程处理
         * 定义: 移动前，切片在轨道上的序号为a; 移动后，切片在轨道上的序号为b; 切片时长为d; 插入的目标位置为p
         * 有下面几种情况：
         * 1）a == b, 即移动后，此切片在轨道上的序号未改变，此时只改变切片在时间线上的偏移量即可
         * 2) b < a, 即移动后，此切片在轨道上的序号变小，首先将切片删除，然后将序号范围[b, a-1]的切片向右移动d，最后将切片插入到p位置
         * 3) a < b, 即移动后，此切片在轨道上的序号变大，首先将切片删除，然后将序号范围[a, b-1]的切片向左移动d, 最后将切片插入到p位置
         * @param clipId 待移动的切片id
         * @param rOffsetOnTrack 目标位置
         * @param pOutSplitClipId 如果切片被拆分，则返回被拆分的切片id
         * @param ppOutLeftClip 如果切片被拆分，则返回被拆分后的左侧切片
         * @param ppOutRightClip 如果切片被拆分，则返回被拆分后的右侧切片
         * @return
         */
        virtual StatusCode moveClip(ID clipId, Rational rOffsetOnTrack, ID *pOutSplitClipId, IClip **ppOutLeftClip, IClip **ppOutRightClip) =0;

        /**
         * 改变切片时长(秒) - 会改变在此切片右侧所有切片在时间线上的偏移量, 需要遍历切片，更新切片UI
         * @param clipId 切片id
         * @param rDuration
         * @return
         */
        virtual StatusCode changeClipDuration(ID clipId, const Rational& rDuration) =0;

        /**
         * 改变切片在媒体中的偏移时间(秒) - 会同时改变此切片在时间线上的偏移量，以及其右侧所有切片在时间线上的偏移量, 需要遍历切片，更新切片UI
         * @param clipId 切片id
         * @param rOffsetInMedia
         * @return
         */
        virtual StatusCode changeClipOffsetInMedia(ID clipId, const Rational& rOffsetInMedia) =0;

        /**
         * 删除指定序号的切片
         * @param nIndex
         */
        virtual StatusCode removeClip(int nIndex) =0;

        /**
         * 删除指定切片
         * @param clipId
         */
        virtual StatusCode removeClipById(ID clipId) =0;

        /**
         * 删除所有切片
         */
        virtual void removeAllClips() =0;
    };

    #define ITrackPtr ITrack*
}

#endif //PROJECT_ITRACK_H
