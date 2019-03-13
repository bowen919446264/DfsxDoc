//
// Created by wendachuan on 2018/6/26.
//

#include "TimeLine.h"
#include "avpub/StatusCode.h"
#include "NLEHelper.h"
#include "xutil/StdUtil.h"
#include <inttypes.h>
#include "DsVideoBuffer.h"
#include "action/AddMediaAction.h"
#include "action/RemoveMediaAction.h"
#include "XEngine.h"
#include "Track.h"

extern "C" {
#if __ANDROID__
#include <jni.h>
#include <libavcodec/jni.h>
#endif
}

using namespace libav;
using namespace xedit;

#define MAX_CACHE_ACTION_COUNT  100

/**
 * 默认构造函数
 */
CTimeLine::CTimeLine(XEngine *pEngine):
        m_pEngine(pEngine),
        m_pPlayEngineMgr(NULL),
        m_pClipFactory(NULL),
        m_pTimeLine(NULL),
        m_pImporter(NULL),
        m_pPlayer(NULL),
        m_pCapture(NULL),
        m_nLastErrorCode(AV_OK),
        m_pGenerator(NULL),
        m_pGenerateObserver(NULL)
{
    memset(&m_setting, 0, sizeof(ProjectSetting));
}

/**
 * 默认析构函数
 */
CTimeLine::~CTimeLine() {
    uninitialize();

    destoryVector(&m_videoTracks);
    destoryVector(&m_audioTracks);

    m_pGenerator = NULL;
    m_pGenerateObserver = NULL;
}

/**
 * 获得引擎
 * @return
 */
IXEngine* CTimeLine::getEngine() const {
    return m_pEngine;
}

/**
 * 清空所有媒体和切片信息
 */
void CTimeLine::clear() {
    if (m_pPlayer)
        m_pPlayer->Stop();

    destoryDeque(&m_didActionStack);
    destoryQueue(&m_unDoActionQueue);

    for (vector<IInnerTrack*>::iterator it = m_audioTracks.begin(); it != m_audioTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        pTrack->removeAllClips();
    }

    for (vector<IInnerTrack*>::iterator it = m_videoTracks.begin(); it != m_videoTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        pTrack->removeAllClips();
    }

    destoryVector(&m_medias);

    m_nLastErrorCode = AV_OK;
    m_strLastErrorMsg = "";
}

/**
 * 释放资源
 */
void CTimeLine::uninitialize() {
    clear();

    m_pCapture = NULL;
    m_pPlayer = NULL;
    m_pImporter = NULL;
    m_pTimeLine = NULL;
    m_pClipFactory = NULL;
    m_pPlayEngineMgr = NULL;

    if (m_pPlayEngineMgr) {
        m_pPlayEngineMgr->DestroyEngine();
    }

}

/**
 * 是否需要重新初始化非编引擎
 * @param setting
 * @return
 */
bool CTimeLine::needReinit(const ProjectSetting &setting) {
    return m_pPlayEngineMgr == NULL ||
            m_setting.nWidth != setting.nWidth ||
            m_setting.nHeight != setting.nHeight;
}

/**
 * 记录错误日志
 * @param logLevel
 * @param code
 * @param format
 * @param ...
 */
void CTimeLine::logError(ELogLevel logLevel, StatusCode code, const char* format, ...) {
    char szBuffer[10240] = {0};
    va_list pArgList;
    va_start(pArgList, format);
    vsnprintf(szBuffer, sizeof(szBuffer) - 1, format, pArgList);
    va_end(pArgList);

    AVLOG(logLevel, szBuffer);
    m_nLastErrorCode = code;
    m_strLastErrorMsg = szBuffer;
}

/**
 * 初始化
 * @param setting
 * @return
 */
