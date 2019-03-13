//
// Created by wendachuan on 2018/12/7.
//

#include "SVGMedia.h"
#include "../NLEHelper.h"
using namespace xedit;

#define SVG_CLIP_DURATION Rational(5, 1)

CSVGMedia::CSVGMedia(CTimeLine *pTimeLine): CBaseMedia(pTimeLine) {

}

CSVGMedia::~CSVGMedia() {

}

/**
 * 获得媒体类型
 * @return
 */
EMediaType CSVGMedia::getMediaType() const {
    return EMediaType_SVG;
}

/**
 * 获得svg内容
 * @return
 */
const char* CSVGMedia::getSvg() const {
    return m_strSvg.c_str();
}

/**
 * 设置svg内容
 * @param svg
 */
void CSVGMedia::setSvg(const char* svg) {
    m_strSvg = svg;
}

/**
 * 创建一个切片(调用者负责释放创建的切片)
 * @return
 */
IClip* CSVGMedia::newClip() const {
    return CBaseMedia::newClip(SVG_CLIP_DURATION);
}