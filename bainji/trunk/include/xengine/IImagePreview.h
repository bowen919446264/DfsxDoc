//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_IIMAGEPREVIEW_H
#define PROJECT_IIMAGEPREVIEW_H

#include "IPreview.h"

namespace xedit {
    /**
     * 图片预览
     */
    class IImagePreview: public IPreview {
    public:
        virtual ~IImagePreview() {}
    };
}

#endif //PROJECT_IIMAGEPREVIEW_H
