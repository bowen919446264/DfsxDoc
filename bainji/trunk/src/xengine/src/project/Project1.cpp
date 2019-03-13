//
// Created by wendachuan on 2018/11/7.
//

#include "Project1.h"
#include "avpub/StatusCode.h"
#include "avpub/Log.h"
#include "avpub/IAVGlobal.h"
#include "avpub/AVMemory.h"
#include "avpub/TSmartPtr.h"
#include <inttypes.h>
#include "xutil/NumberConvertor.h"
#include "xutil/RationalSerialize.h"
#include "xutil/StdUtil.h"
#include "xutil/libxmlEx.h"
#include "../media/AVMediaSerialize.h"
#include "../media/ImageMediaSerialize.h"
#include "../media/SVGMediaSerialize.h"
#include "../clip/ImageClipSerialize.h"
#include "../clip/AVClipSerialize.h"
#include "../clip/SVGClipSerialize.h"
#include "../TrackSerialize.h"

using namespace xedit;
using namespace libav;

CProject1::CProject1() {
    m_id = idGenerateOne();
}

CProject1::~CProject1() {
    destoryVector(&m_medias);
    destoryVector(&m_videoTracks);
    destoryVector(&m_audioTracks);
}

/**
 * 获得工程id
 * @return
 */
ID CProject1::getId() const {
    return m_id;
}

/**
 * 获得工程名称
 * @return
 */
const char* CProject1::getName() const {
    return m_strName.c_str();
}

/**
 * 设置工程名称
 * @param name
 */
void CProject1::setName(const char* name) {
    m_strName = name;
}

/**
 * 获得工程设置
 * @return
 */
const ProjectSetting& CProject1::getSetting() const {
    return m_setting;
}

/**
 * 设置工程设置
 * @return
 */
void CProject1::setSetting(const ProjectSetting& setting) {
    m_setting = setting;
}

/**
 * 保存工程
 * @param pOutputStream 工程输出流
 * @return
 */
StatusCode CProject1::save(IOutputStream *pOutputStream) {
    xmlDocPtr doc = NULL;
    xmlChar *projectStr = NULL;
    int size = 0;
    int nCount = 0;

    doc = xmlCreateDoc();
    if (!doc) {
        AVLOG(ELOG_LEVEL_ERROR, "创建xml文档失败!");
        goto failure;
    }

    xmlDocDumpMemory(doc, &projectStr, &size);
    if (!projectStr) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlDocDumpMemory失败!");
        goto failure;
    }

    nCount = pOutputStream->write(projectStr, size);
    if (nCount != size) {
        AVLOG(ELOG_LEVEL_ERROR, "工程写入到输出流失败!");
        goto failure;
    }

    xmlFree(projectStr);
    xmlFreeDoc(doc);
    return AV_OK;

failure:
    if (projectStr) xmlFree(projectStr);
    if (doc) xmlFreeDoc(doc);
    return AV_OTHER_ERROR;
}

/**
 * 载入工程
 * @param inProjectBuffer 工程buffer
 * @param nBufferSize projectBuffer长度
 * @return
 */
StatusCode CProject1::load(const char *inProjectBuffer, int nBufferSize) {
    return xmlLoadDoc(inProjectBuffer);
}

/**
 * 载入工程
 * @param pInputStream 工程输入流
 * @return
 */
StatusCode CProject1::load(IInputStream *pInputStream) {
    int64_t size = 0;
    uint8_t *buffer = NULL;
    StatusCode code = AV_OK;
    int count = 0;

    size = pInputStream->available();
    buffer = (uint8_t *)AVMalloc(size + 1);
    if (!buffer) {
        code = AV_NULL_PTR;
        goto failure;
    }

    count = pInputStream->read(buffer, size);
    if (count < 0) {
        AVLOG(ELOG_LEVEL_ERROR, "从流中读取数据失败!");
        goto failure;
    }

    buffer[count] = '\0';

    code = load((char*)buffer, count);
    if (code < 0) {
        AVLOG(ELOG_LEVEL_ERROR, "载入工程失败!");
        goto failure;
    }

    if (buffer) AVFree((void**)&buffer);
    return AV_OK;

failure:
    if (buffer) AVFree((void**)&buffer);
    return AV_OTHER_ERROR;
}

