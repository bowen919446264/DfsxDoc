//
// Created by wendachuan on 2018/10/9.
//

#ifndef PROJECT_NLEHELPER_H
#define PROJECT_NLEHELPER_H

#include "avpub/Rational.h"
#include "xengine/ITrack.h"
#include "xengine/GenerateSetting.h"
#include "xengine/ITimeLineObserver.h"
#include "xengine/IProject.h"
#include "NLEHeader.h"
#include "IInnerMedia.h"
#include "IInnerClip.h"

using namespace libav;

#define NLE_FPS 25

namespace xedit {
    class NLEHelper {
    public:
        /**
         * 将分数代表的帧率转化为EDsFrameRate
         * @param rFrameRate
         * @return
         */
        static EDsFrameRate convertFrameRate(Rational rFrameRate);

        /**
         * 将EDsFrameRate转换为Rational
         * @param frameRate
         * @return
         */
        static Rational convertFromDsFrameRate(EDsFrameRate frameRate);

        /**
         * 将分数代表的aspect ratio转化为EDsFrameRate
         * @param rAspectRatio
         * @return
         */
        static EDsAspectRatio convertAspectRatio(Rational rAspectRatio);

        /**
         * 转换扫描方式
         * @param bInterlaced 是否隔行扫描
         * @param bTopFieldFirst 是否顶场优先
         * @return
         */
        static EDsScanMode convertScanMode(bool  bInterlaced, bool  bTopFieldFirst);

        /**
         * 获得ESampleFormat
         * @param audioDataType
         * @param nBitsPerSample
         * @return
         */
        static ESampleFormat getSampleFormat(EDsAudioDataType audioDataType, int nBitsPerSample);

        /**
         *
         * @param scanMode
         * @return
         */
        static bool isInterlaced(EDsScanMode scanMode);

        /**
         * 轨道类型转换
         * @param trackType
         * @return
         */
        static EDsTrackType trackType2DsTrackType(ETrackType trackType);

        /**
         * 引擎状态转时间线状态
         * @param engineStatus
         * @return
         */
        static ETimeLineStatus dsEngineStatus2TimeLineStatus(EDs_EngineStatus engineStatus);

        /**
         * 从ProjectSetting获得SDsResolutionInfo
         * @param setting
         * @param pOutResInfo
         */
        static void getResolutionInfoFromProjectSetting(const ProjectSetting& setting, SDsResolutionInfo *pOutResInfo);

        /**
         * 将SDsFileInfo转换为ImageMedia
         * @param fileInfo
         * @param pOutImageMedia
         */
        static void convertSDsFileInfo2ImageMedia(const SDsFileInfo& fileInfo, IInnerImageMedia *pOutImageMedia);

        /**
         * 将ImageMedia转换为SDsFileInfo
         * @param pImageMedia
         * @param pOutFileInfo
         */
        //static void convertImageMedia2SDsFileInfo(const IImageMedia *pImageMedia, SDsFileInfo *pOutFileInfo);

        /**
         * 将SDsFileInfo转换为AVMedia
         * @param fileInfo
         * @param pOutAVMedia
         */
        static void convertSDsFileInfo2AVMedia(const SDsFileInfo& fileInfo, IInnerAVMedia *pOutAVMedia);

        /**
         * 将AVMedia转换为SDsFileInfo
         * @param pAVMedia
         * @param pOutFileInfo
         */
        //static void convertAVMedia2SDsFileInfo(const IAVMedia *pAVMedia, SDsFileInfo *pOutFileInfo);

        /**
         * 将Media转换为SDsFileInfo
         * @param pMedia
         * @param pOutFileInfo
         */
        //static void convertMedia2SDsFileInfo(const IMedia *pMedia, SDsFileInfo *pOutFileInfo);

        /**
         * 打开媒体
         * @param pImporter
         * @param mediaPath 媒体路径
         * @param bSequence 是否是序列
         * @param pOutFileInfo 输出文件信息
         * @return
         */
        static StatusCode openMedia(IDsImporter* pImporter, const char *mediaPath, bool bSequence, SDsFileInfo *pOutFileInfo);

        /**
         * 打开媒体
         * @param pImporter
         * @param mediaPath 媒体路径
         * @param bSequence 是否是序列
         * @param ppOutMedia 输出媒体
         * @return
         */
        static StatusCode openMedia(IDsImporter* pImporter, const char *mediaPath, bool bSequence, IInnerMedia **ppOutMedia);

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
        static StatusCode createClip(IDsClipFactory *pClipFactory,
                                     const SDsFileInfo& fileInfo,
                                     uint64_t nFrameOffsetOnTrack,
                                     uint64_t nFrameDurationTrack,
                                     uint64_t nMediaFrameStart,
                                     uint64_t nMediaFrameEnd,
                                     IDsFileClip **ppOutClip);

        /**
         * 创建clip
         * @param mediaId 媒体id
         * @param pClipFactory 切片工厂
         * @param fileInfo 文件信息
         * @param eClipType 切片类型
         * @param nFrameOffsetOnTrack 在轨道上的起始位置(帧偏移)
         * @param nFrameDurationTrack 在轨道上的长度(帧数)
         * @param nMediaFrameStart 在媒体上的起始位置(帧偏移)
         * @param nMediaFrameEnd 在媒体上的结束位置(帧偏移)
         * @param ppOutClip 输出切片
         * @return
         */
        static StatusCode createClip(ID mediaId,
                                     IDsClipFactory *pClipFactory,
                                     const SDsFileInfo& fileInfo,
                                     uint64_t nFrameOffsetOnTrack,
                                     uint64_t nFrameDurationTrack,
                                     uint64_t nMediaFrameStart,
                                     uint64_t nMediaFrameEnd,
                                     IInnerClip **ppOutClip);

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
        static StatusCode createAVClip(IDsClipFactory *pClipFactory,
                                       const SDsFileInfo& fileInfo,
                                       EFileType eFileType,
                                       int nStreamIndex,
                                       uint64_t nFrameOffsetOnTrack,
                                       uint64_t nFrameDurationTrack,
                                       uint64_t nMediaFrameStart,
                                       uint64_t nMediaFrameEnd,
                                       IDsFileClip **ppOutClip);

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
        static StatusCode createAVClip(ID mediaId,
                                       IDsClipFactory *pClipFactory,
                                       const SDsFileInfo& fileInfo,
                                       EClipType eClipType,
                                       int nStreamIndex,
                                       uint64_t nFrameOffsetOnTrack,
                                       uint64_t nFrameDurationTrack,
                                       uint64_t nMediaFrameStart,
                                       uint64_t nMediaFrameEnd,
                                       IInnerClip **ppOutClip);

        /**
         * 将生成设置转换为非编格式字符串
         * @param setting
         * @param pOutFormat
         */
        static void generateSettingToDsFormat(const GenerateSetting& setting, string *pOutFormat);

        /**
         * 将时间转换为帧号
         * @param rTime
         * @return
         */
        static int64_t timeToFrameIndex(const Rational& rTime);
    };

}


#endif //PROJECT_NLEHELPER_H
