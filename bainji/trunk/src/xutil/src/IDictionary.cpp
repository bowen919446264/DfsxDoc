//
// Created by wendachuan on 2018/11/23.
//

#include "xutil/IDictionary.h"
#include "Dictionary.h"

/**
 * 创建字典对象
 * @return
 */
IDictionary* createDictionary() {
    return new CDictionary();
}