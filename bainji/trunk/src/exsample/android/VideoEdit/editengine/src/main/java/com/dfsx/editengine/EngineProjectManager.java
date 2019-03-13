package com.dfsx.editengine;

import android.content.Context;
import android.os.Looper;
import android.util.Log;
import android.view.Surface;

import com.dfsx.editengine.bean.EditEngineConfig;
import com.dfsx.editengine.bean.GenerateConfig;
import com.dfsx.editengine.bean.IActionChangeListener;
import com.dfsx.editengine.bean.IEngineGenerateListener;
import com.dfsx.editengine.bean.IEngineObserver;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.editengine.bean.ProjectCongfig;
import com.dfsx.editengine.bean.Render;
import com.dfsx.editengine.util.EngineUtil;
import com.dfsx.editengine.util.LogUtil;
import com.dfsx.editengine.xedit.XEditEngine;
import com.ds.xedit.jni.ITrack;
import com.ds.xedit.jni.Rational;

import java.io.OutputStream;
import java.util.List;

/**
 * 项目的管理代理类。 项目管理全通过此进行调用
 * Xedit 引擎主要运行在EngineThread 中，注意线程控制
 *
 * @bowen liu
 */
public class EngineProjectManager /*implements IEditEngine*/ {

    private Context context;

    private EngineThread engineThread;
    private IEditEngine editEngine;

    public EngineProjectManager(Context context) {
        this.context = context;
        engineThread = new EngineThread();
        editEngine = new XEditEngine();
    }

    public void initSync(EditEngineConfig config) {
        editEngine.init(context, config);
    }

    public void init(EditEngineConfig config) {
        engineThread.excuteTask(new BaseEngineTask<Void>(config) {
            @Override
            public Void run(Object... params) {
                editEngine.init(context, (EditEngineConfig) params[0]);
                return null;
            }
        }, null);
    }

    /**
     * 把任务放在 引擎线程里执行
     *
     * @param task
     * @param callback
     * @param <E>
     */
    public <E> void excuteTask(EngineThread.IEngineTask<E> task, EngineThread.IEngineMainCallback<E> callback) {
        engineThread.excuteTask(task, callback);
    }

    /**
     * 直接获取 引擎线程 的Looper
     * 可以用来创建Rx 的Scheduler such as #AndroidSchedulers.from(getEngineThreadLooper());
     *
     * @return
     */
    public Looper getEngineThreadLooper() {
        return engineThread.getEngineThreadLooper();
    }

    /**
     * 同步执行
     *
     * @param name
     * @param congfig
     * @throws Exception
     */
    public void create(String name, ProjectCongfig congfig) throws Exception {
        editEngine.createProject(name, congfig);
    }

