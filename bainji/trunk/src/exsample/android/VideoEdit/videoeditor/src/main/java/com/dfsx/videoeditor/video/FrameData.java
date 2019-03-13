package com.dfsx.videoeditor.video;

import android.graphics.Bitmap;

/**
 * 指定时间的图像数据
 */
public class FrameData {

    /**
     * 毫秒ms
     */
    public long time;
    public Bitmap frameBitmap;

    public FrameData(long time, Bitmap bitmap) {
        this.time = time;
        this.frameBitmap = bitmap;
    }

    public void release() {
        if (frameBitmap != null) {
            frameBitmap.recycle();
        }
    }
}
