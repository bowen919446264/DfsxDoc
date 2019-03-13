//
// Created by wendachuan on 2018/12/5.
//

#include "Track.h"
#include "xutil/StdUtil.h"
#include "XEngine.h"
#include "avpub/Log.h"
#include "avpub/StatusCode.h"
#include "NLEHelper.h"
#include "XEngine.h"
#include "action/AddClipAction.h"
#include "action/AddAVStreamClipAction.h"
#include "action/MoveClipAction.h"
#include "action/ChangeClipDurationAction.h"
#include "action/ChangeClipOffsetInMediaAction.h"
#include "action/GroupAction.h"
#include "action/RemoveClipAction.h"
#include "action/TChangePropertyAction.h"
#include "clip/BaseClip.h"
#include <inttypes.h>

using namespace libav;

using namespace xedit;

CTrack::CTrack(CTimeLine *pTimeLine): m_pTimeLine(pTimeLine) {
    m_id = idGenerateOne();
    m_trackType = ETrackType_Invalid;
    m_isDisabled = false;
    m_spDsTrack = NULL;
}

CTrack::~CTrack() {
    removeAllClips();
    if (m_spDsTrack) {
        TDsSmartPtr<IDsTimeline> pDsTimeLine = m_pTimeLine->getDsTimeline();
        pDsTimeLine->RemoveTrack(m_spDsTrack);
        m_spDsTrack = NULL;
    }
}

/**
 * 获得轨道id
 * @return
 */
ID CTrack::getId() const {
    return m_id;
}

/**
 * 设置轨道id
 * @param id
 */
void CTrack::setId(ID id) {
    m_id = id;
}

/**
 * 获得轨道类型
 * @return
 */
ETrackType CTrack::getTrackType() const {
    return m_trackType;
}

/**
 * 设置轨道类型
 * @param trackType
 */
void CTrack::setTrackType(ETrackType trackType) {
    m_trackType = trackType;
}

/**
 * 轨道当前是否已禁用
 * @return
 */
bool CTrack::isDisabled() const {
    return m_isDisabled;
}

/**
 * 启用轨道
 */
void CTrack::enable() {
    m_isDisabled = false;
}

/**
 * 禁用轨道
 */
void CTrack::disable() {
    m_isDisabled = true;
}

/**
 * 获取轨道上切片数量
 * @return
 */
int CTrack::getClipCount() const {
    return m_clips.size();
}

/**
 * 获取指定序号的切片
 * @param nIndex
 * @return
 */
IClip* CTrack::getClip(int nIndex) const {
    if (nIndex < m_clips.size())
        return m_clips.at(nIndex);
    else
        return NULL;
}

/**
 * 获取指定id的切片
 */
IClip* CTrack::getClipById(ID clipId) const {
    for (vector<IInnerClip*>::const_iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IClip *pClip = *it;
        if (pClip->getId() == clipId) {
            return pClip;
        }
    }
    return NULL;
}

/**
 * 获得指定位置的切片
 * @param rOffsetOnTrack
 * @return
 */
IClip* CTrack::getClipByOffset(Rational rOffsetOnTrack) const {
    for (vector<IInnerClip*>::const_iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getOffsetOnTrack() <= rOffsetOnTrack && rOffsetOnTrack < pClip->getOffsetOnTrack() + pClip->getDuration()) {
            return pClip;
        }
    }
    return NULL;
}

/**
 * 根据时间，查找切片
 * @param rTime 时间
 * @return
 */
IClip* CTrack::findClip(Rational rTime) const {
    if (!m_spDsTrack) return NULL;

    IDsTrack *pTrack = m_spDsTrack;
    int64_t nFrameIndex = NLEHelper::timeToFrameIndex(rTime);
    IDsClip * pDsClip = pTrack->FindPositionClip(eDsTrackPlayListA, nFrameIndex);
    for (vector<IInnerClip*>::const_iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getBindDsClip() == pDsClip) {
            return pClip;
        }
    }
    return NULL;
}

static bool compareIClip(const IClip *a, const IClip *b){
    return a->getOffsetOnTrack() < b->getOffsetOnTrack();
}

