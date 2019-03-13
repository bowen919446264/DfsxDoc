package com.dfsx.videoeditor.widget.timeline.bean;

import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;

/**
 * 简单的分割Item。可自己实现ITimeLineItem实现定义好的时间线分割内容
 */
public class TimeLineSplitAdd implements ITimeLineItem<TimeLineSplitAdd> {
    @Override
    public TimeLineSplitAdd getItemData() {
        return this;
    }

    @Override
    public long[] getTimeLineTime() {
        return null;
    }

    @Override
    public void setTimeLineTimeOffSet(long timeOffSet) {

    }

    @Override
    public ItemType getItemType() {
        return ItemType.TYPE_SPLIT;
    }
}
