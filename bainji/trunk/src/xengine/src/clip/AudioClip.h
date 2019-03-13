//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_AUDIOCLIP_H
#define PROJECT_AUDIOCLIP_H

#include "AVClip.h"

namespace xedit {
    /**
     * 音频切片
     */
    class CAudioClip: public CAVClip {
    public:
        CAudioClip();

        /**
         * 获得切片类型
         * @return
         */
        virtual xedit::EClipType getType() const;
    };
}

#endif //PROJECT_AUDIOCLIP_H