/**
 * 判断在指定偏移量处是否有切片存在
 * @param rOffsetOnTrack
 * @return
 */
bool CTrack::hasClipOnOffset(Rational rOffsetOnTrack) const {
    for (vector<IInnerClip*>::const_iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getOffsetOnTrack() <= rOffsetOnTrack && rOffsetOnTrack < pClip->getOffsetOnTrack() + pClip->getDuration()) {
            return true;
        }
    }
    return false;
}

/**
 * 判断给定的偏移量是否处于切片中间(不含切片首尾两端)
 * @param rOffsetOnTrack
 * @return
 */
bool CTrack::offsetInMiddleOfClip(Rational rOffsetOnTrack) const {
    for (vector<IInnerClip*>::const_iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getOffsetOnTrack() < rOffsetOnTrack && rOffsetOnTrack < pClip->getOffsetOnTrack() + pClip->getDuration() - Rational(1, NLE_FPS)) {
            return true;
        }
    }
    return false;
}

/**
  * 创建切片原子操作
  * @param mediaId
  * @return
  */
IInnerClip* CTrack::atomCreateClip(ID mediaId) {
    IMedia* pMedia = m_pTimeLine->getMediaById(mediaId);
    if (!pMedia) return NULL;

    IClip *pClip = pMedia->newClip();
    if (!pClip) return NULL;

    IInnerClip *pInnerClip = dynamic_cast<IInnerClip*>(pClip);
    if (!pInnerClip) {
        delete pClip;
        return NULL;
    }

    return pInnerClip;
}

/**
 * 向轨道添加切片原子操作
 * @param pClip
 * @return
 */
StatusCode CTrack::atomAddClip(IInnerClip *pClip) {
    IDsClip *pDsClip = pClip->getBindDsClip();
    if (pDsClip) {
        SClipInfo clipInfo = pDsClip->GetClipInfo();
        clipInfo.m_framePosition = (pClip->getOffsetOnTrack() * NLE_FPS).integerValue();
        clipInfo.m_frameDuration = (pClip->getDuration() * NLE_FPS).integerValue();
        clipInfo.m_frameTrimIn = (pClip->getOffsetInMedia() * NLE_FPS).integerValue();
        clipInfo.m_frameTrimOut = clipInfo.m_frameTrimIn + clipInfo.m_frameDuration;
        pDsClip->SetClipInfo(clipInfo);

        StatusCode code = m_spDsTrack->InsertClip(pDsClip);
        assert(SUCCESS(code));
        if (FAILED(code)) {
            AVLOG(ELOG_LEVEL_ERROR, "插入Clip失败!");
            return AV_OTHER_ERROR;
        }
    }

    //pClip->setTimeLine(m_pTimeLine);
    //pClip->setTrackId(m_id);
    m_clips.push_back(pClip);
    sort(m_clips.begin(), m_clips.end(), compareIClip);
    return AV_OK;
}

/**
 * 从轨道中删除切片原子操作
 * @param clipId
 * @return
 */
StatusCode CTrack::atomRemoveClip(ID clipId) {
    for (vector<IInnerClip*>::iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getId() == clipId) {
            if (m_spDsTrack) {
                IDsClip *pDsClip = pClip->getBindDsClip();
                if (pDsClip) {
                    StatusCode code = m_spDsTrack->RemoveClip(pDsClip);
                    if (FAILED(code)) {
                        return code;
                    }
                }
            }
            m_clips.erase(it);
            delete pClip;
            return AV_OK;
        }
    }
    return AV_OTHER_ERROR;
}

/**
 * 移动切片原子操作
 * @param clipId
 * @param rOffsetOnTrack
 * @return
 */
StatusCode CTrack::atomMoveClip(ID clipId, Rational rOffsetOnTrack) {
    IInnerClip *pClip = dynamic_cast<IInnerClip*>(getClipById(clipId));
    if (!pClip) return AV_OTHER_ERROR;

    pClip->setOffsetOnTrack(rOffsetOnTrack);
    return AV_OK;
}

