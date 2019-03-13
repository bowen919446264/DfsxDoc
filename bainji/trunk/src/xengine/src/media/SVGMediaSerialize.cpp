//
// Created by wendachuan on 2018/12/7.
//

#include "SVGMediaSerialize.h"
#include "BaseMediaSerialize.h"
#include "xutil/libxmlEx.h"

using namespace xedit;

/**
 * 序列化对象
 * @param pMedia 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CSVGMediaSerialize::serialize(const IInnerSVGMedia *pMedia, const char* nodeName) {
    if (!pMedia || !nodeName) return NULL;

    xmlNodePtr mediaNode = CBaseMediaSerialize::serialize(pMedia, nodeName);
    if (!mediaNode) return NULL;

    const char* svg = pMedia->getSvg();
    if (svg) {
        xmlAddChildFromString(mediaNode, svg);
    }

    return mediaNode;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerSVGMedia* CSVGMediaSerialize::deserialize(const xmlNodePtr& node) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    IInnerSVGMedia *pMedia = dynamic_cast<IInnerSVGMedia*>(CBaseMediaSerialize::deserialize(node));
    if (!pMedia) return NULL;

    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        if (!strcmp(nodeName, "svg")) {
            xmlBufferPtr nodeBuffer = xmlGetNodeText(currentNode);
            if (!nodeBuffer)
                goto failure;
            pMedia->setSvg((char*)nodeBuffer->content);
            xmlBufferFree(nodeBuffer);
        }
        currentNode = currentNode->next;
    }

    return pMedia;

failure:
    delete pMedia;
    return NULL;
}