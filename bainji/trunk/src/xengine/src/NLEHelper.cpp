//
// Created by wendachuan on 2018/10/9.
//

#include "NLEHelper.h"
#include "xengine/IProject.h"
#include <cstdlib>
#include "avpub/StatusCode.h"
#include "avpub/Log.h"
#include "media/ImageMedia.h"
#include "media/AVMedia.h"
#include "clip/ImageClip.h"
#include "clip/VideoClip.h"
#include "clip/AudioClip.h"

using namespace xedit;

/**
 * 将分数代表的帧率转化为EDsFrameRate
 * @param rFrameRate
 * @return
 */
EDsFrameRate NLEHelper::convertFrameRate(Rational rFrameRate) {
    double fFrameRate1000 = rFrameRate.doubleValue() * 1000;
    int nFrameRate1000 = (int)fFrameRate1000;
    if (!rFrameRate.nNum) {
        return keDsFrameRateVariable;
    } else if (abs(nFrameRate1000 - 24000) < 1) {
        return keDsFrameRate24;
    } else if (abs(nFrameRate1000 - 23980) < 1) {
        return keDsFrameRate24M;
    } else if (abs(nFrameRate1000 - 25000) < 1) {
        return keDsFrameRate25;
    } else if (abs(nFrameRate1000 - 30000) < 1) {
        return keDsFrameRate30;
    } else if (abs(nFrameRate1000 - 29970) < 1) {
        return keDsFrameRate30M;
    } else if (abs(nFrameRate1000 - 50000) < 1) {
        return keDsFrameRate50;
    } else if (abs(nFrameRate1000 - 60000) < 1) {
        return keDsFrameRate60;
    } else if (abs(nFrameRate1000 - 59940) < 1) {
        return keDsFrameRate60M;
    } else {
        return keDsFrameRateInvalid;
    }
}

/**
 * 将EDsFrameRate转换为Rational
 * @param frameRate
 * @return
 */
Rational NLEHelper::convertFromDsFrameRate(EDsFrameRate frameRate) {
    switch (frameRate) {
        case keDsFrameRate24:
            return Rational(24, 1);
        case keDsFrameRate24M:
            return Rational(24000, 1001);
        case keDsFrameRate25:
            return Rational(25, 1);
        case keDsFrameRate30:
            return Rational(30, 1);
        case keDsFrameRate30M:
            return Rational(30000, 1001);
        case keDsFrameRate50:
            return Rational(50, 1);
        case keDsFrameRate60:
            return Rational(60, 1);
        case keDsFrameRate60M:
            return Rational(60000, 1001);
        default:
            return Rational();
    }
}

/**
 * 将分数代表的aspect ratio转化为EDsFrameRate
 * @param rAspectRatio
 * @return
 */
EDsAspectRatio NLEHelper::convertAspectRatio(Rational rAspectRatio) {
    Rational rTmpAR = AVMakeRational(rAspectRatio.nNum, rAspectRatio.nDen);
    if ( rTmpAR == Rational(16, 9) ) {
        return keDsAspectRatio_16_9;
    } else if ( rTmpAR == Rational(4, 3) ) {
        return keDsAspectRatio_4_3;
    } else if ( rTmpAR == Rational(2, 1) ) {
        return keDsAspectRatio_2_1;
    } else {
        return keDsAspectRatioInvalid;
    }
}

/**
 * 转换扫描方式
 * @param bInterlaced 是否隔行扫描
 * @param bTopFieldFirst 是否顶场优先
 * @return
 */
EDsScanMode NLEHelper::convertScanMode(bool  bInterlaced, bool  bTopFieldFirst) {
    if ( !bInterlaced )
        return keDsScanModeProgressive;
    if (bTopFieldFirst)
        return keDsScanModeFirstFieldTop;
    else
        return keDsScanModeSecondFieldTop;
}

/**
 * 获得ESampleFormat
 * @param audioDataType
 * @param nBitsPerSample
 * @return
 */
ESampleFormat NLEHelper::getSampleFormat(EDsAudioDataType audioDataType, int nBitsPerSample) {
    if (audioDataType == keAudioDataTypePCM) {
        if (nBitsPerSample == 8) {
            return EAV_SAMPLE_FMT_U8;
        } else if (nBitsPerSample == 16) {
            return EAV_SAMPLE_FMT_S16;
        } else if (nBitsPerSample == 32) {
            return EAV_SAMPLE_FMT_S32;
        }
    } else if (audioDataType == keAudioDataTypeIEEEFLoat) {
        return EAV_SAMPLE_FMT_FLT;
    } else
        return EAV_SAMPLE_FMT_UNKNOWN;
}

