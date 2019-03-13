//
// Created by wendachuan on 2018/12/5.
//

#include "ImageSerialize.h"
#include "xutil/libxmlEx.h"

using namespace xedit;

/**
 * 序列化对象
 * @param media 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CImageSerialize::serialize(const Image& image, const char* nodeName) {
    if (!nodeName) return NULL;

    xmlNodePtr imageNode = xmlNewNode(NULL, BAD_CAST nodeName);
    if (!imageNode) return NULL;

    xmlNewChildEx(imageNode, NULL, BAD_CAST "type", (int)image.type);
    xmlNewChildEx(imageNode, NULL, BAD_CAST "width", image.size.nWidth);
    xmlNewChildEx(imageNode, NULL, BAD_CAST "height", image.size.nHeight);
    xmlNewChild(imageNode, NULL, BAD_CAST "path", BAD_CAST image.path.c_str());

    return imageNode;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutImage 输出对象
 */
bool CImageSerialize::deserialize(const xmlNodePtr& node, Image *pOutImage) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!nodeContent) {
            currentNode = currentNode->next;
            continue;
        }

        if (!strcmp(nodeName, "type")) {
            if (!strToNumber(nodeContent, (int*)&pOutImage->type))
                return false;
        } else if (!strcmp(nodeName, "width")) {
            if (!strToNumber(nodeContent, &pOutImage->size.nWidth))
                return false;
        } else if (!strcmp(nodeName, "height")) {
            if (!strToNumber(nodeContent, &pOutImage->size.nHeight))
                return false;
        } else if (!strcmp(nodeName, "path")) {
            pOutImage->path = nodeContent;
        }
        currentNode = currentNode->next;
    }

    return true;
}