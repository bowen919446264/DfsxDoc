//
// Created by wendachuan on 2018/12/4.
//

#include "PreviewSerialize.h"
#include "PreviewFrameSerialize.h"
#include "xutil/libxmlEx.h"
#include "avpub/Log.h"
#include "Preview.h"

using namespace xedit;

/**
 * 序列化对象
 * @param pPreview 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CPreviewSerialize::serialize(const IPreview* pPreview, const char* nodeName) {
    if (!pPreview || ! nodeName) return NULL;

    xmlNodePtr previewNode = xmlNewNode(NULL, BAD_CAST nodeName);
    if (!previewNode) return NULL;


    GSize size = pPreview->getPreviewSize();
    xmlNewPropEx(previewNode, BAD_CAST "width", size.nWidth);
    xmlNewPropEx(previewNode, BAD_CAST "height", size.nHeight);

    for (int i = 0; i < pPreview->getPreviewFrameCount(); i++) {
        PreviewFrame *pPreviewFrame = pPreview->getPreviewFrame(i);
        xmlNodePtr previewFrameNode = CPreviewFrameSerialize::serialize(*pPreviewFrame, "previewFrame");
        if (!previewFrameNode) goto failure;

        xmlAddChild(previewNode, previewFrameNode);
    }

    return previewNode;

failure:
    if (previewNode)
        xmlFreeNode(previewNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerPreview* CPreviewSerialize::deserialize(const xmlNodePtr& node) {
    GSize size;
    xmlGetPropEx(node, BAD_CAST "width", &size.nWidth);
    xmlGetPropEx(node, BAD_CAST "height", &size.nHeight);

    IInnerPreview *pPreview = new CPreview(size);
    PreviewFrame previewFrame;
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        if (!strcmp(nodeName, "previewFrame")) {
            if (!CPreviewFrameSerialize::deserialize(currentNode, &previewFrame))
                goto failure;
            pPreview->addPreviewFrame(previewFrame);
        }
        currentNode = currentNode->next;
    }

    return pPreview;

failure:
    SAFE_DELETE(pPreview);
    return NULL;
}