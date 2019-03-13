//
// Created by wendachuan on 2018/11/20.
//

#ifndef PROJECT_NUMBERCONVERTOR_H
#define PROJECT_NUMBERCONVERTOR_H

#include "avpub/Define.h"
#include <inttypes.h>
#include <type_traits>
#include <errno.h>
#include <limits>
#include <stdio.h>
#include <stdlib.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"

#ifndef LONG_MAX
#define LONG_MAX std::numeric_limits<long>::max()
#endif

#ifndef LONG_MIN
#define LONG_MIN std::numeric_limits<long>::min()
#endif

#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX std::numeric_limits<long long>::max()
#endif

#ifndef LONG_LONG_MIN
#define LONG_LONG_MIN std::numeric_limits<long long>::min()
#endif

#ifndef ULONG_LONG_MAX
#define ULONG_LONG_MAX std::numeric_limits<unsigned long long>::max()
#endif

#ifndef ULONG_LONG_MIN
#define ULONG_LONG_MIN std::numeric_limits<unsigned long long>::min()
#endif

#ifndef INT_MAX
#define INT_MAX std::numeric_limits<int>::max()
#endif

#ifndef INT_MIN
#define INT_MIN std::numeric_limits<int>::min()
#endif

#ifndef ULONG_MAX
#define ULONG_MAX std::numeric_limits<unsigned long>::max()
#endif

#ifndef ULONG_MIN
#define ULONG_MIN std::numeric_limits<unsigned long>::min()
#endif

/**
 * 将字符串转化为数值类型
 * @tparam T 数值类型
 * @param str 待转换的字符串
 * @param outNumber 输出数值
 * @param nBase 进制(2-32)
 * @return 转换是否成功
 */
API_IMPORT_EXPORT
template <typename T>
bool strToNumber(const char* str, T *outNumber, int nBase = 10) {
    errno = 0;
    if (std::is_same<T, bool>::value) {
        long nValue = strtol(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == LONG_MAX || nValue == LONG_MIN)) || (errno != 0 && nValue == 0) || nValue > INT_MAX || nValue < INT_MIN)
            return false;
        *outNumber = nValue == 0 ? false : true;
        return true;
    } else if (std::is_same<T, int>::value) {
        long nValue = strtol(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == INT_MAX || nValue == INT_MIN)) || (errno != 0 && nValue == 0) || nValue > INT_MAX || nValue < INT_MIN)
            return false;
        *outNumber = nValue;
        return true;
    } else if (std::is_same<T, long>::value) {
        long nValue = strtol(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == LONG_MAX || nValue == LONG_MIN)) || (errno != 0 && nValue == 0)) {
            return false;
        }
        *outNumber = nValue;
        return true;
    } else if (std::is_same<T, long long>::value) {
        long long nValue = strtoll(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == LONG_LONG_MAX || nValue == LONG_LONG_MIN)) || (errno != 0 && nValue == 0)) {
            return false;
        }
        *outNumber = nValue;
        return true;
    } else if (std::is_same<T, unsigned int>::value) {
        unsigned long nValue = strtoul(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == ULONG_MAX || nValue == 0)) || (errno != 0 && nValue == 0) || nValue > UINT_MAX) {
            return false;
        }
        *outNumber = nValue;
        return true;
    } else if (std::is_same<T, unsigned long>::value) {
        unsigned long nValue = strtoul(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == ULONG_MAX || nValue == 0)) || (errno != 0 && nValue == 0)) {
            return false;
        }
        *outNumber = nValue;
        return true;
    } else if (std::is_same<T, unsigned long long>::value) {
        unsigned long long nValue = strtoull(str, NULL, nBase);
        if ((errno == ERANGE && (nValue == ULONG_LONG_MAX || nValue == 0)) || (errno != 0 && nValue == 0)) {
            return false;
        }
        *outNumber = nValue;
        return true;
    } else {
        return false;
    }
}

/**
 * 整数转换为字符串
 * @tparam T 数值类型
 * @param integer 待转换的数值
 * @param outStr 输出字符串
 * @param size 输出字符串大小
 * @return 转换是否成功
 */
API_IMPORT_EXPORT
template <typename T>
bool numberToStr(T number, char* outStr, int size) {
    if (std::is_same<T, bool>::value) {
        return snprintf(outStr, size, "%" PRId32, number ? 1 : 0) > 0;
    } else if (std::is_same<T, int>::value) {
        return snprintf(outStr, size, "%" PRId32, number) > 0;
    } else if (std::is_same<T, long>::value) {
        return snprintf(outStr, size, "%" PRIdPTR, number) > 0;
    } else if (std::is_same<T, long long>::value) {
        return snprintf(outStr, size, "%" PRId64, number) > 0;
    } else if (std::is_same<T, unsigned int>::value) {
        return snprintf(outStr, size, "%" PRIu32, number) > 0;
    } else if (std::is_same<T, unsigned long>::value) {
        return snprintf(outStr, size, "%" PRIuPTR, number) > 0;
    } else if (std::is_same<T, unsigned long long>::value) {
        return snprintf(outStr, size, "%" PRIu64, number) > 0;
    } else {
        return false;
    }
}

#pragma GCC diagnostic pop

#endif //PROJECT_NUMBERCONVERTOR_H
