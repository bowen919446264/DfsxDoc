//
// Created by wendachuan on 2018/12/5.
//

#include "AudioClip.h"

using namespace xedit;

CAudioClip::CAudioClip(): CAVClip() {

}

/**
 * 获得切片类型
 * @return
 */
xedit::EClipType CAudioClip::getType() const {
    return EClipType_Audio;
}