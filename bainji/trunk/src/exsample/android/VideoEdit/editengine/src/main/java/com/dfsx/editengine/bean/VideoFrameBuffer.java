package com.dfsx.editengine.bean;

import android.graphics.Bitmap;

public class VideoFrameBuffer {
    private int width;
    private int height;
    private byte[] bufferData;
    /**
     * 类型定义方式参考 @Bitmap.Config
     */
    private int bitmapConfigCount;

    public VideoFrameBuffer() {

    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public byte[] getBufferData() {
        return bufferData;
    }

    public void setBufferData(byte[] bufferData) {
        this.bufferData = bufferData;
    }

    public Bitmap.Config getConfig() {
        Bitmap.Config config = Bitmap.Config.RGB_565;
        switch (bitmapConfigCount) {
            case 1:
                config = Bitmap.Config.ALPHA_8;
                break;
            case 3:
                config = Bitmap.Config.RGB_565;
                break;
            case 4:
                config = Bitmap.Config.ARGB_4444;
                break;
            case 5:
                config = Bitmap.Config.ARGB_8888;
                break;
        }
        return config;
    }

    public int getBitmapConfigCount() {
        return bitmapConfigCount;
    }

    public void setBitmapConfigCount(int bitmapConfigCount) {
        this.bitmapConfigCount = bitmapConfigCount;
    }

    public boolean isEmpty() {
        return bufferData == null || bufferData.length <= 0;
    }

    public int getSize() {
        if (bufferData != null) {
            return bufferData.length;
        }
        return 0;
    }

    public String toString() {
        return "width == " + width + " ---- height == " + height + "---- size == " + bufferData.length;
    }
}
