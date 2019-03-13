package dfsx.com.videodemo.solution1;

import android.os.Build;
import android.os.Bundle;
import android.os.Looper;
import android.support.annotation.Nullable;
import android.support.annotation.RequiresApi;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.dfsx.core.common.Util.ToastUtils;
import com.dfsx.editengine.BaseEngineMainCallBack;
import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.editengine.EngineThread;
import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.bean.EditEngineConfig;
import com.dfsx.editengine.bean.GenerateConfig;
import com.dfsx.editengine.bean.IActionChangeListener;
import com.dfsx.editengine.bean.IEngineObserver;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.editengine.bean.ProjectCongfig;
import com.dfsx.editengine.bean.Track;
import com.dfsx.videoeditor.edit.ActionManager;
import com.dfsx.videoeditor.edit.AddMediaClipAction;
import com.dfsx.videoeditor.edit.BaseActionCreator;
import com.dfsx.videoeditor.edit.ClipRangeChangeAction;
import com.dfsx.videoeditor.edit.DeleteMediaClipAction;
import com.dfsx.videoeditor.edit.IEditAction;
import com.dfsx.videoeditor.edit.SplitMediaClipAction;
import com.dfsx.videoeditor.lang.AbsAction1;
import com.dfsx.videoeditor.out.EngineOutPut;
import com.dfsx.videoeditor.out.IOut;
import com.dfsx.videoeditor.util.EditConstants;
import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.video.VideoSourceHelper;
import com.dfsx.videoeditor.videorender.SGLView;
import com.dfsx.videoeditor.widget.timeline.IPlayerSource;
import com.dfsx.videoeditor.widget.timeline.ITimeLineEventListener;
import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;
import com.dfsx.videoeditor.widget.timeline.ImageListView;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.dialog.OutPutProgress;
import dfsx.com.videodemo.frag.EditMediaFragment;
import dfsx.com.videodemo.frag.FragUtil;
import dfsx.com.videodemo.frag.OutputConfigFragment;
import dfsx.com.videodemo.frag.SelectedEditFragment;
import dfsx.com.videodemo.proj.LocalProject;
import dfsx.com.videodemo.proj.ProjectManager;
import dfsx.com.videodemo.widget.AspectRatioFrameLayout;
import rx.Observable;
import rx.Observer;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

/**
 * 使用编辑引擎来编辑视频
 */
