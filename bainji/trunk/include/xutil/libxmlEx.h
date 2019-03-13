//
// Created by wendachuan on 2018/11/20.
//

#ifndef PROJECT_LIBXMLEX_H
#define PROJECT_LIBXMLEX_H

#include <libxml/tree.h>
#include "NumberConvertor.h"
#include "StdUtil.h"

#ifndef INT_BUF_SIZE
#define INT_BUF_SIZE 21
#endif

/**
 * 创建一个数值型节点
 * @tparam Number
 * @param parent 父节点
 * @param ns 节点命名空间
 * @param name 节点名称
 * @param value 节点值
 * @return
 */
template <typename Number>
XMLPUBFUN xmlNodePtr XMLCALL
		xmlNewChildEx (xmlNodePtr parent,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 Number value) {
    char tmp[INT_BUF_SIZE] = {0};
    if (!numberToStr(value, tmp, INT_BUF_SIZE)) {
        return NULL;
    }

    return xmlNewChild(parent, ns, name, BAD_CAST tmp);
}

/**
 * 创建一个数值型属性
 * @tparam Number
 * @param node 节点
 * @param propName 属性名称
 * @param propValue 属性值
 * @return
 */
template <typename Number>
XMLPUBFUN xmlAttrPtr XMLCALL
        xmlNewPropEx (xmlNodePtr node,
                   const xmlChar *propName,
                   Number propValue) {
    char tmp[INT_BUF_SIZE] = {0};
    if (!numberToStr(propValue, tmp, INT_BUF_SIZE)) {
        return NULL;
    }

    return xmlNewProp(node, propName, BAD_CAST tmp);
}

/**
 * 获得一个数值型属性的值
 * @tparam Number
 * @param node 节点
 * @param propName 属性名称
 * @param pOutPropValue 属性输出值
 * @return
 */
template <typename Number>
XMLPUBFUN bool XMLCALL
xmlGetPropEx (const xmlNodePtr node,
              const xmlChar *propName,
              Number *pOutPropValue) {
    xmlChar* propValue = xmlGetProp(node, propName);
    if (!propValue) return false;

    return strToNumber((char*)propValue, pOutPropValue);
}

/**
 * 载入数组
 * @tparam T
 * @param arrayNode 数组xml节点
 * @param itemNodeName 元素节点名称
 * @param pOutItemVec 输出数组
 * @param loadItemFunc 元素节点载入函数
 * @return
 */
template <typename T>
StatusCode xmlLoadArray(const xmlNodePtr arrayNode, const char* itemNodeName, std::vector<T*> *pOutItemVec, StatusCode (*xmlLoadItemFunc)(const xmlNodePtr itemNode, T **ppOutItem)) {
    StatusCode code;
    xmlNodePtr currentNode = arrayNode->xmlChildrenNode;

    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        if (!strcmp(nodeName, itemNodeName)) {
            T* pItem = NULL;
            code = xmlLoadItemFunc(currentNode, &pItem);
            if (FAILED(code)) {
                destoryVector(pOutItemVec);
                return code;
            }
            pOutItemVec->push_back(pItem);
        }
        currentNode = currentNode->next;
    }
    return 0;
}

/**
 * 从字符串添加一个子节点
 * @param parent
 * @param newNodeStr
 * @return
 */
API_IMPORT_EXPORT
bool xmlAddChildFromString(xmlNodePtr parent, const char *newNodeStr);

/**
 * 获得节点的文本内容
 * @param node
 * @return
 */
API_IMPORT_EXPORT
xmlBufferPtr xmlGetNodeText(const xmlNodePtr node);

#endif //PROJECT_LIBXMLEX_H