StatusCode CTimeLine::initialize(const ProjectSetting &setting) {
    if (!needReinit(setting)) {
        clear();
        return AV_OK;
    }

    uninitialize();

    SDsResolutionInfo sRes = {0};
    SClipInfo timeLineInfo = { PlayEngine::EClipType::eClipTypeTimeline };
    TDsSmartPtr<IDsTrack> pVideoTrack, pAudioTrack;
    HRESULT hr;
#if __ANDROID__
    JavaVM *pJVM = g_pJVM;
    jsize nCount = 0;
    jint jret = 0;
    TDsSmartPtr<IDsCodecParams> pJParams;
#endif

    // 创建引擎管理器
    if ( !m_pPlayEngineMgr ) {
        hr = GetPlayEngineManager( &m_pPlayEngineMgr );
        if (FAILED(hr)) {
            logError(ELOG_LEVEL_FATAL, hr, "调用 GetPlayEngineManager 失败! 错误代码: %x", hr);
            goto failure;
        }
    }

    // 创建引擎
    NLEHelper::getResolutionInfoFromProjectSetting(setting, &sRes);
    hr = m_pPlayEngineMgr->CreateEngine(&sRes);
    if (FAILED(hr)) {
        logError(ELOG_LEVEL_FATAL, hr, "调用 CreateEngine 失败! 错误代码: %x", hr);
        goto failure;
    }

    if (m_pEngine->getEngineSetting().useGpuToDecode) {
#if __ANDROID__
        if (pJVM) {
            hr = m_pPlayEngineMgr->GetControlInterface(IID_IDsCodecParams, (void**)&pJParams);
            if (FAILED(hr)) {
                logError(ELOG_LEVEL_FATAL, hr, "调用 GetControlInterface 失败! 错误代码: %x", hr);
                goto failure;
            }
            pJParams->SetParam(2, pJVM);
        }
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#include <stdio.h>
#elif TARGET_OS_IPHONE
// iOS device
    static int whaterver = 0;
    hr = m_pPlayEngineMgr->GetControlInterface(IID_IDsCodecParams, (void**)&pJParams);
    if (FAILED(hr)) {
        logError(ELOG_LEVEL_FATAL, hr, "调用 GetControlInterface 失败! 错误代码: %x", hr);
        goto failure;
    }
    pJParams->SetParam(2, &whaterver);
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
    #error "Unknown Apple platform"
#endif
#endif
    }

    // 获得Clip Factory
    if (!m_pClipFactory)
        m_pClipFactory = m_pPlayEngineMgr->GetClipFactory();
    assert(m_pClipFactory != NULL);

    // 创建时间线
    hr = m_pClipFactory->CreateTimeline(&m_pTimeLine);
    if (FAILED(hr)) {
        logError(ELOG_LEVEL_FATAL, hr, "调用 CreateTimeline 失败! 错误代码: %x", hr);
        goto failure;
    }

    m_pTimeLine->SetClipInfo(timeLineInfo);

    // 设置活动时间线
    m_pClipFactory->SetMainTimeline(m_pTimeLine);

    m_pImporter = m_pPlayEngineMgr->CreateImporter();
    assert(m_pImporter != NULL);

    // 创建默认视频轨道
    if (m_videoTracks.size() == 0 && !newTrack(ETrackType_Video))
        goto failure;

    // 创建默认音频轨道
    if (m_audioTracks.size() == 0 && !newTrack(ETrackType_Audio))
        goto failure;

    // 获得播放器
    m_pPlayer = m_pPlayEngineMgr->GetPlayer();
    m_pPlayer->SetPlayerStatusNotify(notifyPlayerStatus);
    hr = m_pPlayer->SetLiveWindowCallback(playerVideoCallback, this);
    if (FAILED(hr)) {
        logError(ELOG_LEVEL_FATAL, hr, "调用 SetLiveWindowCallback 失败! 错误代码: %x", hr);
        goto failure;
    }

    m_setting = setting;
    return AV_OK;

failure:
    uninitialize();
    return hr;
}

/**
 * 获取工程设置
 * @return
 */
ProjectSetting CTimeLine::getSetting() const {
    return m_setting;
}

