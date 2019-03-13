//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_IMAGECLIP_H
#define PROJECT_IMAGECLIP_H

#include "BaseClip.h"

namespace xedit {
    /**
     * 图片切片
     */
    class CImageClip: public CBaseClip, public IImageClip {
    public:
        CImageClip();

        /**
         * 获得切片类型
         * @return
         */
        virtual xedit::EClipType getType() const;
    };
}

#endif //PROJECT_IMAGECLIP_H
