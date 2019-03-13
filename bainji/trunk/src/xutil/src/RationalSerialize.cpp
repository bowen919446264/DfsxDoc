//
// Created by wendachuan on 2018/11/19.
//

#include "xutil/RationalSerialize.h"
#include "xutil/NumberConvertor.h"
#include "xutil/libxmlEx.h"
#include <inttypes.h>
#include <string.h>

using namespace xedit;

/**
 * 序列化对象
 * @param object 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CRationalSerializeHelper::serialize(const Rational& object, const char* nodeName) {
    assert(nodeName);

    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST nodeName);
    xmlNewChildEx(node, NULL, BAD_CAST "numerator", object.nNum);
    xmlNewChildEx(node, NULL, BAD_CAST "denominator", object.nDen);

    return node;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutValue 输出对象
 */
bool CRationalSerializeHelper::deserialize(const xmlNodePtr& node, RationalPtr pOutValue) {
    int64_t nNum = 0, nDen = 0;

    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!strcmp(nodeName, "numerator")) {
            if (!strToNumber(nodeContent, &nNum)) {
                return false;
            }
        } else if (!strcmp(nodeName, "denominator")) {
            if (!strToNumber(nodeContent, &nDen)) {
                return false;
            }
        }
        currentNode = currentNode->next;
    }

    if (nDen == 0) {
        return false;
    }

    if (nDen) {
        pOutValue->nNum = nNum;
        pOutValue->nDen = nDen;
    } else {
        pOutValue->nNum = 0;
        pOutValue->nDen = 1;
    }

    return true;
}