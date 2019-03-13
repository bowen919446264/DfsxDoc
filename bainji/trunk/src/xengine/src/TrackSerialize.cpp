//
// Created by wendachuan on 2018/12/5.
//

#include "TrackSerialize.h"
#include "xutil/libxmlEx.h"
#include "xengine/IXEngine.h"
#include "avpub/Log.h"
#include "avpub/StatusCode.h"
#include "clip/AVClipSerialize.h"
#include "clip/ImageClipSerialize.h"
#include "clip/SVGClipSerialize.h"
#include "Track.h"

using namespace xedit;

/**
 * 载入切片
 * @param clipNode
 * @param ppOutClip
 * @return
 */
StatusCode CTrackSerialize::xmlLoadClip(const xmlNodePtr clipNode, IInnerClip **ppOutClip) {
    EClipType clipType = EClipType_Invalid;
    xmlGetPropEx(clipNode, BAD_CAST "type", (int*)&clipType);
    IInnerClip *pClip = NULL;
    switch (clipType) {
        case EClipType_Video:
        case EClipType_Audio:
            pClip = CAVClipSerialize::deserialize(clipNode);
            break;
        case EClipType_Image:
            pClip = CImageClipSerialize::deserialize(clipNode);
            break;
        case EClipType_SVG:
            pClip = CSVGClipSerialize::deserialize(clipNode);
        default:
            break;
    }

    if (pClip) {
        *ppOutClip = pClip;
        return AV_OK;
    } else {
        return AV_OTHER_ERROR;
    }
}

/**
 * 创建切片节点
 * @param pClip
 * @return
 */
xmlNodePtr CTrackSerialize::xmlCreateClipNode(const IClip *pClip) {
    switch (pClip->getType()) {
        case EClipType_Audio:
        case EClipType_Video:
            return CAVClipSerialize::serialize((dynamic_cast<const IAVClipPtr>(pClip)));
        case EClipType_Image:
            return CImageClipSerialize::serialize((dynamic_cast<const IImageClipPtr>(pClip)));
        case EClipType_SVG:
            return CSVGClipSerialize::serialize((dynamic_cast<const ISVGClipPtr>(pClip)));
        default:
            return NULL;
    }
}

/**
 * 创建一个clips节点
 * @param pTrack
 * @return
 */
xmlNodePtr CTrackSerialize::xmlCreateClipsNode(const ITrack *pTrack) {
    xmlNodePtr clipsNode = NULL, clipNode = NULL;
    clipsNode = xmlNewNode(NULL, BAD_CAST "clips");
    if (!clipsNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(clips) failed!");
        goto failure;
    }

    for(int i = 0; i < pTrack->getClipCount(); i++) {
        IClipPtr pClip = pTrack->getClip(i);
        clipNode = xmlCreateClipNode(pClip);
        if (!clipNode)
            goto failure;
        xmlAddChild(clipsNode, clipNode);
    }

    return clipsNode;

failure:
    if (clipsNode)
        xmlFreeNode(clipsNode);
    return NULL;
}

/**
 * 序列化对象
 * @param pTrack 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CTrackSerialize::serialize(const ITrack *pTrack, const char* nodeName) {
    if (!pTrack || !nodeName) return NULL;

    xmlNodePtr trackNode = xmlNewNode(NULL, BAD_CAST nodeName);
    if (!trackNode) return NULL;

    xmlNodePtr idNode = NULL, typeNode = NULL, idDisabledNode = NULL, clipsNode = NULL, clipNode = NULL;

    idNode = xmlNewChildEx(trackNode, NULL, BAD_CAST "id", pTrack->getId());
    if (!idNode) goto failure;

    typeNode = xmlNewChildEx(trackNode, NULL, BAD_CAST "type", (int)pTrack->getTrackType());
    if (!typeNode) goto failure;

    idDisabledNode = xmlNewChildEx(trackNode, NULL, BAD_CAST "isDisabled", pTrack->isDisabled());
    if (!idDisabledNode) goto failure;

    clipsNode = xmlCreateClipsNode(pTrack);
    if (!clipsNode) goto failure;
    xmlAddChild(trackNode, clipsNode);

    return trackNode;

failure:
    if (trackNode)
        xmlFreeNode(trackNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerTrack* CTrackSerialize::deserialize(const xmlNodePtr& node) {
    IInnerTrack *pTrack = new CTrack((CTimeLine*)IXEngine::getSharedInstance()->getTimeLine());
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char *) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!strcmp(nodeName, "id")) {
            if (!nodeContent) goto failure;

            ID id = 0;
            if (!strToNumber(nodeContent, &id))
                goto failure;
            pTrack->setId(id);
            break;
        }
        currentNode = currentNode->next;
    }

    currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!strcmp(nodeName, "type")) {
            if (!nodeContent) goto failure;

            ETrackType trackType = ETrackType_Invalid;
            if (!strToNumber(nodeContent, (int*)&trackType))
                goto failure;
            pTrack->setTrackType(trackType);
        } else if (!strcmp(nodeName, "isDisabled")) {
            if (!nodeContent) goto failure;

            bool bDisabled = 0;
            if (!strToNumber(nodeContent, &bDisabled))
                goto failure;
            if (bDisabled)
                pTrack->disable();
            else
                pTrack->enable();
        } else if (!strcmp(nodeName, "clips")) {
            // 获得切片信息
            vector<IInnerClip*> clipVec;
            StatusCode code = xmlLoadArray(currentNode, "clip", &clipVec, CTrackSerialize::xmlLoadClip);
            if (FAILED(code)) {
                destoryVector(&clipVec);
                goto failure;
            }

            for (vector<IInnerClip*>::iterator it = clipVec.begin(); it != clipVec.end(); it++) {
                pTrack->addClip(*it);
            }
        }
        currentNode = currentNode->next;
    }

    return pTrack;

failure:
    SAFE_DELETE(pTrack);
    return NULL;
}