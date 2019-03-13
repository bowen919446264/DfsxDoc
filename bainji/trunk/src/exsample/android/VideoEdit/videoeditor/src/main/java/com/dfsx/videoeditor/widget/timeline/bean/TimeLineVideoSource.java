package com.dfsx.videoeditor.widget.timeline.bean;

import com.dfsx.videoeditor.test.TimeLineVideo;
import com.dfsx.videoeditor.widget.timeline.IPlayerSource;
import com.dfsx.videoeditor.widget.timeline.ISelectedObject;
import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;
import com.dfsx.videoeditor.widget.timeline.ImageListView;

import java.util.List;

/**
 * 时间线的缩略图展示。
 * <p>
 * 所有的信息都需要在创建的时候直接设置
 *
 * @param <T> 包含組成信息
 */
public class TimeLineVideoSource<T> implements ITimeLineItem<T> {

    private T data;
    /**
     * 标记选中对象。如果是同步选中使用一个对象控制
     */
    private ISelectedObject selectedObject;

    private IPlayerSource playerSource;

    /**
     * 时间线的起始时间
     */
    private long timeLineStartTime;
    /**
     * 图片代表时间线的长度值
     */
    private long timeLineDuration;

    /**
     * 一个毫秒代表的宽度值
     */
    private float oneTimeMSWidthRatio;

    /**
     * 资源的数据
     */
    private List<ImageListView.IFrameImage> frameList;

    private long[] timeLineTime = new long[2];


    public TimeLineVideoSource(T data) {
        this.data = data;
    }

    @Override
    public T getItemData() {
        return data;
    }

    @Override
    public long[] getTimeLineTime() {
        timeLineTime[0] = timeLineStartTime;
        timeLineTime[1] = timeLineStartTime + timeLineDuration;
        return timeLineTime;
    }

    @Override
    public void setTimeLineTimeOffSet(long timeOffSet) {
        long startTime = this.timeLineStartTime + timeOffSet;
        setTimeLineStartTime(startTime);
    }

    @Override
    public ItemType getItemType() {
        return ItemType.TYPE_THUMB;
    }

    public ISelectedObject getSelectedObject() {
        return selectedObject;
    }

    public void setSelectedObject(ISelectedObject selectedObject) {
        this.selectedObject = selectedObject;
    }

    public long getTimeLineDuration() {
        return timeLineDuration;
    }

    public void setTimeLineDuration(long timeLineDuration) {
        this.timeLineDuration = timeLineDuration;
    }

    /**
     * 时间偏移量 + 设置的初始化开始时间
     *
     * @return
     */
    public long getTimeLineStartTime() {
        return timeLineStartTime;
    }

    public void setTimeLineStartTime(long timeLineStartTime) {
        this.timeLineStartTime = timeLineStartTime;
    }

    public IPlayerSource getPlayerSource() {
        return playerSource;
    }

    public void setPlayerSource(IPlayerSource playerSource) {
        this.playerSource = playerSource;
    }

    public List<ImageListView.IFrameImage> getFrameList() {
        return frameList;
    }

    public void setFrameList(List<ImageListView.IFrameImage> frameList) {
        this.frameList = frameList;
    }

    public float getOneTimeMSWidthRatio() {
        return oneTimeMSWidthRatio;
    }

    public void setOneTimeMSWidthRatio(float oneTimeMSWidthRatio) {
        this.oneTimeMSWidthRatio = oneTimeMSWidthRatio;
    }

    public String getFirstFrame() {
        int leftHide = 0;
        if (getPlayerSource().getPlayTimeRange() != null &&
                getPlayerSource().getPlayTimeRange().length == 2) {
            long sourceStartTime = getPlayerSource().getSourceStartTime();
            float onMsWidth = getOneTimeMSWidthRatio();
            leftHide = Math.round(onMsWidth * (getPlayerSource().getPlayTimeRange()[0] -
                    sourceStartTime));
        }
        if (getFrameList() == null || getFrameList().isEmpty()) {
            return null;
        }
        int indexWidth = 0;
        for (ImageListView.IFrameImage frameImage : getFrameList()) {
            indexWidth += frameImage.getFrameWidth();
            if (indexWidth >= Math.abs(leftHide)) {
                return frameImage.getFrameImage();
            }
        }
        return null;
    }
}
