//
// Created by wendachuan on 2018/12/6.
//

#include "clip/AudioClip.h"
#include "clip/VideoClip.h"
#include "clip/ImageClip.h"
#include "clip/SVGClip.h"

using namespace xedit;

/**
 * 创建一个切片对象
 * @param clipType
 * @return
 */
IInnerClip* xedit::createClip(EClipType clipType) {
    switch (clipType) {
        case EClipType_Audio:
            return new CAudioClip();
        case EClipType_Video:
            return new CVideoClip();
        case EClipType_Image:
            return new CImageClip();
        case EClipType_SVG:
            return new CSVGClip();
        default:
            return NULL;
    }
}