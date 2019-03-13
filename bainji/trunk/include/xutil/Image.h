//
// Created by wendachuan on 2018/11/30.
//

#ifndef PROJECT_IMAGE_H
#define PROJECT_IMAGE_H

#include "GSize.h"
#include <string>
#include "avpub/EPixFormat.h"
#include "avpub/Define.h"
using namespace std;
using namespace libav;

namespace xedit {
    /**
     * 图片类型
     */
    enum EImageType {
        EImageType_Unknown,
        EImageType_PNG,
        EImageType_JPG,
        EImageType_GIF,
        EImageType_TGA
    };

    /**
     * 图片
     */
    struct Image {
        EImageType  type;
        string      path;   // 路径
        GSize       size;   // 尺寸
    };

    /**
     * 将图像buffer保存到图片
     * @param path 目标路径
     * @param pBuffer 图像buffer
     * @param srcFormat buffer格式
     * @param nSize buffer大小
     * @param nSrcLineSize 每行像素占用的字节数
     * @param srcSize 原图像尺寸
     * @param dstSize 将要生成的图片尺寸
     * @param dstImageType 将要生成的图片类型
     * @return
     */
    API_IMPORT_EXPORT
    StatusCode xSaveImage(const char* path, const uint8_t *pBuffer, EPixFormat srcFormat, int nSize, int nSrcLineSize, GSize srcSize, GSize dstSize, EImageType dstImageType);

    /**
     * 缩放图片
     * @param src 原图片路径
     * @param dst 目标图片路径
     * @param dstSize 目标尺寸
     * @param dstImageType 目标图片类型
     * @return
     */
    API_IMPORT_EXPORT
    StatusCode xScaleImage(const char* src, const char* dst, GSize dstSize, EImageType dstImageType = EImageType_Unknown);
}

#endif //PROJECT_IMAGE_H