/**
 * 获得工程中媒体数量
 * @return
 */
int CProject1::getMediaCount() const {
    return m_medias.size();
}

/**
 * 获得指定序号的媒体
 * @param nIndex
 * @return
 */
IInnerMedia* CProject1::getMedia(int nIndex) const {
    if (nIndex < m_medias.size())
        return m_medias[nIndex];
    else
        return NULL;
}

/**
 * 添加媒体
 * @param media
 * @return
 */
StatusCode CProject1::addMedia(IInnerMedia *media) {
    m_medias.push_back(media);
    return AV_OK;
}

/**
 * 将指定序号的media从工程中移除并返回(不删除)
 * @param nIndex
 * @return
 */
IInnerMedia* CProject1::popMedia(int nIndex) {
    if (nIndex < m_medias.size()) {
        IInnerMedia *pMedia = m_medias[nIndex];
        m_medias.erase(m_medias.begin() + nIndex);
        return pMedia;
    } else
        return NULL;
}

/**
 * 删除所有媒体
 */
void CProject1::removeAllMedias() {
    destoryVector(&m_medias);
}

/**
 * 删除所有媒体，但不释放资源
 */
void CProject1::popAllMedias() {
    m_medias.clear();
}

/**
 * 删除指定序号的媒体
 * @param nIndex
 * @return
 */
//StatusCode CProject1::removeMedia(int nIndex) {
//    if (m_medias.size() < nIndex) {
//        IMedia *media = m_medias[nIndex];
//        m_medias.erase(m_medias.begin() + nIndex);
//        delete media;
//        return AV_OK;
//    } else {
//        return AV_OUT_OF_RANGE;
//    }
//}

/**
 * 获得轨道数量
 * @param trackType
 * @return
 */
int CProject1::getTrackCount(ETrackType trackType) const {
    switch (trackType) {
        case ETrackType_Video:
            return m_videoTracks.size();
        case ETrackType_Audio:
            return m_audioTracks.size();
        default:
            return 0;
    }
}

/**
 * 获得指定序号的轨道
 * @param trackType
 * @param nIndex
 * @return
 */
ITrack* CProject1::getTrack(ETrackType trackType, int nIndex) const {
    switch (trackType) {
        case ETrackType_Video:
            if (nIndex < m_videoTracks.size())
                return m_videoTracks[nIndex];
            else
                return NULL;
        case ETrackType_Audio:
            if (nIndex < m_audioTracks.size())
                return m_audioTracks[nIndex];
            else
                return NULL;
        default:
            return NULL;
    }
}

/**
 * 添加轨道
 * @param track
 * @return
 */
StatusCode CProject1::addTrack(IInnerTrack *track) {
    switch (track->getTrackType()) {
        case ETrackType_Video:
            m_videoTracks.push_back(track);
            return AV_OK;
        case ETrackType_Audio:
            m_audioTracks.push_back(track);
            return AV_OK;
        default:
            return AV_OTHER_ERROR;
    }
}

/**
 * 删除所有轨道
 */
void CProject1::removeAllTracks() {
    destoryVector(&m_videoTracks);
    destoryVector(&m_audioTracks);
}

/**
 * 删除所有轨道，但不释放资源
 */
void CProject1::popAllTracks() {
    m_videoTracks.clear();
    m_audioTracks.clear();
}

