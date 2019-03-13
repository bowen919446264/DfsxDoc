package com.dfsx.editengine.xedit;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.view.Surface;

import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.NativeHelper;
import com.dfsx.editengine.bean.BaseRunnable;
import com.dfsx.editengine.bean.EditEngineConfig;
import com.dfsx.editengine.bean.EngineTask;
import com.dfsx.editengine.bean.GenerateConfig;
import com.dfsx.editengine.bean.IActionChangeListener;
import com.dfsx.editengine.bean.IEngineGenerateListener;
import com.dfsx.editengine.bean.IEngineObserver;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.editengine.bean.ProjectCongfig;
import com.dfsx.editengine.bean.Render;
import com.dfsx.editengine.util.EngineUtil;
import com.dfsx.editengine.util.LogUtil;
import com.ds.xedit.jni.AVMediaInfo;
import com.ds.xedit.jni.AudioStream;
import com.ds.xedit.jni.CInputStream;
import com.ds.xedit.jni.COutputFileStream;
import com.ds.xedit.jni.EMediaType;
import com.ds.xedit.jni.ETrackType;
import com.ds.xedit.jni.EncodeParam;
import com.ds.xedit.jni.EngineSetting;
import com.ds.xedit.jni.GenerateSetting;
import com.ds.xedit.jni.IAVMedia;
import com.ds.xedit.jni.IAction;
import com.ds.xedit.jni.IClip;
import com.ds.xedit.jni.IDPtr;
import com.ds.xedit.jni.IMedia;
import com.ds.xedit.jni.ITimeLine;
import com.ds.xedit.jni.ITrack;
import com.ds.xedit.jni.IXEngine;
import com.ds.xedit.jni.Rational;
import com.ds.xedit.jni.xedit;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;

public class XEditEngine implements IEditEngine {

    static {
        try {
            System.loadLibrary("native-lib");
        } catch (Throwable ex) {
            ex.printStackTrace();
            Log.e("LoadNativeLibraries", ex.getMessage());
        }
    }

    private static final String TASK_SET_WINDOW = "task_set_window";

    private Context context;
    private IXEngine engine;
    private ITimeLine timeLine;
    private XEditObserver xEditObserver;
    private LogReceiver logReceiver;
    private XEditGenerateObserver generateObserver;
    private XEditActionChangeObserver actionChangeObserver;

    /**
     * 未执行的任务对列
     */
    private LinkedList<EngineTask> noExcuteTask = new LinkedList<>();

    public XEditEngine() {

    }


    protected void addTask(String name, Runnable task) {
        noExcuteTask.add(new EngineTask(name, task));
    }

    /**
     * 执行任务对列
     *
     * @param taskName 选中的任务名字， 如果为null， 就执行全部
     */
    protected void excuteTaskQueue(String taskName) {
        for (int i = 0; i < noExcuteTask.size(); i++) {
            EngineTask task = noExcuteTask.get(i);
            if (taskName == null) {
                if (task.getTask() != null) {
                    task.getTask().run();
                }
                noExcuteTask.remove(task);
                i--;
            } else {
                if (TextUtils.equals(taskName, task.getName())) {
                    if (task.getTask() != null) {
                        task.getTask().run();
                    }
                    noExcuteTask.remove(task);
                    i--;
                }
            }
        }
    }

    @Override
    public void init(Context context, EditEngineConfig config) {
        this.context = context;
        logReceiver = new LogReceiver();
        // 添加一个日志接收者
        xedit.AVLogAddReceiver(logReceiver);
        engine = IXEngine.getSharedInstance();
        timeLine = engine.getTimeLine();
        // 引擎设置
        EngineSetting engineSetting = new EngineSetting();
        engineSetting.setCacheDir(config.getCacheDir());
        engineSetting.setLogDir(config.getLogDir());
        // 引擎初始化
        engine.initialize(engineSetting);
        generateObserver = new XEditGenerateObserver(timeLine);
        actionChangeObserver = new XEditActionChangeObserver(timeLine);
    }

    @Override
    public void addRenderer(Render renderer) {
        timeLine.addRenderer(renderer.getXRender());
    }

