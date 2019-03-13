//
// Created by wendachuan on 2018/11/23.
//

#include "Dictionary.h"

/**
  * 设置键值对
  * @param key
  * @param value
  */
void CDictionary::set(const char* key, const char* value) {
    m_elems[key] = value;
}

/**
  * 是否包含指定键值对
  * @param key
  * @return
  */
bool CDictionary::contains(const char* key) const {
    map<string, string>::const_iterator it = m_elems.find(key);
    return it != m_elems.end();
}

/**
 * 获取指定键的值
 * @param key
 * @return
 */
const char* CDictionary::get(const char* key) const {
    map<string, string>::const_iterator it = m_elems.find(key);
    if (it != m_elems.end()) {
        return it->second.c_str();
    } else {
        return NULL;
    }
}

/**
 * 删除指定键值对
 * @param key
 */
void CDictionary::remove(const char* key) {
    map<string, string>::iterator it = m_elems.find(key);
    if (it != m_elems.end()) {
        m_elems.erase(it);
    }
}