/**
 * 引擎状态转时间线状态
 * @param engineStatus
 * @return
 */
ETimeLineStatus NLEHelper::dsEngineStatus2TimeLineStatus(EDs_EngineStatus engineStatus) {
    ETimeLineStatus timeLineStatus = ETimeLineStatus_None;
    switch (engineStatus) {
        case e_DS_PLAY_DROPFRAME:
            timeLineStatus = ETimeLineStatus_DropFrame;
            break;
        case e_DS_PLAY_END:
            timeLineStatus = ETimeLineStatus_PlayEnd;
            break;
        case e_DS_PLAY_FAILED:
            timeLineStatus = ETimeLineStatus_PlayFailed;
            break;
        case e_DS_PLAY_GETFRAME_FAILED:
            timeLineStatus = ETimeLineStatus_Error;
            break;
        case e_DS_COMPILER_SETP:
            break;
        case e_DS_COMPILER_FINISHED:
            timeLineStatus = ETimeLineStatus_GenerateFinish;
            break;
        case e_DS_COMPILER_FAILED:
            timeLineStatus = ETimeLineStatus_GenerateFailed;
            break;
        case e_DS_COMPILER_WRITEFILE_FILED:
            timeLineStatus = ETimeLineStatus_GenerateFailed;
            break;
        case e_DS_COMPILER_FILE_EXIST_ERROR:
            timeLineStatus = ETimeLineStatus_GenerateFailed;
            break;
    }
    return timeLineStatus;
}

/**
 *
 * @param scanMode
 * @return
 */
bool NLEHelper::isInterlaced(EDsScanMode scanMode) {
    switch (scanMode) {
        case keDsScanModeFirstFieldTop:
        case keDsScanModeSecondFieldTop:
        case keDsScanModeInterlacedFieldsInAFrame:
            return true;
        default:
            return false;
    }
}

/**
 * 轨道类型转换
 * @param trackType
 * @return
 */
EDsTrackType NLEHelper::trackType2DsTrackType(ETrackType trackType) {
    switch (trackType) {
        case ETrackType_Video:
            return eDsTrackerTypeVideo;
        case ETrackType_Audio:
            return eDsTrackerTypeAudio;
        default:
            return eDsTrackerTypeInvalid;
    }
}

 /**
  * 从ProjectSetting获得SDsResolutionInfo
  * @param setting
  * @param pOutResInfo
  */
void NLEHelper::getResolutionInfoFromProjectSetting(const ProjectSetting& setting, SDsResolutionInfo *pOutResInfo) {
    pOutResInfo->ulWidth = setting.nWidth;
    pOutResInfo->ulHeight = setting.nHeight;
    pOutResInfo->ulComponentBitCount = 8;
    pOutResInfo->eFrameRate = NLEHelper::convertFrameRate(setting.rFrameRate);
    pOutResInfo->eAspectRatio = NLEHelper::convertAspectRatio(setting.rAspectRatio);
    pOutResInfo->eScanMode = NLEHelper::convertScanMode(setting.bInterlaced, setting.bTopFieldFirst);
}

/**
 * 将SDsFileInfo转换为ImageMedia
 * @param fileInfo
 * @param pOutImageMedia
 */
void NLEHelper::convertSDsFileInfo2ImageMedia(const SDsFileInfo& fileInfo, IInnerImageMedia *pOutImageMedia) {
    pOutImageMedia->setPath(fileInfo.m_wszFileName);
    pOutImageMedia->setSize(GSize(fileInfo.m_sVideoInfo.m_iWidth, fileInfo.m_sVideoInfo.m_iHeight));
    pOutImageMedia->setImageType(EImageType_Unknown);
}

/**
 * 将ImageMedia转换为SDsFileInfo
 * @param pImageMedia
 * @param pOutFileInfo
 */