    @Override
    public void setUpWindow(Surface surface) {
        if (engine.getCurrentProject() != null) {
            /**
             * 必须执行在工程对象成功
             */
            NativeHelper.setupNativeWindow(surface, engine.getCurrentProject().getSetting().getNWidth(),
                    engine.getCurrentProject().getSetting().getNHeight());
            Log.e(LogUtil.TAG, "setUpWindow -------------- ok");
        } else {
            addTask(TASK_SET_WINDOW, new BaseRunnable<Surface>(surface) {
                @Override
                public void run(Surface data) {
                    NativeHelper.setupNativeWindow(data, engine.getCurrentProject().getSetting().getNWidth(),
                            engine.getCurrentProject().getSetting().getNHeight());
                    Log.e(LogUtil.TAG, "setUpWindow -------------- ok");
                }
            });
        }
    }

    @Override
    public void createProject(String projectName, ProjectCongfig projectCongfig) throws Exception {
        int code = engine.newProject(projectName, projectCongfig.getXEditSettings());
        if (code < 0) {
            throw new Exception("新建工程失败!");
        }
        excuteTaskQueue(TASK_SET_WINDOW);
    }

    @Override
    public boolean openProject(String projectFilePath) {
        try {
            CInputStream inputStream = new CInputStream();
            int code = inputStream.open(projectFilePath);
            if (code < 0) {
                throw new Exception("打开文件 " + projectFilePath + " 失败!");
            }

            code = engine.openProject(inputStream);
            if (code < 0) {
                throw new Exception("打开工程失败!");
            }
            excuteTaskQueue(TASK_SET_WINDOW);
            return true;
        } catch (Exception ex) {
            ex.printStackTrace();
            Log.e("XEngine", ex.getMessage());
        }
        return false;
    }

