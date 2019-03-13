//
// Created by wendachuan on 2018/6/26.
//

#ifndef XENGINE_TIMELINE_H
#define XENGINE_TIMELINE_H

#include "xengine/ITimeLine.h"
#include <queue>
#include <deque>
#include "NLEHeader.h"
#include "IInnerAction.h"
#include "avpub/IThread.h"
#include "avpub/Log.h"
#include "avpub/TSmartPtr.h"
#include "IInnerTrack.h"
#include "IInnerClip.h"
#include "IInnerProject.h"
#include "TimeLineGenerator.h"

using namespace std;

namespace xedit {
    class XEngine;

    /**
     * 时间线实现类
     */
    class CTimeLine: public ITimeLine {
    public:

        CTimeLine(XEngine *pEngine);
        virtual ~CTimeLine();

        /**
         * 获得引擎
         * @return
         */
        virtual IXEngine* getEngine() const;

        /**
         * 获得最近一次错误代码
         * @return
         */
        virtual StatusCode getLastErrorCode() const;

        /**
         * 获得最近一次的错误信息
         * @return
         */
        virtual const char* getLastErrorMessage() const;

        /**
         * 执行动作
         * @param pAction 动作
         * @return 返回0表示成功；否则返回失败代码
         */
        StatusCode doAction(IInnerAction *pAction);

        /**
         * 是否可以做[重做]操作
         * @return
         */
        virtual bool canRedo();

        /**
         * 是否可以做[撤销]操作
         * @return
         */
        virtual bool canUndo();

        /**
         * 重做
         * @return 成功返回动作；否则返回NULL
         */
        virtual IAction* redo();

        /**
         * 撤销
         * @return 成功返回动作；否则返回NULL
         */
        virtual IAction* undo();

        /**
         * 获得时间线当前位置
         * @return
         */
        virtual Rational getCurrentPos() const;

        /**
         * 获得时间线长度
         * @return
         */
        virtual Rational getDuration() const;

        /**
         * 获得时间线上媒体数量
         * @return
         */
        virtual int getMediaCount() const;

        /**
         * 获得指定序号的媒体(序号从0开始)
         * @param nIndex 序号
         * @return 媒体或者NULL
         */
        virtual IMedia* getMedia(int nIndex) const;

        /**
         * 根据媒体id获得媒体
         * @param mediaId 媒体id
         * @return 媒体或者NULL
         */
        virtual IMedia* getMediaById(ID mediaId) const;

        /**
         * 添加媒体
         * @param path 媒体路径
         * @return 媒体或者NULL
         */
        virtual IMedia* addMedia(const char* path);

        /**
         * 添加媒体
         * @param pMedia
         */
        void addMedia(IMediaPtr pMedia);

        /**
         * 删除媒体
         * @param mediaId
         */
        void removeMediaById(ID mediaId);

        /**
         * 删除所有未使用媒体
         */
        void removeUnusedMedias();

        /**
         * 获得轨道数量
         * @param trackType 轨道类型
         * @return
         */
        virtual int getTrackCount(ETrackType trackType) const;

        /**
         * 获得指定序号的轨道
         * @param trackType 轨道类型
         * @param nIndex
         * @return
         */
        virtual ITrack* getTrack(ETrackType trackType, int nIndex) const;

        /**
         * 获得指定id的轨道
         * @param trackId 轨道id
         * @return
         */
        virtual ITrack* getTrack(ID trackId) const;

        /**
         * 新建轨道
         * @param trackType
         * @return
         */
        virtual ITrack* newTrack(ETrackType trackType);

        /**
         * 删除轨道
         * @param id 轨道id
         */
        virtual void removeTrack(ID id);

        /**
         * 删除指定轨道
         * @param pTrack
         */
        virtual void removeTrack(ITrack *pTrack);

        /**
         * 查找切片
         * @param clipId 切片id
         * @return 切片或者NULL
         */
        virtual IClip* findClip(ID clipId) const;

        /**
         * 定位
         * @param rTime 目标时间
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode seek(Rational rTime);

        /**
         * 播放
         */
        virtual void play();

        /**
         * 暂停
         */
        virtual void pause();

        /**
         * 添加一个观察者
         * @param pObserver 观察者
         */
        virtual void addObserver(ITimeLineObserver *pObserver);

        /**
         * 移除指定观察者
         * @param pObserver 待移除的观察者
         */
        virtual void removeObserver(ITimeLineObserver *pObserver);

        /**
         * 移除所有观察者
         */
        virtual void removeAllObservers();

        /**
         * 获得renderer数量
         * @param index
         * @return
         */
        virtual int getRendererCount() const;

        /**
         * 获得指定序号的renderer(序号从0开始)
         * @param nIndex
         * @return renderer或者NULL
         */
        virtual IRenderer *getRenderer(int nIndex) const;