/**
 * 添加切片
 * @param pClip
 */
StatusCode CTrack::addClip(IInnerClip *pClip) {
    return atomAddClip(pClip);
}


/**
 * 添加切片(指定的位置必须没有切片存在)
 * @param mediaId 媒体id
 * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
 * @return
 */
IClip* CTrack::addClip(ID mediaId, Rational rOffsetOnTrack) {
    bool offsetInClip = false;
    for (vector<IInnerClip*>::const_iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getOffsetOnTrack() < rOffsetOnTrack && rOffsetOnTrack < pClip->getOffsetOnTrack() + pClip->getDuration()) {
            offsetInClip = true;
            break;
        }
    }

    if (offsetInClip) {
        AVLOG(ELOG_LEVEL_ERROR, "指定的位置[%" PRId64 ", %" PRId64 "]处已有切片!", rOffsetOnTrack.nNum, rOffsetOnTrack.nDen);
        return NULL;
    } else {
        IMedia* pMedia = m_pTimeLine->getMediaById(mediaId);
        if (!pMedia) return NULL;

        CAddClipAction *pAction = new CAddClipAction(m_pTimeLine, getId(), mediaId, rOffsetOnTrack);
        StatusCode code = m_pTimeLine->doAction(pAction);
        if (FAILED(code)) {
            delete pAction;
            return NULL;
        }

        IInnerClip *pClip = dynamic_cast<IInnerClip*>(getClipById(pAction->getClipId()));
        return pClip;
    }
}

/**
 * 添加切片
 * @param mediaId 媒体id
 * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
 * @return
 */
//IClip* CTrack::innerAddClip(ID mediaId, Rational rOffsetOnTrack) {
//    IMedia* pMedia = m_pTimeLine->getMediaById(mediaId);
//    if (!pMedia) return NULL;
//
//    IClip *pClip = pMedia->newClip();
//    if (!pClip) return NULL;
//
//    IInnerClip *pInnerClip = dynamic_cast<IInnerClip*>(pClip);
//    if (!pInnerClip) {
//        delete pClip;
//        return NULL;
//    }
//
//    pInnerClip->setOffsetOnTrack(rOffsetOnTrack);
//    IDsClip *pDsFileClip = pInnerClip->getBindDsClip();
//
//    // 移动后面的切片
//    if (m_spDsTrack) {
//        for (int i = m_clips.size() - 1; i >= 0; i--) {
//            IInnerClip *pTmpInnerClip = m_clips[i];
//            if (pTmpInnerClip->getOffsetOnTrack() < rOffsetOnTrack)
//                break;
//            IDsClip *pTmpDsClip = pTmpInnerClip->getBindDsClip();
//            if (pTmpDsClip) {
//                int64_t nMoveFrames = pInnerClip->getDuration().nNum * NLE_FPS / pInnerClip->getDuration().nDen;
//                HRESULT hr = m_spDsTrack->MoveClip(pTmpDsClip, nMoveFrames);
//                if (FAILED(hr)) goto failure;
//            }
//        }
//    }
//
//    if (pDsFileClip) {
//        StatusCode code = m_spDsTrack->InsertClip(pDsFileClip);
//        assert(SUCCESS(code));
//        if (FAILED(code)) {
//            AVLOG(ELOG_LEVEL_ERROR, "插入Clip失败!");
//            goto failure;
//        }
//    }
//
//    pInnerClip->setTrackId(m_id);
//    m_clips.push_back(pInnerClip);
//    sort(m_clips.begin(), m_clips.end(), compareIClip);
//    return pInnerClip;
//
//failure:
//    SAFE_DELETE(pInnerClip);
//    return NULL;
//}

/**
 * 添加视音频切片
 * @param mediaId 媒体id
 * @param rOffsetOnTrack 切片在时间线上的偏移量(秒)
 * @param nStreamIndex 视/音频流序号
 * @return
 */