/**
* 创建工程xml文档
* @return
*/
xmlDocPtr CProject1::xmlCreateDoc() {
    xmlDocPtr doc = NULL;
    xmlNodePtr projectNode, nameNode, idNode, settingsNode, mediasNode, clipsNode, timelineNode;
    xmlAttrPtr versionAttr = NULL;
    vector<ITrack*> allTracks;

    // 创建一个xml文档
    doc = xmlNewDoc(BAD_CAST "1.0");
    if (!doc) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewDoc failed!");
        goto failure;
    }

    // 创建根节点
    projectNode = xmlNewNode(NULL, BAD_CAST "project");
    if (!projectNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(project) failed!");
        goto failure;
    }

    // 将根节点加入xml文档
    xmlDocSetRootElement(doc, projectNode);

    // 设置工程版本号
    versionAttr = xmlNewPropEx(projectNode, BAD_CAST "version", getVersion());
    if (!versionAttr) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewProp(version) failed!");
        goto failure;
    }

    // 创建name节点
    nameNode = xmlNewChild(projectNode, NULL, BAD_CAST "name", BAD_CAST m_strName.c_str());
    if (!nameNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(name) failed!");
        goto failure;
    }

    // 创建id节点
    idNode = xmlNewChildEx(projectNode, NULL, BAD_CAST "id", m_id);
    if (!idNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(id) failed!");
        goto failure;
    }

    // 创建settings节点
    settingsNode = xmlCreateSettingNode(m_setting);
    if (!settingsNode) {
        goto failure;
    }

    // 添加settings节点
    xmlAddChild(projectNode, settingsNode);

    // 创建medias节点
    mediasNode = xmlCreateMediasNode(m_medias);
    if (!mediasNode) {
        goto failure;
    }

    // 添加medias节点
    xmlAddChild(projectNode, mediasNode);

    // 创建timeline节点
    allTracks.resize(m_videoTracks.size() + m_audioTracks.size());
    merge(m_videoTracks.begin(), m_videoTracks.end(), m_audioTracks.begin(), m_audioTracks.end(), allTracks.begin());
    timelineNode = xmlCreateTimelineNode(allTracks);
    if (!timelineNode) {
        goto failure;
    }

    // 添加timeline节点
    xmlAddChild(projectNode, timelineNode);

    return doc;

failure:
    if (doc) {
        xmlFreeDoc(doc);
    }
    return NULL;
}

/**
 * 创建一个setting节点
 * @param projectSetting 工程设置
 * @return NULL表示创建失败，否则返回xml节点
 */
xmlNodePtr CProject1::xmlCreateSettingNode(const ProjectSetting& projectSetting) {
    xmlNodePtr settingsNode, audioOutputNode, videoOutputNode, mediasNode;

    settingsNode = xmlNewNode(NULL, BAD_CAST "settings");
    if (!settingsNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(settings) failed!");
        goto failure;
    }

    // 创建audioOutput节点
    audioOutputNode = xmlCreateAudioOutputNode(projectSetting);
    if (!audioOutputNode) goto failure;
    xmlAddChild(settingsNode, audioOutputNode);

    // 创建videoOutput节点
    videoOutputNode = xmlCreateVideoOutputNode(projectSetting);
    if (!videoOutputNode) goto failure;
    xmlAddChild(settingsNode, videoOutputNode);

    return settingsNode;

failure:
    if (settingsNode) {
        xmlFreeNode(settingsNode);
    }
    return NULL;
}

/**
 * 创建一个audioOutput节点
 * @param projectSetting
 * @return
 */
xmlNodePtr CProject1::xmlCreateAudioOutputNode(const ProjectSetting& projectSetting) {
    xmlNodePtr audioOutputNode, sampleRateNode, bitsPerSampleNode, channelCountNode, channelLayoutNode, formatNode;

    audioOutputNode = xmlNewNode(NULL, BAD_CAST "audioOutput");
    if (!audioOutputNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(audioOutput) failed!");
        goto failure;
    }

    sampleRateNode = xmlNewChildEx(audioOutputNode, NULL, BAD_CAST "sampleRate", projectSetting.nSampleRate);
    if (!sampleRateNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(sampleRate) failed!");
        goto failure;
    }

    bitsPerSampleNode = xmlNewChildEx(audioOutputNode, NULL, BAD_CAST "bitsPerSample", projectSetting.nBitsPerSample);
    if (!bitsPerSampleNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(bitsPerSample) failed!");
        goto failure;
    }

    channelCountNode = xmlNewChildEx(audioOutputNode, NULL, BAD_CAST "channelCount", projectSetting.nChannelCount);
    if (!channelCountNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(channelCount) failed!");
        goto failure;
    }

    channelLayoutNode = xmlNewChildEx(audioOutputNode, NULL, BAD_CAST "channelLayout", projectSetting.nChannelLayout);
    if (!channelLayoutNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(channelLayout) failed!");
        goto failure;
    }

    formatNode = xmlNewChildEx(audioOutputNode, NULL, BAD_CAST "format", (int)projectSetting.eSampleFormat);
    if (!formatNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(channelLayout) failed!");
        goto failure;
    }

    return audioOutputNode;

failure:
    if (audioOutputNode) {
        xmlFreeNode(audioOutputNode);
    }
    return NULL;
}

