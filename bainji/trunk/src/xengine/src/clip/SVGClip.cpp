//
// Created by wendachuan on 2018/12/7.
//

#include "SVGClip.h"

using namespace xedit;

CSVGClip::CSVGClip(): CBaseClip() {

}

/**
 * 获得切片类型
 * @return
 */
xedit::EClipType CSVGClip::getType() const {
    return EClipType_SVG;
}