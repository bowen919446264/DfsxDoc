//
// Created by wendachuan on 2018/11/27.
//

#include "RemoveClipAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"
#include "../TrackSerialize.h"
#include "../Track.h"

using namespace xedit;

CRemoveClipAction::CRemoveClipAction(CTimeLine *pTimeLine, ID trackId, ID clipId): CBaseAction(pTimeLine) {
    m_trackId = trackId;
    m_clipId = clipId;
    m_clipNode = NULL;
    addParam(AP_RemoveClip_ClipId, m_clipId);
}

CRemoveClipAction::~CRemoveClipAction() {
    if (m_clipNode) {
        xmlFreeNode(m_clipNode);
    }
}

/**
 * 执行动作
 * @return
 */
StatusCode CRemoveClipAction::excute() {
    CTrack *pTrack = (CTrack*)m_pTimeLine->getTrack(m_trackId);
    if (!pTrack) return AV_OTHER_ERROR;

    IClip *pClip = pTrack->getClipById(m_clipId);
    if (!pClip) return AV_OTHER_ERROR;

    m_clipNode = CTrackSerialize::xmlCreateClipNode(pClip);
    if (!m_clipNode) return AV_OTHER_ERROR;

    return pTrack->atomRemoveClip(m_clipId);
}

/**
 * 取消执行
 */
void CRemoveClipAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CRemoveClipAction::unDo() {
    assert(m_clipNode);
    IInnerClip *pClip = NULL;
    StatusCode code = CTrackSerialize::xmlLoadClip(m_clipNode, &pClip);
    if (FAILED(code)) return code;

    IInnerTrack *pTrack = dynamic_cast<IInnerTrack*>(m_pTimeLine->getTrack(m_trackId));
    if (!pTrack) return AV_OTHER_ERROR;

    IMedia *pMedia = m_pTimeLine->getMediaById(pClip->getRefMediaId());
    if (!pMedia) return AV_OTHER_ERROR;

    IInnerMedia *pInnerMedia = dynamic_cast<IInnerMedia*>(pMedia);
    if (!pInnerMedia) return AV_OTHER_ERROR;

    SDsFileInfo fileInfo = {sizeof(SDsFileInfo)};
    pInnerMedia->getDsFileInfo(&fileInfo);

    uint64_t nFrameOffsetOnTrack = Rational(pClip->getOffsetOnTrack().nNum * NLE_FPS, pClip->getOffsetOnTrack().nDen).integerValue();
    uint64_t nFrameDurationTrack = Rational(pClip->getDuration().nNum * NLE_FPS, pClip->getDuration().nDen).integerValue();
    uint64_t nMediaFrameStart = 0;
    Rational rMediaFrameEnd = pClip->getDuration();
    if (pClip->getType() == EClipType_Audio || pClip->getType() == EClipType_Video) {
        IInnerAVClip *pAVClip = dynamic_cast<IInnerAVClip*>(pClip);
        if (!pAVClip) return AV_OTHER_ERROR;
        nMediaFrameStart = Rational(pAVClip->getOffsetInMedia().nNum * NLE_FPS, pAVClip->getOffsetInMedia().nDen).integerValue();
        rMediaFrameEnd = pAVClip->getOffsetInMedia() + pAVClip->getDuration();
    }

    uint64_t nMediaFrameEnd = Rational(rMediaFrameEnd.nNum * NLE_FPS, rMediaFrameEnd.nDen).integerValue();
    IDsFileClip *pDsFileClip = NULL;
    IDsClipFactory *pDsClipFactory = m_pTimeLine->getDsClipFactory();
    code = NLEHelper::createClip(pDsClipFactory, fileInfo, nFrameOffsetOnTrack, nFrameDurationTrack, nMediaFrameStart, nMediaFrameEnd, &pDsFileClip);
    if (FAILED(code)) return code;

    pClip->bindDsClip(pDsFileClip);
    pTrack->addClip(pClip);
    return AV_OK;
}