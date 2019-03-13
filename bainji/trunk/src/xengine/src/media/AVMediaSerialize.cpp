//
// Created by wendachuan on 2018/11/20.
//

#include "AVMediaSerialize.h"
#include "BaseMediaSerialize.h"
#include "../PreviewSerialize.h"
#include "xutil/NumberConvertor.h"
#include "xutil/libxmlEx.h"
#include "xutil/RationalSerialize.h"
#include "avpub/TSmartPtr.h"
#include "avpub/Log.h"
#include "avpub/StatusCode.h"
#include "AVMedia.h"

using namespace xedit;
using namespace libav;

/**
 * 序列化对象
 * @param object 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CBaseAVStreamSerialize::serialize(const BaseAVStream& object, const char* nodeName) {
    xmlNodePtr durationNode = NULL, timebaseNode = NULL;
    xmlNodePtr streamNode = xmlNewNode(NULL, BAD_CAST nodeName);
    if (!streamNode)
        return NULL;

    xmlNewChildEx(streamNode, NULL, BAD_CAST "index", object.nIndex);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "streamId", object.nID);

    durationNode = CRationalSerializeHelper::serialize(object.rDuration, "duration");
    if (!durationNode) goto failure;
    xmlAddChild(streamNode, durationNode);

    xmlNewChildEx(streamNode, NULL, BAD_CAST "codecID", (int)object.eCodecID);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "codecTag", object.nCodecTag);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "bitrateMode", (int)object.eBitrateMode);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "averageBitrate", (int)object.nBitrate);

    timebaseNode = CRationalSerializeHelper::serialize(object.rTimebase, "timebase");
    if (!timebaseNode) goto failure;
    xmlAddChild(streamNode, timebaseNode);

    return streamNode;

failure:
    if (streamNode)
        xmlFreeNode(streamNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutStream 输出对象
 */
bool CBaseAVStreamSerialize::deserialize(const xmlNodePtr& node, BaseAVStreamPtr pOutStream) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char *) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "index")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nIndex)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "streamId")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nID)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "duration")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutStream->rDuration))
                goto failure;
        } else if (!strcmp(nodeName, "codecID")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int *) &pOutStream->eCodecID)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "codecTag")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nCodecTag)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "bitrateMode")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int *) &pOutStream->eBitrateMode)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "averageBitrate")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nBitrate)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "timebase")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutStream->rTimebase))
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
 * @param pMedia 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CAVMediaSerialize::serialize(const IInnerAVMedia *pMedia, const char* nodeName) {
    if (!pMedia || !nodeName) return NULL;

    AVMediaInfo mediaInfo = {0};
    pMedia->getMediaInfo(&mediaInfo);

    xmlNodePtr durationNode = NULL, streamsNode = NULL;
    xmlNodePtr mediaNode = CBaseMediaSerialize::serialize(dynamic_cast<const IInnerAVMedia*>(pMedia), nodeName);
    if (!mediaNode) goto failure;

    xmlNewChildEx(mediaNode, NULL, BAD_CAST "fileSize", mediaInfo.nFileSize);
    xmlNewChildEx(mediaNode, NULL, BAD_CAST "muxerType", (int)mediaInfo.eMuxerType);
    xmlNewChildEx(mediaNode, NULL, BAD_CAST "overallBitrate", mediaInfo.nOverallBitrate);
    durationNode = CRationalSerializeHelper::serialize(mediaInfo.rDuration, "duration");
    if (!durationNode) goto failure;
    xmlAddChild(mediaNode, durationNode);

    streamsNode = xmlNewChild(mediaNode, NULL, BAD_CAST "streams", NULL);

    if (mediaInfo.nVideoCount > 0) {
        for (int i = 0; i < mediaInfo.nVideoCount; i++) {
            xmlNodePtr videoStreamNode = CVideoStreamSerialize::serialize(mediaInfo.vStreams[i], pMedia->getPreview(mediaInfo.vStreams[i].nIndex), "videoStream");
            if (!videoStreamNode) {
                goto failure;
            }
            xmlAddChild(streamsNode, videoStreamNode);
        }
    }

    if (mediaInfo.nAudioCount > 0) {
        for (int i = 0; i < mediaInfo.nAudioCount; i++) {
            xmlNodePtr audioStreamNode = CAudioStreamSerialize::serialize(mediaInfo.aStreams[i], "audioStream");
            if (!audioStreamNode) {
                goto failure;
            }
            xmlAddChild(streamsNode, audioStreamNode);
        }
    }

    return mediaNode;

failure:
    xmlFreeNode(mediaNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 */
IInnerAVMedia* CAVMediaSerialize::deserialize(const xmlNodePtr& node) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    IInnerAVMedia *pMedia = dynamic_cast<IInnerAVMedia*>(CBaseMediaSerialize::deserialize(node));
    if (!pMedia) return NULL;

    AVMediaInfo mediaInfo = {0};
    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char*) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!strcmp(nodeName, "duration")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &mediaInfo.rDuration))
                goto failure;
        } else if (!strcmp(nodeName, "fileSize")) {
            if (!strToNumber(nodeContent, &mediaInfo.nFileSize)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "muxerType")) {
            if (!strToNumber(nodeContent, (int*)&mediaInfo.eMuxerType)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "overallBitrate")) {
            if (!strToNumber(nodeContent, &mediaInfo.nOverallBitrate)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "streams")) {
            xmlNodePtr streamCurrentNode = currentNode->xmlChildrenNode;
            while (streamCurrentNode) {
                const char *streamNodeName = (const char *) streamCurrentNode->name;
                if (!strcmp(streamNodeName, "videoStream")) {
                    VideoStream videoStream;
                    memset(&videoStream, 0, sizeof(VideoStream));
                    IPreview *pPreview = NULL;
                    if (!CVideoStreamSerialize::deserialize(streamCurrentNode, &videoStream, &pPreview))
                        goto failure;
                    mediaInfo.vStreams[mediaInfo.nVideoCount] = videoStream;
                    mediaInfo.nVideoCount++;
                    if (pPreview)
                        pMedia->setPreview(videoStream.nIndex, pPreview);
                } else if (!strcmp(streamNodeName, "audioStream")) {
                    AudioStream audioStream;
                    memset(&audioStream, 0, sizeof(AudioStream));
                    if (!CAudioStreamSerialize::deserialize(streamCurrentNode, &audioStream))
                        goto failure;
                    mediaInfo.aStreams[mediaInfo.nAudioCount] = audioStream;
                    mediaInfo.nAudioCount++;
                }
                streamCurrentNode = streamCurrentNode->next;
            }
        }
        currentNode = currentNode->next;
    }

    pMedia->setMediaInfo(mediaInfo);
    return pMedia;

