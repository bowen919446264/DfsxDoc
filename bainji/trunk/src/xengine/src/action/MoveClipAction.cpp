//
// Created by wendachuan on 2018/11/27.
//

#include "MoveClipAction.h"
#include "xengine/IXEngine.h"
#include "../TimeLine.h"
#include "avpub/Log.h"
#include "../NLEHelper.h"
#include "avpub/StatusCode.h"
#include <inttypes.h>
#include "../Track.h"
#include "RemoveClipAction.h"
#include "AddClipAction.h"
#include "TChangePropertyAction.h"
#include "../clip/BaseClip.h"

using namespace xedit;

CMoveClipAction::CMoveClipAction(CTimeLine *pTimeLine, ID trackId, ID clipId, Rational rOffsetOnTrack):
        CBaseAction(pTimeLine),
        m_splitAction(pTimeLine),
        m_moveAction(pTimeLine),
        m_trackId(trackId),
        m_clipId(clipId),
        m_rOffsetOnTrack(rOffsetOnTrack) {
    addParam(AP_MoveClip_ClipId, m_clipId);
    addParam(AP_MoveClip_NewOffsetOnTrack, m_rOffsetOnTrack);
}

CMoveClipAction::~CMoveClipAction() {

}

StatusCode CMoveClipAction::splitIfNeeded() {
    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    assert(pTrack);

    // 判断目标位置是否在其他切片中间
    if (pTrack->offsetInMiddleOfClip(m_rOffsetOnTrack)) {
        IClip *pMiddleClip = pTrack->findClip(m_rOffsetOnTrack);
        if (pMiddleClip->getId() != m_clipId) { // 拆分切片
            IMedia* pMiddleMedia = m_pTimeLine->getMediaById(pMiddleClip->getRefMediaId());
            if (!pMiddleMedia) return AV_OTHER_ERROR;

            Rational rSplitOffsetInClip = m_rOffsetOnTrack - pMiddleClip->getOffsetOnTrack();
            Rational oldOffsetInMedia = pMiddleClip->getOffsetInMedia();
            Rational oldDuration = pMiddleClip->getDuration();

            CRemoveClipAction *pRemoveClipAction = new CRemoveClipAction(m_pTimeLine, m_trackId, pMiddleClip->getId());
            CAddClipAction *pLeftAddClipAction = new CAddClipAction(
                    m_pTimeLine,
                    m_trackId,
                    pMiddleMedia->getId(),
                    pMiddleClip->getOffsetOnTrack(),
                    oldOffsetInMedia,
                    rSplitOffsetInClip);
            CAddClipAction *pRightAddClipAction = new CAddClipAction(
                    m_pTimeLine,
                    m_trackId,
                    pMiddleMedia->getId(),
                    pMiddleClip->getOffsetOnTrack() + rSplitOffsetInClip,
                    oldOffsetInMedia + rSplitOffsetInClip,
                    oldDuration - rSplitOffsetInClip);

            m_splitAction.addAction(pRemoveClipAction);
            m_splitAction.addAction(pLeftAddClipAction);
            m_splitAction.addAction(pRightAddClipAction);
        }
    }

    return m_splitAction.excute();
}

/**
 * 执行动作
 * 定义: 移动前，切片在轨道上的序号为a; 移动后，切片在轨道上的序号为b; 切片时长为d; 插入的目标位置为p
 * 有下面几种情况：
 * 1）a == b, 即移动后，此切片在轨道上的序号未改变，此时只改变切片在时间线上的偏移量即可
 * 2) b < a, 即移动后，此切片在轨道上的序号变小，首先将切片删除，然后将序号范围[b, a-1]的切片向右移动d，最后将切片插入到p位置
 * 3) a < b, 即移动后，此切片在轨道上的序号变大，首先将切片删除，然后将序号范围[a, b-1]的切片向左移动d, 最后将切片插入到p位置
 * @return
 */
