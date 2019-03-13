//
// Created by wendachuan on 2018/11/29.
//

#include "BaseMedia.h"
#include "../NLEHelper.h"
#include "../XEngine.h"
#include <inttypes.h>
using namespace xedit;

CBaseMedia::CBaseMedia(CTimeLine *pTimeLine) {
    m_pTimeLine = pTimeLine;
    m_id = idGenerateOne();
}

CBaseMedia::~CBaseMedia() {
}

/**
 * 获得媒体id
 * @return
 */
ID CBaseMedia::getId() const {
    return m_id;
}

/**
 * 设置媒体id
 * @param id
 */
void CBaseMedia::setId(ID id) {
    m_id = id;
}

/**
 * 获得媒体路径
 * @return
 */
const char* CBaseMedia::getPath() const {
    return m_path.c_str();
}

/**
 * 设置媒体路径
 * @param path
 */
void CBaseMedia::setPath(const char* path) {
    m_path = path;
}

/**
* 设置SDsFileInfo
* @param fileInfo
*/
void CBaseMedia::setDsFileInfo(const SDsFileInfo& fileInfo) {
    m_fileInfo = fileInfo;
}

/**
 * 获取SDsFileInfo
 * @param pOutFileInfo
 */
void CBaseMedia::getDsFileInfo(SDsFileInfo *pOutFileInfo) const {
    memcpy(pOutFileInfo, &m_fileInfo, sizeof(SDsFileInfo));
}

/**
 * 创建一个切片(调用者负责释放创建的切片)
 * @param rDuration 切片时长(秒)
 * @return
 */
IClip* CBaseMedia::newClip(Rational rDuration) const {
    TDsSmartPtr<IDsClipFactory> pClipFactory = m_pTimeLine->getDsClipFactory();

    int64_t nTimeLineDuration = AVRationalScale(NLE_FPS, m_pTimeLine->getDuration()).integerValue();
    int64_t nMediaDuration = AVRationalScale(NLE_FPS, rDuration).integerValue();
    IInnerClip *pClip = NULL;
    StatusCode code = NLEHelper::createClip(m_id, pClipFactory, m_fileInfo, nTimeLineDuration, nMediaDuration, 0, nMediaDuration, &pClip);
    if (FAILED(code)) {
        return NULL;
    }
    pClip->setRefMediaId(m_id);
    return pClip;
}

/**
 * 设置时间线
 * @param pTimeLine
 */
void CBaseMedia::setTimeLine(ITimeLine *pTimeLine) {
    m_pTimeLine = dynamic_cast<CTimeLine*>(pTimeLine);
}

/**
 * 获得媒体预览
 * @param nStreamIndex 流序号
 * @return
 */
IPreview* CBaseMedia::getPreview() const {
    return NULL;
}

/**
 * 设置预览
 * @param pPreview
 */
void CBaseMedia::setPreview(IInnerPreview *pPreview) {

}

/**
 * 获得预览目录
 * @param path
 */
void CBaseMedia::getPreviewDir(char path[]) const {
    IXEngine *pEngine = m_pTimeLine->getEngine();
    assert(pEngine);
    EngineSetting engineSetting = pEngine->getEngineSetting();
    ProjectSetting projectSetting = m_pTimeLine->getSetting();
    snprintf(path, AV_MAX_PATH - 1, "%s/preview/%" PRId64, engineSetting.cacheDir, m_id);
}