package com.dfsx.videoeditor.test;

import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

public class TimeLineVideo extends TimeLineVideoSource<VideoSource> {

    public TimeLineVideo(VideoSource data) {
        super(data);
    }

    @Override
    public void setTimeLineDuration(long timeLineDuration) {
        super.setTimeLineDuration(timeLineDuration);
        getItemData().setTimeLineDuration(timeLineDuration);
    }

    @Override
    public void setTimeLineStartTime(long timeLineStartTime) {
        super.setTimeLineStartTime(timeLineStartTime);
        getItemData().timeLineStartTime = timeLineStartTime;
    }
}
