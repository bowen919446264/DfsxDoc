//
// Created by wendachuan on 2018/12/5.
//

#include "AVClip.h"
#include "../NLEHelper.h"
#include <math.h>
#include "../XEngine.h"
#include "../action/TChangePropertyAction.h"
#include "avpub/StatusCode.h"

using namespace xedit;

CAVClip::CAVClip(): CBaseClip() {
    m_nStrmIdx = -1;
}

CAVClip::~CAVClip() {

}

/**
 * 获得切片的视音频流序号
 * @return
 */
int CAVClip::getStreamIndex() const {
    return m_nStrmIdx;
}

/**
 * 设置切片的视音频流序号
 * @param nStrmIdx
 */
void CAVClip::setStreamIndex(int nStrmIdx) {
    m_nStrmIdx = nStrmIdx;
}

/**
 * 获取切片在媒体中的偏移时间(秒)
 * @return
 */
//Rational CAVClip::getOffsetInMedia() const {
//    if (m_spDsClip) {
//        IDsClip *pDsClip = m_spDsClip;
//        SClipInfo clipInfo = pDsClip->GetClipInfo();
//        return Rational(clipInfo.m_frameTrimIn, NLE_FPS);
//    } else {
//        return m_rOffsetInMedia;
//    }
//}

/**
 * 设置切片在媒体中的偏移时间(秒)
 * @param rOffsetInMedia
 */
//void CAVClip::setOffsetInMedia(const Rational& rOffsetInMedia) {
//    if (m_rOffsetInMedia.nNum == LONG_LONG_MIN) { // 初次设置
//        innerSetOffsetInMedia(rOffsetInMedia);
//        return;
//    }
//
//    if (m_rOffsetInMedia == rOffsetInMedia) {
//        return;
//    }
//
//    TChangePropertyAction<CBaseClip, Rational> *pPropertyAction = new TChangePropertyAction<CBaseClip, Rational>(m_pTimeLine,
//            AT_ChangeClipOffsetInMedia,
//            this,
//            getOffsetInMediaWrapper,
//            setOffsetInMediaWrapper,
//            rOffsetInMedia);
//    pPropertyAction->addParam(AP_ChangeClipOffsetInMedia_ClipId, m_id);
//    pPropertyAction->addParam(AP_ChangeClipOffsetInMedia_OldOffsetInMedia, getOffsetInMedia());
//    pPropertyAction->addParam(AP_ChangeClipOffsetInMedia_NewOffsetInMedia, rOffsetInMedia);
//    m_pTimeLine->doAction(pPropertyAction);
//}

//void CAVClip::innerSetOffsetInMedia(const Rational& rOffsetInMedia) {
//    m_rOffsetInMedia = rOffsetInMedia;
//    if (m_spDsClip) {
//        IInnerAVMedia *pAVMedia = dynamic_cast<IInnerAVMedia*>(m_pTimeLine->getMediaById(m_refMediaId));
//        if (pAVMedia) {
//            SDsFileInfo dsFileInfo = {0};
//            pAVMedia->getDsFileInfo(&dsFileInfo);
//            SClipInfo clipInfo = m_spDsClip->GetClipInfo();
//            int64_t nNewOffsetInMedia = rOffsetInMedia.nNum * NLE_FPS / rOffsetInMedia.nDen;
//            clipInfo.m_framePosition += (nNewOffsetInMedia - clipInfo.m_framePosition);
//            clipInfo.m_frameTrimIn = nNewOffsetInMedia;
//            clipInfo.m_frameDuration = clipInfo.m_frameTrimOut - clipInfo.m_frameTrimIn + 1;
//            m_spDsClip->SetClipInfo(clipInfo);
//            update();
//        }
//    }
//}

/**
 * 绑定IDsClip
 * @param pClip
 */
//void CAVClip::bindDsClip(IDsClip *pClip) {
//    CBaseClip::bindDsClip(pClip);
//    setOffsetInMedia(m_rOffsetInMedia);
//}