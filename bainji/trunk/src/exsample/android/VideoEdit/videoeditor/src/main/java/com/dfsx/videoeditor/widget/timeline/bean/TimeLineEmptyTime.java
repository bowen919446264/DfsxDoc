package com.dfsx.videoeditor.widget.timeline.bean;

import com.dfsx.videoeditor.util.EditConstants;
import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;

/**
 * 时间线上的空时间数据
 */
public class TimeLineEmptyTime implements ITimeLineItem<TimeLineEmptyTime> {
    private long startTime;
    private long duration;

    /**
     * 见面展示宽度
     */
    private int viewWidth;

    private long[] timeRange = new long[2];

    /**
     * 空白时间数据
     *
     * @param startTime
     * @param duration
     */
    public TimeLineEmptyTime(long startTime, long duration) {
        this.startTime = startTime;
        this.duration = duration;
        onDurationChange();
    }

    public void onDurationChange() {
        viewWidth = calculateWidth();
    }

    private int calculateWidth() {
        int oneSecondWidth = EditConstants.ONE_SECOND_VIEW_WIDTH;
        return (int) (duration / 1000.0 * oneSecondWidth);
    }

    @Override
    public TimeLineEmptyTime getItemData() {
        return this;
    }

    @Override
    public long[] getTimeLineTime() {
        timeRange[0] = startTime;
        timeRange[1] = startTime + duration;
        return timeRange;
    }

    @Override
    public void setTimeLineTimeOffSet(long timeOffSet) {
        startTime = startTime + timeOffSet;
    }

    @Override
    public ItemType getItemType() {
        return ItemType.TYPE_EMPTY_TIME;
    }

    public int getViewWidth() {
        return viewWidth;
    }
}
