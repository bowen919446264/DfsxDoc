package com.dfsx.editengine.bean;

/**
 * 操作变化回调
 */
public interface IActionChangeListener {

    /**
     * 有资源添加进来了
     *
     * @param mediaSource
     * @param startTimeOfTrack
     */
    void onAddMediaSource(MediaSource mediaSource, long startTimeOfTrack);

    /**
     * 有资源改变长度
     *
     * @param sourceId
     * @param oldDuration
     * @param duration
     */
    void onChangeMediaSourceDuration(long sourceId, long oldDuration, long duration);

    /**
     * 资源在时间线上的时间改变了
     *
     * @param sourceId
     * @param oldStartTime
     * @param startTime
     */
    void onMediaSourceStartTimeChange(long sourceId, long oldStartTime, long startTime);

    /**
     * 有资源被删除了
     *
     * @param sourceId
     */
    void onMediaSourceRemove(long sourceId);

    /**
     * 资源开始播放位置变化
     *
     * @param sourceId
     * @param oldStartRange
     * @param startRange
     */
    void onMediaSourcePlayStartRangeChange(long sourceId, long oldStartRange, long startRange);
}
