//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_AVMEDIA_H
#define PROJECT_AVMEDIA_H

#include "BaseMedia.h"
#include "NLEHeader.h"
#include <map>
using namespace std;

namespace xedit {

    class XEngine;
    class ITimeLine;

    class CAVMedia: public CBaseMedia, public IInnerAVMedia {
    public:
        CAVMedia(CTimeLine *pTimeLine = NULL);
        virtual ~CAVMedia();

        /**
         * 获得媒体类型
         * @return
         */
        virtual EMediaType getMediaType() const;

        /**
         * 获得媒体信息
         * @param pOutMediaInfo
         */
        virtual void getMediaInfo(AVMediaInfo *pOutMediaInfo) const;

        /**
         * 设置媒体信息
         * @param mediaInfo
         */
        virtual void setMediaInfo(const AVMediaInfo& mediaInfo);

        /**
         * 获得媒体预览
         * @param nStreamIndex 流序号
         * @return
         */
        virtual IPreview* getPreview(int nStreamIndex) const;

        /**
         * 设置媒体预览
         * @param nStreamIndex 流序号
         * @param pPreview
         */
        virtual void setPreview(int nStreamIndex, IPreview *pPreview);

        /**
         * 开启预览会话(在调用createPreviewFrame前必须调用)
         * @return
         */
        virtual StatusCode openPreviewSession();

        /**
         * 创建预览帧
         * @param nStreamIndex 流序号
         * @param rOffsetInMedia 预览帧在媒体中的偏移量
         * @return
         */
        virtual PreviewFrame* createPreviewFrame(int nStreamIndex, Rational rOffsetInMedia);

        /**
         * 关闭预览会话(在完成预览创建后，释放资源)
         */
        virtual void closePreviewSession();

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @return
         */
        virtual IClip* newClip() const;

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @param nStreamIndex
         * @return
         */
        virtual IClip* newClip(int nStreamIndex) const;

    private:
        AVMediaInfo             m_mediaInfo;
        map<int, IPreview*>     m_previewMap;

        TDsSmartPtr<IDsImporter>    m_pImporter;
        uint8_t*                    m_pBuffer;
        int                         m_nBufferSize;
    };
}

#endif //PROJECT_AVMEDIA_H
