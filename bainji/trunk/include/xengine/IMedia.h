//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_IMEDIA_H
#define PROJECT_IMEDIA_H

#include "xutil/id.h"
#include "IPreview.h"
#include "avpub/MediaInfo.h"
#include "xutil/Image.h"
#include "IClip.h"

using namespace libav;

namespace xedit {

    /**
     * 媒体类型
     */
    enum EMediaType {
        EMediaType_Invalid,
        EMediaType_AV,
        EMediaType_Image,
        EMediaType_SVG
    };

    /**
     * 媒体接口
     */
    class IMedia {
    public:
        virtual ~IMedia() {}

        /**
         * 获得媒体id
         * @return
         */
        virtual ID getId() const =0;

        /**
         * 获得媒体路径
         * @return
         */
        virtual const char* getPath() const =0;

        /**
         * 获得媒体类型
         * @return
         */
        virtual EMediaType getMediaType() const =0;

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @return
         */
        virtual IClip* newClip() const =0;

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @param rDuration 切片时长(秒)
         * @return
         */
        virtual IClip* newClip(Rational rDuration) const =0;

        /**
         * 获得媒体预览
         * @param nStreamIndex 流序号
         * @return
         */
        virtual IPreview* getPreview() const =0;
    };

    #define IMediaPtr IMedia*


    /**
     * 视音频媒体
     */
    class IAVMedia: virtual public IMedia {
    public:
        virtual ~IAVMedia() {}

        /**
         * 获得媒体信息
         * @param pOutMediaInfo
         */
        virtual void getMediaInfo(AVMediaInfo *pOutMediaInfo) const =0;

        /**
         * 开启预览会话(在调用createPreviewFrame前必须调用)
         * @return
         */
        virtual StatusCode openPreviewSession() =0;

        /**
         * 创建预览帧
         * @param nStreamIndex 流序号
         * @param rOffsetInMedia 预览帧在媒体中的偏移量
         * @return
         */
        virtual PreviewFrame* createPreviewFrame(int nStreamIndex, Rational rOffsetInMedia) =0;

        /**
         * 关闭预览会话(在完成预览创建后，释放资源)
         */
        virtual void closePreviewSession() =0;

        /**
         * 获得媒体预览
         * @param nStreamIndex 流序号
         * @return
         */
        virtual IPreview* getPreview(int nStreamIndex) const =0;

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @param nStreamIndex
         * @return
         */
        virtual IClip* newClip(int nStreamIndex) const =0;
    };

    #define IAVMediaPtr IAVMedia*


    /**
     * 图片媒体
     */
    class IImageMedia: virtual public IMedia {
    public:
        virtual ~IImageMedia() {}

        /**
         * 获取图片类型
         * @return
         */
        virtual EImageType getImageType() const =0;

        /**
         * 获取图片尺寸
         * @return
         */
        virtual GSize getSize() const =0;
    };

    #define IImageMediaPtr IImageMedia*

    /**
     * svg媒体
     */
    class ISVGMedia: virtual public IMedia {
    public:
        virtual ~ISVGMedia() {}

        /**
         * 获得svg内容
         * @return
         */
        virtual const char* getSvg() const =0;
    };

    #define ISVGMediaPtr ISVGMedia*
}

#endif //PROJECT_IMEDIA_H
