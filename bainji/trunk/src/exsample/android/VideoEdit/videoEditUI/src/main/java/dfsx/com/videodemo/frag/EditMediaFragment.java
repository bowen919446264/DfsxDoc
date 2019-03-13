package dfsx.com.videodemo.frag;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.dfsx.core.rx.RxBus;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.edit.ActionManager;
import com.dfsx.videoeditor.edit.AddMediaClipAction;
import com.dfsx.videoeditor.edit.BaseActionCreator;
import com.dfsx.videoeditor.edit.BaseActionCreator2;
import com.dfsx.videoeditor.edit.IEditAction;
import com.dfsx.videoeditor.lang.AbsRunnable;
import com.dfsx.videoeditor.test.TimeLineVideo;
import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.video.VideoSourceHelper;
import com.dfsx.videoeditor.widget.timeline.*;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineSplitAdd;
import com.github.ikidou.fragmentBackHandler.FragmentBackHandler;

import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.proj.LocalProject;
import dfsx.com.videodemo.proj.ProjectManager;
import dfsx.com.videodemo.solution1.BaseParamsAction;
import rx.Observable;
import rx.Subscriber;
import rx.Subscription;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.schedulers.Schedulers;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

/**
 * 媒体的编辑界面
 */
public class EditMediaFragment extends Fragment implements FragmentBackHandler {

    public static final String MSG_SELECED_MEDIA_ADD_POS = "dfsx.com.videodemo.EditMediaFragment_selected_add_pos";

    public final AtomicReference<ITimeLineItem> timeLineActionTimeReference = new AtomicReference<>();

    protected Handler mainHandler = new Handler(Looper.getMainLooper());
    protected Activity activity;
    protected Context context;

    protected View closePageView;
    protected TextView ptojTitltTextView;
    protected View titleOutPutView;
    protected View imageOutPutView;
    protected View imageAddMedia;
    protected ImageView imagePlayMedia;
    protected View imageBackAction;
    protected View imageUnBackAction;
    protected RelativeLayout mediaSurfaceLayout;

    protected TimeLineView timeLineView;
    protected ITimeLineUI timeLine;

    protected VideoSourceHelper sourceHelper;

    protected Subscription mediaSelectorSub;

    protected OutputConfigFragment outputConfigFragment;