//IClip* CTrack::addAVStreamClip(ID mediaId, Rational rOffsetOnTrack, int nStreamIndex) {
//    CAddAVStreamClipAction *pAction = new CAddAVStreamClipAction(m_pTimeLine, getId(), mediaId, rOffsetOnTrack, nStreamIndex);
//    StatusCode code = m_pTimeLine->doAction(pAction);
//    if (FAILED(code)) {
//        delete pAction;
//        return NULL;
//    }
//
//    IClip *pClip = getClipById(pAction->getClipId());
//    return pClip;
//}

/**
 * 拆分切片
 * @param clipId 待拆分的切片id
 * @param rSplitOffsetInClip 拆分位置(在切片中的偏移量)
 * @param ppOutLeftClip 拆分后的左侧切片
 * @param ppOutRightClip 拆分后的右侧切片
 * @return
 */
StatusCode CTrack::splitClip(ID clipId, Rational rSplitOffsetInClip, IClip **ppOutLeftClip, IClip **ppOutRightClip) {
    IClip *pOldClip = getClipById(clipId);
    if (!pOldClip) return AV_OTHER_ERROR;

    IMedia* pMedia = m_pTimeLine->getMediaById(pOldClip->getRefMediaId());
    if (!pMedia) return AV_OTHER_ERROR;

    Rational oldOffsetInMedia = pOldClip->getOffsetInMedia();
    Rational oldDuration = pOldClip->getDuration();

    CRemoveClipAction *pRemoveClipAction = new CRemoveClipAction(m_pTimeLine, m_id, clipId);
    CAddClipAction *pLeftAddClipAction = new CAddClipAction(
            m_pTimeLine,
            m_id,
            pMedia->getId(),
            pOldClip->getOffsetOnTrack(),
            oldOffsetInMedia,
            rSplitOffsetInClip);
    CAddClipAction *pRightAddClipAction = new CAddClipAction(
            m_pTimeLine,
            m_id,
            pMedia->getId(),
            pOldClip->getOffsetOnTrack() + rSplitOffsetInClip + Rational(1, NLE_FPS),
            oldOffsetInMedia + rSplitOffsetInClip,
            oldDuration - rSplitOffsetInClip);

    vector<IInnerAction*> actions;
    actions.push_back(pRemoveClipAction);
    actions.push_back(pLeftAddClipAction);
    actions.push_back(pRightAddClipAction);

    CGroupAction *pGroupAction = new CGroupAction(m_pTimeLine, actions);
    StatusCode code = m_pTimeLine->doAction(pGroupAction);
    if (FAILED(code)) {
        delete pGroupAction;
        return NULL;
    }

    CBaseClip *pLeftClip = dynamic_cast<CBaseClip*>(getClipById(pLeftAddClipAction->getClipId()));
    CBaseClip *pRightClip = dynamic_cast<CBaseClip*>(getClipById(pRightAddClipAction->getClipId()));

    *ppOutLeftClip = pLeftClip;
    *ppOutRightClip = pRightClip;
    return AV_OK;
}

/**
 * 移动切片
 * 如果目标位置处于其它切片中间，则会拆分该切片，然后按下述流程处理
 * 定义: 移动前，切片在轨道上的序号为a; 移动后，切片在轨道上的序号为b; 切片时长为d; 插入的目标位置为p
 * 有下面几种情况：
 * 1）a == b, 即移动后，此切片在轨道上的序号未改变，此时只改变切片在时间线上的偏移量即可
 * 2) b < a, 即移动后，此切片在轨道上的序号变小，首先将切片删除，然后将序号范围[b, a-1]的切片向右移动d，最后将切片插入到p位置
 * 3) a < b, 即移动后，此切片在轨道上的序号变大，首先将切片删除，然后将序号范围[a, b-1]的切片向左移动d, 最后将切片插入到p位置
 * @param clipId 待移动的切片id
 * @param rOffsetOnTrack 目标位置
 * @param pOutSplitClipId 如果切片被拆分，则返回被拆分的切片id
 * @param ppOutLeftClip 如果切片被拆分，则返回被拆分后的左侧切片
 * @param ppOutRightClip 如果切片被拆分，则返回被拆分后的右侧切片
 * @return
 */
