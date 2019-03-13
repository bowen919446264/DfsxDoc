///////////////////////////////////////////////////////////
//  ITimeLine.h
//  Implementation of the Interface ITimeLine
//  Created on:      22-06-2018 16:58:43
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_9D349F31_1452_43d1_BED8_4F9911A59C4F__INCLUDED_)
#define EA_9D349F31_1452_43d1_BED8_4F9911A59C4F__INCLUDED_

#include "ITimeLineObserver.h"
#include "IRenderer.h"
#include "IMedia.h"
#include "ITrack.h"
#include "GenerateSetting.h"
#include "IGenerateObserver.h"
#include "IAction.h"
#include "xutil/IDictionary.h"

namespace xedit {
    class IXEngine;

    /**
     * 时间线
     */
    class ITimeLine {
    public:
        virtual ~ITimeLine(){}

        /**
         * 获得引擎
         * @return
         */
        virtual IXEngine* getEngine() const =0;

        /**
         * 获得最近一次错误代码
         * @return
         */
        virtual StatusCode getLastErrorCode() const =0;

        /**
         * 获得最近一次的错误信息
         * @return
         */
        virtual const char* getLastErrorMessage() const =0;

        /**
         * 是否可以做[重做]操作
         * @return
         */
        virtual bool canRedo() =0;

        /**
         * 是否可以做[撤销]操作
         * @return
         */
        virtual bool canUndo() =0;

        /**
         * 重做
         * @return 成功返回动作；否则返回NULL
         */
        virtual IAction* redo() =0;

        /**
         * 撤销
         * @return 成功返回动作；否则返回NULL
         */
        virtual IAction* undo() =0;

        /**
         * 获得时间线当前位置
         * @return
         */
        virtual Rational getCurrentPos() const =0;

        /**
         * 获得时间线长度
         * @return
         */
        virtual Rational getDuration() const =0;

        /**
         * 获得时间线上媒体数量
         * @return
         */
        virtual int getMediaCount() const =0;

        /**
         * 获得指定序号的媒体(序号从0开始)
         * @param nIndex 序号
         * @return 媒体或者NULL
         */
        virtual IMedia* getMedia(int nIndex) const =0;

        /**
         * 根据媒体id获得媒体
         * @param mediaId 媒体id
         * @return 媒体或者NULL
         */
        virtual IMedia* getMediaById(ID mediaId) const =0;

        /**
         * 添加媒体
         * @param path 媒体路径
         * @return 媒体或者NULL
         */
        virtual IMedia* addMedia(const char* path) =0;

        /**
         * 获得轨道数量
         * @param trackType 轨道类型
         * @return
         */
        virtual int getTrackCount(ETrackType trackType) const =0;

        /**
         * 获得指定序号的轨道
         * @param trackType 轨道类型
         * @param nIndex
         * @return 轨道或者NULL
         */
        virtual ITrack* getTrack(ETrackType trackType, int nIndex) const =0;

        /**
         * 获得指定id的轨道
         * @param trackId 轨道id
         * @return 轨道或者NULL
         */
        virtual ITrack* getTrack(ID trackId) const =0;

        /**
         * 新建轨道
         * @param trackType
         * @return 返回新建的轨道或者NULL
         */
        virtual ITrack* newTrack(ETrackType trackType) =0;

        /**
         * 删除轨道
         * @param id 轨道id
         */
        virtual void removeTrack(ID id) =0;

        /**
         * 删除指定轨道
         * @param pTrack
         */
        virtual void removeTrack(ITrack *pTrack) =0;

        /**
         * 查找切片
         * @param clipId 切片id
         * @return 切片或者NULL
         */
        virtual IClip* findClip(ID clipId) const =0;

        /**
         * 定位
         * @param rTime 目标时间
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode seek(Rational rTime) =0;

        /**
         * 播放
         */
        virtual void play() =0;

        /**
         * 暂停
         */
        virtual void pause() =0;

        /**
         * 添加一个观察者
         * @param pObserver 观察者
         */
        virtual void addObserver(ITimeLineObserver *pObserver) =0;

        /**
         * 移除指定观察者
         * @param pObserver 待移除的观察者
         */
        virtual void removeObserver(ITimeLineObserver *pObserver) =0;

        /**
         * 移除所有观察者
         */
        virtual void removeAllObservers() =0;

        /**
         * 获得renderer数量
         * @return
         */
        virtual int getRendererCount() const =0;

        /**
         * 获得指定序号的renderer(序号从0开始)
         * @param nIndex
         * @return renderer或者NULL
         */
        virtual IRenderer *getRenderer(int nIndex) const =0;

        /**
         * 添加一个renderer
         * @param pRenderer 待添加的renderer
         */
        virtual void addRenderer(IRenderer *pRenderer) =0;

        /**
         * 移除指定序号的renderer(序号从0开始)
         * @param nIndex
         */
        virtual void removeRenderer(int nIndex) =0;

        /**
         * 移除指定renderer
         * @param pRenderer 待移除的renderer
         */
        virtual void removeRenderer(IRenderer *pRenderer) =0;

        /**
         * 移除所有renderer
         */
        virtual void removeAllRenderers() =0;

        /**
         * 生成
         * 异步操作，调用后立即返回，并不代表生成已经完成
         * @param param 生成参数
         * @param pObserver 生成观察者
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode generate(const GenerateSetting &param, IGenerateObserver *pObserver = NULL) =0;

        /**
         * 取消生成
         */
        virtual void cancelGenerate() =0;

        /**
         * 清空所有媒体和切片信息
         */
        virtual void clear() =0;
    }; // end class ITimeLine

}

#endif // !defined(EA_9D349F31_1452_43d1_BED8_4F9911A59C4F__INCLUDED_)
