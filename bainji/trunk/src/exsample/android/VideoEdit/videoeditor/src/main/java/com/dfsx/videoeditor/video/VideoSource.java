package com.dfsx.videoeditor.video;

import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.widget.timeline.IPlayerSource;

import java.io.Serializable;
import java.util.List;

/**
 * 视频源数据对象
 */
public class VideoSource implements IPlayerSource {
    public String sourcePath;
    public int width;
    public int height;
    public int degree;
    public String mintype;
    /**
     * 视频的真实长度
     */
    public long videoDuration;
    /**
     * 在播放器的位置
     */
    public int sourceWindowIndex;
    /**
     * 相对于整个时间线列表
     * 时间线上的开始时间
     */
    public long timeLineStartTime;

    /**
     * 相对于整个时间线列表
     * 时间线上的时间长度
     */
    private long timeLineDuration;

    public boolean tagSelected;
    /**
     * 相对于资源的时间的值
     * 当前对象的资源开始时间。一般为0， 当一个物理地址的视频分成几段的时候。用来标记物理资源的开始时间
     */
    public long sourceStartTime;
    /**
     * ms
     * 相对资源的数据
     * 当前对象代表的资源长度
     */
    public long sourceDuration;
    public List<FrameThumbInfo> frameThumbInfoList;

    /**
     * 相对于资源的一个值。
     * 标记播放器真实的播放时间区间
     */
    private long[] timeRange;

    /**
     * 一个毫秒代表的宽度值
     */
    private float oneTimeMSWidthRatio;

    private MediaSource engineMediaSource;

    public VideoSource() {

    }

    public VideoSource clone() {
        VideoSource videoSource = new VideoSource();
        videoSource.sourcePath = sourcePath;
        videoSource.sourceWindowIndex = sourceWindowIndex;
        videoSource.width = width;
        videoSource.height = height;
        videoSource.mintype = mintype;
        videoSource.timeLineStartTime = timeLineStartTime;
        videoSource.timeLineDuration = timeLineDuration;
        videoSource.oneTimeMSWidthRatio = oneTimeMSWidthRatio;
        videoSource.tagSelected = tagSelected;
        videoSource.timeRange = timeRange;
        videoSource.sourceDuration = sourceDuration;
        videoSource.sourceStartTime = sourceStartTime;
        videoSource.frameThumbInfoList = frameThumbInfoList;
        videoSource.engineMediaSource = engineMediaSource;
        return videoSource;
    }

    /**
     * 只克隆基础的视频信息
     *
     * @return
     */
    public VideoSource cloneBaseVideo() {
        VideoSource videoSource = new VideoSource();
        videoSource.sourcePath = sourcePath;
        videoSource.sourceWindowIndex = sourceWindowIndex;
        videoSource.width = width;
        videoSource.height = height;
        videoSource.mintype = mintype;
        videoSource.sourceDuration = sourceDuration;
        videoSource.oneTimeMSWidthRatio = oneTimeMSWidthRatio;

        return videoSource;
    }

    @Override
    public String getSourceUrl() {
        return sourcePath;
    }

    @Override
    public int getWidth() {
        return width;
    }

    @Override
    public int getHeight() {
        return height;
    }

    @Override
    public int getRotationDegree() {
        return degree;
    }

    @Override
    public void setWidth(int width) {
        this.width = width;
    }

    @Override
    public void setHeight(int height) {
        this.height = height;
    }

    @Override
    public void setRotationDegree(int rotationDegree) {
        this.degree = rotationDegree;
    }

    @Override
    public long[] getPlayTimeRange() {
        return timeRange;
    }

    @Override
    public void setPlayTimeRange(long[] timeRange) {
        if (timeRange != null && timeRange.length == 2) {
            if (this.timeRange == null) {
                this.timeRange = new long[2];
            }
            this.timeRange[0] = timeRange[0];
            this.timeRange[1] = timeRange[1];
        } else {
            this.timeRange = null;
        }
    }

    @Override
    public long getSourceTimeLineStartTime() {
        return timeLineStartTime;
    }

    @Override
    public long getSourceDuration() {
        return sourceDuration;
    }

    @Override
    public long getSourceStartTime() {
        return sourceStartTime;
    }

    @Override
    public int getSourceWindowIndex() {
        return sourceWindowIndex;
    }

    @Override
    public void setSourceWindowIndex(int index) {
        this.sourceWindowIndex = index;
    }

    public long getTimeLineDuration() {
        if (timeLineDuration == 0) {
            timeLineDuration = sourceDuration;
        }
        return timeLineDuration;
    }

    public void setTimeLineDuration(long timeLineDuration) {
        this.timeLineDuration = timeLineDuration;
    }

    public float getOneTimeMSWidthRatio() {
        return oneTimeMSWidthRatio;
    }

    public void setOneTimeMSWidthRatio(float oneTimeMSWidthRatio) {
        this.oneTimeMSWidthRatio = oneTimeMSWidthRatio;
    }

    public MediaSource getEngineMediaSource() {
        return engineMediaSource;
    }

    public void setEngineMediaSource(MediaSource engineMediaSource) {
        this.engineMediaSource = engineMediaSource;
    }
}
