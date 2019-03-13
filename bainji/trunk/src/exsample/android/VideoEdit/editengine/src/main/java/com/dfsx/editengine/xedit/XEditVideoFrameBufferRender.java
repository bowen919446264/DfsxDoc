package com.dfsx.editengine.xedit;

import android.util.Log;

import com.dfsx.editengine.NativeHelper;
import com.dfsx.editengine.bean.VideoFrameBuffer;
import com.ds.xedit.jni.EPixFormat;
import com.ds.xedit.jni.IBuffer;
import com.ds.xedit.jni.IVideoRenderer;

public abstract class XEditVideoFrameBufferRender extends IVideoRenderer {
    private VideoFrameBuffer frameBuffer = new VideoFrameBuffer();

    @Override
    public int init(int nWidth, int nHeight, EPixFormat ePixFormat) {
        return 0;
    }

    @Override
    public int render(IBuffer pBuffer) {
        NativeHelper.getVideoFrameBuffer(NativeHelper.IBufferWrapper.getCPtr(pBuffer), pBuffer, frameBuffer);
        Log.e("Engine", "buffer info == " + frameBuffer.toString());
        renderFrame(frameBuffer);
        return 0;
    }

    public abstract void renderFrame(VideoFrameBuffer frameBuffer);
}