/**
 * 创建一个videoOutput节点
 * @param projectSetting
 * @return
 */
xmlNodePtr CProject1::xmlCreateVideoOutputNode(const ProjectSetting& projectSetting) {
    xmlNodePtr videoOutputNode, widthNode, heightNode, framerateNode, aspectRatioNode, formatNode, isInterlacedNode, isTopFieldFirstNode;

    videoOutputNode = xmlNewNode(NULL, BAD_CAST "videoOutput");
    if (!videoOutputNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(videoOutput) failed!");
        goto failure;
    }

    widthNode = xmlNewChildEx(videoOutputNode, NULL, BAD_CAST "width", projectSetting.nWidth);
    if (!widthNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(sampleRate) failed!");
        goto failure;
    }

    heightNode = xmlNewChildEx(videoOutputNode, NULL, BAD_CAST "height", projectSetting.nHeight);
    if (!heightNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(bitsPerSample) failed!");
        goto failure;
    }

    framerateNode = CRationalSerializeHelper::serialize(projectSetting.rFrameRate, "framerate");
    if (!framerateNode) {
        AVLOG(ELOG_LEVEL_ERROR, "序列化帧率失败!");
        goto failure;
    }
    xmlAddChild(videoOutputNode, framerateNode);

    formatNode = xmlNewChildEx(videoOutputNode, NULL, BAD_CAST "format", (int)projectSetting.ePixFormat);
    if (!formatNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(format) failed!");
        goto failure;
    }

    aspectRatioNode = CRationalSerializeHelper::serialize(projectSetting.rAspectRatio, "aspectRatio");
    if (!aspectRatioNode) {
        AVLOG(ELOG_LEVEL_ERROR, "序列化aspectRatio失败!");
        goto failure;
    }
    xmlAddChild(videoOutputNode, aspectRatioNode);

    isInterlacedNode = xmlNewChildEx(videoOutputNode, NULL, BAD_CAST "isInterlaced", projectSetting.bInterlaced);
    if (!isInterlacedNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(isInterlaced) failed!");
        goto failure;
    }

    isTopFieldFirstNode = xmlNewChildEx(videoOutputNode, NULL, BAD_CAST "isTopFieldFirst", projectSetting.bTopFieldFirst);
    if (!isTopFieldFirstNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewChild(isTopFieldFirst) failed!");
        goto failure;
    }

    return videoOutputNode;

failure:
    if (videoOutputNode) {
        xmlFreeNode(videoOutputNode);
    }
    return NULL;
}


/**
 * 载入工程xml文档
 * @param str
 * @return
 */
