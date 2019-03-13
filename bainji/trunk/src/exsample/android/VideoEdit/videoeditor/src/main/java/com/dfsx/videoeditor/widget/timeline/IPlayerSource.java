package com.dfsx.videoeditor.widget.timeline;

import com.dfsx.editengine.bean.MediaSource;

/**
 * 播放器的播放资源信息
 */
public interface IPlayerSource {

    /**
     * 播放器的播放地址
     *
     * @return
     */
    String getSourceUrl();

    int getWidth();

    int getHeight();

    int getRotationDegree();

    void setWidth(int width);

    void setHeight(int height);

    void setRotationDegree(int rotationDegree);

    /**
     * 获取播放的时间区间
     * 相对于资源的时间值
     * <p>
     * 标识在物理地址中的播放区间（用来控制播放器播放）
     * 下标0 表示开始时间， 下标1表示结束时间
     *
     * @return
     */
    long[] getPlayTimeRange();

    /**
     * 设置播放器的播放区间。相对于在资源的时间
     *
     * @param timeRange
     */
    void setPlayTimeRange(long[] timeRange);

    /**
     * 返回这个资源在时间线上的起始时间
     * <p>
     * 相对一整个时间线
     *
     * @return
     */
    long getSourceTimeLineStartTime();

    /**
     * 获取这个资源的时间长度
     * 相对于资源的时间
     *
     * @return
     */
    long getSourceDuration();

    /**
     * 相对于资源的时间值
     * 获取资源的开始时间。一般为0， 当有裁剪的时候此资源的起始时间
     *
     * @return
     */
    long getSourceStartTime();

    int getSourceWindowIndex();

    void setSourceWindowIndex(int index);

    /**
     * 编辑引擎 对应的资源数据
     *
     * @return
     */
    MediaSource getEngineMediaSource();
}