failure:
    delete pMedia;
    return NULL;
}

/**
 * 序列化对象
 * @param object 待序列化对象
 * @param pPreview 预览
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CVideoStreamSerialize::serialize(const VideoStream& object, IPreview *pPreview, const char* nodeName) {
    assert(nodeName);

    xmlNodePtr sarNode = NULL, darNode = NULL, averageFramerateNode = NULL, previewNode = NULL;
    xmlNodePtr streamNode = CBaseAVStreamSerialize::serialize(object, nodeName);
    if (!streamNode)
        return NULL;

    xmlNewChildEx(streamNode, NULL, BAD_CAST "width", object.nWidth);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "height", object.nHeight);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "pixelForamt", (int)object.ePixFmt);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "standard", (int)object.eStandard);

    sarNode = CRationalSerializeHelper::serialize(object.rSAR, "sar");
    if (!sarNode) goto failure;
    xmlAddChild(streamNode, sarNode);

    darNode = CRationalSerializeHelper::serialize(object.rDAR, "dar");
    if (!darNode) goto failure;
    xmlAddChild(streamNode, darNode);

    xmlNewChildEx(streamNode, NULL, BAD_CAST "framerateMode", (int)object.eFrameRateMode);

    averageFramerateNode = CRationalSerializeHelper::serialize(object.rFrameRate, "averageFramerate");
    if (!averageFramerateNode) goto failure;
    xmlAddChild(streamNode, averageFramerateNode);

    xmlNewChildEx(streamNode, NULL, BAD_CAST "frameCount", object.nFrameCount);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "isInterlaced", object.bInterlaced);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "isTopFieldFirst", object.bTopFieldFirst);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "profile", (int)object.eProfile);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "level", (int)object.eLevel);

    if (pPreview) {
        previewNode = CPreviewSerialize::serialize(pPreview, "preview");
        if (!previewNode) goto failure;
        xmlAddChild(streamNode, previewNode);
    }

    return streamNode;

failure:
    if (streamNode)
        xmlFreeNode(streamNode);
    return NULL;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutStream 输出对象
 * @param ppOutPreview 输出预览
 */