//void NLEHelper::convertImageMedia2SDsFileInfo(const IImageMedia *pImageMedia, SDsFileInfo *pOutFileInfo) {
//    memset(pOutFileInfo, 0, sizeof(SDsFileInfo));
//    pOutFileInfo->m_size = sizeof(SDsFileInfo);
//    strncpy(pOutFileInfo->m_wszFileName, pImageMedia->getPath(), sizeof(pOutFileInfo->m_wszFileName) - 1);
//    pOutFileInfo->m_ui64Duration = 0;
//    //pOutFileInfo->m_ui64FileSize = 0;
//    pOutFileInfo->m_eFileType = eFileTypeCg;
//    //pOutFileInfo->m_lastModifyTime = 0;
//    pOutFileInfo->m_iFileSubType = CG_SINGLE;
//    pOutFileInfo->m_iStreamIndex = 0;
//    pOutFileInfo->m_sVideoInfo.m_size = sizeof(SDsVideoFileInfo);
//    pOutFileInfo->m_sVideoInfo.m_left = 0;
//    pOutFileInfo->m_sVideoInfo.m_top = 0;
//    pOutFileInfo->m_sVideoInfo.m_iWidth = pImageMedia->getSize().nWidth;
//    pOutFileInfo->m_sVideoInfo.m_iHeight = pImageMedia->getSize().nHeight;
//    pOutFileInfo->m_sVideoInfo.m_eSurfaceFormat = keDsSurfaceFormatInvalid;
//    pOutFileInfo->m_sVideoInfo.m_eAspectRatio = keDsAspectRatioInvalid;
//    pOutFileInfo->m_sVideoInfo.m_eFrameRate = keDsFrameRate25;
//    pOutFileInfo->m_sVideoInfo.m_eScanMode = keDsScanModeFirstFieldTop;
//    pOutFileInfo->m_sVideoInfo.m_ulComponentBitCount = 8;
//    pOutFileInfo->m_sVideoInfo.m_ulDataRate = 0;
//    pOutFileInfo->m_eActualSound = eDsVANone;
//    pOutFileInfo->m_AudioFileCount = 0;
//    pOutFileInfo->m_wszVideoIndexFile[0] = '\0';
//}

/**
 * 将SDsFileInfo转换为AVMedia
 * @param fileInfo
 * @param pOutAVMedia
 */
void NLEHelper::convertSDsFileInfo2AVMedia(const SDsFileInfo& fileInfo, IInnerAVMedia *pOutAVMedia) {
    pOutAVMedia->setPath(fileInfo.m_wszFileName);

    AVMediaInfo mediaInfo = {0};
    strncpy(mediaInfo.path, fileInfo.m_wszFileName, sizeof(mediaInfo.path) - 1);
    //mediaInfo.codec;
    mediaInfo.nFileSize = fileInfo.m_ui64FileSize;
    mediaInfo.rDuration = AVMakeRational(fileInfo.m_ui64Duration, NLE_FPS);
    //mediaInfo.eMuxerType;
    mediaInfo.nOverallBitrate = fileInfo.m_sVideoInfo.m_ulDataRate;
    mediaInfo.nVideoCount = fileInfo.m_eFileType == eFileTypeVideo ? 1 : 0;
    if (mediaInfo.nVideoCount > 0) {
        VideoStream& videoStream = mediaInfo.vStreams[0];
        videoStream.nIndex = 0;
        //videoStream.nID;
        //videoStream.nCodecTag;
        videoStream.nBitrate = fileInfo.m_sVideoInfo.m_ulDataRate;
        //videoStream.eBitrateMode =
        //videoStream.eCodecID;
        videoStream.rDuration = mediaInfo.rDuration;
        //videoStream.rTimebase;
        //videoStream.ePixFmt;
        videoStream.nWidth = fileInfo.m_sVideoInfo.m_iWidth;
        videoStream.nHeight = fileInfo.m_sVideoInfo.m_iHeight;
        //videoStream.rSAR;
        //videoStream.rDAR;
        //videoStream.eFrameRateMode;
        videoStream.rFrameRate = convertFromDsFrameRate(fileInfo.m_sVideoInfo.m_eFrameRate);
        videoStream.nFrameCount = (videoStream.rFrameRate * videoStream.rDuration).integerValue();
        //videoStream.eStandard;
        videoStream.bInterlaced = isInterlaced(fileInfo.m_sVideoInfo.m_eScanMode);
        videoStream.bTopFieldFirst = fileInfo.m_sVideoInfo.m_eScanMode == keDsScanModeFirstFieldTop;
        //videoStream.bLossless;
        //videoStream.eProfile;
        //videoStream.eLevel;
        //videoStream.bHasBFrames;
    }

    mediaInfo.nAudioCount = fileInfo.m_AudioFileCount;
    for (int i = 0; i < mediaInfo.nAudioCount; i++) {
        AudioStream& audioStream = mediaInfo.aStreams[i];
        audioStream.nIndex = i + 1;
        //audioStream.nID;
        //audioStream.nCodecTag;
        //audioStream.nBitrate;
        //audioStream.eBitrateMode =
        //audioStream.eCodecID;
        audioStream.rDuration = mediaInfo.rDuration;
        //audioStream.rTimebase;
        audioStream.eSampleFmt = getSampleFormat(fileInfo.m_sAudioInfoList[i].eDataType, fileInfo.m_sAudioInfoList[i].ulBitsPerSample);
        audioStream.nSampleRate = fileInfo.m_sAudioInfoList[i].ulSamplesPerSec;
        audioStream.nBitsPerSample = fileInfo.m_sAudioInfoList[i].ulBitsPerSample;
        audioStream.nChannels = (int)fileInfo.m_sAudioInfoList[i].eChannelType;
        //audioStream.nChannelLayout;
        //audioStream.bLossless;
    }

    pOutAVMedia->setMediaInfo(mediaInfo);
}

