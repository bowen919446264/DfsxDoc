//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_IMEDIAPREVIEW_H
#define PROJECT_IMEDIAPREVIEW_H

#include "PreviewFrame.h"
#include "GSize.h"

namespace xedit {

    /**
     * 预览
     */
    class IPreview {
    public:
        virtual ~IPreview() {}

        /**
         * 获得预览图像尺寸
         * @return
         */
        virtual GSize getPreviewSize() const =0;

        /**
         * 获得预览帧数量
         * @return
         */
        virtual int getPreviewFrameCount() const =0;

        /**
         * 获得指定序号的预览帧
         * @param nIndex
         * @return
         */
        virtual PreviewFrame* getPreviewFrame(int nIndex) const =0;

        /**
         * 获得指定位置附近的一个预览帧
         * @param rOffsetInMedia 在文件中的时间偏移量
         * @return
         */
        virtual PreviewFrame* getPreviewFrameNearBy(Rational rOffsetInMedia) const =0;
    };
}

#endif //PROJECT_IMEDIAPREVIEW_H
