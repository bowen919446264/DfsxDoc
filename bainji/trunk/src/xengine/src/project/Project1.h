//
// Created by wendachuan on 2018/11/7.
//

#ifndef XEDIT_CPROJECT1_H
#define XEDIT_CPROJECT1_H

#include "IInnerProject.h"
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <vector>
#include "IInnerMedia.h"

using namespace std;

namespace xedit {
    /**
     * 工程（版本1）
     */
    class CProject1: public IInnerProject {
    public:
        CProject1();
        virtual ~CProject1();

        /**
		 * 获得工程版本号
		 * @return
		 */
        virtual int getVersion() const { return 1; }

        /**
         * 获得工程id
         * @return
         */
        virtual ID getId() const;

        /**
         * 获得工程名称
         * @return
         */
        virtual const char* getName() const;

        /**
         * 设置工程名称
         * @param name
         */
        virtual void setName(const char* name);

        /**
         * 获得工程设置
         * @return
         */
        virtual const ProjectSetting& getSetting() const;

        /**
         * 设置工程设置
         * @return
         */
        virtual void setSetting(const ProjectSetting& setting);

        /**
         * 保存工程
         * @param pOutputStream 工程输出流
         * @return
         */
        virtual StatusCode save(IOutputStream *pOutputStream);

        /**
         * 载入工程
         * @param inProjectBuffer 工程buffer
         * @param nBufferSize projectBuffer长度
         * @return
         */
        virtual StatusCode load(const char *inProjectBuffer, int nBufferSize);

        /**
         * 载入工程
         * @param pInputStream 工程输入流
         * @return
         */
        virtual StatusCode load(IInputStream *pInputStream);

        /**
         * 获得工程中媒体数量
         * @return
         */
        virtual int getMediaCount() const;

        /**
         * 获得指定序号的媒体
         * @param nIndex
         * @return
         */
        virtual IInnerMedia *getMedia(int nIndex) const;

        /**
         * 添加媒体
         * @param media
         * @return
         */
        virtual StatusCode addMedia(IInnerMedia *media);

        /**
         * 将指定序号的media从工程中移除并返回(不删除)
         * @param nIndex
         * @return
         */
        virtual IInnerMedia* popMedia(int nIndex);

        /**
         * 删除所有媒体
         */
        virtual void removeAllMedias();

        /**
         * 删除所有媒体，但不释放资源
         */
        virtual void popAllMedias();

        /**
		 * 获得轨道数量
		 * @param trackType
		 * @return
		 */
        virtual int getTrackCount(ETrackType trackType) const;

        /**
         * 获得指定序号的轨道
         * @param trackType
         * @param nIndex
         * @return
         */
        virtual ITrack* getTrack(ETrackType trackType, int nIndex) const;

        /**
         * 添加轨道
         * @param track
         * @return
         */
        virtual StatusCode addTrack(IInnerTrack *track);

        /**
         * 删除所有轨道
         */
        virtual void removeAllTracks();

        /**
         * 删除所有轨道，但不释放资源
         */
        virtual void popAllTracks();

    private:
        /**
         * 创建工程xml文档
         * @return
         */
        xmlDocPtr xmlCreateDoc();

        /**
         * 创建一个setting节点
         * @param projectSetting 工程设置
         * @return NULL表示创建失败，否则返回xml节点
         */
        static xmlNodePtr xmlCreateSettingNode(const ProjectSetting& projectSetting);

        /**
         * 创建一个audioOutput节点
         * @param projectSetting
         * @return
         */
        static xmlNodePtr xmlCreateAudioOutputNode(const ProjectSetting& projectSetting);

        /**
         * 创建一个videoOutput节点
         * @param projectSetting
         * @return
         */
        static xmlNodePtr xmlCreateVideoOutputNode(const ProjectSetting& projectSetting);

        /**
         * 创建一个medias节点
         * @param mediaVector
         * @return
         */
        static xmlNodePtr xmlCreateMediasNode(const std::vector<IInnerMedia*> mediaVector);

        /**
         * 创建timeline节点
         * @param trackVector
         * @return
         */
        static xmlNodePtr xmlCreateTimelineNode(const std::vector<ITrack*> trackVector);

        /**
         * 创建tracks节点
         * @param trackVector
         * @return
         */
        static xmlNodePtr xmlCreateTracksNode(const std::vector<ITrack*> trackVector);


        /**
         * 载入工程xml文档
         * @param str
         * @return
         */
        StatusCode xmlLoadDoc(const char* str);

        /**
         * 载入工程设置
         * @param settingNode
         * @param pOutSetting
         * @return
         */
        static StatusCode xmlLoadSetting(const xmlNodePtr settingNode, ProjectSetting *pOutSetting);

        /**
         * 载入工程音频输出设置
         * @param audioOutputNode
         * @param pOutSetting
         * @return
         */
        static StatusCode xmlLoadAudioOutput(const xmlNodePtr audioOutputNode, ProjectSetting *pOutSetting);

        /**
         * 载入工程视频输出设置
         * @param videoOutputNode
         * @param pOutSetting
         * @return
         */
        static StatusCode xmlLoadVideoOutput(const xmlNodePtr videoOutputNode, ProjectSetting *pOutSetting);

        /**
         * 载入媒体信息
         * @param mediaNode
         * @param ppOutMedia
         * @return
         */
        static StatusCode xmlLoadMedia(const xmlNodePtr mediaNode, IInnerMedia **ppOutMedia);

        /**
         * 载入轨道
         * @param trackNode
         * @param ppOutTrack
         * @return
         */
        static StatusCode xmlLoadTrack(const xmlNodePtr trackNode, ITrack **ppOutTrack);

        /**
         * 载入时间线
         * @param timelineNode
         * @return
         */
        StatusCode xmlLoadTimeLine(const xmlNodePtr timelineNode);

    private:
        ID              m_id;       // 工程id
        string          m_strName;  // 工程名称
        ProjectSetting  m_setting;  // 工程设置

        vector<IInnerTrack*>    m_videoTracks;
        vector<IInnerTrack*>    m_audioTracks;
        vector<IInnerMedia*>    m_medias;
    };
}

#endif //XEDIT_CPROJECT1_H
