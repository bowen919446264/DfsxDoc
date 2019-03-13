//
// Created by wendachuan on 2019/3/11.
//

#include "ChangeClipOffsetInMediaAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"
#include "../Track.h"

using namespace xedit;

CChangeClipOffsetInMediaAction::CChangeClipOffsetInMediaAction(CTimeLine *pTimeLine, ID trackId, ID clipId, Rational rOffsetInMedia):
        CBaseAction(pTimeLine),
        m_trackId(trackId),
        m_clipId(clipId),
        m_rOffsetInMedia(rOffsetInMedia) {
    addParam(AP_ChangeClipOffsetInMedia_ClipId, clipId);
    addParam(AP_ChangeClipOffsetInMedia_NewOffsetInMedia, rOffsetInMedia);
}

CChangeClipOffsetInMediaAction::~CChangeClipOffsetInMediaAction() {

}

/**
 * 改变切片在文件内的偏移量
 * @param rOldOffsetInMedia
 * @param rNewOffsetInMedia
 * @return
 */
StatusCode CChangeClipOffsetInMediaAction::changeClipOffsetInMedia(Rational rOldOffsetInMedia, Rational rNewOffsetInMedia) {
    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    assert(pTrack);

    vector<IInnerClip*>& clips = pTrack->getClips();

    IInnerClip *pClip = dynamic_cast<IInnerClip*>(pTrack->getClipById(m_clipId));
    assert(pClip);

    Rational rDiff = rNewOffsetInMedia - rOldOffsetInMedia;
    if (rOldOffsetInMedia < rNewOffsetInMedia) {
        pClip->setOffsetInMedia(rNewOffsetInMedia);
        //pClip->setOffsetOnTrack(pClip->getOffsetOnTrack() + rDiff);
        pClip->setDuration(pClip->getDuration() - rDiff);
        StatusCode code = pTrack->updateClip(pClip->getId());
        if (FAILED(code)) return code;

        bool bFindClip = false;
        for (vector<IInnerClip*>::const_iterator it = clips.begin(); it != clips.end(); it++) {
            IInnerClip *pTmpClip = *it;

            if (bFindClip) {
                pTmpClip->setOffsetOnTrack(pTmpClip->getOffsetOnTrack() - rDiff);
                code = pTrack->updateClip(pTmpClip->getId());
                if (FAILED(code)) return code;
            }

            if (pTmpClip->getId() == m_clipId) {
                bFindClip = true;
            }
        }
    } else if (rOldOffsetInMedia > rNewOffsetInMedia) {
        bool bFindClip = false;
        for (vector<IInnerClip*>::const_iterator it = clips.begin(); it != clips.end(); it++) {
            IInnerClip *pTmpClip = *it;

            if (bFindClip) {
                pTmpClip->setOffsetOnTrack(pTmpClip->getOffsetOnTrack() - rDiff);
                StatusCode code = pTrack->updateClip(pTmpClip->getId());
                if (FAILED(code)) return code;
            }

            if (pTmpClip->getId() == m_clipId) {
                bFindClip = true;
            }
        }

        pClip->setOffsetInMedia(rNewOffsetInMedia);
        pClip->setDuration(pClip->getDuration() - rDiff);
        return pTrack->updateClip(pClip->getId());
    }

    return AV_OK;
}

/**
 * 执行动作
 * @return
 */
StatusCode CChangeClipOffsetInMediaAction::excute() {
    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    assert(pTrack);

    IInnerClip *pClip = dynamic_cast<IInnerClip*>(pTrack->getClipById(m_clipId));
    assert(pClip);

    m_rOldOffsetInMedia = pClip->getOffsetInMedia();
    StatusCode code = changeClipOffsetInMedia(m_rOldOffsetInMedia, m_rOffsetInMedia);
    if (FAILED(code)) return code;

    addParam(AP_ChangeClipOffsetInMedia_OldOffsetInMedia, m_rOldOffsetInMedia);
    return AV_OK;
}

/**
 * 取消执行
 */
void CChangeClipOffsetInMediaAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CChangeClipOffsetInMediaAction::unDo() {
    return changeClipOffsetInMedia(m_rOffsetInMedia, m_rOldOffsetInMedia);
}