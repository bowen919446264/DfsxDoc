//
// Created by wendachuan on 2018/12/5.
//

#include "BaseClipSerialize.h"
#include "xutil/RationalSerialize.h"
#include "xutil/libxmlEx.h"
#include "../PreviewSerialize.h"

using namespace xedit;

/**
 * 序列化对象
 * @param pClip 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CBaseClipSerialize::serialize(const IClip *pClip, const char* nodeName) {
    xmlNodePtr clipNode = xmlNewNode(NULL, BAD_CAST nodeName);
    if (!clipNode) return NULL;

    xmlNodePtr offsetOnTrackNode = NULL, durationNode = NULL, thumbnailNode = NULL, previewNode = NULL;
    IPreview *pPreview = NULL;

    xmlNewPropEx(clipNode, BAD_CAST "type", (int)pClip->getType());
    xmlNewChildEx(clipNode, NULL, BAD_CAST "id", pClip->getId());
    xmlNewChildEx(clipNode, NULL, BAD_CAST "refMediaId", pClip->getRefMediaId());

    offsetOnTrackNode = CRationalSerializeHelper::serialize(pClip->getOffsetOnTrack(), "offsetOnTrack");
    if (!offsetOnTrackNode) goto failure;
    xmlAddChild(clipNode, offsetOnTrackNode);

    durationNode = CRationalSerializeHelper::serialize(pClip->getDuration(), "duration");
    if (!durationNode) goto failure;
    xmlAddChild(clipNode, durationNode);

//    pPreview = pClip->getPreview();
//    if (pPreview) {
//        previewNode = CPreviewSerialize::serialize(pPreview, "preview");
//        if (!previewNode) goto failure;
//        xmlAddChild(clipNode, previewNode);
//    }

    return clipNode;

failure:
    if (clipNode)
        xmlFreeNode(clipNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerClip* CBaseClipSerialize::deserialize(const xmlNodePtr& node) {
    EClipType clipType = EClipType_Invalid;
    IInnerPreview *pPreview = NULL;
    xmlGetPropEx(node, BAD_CAST "type", (int*)&clipType);
    IInnerClip *pClip = createClip(clipType);
    if (!pClip)
        return NULL;

    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "id")) {
            if (!nodeContent) goto failure;

            ID id = 0;
            if (!strToNumber(nodeContent, &id))
                goto failure;
            pClip->setId(id);
        } else if (!strcmp(nodeName, "refMediaId")) {
            if (!nodeContent) goto failure;

            ID refMediaId = 0;
            if (!strToNumber(nodeContent, &refMediaId))
                goto failure;
            pClip->setRefMediaId(refMediaId);
        } else if (!strcmp(nodeName, "offsetOnTrack")) {
            Rational rOffsetOnTrack;
            if (!CRationalSerializeHelper::deserialize(currentNode, &rOffsetOnTrack)) {
                goto failure;
            }
            pClip->setOffsetOnTrack(rOffsetOnTrack);
        } else if (!strcmp(nodeName, "duration")) {
            Rational rDuration;
            if (!CRationalSerializeHelper::deserialize(currentNode, &rDuration))
                goto failure;
            pClip->setDuration(rDuration);
//        } else if (!strcmp(nodeName, "preview")) {
//            SAFE_DELETE(pPreview);
//            pPreview = CPreviewSerialize::deserialize(currentNode);
//            if (!pPreview) {
//                goto failure;
//            }
//            pClip->setPreview(pPreview);
        }
        currentNode = currentNode->next;
    }

    return pClip;

failure:
    SAFE_DELETE(pClip);
    return NULL;
}