//
// Created by wendachuan on 2018/11/23.
//

#ifndef PROJECT_DICTIONARY_H
#define PROJECT_DICTIONARY_H

#include "xutil/IDictionary.h"
#include <map>
#include <string>
using namespace std;

class CDictionary: public IDictionary {
public:
    /**
    * 设置键值对
    * @param key
    * @param value
    */
    virtual void set(const char* key, const char* value);

    /**
     * 是否包含指定键值对
     * @param key
     * @return
     */
    virtual bool contains(const char* key) const;

    /**
     * 获取指定键的值
     * @param key
     * @return
     */
    virtual const char* get(const char* key) const;

    /**
     * 删除指定键值对
     * @param key
     */
    virtual void remove(const char* key);

private:
    map<string, string>     m_elems;
};


#endif //PROJECT_DICTIONARY_H