StatusCode CMoveClipAction::excute() {
    StatusCode code = splitIfNeeded();
    if (FAILED(code)) return code;

    CTrack *pTrack = dynamic_cast<CTrack*>(m_pTimeLine->getTrack(m_trackId));
    assert(pTrack);

    vector<IInnerClip*>& clips = pTrack->getClips();

    // 确保目标位置不在其他切片中间
    for (vector<IInnerClip*>::const_iterator it = clips.begin(); it != clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getOffsetOnTrack() < m_rOffsetOnTrack
            && m_rOffsetOnTrack < pClip->getOffsetOnTrack() + pClip->getDuration() - Rational(1, NLE_FPS)
                && pClip->getId() != m_clipId) {
            return AV_OTHER_ERROR;
        }
    }

    int a = -1, b = -1;
    vector<IInnerClip*> tmpClips(clips);
    for (int i = 0; i < tmpClips.size(); i++) {
        IInnerClip *pClip = tmpClips[i];
        if (pClip->getId() == m_clipId) {
            a = i;
            tmpClips.erase(tmpClips.begin() + i);
            break;
        }
    }
    Rational rLeftClipEndPos = Rational::zero;
    Rational rRightClipStartPos = Rational::zero;
    for (int i = 0; i < tmpClips.size(); i++) {
        IInnerClip *pClip = tmpClips[i];
        rRightClipStartPos = pClip->getOffsetOnTrack();
        if (rLeftClipEndPos <= m_rOffsetOnTrack && m_rOffsetOnTrack <= rRightClipStartPos) {
            b = i;
        }
        rLeftClipEndPos = pClip->getOffsetOnTrack() + pClip->getDuration();
    }
    if (b < 0 && tmpClips.size() > 0) {
        if (rLeftClipEndPos <= m_rOffsetOnTrack) {
            b = tmpClips.size();
        }
    }
    if (a < 0 || b < 0) return AV_OTHER_ERROR;

    IInnerClip *pClip = dynamic_cast<IInnerClip*>(pTrack->getClipById(m_clipId));
    if (!pClip) return AV_OTHER_ERROR;

    //m_rOldOffsetOnTrack = pClip->getOffsetOnTrack();
    Rational d = pClip->getDuration();
    Rational rOffsetInMedia = pClip->getOffsetInMedia();
    ID mediaId = pClip->getRefMediaId();
    CAddClipAction *pAddClipAction = NULL;

    if (a == b) {
        TChangePropertyAction<IInnerClip, Rational> *pPropertyAction = new TChangePropertyAction<IInnerClip, Rational>(
                m_pTimeLine,
                AT_ChangeClipOffsetOnTrack,
                pClip,
                pTrack,
                CBaseClip::getOffsetOnTrackWrapper,
                CTrack::clipSetOffsetOnTrackWrapper,
                m_rOffsetOnTrack);
        pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_ClipId, m_clipId);
        pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_OldOffsetOnTrack, pClip->getOffsetOnTrack());
        pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_NewOffsetOnTrack, m_rOffsetOnTrack);
        m_moveAction.addAction(pPropertyAction);
    } else if (b < a) {
        CRemoveClipAction *pRemoveClipAction = new CRemoveClipAction(m_pTimeLine, m_trackId, m_clipId);
        m_moveAction.addAction(pRemoveClipAction);

        for (int i = a - 1; i >= b; i--) {
            IInnerClip *pTmpClip = clips[i];
            TChangePropertyAction<IInnerClip, Rational> *pPropertyAction = new TChangePropertyAction<IInnerClip, Rational>(
                    m_pTimeLine,
                    AT_ChangeClipOffsetOnTrack,
                    pTmpClip,
                    pTrack,
                    CBaseClip::getOffsetOnTrackWrapper,
                    CTrack::clipSetOffsetOnTrackWrapper,
                    pTmpClip->getOffsetOnTrack() + d);
            pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_ClipId, pTmpClip->getId());
            pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_OldOffsetOnTrack, pTmpClip->getOffsetOnTrack());
            pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_NewOffsetOnTrack, pTmpClip->getOffsetOnTrack() + d);
            m_moveAction.addAction(pPropertyAction);
        }

        pAddClipAction = new CAddClipAction(m_pTimeLine, m_trackId, mediaId, m_rOffsetOnTrack, rOffsetInMedia, d);
        m_moveAction.addAction(pAddClipAction);
    } else {
        CRemoveClipAction *pRemoveClipAction = new CRemoveClipAction(m_pTimeLine, m_trackId, m_clipId);
        m_moveAction.addAction(pRemoveClipAction);

        for (int i = a + 1; i <= b; i++) {
            IInnerClip *pTmpClip = clips[i];
            TChangePropertyAction<IInnerClip, Rational> *pPropertyAction = new TChangePropertyAction<IInnerClip, Rational>(
                    m_pTimeLine,
                    AT_ChangeClipOffsetOnTrack,
                    pTmpClip,
                    pTrack,
                    CBaseClip::getOffsetOnTrackWrapper,
                    CTrack::clipSetOffsetOnTrackWrapper,
                    pTmpClip->getOffsetOnTrack() - d);
            pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_ClipId, pTmpClip->getId());
            pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_OldOffsetOnTrack, pTmpClip->getOffsetOnTrack());
            pPropertyAction->addParam(AP_ChangeClipOffsetOnTrack_NewOffsetOnTrack, pTmpClip->getOffsetOnTrack() - d);
            m_moveAction.addAction(pPropertyAction);
        }

        pAddClipAction = new CAddClipAction(m_pTimeLine, m_trackId, mediaId, m_rOffsetOnTrack - d, rOffsetInMedia, d);
        m_moveAction.addAction(pAddClipAction);
    }

    code = m_moveAction.excute();
    if (SUCCESS(code) && pAddClipAction) {
        ID newClipId = pAddClipAction->getClipId();
        IInnerClip *pNewClip = dynamic_cast<IInnerClip*>(pTrack->getClipById(newClipId));
        assert(pNewClip);
        pNewClip->setId(m_clipId);
    }
    return AV_OK;
}

/**
 * 取消执行
 */
void CMoveClipAction::cancel() {

}

/**
 * 回退
 * @return
 */
StatusCode CMoveClipAction::unDo() {
    StatusCode code = m_moveAction.unDo();
    if (FAILED(code)) return code;

    return m_splitAction.unDo();
}