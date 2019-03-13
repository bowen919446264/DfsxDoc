package com.dfsx.videoeditor.video;

import com.dfsx.videoeditor.util.EditConstants;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.ImageListView;

import java.io.Serializable;
import java.util.List;

/**
 * 每一片图片的数据信息，不占占大量内存
 * <p>
 * 时间的单位为Us
 */
public class FrameThumbInfo implements ImageListView.IFrameImage, Serializable {

    public String imagePath;

    public long startTime;

    public long durationTime;

    public int viewWidth;

    public List<FrameImageInfo> includeFrameInfoList;

    public FrameThumbInfo() {

    }

    public FrameThumbInfo(long startTime, long duration, String path) {
        this.startTime = startTime;
        this.durationTime = duration;
        this.imagePath = path;
        viewWidth = calculateWidth();
    }

    public void durationDataChanged() {
        viewWidth = calculateWidth();
    }

    private int calculateWidth() {
        int oneSecondWidth = EditConstants.ONE_SECOND_VIEW_WIDTH;
        return (int) (durationTime / 1000.0 * oneSecondWidth);
    }

    public FrameImageInfo getFrameImageInfo(long time) {
        if (includeFrameInfoList != null) {
            for (FrameImageInfo imageInfo : includeFrameInfoList) {
                if (imageInfo.time >= time) {
                    return imageInfo;
                }
            }
        }
        return null;
    }

    public FrameThumbInfo clone() {
        FrameThumbInfo thumbInfo = new FrameThumbInfo();
        thumbInfo.imagePath = imagePath;
        thumbInfo.startTime = startTime;
        thumbInfo.durationTime = durationTime;
        thumbInfo.viewWidth = viewWidth;
        thumbInfo.includeFrameInfoList = includeFrameInfoList;
        return thumbInfo;
    }

    @Override
    public int getFrameWidth() {
        return viewWidth;
    }

    @Override
    public int getFrameHeight() {
        return ITimeLineUI.TIMELINE_HEIGHT;
    }

    @Override
    public String getFrameImage() {
        return imagePath;
    }
}
