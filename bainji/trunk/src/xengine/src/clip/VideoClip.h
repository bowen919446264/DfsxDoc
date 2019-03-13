//
// Created by wendachuan on 2018/12/5.
//

#ifndef PROJECT_VIDEOCLIP_H
#define PROJECT_VIDEOCLIP_H

#include "AVClip.h"
#include "xutil/Image.h"
#include "avpub/EPixFormat.h"

using namespace libav;

namespace xedit {
    /**
     * 视频切片
     */
    class CVideoClip: public CAVClip {
    public:
        CVideoClip();

        /**
         * 获得切片类型
         * @return
         */
        virtual xedit::EClipType getType() const;

        /**
         * 添加预览帧
         * @param rOffsetInMedia 在文件中的时间偏移量
         * @return
         */
        //virtual StatusCode addPreviewFrame(Rational rOffsetInMedia);
    };
}

#endif //PROJECT_VIDEOCLIP_H