/**
 * 将AVMedia转换为SDsFileInfo
 * @param pAVMedia
 * @param pOutFileInfo
 */
//void NLEHelper::convertAVMedia2SDsFileInfo(const IAVMedia *pAVMedia, SDsFileInfo *pOutFileInfo) {
//    AVMediaInfo mediaInfo;
//    pAVMedia->getMediaInfo(&mediaInfo);
//
//    memset(pOutFileInfo, 0, sizeof(SDsFileInfo));
//    pOutFileInfo->m_size = sizeof(SDsFileInfo);
//    strncpy(pOutFileInfo->m_wszFileName, pAVMedia->getPath(), sizeof(pOutFileInfo->m_wszFileName) - 1);
//    pOutFileInfo->m_ui64Duration = mediaInfo.rDuration.nNum * NLE_FPS / mediaInfo.rDuration.nDen;
//    pOutFileInfo->m_ui64FileSize = mediaInfo.nFileSize;
//    pOutFileInfo->m_eFileType = mediaInfo.nVideoCount > 0 ? eFileTypeVideo : eFileTypeAudio;
//    //pOutFileInfo->m_lastModifyTime = 0;
//    pOutFileInfo->m_iFileSubType = CG_SINGLE;
//    pOutFileInfo->m_iStreamIndex = 0;
//    pOutFileInfo->m_sVideoInfo.m_size = sizeof(SDsVideoFileInfo);
//    pOutFileInfo->m_sVideoInfo.m_left = 0;
//    pOutFileInfo->m_sVideoInfo.m_top = 0;
//    pOutFileInfo->m_sVideoInfo.m_iWidth = pAVMedia->getSize().nWidth;
//    pOutFileInfo->m_sVideoInfo.m_iHeight = pAVMedia->getSize().nHeight;
//    pOutFileInfo->m_sVideoInfo.m_eSurfaceFormat = keDsSurfaceFormatInvalid;
//    pOutFileInfo->m_sVideoInfo.m_eAspectRatio = keDsAspectRatioInvalid;
//    pOutFileInfo->m_sVideoInfo.m_eFrameRate = keDsFrameRate25;
//    pOutFileInfo->m_sVideoInfo.m_eScanMode = keDsScanModeFirstFieldTop;
//    pOutFileInfo->m_sVideoInfo.m_ulComponentBitCount = 8;
//    pOutFileInfo->m_sVideoInfo.m_ulDataRate = 0;
//    pOutFileInfo->m_eActualSound = eDsVANone;
//    pOutFileInfo->m_AudioFileCount = 0;
//    pOutFileInfo->m_wszVideoIndexFile[0] = '\0';
//}

/**
 * 将Media转换为SDsFileInfo
 * @param pMedia
 * @param pOutFileInfo
 */