    protected IProjectInfo currentEditProjectInfo;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        getActivity().getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        activity = getActivity();
        context = getContext();
        return inflater.inflate(R.layout.frag_edit_media, null);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        sourceHelper = new VideoSourceHelper();
        initView(view);
        doIntentBundle();
        initRegister();
    }

    private void initView(View v) {
        initTitleBarView(v);
        initToolBar(v);
        mediaSurfaceLayout = (RelativeLayout) v.findViewById(R.id.media_image_layout);
        setupMediaSurfaceView(mediaSurfaceLayout);
        timeLineView = (TimeLineView) v.findViewById(R.id.time_line_view);
        timeLine = timeLineView;
    }

    protected void setupMediaSurfaceView(RelativeLayout mediaSurfaceLayout) {

    }

    private void doIntentBundle() {
        if (getArguments() != null) {
            IProjectInfo projectInfo = (IProjectInfo) getArguments().getSerializable("edit_project");
            List<ISelector<String>> iSelectors = (List<ISelector<String>>) getArguments().getSerializable("edit_media_source");
            if (iSelectors != null) {
                if (currentEditProjectInfo == null) {
                    currentEditProjectInfo = new LocalProject(null, 0, null);
                }
                ProjectManager.getInstance().setCurrentProjectInfo(currentEditProjectInfo);
                createNewProject(currentEditProjectInfo, iSelectors);
            } else if (projectInfo != null) {//编辑project
                currentEditProjectInfo = projectInfo;
                editProject(projectInfo);
            }
        }
    }

    /**
     * 编辑项目
     *
     * @param projectInfo
     */
    protected void editProject(IProjectInfo projectInfo) {
    }

    /**
     * 创建新项目
     */
    protected void createNewProject(IProjectInfo projectInfo, List<ISelector<String>> selectors) {
    }

    private void initRegister() {
        mediaSelectorSub = RxBus.getInstance().toObserverable(RXBusMessage.class)
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<RXBusMessage>() {
                    @Override
                    public void call(RXBusMessage rxBusMessage) {
                        if (TextUtils.equals(rxBusMessage.action, LocalMediaSelectFragment.MSG_SELECTED_MEDIA_OK)) {
                            List<ISelector> iSelectors = (List<ISelector>) rxBusMessage.getData();
                            for (ISelector selector : iSelectors) {
                                ActionManager.getInstance().dispatchAction(new BaseActionCreator<ISelector<String>>(selector) {
                                    @Override
                                    public IEditAction createAction() {
                                        IEditAction action = createAddMediaClipAction();
                                        if (action instanceof AddMediaClipAction) {
                                            AddMediaClipAction addAction = (AddMediaClipAction) action;
                                            addAction.setAddParams(params.getSelector(), -1);
                                        }
                                        return action;
                                    }
                                });
                            }
                            /*MediaSource[] sources = updateEngineData(iSelectors, true, false, -1);
                            if (sources != null) {
                                updateUI(extractSource(sources), false, -1, null);
                            }*/
                        } else if (TextUtils.equals(rxBusMessage.action, MSG_SELECED_MEDIA_ADD_POS)) {
                            ITimeLineItem item = timeLineActionTimeReference.get();
                            int pos = timeLine.getTimeLineItemList().indexOf(item);
                            List<ISelector> iSelectors = (List<ISelector>) rxBusMessage.getData();
                            if (pos != -1) {
                                for (ISelector selector : iSelectors) {
                                    ActionManager.getInstance().dispatchAction(new BaseActionCreator2<ISelector<String>, Integer>(selector, pos) {
                                        @Override
                                        public IEditAction createAction() {
                                            IEditAction action = createAddMediaClipAction();
                                            if (action instanceof AddMediaClipAction) {
                                                AddMediaClipAction addAction = (AddMediaClipAction) action;
                                                addAction.setAddParams(params.getSelector(), params2);
                                            }
                                            return action;
                                        }
                                    });
                                }
                                /*MediaSource[] mediaSources = updateEngineData(iSelectors, true, true, timeLinePositionPlayerPos(pos));
                                if (mediaSources != null) {
                                    updateUI(extractSource(mediaSources), true, pos, null);
                                }*/
                            }
                        }
                    }
                });
    }

    private void unRegister() {
        if (mediaSelectorSub != null) {
            mediaSelectorSub.unsubscribe();
        }
    }

    private void initToolBar(View v) {
        imageAddMedia = v.findViewById(R.id.img_add_media);
        imagePlayMedia = (ImageView) v.findViewById(R.id.img_play_media);
        imageBackAction = v.findViewById(R.id.img_back_action);
        imageUnBackAction = v.findViewById(R.id.img_unback_action);

        imageBackAction.setEnabled(false);
        imageUnBackAction.setEnabled(false);
        ActionManager.getInstance().setActionStackChangeListener(new ActionManager.OnActionStackChangeListener() {
            @Override
            public void onBackStackChange() {
                imageBackAction.setEnabled(ActionManager.getInstance().isCanBack());
            }

            @Override
            public void onUnBackStackChange() {
                imageUnBackAction.setEnabled(ActionManager.getInstance().isCanUnBack());
            }
        });

        imageAddMedia.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FragUtil.startSingleMediaSelect(getActivity());
            }
        });
        imagePlayMedia.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean isPlaying = v.getTag() != null ? (Boolean) v.getTag() : false;
                if (isPlaying) {
                    pausePlayer();
                } else {
                    startPlayer();
                }
            }
        });
        imageBackAction.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackActionClick(v);
            }
        });

        imageUnBackAction.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onUnBackActionClick(v);
            }
        });
    }

    /**
     * 撤销点击事件
     *
     * @param v
     */
    protected void onBackActionClick(View v) {
        ActionManager.getInstance().onBackAction();
    }

    /**
     * 反撤销点击事件
     *
     * @param v
     */
    protected void onUnBackActionClick(View v) {
        ActionManager.getInstance().onUnBackAction();
    }

    public void updatePlayViewImage(boolean isPlaying) {
        mainHandler.post(new AbsRunnable<Boolean>(isPlaying) {
            @Override
            public void run(Boolean data) {
                imagePlayMedia.setTag(data);
                int res = data ? R.mipmap.icon_img_pause_white : R.mipmap.icon_img_play_white;
                imagePlayMedia.setImageResource(res);
            }
        });
    }

    public void startPlayer() {
        imagePlayMedia.setTag(true);
    }

    public void pausePlayer() {
        imagePlayMedia.setTag(false);
    }

    private void initTitleBarView(View v) {
        closePageView = v.findViewById(R.id.close_page);
        ptojTitltTextView = (TextView) v.findViewById(R.id.title_proj_name);
        titleOutPutView = v.findViewById(R.id.title_out_put);
        imageOutPutView = v.findViewById(R.id.image_out_put);
        imageOutPutView.setVisibility(View.GONE);

        closePageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getActivity().finish();
            }
        });

        titleOutPutView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean isOutputFragVisible = outputConfigFragment != null && outputConfigFragment.isVisible();
                setOutputFrag(!isOutputFragVisible);
            }
        });
        imageOutPutView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                setOutputFrag(false);
            }
        });
    }

    public void setProjectTitle(String title) {
        ptojTitltTextView.setText(title);
        if (currentEditProjectInfo != null) {
            ((LocalProject) currentEditProjectInfo).setTitle(title);
        }
    }

    private void setOutputFrag(boolean isShow) {
        FragmentManager fm = getChildFragmentManager();
        outputConfigFragment = (OutputConfigFragment) fm.findFragmentByTag("OUT_PUT_CONFIG");
        FragmentTransaction transaction = fm.beginTransaction();
        transaction.setCustomAnimations(R.anim.frag_bottom_in, R.anim.frag_bottom_out);
        if (outputConfigFragment == null) {
            outputConfigFragment = new OutputConfigFragment();
            outputConfigFragment.setOnOutputClickListener(new OutputConfigFragment.OnOutputClickListener() {
                @Override
                public void onOutputClick(View v, OutputConfigFragment.IOutputConfig outputConfig) {
                    onOutPut(outputConfig);
                }
            });
            if (isShow) {
                transaction.add(R.id.bottom_view_container, outputConfigFragment, "OUT_PUT_CONFIG")
                        .commitAllowingStateLoss();
            }
        } else {
            if (isShow) {
                transaction.add(R.id.bottom_view_container, outputConfigFragment, "OUT_PUT_CONFIG")
                        .commitAllowingStateLoss();
            } else {
                transaction.remove(outputConfigFragment)
                        .commitAllowingStateLoss();
            }
        }
    }


    @Override
    public void onPause() {
        super.onPause();
    }

    /**
     * 输出
     *
     * @param outputConfig
     */
    public void onOutPut(OutputConfigFragment.IOutputConfig outputConfig) {

    }

    //test-------------------------------

    /**
     * 把选择的数据解析成视频资源类型
     *
     * @param sources
     * @return
     */
    private Observable.OnSubscribe<List<VideoSource>> extractSource(List<ISelector> sources) {
        return new Selector2VideoSourceOnSubscribe(sources);
    }


    protected Observable.OnSubscribe<List<VideoSource>> extractSource(MediaSource... sources) {
        return new MediaSource2VideoSourceSubscribe(sources);
    }

    /**
     * 获取项目的 时间线资源数据数据
     *
     * @param projectInfo
     * @return
     */
    protected Observable.OnSubscribe<List<VideoSource>> getProjectSource(final IProjectInfo projectInfo) {
        Observable.OnSubscribe<List<VideoSource>> subscribe = new Observable.OnSubscribe<List<VideoSource>>() {
            @Override
            public void call(Subscriber<? super List<VideoSource>> subscriber) {
                subscriber.onNext(null);
            }
        };
        return subscribe;
    }

    /**
     * 添加时间线上资源
     *
     * @param videoSources
     * @param isAddPosition
     * @param addPosition
     */
    public final void updateTimeLineData(List<VideoSource> videoSources, final boolean isAddPosition, final int addPosition) {
        if (isAddPosition && addPosition != -1) {
            calculateSourceTimeLineStartTime(timeLine.getTimeLinePositionEndTime(addPosition), videoSources);
            List<ITimeLineItem> datas = createTimeLineData(videoSources);
            datas.add(0, new TimeLineSplitAdd());
            timeLine.addPositionData(addPosition, datas);
            timeLine.updateUI();
        } else {
            calculateSourceTimeLineStartTime(timeLine.getTimeLineEndTime(), videoSources);
            List<ITimeLineItem> datas = createTimeLineData(videoSources);
            if (timeLine.getTimeLineItemList() != null && timeLine.getTimeLineItemList().size() > 0) {
                //有数据的时候增加数据需要添加分割数据
                datas.add(0, new TimeLineSplitAdd());
            }
            timeLine.updateData(datas, true);
        }
    }

    protected Looper getEngineThreadLooper() {
        return null;
    }

    protected IEditAction createAddMediaClipAction() {
        return null;
    }

    public final void updateUI(Observable.OnSubscribe<List<VideoSource>> sourceSubscribe,
                               final boolean isAddPosition, final int addPosition, final Action1<List<VideoSource>> afterAction) {
        Observable.create(sourceSubscribe)
                .subscribeOn(AndroidSchedulers.from(getEngineThreadLooper()))
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<List<VideoSource>>() {
                    @Override
                    public void call(List<VideoSource> list) {
                        if (afterAction != null) {
                            afterAction.call(list);
                        }
                        updateTimeLineData(list, isAddPosition, addPosition);
                    }
                });
    }

    /**
     * 时间线的位置转播放数据的位置
     *
     * @param pos
     * @return
     */
    protected int timeLinePositionPlayerPos(int pos) {

        return pos;
    }

    private void calculateSourceTimeLineStartTime(long startOffTime, List<VideoSource> data) {
        if (data != null) {
            long startTime = startOffTime;
            for (VideoSource source : data) {
                source.timeLineStartTime = startTime;
                startTime += source.getTimeLineDuration();
            }
        }
    }

    private List<ITimeLineItem> createTimeLineData(List<VideoSource> data) {
        List<ITimeLineItem> itemList = new ArrayList<>();
        if (data != null) {
            for (int i = 0; i < data.size(); i++) {
                VideoSource source = data.get(i);
                long startOffSet = source.timeLineStartTime;
                itemList.addAll(createSourceItemList(startOffSet, source));
                if (i != data.size() - 1) {
                    itemList.add(new TimeLineSplitAdd());
                }
            }
        }

        return itemList;
    }


    private List<ITimeLineItem> createSourceItemList(long startTime, VideoSource sourceData) {
        List<ITimeLineItem> list = new ArrayList<>();
        if (sourceData != null && sourceData.frameThumbInfoList != null) {
            long start = startTime;
            float ratio = sourceData.degree == 90 || sourceData.degree == 270 ? sourceData.height / ((float) sourceData.width)
                    : sourceData.width / ((float) sourceData.height);
            //计算显示宽度。按宽高比例显示
            float imageWidth = ratio * ITimeLineUI.TIMELINE_HEIGHT;
            float perSecondsWidth = imageWidth / 3000;
            sourceData.setOneTimeMSWidthRatio(perSecondsWidth);
            sourceData.timeLineStartTime = start;
            TimeLineVideo videoFrameThumb = VideoSourceHelper.sourceToTimeLineItem(sourceData);
            list.add(videoFrameThumb);
        }
        return list;
    }


    @Override
    public void onDestroyView() {
        super.onDestroyView();
        ActionManager.getInstance().destory();
        ProjectManager.getInstance().clearCurrentInfo();
        unRegister();
    }

    @Override
    public boolean onBackPressed() {
        //禁止返回键
        return true;
    }

    static class MediaSource2VideoSourceSubscribe implements Observable.OnSubscribe<List<VideoSource>> {
        private MediaSource[] sources;
        private VideoSourceHelper sourceHelper;

        public MediaSource2VideoSourceSubscribe(MediaSource... sources) {
            this.sources = sources;
            sourceHelper = new VideoSourceHelper();
        }

        @Override
        public void call(Subscriber<? super List<VideoSource>> subscriber) {
            List<VideoSource> list = new ArrayList<>();
            for (int i = 0; i < sources.length; i++) {
                MediaSource media = sources[i];
                VideoSource source = sourceHelper.createVideoSource(media);
                source.sourceWindowIndex = i;
                list.add(source);
            }
            subscriber.onNext(list);
        }
    }

    static class Selector2VideoSourceOnSubscribe implements Observable.OnSubscribe<List<VideoSource>> {

        private List<ISelector> selectors;
        private VideoSourceHelper sourceHelper;

        public Selector2VideoSourceOnSubscribe(List<ISelector> selectors) {
            this.selectors = selectors;
            sourceHelper = new VideoSourceHelper();
        }

        @Override
        public void call(Subscriber<? super List<VideoSource>> subscriber) {
            List<VideoSource> list = new ArrayList<>();
            for (int i = 0; i < selectors.size(); i++) {
                ISelector<String> media = selectors.get(i);
                VideoSource source = sourceHelper.createVideoSource(media.getSelector());
                source.sourceWindowIndex = i;
                list.add(source);
            }
            subscriber.onNext(list);
        }
    }
}
