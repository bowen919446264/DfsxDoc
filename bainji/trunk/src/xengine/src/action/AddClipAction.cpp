//
// Created by wendachuan on 2018/11/27.
//

#include "AddClipAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"
#include "../Track.h"

using namespace xedit;

CAddClipAction::CAddClipAction(CTimeLine *pTimeLine, ID trackId, ID mediaId, Rational rOffsetOnTrack):
        CBaseAction(pTimeLine),
        m_trackId(trackId),
        m_mediaId(mediaId),
        m_rOffsetOnTrack(rOffsetOnTrack) {
    m_rOffsetInMedia.nNum = -1;
    m_rDuration.nNum = -1;

    addParam(AP_AddClip_TrackId, trackId);
    addParam(AP_AddClip_MediaId, mediaId);
    addParam(AP_AddClip_OffsetOnTrack, rOffsetOnTrack);
}

CAddClipAction::CAddClipAction(CTimeLine *pTimeLine, ID trackId, ID mediaId, Rational rOffsetOnTrack, Rational rOffsetInMedia, Rational rDuration):
        CAddClipAction(pTimeLine, trackId, mediaId, rOffsetOnTrack) {
    m_rOffsetInMedia = rOffsetInMedia;
    m_rDuration = rDuration;

    addParam(AP_AddClip_OffsetInMedia, rOffsetInMedia);
    addParam(AP_AddClip_Duration, rDuration);
}

CAddClipAction::~CAddClipAction() {

}

/**
 * 执行动作
 * @return
 */
StatusCode CAddClipAction::excute() {
    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    if (!pTrack) return AV_OTHER_ERROR;

    // 创建切片
    IInnerClip *pClip = pTrack->atomCreateClip(m_mediaId);
    pClip->setOffsetOnTrack(m_rOffsetOnTrack);
    if (m_rOffsetInMedia.nNum >= 0)
        pClip->setOffsetInMedia(m_rOffsetInMedia);
    if (m_rDuration.nNum >= 0)
        pClip->setDuration(m_rDuration);

    // 移动后面的切片
//    for (int i = pTrack->m_clips.size() - 1; i >= 0; i--) {
//        IInnerClip *pTmpInnerClip = pTrack->m_clips[i];
//        if (pTmpInnerClip->getOffsetOnTrack() < m_rOffsetOnTrack)
//            break;
//        StatusCode code = pTrack->atomMoveClip(pTmpInnerClip->getId(), pTmpInnerClip->getOffsetOnTrack() + pClip->getDuration());
//        if (FAILED(code)) {
//            delete pClip;
//            return code;
//        }
//    }

    // 添加切片
    StatusCode code = pTrack->atomAddClip(pClip);
    if (FAILED(code)) {
        delete pClip;
        return code;
    }

    m_clipId = pClip->getId();
    addParam(AP_AddClip_ClipId, m_clipId);
    return AV_OK;
}

/**
 * 取消执行
 */
void CAddClipAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CAddClipAction::unDo() {
    CTrack* pTrack = (CTrack*)m_pTimeLine->getTrack(m_trackId);
    if (!pTrack)
        return AV_OTHER_ERROR;

    IClip *pClip = pTrack->getClipById(m_clipId);
    if (!pClip) return AV_OTHER_ERROR;

    Rational rClipDuration = pClip->getDuration();

    // 删除切片
    StatusCode code = pTrack->atomRemoveClip(m_clipId);
    if (FAILED(code)) return code;

    // 移动后面的切片
//    for (int i = pTrack->m_clips.size() - 1; i >= 0; i--) {
//        IInnerClip *pTmpInnerClip = pTrack->m_clips[i];
//        if (pTmpInnerClip->getOffsetOnTrack() < m_rOffsetOnTrack)
//            break;
//        StatusCode code = pTrack->atomMoveClip(pTmpInnerClip->getId(), pTmpInnerClip->getOffsetOnTrack() - rClipDuration);
//        if (FAILED(code)) {
//            delete pClip;
//            return code;
//        }
//    }

    return AV_OK;
}

/**
 * 获得切片id
 * @return
 */
ID CAddClipAction::getClipId() const {
    return m_clipId;
}