StatusCode CProject1::xmlLoadDoc(const char* str) {
    xmlDocPtr doc = NULL;
    xmlNodePtr projectNode, currentNode;
    xmlChar* xmlTmpChar = NULL;
    int nVersion = 0;
    StatusCode code = AV_OK;

    // 分析xml文档
    doc = xmlParseDoc(BAD_CAST str);
    if (!doc) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlParseDoc失败!");
        goto failure;
    }

    projectNode = xmlDocGetRootElement(doc);
    if (!projectNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlDocGetRootElement失败!");
        goto failure;
    }

    if (xmlStrcmp(projectNode->name, BAD_CAST "project")) {
        AVLOG(ELOG_LEVEL_ERROR, "缺少project节点!");
        goto failure;
    }

    // 获得工程版本号
    xmlTmpChar = xmlGetProp(projectNode, BAD_CAST "version");
    if (!xmlTmpChar) {
        AVLOG(ELOG_LEVEL_ERROR, "project节点缺少version属性!");
        goto failure;
    }

    if (!strToNumber((const char*)xmlTmpChar, &nVersion)) {
        AVLOG(ELOG_LEVEL_ERROR, "project节点的version属性不是整数!");
        goto failure;
    }

    assert(nVersion == 1);
    if (nVersion != 1) {
        AVLOG(ELOG_LEVEL_ERROR, "工程版本号不匹配!");
        goto failure;
    }

    currentNode = projectNode->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *)currentNode->name;
        const char *nodeContent = (const char*)xmlNodeListGetString(doc, currentNode->xmlChildrenNode, 1);
        if (!strcmp(nodeName, "name")) {
            // 获得工程名称
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "工程名称为空!");
                goto failure;
            }
            m_strName = nodeContent;
        } else if (!strcmp(nodeName, "id")) {
            // 获得工程id
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "工程id为空!");
                goto failure;
            }

            ID id = 0;
            if (!strToNumber(nodeContent, &id) || id <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "非法的工程id: %" PRId64 "!", id);
                goto failure;
            }
            m_id = id;
        } else if (!strcmp(nodeName, "settings")) {
            // 获得工程设置
            ProjectSetting projectSetting;
            memset(&projectSetting, 0, sizeof(ProjectSetting));
            code = xmlLoadSetting(currentNode, &projectSetting);
            if (FAILED(code)) goto failure;

            m_setting = projectSetting;
        } else if (!strcmp(nodeName, "medias")) {
            // 获得媒体信息
            vector<IInnerMedia*> mediaVec;
            code = xmlLoadArray(currentNode, "media", &mediaVec, CProject1::xmlLoadMedia);
            if (FAILED(code)) {
                destoryVector(&mediaVec);
                goto failure;
            }

            m_medias = mediaVec;
        } else if (!strcmp(nodeName, "timeline")) {
            // 获得时间线信息
            code = xmlLoadTimeLine(currentNode);
            if (FAILED(code)) goto failure;
        }
        currentNode = currentNode->next;
    }

    xmlFreeDoc(doc);
    return AV_OK;

failure:
    if (doc) xmlFreeDoc(doc);
    return AV_OTHER_ERROR;
}

/**
 * 载入工程设置
 * @param settingNode
 * @param pOutSetting
 * @return
 */
StatusCode CProject1::xmlLoadSetting(const xmlNodePtr settingNode, ProjectSetting *pOutSetting) {
    StatusCode code = AV_OK;
    xmlNodePtr currentNode = settingNode->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        if (!strcmp(nodeName, "audioOutput")) {
            code = xmlLoadAudioOutput(currentNode, pOutSetting);
            if (FAILED(code)) return code;
        } else if (!strcmp(nodeName, "videoOutput")) {
            code = xmlLoadVideoOutput(currentNode, pOutSetting);
            if (FAILED(code)) return code;
        }
        currentNode = currentNode->next;
    }
    return AV_OK;
}

/**
 * 载入工程音频输出设置
 * @param audioOutputNode
 * @param pOutSetting
 * @return
 */