StatusCode CTrack::moveClip(ID clipId, Rational rOffsetOnTrack, ID *pOutSplitClipId, IClip **ppOutLeftClip, IClip **ppOutRightClip) {
    CMoveClipAction *pAction = new CMoveClipAction(m_pTimeLine, m_id, clipId, rOffsetOnTrack);
    StatusCode code = m_pTimeLine->doAction(pAction);
    if (FAILED(code)) {
        delete pAction;
        return code;
    }
    return AV_OK;
}

/**
 * 改变切片时长(秒) - 会改变在此切片右侧所有切片在时间线上的偏移量, 需要遍历切片，更新切片UI
 * @param clipId 切片id
 * @param rDuration
 * @return
 */
StatusCode CTrack::changeClipDuration(ID clipId, const Rational& rDuration) {
    CChangeClipDurationAction *pAction = new CChangeClipDurationAction(m_pTimeLine, m_id, clipId, rDuration);
    StatusCode code = m_pTimeLine->doAction(pAction);
    if (FAILED(code)) {
        delete pAction;
        return code;
    }
    return AV_OK;
}

/**
 * 改变切片在媒体中的偏移时间(秒) - 会同时改变此切片在时间线上的偏移量，以及其右侧所有切片在时间线上的偏移量, 需要遍历切片，更新切片UI
 * @param clipId 切片id
 * @param rOffsetInMedia
 * @return
 */
StatusCode CTrack::changeClipOffsetInMedia(ID clipId, const Rational& rOffsetInMedia) {
    CChangeClipOffsetInMediaAction *pAction = new CChangeClipOffsetInMediaAction(m_pTimeLine, m_id, clipId, rOffsetInMedia);
    StatusCode code = m_pTimeLine->doAction(pAction);
    if (FAILED(code)) {
        delete pAction;
        return code;
    }
    return AV_OK;
}

/**
 * 将指定切片与其左侧切片交换
 * @param clipId
 * @return
 */
//IClip* CTrack::swapWithLeftClip(ID clipId) {
//    IInnerClip *pClip = NULL, *pLeftClip = NULL;
//    for (int i = 0; i < m_clips.size(); i++) {
//        if (m_clips[i]->getId() == clipId) {
//            if (i == 0) {
//                AVLOG(ELOG_LEVEL_ERROR, "swapWithLeftClip: 指定切片左侧没有切片！");
//                return NULL;
//            }
//            pClip = m_clips[i];
//            pLeftClip = m_clips[i - 1];
//            break;
//        }
//    }
//
//    if (!pClip) {
//        AVLOG(ELOG_LEVEL_ERROR, "swapWithLeftClip: 没有找到指定切片！");
//        return NULL;
//    }
//
//    // TODO:
//}

/**
 * 将指定切片与其右侧切片交换
 * @param clipId
 * @return
 */
//IClip* CTrack::swapWithRightClip(ID clipId) {
//    // TODO:
//}

/**
* 删除指定序号的切片
* @param nIndex
*/
StatusCode CTrack::removeClip(int nIndex) {
    if(nIndex < m_clips.size()) {
        vector<IInnerClip*>::iterator it = m_clips.begin() + nIndex;
        CRemoveClipAction *pAction = new CRemoveClipAction(m_pTimeLine, getId(), (*it)->getId());
        return m_pTimeLine->doAction(pAction);
    } else {
        return AV_OTHER_ERROR;
    }
}

/**
 * 删除指定切片
 * @param clipId
 */
StatusCode CTrack::removeClipById(ID clipId) {
    CRemoveClipAction *pAction = new CRemoveClipAction(m_pTimeLine, getId(), clipId);
    return m_pTimeLine->doAction(pAction);
}

/**
 * 删除切片
 * @param clipId 切片id
 */
//StatusCode CTrack::innerRemoveClip(ID clipId) {
//    for (vector<IInnerClip*>::iterator it = m_clips.begin(); it != m_clips.end(); it++) {
//        IInnerClip *pClip = *it;
//        if (pClip->getId() == clipId) {
//            m_clips.erase(it);
//            if (m_spDsTrack) {
//                IDsClip *pDsClip = pClip->getBindDsClip();
//                if (pDsClip)
//                    m_spDsTrack->RemoveClip(pDsClip);
//            }
//            delete pClip;
//            return AV_OK;
//        }
//    }
//    return AV_OTHER_ERROR;
//}

