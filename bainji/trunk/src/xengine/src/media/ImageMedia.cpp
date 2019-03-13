//
// Created by wendachuan on 2018/11/29.
//

#include "ImageMedia.h"
#include "../NLEHelper.h"
#include "../XEngine.h"
#include "xutil/Image.h"
#include <stdio.h>

using namespace xedit;

#define IMAGE_CLIP_DURATION Rational(5, 1)

CImageMedia::CImageMedia(CTimeLine *pTimeLine):
        CBaseMedia(pTimeLine),
        m_pPreview(NULL) {
    m_imageType = EImageType_Unknown;
}

CImageMedia::~CImageMedia() {

}

/**
 * 获得媒体类型
 * @return
 */
EMediaType CImageMedia::getMediaType() const {
    return EMediaType_Image;
}

/**
 * 获取图片类型
 * @return
 */
EImageType CImageMedia::getImageType() const {
    return m_imageType;
}

/**
 * 设置图片类型
 * @param imageType
 */
void CImageMedia::setImageType(EImageType imageType) {
    m_imageType = imageType;
}

/**
 * 获取图片宽度
 * @return
 */
GSize CImageMedia::getSize() const {
    return m_size;
}

/**
 * 设置图片宽度
 * @param size
 */
void CImageMedia::setSize(GSize size) {
    m_size = size;
}

/**
 * 创建一个切片(调用者负责释放创建的切片)
 * @return
 */
IClip* CImageMedia::newClip() const {
    return CBaseMedia::newClip(IMAGE_CLIP_DURATION);
}

/**
 * 获得媒体预览
 * @param nStreamIndex 流序号
 * @return
 */
IPreview* CImageMedia::getPreview() const {
    if (!m_pPreview.get()) {
        auto_ptr<IInnerPreview> *ptr = const_cast<auto_ptr<IInnerPreview>*>(&m_pPreview);
        ptr->reset(new CPreview(m_pTimeLine->getEngine()->getEngineSetting().previewFrameSize));
    }
    if (m_pPreview->getPreviewFrameCount() == 0) {
        char dstPath[AV_MAX_PATH] = {0};
        getPreviewDir(dstPath);
        snprintf(dstPath, AV_MAX_PATH - 1, "%s/0.jpg", dstPath);

        IXEngine *pEngine = m_pTimeLine->getEngine();
        assert(pEngine);
        EngineSetting engineSetting = pEngine->getEngineSetting();
        StatusCode code = xScaleImage(m_path.c_str(), dstPath, engineSetting.previewFrameSize);
        if (FAILED(code)) return NULL;

        PreviewFrame frame;
        frame.path = dstPath;
        m_pPreview->addPreviewFrame(frame);
    }
    return m_pPreview.get();
}

/**
 * 设置预览
 * @param pPreview
 */
void CImageMedia::setPreview(IInnerPreview *pPreview) {
    m_pPreview.reset(pPreview);
}