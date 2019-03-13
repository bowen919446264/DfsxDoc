package com.dfsx.videoeditor.bean;

import com.dfsx.videoeditor.video.FrameThumbInfo;

public class FrameAdapterData implements IAdapterItem<FrameThumbInfo> {
    private FrameThumbInfo frameInfo;
    private Object tag;
    /**
     * 在时间线上显示的时间
     */
    private long adapterStartTime;

    public FrameAdapterData(FrameThumbInfo frameInfo, Object tag, long startTime) {
        this.frameInfo = frameInfo;
        this.tag = tag;
        this.adapterStartTime = startTime;
    }

    @Override
    public ItemType getItemType() {
        return ItemType.TYPE_FRAME;
    }

    @Override
    public FrameThumbInfo getItemData() {
        return frameInfo;
    }

    public Object getTag() {
        return tag;
    }

    public long getAdapterStartTime() {
        return adapterStartTime;
    }
}
