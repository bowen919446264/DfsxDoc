//
// Created by wendachuan on 2018/12/4.
//

#include "BaseMediaSerialize.h"
#include "xutil/libxmlEx.h"
#include "DsFileInfoSerialize.h"

using namespace xedit;


/**
 * 序列化对象
 * @param pMedia 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CBaseMediaSerialize::serialize(const IInnerMedia *pMedia, const char* nodeName) {
    if (!pMedia || ! nodeName) return NULL;

    xmlNodePtr mediaNode = xmlNewNode(NULL, BAD_CAST nodeName);

    xmlNewPropEx(mediaNode, BAD_CAST "type", (int)pMedia->getMediaType());
    xmlNewChildEx(mediaNode, NULL, BAD_CAST "id", pMedia->getId());
    xmlNewChild(mediaNode, NULL, BAD_CAST "path", BAD_CAST pMedia->getPath());
    xmlNodePtr privDataNode = xmlNewChild(mediaNode, NULL, BAD_CAST "privData", NULL);

    SDsFileInfo fileInfo;
    pMedia->getDsFileInfo(&fileInfo);
    xmlNodePtr dsFileInfoNode = CDsFileInfoSerialize::serialize(fileInfo, "dsFileInfo");
    if (!dsFileInfoNode) {
        goto failure;
    }

    xmlAddChild(privDataNode, dsFileInfoNode);
    return mediaNode;

failure:
    xmlFreeNode(mediaNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerMedia* CBaseMediaSerialize::deserialize(const xmlNodePtr& node) {
    EMediaType mediaType = EMediaType_Invalid;
    xmlGetPropEx(node, BAD_CAST "type", (int*)&mediaType);

    if (mediaType == EMediaType_Invalid)
        return NULL;

    IInnerMedia *pMedia = createMedia(mediaType);
    if (!pMedia) return NULL;

    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "id")) {
            if (!nodeContent) goto failure;

            ID id = 0;
            if (!strToNumber(nodeContent, &id))
                goto failure;
            pMedia->setId(id);
        } else if (!strcmp(nodeName, "path")) {
            if (!nodeContent) goto failure;

            pMedia->setPath(nodeContent);
        } else if (!strcmp(nodeName, "privData")) {
            xmlNodePtr childCurrentNode = currentNode->xmlChildrenNode;
            while (childCurrentNode) {
                const char *childNodeName = (const char *) childCurrentNode->name;
                if (!strcmp(childNodeName, "dsFileInfo")) {
                    SDsFileInfo fileInfo = {sizeof(fileInfo)};
                    if (!CDsFileInfoSerialize::deserialize(childCurrentNode, &fileInfo)) {
                        goto failure;
                    }
                    pMedia->setDsFileInfo(fileInfo);
                }
                childCurrentNode = childCurrentNode->next;
            }
        }
        currentNode = currentNode->next;
    }

    return pMedia;

failure:
    delete pMedia;
    return NULL;
}