//
// Created by wendachuan on 2018/12/11.
//

#include "DsFileInfoSerialize.h"
#include "xutil/libxmlEx.h"

using namespace xedit;

/**
 * 序列化对象
 * @param fileInfo 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CDsFileInfoSerialize::serialize(const SDsFileInfo& fileInfo, const char* nodeName) {
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST nodeName);

    xmlNewChild(node, NULL, BAD_CAST "m_wszFileName", BAD_CAST fileInfo.m_wszFileName);
    xmlNewChildEx(node, NULL, BAD_CAST "m_ui64Duration", fileInfo.m_ui64Duration);
    xmlNewChildEx(node, NULL, BAD_CAST "m_ui64FileSize", fileInfo.m_ui64FileSize);
    xmlNewChildEx(node, NULL, BAD_CAST "m_eFileType", (int)fileInfo.m_eFileType);
    xmlNewChildEx(node, NULL, BAD_CAST "m_lastModifyTime.dwHighDateTime", fileInfo.m_lastModifyTime.dwHighDateTime);
    xmlNewChildEx(node, NULL, BAD_CAST "m_lastModifyTime.dwLowDateTime", fileInfo.m_lastModifyTime.dwLowDateTime);
    xmlNewChildEx(node, NULL, BAD_CAST "m_iFileSubType", fileInfo.m_iFileSubType);
    xmlNewChildEx(node, NULL, BAD_CAST "m_iStreamIndex", fileInfo.m_iStreamIndex);
    xmlNewChildEx(node, NULL, BAD_CAST "m_eActualSound", (int)fileInfo.m_eActualSound);
    xmlNewChildEx(node, NULL, BAD_CAST "m_AudioFileCount", fileInfo.m_AudioFileCount);
    xmlNewChild(node, NULL, BAD_CAST "m_wszVideoIndexFile", BAD_CAST fileInfo.m_wszVideoIndexFile);

    xmlNodePtr videoInfoNode = CSDsVideoFileInfoSerialize::serialize(fileInfo.m_sVideoInfo, "m_sVideoInfo");
    xmlAddChild(node, videoInfoNode);

    xmlNodePtr audioInfoListNode = xmlNewChild(node, NULL, BAD_CAST "m_sAudioInfoList", NULL);
    for (int i = 0; i < fileInfo.m_AudioFileCount; i++) {
        xmlNodePtr audioInfoNode = CSDsAudioFileInfoSerialize::serialize(fileInfo.m_sAudioInfoList[i], "sAudioInfo");
        xmlAddChild(audioInfoListNode, audioInfoNode);
    }

    return node;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutFileInfo
 * @return
 */
bool CDsFileInfoSerialize::deserialize(const xmlNodePtr& node, SDsFileInfo *pOutFileInfo) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "m_wszFileName")) {
            if (!nodeContent) goto failure;
            strncpy(pOutFileInfo->m_wszFileName, nodeContent, sizeof(pOutFileInfo->m_wszFileName) - 1);
        } else if (!strcmp(nodeName, "m_ui64Duration")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_ui64Duration))
                goto failure;
        } else if (!strcmp(nodeName, "m_ui64FileSize")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_ui64FileSize))
                goto failure;
        } else if (!strcmp(nodeName, "m_ui64Duration")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_ui64Duration))
                goto failure;
        } else if (!strcmp(nodeName, "m_eFileType")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->m_eFileType))
                goto failure;
        } else if (!strcmp(nodeName, "m_lastModifyTime.dwHighDateTime")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_lastModifyTime.dwHighDateTime))
                goto failure;
        } else if (!strcmp(nodeName, "m_lastModifyTime.dwLowDateTime")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_lastModifyTime.dwLowDateTime))
                goto failure;
        } else if (!strcmp(nodeName, "m_iFileSubType")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_iFileSubType))
                goto failure;
        } else if (!strcmp(nodeName, "m_iStreamIndex")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_iStreamIndex))
                goto failure;
        } else if (!strcmp(nodeName, "m_eActualSound")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->m_eActualSound))
                goto failure;
        } else if (!strcmp(nodeName, "m_AudioFileCount")) {
            if (!nodeContent) goto failure;
            if (!strToNumber(nodeContent, &pOutFileInfo->m_AudioFileCount))
                goto failure;
        } else if (!strcmp(nodeName, "m_wszVideoIndexFile")) {
            if (nodeContent)
                strncpy(pOutFileInfo->m_wszVideoIndexFile, nodeContent, sizeof(pOutFileInfo->m_wszVideoIndexFile) - 1);
        } else if (!strcmp(nodeName, "m_sVideoInfo")) {
            if (!CSDsVideoFileInfoSerialize::deserialize(currentNode, &pOutFileInfo->m_sVideoInfo))
                goto failure;
        } else if (!strcmp(nodeName, "m_sAudioInfoList")) {
            xmlNodePtr childCurrentNode = currentNode->xmlChildrenNode;
            int nAudioInfoCount = 0;
            while (childCurrentNode) {
                const char *childNodeName = (const char *) childCurrentNode->name;
                if (!strcmp(childNodeName, "sAudioInfo")) {
                    if (!CSDsAudioFileInfoSerialize::deserialize(childCurrentNode, &pOutFileInfo->m_sAudioInfoList[nAudioInfoCount]))
                        goto failure;
                    nAudioInfoCount++;
                }
                childCurrentNode = childCurrentNode->next;
            }
        }
        currentNode = currentNode->next;
    }

    return true;

