//
// Created by wendachuan on 2018/11/20.
//

#ifndef PROJECT_STDUTIL_H
#define PROJECT_STDUTIL_H

#include "avpub/Define.h"
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <deque>
using namespace std;

/**
 * 释放vector元素
 * @tparam T
 * @param pVector
 */
API_IMPORT_EXPORT
template <class T>
void destoryVector(vector<T*> *pVector) {
    while(!pVector->empty()) {
        T* object = pVector->back();
        pVector->pop_back();
        delete object;
    }
}

/**
 * 释放queue元素
 * @tparam T
 * @param pQueue
 */
API_IMPORT_EXPORT
template <class T>
void destoryQueue(queue<T*> *pQueue) {
    while(!pQueue->empty()) {
        T* object = pQueue->front();
        pQueue->pop();
        delete object;
    }
}

/**
 * 释放stack元素
 * @tparam T
 * @param pStack
 */
API_IMPORT_EXPORT
template <class T>
void destoryStack(stack<T*> *pStack) {
    while(!pStack->empty()) {
        T* object = pStack->top();
        pStack->pop();
        delete object;
    }
}

/**
 * 释放map元素
 * @tparam T
 * @param pMap
 */
API_IMPORT_EXPORT
template <class K, class V>
void destoryMap(map<K, V*> *pMap) {
    while(!pMap->empty()) {
        typename map<K, V*>::iterator it = pMap->begin();
        delete it->second;
        pMap->erase(it);
    }
}

/**
 * 释放deque元素
 * @tparam T
 * @param pQueue
 */
API_IMPORT_EXPORT
template <class T>
void destoryDeque(deque<T*> *pQueue) {
    while(!pQueue->empty()) {
        T* object = pQueue->back();
        pQueue->pop_back();
        delete object;
    }
}


#endif //PROJECT_STDUTIL_H