//void NLEHelper::convertMedia2SDsFileInfo(const IMedia *pMedia, SDsFileInfo *pOutFileInfo) {
//    EMediaType mediaType = pMedia->getMediaType();
//    if (mediaType == EMediaType_AV) {
//        const IAVMediaPtr pAVMedia = dynamic_cast<const IAVMediaPtr>(pMedia);
//        if (pAVMedia)
//            convertAVMedia2SDsFileInfo(pAVMedia, pOutFileInfo);
//    } else if (mediaType == EMediaType_Image) {
//        const IImageMediaPtr pImageMedia = dynamic_cast<const IImageMediaPtr>(pMedia);
//        if (pImageMedia)
//            convertImageMedia2SDsFileInfo(pImageMedia, pOutFileInfo);
//    }
//}

/**
 * 打开媒体
 * @param pImporter
 * @param mediaPath 媒体路径
 * @param bSequence 是否是序列
 * @param pOutFileInfo 输出文件信息
 * @return
 */
StatusCode NLEHelper::openMedia(IDsImporter* pImporter, const char *mediaPath, bool bSequence, SDsFileInfo *pOutFileInfo) {
    StatusCode code = pImporter->OpenFile((char*)mediaPath, bSequence);
    if (FAILED(code)) {
        AVLOG(ELOG_LEVEL_ERROR, "NLEHelper::openMedia: 打开文件[%s]失败!", mediaPath);
        return code;
    }

    code = pImporter->GetFileInfo(pOutFileInfo);
    if (FAILED(code)) {
        AVLOG(ELOG_LEVEL_ERROR, "NLEHelper::openMedia: 取文件[%s]信息失败!", mediaPath);
        return code;
    }

    if (pOutFileInfo->m_eFileType == EFileType::eFileTypeCg) {
        // 图片默认长度为5秒
        pOutFileInfo->m_ui64Duration = 5*NLE_FPS;
    }
    return AV_OK;
}

/**
 * 打开媒体
 * @param pImporter
 * @param mediaPath 媒体路径
 * @param bSequence 是否是序列
 * @param ppOutMedia 输出媒体
 * @return
 */
StatusCode NLEHelper::openMedia(IDsImporter* pImporter, const char *mediaPath, bool bSequence, IInnerMedia **ppOutMedia) {
    SDsFileInfo sFileInfo = { sizeof(SDsFileInfo) };
    StatusCode code = openMedia(pImporter, mediaPath, bSequence, &sFileInfo);
    if (FAILED(code)) {
        return code;
    }

    if (sFileInfo.m_eFileType == EFileType::eFileTypeCg) {
        CImageMedia *pMedia = new CImageMedia();
        convertSDsFileInfo2ImageMedia(sFileInfo, pMedia);
        pMedia->setDsFileInfo(sFileInfo);
        *ppOutMedia = pMedia;
    } else {
        CAVMedia *pMedia = new CAVMedia();
        convertSDsFileInfo2AVMedia(sFileInfo, pMedia);
        pMedia->setDsFileInfo(sFileInfo);
        *ppOutMedia = pMedia;
    }

    return AV_OK;
}

/**
 * 创建clip
 * @param pClipFactory 切片工厂
 * @param fileInfo 文件信息
 * @param nFrameOffsetOnTrack 在轨道上的起始位置(帧偏移)
 * @param nFrameDurationTrack 在轨道上的长度(帧数)
 * @param nMediaFrameStart 在媒体上的起始位置(帧偏移)
 * @param nMediaFrameEnd 在媒体上的结束位置(帧偏移)
 * @param ppOutClip 输出切片
 * @return
 */