    public void create(String name, ProjectCongfig congfig, EngineThread.IEngineMainCallback<Boolean> callback) {
        excuteTask(new BaseEngineTask<Boolean>(name, congfig) {
            @Override
            public Boolean run(Object... params) {
                try {
                    editEngine.createProject((String) params[0], (ProjectCongfig) params[1]);
                    return true;
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return false;
            }
        }, callback);
    }

    /**
     * 同步打开
     *
     * @param projFilePath
     * @return
     */
    public boolean openProject(String projFilePath) {
        return editEngine.openProject(projFilePath);
    }

    /**
     * 同步保存
     *
     * @param savePath
     * @return
     */
    public boolean saveProject(String savePath) {
        return editEngine.saveProject(savePath);
    }

    public void openProject(String projFilePath, EngineThread.IEngineMainCallback<Boolean> callback) {
        excuteTask(new BaseEngineTask<Boolean>(projFilePath) {
            @Override
            public Boolean run(Object... params) {
                return editEngine.openProject((String) params[0]);
            }
        }, callback);
    }

    public void saveProject(String savePath, EngineThread.IEngineMainCallback<Boolean> callback) {
        excuteTask(new BaseEngineTask<Boolean>(savePath) {
            @Override
            public Boolean run(Object... params) {
                return editEngine.saveProject((String) params[0]);
            }
        }, callback);
    }

    /**
     * 同步添加资源
     *
     * @param sourcePath
     * @return
     */
    public MediaSource addSource(String sourcePath) {
        return editEngine.addSource(sourcePath);
    }

    public MediaSource addSourceInTimeLineTime(String sourcePath, IEditEngine.TrackType trackType, int trackIndex, long timeLineTime) {
        return editEngine.addSourceInTimeLineTime(sourcePath, trackType, trackIndex, timeLineTime);
    }

    public MediaSource addSourceInTimeLineTime(MediaSource addSource, IEditEngine.TrackType trackType, int trackIndex, long timeLineTime) {
        return editEngine.addSourceInTimeLineTime(addSource, trackType, trackIndex, timeLineTime);
    }

    public void addSource(String sourcePath, EngineThread.IEngineMainCallback<MediaSource> callback) {
        excuteTask(new BaseEngineTask<MediaSource>(sourcePath) {
            @Override
            public MediaSource run(Object... params) {
                return editEngine.addSource((String) params[0]);
            }
        }, callback);

    }

    public void addSourceInTimeLineTime(String sourcePath, IEditEngine.TrackType trackType, int trackIndex, long timeLineTime,
                                        EngineThread.IEngineMainCallback<MediaSource> callback) {
        excuteTask(new BaseEngineTask<MediaSource>(sourcePath, trackType, trackIndex, timeLineTime) {
            @Override
            public MediaSource run(Object... params) {
                return editEngine.addSourceInTimeLineTime((String) params[0], (IEditEngine.TrackType) params[1],
                        (int) params[2], (long) params[3]);
            }
        }, callback);
    }

    public void addSourceInTimeLineTime(MediaSource addSource, IEditEngine.TrackType trackType, int trackIndex, long timeLineTime, EngineThread.IEngineMainCallback<MediaSource> callback) {
        excuteTask(new BaseEngineTask<MediaSource>(addSource, trackType, trackIndex, timeLineTime) {
            @Override
            public MediaSource run(Object... params) {
                return editEngine.addSourceInTimeLineTime((MediaSource) params[0], (IEditEngine.TrackType) params[1],
                        (int) params[2], (long) params[3]);
            }
        }, callback);
    }

    public int getTrackCount(IEditEngine.TrackType trackType) {
        return editEngine.getTrackCount(trackType);
    }

    /**
     * 获取 对应轨道上的MediaSource
     *
     * @param trackCount
     * @return
     */
    public List<MediaSource> getTrackSources(IEditEngine.TrackType trackType, int trackCount) {
        return editEngine.getTrackSources(trackType, trackCount);
    }

    public MediaSource getSource(IEditEngine.TrackType trackType, int trackCount, int sourceIndexInTimeLine) {
        return editEngine.getSource(trackType, trackCount, sourceIndexInTimeLine);
    }

    public MediaSource[] setMediaSourceStartTimeOnTrack(MediaSource mediaSource, long startTime) {
        MediaSource[] changedSource = editEngine.setMediaSourceStartTimeOnTrack(mediaSource, startTime);
        return changedSource;
    }

    public void setMediaSourceStartTimeOnTrack(MediaSource mediaSource, long startTime, EngineThread.IEngineMainCallback<MediaSource[]> callback) {
        excuteTask(new BaseEngineTask<MediaSource[]>(mediaSource, startTime) {
            @Override
            public MediaSource[] run(Object... params) {
                MediaSource[] changedSource = editEngine.setMediaSourceStartTimeOnTrack((MediaSource) params[0], (long) params[1]);
                return changedSource;
            }
        }, callback);
    }

    public MediaSource[] setMediaSourceOffsetTimeOnTrack(MediaSource mediaSource, long offset) {
        return editEngine.setMediaSourceOffsetTimeOnTrack(mediaSource, offset);
    }

    public void setMediaSourceOffsetTimeOnTrack(MediaSource mediaSource, long offset, EngineThread.IEngineMainCallback<MediaSource[]> callback) {
        excuteTask(new BaseEngineTask<MediaSource[]>(mediaSource, offset) {
            @Override
            public MediaSource[] run(Object... params) {
                return editEngine.setMediaSourceOffsetTimeOnTrack((MediaSource) params[0], (long) params[1]);
            }
        }, callback);

    }

    public long getCurrentPlayingTime() {
        return editEngine.getCurrentPlayingTime();
    }

    /**
     * 初始化显示界面
     *
     * @param surface
     */
    public void setUpWindowSync(Surface surface) {
        editEngine.setUpWindow(surface);
    }

    /**
     * 初始化显示界面
     *
     * @param surface
     */
    public void setUpWindow(Surface surface) {
        excuteTask(new BaseEngineTask<Void>(surface) {

            @Override
            public Void run(Object... params) {
                editEngine.setUpWindow((Surface) params[0]);
                return null;
            }
        }, null);
    }


    public boolean deleteSource(MediaSource mediaSource) {
        return editEngine.deleteSource(mediaSource);
    }

    public void deleteSource(MediaSource mediaSource, EngineThread.IEngineMainCallback<Boolean> callback) {
        excuteTask(new BaseEngineTask<Boolean>(mediaSource) {
            @Override
            public Boolean run(Object... params) {
                return editEngine.deleteSource((MediaSource) params[0]);
            }
        }, callback);

    }

    public long getDuration() {
        return editEngine.getDuration();
    }


    public void playSync() {
        editEngine.play();
    }

    public void play() {
        excuteTask(new BaseEngineTask<Void>() {
            @Override
            public Void run(Object... params) {
                editEngine.play();
                return null;
            }
        }, null);
    }

    public void pauseSync() {
        editEngine.pause();
    }

    public void pause() {
        excuteTask(new BaseEngineTask<Void>() {
            @Override
            public Void run(Object... params) {
                editEngine.pause();
                return null;
            }
        }, null);
    }

    public boolean seekToSync(long time) {
        return editEngine.seekTo(time);
    }

    public boolean seekTo(long time) {
        excuteTask(new BaseEngineTask<Boolean>(time) {
            @Override
            public Boolean run(Object... params) {
                long time = (long) params[0];
                return editEngine.seekTo(time);
            }
        }, new BaseEngineMainCallBack<Boolean>(time) {
            @Override
            public void onCallBack(Boolean data) {
                long time = (long) params[0];
                Log.e(LogUtil.TAG, "seek time == " + time + " --- " + data);
            }
        });
        return true;
    }

    public void output(String path, GenerateConfig config, IEngineGenerateListener generateListener) {
        output(path, config, 0, editEngine.getDuration(), generateListener);
    }

    public void output(String path, GenerateConfig config, long startTime, long durationTime, IEngineGenerateListener generateListener) {
        if (startTime < 0) {
            startTime = 0;
        }
        if (durationTime < 0) {
            durationTime = editEngine.getDuration();
        }
        editEngine.output(path, config, startTime, durationTime, generateListener);
    }

    public void cancelGenerate() {
        editEngine.cancelGenerate();
    }

    public void addEngineObserver(IEngineObserver engineObserver) {
        excuteTask(new BaseEngineTask<Void>(engineObserver) {
            @Override
            public Void run(Object... params) {
                editEngine.addEngineObserver((IEngineObserver) params[0]);
                return null;
            }
        }, null);

    }

    public void setEngineActionChangeListener(IActionChangeListener actionChangeListener) {
        excuteTask(new BaseEngineTask<Void>(actionChangeListener) {
            @Override
            public Void run(Object... params) {
                editEngine.setEngineActionChangeListener((IActionChangeListener) params[0]);
                return null;
            }
        }, null);
    }

    public boolean canUnDo() {
        return editEngine.canUnDo();
    }

    public boolean canReDo() {
        return editEngine.canReDo();
    }

    public void unDo() {
        editEngine.unDo();
    }

    public void reDo() {
        editEngine.reDo();
    }

    public void addRendererSync(Render renderer) {
        editEngine.addRenderer(renderer);
    }

    public void addRenderer(Render renderer) {
        excuteTask(new BaseEngineTask<Void>(renderer) {
            @Override
            public Void run(Object... params) {
                editEngine.addRenderer((Render) params[0]);
                return null;
            }
        }, null);
    }

    public void destroy() {
        editEngine.destroy();
        engineThread.stop();
    }
}
