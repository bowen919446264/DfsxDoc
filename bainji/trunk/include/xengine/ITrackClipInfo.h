//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_ITRACKCLIPINFO_H
#define PROJECT_ITRACKCLIPINFO_H

#include "xutil/id.h"
#include "avpub/Rational.h"

using namespace libav;

namespace xedit {
    /**
     * 轨道切片信息
     */
    class ITrackClipInfo {
    public:
        virtual ~ITrackClipInfo() {}

        /**
         * 获取引用的切片id
         * @return
         */
        virtual ID getRefClipId() const =0;

        /**
         * 获得切片在时间线上的偏移量
         * @return
         */
        virtual Rational getOffsetOnTrack() const =0;
    };

    #define ITrackClipInfoPtr ITrackClipInfo*
}

#endif //PROJECT_ITRACKCLIPINFO_H