StatusCode NLEHelper::createClip(IDsClipFactory *pClipFactory,
                                const SDsFileInfo& fileInfo,
                                uint64_t nFrameOffsetOnTrack,
                                uint64_t nFrameDurationTrack,
                                uint64_t nMediaFrameStart,
                                uint64_t nMediaFrameEnd,
                                IDsFileClip **ppOutClip) {
    assert(nFrameDurationTrack <= fileInfo.m_ui64Duration);

    TDsSmartPtr<IDsFileClip> pFileClip;
    StatusCode code = pClipFactory->CrateFileClip(fileInfo.m_eFileType, &pFileClip);
    if (FAILED(code)) {
        AVLOG(ELOG_LEVEL_ERROR, "NLEHelper::createClip: 创建切片失败!文件: [%s]", fileInfo.m_wszFileName);
        return code;
    }

    SClipInfo clipInfo = { PlayEngine::EClipType::eClipTypeFile };
    clipInfo.m_framePosition = nFrameOffsetOnTrack;
    clipInfo.m_frameDuration = nFrameDurationTrack;
    clipInfo.m_frameTrimIn = nMediaFrameStart;
    clipInfo.m_frameTrimOut = nMediaFrameEnd;
    pFileClip->SetClipInfo(clipInfo);

    SFileClipInfo fileClipInfo = { FALSE };
    strcpy(fileClipInfo.m_wszFilename, fileInfo.m_wszFileName);
    fileClipInfo.m_eFileType = fileInfo.m_eFileType;
    fileClipInfo.m_frameTotal = fileInfo.m_ui64Duration;
    fileClipInfo.m_iFileSubtype = fileInfo.m_iFileSubType;
    fileClipInfo.m_iAudioFileCount = fileInfo.m_AudioFileCount;
    fileClipInfo.m_eActualSound = fileInfo.m_eActualSound;
    fileClipInfo.m_eSurfaceFormat = fileInfo.m_sVideoInfo.m_eSurfaceFormat;

    if (fileInfo.m_AudioFileCount > 0) {
        //@ multi stream file ??
        int nChannels = fileInfo.m_sAudioInfoList[0].eChannelType;
        int nTotalChannels = nChannels * fileInfo.m_AudioFileCount;
        if (nTotalChannels > 16)
            nTotalChannels = 16;
        fileClipInfo.m_iAudioChannelsCount = nTotalChannels;
        for (int i = 0; i < fileInfo.m_AudioFileCount; i++) {
            for (int j = 0; j < nChannels; j++) {
                strcpy(fileClipInfo.m_AudioChannels[i].m_wszFileName, fileInfo.m_sAudioInfoList[i].m_wszFileName);
                fileClipInfo.m_AudioChannels[i].m_iAudioStreamIndex = i;
                fileClipInfo.m_AudioChannels[i].m_iChannelIndex = i * nChannels + j;
            }
        }
        fileClipInfo.m_iAudioPlayListCount = fileInfo.m_AudioFileCount;
    }

    pFileClip->SetFileClipInfo(fileClipInfo);
    pFileClip->AddRef();
    *ppOutClip = pFileClip;
    return AV_OK;
}

/**
 * 创建clip
 * @param mediaId 媒体id
 * @param pClipFactory 切片工厂
 * @param fileInfo 文件信息
 * @param nFrameOffsetOnTrack 在轨道上的起始位置(帧偏移)
 * @param nFrameDurationTrack 在轨道上的长度(帧数)
 * @param nMediaFrameStart 在媒体上的起始位置(帧偏移)
 * @param nMediaFrameEnd 在媒体上的结束位置(帧偏移)
 * @param ppOutClip 输出切片
 * @return
 */
StatusCode NLEHelper::createClip(ID mediaId,
                                 IDsClipFactory *pClipFactory,
                                 const SDsFileInfo& fileInfo,
                                 uint64_t nFrameOffsetOnTrack,
                                 uint64_t nFrameDurationTrack,
                                 uint64_t nMediaFrameStart,
                                 uint64_t nMediaFrameEnd,
                                 IInnerClip **ppOutClip) {
    IDsFileClip *pDsFileClip = NULL;
    StatusCode code = createClip(pClipFactory, fileInfo, nFrameOffsetOnTrack, nFrameDurationTrack, nMediaFrameStart, nMediaFrameEnd, &pDsFileClip);
    if (FAILED(code)) return code;

    IInnerClip *pClip = NULL;
    SFileClipInfo fileClipInfo = pDsFileClip->GetFileClipInfo();
    if (fileClipInfo.m_eFileType == EFileType::eFileTypeCg) {
        pClip = new CImageClip();
    } else if (fileClipInfo.m_eFileType == EFileType::eFileTypeVideo) {
        CVideoClip *pVideoClip = new CVideoClip();
        pVideoClip->setOffsetInMedia(Rational(0, 1));
        pClip = pVideoClip;
    } else if (fileClipInfo.m_eFileType == EFileType::eFileTypeAudio) {
        CAudioClip *pAudioClip = new CAudioClip();
        pAudioClip->setOffsetInMedia(Rational(0, 1));
        pClip = pAudioClip;
    } else {
        delete pDsFileClip;
        return AV_OTHER_ERROR;
    }
    pClip->setRefMediaId(mediaId);
    pClip->setOffsetOnTrack(AVMakeRational(nFrameOffsetOnTrack, NLE_FPS));
    pClip->setDuration(AVMakeRational(nFrameDurationTrack, NLE_FPS));
    pClip->bindDsClip(pDsFileClip);
    *ppOutClip = pClip;
    return AV_OK;
}

