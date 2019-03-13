//
// Created by wendachuan on 2018/12/6.
//

#ifndef PROJECT_IINNERCLIP_H
#define PROJECT_IINNERCLIP_H

#include "xengine/IClip.h"
#include "NLEHeader.h"
#include "IInnerPreview.h"

namespace xedit {
    class ITimeLine;
    class IInnerClip: virtual public IClip {
    public:
        virtual ~IInnerClip() {}

        /**
         * 设置切片id
         * @param id
         */
        virtual void setId(ID id) =0;

        /**
         * 设置切片引用的媒体id
         * @param id
         */
        virtual void setRefMediaId(ID id) =0;

        /**
         * 设置切片在时间线上的偏移量(并不会立即生效，需要调用ITrack->updateClip来更新)
         * @param rOffsetOnTrack
         * @return
         */
        virtual void setOffsetOnTrack(Rational rOffsetOnTrack) =0;

        /**
         * 设置切片时长(秒)(并不会立即生效，需要调用ITrack->updateClip来更新)
         * @param rDuration
         */
        virtual void setDuration(const Rational& rDuration) =0;

        /**
         * 设置切片在媒体中的偏移时间(秒)(并不会立即生效，需要调用ITrack->updateClip来更新)
         * @param rOffsetInMedia
         */
        virtual void setOffsetInMedia(const Rational& rOffsetInMedia) =0;

        /**
         * 绑定IDsClip
         * @param pClip
         */
        virtual void bindDsClip(IDsClip *pClip) =0;

        /**
         * 返回绑定的IDsClip
         * @return
         */
        virtual IDsClip* getBindDsClip() const =0;
    };

    class IInnerAVClip: virtual public IInnerClip, public IAVClip {
    public:
        virtual ~IInnerAVClip() {}

        /**
         * 设置切片的视音频流序号
         * @param nStrmIdx
         */
        virtual void setStreamIndex(int nStrmIdx) =0;
    };

    /**
     * 创建一个切片对象
     * @param clipType
     * @return
     */
    IInnerClip* createClip(EClipType clipType);
}

#endif //PROJECT_IINNERCLIP_H