    /**
     * @param projectFilePath 路径以 .xeproj 为后缀
     * @return
     */
    @Override
    public boolean saveProject(String projectFilePath) {
        try {
            // 打开输出流
            COutputFileStream outputStream = new COutputFileStream();
            long code = outputStream.open(projectFilePath);
            if (code < 0) {
                throw new Exception("打开输出流" + projectFilePath + "失败!");
            }
            // 保存工程
            code = engine.saveProject(outputStream);
            if (code < 0) {
                throw new Exception("保存工程失败!");
            }
            outputStream.flush();
            outputStream.close();
            Log.e(LogUtil.TAG, "saveProject --- " + projectFilePath + " ---- ok");
            return true;
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return false;
    }

    @Override
    public MediaSource addSource(String sourcePath) {
        long time = EngineUtil.rationalToTime(timeLine.getDuration());
        return addSourceInTimeLineTime(sourcePath, TrackType.VIDEO, 0, time);
    }

    @Override
    public MediaSource addSourceInTimeLineTime(String sourcePath, TrackType trackType, int trackIndex, long timeLineStartTime) {
        try {
            IMedia iMedia = timeLine.addMedia(sourcePath);
            IClip clip = iMedia.newClip();
            MediaSource source = new MediaSource(iMedia, clip, null);
            return addSourceInTimeLineTime(source, trackType, trackIndex, timeLineStartTime);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public MediaSource addSourceInTimeLineTime(MediaSource addSource, TrackType trackType, int trackIndex, long timeLineStartTime) {
        try {
            IMedia media = addSource.getMedia();
            ITrack track = timeLine.getTrack(EngineUtil.getETrackType(trackType), trackIndex);

            Rational offsetOnTrack = EngineUtil.timeToRational(timeLineStartTime);

            IClip addedClip = null;
            if (media.getMediaType() == EMediaType.EMediaType_AV) {
                IAVMedia avMedia = IAVMedia.dynamic_cast(media);
                AVMediaInfo mediaInfo = new AVMediaInfo();
                avMedia.getMediaInfo(mediaInfo);
                if (mediaInfo.getNVideoCount() > 0) {
                    addedClip = track.addClip(media.getId(), offsetOnTrack);
                } else if (mediaInfo.getNAudioCount() > 0) {
                    //TODO 待实现音频添加切片
//                    AudioStream[] audioStreams = mediaInfo.getAStreams();
//                    addedClip = track.addAVStreamClip(media.getId(), offsetOnTrack, audioStreams[0].getNIndex());
                }
            } else if (media.getMediaType() == EMediaType.EMediaType_Image) {
                addedClip = track.addClip(media.getId(), offsetOnTrack);
            }
            if (addedClip != null) {
                return new MediaSource(media, addedClip, track);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public int getTrackCount(TrackType trackType) {
        return timeLine.getTrackCount(EngineUtil.getETrackType(trackType));
    }

    @Override
    public List<MediaSource> getTrackSources(TrackType trackType, int trackCount) {
        try {
            ITrack track = timeLine.getTrack(EngineUtil.getETrackType(trackType), trackCount);
            if (track != null) {
                List<MediaSource> list = new ArrayList<>();
                for (int i = 0; i < track.getClipCount(); i++) {
                    IClip clip = track.getClip(i);
                    MediaSource source = new MediaSource(timeLine.getMediaById(clip.getRefMediaId()), clip, track);
                    list.add(source);
                }
                Log.e(LogUtil.TAG, "Track " + trackCount + " Clip count == " + list.size());
                Collections.sort(list, new Comparator<MediaSource>() {
                    @Override
                    public int compare(MediaSource o1, MediaSource o2) {
                        try {
                            Rational r1 = o1.getSourceClip().getOffsetOnTrack();
                            Rational r2 = o2.getSourceClip().getOffsetOnTrack();
                            if (r1.doubleValue() < r2.doubleValue()) {
                                return -1;
                            } else if (r1.doubleValue() > r2.doubleValue()) {
                                return 1;
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        return 0;
                    }
                });
                return list;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }


    @Override
    public MediaSource getSource(TrackType trackType, int trackCount, int sourceIndexInTimeLine) {
        try {
            ITrack track = timeLine.getTrack(EngineUtil.getETrackType(trackType), trackCount);
            IClip clip = track.getClip(sourceIndexInTimeLine);
            return new MediaSource(timeLine.getMediaById(clip.getRefMediaId()), clip, track);
        } catch (Exception e) {
            e.printStackTrace();
            Log.e(LogUtil.TAG, "获取失败");
        }
        return null;
    }

    @Override
    public MediaSource[] setMediaSourceStartTimeOnTrack(MediaSource mediaSource, long startTime) {
        Rational newOffset = EngineUtil.timeToRational(startTime);
        return moveMediaSource(mediaSource, newOffset);
    }


    /**
     * @param mediaSource
     * @param newSetTime
     * @return 返回null， 表示失败， 有数据，表示成功， 数据表示移动所产生的改变 (移动过程中，有切片被拆分了)
     * (长度为3， 0表示改变的资源，1，改后左边资源，2改后右边资源)
     */
    private MediaSource[] moveMediaSource(MediaSource mediaSource, Rational newSetTime) {
        ITrack track = mediaSource.getTrack() != null ? mediaSource.getTrack() : getTrackByClip(mediaSource.getSourceClip());
        if (track != null) {
            IDPtr idPtr = new IDPtr();
            IClip leftClip = new IClip();
            IClip rightClip = new IClip();
            int code = track.moveClip(mediaSource.getSourceClip().getId(),
                    newSetTime, idPtr.cast(), leftClip, rightClip);
            if (code < 0) {
                return null;
            }
            MediaSource[] afterSource = new MediaSource[3];
            long splitedClipId = idPtr.value();
            if (splitedClipId > 0) {// 移动过程中，有切片被拆分了
                long leftClipId = leftClip.getId();     // 被拆分切片的左侧部分
                long rightClipId = rightClip.getId();   // 被拆分切片的右侧部分
                afterSource[0] = getMediaSourceByClipId(track, splitedClipId);
                afterSource[1] = getMediaSourceByClipId(track, leftClipId);
                afterSource[2] = getMediaSourceByClipId(track, rightClipId);
            }
            return afterSource;
        }
        return null;
    }

    /**
     * 根据Track和ID资源新创建MediaSource对象
     *
     * @param track
     * @param clipId
     * @return
     */
    private MediaSource getMediaSourceByClipId(ITrack track, long clipId) {
        IClip clip = track.getClipById(clipId);
        IMedia media = timeLine.getMediaById(clip.getRefMediaId());
        MediaSource source = new MediaSource(media, clip, track);
        return source;
    }

    @Override
    public MediaSource[] setMediaSourceOffsetTimeOnTrack(MediaSource mediaSource, long offset) {
        Rational oldOffset = mediaSource.getSourceClip().getOffsetOnTrack();
        Rational newOffset = oldOffset.add(EngineUtil.timeToRational(offset));
        return moveMediaSource(mediaSource, newOffset);
    }

    /**
     * 通过切片 查找切片所在的线路
     *
     * @param clip
     * @return
     */
    protected ITrack getTrackByClip(IClip clip) {
        if (clip == null) {
            return null;
        }
        ETrackType[] trackTypes = new ETrackType[]{ETrackType.ETrackType_Video, ETrackType.ETrackType_Audio};
        for (ETrackType type : trackTypes) {
            for (int i = 0; i < timeLine.getTrackCount(type); i++) {
                ITrack track = timeLine.getTrack(type, i);
                if (getClipIndexOnTrack(clip.getId(), track) >= 0) {
                    return track;
                }
            }
        }
        return null;
    }

    /**
     * 获取 Clip 在track上的位置
     *
     * @param clipId
     * @param track
     * @return
     */
    protected int getClipIndexOnTrack(long clipId, ITrack track) {
        for (int clipIndex = 0; clipIndex < track.getClipCount(); clipIndex++) {
            if (track.getClip(clipIndex).getId() == clipId) {
                return clipIndex;
            }
        }
        return -1;
    }

    @Override
    public long getCurrentPlayingTime() {
        return EngineUtil.rationalToTime(timeLine.getCurrentPos());
    }

    @Override
    public long getDuration() {
        return EngineUtil.rationalToTime(timeLine.getDuration());
    }

    @Override
    public boolean deleteSource(MediaSource mediaSource) {
        try {
            if (mediaSource != null) {
                ITrack track = mediaSource.getTrack();
                if (track == null) {
                    track = getTrackByClip(mediaSource.getSourceClip());
                }
                track.removeClipById(mediaSource.getSourceClip().getId());
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    @Override
    public void play() {
        if (timeLine.getTrack(ETrackType.ETrackType_Video, 0).getClipCount() > 0) {
            timeLine.play();
        } else {
            Log.e(LogUtil.TAG, "没有添加视频资源");
        }
    }

    @Override
    public void pause() {
        timeLine.pause();
    }

    @Override
    public boolean seekTo(long time) {
        return timeLine.seek(EngineUtil.timeToRational(time)) >= 0;
    }

    @Override
    public void output(String path, GenerateConfig config, long startTime, long durationTime, IEngineGenerateListener generateListener) {
        try {
            int lastSplit = path.lastIndexOf("/") + 1;
            String dir = path.substring(0, lastSplit);
            String name = path.substring(lastSplit);

            GenerateSetting setting = new GenerateSetting();
            setting.setStrDestDir(dir);
            setting.setStrDestName(name);
            setting.setRStartTime(EngineUtil.timeToRational(startTime));
            setting.setRDuration(EngineUtil.timeToRational(durationTime));
            EncodeParam encodeParam = config.getEngineEncodeParams();
            setting.setEncodeParam(encodeParam);
            generateObserver.setEngineGenerateListener(generateListener);
            timeLine.generate(setting, generateObserver);
            Log.e(LogUtil.TAG, "output settings ok -- dir== " + dir + "  name === " + name);
        } catch (Exception e) {
            throw e;
        }
    }

    @Override
    public void cancelGenerate() {
        timeLine.cancelGenerate();
    }

    @Override
    public void addEngineObserver(IEngineObserver engineObserver) {
        if (xEditObserver == null) {
            xEditObserver = new XEditObserver(timeLine);
            timeLine.addObserver(xEditObserver);
        }
        xEditObserver.addEngineObserver(engineObserver);
        Log.e(LogUtil.TAG, "addEngineObserver ----- OK");
    }

    @Override
    public void setEngineActionChangeListener(IActionChangeListener actionChangeListener) {
        if (actionChangeObserver != null) {
            actionChangeObserver.setActionChangeListener(actionChangeListener);
        }
    }

    @Override
    public boolean canUnDo() {
        return timeLine.canUndo();
    }

    @Override
    public boolean canReDo() {
        return timeLine.canRedo();
    }

    @Override
    public void unDo() {
        if (timeLine.canUndo()) {
            IAction action = timeLine.undo();
            if (action == null) {
                Log.e(LogUtil.TAG, "unDo error------------");
            } else {
                actionChangeObserver.onPushAction(action, true);
            }
        } else {
            Log.e(LogUtil.TAG, "canUndo ------ false");
        }
    }

    @Override
    public void reDo() {
        if (timeLine.canRedo()) {
            IAction action = timeLine.redo();
            if (action == null) {
                Log.e(LogUtil.TAG, "unDo error------------");
            } else {
                actionChangeObserver.onPushAction(action, false);
            }
        } else {
            Log.e(LogUtil.TAG, "canRedo ------ false");
        }
    }

    @Override
    public void destroy() {
        try {
            if (timeLine != null) {
                timeLine.removeAllRenderers();
                timeLine.removeAllObservers();
            }
            if (engine != null) {
                engine.closeCurrentProject();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        engine = null;
        timeLine = null;
    }
}
