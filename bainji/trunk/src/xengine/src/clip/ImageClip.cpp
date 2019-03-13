//
// Created by wendachuan on 2018/12/5.
//

#include "ImageClip.h"

using namespace xedit;

CImageClip::CImageClip(): CBaseClip() {

}

/**
 * 获得切片类型
 * @return
 */
xedit::EClipType CImageClip::getType() const {
    return EClipType_Image;
}