/**
 * 创建clip
 * @param pClipFactory 切片工厂
 * @param fileInfo 文件信息
 * @param eFileType 切片类型
 * @param nStreamIndex 流序号
 * @param nFrameOffsetOnTrack 在轨道上的起始位置(帧偏移)
 * @param nFrameDurationTrack 在轨道上的长度(帧数)
 * @param nMediaFrameStart 在媒体上的起始位置(帧偏移)
 * @param nMediaFrameEnd 在媒体上的结束位置(帧偏移)
 * @param ppOutClip 输出切片
 * @return
 */
StatusCode NLEHelper::createAVClip(IDsClipFactory *pClipFactory,
                               const SDsFileInfo& fileInfo,
                               EFileType eFileType,
                               int nStreamIndex,
                               uint64_t nFrameOffsetOnTrack,
                               uint64_t nFrameDurationTrack,
                               uint64_t nMediaFrameStart,
                               uint64_t nMediaFrameEnd,
                               IDsFileClip **ppOutClip) {
    assert(nFrameDurationTrack <= fileInfo.m_ui64Duration);

    TDsSmartPtr<IDsFileClip> pFileClip;
    StatusCode code = pClipFactory->CrateFileClip(eFileType, &pFileClip);
    if (FAILED(code)) {
        AVLOG(ELOG_LEVEL_ERROR, "NLEHelper::createClip: 创建切片失败!文件: [%s]", fileInfo.m_wszFileName);
        return code;
    }

    SClipInfo clipInfo = { PlayEngine::EClipType::eClipTypeFile };
    clipInfo.m_framePosition = nFrameOffsetOnTrack;
    clipInfo.m_frameDuration = nFrameDurationTrack;
    clipInfo.m_frameTrimIn = nMediaFrameStart;
    clipInfo.m_frameTrimOut = nMediaFrameEnd;
    pFileClip->SetClipInfo(clipInfo);

    SFileClipInfo fileClipInfo = { FALSE };
    strcpy(fileClipInfo.m_wszFilename, fileInfo.m_wszFileName);
    fileClipInfo.m_eFileType = eFileType;
    fileClipInfo.m_frameTotal = fileInfo.m_ui64Duration;
    fileClipInfo.m_iFileSubtype = fileInfo.m_iFileSubType;
    fileClipInfo.m_iAudioFileCount = eFileType == eFileTypeVideo ? 0 : 1;
    fileClipInfo.m_eActualSound = fileInfo.m_eActualSound;
    fileClipInfo.m_eSurfaceFormat = fileInfo.m_sVideoInfo.m_eSurfaceFormat;

    if (fileInfo.m_AudioFileCount > 0) {
        if (nStreamIndex >= fileInfo.m_AudioFileCount)
            return AV_OTHER_ERROR;

        int nChannels = fileInfo.m_sAudioInfoList[nStreamIndex].eChannelType;
        fileClipInfo.m_iAudioChannelsCount = nChannels;
        for (int i = 0; i < nChannels; i++) {
            strcpy(fileClipInfo.m_AudioChannels[nStreamIndex].m_wszFileName, fileInfo.m_sAudioInfoList[nStreamIndex].m_wszFileName);
            fileClipInfo.m_AudioChannels[nStreamIndex].m_iAudioStreamIndex = nStreamIndex;
            fileClipInfo.m_AudioChannels[nStreamIndex].m_iChannelIndex = nStreamIndex * nChannels + i;
        }

        fileClipInfo.m_iAudioPlayListCount = fileInfo.m_AudioFileCount;
    }

    pFileClip->SetFileClipInfo(fileClipInfo);
    pFileClip->AddRef();
    *ppOutClip = pFileClip;
    return AV_OK;
}

/**
 * 创建clip
 * @param mediaId 媒体id
 * @param pClipFactory 切片工厂
 * @param fileInfo 文件信息
 * @param eClipType 切片类型
 * @param nStreamIndex 流序号
 * @param nFrameOffsetOnTrack 在轨道上的起始位置(帧偏移)
 * @param nFrameDurationTrack 在轨道上的长度(帧数)
 * @param nMediaFrameStart 在媒体上的起始位置(帧偏移)
 * @param nMediaFrameEnd 在媒体上的结束位置(帧偏移)
 * @param ppOutClip 输出切片
 * @return
 */
