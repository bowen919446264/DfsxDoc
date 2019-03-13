package com.dfsx.editengine;

import android.content.Context;
import android.view.Surface;

import com.dfsx.editengine.bean.EditEngineConfig;
import com.dfsx.editengine.bean.GenerateConfig;
import com.dfsx.editengine.bean.IActionChangeListener;
import com.dfsx.editengine.bean.IEngineGenerateListener;
import com.dfsx.editengine.bean.IEngineObserver;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.editengine.bean.ProjectCongfig;
import com.dfsx.editengine.bean.Render;

import java.util.List;

public interface IEditEngine {

    /**
     * 线路类型
     */
    enum TrackType {
        VIDEO,
        AUDIO
    }

    /**
     * 初始化引擎
     *
     * @param context
     * @param config
     */
    void init(Context context, EditEngineConfig config);

    /**
     * 添加渲染器
     *
     * @param renderer
     */
    void addRenderer(Render renderer);

    /**
     * 添加显示surface
     *
     * @param surface
     */
    void setUpWindow(Surface surface);

    /**
     * 创建项目
     *
     * @param projectName
     * @param projectCongfig
     * @throws Exception
     */
    void createProject(String projectName, ProjectCongfig projectCongfig) throws Exception;

    /**
     * 打开工程
     *
     * @param projectFilePath
     * @return
     */
    boolean openProject(String projectFilePath);

    /**
     * 保存工程
     *
     * @param projectFilePath
     * @return
     */
    boolean saveProject(String projectFilePath);

    /**
     * 默认添加到视频线路末尾位置
     *
     * @param sourcePath
     * @return
     */
    MediaSource addSource(String sourcePath);

    /**
     * 添加资源到指定位置
     *
     * @param sourcePath        资源路径
     * @param trackType         线路类型
     * @param trackIndex        线路下标
     * @param timeLineStartTime 线上的起始时间
     * @return
     */
    MediaSource addSourceInTimeLineTime(String sourcePath, IEditEngine.TrackType trackType, int trackIndex, long timeLineStartTime);

    /**
     * 添加资源
     *
     * @param addSource         添加的资源
     * @param trackType
     * @param trackIndex
     * @param timeLineStartTime 线上的起始时间
     * @return
     */
    MediaSource addSourceInTimeLineTime(MediaSource addSource, IEditEngine.TrackType trackType, int trackIndex, long timeLineStartTime);

    /**
     * 获取 track 总条数
     *
     * @param trackType
     * @return
     */
    int getTrackCount(TrackType trackType);

    /**
     * 获取线路上的所有资源
     *
     * @param trackType
     * @param trackCount
     * @return
     */
    List<MediaSource> getTrackSources(TrackType trackType, int trackCount);

    /**
     * 获取 线路上指定位置的资源
     *
     * @param sourceIndexInTimeLine 在时间线上的位置
     * @return
     */
    MediaSource getSource(TrackType trackType, int trackIndex, int sourceIndexInTimeLine);


    /**
     * 修改资源在Track上的开始时间位置
     *
     * @param startTime
     */
    MediaSource[] setMediaSourceStartTimeOnTrack(MediaSource mediaSource, long startTime);

    /**
     * 修改资源在Track上的开始位置
     *
     * @param mediaSource
     * @param offset      相对原来时间的偏移量 有正负
     */
    MediaSource[] setMediaSourceOffsetTimeOnTrack(MediaSource mediaSource, long offset);

    /**
     * 获取引擎播放的当前时间
     *
     * @return
     */
    long getCurrentPlayingTime();

    /**
     * 获取时间线的总时间
     *
     * @return
     */
    long getDuration();

    /**
     * 移除资源
     *
     * @param mediaSource
     * @return
     */
    boolean deleteSource(MediaSource mediaSource);

    /**
     * 开始播放
     */
    void play();

    /**
     * 暂停播放
     */
    void pause();

    /**
     * seek 到指定时间
     *
     * @param time
     * @return
     */
    boolean seekTo(long time);

    /**
     * 生成输出文件
     *
     * @param path             输出文件路劲
     * @param config           输出设置
     * @param startTime        时间线上的开始时间
     * @param durationTime     时间线上结束时间
     * @param generateListener 输出回调
     */
    void output(String path, GenerateConfig config, long startTime, long durationTime, IEngineGenerateListener generateListener);

    /**
     * 取消生成
     */
    void cancelGenerate();

    /**
     * 添加引擎的观察者
     *
     * @param engineObserver
     */
    void addEngineObserver(IEngineObserver engineObserver);

    void setEngineActionChangeListener(IActionChangeListener actionChangeListener);

    /**
     * 是否可以撤销
     *
     * @return
     */
    boolean canUnDo();

    /**
     * 是否可以反撤销
     *
     * @return
     */
    boolean canReDo();

    /**
     * 撤销
     */
    void unDo();

    /**
     * 反撤销
     */
    void reDo();


    /**
     * 关闭引擎，回收资源
     */
    void destroy();
}
