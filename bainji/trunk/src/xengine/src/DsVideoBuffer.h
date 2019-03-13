//
// Created by wendachuan on 2018/12/14.
//

#ifndef PROJECT_DSVIDEOBUFFER_H
#define PROJECT_DSVIDEOBUFFER_H

#include "IBuffer.h"

namespace xedit {
    class CDsVideoBuffer: public IVideoBuffer {
    public:
        CDsVideoBuffer(uint8_t *data, int nSize, GSize rect, EPixFormat pixFormat);
        virtual ~CDsVideoBuffer();

        /**
	     * 获得buffer类型
	     * @return
	     */
        virtual EBufferType getType() const;

        /**
		 * 获得视频矩形
		 * @return
		 */
        virtual GSize getVideoRect() const;

        /**
         * 获得像素格式
         * @return
         */
        virtual EPixFormat getPixelFormat() const;

        /**
         * 获得buffer位置
         * @return
         */
        virtual EBufferLocation getLocation() const;

        /**
         * 获得平面数(YUV或者RGB平面)
         * @return
         */
        virtual int getPlaneCount() const;

        /**
         * 获得平面数据指针
         * @param nPlane
         * @return
         */
        virtual uint8_t* getPlanePointer(int nPlane) const;

        /**
		 * 获得平面一行数据大小
		 * @param nPlane
		 * @return
		 */
        virtual int getPlaneLineSize(int nPlane) const;

        /**
         * 获得平面数据大小
         * @param nPlane
         * @return
         */
        virtual int getSize(int nPlane) const;

    private:
        uint8_t*        m_pData;
        int             m_nSize;
        GSize           m_rect;
        EPixFormat      m_pixFormat;
    };
}

#endif //PROJECT_DSVIDEOBUFFER_H
