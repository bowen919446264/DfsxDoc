//
// Created by wendachuan on 2018/12/5.
//

#include "PreviewFrameSerialize.h"
#include "xutil/libxmlEx.h"
#include "xutil/RationalSerialize.h"
#include "avpub/Log.h"

using namespace xedit;

/**
 * 序列化对象
 * @param previewFrame 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CPreviewFrameSerialize::serialize(const PreviewFrame& previewFrame, const char* nodeName) {
    if (!nodeName) return NULL;

    xmlNodePtr previewFrameNode = xmlNewNode(NULL, BAD_CAST nodeName);
    if (!previewFrameNode) return NULL;

    xmlNodePtr pathNode = NULL, timeOffsetNode = NULL;

    pathNode = xmlNewChild(previewFrameNode, NULL, BAD_CAST "path", BAD_CAST previewFrame.path.c_str());
    if (!pathNode) goto failure;

    timeOffsetNode = CRationalSerializeHelper::serialize(previewFrame.rTimeOffset, "timeOffset");
    if (!timeOffsetNode) goto failure;
    xmlAddChild(previewFrameNode, timeOffsetNode);

    return previewFrameNode;

failure:
    if (previewFrameNode)
        xmlFreeNode(previewFrameNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutValue 输出对象
 */
bool CPreviewFrameSerialize::deserialize(const xmlNodePtr& node, PreviewFrame *pOutValue) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char *) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!strcmp(nodeName, "path")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }
            pOutValue->path = nodeContent;
        } else if (!strcmp(nodeName, "timeOffset")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutValue->rTimeOffset)) {
                goto failure;
            }
        }
        currentNode = currentNode->next;
    }

    return true;

failure:
    return false;
}