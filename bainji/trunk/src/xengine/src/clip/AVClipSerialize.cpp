//
// Created by wendachuan on 2018/12/5.
//

#include "AVClipSerialize.h"
#include "BaseClipSerialize.h"
#include "xutil/libxmlEx.h"
#include "AVClip.h"
#include "xutil/RationalSerialize.h"

using namespace xedit;

/**
 * 序列化对象
 * @param imageClip 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CAVClipSerialize::serialize(const IAVClip* pClip, const char* nodeName) {
    xmlNodePtr clipNode = CBaseClipSerialize::serialize(pClip, nodeName);
    if (!clipNode) return NULL;

    xmlNewChildEx(clipNode, NULL, BAD_CAST "streamIndex", pClip->getStreamIndex());

    xmlNodePtr offsetInMediaNode = CRationalSerializeHelper::serialize(pClip->getOffsetInMedia(), "offsetInMedia");
    if (!offsetInMediaNode) goto failure;
    xmlAddChild(clipNode, offsetInMediaNode);

    return clipNode;

failure:
    xmlFreeNode(clipNode);
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @return
 */
IInnerAVClip* CAVClipSerialize::deserialize(const xmlNodePtr& node) {
    IInnerAVClip* pAVClip = dynamic_cast<IInnerAVClip*>(CBaseClipSerialize::deserialize(node));
    if (!pAVClip) return NULL;

    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char *) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "streamIndex")) {
            if (!nodeContent) goto failure;

            int nStrmIdx = -1;
            if (!strToNumber(nodeContent, &nStrmIdx)) {
                goto failure;
            }
            pAVClip->setStreamIndex(nStrmIdx);
        } else if (!strcmp(nodeName, "offsetInMedia")) {
            Rational rOffsetInMedia;
            if (!CRationalSerializeHelper::deserialize(currentNode, &rOffsetInMedia))
                goto failure;
            pAVClip->setOffsetInMedia(rOffsetInMedia);
        }
        currentNode = currentNode->next;
    }

    return pAVClip;

failure:
    SAFE_DELETE(pAVClip);
    return NULL;
}