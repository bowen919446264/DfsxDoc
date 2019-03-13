//
// Created by wendachuan on 2018/12/6.
//

#ifndef PROJECT_IINNERPREVIEW_H
#define PROJECT_IINNERPREVIEW_H

#include "xengine/IPreview.h"

namespace xedit {
    class IInnerPreview: public IPreview {
    public:
        virtual ~IInnerPreview() {}

        /**
         * 添加预览帧
         * @param previewFrame
         */
        virtual void addPreviewFrame(const PreviewFrame& previewFrame) =0;
    };
}

#endif //PROJECT_IINNERPREVIEW_H
