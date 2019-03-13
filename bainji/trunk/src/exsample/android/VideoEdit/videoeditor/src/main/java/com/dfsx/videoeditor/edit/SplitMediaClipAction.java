package com.dfsx.videoeditor.edit;

import android.content.Context;
import android.util.Log;

import com.dfsx.editengine.BaseEngineMainCallBack;
import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.test.TimeLineVideo;
import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.video.VideoSourceHelper;
import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineSplitAdd;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import java.util.ArrayList;
import java.util.List;

import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;

/**
 * 把一个切片拆分成2个
 */
public class SplitMediaClipAction extends EngineBaseEditAction {

    /**
     * 拆分前的数据
     */
    private TimeLineVideoSource<VideoSource> selectedItem;
    private int selectedItemIndex;
    private long lineTime;
    private List<ITimeLineItem> splitedAddDataList;
    /**
     * 拆分后的数据
     */
    private ITimeLineItem splitedItemData;

    /**
     * 拆分前的视频资源时间
     */
    private long[] sourceRange1;
    /**
     * 拆分后的视频资源时间
     */
    private long[] sourceRange2;

    private VideoSourceHelper sourceHelper;

    public SplitMediaClipAction(Context context, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        super(context, timeLineUI, projectManager);
        sourceHelper = new VideoSourceHelper();
        ITimeLineItem timeLineItem = timeLineUI.getSelectedItem();
        if (timeLineItem instanceof TimeLineVideoSource) {
            selectedItem = (TimeLineVideoSource<VideoSource>) timeLineItem;
            selectedItemIndex = timeLineUI.getTimeLineItemList().indexOf(selectedItem);
        }
        lineTime = timeLineUI.getLineTime();
    }

    @Override
    public void onDo(Action1<Boolean> action) {
        try {
            if (selectedItem != null) {
                long[] playRange = selectedItem.getPlayerSource().getPlayTimeRange();
                if (playRange == null) {
                    playRange = new long[2];
                    playRange[0] = 0;
                    playRange[1] = selectedItem.getPlayerSource().getSourceDuration();
                }
                sourceRange1 = playRange.clone();

                long[] timeLineTimeRange = selectedItem.getTimeLineTime();
                if (timeLineTimeRange != null && lineTime > timeLineTimeRange[0] &&
                        lineTime < timeLineTimeRange[1]) {

                    Observable.just(timeLineTimeRange)
                            .subscribeOn(AndroidSchedulers.from(engineProjectManager.getEngineThreadLooper()))
                            .flatMap(new Func1<long[], Observable<MediaSource>>() {
                                @Override
                                public Observable<MediaSource> call(long[] timeLineTimeRange) {
                                    long spliteSourceTime = lineTime - timeLineTimeRange[0];
                                    sourceRange2 = new long[2];
                                    //时间段1
                                    sourceRange2[0] = sourceRange1[0];
                                    sourceRange2[1] = spliteSourceTime;
                                    splitedItemData = selectedItem;
                                    //片段1设置引擎
                                    ((TimeLineVideoSource) splitedItemData).getPlayerSource().getEngineMediaSource()
                                            .setPlayRangeDuration(spliteSourceTime);
                                    //片段1设置UI数据
                                    ((TimeLineVideoSource) splitedItemData).setTimeLineDuration(spliteSourceTime);
                                    ((TimeLineVideoSource) splitedItemData).getPlayerSource().setPlayTimeRange(sourceRange2);
                                    long dDuration = (sourceRange2[1] - sourceRange2[0]) - (sourceRange1[1] - sourceRange1[0]);
                                    for (int i = selectedItemIndex + 1; i < timeLineUI.getTimeLineItemList().size(); i++) {
                                        timeLineUI.getTimeLineItemList().get(i).setTimeLineTimeOffSet(dDuration);
                                    }
                                    //片段2
                                    //引擎添加片段
                                    MediaSource source = selectedItem.getPlayerSource().getEngineMediaSource();
                                    MediaSource mediaSource2 = engineProjectManager.addSourceInTimeLineTime(source,
                                            IEditEngine.TrackType.VIDEO, 0,
                                            lineTime);
                                    return Observable.just(mediaSource2);
                                }
                            })
                            .observeOn(AndroidSchedulers.from(engineProjectManager.getEngineThreadLooper()))
                            .concatMap(new Func1<MediaSource, Observable<MediaSource>>() {
                                @Override
                                public Observable<MediaSource> call(MediaSource source2) {
                                    if (source2 != null) {
                                        //设置时间段2
                                        long[] range2 = new long[2];
                                        range2[0] = lineTime - selectedItem.getTimeLineTime()[0];
                                        range2[1] = sourceRange1[1];
                                        source2.setPlayRangeStart(range2[0]);
                                        source2.setPlayRangeDuration(range2[1] - range2[0]);
                                    }
                                    return Observable.just(source2);
                                }
                            })
                            .observeOn(AndroidSchedulers.mainThread())
                            .map(new Func1<MediaSource, Boolean>() {
                                @Override
                                public Boolean call(MediaSource source2) {
                                    //设置片段2UI
                                    //设置UI
                                    if (source2 != null) {
                                        VideoSource videoSource2 = sourceHelper.createVideoSource(source2);
                                        TimeLineSplitAdd splitAdd = new TimeLineSplitAdd();
                                        TimeLineVideo timeLineVideo = VideoSourceHelper.sourceToTimeLineItem(videoSource2);
                                        splitedAddDataList = new ArrayList<>();
                                        splitedAddDataList.add(splitAdd);
                                        splitedAddDataList.add(timeLineVideo);
                                        selectedItem.getSelectedObject().setSelected(false);
                                        timeLineUI.addPositionData(selectedItemIndex + 1, splitedAddDataList);
                                    }
                                    return source2 != null;
                                }
                            })
                            .subscribe(action);
                } else {
                    Log.e("Edit", "data no work well----");
                    action.call(false);
                }
            } else {
                action.call(false);
            }
        } catch (Exception e) {
            e.printStackTrace();
            action.call(false);
        }
    }

