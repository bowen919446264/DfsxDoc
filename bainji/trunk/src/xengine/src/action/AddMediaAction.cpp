//
// Created by wendachuan on 2018/11/22.
//

#include "AddMediaAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"
#include "xutil/NumberConvertor.h"

using namespace xedit;

CAddMediaAction::CAddMediaAction(CTimeLine *pTimeLine, const string& mediaPath): CBaseAction(pTimeLine) {
    m_mediaPath = mediaPath;
    m_mediaId = 0;

    addParam(AP_AddMedia_MediaPath, m_mediaPath);
}

CAddMediaAction::~CAddMediaAction(){

}

/**
 * 执行动作
 * @return
 */
StatusCode CAddMediaAction::excute() {
    TDsSmartPtr<IDsImporter> pImporter = m_pTimeLine->getDsImporter();
    assert(pImporter);

    // 打开文件
    IInnerMedia *pMedia = NULL;
    StatusCode code = NLEHelper::openMedia(pImporter, m_mediaPath.c_str(), false, &pMedia);
    if (FAILED(code)) {
        AVLOG(ELOG_LEVEL_ERROR, "AddMediaAction: 打开文件[%s]失败!", m_mediaPath.c_str());
        return code;
    }

    // 添加媒体
    m_pTimeLine->addMedia(pMedia);
    m_mediaId = pMedia->getId();
    return AV_OK;
}

/**
 * 取消执行
 */
void CAddMediaAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CAddMediaAction::unDo() {
    // 删除媒体
    m_pTimeLine->removeMediaById(m_mediaId);
    m_mediaId = 0;
    return AV_OK;
}

/**
 * 返回媒体id
 */
ID CAddMediaAction::getMediaId() const {
    return m_mediaId;
}