/**
 * 删除所有切片
 */
void CTrack::removeAllClips() {
    while(!m_clips.empty()) {
        vector<IInnerClip*>::iterator it = m_clips.begin();
        IInnerClip *pClip = *it;
        m_clips.erase(it);
        if (m_spDsTrack) {
            IDsClip *pDsClip = pClip->getBindDsClip();
            if (pDsClip)
                m_spDsTrack->RemoveClip(pDsClip);
        }
        delete pClip;
    }
}

/**
 * 移除指定序号的切片并返回(不)
 * @param nIndex
 * @return
 */
IInnerClip* CTrack::popClip(int nIndex) {
    if(nIndex < m_clips.size()) {
        IInnerClip *pClip = m_clips[nIndex];
        m_clips.erase(m_clips.begin() + nIndex);
        return pClip;
    } else {
        return NULL;
    }
}

/**
 * 绑定IDsTrack
 * @param pDsTrack
 */
void CTrack::bindDsTrack(IDsTrack *pDsTrack) {
    m_spDsTrack = pDsTrack;
}

/**
 * 获得绑定的IDsTrack
 * @return
 */
IDsTrack* CTrack::getBindDsTrack() const {
    return m_spDsTrack;
}

/**
 * 清空轨道
 */
void CTrack::clear() {

}

/**
 * 更新切片(在修改了切片信息后，需要调用此函数)
 * @param clipId
 */
StatusCode CTrack::updateClip(ID clipId) {
    for (vector<IInnerClip*>::iterator it = m_clips.begin(); it != m_clips.end(); it++) {
        IInnerClip *pClip = *it;
        if (pClip->getId() == clipId) {
            if (m_spDsTrack) {
                IDsClip *pDsClip = pClip->getBindDsClip();
                if (pDsClip) {
                    StatusCode code = (StatusCode)m_spDsTrack->RemoveClip(pDsClip);
                    if (FAILED(code)) return code;

                    SClipInfo clipInfo = pDsClip->GetClipInfo();
                    clipInfo.m_framePosition = (pClip->getOffsetOnTrack() * NLE_FPS).integerValue();
                    clipInfo.m_frameDuration = (pClip->getDuration() * NLE_FPS).integerValue();
                    clipInfo.m_frameTrimIn = (pClip->getOffsetInMedia() * NLE_FPS).integerValue();
                    clipInfo.m_frameTrimOut = clipInfo.m_frameTrimIn + clipInfo.m_frameDuration;
                    pDsClip->SetClipInfo(clipInfo);
                    code = m_spDsTrack->InsertClip(pDsClip);
                    return code;
                }
            }
        }
    }
    return AV_OTHER_ERROR;
}

StatusCode CTrack::clipSetOffsetOnTrackWrapper(IInnerClip *pClip, const Rational& rOffsetOnTrack, void *pPriv) {
    CTrack *pTrack = (CTrack*)pPriv;
    if (!pTrack) return AV_OTHER_ERROR;

    pClip->setOffsetOnTrack(rOffsetOnTrack);
    return pTrack->updateClip(pClip->getId());
}

StatusCode CTrack::clipSetOffsetInMediaWrapper(IInnerClip *pClip, const Rational& rOffsetInMedia, void *pPriv) {
    CTrack *pTrack = (CTrack*)pPriv;
    if (!pTrack) return AV_OTHER_ERROR;

    pClip->setOffsetInMedia(rOffsetInMedia);
    return pTrack->updateClip(pClip->getId());
}

StatusCode CTrack::clipSetDurationWrapper(IInnerClip *pClip, const Rational& rDuration, void *pPriv) {
    CTrack *pTrack = (CTrack*)pPriv;
    if (!pTrack) return AV_OTHER_ERROR;

    pClip->setDuration(rDuration);
    return pTrack->updateClip(pClip->getId());
}