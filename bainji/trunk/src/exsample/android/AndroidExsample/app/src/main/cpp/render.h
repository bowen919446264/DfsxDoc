//
// Created by wendachuan on 2018/12/14.
//

#ifndef ANDROIDEXSAMPLE_RENDERVIDEOFRAME_H
#define ANDROIDEXSAMPLE_RENDERVIDEOFRAME_H

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "IBuffer.h"
using namespace xedit;

/**
 * 渲染视频帧
 * @param pWindow
 * @param pBuffer
 * @return
 */
StatusCode renderVideoFrame(ANativeWindow *pWindow, IVideoBuffer *pBuffer);

#endif //ANDROIDEXSAMPLE_RENDERVIDEOFRAME_H