StatusCode CProject1::xmlLoadAudioOutput(const xmlNodePtr audioOutputNode, ProjectSetting *pOutSetting) {
    xmlNodePtr currentNode = audioOutputNode->xmlChildrenNode;

    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*)xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!nodeContent) {
            currentNode = currentNode->next;
            continue;
        }

        if (!strcmp(nodeName, "sampleRate")) {
            long nSampleRate = 0;
            if (!strToNumber(nodeContent, &nSampleRate) || nSampleRate <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "采样率不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->nSampleRate = nSampleRate;
        } else if (!strcmp(nodeName, "bitsPerSample")) {
            long nBitsPerSample = 0;
            if (!strToNumber(nodeContent, &nBitsPerSample) || nBitsPerSample <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "采样大小不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->nBitsPerSample = nBitsPerSample;
        } else if (!strcmp(nodeName, "channelCount")) {
            long nChannelCount = 0;
            if (!strToNumber(nodeContent, &nChannelCount) || nChannelCount <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "声道数不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->nChannelCount = nChannelCount;
        } else if (!strcmp(nodeName, "channelLayout")) {
            long nChannelLayout = 0;
            if (!strToNumber(nodeContent, &nChannelLayout) || nChannelLayout < 0) {
                AVLOG(ELOG_LEVEL_ERROR, "声道布局不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->nChannelLayout = nChannelLayout;
        } else if (!strcmp(nodeName, "format")) {
            long nFormat = 0;
            if (!strToNumber(nodeContent, &nFormat) || nFormat <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "采样格式不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->eSampleFormat = (ESampleFormat) nFormat;
        }
        currentNode = currentNode->next;
    }
    return AV_OK;
}

/**
 * 载入工程视频输出设置
 * @param videoOutputNode
 * @param pOutSetting
 * @return
 */
StatusCode CProject1::xmlLoadVideoOutput(const xmlNodePtr videoOutputNode, ProjectSetting *pOutSetting) {
    StatusCode code = AV_OK;
    xmlNodePtr currentNode = videoOutputNode->xmlChildrenNode;

    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*)xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "width")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                return AV_OTHER_ERROR;
            }

            long nWidth = 0;
            if (!strToNumber(nodeContent, &nWidth) || nWidth <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "像素宽度不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->nWidth = nWidth;
        } else if (!strcmp(nodeName, "height")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                return AV_OTHER_ERROR;
            }

            long nHeight = 0;
            if (!strToNumber(nodeContent, &nHeight) || nHeight <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "像素高度不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->nHeight = nHeight;
        } else if (!strcmp(nodeName, "aspectRatio")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutSetting->rAspectRatio)) {
                AVLOG(ELOG_LEVEL_ERROR, "反序列化aspectRatio失败！");
                return AV_OTHER_ERROR;
            }
        } else if (!strcmp(nodeName, "framerate")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutSetting->rFrameRate)) {
                AVLOG(ELOG_LEVEL_ERROR, "反序列化framerate失败！");
                return AV_OTHER_ERROR;
            }
        } else if (!strcmp(nodeName, "format")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                return AV_OTHER_ERROR;
            }

            long nFormat = 0;
            if (!strToNumber(nodeContent, &nFormat) || nFormat <= 0) {
                AVLOG(ELOG_LEVEL_ERROR, "像素格式不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->ePixFormat = (EPixFormat) nFormat;
        } else if (!strcmp(nodeName, "isInterlaced")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                return AV_OTHER_ERROR;
            }

            long nInterlaced = 0;
            if (!strToNumber(nodeContent, &nInterlaced) || nInterlaced < 0) {
                AVLOG(ELOG_LEVEL_ERROR, "isInterlaced不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->bInterlaced = (bool) nInterlaced;
        } else if (!strcmp(nodeName, "isTopFieldFirst")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                return AV_OTHER_ERROR;
            }

            long nTopFieldFirst = 0;
            if (!strToNumber(nodeContent, &nTopFieldFirst) || nTopFieldFirst < 0) {
                AVLOG(ELOG_LEVEL_ERROR, "isTopFieldFirst不正确！");
                return AV_OTHER_ERROR;
            }
            pOutSetting->bTopFieldFirst = (bool) nTopFieldFirst;
        }
        currentNode = currentNode->next;
    }
    return AV_OK;
}

/**
 * 载入媒体信息
 * @param mediaNode
 * @param ppOutMedia
 * @return
 */
StatusCode CProject1::xmlLoadMedia(const xmlNodePtr mediaNode, IInnerMedia **ppOutMedia) {
    StatusCode code = AV_OK;
    EMediaType mediaType = EMediaType_Invalid;
    xmlGetPropEx(mediaNode, BAD_CAST "type", (int*)&mediaType);
    IInnerMedia *pMedia = NULL;
    switch (mediaType) {
        case EMediaType_AV:
            pMedia = CAVMediaSerialize::deserialize(mediaNode);
            break;
        case EMediaType_Image:
            pMedia = CImageMediaSerialize::deserialize(mediaNode);
            break;
        case EMediaType_SVG:
            pMedia = CSVGMediaSerialize::deserialize(mediaNode);
            break;
        default:
            break;
    }

    if (pMedia) {
        *ppOutMedia = pMedia;
        return AV_OK;
    } else {
        return AV_OTHER_ERROR;
    }
}


/**
 * 创建一个medias节点
 * @param mediaVector
 * @return
 */
