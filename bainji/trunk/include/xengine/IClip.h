//
// Created by wendachuan on 2018/11/30.
//

#ifndef PROJECT_ICLIP_H
#define PROJECT_ICLIP_H

#include "avpub/Rational.h"
#include "xutil/id.h"
#include "IPreview.h"

using namespace libav;

namespace xedit {
    /**
     * 切片类型
     */
    enum EClipType {
        EClipType_Invalid,
        EClipType_Video,
        EClipType_Audio,
        EClipType_Image,
        EClipType_SVG
    };

    /**
     * 切片
     */
    class IClip {
    public:
        virtual ~IClip() {}

        /**
         * 获得切片id
         * @return
         */
        virtual ID getId() const =0;

        /**
         * 获得切片类型
         * @return
         */
        virtual EClipType getType() const =0;

        /**
         * 获得切片引用的媒体id
         * @return
         */
        virtual ID getRefMediaId() const =0;

        /**
         * 获得切片在时间线上的偏移量
         * @return
         */
        virtual Rational getOffsetOnTrack() const =0;

        /**
         * 获取切片时长(秒)
         * @return
         */
        virtual Rational getDuration() const =0;

        /**
         * 获取切片在媒体中的偏移时间(秒)
         * @return
         */
        virtual Rational getOffsetInMedia() const =0;
    };

    #define IClipPtr IClip*


    /**
     * 视音频切片
     */
    class IAVClip: virtual public IClip {
    public:
        virtual ~IAVClip() {}

        /**
         * 获得切片的视音频流序号
         * @return
         */
        virtual int getStreamIndex() const =0;
    };

    #define IAVClipPtr IAVClip*

    /**
     * 图片切片
     */
    class IImageClip: virtual public IClip {
    public:
        virtual ~IImageClip() {}
    };

    #define IImageClipPtr IImageClip*

    /**
     * svg切片
     */
    class ISVGClip: virtual public IClip {
    public:
        virtual ~ISVGClip() {}
    };

    #define ISVGClipPtr ISVGClip*
}

#endif //PROJECT_ICLIP_H
