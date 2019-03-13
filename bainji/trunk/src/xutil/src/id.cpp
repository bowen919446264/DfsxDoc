//
// Created by wendachuan on 2018/11/7.
//

#include "xutil/id.h"
#include <time.h>
#include <stdlib.h>

/**
 * 生成一个随机id
 * @return
 */
ID idGenerateOne() {
    static bool seedInited = false;
    if (!seedInited) {
        seedInited = true;
        srand(time(NULL));
    }
    static int64_t min = 0x0FFFFFFFFFFFFFFF;
    static int64_t mod = 0x7000000000000000;
    int64_t nLow = (rand() % mod + min) & 0x00000000FFFFFFFF;
    int64_t nHigh = ((rand() % mod + min) << 32) & 0x7FFFFFFF00000000;
    int64_t id = nHigh|nLow;
    return id;
}