/**
 * 执行动作
 * @param pAction 动作
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode CTimeLine::doAction(IInnerAction *pAction) {
    StatusCode code = pAction->excute();
    if (SUCCESS(code)) {
        m_didActionStack.push_back(pAction);
        if (m_didActionStack.size() > MAX_CACHE_ACTION_COUNT) {
            IAction *pOldestAction = m_didActionStack.front();
            m_didActionStack.pop_front();
            delete pOldestAction;
        }
    }
    return code;
}

/**
 * 同步执行动作
 * @param pAction 动作
 * @return 返回0表示成功；否则返回失败代码
 */
//StatusCode CTimeLine::doAction(EActionType actionType, const IDictionary& actionParam) {
//    IAction *pAction = createAction(actionType, actionParam);
//    if (!pAction) {
//        return AV_OTHER_ERROR;
//    }
//    StatusCode code = pAction->excute();
//    if (SUCCESS(code)) {
//        m_didActionStack.push_back(pAction);
//        if (m_didActionStack.size() > MAX_CACHE_ACTION_COUNT) {
//            IAction *pOldestAction = m_didActionStack.front();
//            m_didActionStack.pop_front();
//            delete pOldestAction;
//        }
//    }
//    return code;
//}

/**
 * 获得最近一次错误代码
 * @return
 */
StatusCode CTimeLine::getLastErrorCode() const {
    return m_nLastErrorCode;
}

/**
 * 获得最近一次的错误信息
 * @return
 */
const char* CTimeLine::getLastErrorMessage() const {
    return m_strLastErrorMsg.c_str();
}

/**
 * 是否可以做[重做]操作
 * @return
 */
bool CTimeLine::canRedo() {
    return !m_unDoActionQueue.empty();
}

/**
 * 是否可以做[撤销]操作
 * @return
 */
bool CTimeLine::canUndo() {
    return !m_didActionStack.empty();
}

/**
 * 重做
 * @return 成功返回动作；否则返回NULL
 */
IAction* CTimeLine::redo() {
    if (canRedo()) {
        IInnerAction *pAction = m_unDoActionQueue.front();
        StatusCode code = pAction->excute();
        if (SUCCESS(code)) {
            m_unDoActionQueue.pop();
            m_didActionStack.push_back(pAction);
            return pAction;
        }
    }
    return NULL;
}

/**
 * 撤销
 * @return 成功返回动作；否则返回NULL
 */
IAction* CTimeLine::undo() {
    if (canUndo()) {
        IInnerAction *pAction = m_didActionStack.back();
        StatusCode code = pAction->unDo();
        if (SUCCESS(code)) {
            m_didActionStack.pop_back();
            m_unDoActionQueue.push(pAction);
            return pAction;
        }
    }
    return NULL;
}

/**
 * 获得时间线当前位置
 * @return
 */
Rational CTimeLine::getCurrentPos() const {
    if (m_pPlayer) {
        IDsPlayer *pDsPlayer = m_pPlayer;
        uint64_t nCurrentPos = pDsPlayer->GetPosition();
        return Rational(nCurrentPos, NLE_FPS);
    } else {
        return Rational();
    }
}

/**
 * 获得时间线长度
 * @return
 */
Rational CTimeLine::getDuration() const {
    const IDsTimeline *pTimeLine = m_pTimeLine;
    uint64_t nFrameDuration = const_cast<IDsTimeline*>(pTimeLine)->GetFrameEndPosition();
    return Rational(nFrameDuration, NLE_FPS);
}

/**
 * 获得时间线上媒体数量
 * @return
 */
int CTimeLine::getMediaCount() const {
    return m_medias.size();
}

/**
 * 获得指定序号的媒体(序号从0开始)
 * @param nIndex 序号
 * @return 媒体或者NULL
 */
IMedia* CTimeLine::getMedia(int nIndex) const {
    assert(nIndex < m_medias.size());
    if (nIndex < m_medias.size()) {
        return m_medias[nIndex];
    } else {
        return NULL;
    }
}

/**
 * 根据媒体id获得媒体
 * @param mediaId 媒体id
 * @return 媒体或者NULL
 */