failure:
    return false;
}

/**
 * 序列化对象
 * @param fileInfo 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CSDsVideoFileInfoSerialize::serialize(const SDsVideoFileInfo& fileInfo, const char* nodeName) {
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST nodeName);
    xmlNewChildEx(node, NULL, BAD_CAST "m_left", fileInfo.m_left);
    xmlNewChildEx(node, NULL, BAD_CAST "m_top", fileInfo.m_top);
    xmlNewChildEx(node, NULL, BAD_CAST "m_iWidth", fileInfo.m_iWidth);
    xmlNewChildEx(node, NULL, BAD_CAST "m_iHeight", fileInfo.m_iHeight);
    xmlNewChildEx(node, NULL, BAD_CAST "m_eSurfaceFormat", (int) fileInfo.m_eSurfaceFormat);
    xmlNewChildEx(node, NULL, BAD_CAST "m_eAspectRatio", (int) fileInfo.m_eAspectRatio);
    xmlNewChildEx(node, NULL, BAD_CAST "m_eFrameRate", (int) fileInfo.m_eFrameRate);
    xmlNewChildEx(node, NULL, BAD_CAST "m_eScanMode", (int) fileInfo.m_eScanMode);
    xmlNewChildEx(node, NULL, BAD_CAST "m_ulComponentBitCount", fileInfo.m_ulComponentBitCount);
    xmlNewChildEx(node, NULL, BAD_CAST "m_ulDataRate", fileInfo.m_ulDataRate);
    return node;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutFileInfo
 * @return
 */
bool CSDsVideoFileInfoSerialize::deserialize(const xmlNodePtr& node, SDsVideoFileInfo *pOutFileInfo) {
    pOutFileInfo->m_size = sizeof(SDsVideoFileInfo);
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!nodeContent) {
            currentNode = currentNode->next;
            continue;
        }

        if (!strcmp(nodeName, "m_left")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->m_left))
                goto failure;
        } else if (!strcmp(nodeName, "m_top")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->m_top))
                goto failure;
        } else if (!strcmp(nodeName, "m_iWidth")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->m_iWidth))
                goto failure;
        } else if (!strcmp(nodeName, "m_iHeight")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->m_iHeight))
                goto failure;
        } else if (!strcmp(nodeName, "m_eSurfaceFormat")) {
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->m_eSurfaceFormat))
                goto failure;
        } else if (!strcmp(nodeName, "m_eAspectRatio")) {
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->m_eAspectRatio))
                goto failure;
        } else if (!strcmp(nodeName, "m_eFrameRate")) {
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->m_eFrameRate))
                goto failure;
        } else if (!strcmp(nodeName, "m_eScanMode")) {
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->m_eScanMode))
                goto failure;
        } else if (!strcmp(nodeName, "m_ulComponentBitCount")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->m_ulComponentBitCount))
                goto failure;
        } else if (!strcmp(nodeName, "m_ulDataRate")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->m_ulDataRate))
                goto failure;
        }
        currentNode = currentNode->next;
    }
    return true;

failure:
    return false;
}

/**
 * 序列化对象
 * @param fileInfo 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CSDsAudioFileInfoSerialize::serialize(const SDsAudioFileInfo& fileInfo, const char* nodeName) {
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST nodeName);
    xmlNewChildEx(node, NULL, BAD_CAST "eChannelType", (int)fileInfo.eChannelType);
    xmlNewChildEx(node, NULL, BAD_CAST "eDataType", (int)fileInfo.eDataType);
    xmlNewChildEx(node, NULL, BAD_CAST "ulSamplesPerSec", fileInfo.ulSamplesPerSec);
    xmlNewChildEx(node, NULL, BAD_CAST "ulBitsPerSample", fileInfo.ulBitsPerSample);
    xmlNewChildEx(node, NULL, BAD_CAST "ulValidBitsPerSample", fileInfo.ulValidBitsPerSample);
    xmlNewChild(node, NULL, BAD_CAST "m_wszFileName", BAD_CAST fileInfo.m_wszFileName);
    return node;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutFileInfo
 * @return
 */
bool CSDsAudioFileInfoSerialize::deserialize(const xmlNodePtr& node, SDsAudioFileInfo *pOutFileInfo) {
    pOutFileInfo->m_size = sizeof(SDsAudioFileInfo);
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!nodeContent) {
            currentNode = currentNode->next;
            continue;
        }

        if (!strcmp(nodeName, "eChannelType")) {
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->eChannelType))
                goto failure;
        } else if (!strcmp(nodeName, "eDataType")) {
            if (!strToNumber(nodeContent, (int*)&pOutFileInfo->eDataType))
                goto failure;
        } else if (!strcmp(nodeName, "ulSamplesPerSec")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->ulSamplesPerSec))
                goto failure;
        } else if (!strcmp(nodeName, "ulBitsPerSample")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->ulBitsPerSample))
                goto failure;
        } else if (!strcmp(nodeName, "ulValidBitsPerSample")) {
            if (!strToNumber(nodeContent, &pOutFileInfo->ulValidBitsPerSample))
                goto failure;
        } else if (!strcmp(nodeName, "m_wszFileName")) {
            strncpy(pOutFileInfo->m_wszFileName, nodeContent, sizeof(pOutFileInfo->m_wszFileName) - 1);
        }
        currentNode = currentNode->next;
    }
    return true;

failure:
    return false;
}