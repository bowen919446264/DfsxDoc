//
// Created by wendachuan on 2018/6/15.
//

#ifndef XENGINE_XENGINE_H
#define XENGINE_XENGINE_H

#include "xengine/IXEngine.h"
#include "IInnerProject.h"
#include "TimeLine.h"
#include "avpub/TSmartPtr.h"

namespace xedit {
    /**
     * 引擎实现类
     */
    class XEngine: public IXEngine {
    public:

        XEngine();
        virtual ~XEngine();

        /**
         * 初始化
         * @param setting
         * @return
         */
        virtual StatusCode initialize(const EngineSetting &setting);

        /**
         * 获取引擎设置
         * @return
         */
        virtual EngineSetting getEngineSetting() const;

        /**
         * 新建工程
         * 如果当前有工程已打开，则新建前会关闭当前已打开的工程
         * @param name 工程名称
         * @param setting 工程设置
         * @param nVersion 工程版本
         * @return 返回0表示成功；否则返回失败代码
       */
        virtual StatusCode newProject(const char *name, const ProjectSetting &setting, int nVersion=0);

        /**
         * 获取当前工程
         * @return
         */
        virtual IProject* getCurrentProject() const;

        /**
         * 打开工程
         * 如果当前有工程已打开，则打开前会关闭当前已打开的工程
         * @param pInputStream 工程文件输入流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode openProject(IInputStream *pInputStream);

        /**
         * 保存当前工程
         * @param pOutputStream 工程文件输出流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode saveProject(IOutputStream *pOutputStream);

        /**
         * 关闭当前工程
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode closeCurrentProject();

        /**
         * 获得时间线
         * @return
         */
        virtual ITimeLine *getTimeLine() const;

        /**
         * 清理缓存
         */
        virtual void clearCaches();

        /**
         * 清理日志
         */
        virtual void clearLogs();

    private:
        /**
         * 如果轨道不够，则创建
         * @param trackType
         * @return
         */
        StatusCode createTrackIfNeeded(const IInnerProject *pProject, ETrackType trackType);

        /**
         * 创建IDsClip
         * @param pProject
         * @param trackType
         * @return
         */
        StatusCode createDsClip(IInnerProject *pProject, ETrackType trackType);

    private:
        TSmartPtr<CTimeLine>        m_pTimeLine;
        TSmartPtr<IInnerProject>    m_pProject;
        EngineSetting               m_engineSetting;
    };
}

#if __ANDROID__
#include <jni.h>
#endif

extern JavaVM *g_pJVM;

#endif //XENGINE_XENGINE_H