bool CVideoStreamSerialize::deserialize(const xmlNodePtr& node, VideoStreamPtr pOutStream, IPreview **ppOutPreview) {
    IPreview *pPreview = NULL;
    xmlNodePtr currentNode = node->xmlChildrenNode;
    if (!CBaseAVStreamSerialize::deserialize(node, pOutStream))
        goto failure;

    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char *) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);

        if (!strcmp(nodeName, "width")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nWidth)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "height")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nHeight)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "pixelFormat")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int*)&pOutStream->ePixFmt)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "standard")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int*)&pOutStream->eStandard)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "sar")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutStream->rSAR))
                goto failure;
        } else if (!strcmp(nodeName, "dar")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutStream->rDAR))
                goto failure;
        } else if (!strcmp(nodeName, "framerateMode")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int*)&pOutStream->eFrameRateMode)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "averageFramerate")) {
            if (!CRationalSerializeHelper::deserialize(currentNode, &pOutStream->rFrameRate))
                goto failure;
        } else if (!strcmp(nodeName, "frameCount")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->nFrameCount)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "isInterlaced")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->bInterlaced)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "isTopFieldFirst")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, &pOutStream->bTopFieldFirst)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "profile")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int*)&pOutStream->eProfile)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "level")) {
            if (!nodeContent) {
                AVLOG(ELOG_LEVEL_ERROR, "节点[%s]的内容为空！", nodeName);
                goto failure;
            }

            if (!strToNumber(nodeContent, (int*)&pOutStream->eLevel)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "preview")) {
            SAFE_DELETE(pPreview);
            pPreview = CPreviewSerialize::deserialize(currentNode);
            if (!pPreview) {
                AVLOG(ELOG_LEVEL_ERROR, "反序列化preview失败！");
                goto failure;
            }
        }
        currentNode = currentNode->next;
    }

    *ppOutPreview = pPreview;
    return true;

failure:
    SAFE_DELETE(pPreview);
    return  false;
}

/**
 * 序列化对象
 * @param object 待序列化对象
 * @param nodeName 对象序列化后的节点名称
 * @return 序列化后的数据
 */
xmlNodePtr CAudioStreamSerialize::serialize(const AudioStream& object, const char* nodeName) {
    assert(nodeName);

    xmlNodePtr streamNode = CBaseAVStreamSerialize::serialize(object, nodeName);
    if (!streamNode)
        return NULL;

    xmlNewChildEx(streamNode, NULL, BAD_CAST "sampleFormat", (int)object.eSampleFmt);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "sampleRate", object.nSampleRate);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "bitsPerSample", object.nBitsPerSample);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "channelCount", object.nChannels);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "channelLayout", object.nChannelLayout);
    xmlNewChildEx(streamNode, NULL, BAD_CAST "isLossless", object.bLossless);

    return streamNode;
}

/**
 * 反序列化
 * @param node 待反序列化的数据
 * @param pOutStream 输出对象
 */
bool CAudioStreamSerialize::deserialize(const xmlNodePtr& node, AudioStreamPtr pOutStream) {
    xmlNodePtr currentNode = node->xmlChildrenNode;
    if (!CBaseAVStreamSerialize::deserialize(node, pOutStream))
        goto failure;

    while (currentNode) {
        const char *nodeName = (const char *) currentNode->name;
        const char *nodeContent = (const char *) xmlNodeListGetString(currentNode->doc, currentNode->xmlChildrenNode, 1);
        if (!nodeContent) {
            currentNode = currentNode->next;
            continue;
        }

        if (!strcmp(nodeName, "sampleFormat")) {
            if (!strToNumber(nodeContent, (int*)&pOutStream->eSampleFmt)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "sampleRate")) {
            if (!strToNumber(nodeContent, &pOutStream->nSampleRate)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "bitsPerSample")) {
            if (!strToNumber(nodeContent, &pOutStream->nBitsPerSample)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "channelCount")) {
            if (!strToNumber(nodeContent, &pOutStream->nChannels)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "channelLayout")) {
            if (!strToNumber(nodeContent, &pOutStream->nChannelLayout)) {
                goto failure;
            }
        } else if (!strcmp(nodeName, "isLossless")) {
            if (!strToNumber(nodeContent, &pOutStream->bLossless)) {
                goto failure;
            }
        }
        currentNode = currentNode->next;
    }

    return true;

failure:
    return false;
}