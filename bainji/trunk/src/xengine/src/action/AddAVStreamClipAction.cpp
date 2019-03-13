//
// Created by wendachuan on 2018/12/18.
//

#include "AddAVStreamClipAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"

using namespace xedit;

CAddAVStreamClipAction::CAddAVStreamClipAction(CTimeLine *pTimeLine, ID trackId, ID mediaId, Rational rOffsetOnTrack, int nStreamIndex): CBaseAction(pTimeLine) {
    m_trackId = trackId;
    m_mediaId = mediaId;
    m_rOffsetOnTrack = rOffsetOnTrack;
    m_nStreamIndex = nStreamIndex;
}

CAddAVStreamClipAction::~CAddAVStreamClipAction() {

}

/**
 * 执行动作
 * @return
 */
StatusCode CAddAVStreamClipAction::excute() {
    IInnerTrack *pTrack = dynamic_cast<IInnerTrack*>(m_pTimeLine->getTrack(m_trackId));
    if (!pTrack) return AV_OTHER_ERROR;

    IAVMedia* pMedia = dynamic_cast<IAVMedia*>(m_pTimeLine->getMediaById(m_mediaId));
    if (!pMedia) return AV_OTHER_ERROR;

    IClip *pClip = pMedia->newClip(m_nStreamIndex);
    if (!pClip) return AV_OTHER_ERROR;

    IInnerClip *pInnerClip = dynamic_cast<IInnerClip*>(pClip);
    if (!pInnerClip) {
        delete pClip;
        return AV_OTHER_ERROR;
    }

    m_clipId = pInnerClip->getId();
    pInnerClip->setOffsetOnTrack(m_rOffsetOnTrack);
    pTrack->addClip(pInnerClip);
    return AV_OK;
}

/**
 * 取消执行
 */
void CAddAVStreamClipAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CAddAVStreamClipAction::unDo() {
    ITrack* pTrack = m_pTimeLine->getTrack(m_trackId);
    if (!pTrack)
        return AV_OTHER_ERROR;

    pTrack->removeClipById(m_clipId);
    return AV_OK;
}

/**
 * 获得切片id
 * @return
 */
ID CAddAVStreamClipAction::getClipId() const {
    return m_clipId;
}