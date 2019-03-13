package com.dfsx.videoeditor.video;

/**
 * 每一帧的数据
 */
public class FrameImageInfo {
    public long time;

    public String imagePath;

    public FrameImageInfo() {

    }

    public FrameImageInfo(long time, String imagePath) {
        this.time = time;
        this.imagePath = imagePath;
    }
}