xmlNodePtr CProject1::xmlCreateMediasNode(const std::vector<IInnerMedia*> mediaVector) {
    xmlNodePtr mediasNode = NULL, mediaNode = NULL;
    mediasNode = xmlNewNode(NULL, BAD_CAST "medias");
    if (!mediasNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(medias) failed!");
        goto failure;
    }

    for(vector<IInnerMedia*>::const_iterator it = mediaVector.begin(); it != mediaVector.end(); it++) {
        IMedia *media = *it;
        switch (media->getMediaType()) {
            case EMediaType_AV:
                mediaNode = CAVMediaSerialize::serialize((dynamic_cast<IInnerAVMedia*>(media)));
                break;
            case EMediaType_Image:
                mediaNode = CImageMediaSerialize::serialize((dynamic_cast<IInnerImageMedia*>(media)));
                break;
            case EMediaType_SVG:
                mediaNode = CSVGMediaSerialize::serialize((dynamic_cast<IInnerSVGMedia*>(media)));
                break;
            default:
                break;
        }
        if (!mediaNode)
            goto failure;
        xmlAddChild(mediasNode, mediaNode);
    }

    return mediasNode;

failure:
    if (mediasNode)
        xmlFreeNode(mediasNode);
    return NULL;
}

/**
 * 载入轨道
 * @param trackNode
 * @param ppOutTrack
 * @return
 */
StatusCode CProject1::xmlLoadTrack(const xmlNodePtr trackNode, ITrack **ppOutTrack) {
    ITrackPtr pTrack = CTrackSerialize::deserialize(trackNode);
    if (pTrack) {
        *ppOutTrack = pTrack;
        return AV_OK;
    } else {
        return AV_OTHER_ERROR;
    }
}


/**
 * 创建timeline节点
 * @param trackVector
 * @return
 */
xmlNodePtr CProject1::xmlCreateTimelineNode(const std::vector<ITrack*> trackVector) {
    xmlNodePtr timelineNode = xmlNewNode(NULL, BAD_CAST "timeline");
    if (!timelineNode) return NULL;

    xmlNodePtr tracksNode = xmlCreateTracksNode(trackVector);
    if (!tracksNode) {
        xmlFreeNode(timelineNode);
        return NULL;
    }

    xmlAddChild(timelineNode, tracksNode);
    return timelineNode;
}

/**
 * 载入时间线
 * @param timelineNode
 * @return
 */
StatusCode CProject1::xmlLoadTimeLine(const xmlNodePtr timelineNode) {
    xmlNodePtr currentNode = timelineNode->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *)currentNode->name;
        if (!strcmp(nodeName, "tracks")) {
            vector<ITrack*> trackVec;
            StatusCode code = xmlLoadArray(currentNode, "track", &trackVec, CProject1::xmlLoadTrack);
            if (FAILED(code)) {
                destoryVector(&trackVec);
                return code;
            }

            for (vector<ITrack*>::iterator it = trackVec.begin(); it != trackVec.end(); it++) {
                IInnerTrack *pTrack = dynamic_cast<IInnerTrack*>(*it);
                if (!pTrack) {
                    destoryVector(&trackVec);
                    return AV_OTHER_ERROR;
                }
                switch(pTrack->getTrackType()) {
                    case ETrackType_Video:
                        m_videoTracks.push_back(pTrack);
                        break;
                    case ETrackType_Audio:
                        m_audioTracks.push_back(pTrack);
                        break;
                }
            }
        }
        currentNode = currentNode->next;
    }

    return AV_OK;
}

/**
 * 创建tracks节点
 * @param trackVector
 * @return
 */
xmlNodePtr CProject1::xmlCreateTracksNode(const std::vector<ITrack*> trackVector) {
    xmlNodePtr tracksNode = NULL, trackNode = NULL;
    tracksNode = xmlNewNode(NULL, BAD_CAST "tracks");
    if (!tracksNode) {
        AVLOG(ELOG_LEVEL_ERROR, "xmlNewNode(tracks) failed!");
        goto failure;
    }

    for(vector<ITrack*>::const_iterator it = trackVector.begin(); it != trackVector.end(); it++) {
        ITrackPtr pTrack = *it;
        trackNode = CTrackSerialize::serialize(pTrack, "track");
        if (!trackNode) goto failure;
        xmlAddChild(tracksNode, trackNode);
    }

    return tracksNode;

failure:
    if (tracksNode)
        xmlFreeNode(tracksNode);
    return NULL;
}