        /**
         * 添加一个renderer
         * @param pRenderer 待添加的renderer
         */
        virtual void addRenderer(IRenderer *pRenderer);

        /**
         * 移除指定序号的renderer(序号从0开始)
         * @param nIndex
         */
        virtual void removeRenderer(int nIndex);

        /**
         * 移除指定renderer
         * @param pRenderer 待移除的renderer
         */
        virtual void removeRenderer(IRenderer *pRenderer);

        /**
         * 移除所有renderer
         */
        virtual void removeAllRenderers();

        /**
         * 生成
         * 异步操作，调用后立即返回，并不代表生成已经完成
         * @param param 生成参数
         * @param pObserver 生成观察者
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode generate(const GenerateSetting &param, IGenerateObserver *pObserver = NULL);

        /**
         * 取消生成
         */
        virtual void cancelGenerate();

        /**
         * 清空所有媒体和切片信息
         */
        virtual void clear();

    public:
        /**
         * 初始化
         * @param setting
         * @return
         */
        StatusCode initialize(const ProjectSetting &setting);

        /**
         * 释放资源
         */
        void uninitialize();

        /**
         * 获取工程设置
         * @return
         */
        ProjectSetting getSetting() const;

        TDsSmartPtr<IDsPlayEngineManager> getDsPlayEngineManager() const;
        TDsSmartPtr<IDsClipFactory> getDsClipFactory() const;
        TDsSmartPtr<IDsTimeline> getDsTimeline() const;
        TDsSmartPtr<IDsImporter> getDsImporter() const;
        TDsSmartPtr<IDsPlayer> getDsPlayer() const;
        TDsSmartPtr<IDsCapture> getDsCapture() const;

        /**
         * 记录错误日志
         * @param logLevel
         * @param code
         * @param format
         * @param ...
         */
        void logError(ELogLevel logLevel, StatusCode code, const char* format, ...);

        /**
         * 返回生成观察者
         * @return
         */
        IGenerateObserver* getGenerateObserver() const;

        /**
         * 返回生成设置
         */
        const GenerateSetting& getGenerateSetting() const;

    private:

        /**
         * 是否需要重新初始化非编引擎
         * @param setting
         * @return
         */
        bool needReinit(const ProjectSetting &setting);

        /**
         * 删除指定轨道
         * @param trackVector
         * @param trackId
         */
        void removeTrack(vector<IInnerTrack*>& trackVector, ID trackId);

        /**
         * 删除指定轨道
         * @param trackVector
         * @param pTrack
         */
        void removeTrack(vector<IInnerTrack*>& trackVector, ITrack *pTrack);

        /**
         * 播放器状态通知回调函数
         * @param engineStatus
         * @param nOperatorStatus
         * @return
         */
        static HRESULT notifyPlayerStatus(EDs_EngineStatus engineStatus, long nOperatorStatus);

        /**
         * 播放器视频render回调
         * @param pBuffer
         * @param nBufferSize
         * @param lpParam
         * @return
         */
        static HRESULT playerVideoCallback(BYTE* pBuffer, unsigned long nBufferSize, void* lpParam);

        /**
         * 视频render
         * @param pBuffer
         * @param nBufferSize
         */
        HRESULT renderVideo(BYTE* pBuffer, unsigned long nBufferSize);

        /**
         * 时间线状态通知
         * @param timeLineStatus
         * @param nValue
         */
        void notifyStatus(ETimeLineStatus timeLineStatus, long nValue);

    private:
        XEngine*    m_pEngine;
        StatusCode  m_nLastErrorCode;
        string      m_strLastErrorMsg;

        ProjectSetting          m_setting;
        vector<IMedia*>         m_medias;
        vector<IInnerTrack*>    m_videoTracks;
        vector<IInnerTrack*>    m_audioTracks;

        TDsSmartPtr<IDsPlayEngineManager>   m_pPlayEngineMgr;
        TDsSmartPtr<IDsClipFactory>         m_pClipFactory;
        TDsSmartPtr<IDsTimeline>            m_pTimeLine;
        TDsSmartPtr<IDsImporter>            m_pImporter;
        TDsSmartPtr<IDsPlayer>              m_pPlayer;
        TDsSmartPtr<IDsCapture>             m_pCapture;

        deque<IInnerAction*>     m_didActionStack;   // 已经执行成功的动作栈
        queue<IInnerAction*>     m_unDoActionQueue;  // 已经撤销执行的动作队列

        vector<ITimeLineObserver*>      m_observers;
        vector<IRenderer*>              m_renders;
        TSmartPtr<CTimeLineGenerator>   m_pGenerator;
        GenerateSetting                 m_generateSetting;
        IGenerateObserver               *m_pGenerateObserver;
    };

}

#endif //XENGINE_TIMELINE_H
