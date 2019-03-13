//
// Created by wendachuan on 2018/11/22.
//

#ifndef PROJECT_IDICTIONARY_H
#define PROJECT_IDICTIONARY_H

#include "avpub/Define.h"

/**
 * 字典
 */
class IDictionary {
public:
    virtual ~IDictionary() {}

    /**
     * 设置键值对
     * @param key
     * @param value
     */
    virtual void set(const char* key, const char* value) =0;

    /**
     * 是否包含指定键值对
     * @param key
     * @return
     */
    virtual bool contains(const char* key) const =0;

    /**
     * 获取指定键的值
     * @param key
     * @return
     */
    virtual const char* get(const char* key) const =0;

    /**
     * 删除指定键值对
     * @param key
     */
    virtual void remove(const char* key) =0;
};

/**
 * 创建字典对象
 * @return
 */
API_IMPORT_EXPORT
IDictionary* createDictionary();

#endif //PROJECT_IDICTIONARY_H
