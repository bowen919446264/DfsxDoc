//
// Created by wendachuan on 2018/12/5.
//

#include "BaseClip.h"
#include "../NLEHelper.h"
#include "xengine/IXEngine.h"
#include "avpub/StatusCode.h"
#include "../XEngine.h"
#include "../action/TChangePropertyAction.h"
#include "../Preview.h"
#include "xutil/NumberConvertor.h"

using namespace xedit;

CBaseClip::CBaseClip() {
    m_id = idGenerateOne();
    m_refMediaId = -1;
    m_spDsClip = NULL;
    m_rDuration.nNum = LONG_LONG_MIN;
}

CBaseClip::~CBaseClip() {

}

/**
 * 获得切片id
 * @return
 */
ID CBaseClip::getId() const {
    return m_id;
}

/**
 * 设置切片id
 * @param id
 */
void CBaseClip::setId(ID id) {
    m_id = id;
}

/**
 * 获得切片引用的媒体id
 * @return
 */
ID CBaseClip::getRefMediaId() const {
    return m_refMediaId;
}

/**
 * 设置切片引用的媒体id
 * @param id
 */
void CBaseClip::setRefMediaId(ID id) {
    m_refMediaId = id;
}

/**
 * 获得切片在时间线上的偏移量
 * @return
 */
Rational CBaseClip::getOffsetOnTrack() const {
    return m_rOffsetOnTrack;
//    if (m_spDsClip) {
//        IDsClip *pDsClip = m_spDsClip;
//        SClipInfo clipInfo = pDsClip->GetClipInfo();
//        return Rational(clipInfo.m_framePosition, NLE_FPS);
//    } else
//        return m_rOffsetOnTrack;
}

/**
 * 设置切片在时间线上的偏移量
 * @param rOffsetOnTrack
 * @return
 */
void CBaseClip::setOffsetOnTrack(Rational rOffsetOnTrack) {
    m_rOffsetOnTrack = rOffsetOnTrack;
//    if (m_spDsClip) {
//        SClipInfo clipInfo = m_spDsClip->GetClipInfo();
//        int64_t nOldPos = clipInfo.m_framePosition;
//        int64_t nNewPos = rOffsetOnTrack.nNum * NLE_FPS / rOffsetOnTrack.nDen;
//
//        IInnerTrack *pTrack = dynamic_cast<IInnerTrack*>(m_pTimeLine->getTrack(m_trackId));
//        if (pTrack) {
//            IDsTrack *pDsTrack = pTrack->getBindDsTrack();
//            if (pDsTrack)
//                pDsTrack->MoveClip(m_spDsClip, nNewPos - nOldPos);
//        } else {
//            clipInfo.m_framePosition = nNewPos;
//            m_spDsClip->SetClipInfo(clipInfo);
//        }
//    }
}

const Rational& CBaseClip::getOffsetOnTrackWrapper(const IInnerClip *pClip) {
    return pClip->getOffsetOnTrack();
}

//StatusCode CBaseClip::setOffsetOnTrackWrapper(CBaseClip *pClip, const Rational& rOffsetOnTrack) {
//    pClip->setOffsetOnTrack(rOffsetOnTrack);
//}

/**
 * 获取切片时长(秒)
 * @return
 */
Rational CBaseClip::getDuration() const {
    return m_rDuration;
//    if (m_spDsClip) {
//        IDsClip *pDsClip = m_spDsClip;
//        SClipInfo clipInfo = pDsClip->GetClipInfo();
//        return Rational(clipInfo.m_frameDuration, NLE_FPS);
//    } else {
//        return m_rDuration;
//    }
}

/**
 * 设置切片时长(秒)
 * @param rDuration
 */
void CBaseClip::setDuration(const Rational& rDuration) {
    m_rDuration = rDuration;
//    if (m_rDuration.nNum == LONG_LONG_MIN) { // 初次设置
//        innerSetDuration(rDuration);
//        return;
//    }
//
//    if (m_rDuration == rDuration) {
//        return;
//    }
//
//    TChangePropertyAction<CBaseClip, Rational> *pPropertyAction = new TChangePropertyAction<CBaseClip, Rational>(m_pTimeLine, AT_ChangeClipDuration, this, getDurationWrapper, setDurationWrapper, rDuration);
//    pPropertyAction->addParam(AP_ChangeClipDuration_ClipId, m_id);
//    pPropertyAction->addParam(AP_ChangeClipDuration_OldDuration, getDuration());
//    pPropertyAction->addParam(AP_ChangeClipDuration_NewDuration, rDuration);
//    m_pTimeLine->doAction(pPropertyAction);
}

