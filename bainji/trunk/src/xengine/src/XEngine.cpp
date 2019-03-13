//
// Created by wendachuan on 2018/6/15.
//

#include "XEngine.h"
#include "avpub/StatusCode.h"
#include "avpub/api.h"
#include "NLEHelper.h"

#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/filesystem.hpp>

using namespace libav;
using namespace xedit;
using namespace boost;

#define DEFAULT_PREVIEW_FRAME_SIZE GSize(240, 135)

XEngine::XEngine(): m_pTimeLine(NULL) {
    m_pTimeLine = new CTimeLine(this);
    m_engineSetting.previewFrameSize = DEFAULT_PREVIEW_FRAME_SIZE;
}

XEngine::~XEngine() {
    closeCurrentProject();
    m_pTimeLine = NULL;
}

/**
 * 初始化
 * @param setting
 * @return
 */
StatusCode XEngine::initialize(const EngineSetting &setting) {
    m_engineSetting = setting;
    StatusCode code = AVMakeDir(m_engineSetting.cacheDir);
    if (FAILED(code)) return code;

    code = AVMakeDir(m_engineSetting.logDir);
    if (FAILED(code)) return code;

    if (m_engineSetting.previewFrameSize.nWidth <= 0 || m_engineSetting.previewFrameSize.nWidth <= 0) {
        m_engineSetting.previewFrameSize = DEFAULT_PREVIEW_FRAME_SIZE;
    }
    return AV_OK;
}

/**
 * 获取引擎设置
 * @return
 */
EngineSetting XEngine::getEngineSetting() const {
    return m_engineSetting;
}

/**
 * 新建工程
 * 如果当前有工程已打开，则新建前会关闭当前已打开的工程
 * @param name 工程名称
 * @param setting 工程设置
 * @param nVersion 工程版本
 * @return 返回0表示成功；否则返回失败代码
*/
StatusCode XEngine::newProject(const char *name, const ProjectSetting &setting, int nVersion) {
    closeCurrentProject();

    StatusCode code = m_pTimeLine->initialize(setting);
    if (FAILED(code))
        return code;

    IInnerProject *project = createProject(setting, nVersion);
    if (!project) {
        return AV_OTHER_ERROR;
    }

    project->setName(name);
    m_pProject = project;
    return AV_OK;
}

/**
 * 获取当前工程
 * @return
 */
IProject* XEngine::getCurrentProject() const {
    return m_pProject;
}

/**
 * 打开工程
 * 如果当前有工程已打开，则打开前会关闭当前已打开的工程
 * @param pInputStream 工程文件输入流
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode XEngine::openProject(IInputStream *pInputStream) {
    closeCurrentProject();

    TSmartPtr<IInnerProject> project = loadProject(pInputStream);
    if (!project) {
        return AV_OTHER_ERROR;
    }

    // 时间线初始化
    StatusCode code = m_pTimeLine->initialize(project->getSetting());
    if (FAILED(code)) goto failure;

    // 载入媒体
    while (project->getMediaCount() > 0) {
        IMediaPtr pMedia = project->popMedia(0);
        m_pTimeLine->addMedia(pMedia);
    }

    // 如果音频轨道不够，则创建
    code = createTrackIfNeeded(project, ETrackType_Audio);
    if (FAILED(code)) goto failure;

    // 如果视频轨道不够，则创建
    code = createTrackIfNeeded(project, ETrackType_Video);
    if (FAILED(code)) goto failure;

    // 创建音频轨道IDsClip切片
    code = createDsClip(project, ETrackType_Audio);
    if (FAILED(code)) goto failure;

    // 创建视频轨道IDsClip切片
    code = createDsClip(project, ETrackType_Video);
    if (FAILED(code)) goto failure;

    m_pProject = project;
    return AV_OK;

failure:
    m_pTimeLine->clear();
    return code;
}

/**
 * 保存当前工程
 * @param pOutputStream 工程文件输出流
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode XEngine::saveProject(IOutputStream *pOutputStream) {
    if (m_pProject) {
        m_pProject->removeAllTracks();
        m_pProject->removeAllMedias();

        m_pProject->setSetting(m_pTimeLine->getSetting());
        m_pTimeLine->removeUnusedMedias();
        for (int i = 0; i < m_pTimeLine->getMediaCount(); i++) {
            IMedia *pMedia = m_pTimeLine->getMedia(i);
            if (!pMedia) return AV_OTHER_ERROR;

            IInnerMedia *pInnerMedia = dynamic_cast<IInnerMedia*>(pMedia);
            if (!pInnerMedia) return AV_OTHER_ERROR;

            m_pProject->addMedia(pInnerMedia);
        }
        for (int i = 0; i < m_pTimeLine->getTrackCount(ETrackType_Video); i++) {
            ITrack *pTrack = m_pTimeLine->getTrack(ETrackType_Video, i);
            if (!pTrack) return AV_OTHER_ERROR;

            IInnerTrack *pInnerTrack = dynamic_cast<IInnerTrack*>(pTrack);
            if (!pInnerTrack) return AV_OTHER_ERROR;

            m_pProject->addTrack(pInnerTrack);
        }
        for (int i = 0; i < m_pTimeLine->getTrackCount(ETrackType_Audio); i++) {
            ITrack *pTrack = m_pTimeLine->getTrack(ETrackType_Audio, i);
            if (!pTrack) return AV_OTHER_ERROR;

            IInnerTrack *pInnerTrack = dynamic_cast<IInnerTrack*>(pTrack);
            if (!pInnerTrack) return AV_OTHER_ERROR;

            m_pProject->addTrack(pInnerTrack);
        }

        StatusCode code = m_pProject->save(pOutputStream);
        m_pProject->popAllTracks();
        m_pProject->popAllMedias();
        return code;
    } else
        return AV_OTHER_ERROR;
}

/**
 * 关闭当前工程
 * @return 返回0表示成功；否则返回失败代码
 */
