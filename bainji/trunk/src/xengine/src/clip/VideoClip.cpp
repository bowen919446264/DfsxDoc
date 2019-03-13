//
// Created by wendachuan on 2018/12/5.
//

#include "VideoClip.h"
#include "avpub/StatusCode.h"
#include "avpub/AVMemory.h"
#include "../NLEHelper.h"
#include "../XEngine.h"
#include <inttypes.h>

using namespace xedit;

CVideoClip::CVideoClip(): CAVClip() {

}

/**
 * 获得切片类型
 * @return
 */
xedit::EClipType CVideoClip::getType() const {
    return EClipType_Video;
}


/**
 * 添加预览帧
 * @param rOffsetInMedia 在文件中的时间偏移量
 * @return
 */
//StatusCode CVideoClip::addPreviewFrame(Rational rOffsetInMedia) {
//    assert(m_spPreview);
//
//    EngineSetting engineSetting = pEngine->getEngineSetting();
//    ProjectSetting projectSetting = m_pTimeLine->getSetting();
//
//    IDsPlayer *pPlayer = m_pTimeLine->getDsPlayer();
//    if (!pPlayer) return AV_OTHER_ERROR;
//
//    Rational rFrameOffsetOnTrack = m_rOffsetOnTrack + rOffsetInMedia - m_rOffsetInMedia;
//    int64_t nPos = AVRationalScale(NLE_FPS, rFrameOffsetOnTrack).integerValue();
//    int nLineSize = projectSetting.nWidth * 4;
//    int nBufferSize = nLineSize * projectSetting.nHeight;
//    BYTE *pBuffer = (BYTE*)AVMalloc(nBufferSize);
//    if (!pBuffer) {
//        return AV_OTHER_ERROR;
//    }
//    StatusCode code = pPlayer->GetSingleFrame(nPos, pBuffer, nBufferSize);
//    if (FAILED(code)) {
//        AVFree((void**)&pBuffer);
//        return AV_OTHER_ERROR;
//    }
//
//    char path[AV_MAX_PATH] = {0};
//    snprintf(path, AV_MAX_PATH - 1, "%s/preview/%" PRId64 "-%" PRId64 "_%" PRId64 ".jpg", engineSetting.cacheDir, m_id, rOffsetInMedia.nNum, rOffsetInMedia.nDen);
//
//    // 保存jpg
//    code = xSaveImage(path, pBuffer, EPIX_FMT_ABGR, nBufferSize, nLineSize, projectSetting.nWidth, projectSetting.nHeight, engineSetting.previewFrameSize, EImageType_PNG);
//    if (FAILED(code)) {
//        AVFree((void**)&pBuffer);
//        return code;
//    }
//
//    PreviewFrame frame;
//    frame.path = path;
//    frame.rTimeOffset = rOffsetInMedia;
//    m_spPreview->addPreviewFrame(frame);
//    return AV_OK;
//}
