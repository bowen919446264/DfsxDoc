//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_IMAGEMEDIA_H
#define PROJECT_IMAGEMEDIA_H

#include "BaseMedia.h"
#include "../Preview.h"
#include <memory>
using namespace std;

namespace xedit {

    class CImageMedia: public CBaseMedia, public IInnerImageMedia {
    public:
        CImageMedia(CTimeLine *pTimeLine = NULL);
        virtual ~CImageMedia();

        /**
         * 获得媒体类型
         * @return
         */
        EMediaType getMediaType() const;

        /**
         * 获取图片类型
         * @return
         */
        virtual EImageType getImageType() const;

        /**
         * 设置图片类型
         * @param imageType
         */
        virtual void setImageType(EImageType imageType);

        /**
         * 获取图片尺寸
         * @return
         */
        virtual GSize getSize() const;

        /**
         * 设置图片尺寸
         * @param size
         */
        virtual void setSize(GSize size);

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @return
         */
        virtual IClip* newClip() const;

        /**
         * 获得媒体预览
         * @param nStreamIndex 流序号
         * @return
         */
        virtual IPreview* getPreview() const;

        /**
         * 设置预览
         * @param pPreview
         */
        virtual void setPreview(IInnerPreview *pPreview);

    private:
        EImageType  m_imageType;
        GSize       m_size;
        auto_ptr<IInnerPreview>    m_pPreview;
    };
}



#endif //PROJECT_IMAGEMEDIA_H
