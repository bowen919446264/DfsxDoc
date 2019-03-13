//
// Created by wendachuan on 2018/11/6.
//

#ifndef XEDIT_PREVIEWFRAME_H
#define XEDIT_PREVIEWFRAME_H

#include "avpub/Rational.h"
#include <string>

using  namespace std;
using namespace libav;

namespace xedit {
    /**
     * 预览帧
     */
    struct PreviewFrame {
        string      path;           // 路径
        Rational    rTimeOffset;    // 时间偏移
    };
}

#endif //XEDIT_PREVIEWFRAME_H
