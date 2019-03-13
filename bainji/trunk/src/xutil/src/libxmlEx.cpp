//
// Created by wendachuan on 2018/12/7.
//

#include "xutil/libxmlEx.h"

/**
 * 从字符串添加一个子节点
 * @param parent
 * @param newNodeStr
 * @return
 */
bool xmlAddChildFromString(xmlNodePtr parent, const char *newNodeStr) {
    xmlDocPtr newDoc = xmlReadMemory(newNodeStr, strlen(newNodeStr), NULL, NULL, 0);
    if (!newDoc) return false;

    xmlNodePtr newNode = xmlDocCopyNode(xmlDocGetRootElement(newDoc), parent->doc, 1);
    xmlFreeDoc(newDoc);
    if (!newNode) return false;

    xmlNodePtr addedNode = xmlAddChild(parent, newNode);
    if (!addedNode) {
        xmlFreeNode(newNode);
        return false;
    }
    return true;
}

/**
 * 获得节点的文本内容
 * @param node
 * @return
 */
xmlBufferPtr xmlGetNodeText(const xmlNodePtr node) {
    xmlBufferPtr nodeBuffer = xmlBufferCreate();
    if (xmlNodeDump(nodeBuffer, node->doc, node, 0, 0) < 0) {
        xmlBufferFree(nodeBuffer);
        return NULL;
    }

    return nodeBuffer;
}
