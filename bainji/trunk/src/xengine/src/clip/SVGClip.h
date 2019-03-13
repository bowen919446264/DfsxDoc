//
// Created by wendachuan on 2018/12/7.
//

#ifndef PROJECT_SVGCLIP_H
#define PROJECT_SVGCLIP_H

#include "BaseClip.h"

namespace xedit {
    /**
     * 图片切片
     */
    class CSVGClip: public CBaseClip, public ISVGClip {
    public:
        CSVGClip();

        /**
         * 获得切片类型
         * @return
         */
        virtual xedit::EClipType getType() const;
    };
}



#endif //PROJECT_SVGCLIP_H
