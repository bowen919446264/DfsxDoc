//
// Created by wendachuan on 2018/11/29.
//

#include "AVMedia.h"
#include "xutil/StdUtil.h"
#include "../clip/AVClip.h"
#include "../NLEHelper.h"
#include "../XEngine.h"
#include "../Preview.h"
#include "../XEngine.h"
#include "avpub/AVMemory.h"
#include <inttypes.h>
#include "avpub/StatusCode.h"

using namespace xedit;

CAVMedia::CAVMedia(CTimeLine *pTimeLine):
        CBaseMedia(pTimeLine),
        m_pImporter(NULL),
        m_pBuffer(NULL),
        m_nBufferSize(0) {
    memset(&m_mediaInfo, 0, sizeof(m_mediaInfo));
}

CAVMedia::~CAVMedia() {
    m_pImporter = NULL;
    destoryMap(&m_previewMap);
    if (m_pBuffer) {
        AVFree((void**)&m_pBuffer);
    }
}

/**
 * 获得媒体类型
 * @return
 */
EMediaType CAVMedia::getMediaType() const {
    return EMediaType_AV;
}

/**
 * 获得媒体信息
 * @param pOutMediaInfo
 */
void CAVMedia::getMediaInfo(AVMediaInfo *pOutMediaInfo) const{
    *pOutMediaInfo = m_mediaInfo;
}

/**
 * 设置媒体信息
 * @param mediaInfo
 */
void CAVMedia::setMediaInfo(const AVMediaInfo& mediaInfo) {
    m_mediaInfo = mediaInfo;
}

/**
 * 获得媒体预览
 * @param nStreamIndex 流序号
 * @return
 */
IPreview* CAVMedia::getPreview(int nStreamIndex) const {
    assert(nStreamIndex >= 0);
    map<int, IPreview*>::const_iterator it = m_previewMap.find(nStreamIndex);
    if (it != m_previewMap.end())
        return it->second;
    else
        return NULL;
}

/**
 * 设置媒体预览
 * @param pPreview
 */
void CAVMedia::setPreview(int nStreamIndex, IPreview *pPreview) {
    assert(nStreamIndex >= 0 && pPreview);
    map<int, IPreview*>::const_iterator it = m_previewMap.find(nStreamIndex);
    if (it != m_previewMap.end()) {
        IPreview *pPreview = it->second;
        m_previewMap.erase(it);
        delete pPreview;
    }
    m_previewMap[nStreamIndex] = pPreview;
}

/**
 * 开启预览会话(在调用createPreviewFrame前必须调用)
 * @return
 */
StatusCode CAVMedia::openPreviewSession() {
    TDsSmartPtr<IDsPlayEngineManager> pEngineMgr = m_pTimeLine->getDsPlayEngineManager();
    m_pImporter = pEngineMgr->CreateImporter();
    if (!m_pImporter) return AV_OTHER_ERROR;

    StatusCode code = m_pImporter->OpenFile(m_mediaInfo.path, FALSE);
    if (FAILED(code)) return code;

    ProjectSetting projectSetting = m_pTimeLine->getSetting();
    if (!m_pBuffer) {
        int nLineSize = projectSetting.nWidth * 4;
        int nBufferSize = nLineSize * projectSetting.nHeight;
        m_pBuffer = (uint8_t*)AVMalloc(nBufferSize);
        m_nBufferSize = nBufferSize;
    }

    return AV_OK;
}

/**
 * 创建预览帧
 * @param nStreamIndex 流序号
 * @param rOffsetInMedia 预览帧在媒体中的偏移量
 * @return
 */
PreviewFrame* CAVMedia::createPreviewFrame(int nStreamIndex, Rational rOffsetInMedia) {
    int64_t nPos = AVRationalScale(NLE_FPS, rOffsetInMedia).integerValue();
    StatusCode code = m_pImporter->GetFrame(nPos, m_pBuffer, m_nBufferSize, 32768);
    if (FAILED(code)) return NULL;

    IInnerPreview *pPreview = NULL;
    map<int, IPreview*>::const_iterator it = m_previewMap.find(nStreamIndex);
    if (it == m_previewMap.end()) {
        pPreview = new CPreview(m_pTimeLine->getEngine()->getEngineSetting().previewFrameSize);
        m_previewMap[nStreamIndex] = pPreview;
    } else {
        pPreview = dynamic_cast<IInnerPreview*>(it->second);
    }

    IXEngine *pEngine = m_pTimeLine->getEngine();
    assert(pEngine);
    EngineSetting engineSetting = pEngine->getEngineSetting();
    ProjectSetting projectSetting = m_pTimeLine->getSetting();

    char path[AV_MAX_PATH] = {0};
    getPreviewDir(path);
    snprintf(path, AV_MAX_PATH - 1, "%s/%" PRId64 "_%" PRId64 ".png", path, rOffsetInMedia.nNum, rOffsetInMedia.nDen);

    // 保存
    int nLineSize = projectSetting.nWidth * 4;
    code = xSaveImage(path, m_pBuffer, EPIX_FMT_ABGR, m_nBufferSize, nLineSize, GSize(projectSetting.nWidth, projectSetting.nHeight), engineSetting.previewFrameSize, EImageType_PNG);
    if (FAILED(code)) {
        return NULL;
    }

    PreviewFrame frame;
    frame.path = path;
    frame.rTimeOffset = rOffsetInMedia;
    pPreview->addPreviewFrame(frame);

    return pPreview->getPreviewFrame(pPreview->getPreviewFrameCount() - 1);
}

/**
 * 关闭预览会话(在完成预览创建后，释放资源)
 */
void CAVMedia::closePreviewSession() {
    m_pImporter = NULL;
}

/**
 * 创建一个切片(调用者负责释放创建的切片)
 * @return
 */
IClip* CAVMedia::newClip() const {
    return CBaseMedia::newClip(m_mediaInfo.rDuration);
}

/**
 * 创建一个切片(调用者负责释放创建的切片)
 * @param nStreamIndex
 * @return
 */
IClip* CAVMedia::newClip(int nStreamIndex) const {
    TDsSmartPtr<IDsClipFactory> pClipFactory = m_pTimeLine->getDsClipFactory();

    EClipType clipType = EClipType_Invalid;
    for (int i = 0; i < m_mediaInfo.nVideoCount; i++) {
        if (m_mediaInfo.vStreams[i].nIndex == nStreamIndex) {
            clipType = EClipType_Video;
        }
    }
    if (clipType == EClipType_Invalid) {
        for (int i = 0; i < m_mediaInfo.nAudioCount; i++) {
            if (m_mediaInfo.aStreams[i].nIndex == nStreamIndex) {
                clipType = EClipType_Audio;
            }
        }
    }
    if (clipType == EClipType_Invalid) {
        AVLOG(ELOG_LEVEL_ERROR, "没有找到序号为[%d]的流", nStreamIndex);
        return NULL;
    }

    int64_t nTimeLineDuration = AVRationalScale(NLE_FPS, m_pTimeLine->getDuration()).integerValue();
    int64_t nMediaDuration = AVRationalScale(NLE_FPS, m_mediaInfo.rDuration).integerValue();
    IInnerClip *pClip = NULL;
    StatusCode code = NLEHelper::createAVClip(m_id, pClipFactory, m_fileInfo, clipType, nStreamIndex, nTimeLineDuration, nMediaDuration, 0, nMediaDuration, &pClip);
    if (FAILED(code)) {
        return NULL;
    }
    pClip->setRefMediaId(m_id);
    return pClip;
}