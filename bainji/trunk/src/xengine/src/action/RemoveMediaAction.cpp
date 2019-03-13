//
// Created by wendachuan on 2018/11/27.
//

#include "RemoveMediaAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"

using namespace xedit;

CRemoveMediaAction::CRemoveMediaAction(CTimeLine *pTimeLine, ID mediaId): CBaseAction(pTimeLine) {
    m_mediaId = mediaId;
    addParam(AP_RemoveMedia_MediaId, m_mediaId);
}

CRemoveMediaAction::~CRemoveMediaAction() {

}

/**
 * 执行动作
 * @return
 */
StatusCode CRemoveMediaAction::excute() {

}

/**
 * 取消执行
 */
void CRemoveMediaAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CRemoveMediaAction::unDo() {

}