    @Override
    public void onCancelDo(Action1<Boolean> action) {
        try {
            if (splitedItemData != null && splitedAddDataList != null && sourceRange1 != null) {
                TimeLineVideoSource timeLineVideoSource = ((TimeLineVideoSource) splitedItemData);
                //设置UI
                timeLineVideoSource.getPlayerSource().setPlayTimeRange(sourceRange1);
                timeLineVideoSource.setTimeLineDuration(sourceRange1[1] - sourceRange1[0]);
                timeLineUI.removeTimeLineDataList(splitedAddDataList);

                //设置引擎
                Observable.from(splitedAddDataList)
                        .flatMap(new Func1<ITimeLineItem, Observable<Boolean>>() {
                            @Override
                            public Observable<Boolean> call(final ITimeLineItem item) {
                                if (item instanceof TimeLineVideoSource) {
                                    Observable.OnSubscribe<Boolean> onSubscribe = new Observable.OnSubscribe<Boolean>() {
                                        @Override
                                        public void call(final Subscriber<? super Boolean> subscriber) {
                                            engineProjectManager.deleteSource(((TimeLineVideoSource) item).
                                                    getPlayerSource().getEngineMediaSource(), new BaseEngineMainCallBack<Boolean>() {
                                                @Override
                                                public void onCallBack(Boolean data) {
                                                    subscriber.onNext(data);
                                                }
                                            });
                                        }
                                    };
                                    return Observable.create(onSubscribe);
                                }
                                return Observable.just(true);
                            }
                        })
                        .toList()
                        .observeOn(AndroidSchedulers.from(engineProjectManager.getEngineThreadLooper()))
                        .map(new Func1<List<Boolean>, Boolean>() {
                            @Override
                            public Boolean call(List<Boolean> booleans) {
                                boolean isOk = true;
                                for (Boolean a : booleans) {
                                    isOk = isOk && a;
                                    if (!isOk) {
                                        break;
                                    }
                                }
                                if (isOk) {
                                    ((TimeLineVideoSource) splitedItemData).getPlayerSource().getEngineMediaSource()
                                            .setPlayRangeDuration(sourceRange1[1] - sourceRange1[0]);
                                }
                                return true;
                            }
                        })
                        .observeOn(AndroidSchedulers.mainThread())
                        .subscribe(action);
            }
        } catch (Exception e) {
            e.printStackTrace();
            action.call(false);
        }
    }
}