IMedia* CTimeLine::getMediaById(ID mediaId) const {
    for (vector<IMedia*>::const_iterator it = m_medias.begin(); it != m_medias.end(); it++) {
        IMedia* media = *it;
        if (media->getId() == mediaId) {
            return media;
        }
    }
    return NULL;
}

/**
 * 添加媒体
 * @param path 媒体路径
 * @return 媒体或者NULL
 */
IMedia* CTimeLine::addMedia(const char* path) {
    CAddMediaAction *pAction = new CAddMediaAction(this, path);
    StatusCode code = doAction(pAction);
    if (FAILED(code)) {
        logError(ELOG_LEVEL_ERROR, code, "添加媒体[%s]失败!错误代码: %x", path, code);
        return NULL;
    }

    return getMediaById(pAction->getMediaId());
}

/**
 * 添加媒体
 * @param pMedia
 */
void CTimeLine::addMedia(IMediaPtr pMedia) {
    IInnerMedia *pInnerMedia = dynamic_cast<IInnerMedia*>(pMedia);
    pInnerMedia->setTimeLine(this);
    m_medias.push_back(pMedia);
}

/**
 * 删除媒体
 * @param mediaId
 */
void CTimeLine::removeMediaById(ID mediaId) {
    for (vector<IMedia*>::iterator it = m_medias.begin(); it != m_medias.end(); it++) {
        IMedia *pMedia = *it;
        if (pMedia->getId() == mediaId) {
            m_medias.erase(it);
            break;
        }
    }
}

/**
 * 删除所有未使用媒体
 */
void CTimeLine::removeUnusedMedias() {
    map<ID, int> mediaRef;
    for (vector<IMedia*>::const_iterator it = m_medias.begin(); it != m_medias.end(); it++) {
        IMedia *pMedia = *it;
        mediaRef[pMedia->getId()] = 0;
    }

    for (vector<IInnerTrack*>::const_iterator it = m_videoTracks.begin(); it != m_videoTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        for (int i = 0; i < pTrack->getClipCount(); i++) {
            IClip *pClip = pTrack->getClip(i);
            if (mediaRef.find(pClip->getRefMediaId()) != mediaRef.end()) {
                mediaRef[pClip->getRefMediaId()] = mediaRef[pClip->getRefMediaId()] + 1;
            } else {
                logError(ELOG_LEVEL_FATAL, AV_OTHER_ERROR, "切片[%" PRId64 "]对应的媒体[%" PRId64 "]不存在!!!", pClip->getId(), pClip->getRefMediaId());
            }
        }
    }

    for (map<ID, int>::const_iterator it = mediaRef.begin(); it != mediaRef.end(); it++) {
        if (it->second == 0) {
            removeMediaById(it->first);
        }
    }
}

/**
 * 获得轨道数量
 * @param trackType 轨道类型
 * @return
 */
