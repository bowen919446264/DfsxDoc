//
// Created by wendachuan on 2018/12/6.
//

#ifndef PROJECT_IINNERMEDIA_H
#define PROJECT_IINNERMEDIA_H

#include "xengine/IMedia.h"
#include "NLEHeader.h"
#include "IInnerPreview.h"

using namespace PlayEngine;

namespace xedit {
    class ITimeLine;

    class IInnerMedia: virtual public IMedia {
    public:
        virtual ~IInnerMedia() {}

        /**
         * 设置媒体id
         * @param id
         */
        virtual void setId(ID id) =0;

        /**
         * 设置媒体路径
         * @param path
         */
        virtual void setPath(const char* path) =0;

        /**
         * 设置SDsFileInfo
         * @param fileInfo
         */
        virtual void setDsFileInfo(const SDsFileInfo& fileInfo) =0;

        /**
         * 获取SDsFileInfo
         * @param pOutFileInfo
         */
        virtual void getDsFileInfo(SDsFileInfo *pOutFileInfo) const =0;

        /**
         * 设置时间线
         * @param pTimeLine
         */
        virtual void setTimeLine(ITimeLine *pTimeLine) =0;

        /**
         * 设置预览
         * @param pPreview
         */
        virtual void setPreview(IInnerPreview *pPreview) =0;
    };

    class IInnerAVMedia: virtual public IInnerMedia, public IAVMedia {
    public:
        virtual ~IInnerAVMedia() {}

        /**
         * 设置媒体信息
         * @param mediaInfo
         */
        virtual void setMediaInfo(const AVMediaInfo& mediaInfo) =0;

        /**
         * 设置媒体预览
         * @param pMediaPreview
         */
        virtual void setPreview(int nStreamIndex, IPreview *pMediaPreview) =0;
    };

    class IInnerImageMedia: virtual public IInnerMedia, public IImageMedia {
    public:
        virtual ~IInnerImageMedia() {}

        /**
         * 设置图片类型
         * @param imageType
         */
        virtual void setImageType(EImageType imageType) =0;

        /**
         * 设置图片尺寸
         * @param size
         */
        virtual void setSize(GSize size) =0;
    };

    class IInnerSVGMedia: virtual public IInnerMedia, public ISVGMedia {
    public:
        virtual ~IInnerSVGMedia() {}

        /**
         * 设置svg内容
         * @param svg
         */
        virtual void setSvg(const char* svg) =0;
    };

    /**
     * 创建媒体
     * @param mediaType
     * @return
     */
    IInnerMedia* createMedia(EMediaType mediaType);
}

#endif //PROJECT_IINNERMEDIA_H
