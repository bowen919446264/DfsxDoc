//
// Created by wendachuan on 2019/3/11.
//

#include "ChangeClipDurationAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"
#include "../Track.h"

using namespace xedit;

CChangeClipDurationAction::CChangeClipDurationAction(CTimeLine *pTimeLine, ID trackId, ID clipId, Rational rDuration):
        CBaseAction(pTimeLine),
        m_trackId(trackId),
        m_clipId(clipId),
        m_rDuration(rDuration) {
    addParam(AP_ChangeClipDuration_ClipId, clipId);
    addParam(AP_ChangeClipDuration_NewDuration, rDuration);
}

CChangeClipDurationAction::~CChangeClipDurationAction() {

}

/**
 * 改变切片时长
 * @param rOldDuration
 * @param rNewDuration
 * @return
 */
StatusCode CChangeClipDurationAction::changeClipDuration(Rational rOldDuration, Rational rNewDuration) {
    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    assert(pTrack);

    vector<IInnerClip*>& clips = pTrack->getClips();

    IInnerClip *pClip = dynamic_cast<IInnerClip*>(pTrack->getClipById(m_clipId));
    assert(pClip);

    Rational rDiff = rNewDuration - rOldDuration;
    if (rOldDuration < rNewDuration) {
        bool bFindClip = false;
        for (vector<IInnerClip*>::const_iterator it = clips.begin(); it != clips.end(); it++) {
            IInnerClip *pTmpClip = *it;

            if (bFindClip) {
                pTmpClip->setOffsetOnTrack(pTmpClip->getOffsetOnTrack() + rDiff);
                StatusCode code = pTrack->updateClip(pTmpClip->getId());
                if (FAILED(code)) return code;
            }

            if (pTmpClip->getId() == m_clipId) {
                bFindClip = true;
            }
        }

        pClip->setDuration(rNewDuration);
        return pTrack->updateClip(pClip->getId());
    } else if (rOldDuration > rNewDuration) {
        pClip->setDuration(rNewDuration);
        StatusCode code = pTrack->updateClip(pClip->getId());
        if (FAILED(code)) return code;

        bool bFindClip = false;
        for (vector<IInnerClip*>::const_iterator it = clips.begin(); it != clips.end(); it++) {
            IInnerClip *pTmpClip = *it;

            if (bFindClip) {
                pTmpClip->setOffsetOnTrack(pTmpClip->getOffsetOnTrack() + rDiff);
                StatusCode code = pTrack->updateClip(pTmpClip->getId());
                if (FAILED(code)) return code;
            }

            if (pTmpClip->getId() == m_clipId) {
                bFindClip = true;
            }
        }
    }

    return AV_OK;
}

/**
 * 执行动作
 * @return
 */
StatusCode CChangeClipDurationAction::excute() {
    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    assert(pTrack);

    IInnerClip *pClip = dynamic_cast<IInnerClip*>(pTrack->getClipById(m_clipId));
    assert(pClip);

    m_rOldDuration = pClip->getDuration();
    StatusCode code = changeClipDuration(m_rOldDuration, m_rDuration);
    if (FAILED(code)) return code;

    addParam(AP_ChangeClipDuration_OldDuration, m_rOldDuration);
    return AV_OK;
}

/**
 * 取消执行
 */
void CChangeClipDurationAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CChangeClipDurationAction::unDo() {
    return changeClipDuration(m_rDuration, m_rOldDuration);
}