StatusCode NLEHelper::createAVClip(ID mediaId,
                                   IDsClipFactory *pClipFactory,
                                   const SDsFileInfo& fileInfo,
                                   EClipType eClipType,
                                   int nStreamIndex,
                                   uint64_t nFrameOffsetOnTrack,
                                   uint64_t nFrameDurationTrack,
                                   uint64_t nMediaFrameStart,
                                   uint64_t nMediaFrameEnd,
                                   IInnerClip **ppOutClip) {
    EFileType fileType = eFileTypeInvalid;
    switch (eClipType) {
        case EClipType_Video:
            fileType = eFileTypeVideo;
            break;
        case EClipType_Audio:
            fileType = eFileTypeAudio;
            break;
    }
    if (fileType == eFileTypeInvalid)
        return AV_OTHER_ERROR;

    IDsFileClip *pDsFileClip = NULL;
    StatusCode code = createAVClip(pClipFactory, fileInfo, fileType, nStreamIndex, nFrameOffsetOnTrack, nFrameDurationTrack, nMediaFrameStart, nMediaFrameEnd, &pDsFileClip);
    if (FAILED(code)) return code;

    IInnerClip *pClip = NULL;
    if (fileType == EFileType::eFileTypeVideo) {
        pClip = new CVideoClip();
    } else if (fileType == EFileType::eFileTypeAudio) {
        pClip = new CAudioClip();
    }
    pClip->setRefMediaId(mediaId);
    pClip->setOffsetOnTrack(AVMakeRational(nFrameOffsetOnTrack, NLE_FPS));
    pClip->setDuration(AVMakeRational(nFrameDurationTrack, NLE_FPS));
    pClip->bindDsClip(pDsFileClip);
    *ppOutClip = pClip;
    return AV_OK;
}

/**
 * 将生成设置转换为非编格式字符串
 * @param setting
 * @param pOutFormat
 */
void NLEHelper::generateSettingToDsFormat(const GenerateSetting& setting, string *pOutFormat) {

    const char strFormat[] =
            "      <param>\n"
            "        <param>\n"
            "          <format formatClsid=\"{31435641-0000-0010-8000-00AA00389B71}\" />\n"
            "          <param name=\"mux\" SetValue=\"ps\" />\n"
            "          <param name=\"高标清\" SetValue=\"高清\" />\n"
            "          <param name=\"码率(M)\" SetValue=\"%.2f\" />\n"
            "          <param name=\"宽\" SetValue=\"%d\" />\n"
            "          <param name=\"高\" SetValue=\"%d\" />\n"
            "          <param name=\"帧率\" SetValue=\"25\" />\n"
            "          <param name=\"packet\" SetValue=\"7\" />\n"
            "          <param name=\"GOPSize\" SetValue=\"25\" />\n"
            "          <!-- 25-250 -->\n"
            "          <param name=\"拉伸缩放\" SetValue=\"1\" />\n"
            "        </param>\n"
            "        <param>\n"
            "          <format formatClsid=\"{000000ff-0000-0010-8000-00AA00389B71}\" />\n"
            "          <param name=\"声道个数\" SetValue=\"%d\" />\n"
            "          <param name=\"采样位数\" SetValue=\"%d\" />\n"
            "          <param name=\"采样频率\" SetValue=\"%d\" />\n"
            "        </param>\n"
            "      </param>\n"
    ;

    char strFormatBuf[1024] = {0};
    snprintf(strFormatBuf, sizeof(strFormatBuf) - 1, strFormat,
             setting.encodeParam.videoParam.nBitrate * 1.0 / 8000000,
             setting.encodeParam.videoParam.nWidth,
             setting.encodeParam.videoParam.nHeight,
             setting.encodeParam.audioParam.nChannels,
             setting.encodeParam.audioParam.nBitsPerSample,
             setting.encodeParam.audioParam.nSampleRate);

    *pOutFormat = strFormatBuf;
}

/**
 * 将时间转换为帧号
 * @param rTime
 * @return
 */
int64_t NLEHelper::timeToFrameIndex(const Rational& rTime) {
    return rTime.nNum * NLE_FPS / rTime.nDen;
}