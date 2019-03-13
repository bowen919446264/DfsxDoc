//
// Created by wendachuan on 2018/12/6.
//

#ifndef PROJECT_IPROJECTINNER_H
#define PROJECT_IPROJECTINNER_H

#include "xengine/IProject.h"
#include "IInnerTrack.h"
#include "IInnerMedia.h"

namespace xedit {
    class IInnerProject: public IProject {
    public:
        virtual ~IInnerProject() {}

        /**
		 * 保存工程
		 * @param pOutputStream 工程输出流
		 * @return
		 */
        virtual StatusCode save(IOutputStream *pOutputStream) =0;

        /**
         * 载入工程
         * @param projectBuffer 工程buffer
         * @param nBufferSize projectBuffer长度
         * @return
         */
        virtual StatusCode load(const char *projectBuffer, int nBufferSize) =0;

        /**
         * 载入工程
         * @param pInputStream 工程输入流
         * @return
         */
        virtual StatusCode load(IInputStream *pInputStream) =0;

        /**
         * 获得工程中媒体数量
         * @return
         */
        virtual int getMediaCount() const =0;

        /**
         * 获得指定序号的媒体
         * @param nIndex
         * @return
         */
        virtual IMedia *getMedia(int nIndex) const =0;

        /**
         * 获得轨道数量
         * @param trackType
         * @return
         */
        virtual int getTrackCount(ETrackType trackType) const =0;

        /**
         * 获得指定序号的轨道
         * @param trackType
         * @param nIndex
         * @return
         */
        virtual ITrack* getTrack(ETrackType trackType, int nIndex) const =0;

        /**
         * 设置工程名称
         * @param name
         */
        virtual void setName(const char* name) =0;

        /**
         * 设置工程设置
         * @return
         */
        virtual void setSetting(const ProjectSetting& setting) =0;

        /**
         * 添加媒体
         * @param media
         * @return
         */
        virtual StatusCode addMedia(IInnerMedia *media) =0;

        /**
         * 将指定序号的media从工程中移除并返回(不删除)
         * @param nIndex
         * @return
         */
        virtual IInnerMedia* popMedia(int nIndex) =0;

        /**
         * 删除所有媒体
         */
        virtual void removeAllMedias() =0;

        /**
         * 删除所有媒体，但不释放资源
         */
        virtual void popAllMedias() =0;

        /**
         * 添加轨道
         * @param track
         * @return
         */
        virtual StatusCode addTrack(IInnerTrack *track) =0;

        /**
         * 删除所有轨道
         */
        virtual void removeAllTracks() =0;

        /**
         * 删除所有轨道，但不释放资源
         */
        virtual void popAllTracks() =0;
    };


    /**
     * 创建工程
     * @param setting 工程参数
     * @param version 工程版本，<= 0 表示使用最新版本
     * @return
     */
    API_IMPORT_EXPORT
    IInnerProject* createProject(const ProjectSetting& setting, int nVersion = -1);

    /**
     * 载入工程
     * @param pInputStream 输入流
     * @return NULL表示载入失败，否则返回工程对象
     */
    API_IMPORT_EXPORT
    IInnerProject* loadProject(IInputStream *pInputStream);
}

#endif //PROJECT_IPROJECTINNER_H
