///////////////////////////////////////////////////////////
//  IXEngine.h
//  Implementation of the Interface IXEngine
//  Created on:      22-06-2018 16:58:43
//  Original author: wendachuan
///////////////////////////////////////////////////////////

#if !defined(EA_479B3143_D7E0_44cb_AFFF_549945FF4023__INCLUDED_)
#define EA_479B3143_D7E0_44cb_AFFF_549945FF4023__INCLUDED_

#include "avpub/Define.h"
#include "ITimeLine.h"
#include "IInputStream.h"
#include "IOutputStream.h"
#include "IProject.h"

namespace xedit {

    /**
     * 引擎设置
     */
    struct EngineSetting {
        // 缓存目录
        char    cacheDir[AV_MAX_PATH];

        // 日志目录
        char    logDir[AV_MAX_PATH];

        // 预览帧尺寸
        GSize   previewFrameSize;

        // 是否优先使用GPU解码(默认false)
        bool    useGpuToDecode;
    };

    /**
     * 引擎
     */
    class API_IMPORT_EXPORT IXEngine {
    public:
        virtual ~IXEngine() {}

        /**
         * 获得全局共享引擎实例
         * @return
         */
        static IXEngine *getSharedInstance();

        /**
         * 销毁全局共享引擎实例
         */
        static void destroySharedInstance();

        /**
         * 检测是否GPU比CPU快
         * @param sampleFilePath 用于性能检测的视频文件
         * @return true - GPU比CPU快; false - CPU比GPU快
         */
        static bool checkIfGpuFasterThanCpu(const char* sampleFilePath);

        /**
         * 初始化
         * @param setting
         * @return
         */
        virtual StatusCode initialize(const EngineSetting &setting) =0;

        /**
         * 获取引擎设置
         * @return
         */
        virtual EngineSetting getEngineSetting() const =0;

        /**
         * 新建工程
         * 如果当前有工程已打开，则新建前会关闭当前已打开的工程
         * @param name 工程名称
         * @param setting 工程设置
         * @param nVersion 工程版本
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode newProject(const char *name, const ProjectSetting &setting, int nVersion=0) =0;

        /**
         * 获取当前工程
         * @return
         */
        virtual IProject* getCurrentProject() const =0;

        /**
         * 打开工程
         * 如果当前有工程已打开，则打开前会关闭当前已打开的工程
         * @param pInputStream 工程文件输入流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode openProject(IInputStream *pInputStream) =0;

        /**
         * 保存当前工程
         * @param pOutputStream 工程文件输出流
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode saveProject(IOutputStream *pOutputStream) =0;

        /**
         * 关闭当前工程
         * @return 返回0表示成功；否则返回失败代码
         */
        virtual StatusCode closeCurrentProject() =0;

        /**
         * 获得时间线
         * @return
         */
        virtual ITimeLine *getTimeLine() const =0;

        /**
         * 清理缓存
         */
        virtual void clearCaches() =0;

        /**
         * 清理日志
         */
        virtual void clearLogs() =0;
    };

}
#endif // !defined(EA_479B3143_D7E0_44cb_AFFF_549945FF4023__INCLUDED_)
