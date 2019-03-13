//
// Created by wendachuan on 2018/11/20.
//

#include "ImageMediaSerialize.h"
#include "BaseMediaSerialize.h"
#include "xutil/NumberConvertor.h"
#include "xutil/libxmlEx.h"
#include "ImageMedia.h"
#include "../PreviewSerialize.h"

using namespace xedit;

/**
 * 序列化对象
 * @param pMedia 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CImageMediaSerialize::serialize(const IInnerImageMedia *pMedia, const char* nodeName) {
    if (!pMedia || !nodeName) return NULL;

    xmlNodePtr previewNode = NULL;
    IPreview *pPreview = NULL;

    xmlNodePtr mediaNode = CBaseMediaSerialize::serialize(pMedia, nodeName);
    if (!mediaNode) return NULL;

    xmlNewChildEx(mediaNode, NULL, BAD_CAST "imageType", (int)pMedia->getImageType());
    xmlNewChildEx(mediaNode, NULL, BAD_CAST "width", pMedia->getSize().nWidth);
    xmlNewChildEx(mediaNode, NULL, BAD_CAST "height", pMedia->getSize().nHeight);

    pPreview = pMedia->getPreview();
    if (pPreview) {
        previewNode = CPreviewSerialize::serialize(pPreview, "preview");
        if (!previewNode) goto failure;
        xmlAddChild(mediaNode, previewNode);
    }

    return mediaNode;

failure:
    if (mediaNode)
        xmlFreeNode(mediaNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerImageMedia* CImageMediaSerialize::deserialize(const xmlNodePtr& node) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    IInnerImageMedia *pMedia = dynamic_cast<IInnerImageMedia*>(CBaseMediaSerialize::deserialize(node));
    if (!pMedia) return NULL;

    IInnerPreview *pPreview = NULL;
    GSize size;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "imageType")) {
            if (!nodeContent) goto failure;

            int nImageType = 0;
            if (!strToNumber(nodeContent, &nImageType)) {
                goto failure;
            }
            pMedia->setImageType((EImageType)nImageType);
        } else if (!strcmp(nodeName, "width")) {
            if (!nodeContent) goto failure;

            if (!strToNumber(nodeContent, &size.nWidth)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "height")) {
            if (!nodeContent) goto failure;

            if (!strToNumber(nodeContent, &size.nHeight)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "preview")) {
            SAFE_DELETE(pPreview);
            pPreview = CPreviewSerialize::deserialize(currentNode);
            if (!pPreview) {
                goto failure;
            }
            pMedia->setPreview(pPreview);
        }
        currentNode = currentNode->next;
    }

    pMedia->setSize(size);
    return pMedia;

failure:
    SAFE_DELETE(pPreview);
    delete pMedia;
    return NULL;
}