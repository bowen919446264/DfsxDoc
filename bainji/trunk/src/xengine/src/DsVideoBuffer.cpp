//
// Created by wendachuan on 2018/12/14.
//

#include "DsVideoBuffer.h"

using namespace xedit;

CDsVideoBuffer::CDsVideoBuffer(uint8_t *data, int nSize, GSize rect, EPixFormat pixFormat) {
    m_pData = data;
    m_nSize = nSize;
    m_rect = rect;
    m_pixFormat = pixFormat;
}

CDsVideoBuffer::~CDsVideoBuffer() {

}

/**
 * 获得buffer类型
 * @return
 */
EBufferType CDsVideoBuffer::getType() const {
    return EBufferType_Video;
}

/**
 * 获得视频矩形
 * @return
 */
GSize CDsVideoBuffer::getVideoRect() const {
    return m_rect;
}

/**
 * 获得像素格式
 * @return
 */
EPixFormat CDsVideoBuffer::getPixelFormat() const {
    return m_pixFormat;
}

/**
 * 获得buffer位置
 * @return
 */
EBufferLocation CDsVideoBuffer::getLocation() const {
    return EBufferLocation_Memory;
}

/**
 * 获得平面数(YUV或者RGB平面)
 * @return
 */
int CDsVideoBuffer::getPlaneCount() const {
    return 1;
}

/**
 * 获得平面数据指针
 * @param nPlane
 * @return
 */
uint8_t* CDsVideoBuffer::getPlanePointer(int nPlane) const {
    return m_pData;
}

/**
 * 获得平面一行数据大小
 * @param nPlane
 * @return
 */
int CDsVideoBuffer::getPlaneLineSize(int nPlane) const {
    return m_nSize / m_rect.nHeight;
}

/**
 * 获得平面数据大小
 * @param nPlane
 * @return
 */
int CDsVideoBuffer::getSize(int nPlane) const {
    return m_nSize;
}