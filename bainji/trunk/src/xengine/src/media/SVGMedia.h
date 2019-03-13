//
// Created by wendachuan on 2018/12/7.
//

#ifndef PROJECT_SVGMEDIA_H
#define PROJECT_SVGMEDIA_H

#include "BaseMedia.h"
#include <string>
using namespace std;

namespace xedit {
    class CSVGMedia: public CBaseMedia, public IInnerSVGMedia {
    public:
        CSVGMedia(CTimeLine *pTimeLine = NULL);
        virtual ~CSVGMedia();

        /**
         * 获得媒体类型
         * @return
         */
        EMediaType getMediaType() const;

        /**
         * 获得svg内容
         * @return
         */
        virtual const char* getSvg() const;

        /**
         * 设置svg内容
         * @param svg
         */
        virtual void setSvg(const char* svg);

        /**
         * 创建一个切片(调用者负责释放创建的切片)
         * @return
         */
        virtual IClip* newClip() const;

    private:
        string m_strSvg;
    };
}

#endif //PROJECT_SVGMEDIA_H
