//
// Created by wendachuan on 2019/1/10.
//

#include "xutil/Image.h"
#include <FreeImage/FreeImage.h>
#include "avpub/StatusCode.h"
#include "avpub/api.h"
#include "avpub/Log.h"
#include "avpub/AutoInitProxy.h"
#include <errno.h>
using namespace xedit;
using namespace libav;

static void outputMessageFunctionStdCall(FREE_IMAGE_FORMAT fif, const char *msg) {
    AVLOG(ELOG_LEVEL_ERROR, "%s System error code: %d, system error message: %s", msg, errno, strerror(errno));
}

/**
 * 初始化freeimage
 */
static void initFreeImage() {
    FreeImage_Initialise(TRUE);
    FreeImage_SetOutputMessageStdCall(outputMessageFunctionStdCall);
    FreeImage_SetOutputMessage(outputMessageFunctionStdCall);
}

/**
 * 卸载freeeimage
 */
static void deinitFreeImage() {
    FreeImage_DeInitialise();
}

// freeimage自动初始化和卸载对象
static CAutoInitProxy freeImageInitProxy(initFreeImage, deinitFreeImage);


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
StatusCode xedit::xSaveImage(const char* path, const uint8_t *pBuffer, EPixFormat srcFormat, int nSize, int nSrcLineSize, GSize srcSize,  GSize dstSize, EImageType dstImageType) {
    // 创建目录
    const char *p = strrchr(path, '/');
    char dir[AV_MAX_PATH] = {0};
    strncpy(dir, path, p - path);
    if ( !AVFileOrDirExist(dir) ) {
        StatusCode code = AVMakeDir(dir);
        if (FAILED(code)) {
            AVLOG(ELOG_LEVEL_ERROR, "创建目录[%s]失败!", dir);
            return AV_OTHER_ERROR;
        }
    }

    FREE_IMAGE_FORMAT imageFormat = FIF_UNKNOWN;
    switch (dstImageType) {
        case EImageType_JPG:
            imageFormat = FIF_JPEG;
            break;
        case EImageType_GIF:
            imageFormat = FIF_GIF;
            break;
        case EImageType_PNG:
            imageFormat = FIF_PNG;
            break;
    }
    if (imageFormat == FIF_UNKNOWN) {
        return AV_OTHER_ERROR;
    }

    FREE_IMAGE_TYPE imageType = FIT_UNKNOWN;
    int bpp;
    switch (srcFormat) {
        case EPIX_FMT_ABGR:
            imageType = FIT_BITMAP;
            bpp = 32;
            break;
    }

    if (imageType == FIT_UNKNOWN) {
        return AV_OTHER_ERROR;
    }

    StatusCode code = AV_OK;
    FIBITMAP *dibSrc = NULL;
    FIBITMAP *dibDst = NULL;
    if (!FreeImage_FIFSupportsExportType(imageFormat, imageType)) {
        code = AV_OTHER_ERROR;
        goto finish;
    }

    dibSrc = FreeImage_ConvertFromRawBitsEx(0, const_cast<BYTE*>(pBuffer), imageType, srcSize.nWidth, srcSize.nHeight, nSrcLineSize, bpp, 0, 0, 0, TRUE);
    if (!dibSrc) {
        code = AV_OTHER_ERROR;
        goto finish;
    }

    dibDst = FreeImage_Rescale(dibSrc, dstSize.nWidth, dstSize.nHeight, FILTER_BOX);
    FreeImage_Unload(dibSrc);
    if (!dibDst) {
        code = AV_OTHER_ERROR;
        goto finish;
    }

    if ( !FreeImage_Save(imageFormat, dibDst, path) ) {
        code = AV_OTHER_ERROR;
        goto finish;
    }

finish:
    if (dibDst)
        FreeImage_Unload(dibDst);
    return code;
}

/**
 * 缩放图片
 * @param src 原图片路径
 * @param dst 目标图片路径
 * @param dstSize 目标尺寸
 * @param dstImageType 目标图片类型
 * @return
 */
StatusCode xedit::xScaleImage(const char* src, const char* dst, GSize dstSize, EImageType dstImageType) {
    // 获取源格式
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    fif = FreeImage_GetFileType(src);
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(src);
    if (fif == FIF_UNKNOWN) return AV_OTHER_ERROR;

    if (!FreeImage_FIFSupportsReading(fif)) return AV_OTHER_ERROR;

    // 获取目标格式
    FREE_IMAGE_FORMAT imageFormat = fif;
    switch (dstImageType) {
        case EImageType_JPG:
            imageFormat = FIF_JPEG;
            break;
        case EImageType_GIF:
            imageFormat = FIF_GIF;
            break;
        case EImageType_PNG:
            imageFormat = FIF_PNG;
            break;
    }

    // 创建目录
    const char *p = strrchr(dst, '/');
    char dir[AV_MAX_PATH] = {0};
    strncpy(dir, dst, p - dst);
    if ( !AVFileOrDirExist(dir) ) {
        StatusCode code = AVMakeDir(dir);
        if (FAILED(code)) {
            AVLOG(ELOG_LEVEL_ERROR, "创建目录[%s]失败!", dir);
            return AV_OTHER_ERROR;
        }
    }

    FIBITMAP *srcFib = NULL, *dstFib = NULL;
    StatusCode code = AV_OTHER_ERROR;

    srcFib = FreeImage_Load(fif, src);
    if (!srcFib) goto end;

    dstFib = FreeImage_Rescale(srcFib, dstSize.nWidth, dstSize.nHeight);
    if (!dstFib) goto end;

    if (!FreeImage_Save(imageFormat, dstFib, dst)) goto end;

    code = AV_OK;
end:
    if (srcFib)
        FreeImage_Unload(srcFib);
    if (dstFib)
        FreeImage_Unload(dstFib);
    return code;
}