//
// Created by wendachuan on 2018/11/29.
//

#ifndef PROJECT_BASEMEDIA_H
#define PROJECT_BASEMEDIA_H

#include "avpub/TSmartPtr.h"
#include "IInnerMedia.h"
#include "xutil/id.h"
#include <string>

using namespace std;
using namespace libav;

namespace xedit {
    class CTimeLine;
    /**
     * 媒体基类
     */
    class CBaseMedia: virtual public IInnerMedia {
    public:
        CBaseMedia(CTimeLine *pTimeLine = NULL);
        virtual ~CBaseMedia();

        /**
         * 获得媒体id
         * @return
         */
        virtual ID getId() const;

        /**
         * 设置媒体id
         * @param id
         */
        virtual void setId(ID id);

        /**
         * 获得媒体路径
         * @return
         */
        virtual const char* getPath() const;

        /**
         * 设置媒体路径
         * @param path
         */
        virtual void setPath(const char* path);

        /**
         * 设置SDsFileInfo
         * @param fileInfo
         */
        virtual void setDsFileInfo(const SDsFileInfo& fileInfo);

        /**
         * 获取SDsFileInfo
         * @param pOutFileInfo
         */
        virtual void getDsFileInfo(SDsFileInfo *pOutFileInfo) const;

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @param rDuration 切片时长(秒)
         * @return
         */
        virtual IClip* newClip(Rational rDuration) const;

        /**
         * 设置时间线
         * @param pTimeLine
         */
        virtual void setTimeLine(ITimeLine *pTimeLine);

        /**
         * 获得媒体预览
         * @param nStreamIndex 流序号
         * @return
         */
        virtual IPreview* getPreview() const;

        /**
         * 设置预览
         * @param pPreview
         */
        virtual void setPreview(IInnerPreview *pPreview);

    protected:
        /**
         * 获得预览目录
         * @param path
         */
        void getPreviewDir(char path[]) const;

    protected:
        CTimeLine* m_pTimeLine;
        ID      m_id;
        string  m_path;
        SDsFileInfo m_fileInfo;
    };
}



#endif //PROJECT_BASEMEDIA_H