int CTimeLine::getTrackCount(ETrackType trackType) const {
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
 * @param trackType 轨道类型
 * @param nIndex
 * @return
 */
ITrack* CTimeLine::getTrack(ETrackType trackType, int nIndex) const {
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
 * 获得指定id的轨道
 * @param trackId 轨道id
 * @return
 */
ITrack* CTimeLine::getTrack(ID trackId) const {
    for (vector<IInnerTrack*>::const_iterator it = m_videoTracks.begin(); it != m_videoTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        if (pTrack->getId() == trackId) {
            return pTrack;
        }
    }
    for (vector<IInnerTrack*>::const_iterator it = m_audioTracks.begin(); it != m_audioTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        if (pTrack->getId() == trackId) {
            return pTrack;
        }
    }
    return NULL;
}

/**
 * 新建轨道
 * @param trackType
 * @return
 */
ITrack* CTimeLine::newTrack(ETrackType trackType) {
    EDsTrackType dsTrackType = NLEHelper::trackType2DsTrackType(trackType);
    if (dsTrackType == eDsTrackerTypeInvalid) {
        logError(ELOG_LEVEL_ERROR, AV_OTHER_ERROR, "非法轨道类型[%d]!", trackType);
        return NULL;
    }

    // 创建轨道
    TDsSmartPtr<IDsTrack>  pDsTrack = NULL;
    int nTrackCount = m_pTimeLine->GetTrackCount(dsTrackType);
    StatusCode code = m_pClipFactory->CreateTrack(dsTrackType, nTrackCount, &pDsTrack);
    if (FAILED(code)) {
        logError(ELOG_LEVEL_ERROR, code, "调用 CreateTrack 失败!错误代码: %x", code);
        return NULL;
    }

    // 将轨道插入到时间线
    code = m_pTimeLine->InsertTrack(nTrackCount, pDsTrack);
    if (FAILED(code)) {
        logError(ELOG_LEVEL_ERROR, code, "调用 InsertTrack 失败!错误代码: %x", code);
        return NULL;
    }

    // 创建IInnerTrack
    pDsTrack->AddRef();
    CTrack *pTrack = new CTrack(this);
    pTrack->setTrackType(trackType);
    pTrack->bindDsTrack(pDsTrack);

    if (trackType == ETrackType_Audio)
        m_audioTracks.push_back(pTrack);
    else if (trackType == ETrackType_Video)
        m_videoTracks.push_back(pTrack);

    for (vector<ITimeLineObserver*>::const_iterator it = m_observers.begin(); it != m_observers.end(); it++) {
        ITimeLineObserver *pObserver = *it;
        pObserver->onTrackCreated(pTrack);
    }
    return pTrack;
}

/**
 * 删除指定轨道
 * @param trackVector
 * @param trackId
 */
void CTimeLine::removeTrack(vector<IInnerTrack*>& trackVector, ID trackId) {
    for (vector<IInnerTrack*>::iterator it = trackVector.begin(); it != trackVector.end(); it++) {
        ITrack *pTrack = *it;
        if (pTrack->getId() == trackId) {
            trackVector.erase(it);
            delete pTrack;

            for (vector<ITimeLineObserver*>::const_iterator it2 = m_observers.begin(); it2 != m_observers.end(); it2++) {
                ITimeLineObserver *pObserver = *it2;
                pObserver->onTrackRemoved(trackId);
            }
            return;
        }
    }
}

/**
 * 删除指定轨道
 * @param trackVector
 * @param pTrack
 */
void CTimeLine::removeTrack(vector<IInnerTrack*>& trackVector, ITrack *pTrack) {
    for (vector<IInnerTrack*>::iterator it = trackVector.begin(); it != trackVector.end(); it++) {
        if (*it == pTrack) {
            trackVector.erase(it);
            delete pTrack;
            return;
        }
    }
}

/**
 * 删除轨道
 * @param id 轨道id
 */
void CTimeLine::removeTrack(ID id) {
    removeTrack(m_audioTracks, id);
    removeTrack(m_videoTracks, id);
}

/**
 * 删除指定轨道
 * @param pTrack
 */
void CTimeLine::removeTrack(ITrack *pTrack) {
    removeTrack(m_audioTracks, pTrack);
    removeTrack(m_videoTracks, pTrack);
}

/**
 * 查找切片
 * @param clipId 切片id
 * @return 切片或者NULL
 */
IClip* CTimeLine::findClip(ID clipId) const {
    for (vector<IInnerTrack*>::const_iterator it = m_videoTracks.begin(); it != m_videoTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        IClip *pClip = pTrack->getClipById(clipId);
        if (pClip)
            return pClip;
    }
    for (vector<IInnerTrack*>::const_iterator it = m_audioTracks.begin(); it != m_audioTracks.end(); it++) {
        IInnerTrack *pTrack = *it;
        IClip *pClip = pTrack->getClipById(clipId);
        if (pClip)
            return pClip;
    }
    return NULL;
}

/**
 * 定位
 * @param rTime 目标时间
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode CTimeLine::seek(Rational rTime) {
    return m_pPlayer->Seek(AVRationalScale(NLE_FPS, rTime).integerValue());
}

/**
 * 播放
 */
void CTimeLine::play() {
    uint64_t nCurrentPos = m_pPlayer->GetPosition();
    const IDsTimeline *pTimeLine = m_pTimeLine;
    uint64_t nTotalFrames = const_cast<IDsTimeline*>(pTimeLine)->GetFrameEndPosition();
    HRESULT hr = m_pPlayer->Play(nCurrentPos, nTotalFrames - nCurrentPos, false);
    if (SUCCESS(hr)) {
        notifyStatus(ETimeLineStatus_Playing, 0);
    }
}

/**
 * 暂停
 */
void CTimeLine::pause() {
    m_pPlayer->Pause();
    notifyStatus(ETimeLineStatus_Pause, 0);
}

/**
 * 添加一个观察者
 * @param pObserver 观察者
 */
void CTimeLine::addObserver(ITimeLineObserver *pObserver) {
    m_observers.push_back(pObserver);
}

/**
 * 移除指定观察者
 * @param pObserver 待移除的观察者
 */
void CTimeLine::removeObserver(ITimeLineObserver *pObserver) {
    for (vector<ITimeLineObserver*>::iterator it = m_observers.begin(); it != m_observers.end(); it++) {
        if (*it == pObserver) {
            m_observers.erase(it);
            break;
        }
    }
}

/**
 * 移除所有观察者
 */
void CTimeLine::removeAllObservers() {
    m_observers.clear();
}

/**
 * 获得renderer数量
 * @param index
 * @return
 */
int CTimeLine::getRendererCount() const {
    return m_renders.size();
}

/**
 * 获得指定序号的renderer(序号从0开始)
 * @param nIndex
 * @return renderer或者NULL
 */
IRenderer* CTimeLine::getRenderer(int nIndex) const {
    if (nIndex < m_renders.size())
        return m_renders[nIndex];
    else
        return NULL;
}

/**
 * 添加一个renderer
 * @param pRenderer 待添加的renderer
 */
void CTimeLine::addRenderer(IRenderer *pRenderer) {
    m_renders.push_back(pRenderer);
}

/**
 * 移除指定序号的renderer(序号从0开始)
 * @param nIndex
 */
void CTimeLine::removeRenderer(int nIndex) {
    if (nIndex < m_renders.size()) {
        vector<IRenderer*>::iterator it = m_renders.begin() + nIndex;
        IRenderer *pRenderer = *it;
        m_renders.erase(it);
        delete pRenderer;
    }
}

/**
 * 移除指定renderer
 * @param pRenderer 待移除的renderer
 */
void CTimeLine::removeRenderer(IRenderer *pRenderer) {
    for (vector<IRenderer*>::iterator it = m_renders.begin(); it != m_renders.end(); it++) {
        if (*it == pRenderer) {
            m_renders.erase(it);
            break;
        }
    }
}

/**
 * 移除所有renderer
 */
void CTimeLine::removeAllRenderers() {
    m_renders.clear();
}

TDsSmartPtr<IDsPlayEngineManager> CTimeLine::getDsPlayEngineManager() const {
    return m_pPlayEngineMgr;
}

TDsSmartPtr<IDsClipFactory> CTimeLine::getDsClipFactory() const {
    return m_pClipFactory;
}

TDsSmartPtr<IDsTimeline> CTimeLine::getDsTimeline() const {
    return m_pTimeLine;
}

TDsSmartPtr<IDsImporter> CTimeLine::getDsImporter() const {
    return m_pImporter;
}

TDsSmartPtr<IDsPlayer> CTimeLine::getDsPlayer() const {
    return m_pPlayer;
}

TDsSmartPtr<IDsCapture> CTimeLine::getDsCapture() const {
    return m_pCapture;
}

/**
 * 播放器状态通知回调函数
 * @param engineStatus
 * @param nOperatorStatus
 * @return
 */
HRESULT CTimeLine::notifyPlayerStatus(EDs_EngineStatus engineStatus, long nOperatorStatus) {
    IXEngine *pEngine = IXEngine::getSharedInstance();
    CTimeLine *pTimeLine = dynamic_cast<CTimeLine*>(pEngine->getTimeLine());
    ETimeLineStatus timeLineStatus = NLEHelper::dsEngineStatus2TimeLineStatus(engineStatus);
    pTimeLine->notifyStatus(timeLineStatus, nOperatorStatus);
    return AV_OK;
}

/**
 * 时间线状态通知
 * @param timeLineStatus
 * @param nValue
 */
void CTimeLine::notifyStatus(ETimeLineStatus timeLineStatus, long nValue) {
    switch (timeLineStatus) {
        case ETimeLineStatus_Playing:
            AVLOG(ELOG_LEVEL_INFO, "播放中...");
            break;
        case ETimeLineStatus_PlayEnd:
            AVLOG(ELOG_LEVEL_INFO, "播放结束.");
            break;
        case ETimeLineStatus_DropFrame:
            logError(ELOG_LEVEL_ERROR, AV_OTHER_ERROR, "播放掉帧!帧号:%ld", nValue);
            break;
        case ETimeLineStatus_PlayFailed:
            logError(ELOG_LEVEL_ERROR, AV_OTHER_ERROR, "播放失败!");
            break;
        case ETimeLineStatus_Error:
            logError(ELOG_LEVEL_ERROR, AV_OTHER_ERROR, "未知错误!");
            break;
        case ETimeLineStatus_Generating:
            AVLOG(ELOG_LEVEL_INFO, "生成中...");
            break;
        case ETimeLineStatus_GenerateFinish:
            AVLOG(ELOG_LEVEL_INFO, "生成成功.");
            break;
        case ETimeLineStatus_GenerateFailed:
            AVLOG(ELOG_LEVEL_INFO, "生成失败!");
            break;
    }
    for (vector<ITimeLineObserver*>::const_iterator it = m_observers.begin(); it != m_observers.end(); it++) {
        ITimeLineObserver *pObserver = *it;
        pObserver->onTimeLineStatusChanged(timeLineStatus);
    }
}

/**
 * 播放器视频render回调
 * @param pBuffer
 * @param nBufferSize
 * @param lpParam
 * @return
 */
HRESULT CTimeLine::playerVideoCallback(BYTE* pBuffer, unsigned long nBufferSize, void* lpParam) {
    CTimeLine *pTimeLine = (CTimeLine*)lpParam;
    return pTimeLine->renderVideo(pBuffer, nBufferSize);
}

/**
 * 视频render
 * @param pBuffer
 * @param nBufferSize
 */
HRESULT CTimeLine::renderVideo(BYTE* pBuffer, unsigned long nBufferSize) {
    for (vector<IRenderer*>::iterator it = m_renders.begin(); it != m_renders.end(); it++) {
        IRenderer *pRenderer = *it;
        CDsVideoBuffer videoBuffer(pBuffer, nBufferSize, GSize(m_setting.nWidth, m_setting.nHeight), EPIX_FMT_RGBA);
        StatusCode code = pRenderer->render(&videoBuffer);
        if (FAILED(code)) return code;
    }
    return AV_OK;
}

/**
 * 生成
 * 异步操作，调用后立即返回，并不代表生成已经完成
 * @param param 生成参数
 * @param pObserver 生成观察者
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode CTimeLine::generate(const GenerateSetting &param, IGenerateObserver *pObserver) {
    m_generateSetting = param;
    m_pGenerateObserver = pObserver;
    m_pGenerator = new CTimeLineGenerator(this);
    return m_pGenerator->generate();
}

/**
 * 取消生成
 */
void CTimeLine::cancelGenerate() {
    if (m_pGenerator)
        m_pGenerator->cancel();
    m_pGenerator = NULL;
    m_pGenerateObserver = NULL;
}

/**
 * 返回生成观察者
 * @return
 */
IGenerateObserver* CTimeLine::getGenerateObserver() const {
    return m_pGenerateObserver;
}

/**
 * 返回生成设置
 */
const GenerateSetting& CTimeLine::getGenerateSetting() const {
    return m_generateSetting;
}