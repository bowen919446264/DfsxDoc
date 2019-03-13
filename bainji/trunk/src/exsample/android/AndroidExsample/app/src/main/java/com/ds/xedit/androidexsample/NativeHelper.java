package com.ds.xedit.androidexsample;

import android.view.Surface;

import com.ds.xedit.jni.AVMediaInfo;
import com.ds.xedit.jni.EPixFormat;
import com.ds.xedit.jni.IAVMedia;
import com.ds.xedit.jni.IBuffer;
import com.ds.xedit.jni.IClip;
import com.ds.xedit.jni.IMedia;
import com.ds.xedit.jni.IVideoRenderer;
import com.ds.xedit.jni.Rational;
import com.ds.xedit.jni.xeditJNI;

/**
 * 本地代码访问助手类
 */
public final class NativeHelper {
    /**
     * 初始化native window
     * @param surface
     * @param width
     * @param height
     * @return
     */
    public final static native int setupNativeWindow(Surface surface, int width, int height);

    /**
     * 渲染视频帧
     * @param bufferCPtr
     * @param videoBuffer
     * @return
     */
    public final static native int renderVideoFrame(long bufferCPtr, IBuffer videoBuffer);

    /**
     * 继承IBuffer，将getCPtr从protected改为public
     */
    public static class IBufferWrapper extends IBuffer {
        public static long getCPtr(IBuffer buffer) {
            return buffer.getCPtr(buffer);
        }
    }

    /**
     * 视频renderer
     */
    public static class VideoRenderer extends IVideoRenderer {
        @Override
        public int init(int nWidth, int nHeight, EPixFormat ePixFormat) {
            return 0;
        }

        @Override
        public int render(IBuffer pBuffer) {
            return renderVideoFrame(IBufferWrapper.getCPtr(pBuffer), pBuffer);
        }
    }
}
