//
// Created by wendachuan on 2018/12/14.
//

#include "render.h"
#include <android/log.h>
#include <unistd.h>
#include "avpub/api.h"
#include "xengine/IMedia.h"

ANativeWindow* nativeWindow = NULL;

extern "C"
JNIEXPORT jint JNICALL
Java_com_ds_xedit_androidexsample_NativeHelper_setupNativeWindow(JNIEnv *jenv, jclass jcls, jobject surface, jint width, jint height) {
    nativeWindow = ANativeWindow_fromSurface(jenv, surface);
    if (!nativeWindow)
        return -1;
    return ANativeWindow_setBuffersGeometry(nativeWindow, width, height, WINDOW_FORMAT_RGBX_8888);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_ds_xedit_androidexsample_NativeHelper_renderVideoFrame(JNIEnv *jenv, jclass jcls, jlong bufferCPtr, jobject buffer) {
    xedit::IVideoBuffer *pVideoBuffer = *(xedit::IVideoBuffer **)&bufferCPtr;
    return renderVideoFrame(nativeWindow, pVideoBuffer);
}

/**
 * 渲染视频帧
 * @param pWindow
 * @param pBuffer
 * @return
 */
StatusCode renderVideoFrame(ANativeWindow *pWindow, IVideoBuffer *pBuffer) {
    //uint64_t nStartTime = libav::AVGetCurrentTime();

    ANativeWindow_Buffer windowBuffer;
    ANativeWindow_lock(pWindow, &windowBuffer, 0);

    // 获取stride
    uint8_t * dst = (uint8_t*)windowBuffer.bits;
    int dstStride = windowBuffer.stride * 4;
    uint8_t * src = pBuffer->getPlanePointer(0);
    int srcStride = pBuffer->getVideoRect().nWidth * 4;

    // 由于window的stride和帧的stride不同,因此需要逐行复制
    uint8_t * sl = src;
    uint8_t * dl = dst;
    uint8_t r,g,b,x;

    for (int h = 0; h < pBuffer->getVideoRect().nHeight; h++) {
        sl = src + h * srcStride;
        dl = dst + h * dstStride;

        //memcpy(dl, sl, pBuffer->getVideoRect().nWidth * 4);

        for(int w = 0 ; w < pBuffer->getVideoRect().nWidth; w ++) {
            b = *sl ++;
            g = *sl ++;
            r = *sl ++;
            x = *sl ++;

            *dl++ = r;
            *dl++ = g;
            *dl++ = b;
            *dl++ = x;
        }
    }

    ANativeWindow_unlockAndPost(pWindow);

    //uint64_t nSpendTime = libav::AVGetCurrentTime() - nStartTime;

    return 0;
}

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_ds_xedit_androidexsample_NativeHelper_getAVMediaInfo(JNIEnv *jenv, jclass jcls, jlong mediaCPtr, jobject media, jlong mediaInfoCPtr, jobject mediaInfo) {
//    xedit::IMedia *pMedia = *(xedit::IMedia **)&mediaCPtr;
//    xedit::IAVMedia *pAVMedia = dynamic_cast<IAVMedia*>(pMedia);
//    if (pAVMedia) {
//        libav::AVMediaInfo *pMediaInfo = *(libav::AVMediaInfo **)&mediaInfoCPtr;
//        pAVMedia->getMediaInfo(pMediaInfo);
//    }
//}

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_ds_xedit_androidexsample_NativeHelper_setAVClipOffsetInMedia(JNIEnv *jenv, jclass jcls, jlong clipCPtr, jobject clip, jlong offsetInMediaCPtr, jobject offsetInMedia) {
//    xedit::IClip *pClip = *(xedit::IClip **)&clipCPtr;
//    xedit::IAVClip *pAVClip = dynamic_cast<IAVClip*>(pClip);
//    if (pAVClip) {
//        libav::Rational *pRational = *(libav::Rational **)&offsetInMediaCPtr;
//        pAVClip->setOffsetInMedia(*pRational);
//    }
//}

//extern "C"
//JNIEXPORT void JNICALL
//Java_com_ds_xedit_androidexsample_NativeHelper_getAVClipOffsetInMedia(JNIEnv *jenv, jclass jcls, jlong clipCPtr, jobject clip, jlong offsetInMediaCPtr, jobject offsetInMedia) {
//    xedit::IClip *pClip = *(xedit::IClip **)&clipCPtr;
//    xedit::IAVClip *pAVClip = dynamic_cast<IAVClip*>(pClip);
//    if (pAVClip) {
//        libav::Rational *pRational = *(libav::Rational **)&offsetInMediaCPtr;
//        *pRational = pAVClip->getOffsetInMedia();
//    }
//}