const Rational& CBaseClip::getDurationWrapper(const IInnerClip *pClip) {
    return pClip->getDuration();
}

//StatusCode CBaseClip::setDurationWrapper(CBaseClip *pClip, const Rational& rDuration) {
//    pClip->setDuration(rDuration);
//}

/**
 * 获取切片在媒体中的偏移时间(秒)
 * @return
 */
Rational CBaseClip::getOffsetInMedia() const {
    return m_rOffsetInMedia;
}

/**
 * 设置切片在媒体中的偏移时间(秒)
 * @param rOffsetInMedia
 */
void CBaseClip::setOffsetInMedia(const Rational& rOffsetInMedia) {
    m_rOffsetInMedia = rOffsetInMedia;
}

const Rational& CBaseClip::getOffsetInMediaWrapper(const IInnerClip *pClip) {
    return pClip->getOffsetInMedia();
}

//StatusCode CBaseClip::setOffsetInMediaWrapper(CBaseClip *pClip, const Rational& rOffsetInMedia) {
//    pClip->setOffsetInMedia(rOffsetInMedia);
//}

/**
 * 设置切片在媒体中的偏移时间(秒)
 * @param rOffsetInMedia
 */
//void CBaseClip::innerSetOffsetInMedia(const Rational& rOffsetInMedia) {
//
//}
//

//
//void CBaseClip::innerSetDuration(const Rational& rDuration) {
//    m_rDuration = rDuration;
//    if (m_spDsClip) {
//        IInnerAVMedia *pAVMedia = dynamic_cast<IInnerAVMedia*>(m_pTimeLine->getMediaById(m_refMediaId));
//        if (pAVMedia) {
//            SDsFileInfo dsFileInfo = {0};
//            pAVMedia->getDsFileInfo(&dsFileInfo);
//            SClipInfo clipInfo = m_spDsClip->GetClipInfo();
//            clipInfo.m_frameDuration = rDuration.nNum * NLE_FPS / rDuration.nDen;
//            clipInfo.m_frameTrimOut = std::min(clipInfo.m_frameTrimIn + clipInfo.m_frameDuration, dsFileInfo.m_ui64Duration);
//            m_spDsClip->SetClipInfo(clipInfo);
//        } else {
//            SClipInfo clipInfo = m_spDsClip->GetClipInfo();
//            clipInfo.m_frameDuration = rDuration.nNum * NLE_FPS / rDuration.nDen;
//            clipInfo.m_frameTrimOut = clipInfo.m_frameTrimIn + clipInfo.m_frameDuration;
//            m_spDsClip->SetClipInfo(clipInfo);
//        }
//        update();
//    }
//}

/**
 * 绑定IDsClip
 * @param pClip
 */
void CBaseClip::bindDsClip(IDsClip *pClip) {
    m_spDsClip = pClip;
    //innerSetDuration(m_rDuration);
}

/**
 * 返回绑定的IDsClip
 * @return
 */
IDsClip* CBaseClip::getBindDsClip() const {
    return m_spDsClip;
}

/**
 * 更新切片
 * @return
 */
//StatusCode CBaseClip::update() {
//    if (m_trackId < 0) {
//        return AV_OTHER_ERROR;
//    }
//
//    IInnerTrack *pTrack = dynamic_cast<IInnerTrack*>(m_pTimeLine->getTrack(m_trackId));
//    assert(pTrack);
//
//    return pTrack->updateClip(m_id);
//}

/**
 * 设置切片所属轨道id
 * @param trackId
 */
//void CBaseClip::setTrackId(ID trackId) {
//    m_trackId = trackId;
//}

/**
 * 获得切片所属轨道id
 * @return
 */
//ID CBaseClip::getTrackId() const {
//    return m_trackId;
//}
//
//void CBaseClip::setTimeLine(ITimeLine *pTimeLine) {
//    m_pTimeLine = dynamic_cast<CTimeLine*>(pTimeLine);
//}