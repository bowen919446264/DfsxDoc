///////////////////////////////////////////////////////////
//  GenerateSetting.h
//  Implementation of the Class GenerateSetting
//  Created on:      22-06-2018 16:58:42
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_5C42AFB8_0A16_439e_82FB_4AE10F0EAB39__INCLUDED_)
#define EA_5C42AFB8_0A16_439e_82FB_4AE10F0EAB39__INCLUDED_

#include "EncodeParam.h"
#include "avpub/Rational.h"
using namespace libav;

namespace xedit {
    /**
     * 生成设置
     */
    struct GenerateSetting {
        // 生成目录
        char strDestDir[AV_MAX_PATH];

        // 生成名称
        char strDestName[AV_MAX_PATH];

        // 编码参数
        EncodeParam encodeParam;

        // 时间线起始时间
        Rational rStartTime;

        // 时间长度
        Rational rDuration;
    };
}

#endif // !defined(EA_5C42AFB8_0A16_439e_82FB_4AE10F0EAB39__INCLUDED_)