public class EngineEditVideoFragment extends EditMediaFragment implements SurfaceHolder.Callback, IEngineObserver,
        SelectedEditFragment.OnEditActionListener, IActionChangeListener {

    private SGLView playerSurfaceView;
    private EngineProjectManager engineManager;
    private EngineVideoRender xEditVideoRender;
    private Timer refreshTimer;
    private boolean isPlayerPlaying;
    private boolean isChangeSourceLength;

    /**
     * 编辑动作管理器
     */
    private ActionManager editActionManager;
    private IOut output;

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        engineManager = new EngineProjectManager(getContext());
        engineManager.init(new EditEngineConfig.Builder()
                .setCacheDir(EditConstants.getProjCacheDir())
                .setLogDir(EditConstants.getProjLogDir())
                .build());
        xEditVideoRender = new EngineVideoRender();
        engineManager.addRenderer(xEditVideoRender);
        engineManager.addEngineObserver(this);
        engineManager.setEngineActionChangeListener(this);
        editActionManager = ActionManager.getInstance();
        super.onViewCreated(view, savedInstanceState);
        initViewRegister();
        initRefreshUiTimer();
    }

    private void initRefreshUiTimer() {
        refreshTimer = new Timer("Video_edit_refresh");
        refreshTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                onScheduleRun();
            }
        }, 50, 50);
    }

    /**
     * 循环不停的刷新
     */
    public void onScheduleRun() {
        if (isPlayerPlaying) {
            //scroll time line
            long playingTime = engineManager.getCurrentPlayingTime();
            Log.e("TAG", "playingTime === " + playingTime);
            if (playingTime != -1) {
                timeLine.scrollCurrentTimeToTime(playingTime);
            }
        }
    }

    @Override
    protected void createNewProject(IProjectInfo projectInfo, List<ISelector<String>> selectors) {
        engineManager.create(projectInfo.getProjTitle(), new ProjectCongfig.DefaultConfigBuilder().build(),
                new BaseEngineMainCallBack<Boolean>(selectors) {
                    @Override
                    public void onCallBack(Boolean data) {
                        List<ISelector<String>> selectors = (List<ISelector<String>>) params[0];
                        Log.w("TAG", "createNewProject ---- " + data);
                        if (data) {
                            for (ISelector<String> selector : selectors) {
                                AddMediaClipAction addMediaClipAction = new AddMediaClipAction(context, timeLine, engineManager);
                                addMediaClipAction.setCouldBackOrUnBack(false);
                                addMediaClipAction.setAddParams(selector.getSelector(), -1);
                                addMediaClipAction.onDo(new Action1<Boolean>() {
                                    @Override
                                    public void call(Boolean aBoolean) {
                                        Log.e("TAG", "create proj ---- " + aBoolean);
                                    }
                                });
                            }

                        }
                    }
                });
    }

    @Override
    protected void editProject(final IProjectInfo projectInfo) {
        if (projectInfo != null) {
            engineManager.openProject(projectInfo.getProjConfigFilePath(), new EngineThread.IEngineMainCallback<Boolean>() {
                @Override
                public void onCallBack(Boolean data) {
                    Log.e("TAG", "open editProject -------------- " + data);
                    if (data) {
                        updateUI(getProjectSource(projectInfo), false, -1, null);
                    }
                }
            });
        } else {
            Log.e("TAG", "projectInfo === null 流程错误");
        }
    }

    @Override
    protected Observable.OnSubscribe<List<VideoSource>> getProjectSource(IProjectInfo projectInfo) {
        Observable.OnSubscribe<List<VideoSource>> subscribe = new Observable.OnSubscribe<List<VideoSource>>() {
            @Override
            public void call(Subscriber<? super List<VideoSource>> subscriber) {
                //前提在引擎的准备好了
                try {
                    for (int i = 0; i < engineManager.getTrackCount(IEditEngine.TrackType.VIDEO) && i < 1; i++) { //i<1目前为单轨道
                        List<MediaSource> mediaSources = engineManager.getTrackSources(IEditEngine.TrackType.VIDEO, i);
                        List<VideoSource> sourceList = new ArrayList<>();
                        VideoSourceHelper sourceHelper = new VideoSourceHelper();
                        long startTime = 0;
                        for (MediaSource mediaSource : mediaSources) {
                            VideoSource videoSource = sourceHelper.createVideoSource(mediaSource);
                            videoSource.timeLineStartTime = startTime;
                            sourceList.add(videoSource);
                            startTime += videoSource.getTimeLineDuration();
                        }
                        subscriber.onNext(sourceList);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    subscriber.onNext(null);
                }
            }
        };
        return subscribe;
    }


    private void initViewRegister() {
        timeLineView.addRecyclerViewOnItemTouchListener(new RecyclerView.SimpleOnItemTouchListener() {
            @Override
            public boolean onInterceptTouchEvent(RecyclerView rv, MotionEvent e) {
                pausePlayer();
                return super.onInterceptTouchEvent(rv, e);
            }
        });

        timeLineView.setOnDragSizeChangeListener(new ImageListView.OnDragSizeChangeListener() {
            private long[] dragStartRange;

            @Override
            public void onDragStart(View v) {
                dragStartRange = new long[2];
                Log.e("WTD", "onDragStart---------------");
                if (v.getTag() instanceof TimeLineVideoSource) {
                    TimeLineVideoSource timeLineVideoSource = (TimeLineVideoSource) v.getTag();
                    IPlayerSource playerSource = timeLineVideoSource.getPlayerSource();
                    long[] playRange = playerSource.getPlayTimeRange();
                    if (playRange != null && playRange.length == 2) {
                        dragStartRange[0] = playRange[0];
                        dragStartRange[1] = playRange[1];
                    } else {
                        dragStartRange[0] = playerSource.getSourceStartTime();
                        dragStartRange[1] = playerSource.getSourceStartTime() +
                                playerSource.getSourceDuration();
                    }
                }
            }

            @Override
            public void onDragSizeChange(View v, int viewWidth, int dWidth, int leftAllHidSize, int rightAllHideSize) {
                isChangeSourceLength = true;
            }

            @Override
            public void onDragEnd(View v, ImageListView.DragPosition dragPos, int viewWidth, int leftAllHidSize, int rightAllHideSize) {
                Log.e("WTD", "onDragEnd---------------");
                if (v.getTag() instanceof TimeLineVideoSource) {
                    TimeLineVideoSource timeLineVideoSource = (TimeLineVideoSource) v.getTag();
                    IPlayerSource playerSource = timeLineVideoSource.getPlayerSource();
                    long[] playRange = playerSource.getPlayTimeRange();
                    if (playRange != null && playRange.length == 2) {
                        Log.e("TAG", "source rage == " + playRange[0] + " ---- " + playRange[1]);
                        MediaSource mediaSource = playerSource.getEngineMediaSource();
                        if (mediaSource != null) {
                            if (dragPos == ImageListView.DragPosition.left) {
                                editActionManager.dispatchAction(new BaseActionCreator<long[]>(playRange) {
                                    @Override
                                    public IEditAction createAction() {
                                        ClipRangeChangeAction action = new ClipRangeChangeAction(context,
                                                timeLine, engineManager);
                                        action.setStartRange(dragStartRange);
                                        action.setLeftTimeOfMedia(params[0]);
                                        action.setMediaDuration(params[1] - params[0]);
                                        action.setUpdateUIOnDo(false);
                                        return action;
                                    }
                                });
                            } else if (dragPos == ImageListView.DragPosition.right) {
                                long clipDuration = playRange[1] - playRange[0];
                                editActionManager.dispatchAction(new BaseActionCreator<Long>(clipDuration) {
                                    @Override
                                    public IEditAction createAction() {
                                        ClipRangeChangeAction action = new ClipRangeChangeAction(context,
                                                timeLine, engineManager);
                                        action.setStartRange(dragStartRange);
                                        action.setMediaDuration(params);
                                        action.setUpdateUIOnDo(false);
                                        return action;
                                    }
                                });
                            }
                        }
                    }
                }
                isChangeSourceLength = false;
            }
        });

        timeLine.addTimeLineEventListener(new ITimeLineEventListener() {
            @Override
            public void onAddSourceClick(View v, ITimeLineItem item) {
                FragUtil.startSingleMediaSelect(getActivity(), MSG_SELECED_MEDIA_ADD_POS);
                timeLineActionTimeReference.set(item);
            }

            @Override
            public void onThumbImageClick(View v, ITimeLineItem item) {
                if (item instanceof TimeLineVideoSource) {
                    TimeLineVideoSource lineThumb = (TimeLineVideoSource) item;
                    if (lineThumb.getSelectedObject().isSelected()) {//选中
                        setSelectedFrag(true);
                    } else {
                        setSelectedFrag(false);
                    }
                } else {
                    setSelectedFrag(false);
                }
            }

            @Override
            public void onTimeLineTimeChangeListener(View v, ITimeLineItem timeLineItem, long time, int recyclerViewState) {
                Log.e("TAG", "onTimeLineTimeChangeListener == " + isPlayerPlaying + " --- " + isChangeSourceLength + "--- " + recyclerViewState);
                if (!isPlayerPlaying && !isChangeSourceLength && recyclerViewState != RecyclerView.SCROLL_STATE_IDLE) {
                    Log.e("TAG", "UI seek Time == " + time);
                    engineManager.seekTo(time);
                }
            }
        });
    }

    @Override
    protected void setupMediaSurfaceView(RelativeLayout mediaSurfaceLayout) {
        AspectRatioFrameLayout contentFrame = new AspectRatioFrameLayout(context);
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        contentFrame.setLayoutParams(params);
        mediaSurfaceLayout.addView(contentFrame);

        playerSurfaceView = new SGLView(context);
        FrameLayout.LayoutParams params1 = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT);
        playerSurfaceView.setLayoutParams(params1);
//        playerSurfaceView.getHolder().addCallback(this);
        contentFrame.addView(playerSurfaceView, params1);
        xEditVideoRender.setSglView(playerSurfaceView);
        contentFrame.setResizeMode(AspectRatioFrameLayout.RESIZE_MODE_FIT);
    }

    private void setSelectedFrag(boolean isShow) {
        FragmentManager fm = getChildFragmentManager();
        SelectedEditFragment editFragment = (SelectedEditFragment) fm.findFragmentByTag("SelectedEditFragment");
        FragmentTransaction transaction = fm.beginTransaction();
        transaction.setCustomAnimations(R.anim.frag_bottom_in, R.anim.frag_bottom_out);
        if (editFragment == null) {
            editFragment = new SelectedEditFragment();
            editFragment.setEditActionListener(this);
            if (isShow) {
                transaction.add(R.id.bottom_view_container, editFragment, "SelectedEditFragment")
                        .commitAllowingStateLoss();
            }
        } else {
            if (isShow) {
                transaction.show(editFragment)
                        .commitAllowingStateLoss();
            } else {
                transaction.remove(editFragment)
                        .commitAllowingStateLoss();
            }
        }
    }

    @Override
    protected Looper getEngineThreadLooper() {
        return engineManager.getEngineThreadLooper();
    }

    @Override
    protected IEditAction createAddMediaClipAction() {
        AddMediaClipAction addMediaClipAction = new AddMediaClipAction(context, timeLine, engineManager);
        return addMediaClipAction;
    }

    @Override
    public void startPlayer() {
        super.startPlayer();
        engineManager.play();
    }

    @Override
    public void pausePlayer() {
        super.pausePlayer();
        engineManager.pause();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        engineManager.setUpWindow(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    public void onPause() {
        super.onPause();
        engineManager.pause();
        Observable.just(currentEditProjectInfo)
                .observeOn(Schedulers.io())
                .map(new Func1<IProjectInfo, Boolean>() {
                    @Override
                    public Boolean call(IProjectInfo projectInfo) {
                        if (projectInfo != null) {
                            if (projectInfo instanceof LocalProject) {
                                ((LocalProject) projectInfo).setProjectContentLength(engineManager.getDuration());
                                ((LocalProject) projectInfo).setProjectThumbPath(timeLine.getTimeLineFirstFrame());
                            }
                            engineManager.saveProject(projectInfo.getProjConfigFilePath(), null);
                            return true;
                        }
                        return false;
                    }
                })
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Observer<Boolean>() {
                    @Override
                    public void onCompleted() {

                    }

                    @Override
                    public void onError(Throwable e) {

                    }

                    @Override
                    public void onNext(Boolean aBoolean) {
                        if (aBoolean) {
                            ProjectManager.getInstance().save();
                            Log.e("TAG", "save project ------ OK");
                        }
                    }
                });

    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    public void onOutPut(OutputConfigFragment.IOutputConfig outputConfig) {
        super.onOutPut(outputConfig);
        if (engineManager.getDuration() <= 0) {
            Toast.makeText(context, "没有内容输出", Toast.LENGTH_SHORT).show();
            return;
        }

        File saveFile = new File(EditConstants.getVideoOutputDir(), currentEditProjectInfo.getProjTitle() + "_video.mp4");
        OutPutProgress outPutProgress = new OutPutProgress();
        outPutProgress.setOutputPath(saveFile.getPath());
        outPutProgress.show(getFragmentManager(), "OutPutProgress");
        outPutProgress.setOnCancelClickListener(new OutPutProgress.OnCancelClickListener() {
            @Override
            public void onCancelClick(View v) {
                if (output != null) {
                    output.onCancelOutPut();
                }
            }
        });
        OutputConfigFragment.SizeConfig sizeConfig = (OutputConfigFragment.SizeConfig) outputConfig;
        GenerateConfig config = new GenerateConfig.DefaultBuild()
                .setWidth(sizeConfig.getSize().getWidth())
                .setHeight(sizeConfig.getSize().getHeight())
                .build();
        output = new EngineOutPut(engineManager, config, outPutProgress);
        output.onFileOutPut(saveFile);
    }

    @Override
    public void onDestroyView() {
        engineManager.destroy();
        super.onDestroyView();
        if (refreshTimer != null) {
            refreshTimer.cancel();
            refreshTimer = null;
        }
    }

    @Override
    public void onPlayStatusChanged(PlayStatus status) {
        onVideoPlayStatusChange(status == PlayStatus.Playing || status == PlayStatus.DropFrame);
    }

    @Override
    public void onTrackCreated(Track track) {

    }

    @Override
    public void onTrackRemoved(Track track) {

    }

    /**
     * 播放器播放
     */
    private void onVideoPlayStatusChange(boolean isPlaying) {
        Log.e("TAG", "video playing status----------" + isPlaying);
        isPlayerPlaying = isPlaying;
        updatePlayViewImage(isPlaying);
    }

    @Override
    public void onSplitActionClick() {
        editActionManager.dispatchAction(new BaseActionCreator<Void>(null) {
            @Override
            public IEditAction createAction() {
                SplitMediaClipAction splitMediaClipAction = new SplitMediaClipAction(context, timeLine, engineManager);
                return splitMediaClipAction;
            }
        });
    }

    @Override
    public void onDeleteActionClick() {
        editActionManager.dispatchAction(new BaseActionCreator<Void>(null) {
            @Override
            public IEditAction createAction() {
                DeleteMediaClipAction action = new DeleteMediaClipAction(context, timeLine, engineManager);
                return action;
            }
        });
    }

    /**
     * 根据MediaSource 查找UI时间线上的数据
     *
     * @param mediaSourceId
     * @return
     */
    private int getItemByMediaSourceId(long mediaSourceId) {
        List<ITimeLineItem> items = timeLine.getTimeLineItemList();
        if (items != null) {
            for (int i = 0; i < items.size(); i++) {
                ITimeLineItem item = items.get(i);
                if (item instanceof TimeLineVideoSource) {
                    IPlayerSource source = ((TimeLineVideoSource) item).getPlayerSource();
                    if (source.getEngineMediaSource().getId() == mediaSourceId) {
                        Log.e("TAG", "source index === " + i);
                        return i;
                    }
                }
            }
        }
        return -1;
    }

    private ITimeLineItem getUIItemByMediaSourceId(long mediaSourceId) {
        List<ITimeLineItem> items = timeLine.getTimeLineItemList();
        if (items != null) {
            for (int i = 0; i < items.size(); i++) {
                ITimeLineItem item = items.get(i);
                if (item instanceof TimeLineVideoSource) {
                    IPlayerSource source = ((TimeLineVideoSource) item).getPlayerSource();
                    if (source.getEngineMediaSource().getId() == mediaSourceId) {
                        return item;
                    }
                }
            }
        }
        return null;
    }

    private int getItemByStartTime(long startTime) {
        List<ITimeLineItem> items = timeLine.getTimeLineItemList();
        if (items != null) {
            for (int i = 0; i < items.size(); i++) {
                ITimeLineItem item = items.get(i);
                if (item.getTimeLineTime() != null && item.getTimeLineTime().length == 2 &&
                        item.getTimeLineTime()[0] >= startTime) {
                    return i;
                }
            }
            return items.size();
        }
        return -1;
    }

    @Override
    public void onAddMediaSource(MediaSource mediaSource, long startTimeOfTrack) {
        if (mediaSource != null && getItemByMediaSourceId(mediaSource.getId()) < 0) {//没有数据。
            VideoSourceHelper sourceHelper = new VideoSourceHelper();
            VideoSource source = sourceHelper.createVideoSource(mediaSource);
            List<ITimeLineItem> items = timeLine.getTimeLineItemList();
            int index = getItemByStartTime(startTimeOfTrack);
            if (index >= 0) {
                List<VideoSource> addList = new ArrayList<>();
                addList.add(source);
                updateTimeLineData(addList, index == items.size(), index);
            }
        }
    }

    @Override
    public void onChangeMediaSourceDuration(long sourceId, long oldDuration, long duration) {
        int index = getItemByMediaSourceId(sourceId);
        if (index >= 0) {
            List<ITimeLineItem> items = timeLine.getTimeLineItemList();
            ITimeLineItem item = items.get(index);
            ((TimeLineVideoSource) item).setTimeLineDuration(duration);
            long[] range = ((TimeLineVideoSource) item).getPlayerSource().getPlayTimeRange();
            if (range == null) {
                range = new long[2];
                range[0] = 0;
                range[1] = ((TimeLineVideoSource) item).getPlayerSource().getSourceDuration();
            }
            range[1] = duration + range[0];
            ((TimeLineVideoSource) item).getPlayerSource().setPlayTimeRange(range);
            for (int i = index + 1; i < items.size(); i++) {
                items.get(i).setTimeLineTimeOffSet(duration - oldDuration);
            }
            timeLine.updateUI();
        } else {
            Log.e("TAG", "onChangeMediaSourceDuration ---- item == null -- sourceId === " + sourceId);
        }
    }

    @Override
    public void onMediaSourceStartTimeChange(long sourceId, long oldStartTime, long startTime) {
        int index = getItemByMediaSourceId(sourceId);
        if (index >= 0) {
            List<ITimeLineItem> items = timeLine.getTimeLineItemList();
            ITimeLineItem item = items.get(index);
            item.setTimeLineTimeOffSet(startTime - oldStartTime);
            timeLine.sortByTime();
        }
    }

    @Override
    public void onMediaSourceRemove(long sourceId) {
        int index = getItemByMediaSourceId(sourceId);
        if (index >= 0) {
            List<ITimeLineItem> temp = new ArrayList<>();
            List<ITimeLineItem> allList = timeLine.getTimeLineItemList();
            if (index == allList.size() - 1) {//最后一个
                temp.add(allList.get(index - 1));
                temp.add(allList.get(index));
            } else {
                temp.add(allList.get(index));
                temp.add(allList.get(index + 1));
            }
            timeLine.removeTimeLineDataList(temp);
        } else {
            Log.e("TAG", "onMediaSourceRemove ---- no source-- sourceId === " + sourceId);
        }
    }

    @Override
    public void onMediaSourcePlayStartRangeChange(long sourceId, long oldStartRange, long startRange) {
        int index = getItemByMediaSourceId(sourceId);
        if (index >= 0) {
            List<ITimeLineItem> allList = timeLine.getTimeLineItemList();
            ITimeLineItem item = allList.get(index);
            if (item != null && item instanceof TimeLineVideoSource) {
                IPlayerSource videoSource = ((TimeLineVideoSource) item).getPlayerSource();
                long[] range = videoSource.getPlayTimeRange();
                if (range == null) {
                    range = new long[2];
                    range[0] = 0;
                    range[1] = videoSource.getSourceDuration();
                }
                range[0] = startRange;
                videoSource.setPlayTimeRange(range);
                ((TimeLineVideoSource) item).setTimeLineDuration(range[1] - range[0]);
                for (int i = index + 1; i < allList.size(); i++) {
                    allList.get(i).setTimeLineTimeOffSet(startRange - oldStartRange);
                }
                timeLine.updateUI();
            }
        } else {
            Log.e("TAG", "onMediaSourcePlayStartRangeChange ---- no source-- sourceId === " + sourceId);
        }
    }
}