StatusCode XEngine::closeCurrentProject() {
    m_pTimeLine->uninitialize();
    m_pProject = NULL;
    return AV_OK;
}

/**
 * 获得时间线
 * @return
 */
ITimeLine* XEngine::getTimeLine() const {
    return m_pTimeLine;
}

/**
 * 如果轨道不够，则创建
 * @param trackType
 * @return
 */
StatusCode XEngine::createTrackIfNeeded(const IInnerProject *pProject, ETrackType trackType) {
    int nProjectAudioTrackCount = pProject->getTrackCount(trackType);
    int nCurrentAudioTrackCount = m_pTimeLine->getTrackCount(trackType);
    if (nCurrentAudioTrackCount < nProjectAudioTrackCount) {
        for (int i = nCurrentAudioTrackCount; i < nProjectAudioTrackCount; i++) {
            ITrackPtr pTrack = m_pTimeLine->newTrack(trackType);
            if (!pTrack) {
                return AV_OTHER_ERROR;
            }
        }
    }
    nCurrentAudioTrackCount = m_pTimeLine->getTrackCount(trackType);
    for (int i = 0; i < nCurrentAudioTrackCount; i++) {
        IInnerTrack* pTrack = dynamic_cast<IInnerTrack*>(m_pTimeLine->getTrack(trackType, i));
        if (!pTrack) {
            return AV_OTHER_ERROR;
        }

        ITrack *pProjectTrack = pProject->getTrack(trackType, i);
        pTrack->setId(pProjectTrack->getId());
    }
    return AV_OK;
}

/**
 * 创建IDsClip
 * @param pProject
 * @param trackType
 * @return
 */
StatusCode XEngine::createDsClip(IInnerProject *pProject, ETrackType trackType) {
    IDsClipFactory *pDsClipFactory = m_pTimeLine->getDsClipFactory();

    int nProjectTrackCount = pProject->getTrackCount(trackType);
    for (int i = 0; i <  nProjectTrackCount; i++) {
        IInnerTrack *pProjectTrack = dynamic_cast<IInnerTrack*>(pProject->getTrack(trackType, i));
        if (!pProjectTrack) return AV_OTHER_ERROR;

        IInnerTrack *pTimeLineTrack = dynamic_cast<IInnerTrack*>(m_pTimeLine->getTrack(trackType, i));
        if (!pTimeLineTrack) return AV_OTHER_ERROR;

        while (pProjectTrack->getClipCount() > 0) {
            IInnerClip *pClip = dynamic_cast<IInnerClip*>(pProjectTrack->getClip(0));
            if (!pClip) return AV_OTHER_ERROR;

            IMedia *pMedia = m_pTimeLine->getMediaById(pClip->getRefMediaId());
            if (!pMedia) return AV_OTHER_ERROR;

            IInnerMedia *pInnerMedia = dynamic_cast<IInnerMedia*>(pMedia);
            if (!pInnerMedia) return AV_OTHER_ERROR;

            SDsFileInfo fileInfo = {sizeof(SDsFileInfo)};
            pInnerMedia->getDsFileInfo(&fileInfo);

            uint64_t nFrameOffsetOnTrack = Rational(pClip->getOffsetOnTrack().nNum * NLE_FPS, pClip->getOffsetOnTrack().nDen).integerValue();
            uint64_t nFrameDurationTrack = Rational(pClip->getDuration().nNum * NLE_FPS, pClip->getDuration().nDen).integerValue();
            uint64_t nMediaFrameStart = 0;
            Rational rMediaFrameEnd = pClip->getDuration();
            if (pClip->getType() == EClipType_Audio || pClip->getType() == EClipType_Video) {
                IInnerAVClip *pAVClip = dynamic_cast<IInnerAVClip*>(pClip);
                if (!pAVClip) return AV_OTHER_ERROR;
                nMediaFrameStart = Rational(pAVClip->getOffsetInMedia().nNum * NLE_FPS, pAVClip->getOffsetInMedia().nDen).integerValue();
                rMediaFrameEnd = pAVClip->getOffsetInMedia() + pAVClip->getDuration();
            }

            uint64_t nMediaFrameEnd = Rational(rMediaFrameEnd.nNum * NLE_FPS, rMediaFrameEnd.nDen).integerValue();
            IDsFileClip *pDsFileClip = NULL;
            StatusCode code = NLEHelper::createClip(pDsClipFactory, fileInfo, nFrameOffsetOnTrack, nFrameDurationTrack, nMediaFrameStart, nMediaFrameEnd, &pDsFileClip);
            if (FAILED(code)) return code;

            pClip->bindDsClip(pDsFileClip);
            pTimeLineTrack->addClip(pClip);
            pProjectTrack->popClip(0);
        }
    }
    return AV_OK;
}

/**
 * 清理缓存
 */
void XEngine::clearCaches() {
    filesystem::remove_all(filesystem::path(m_engineSetting.cacheDir));
}

/**
 * 清理日志
 */
void XEngine::clearLogs() {
    filesystem::remove_all(filesystem::path(m_engineSetting.logDir));
}