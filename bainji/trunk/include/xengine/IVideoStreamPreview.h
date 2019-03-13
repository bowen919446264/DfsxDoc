//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_IVIDEOSTREAMPREVIEW_H
#define PROJECT_IVIDEOSTREAMPREVIEW_H

#include "IPreview.h"

namespace xedit {
    /**
     * 视频流预览
     */
    class IVideoStreamPreview: public IPreview {
    public:
        virtual ~IVideoStreamPreview() {}

        /**
         * 流序号
         * @return
         */
        virtual int getStreamIndex() const =0;
    };
}

#endif //PROJECT_IVIDEOSTREAMPREVIEW_H
