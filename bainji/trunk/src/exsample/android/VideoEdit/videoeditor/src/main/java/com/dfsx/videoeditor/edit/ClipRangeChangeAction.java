package com.dfsx.videoeditor.edit;

import android.content.Context;
import android.support.annotation.NonNull;
import android.util.Log;

import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import rx.Observable;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;

/**
 * 设置切片的播放时间区域
 */
public class ClipRangeChangeAction extends EngineBaseEditAction {

    private TimeLineVideoSource selectedVideoItem;

    private long oldLeft;
    private long oldDuration;

    private long toSetLeft = -1;
    private long toDuration = -1;

    private boolean isUpdateUIOnDo;

    public ClipRangeChangeAction(Context context, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        super(context, timeLineUI, projectManager);
        try {
            selectedVideoItem = (TimeLineVideoSource) timeLineUI.getSelectedItem();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public ClipRangeChangeAction(Context context, TimeLineVideoSource videoSourceItem, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        super(context, timeLineUI, projectManager);
        try {
            selectedVideoItem = videoSourceItem;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 设置初始区间
     *
     * @param range
     */
    public void setStartRange(@NonNull long[] range) {
        oldLeft = range[0];
        oldDuration = range[1] - range[0];
    }

    public void setLeftTimeOfMedia(long leftTimeOfMedia) {
        toSetLeft = leftTimeOfMedia;
    }

    public void setMediaDuration(long mediaDuration) {
        toDuration = mediaDuration;
    }


    @Override
    public void onDo(Action1<Boolean> action) {
        try {
            Observable.just(null)
                    .observeOn(AndroidSchedulers.from(engineProjectManager.getEngineThreadLooper()))
                    .flatMap(new Func1<Object, Observable<Boolean>>() {
                        @Override
                        public Observable<Boolean> call(Object o) {
                            if (selectedVideoItem != null) {
                                //设置引擎
                                MediaSource mediaSource = selectedVideoItem.getPlayerSource().getEngineMediaSource();
                                if (toSetLeft != -1) {
                                    mediaSource.setPlayRangeStart(toSetLeft);
                                }
                                if (toDuration != -1) {
                                    mediaSource.setPlayRangeDuration(toDuration);
                                }
                                return Observable.just(true);
                            }
                            return Observable.just(false);
                        }
                    })
                    .observeOn(AndroidSchedulers.mainThread())
                    .map(new Func1<Boolean, Boolean>() {
                        @Override
                        public Boolean call(Boolean aBoolean) {
                            if (aBoolean && isUpdateUIOnDo) {
                                //设置UI
                                long[] range2 = new long[2];
                                range2[0] = toSetLeft != -1 ? toSetLeft : oldLeft;
                                range2[1] = toDuration != -1 ? range2[0] + toDuration : range2[0] + oldDuration;
                                long duration2 = toDuration != -1 ? toDuration : oldDuration;
                                selectedVideoItem.getPlayerSource().setPlayTimeRange(range2);
                                selectedVideoItem.setTimeLineDuration(duration2);
                                //修改列表时间
                                long dTime = duration2 - oldDuration;
                                int pos = timeLineUI.getTimeLineItemList().indexOf(selectedVideoItem);
                                for (int i = pos + 1; i < timeLineUI.getTimeLineItemList().size(); i++) {
                                    timeLineUI.getTimeLineItemList().get(i).setTimeLineTimeOffSet(dTime);
                                }
                                timeLineUI.updateUI();
                            }
                            Log.e("TAG", "Clip range do === " + aBoolean);
                            return aBoolean;
                        }
                    })
                    .subscribe(action);
        } catch (Exception e) {
            e.printStackTrace();
            action.call(false);
        }
    }

    @Override
    public void onCancelDo(Action1<Boolean> action) {
        try {
            Observable.just(null)
                    .observeOn(AndroidSchedulers.from(engineProjectManager.getEngineThreadLooper()))
                    .flatMap(new Func1<Object, Observable<long[]>>() {
                        @Override
                        public Observable<long[]> call(Object o) {
                            if (selectedVideoItem != null) {
                                long[] range = new long[2];
                                range[0] = oldLeft;
                                range[1] = oldLeft + oldDuration;
                                //设置引擎
                                MediaSource mediaSource = selectedVideoItem.getPlayerSource().getEngineMediaSource();
                                mediaSource.setPlayRangeStart(oldLeft);
                                mediaSource.setPlayRangeDuration(oldDuration);
                                return Observable.just(range);
                            }
                            return Observable.just(null);
                        }
                    })
                    .observeOn(AndroidSchedulers.mainThread())
                    .map(new Func1<long[], Boolean>() {
                        @Override
                        public Boolean call(long[] range) {
                            if (range != null) {
                                //设置UI
                                selectedVideoItem.setTimeLineDuration(oldDuration);
                                selectedVideoItem.getPlayerSource().setPlayTimeRange(range);
                                //设置时间线列表时间
                                long justDuration = toDuration == -1 ? oldDuration : toDuration;
                                long dTime = oldDuration - justDuration;
                                int pos = timeLineUI.getTimeLineItemList().indexOf(selectedVideoItem);
                                for (int i = pos + 1; i < timeLineUI.getTimeLineItemList().size(); i++) {
                                    timeLineUI.getTimeLineItemList().get(i).setTimeLineTimeOffSet(dTime);
                                }
                                timeLineUI.updateUI();
                                return true;
                            }
                            return false;
                        }
                    }).subscribe(action);

        } catch (Exception e) {
            e.printStackTrace();
            action.call(false);
        }

    }

    /**
     * 在执行的时候， 是否更新UI （仅在 onDo的时候有用）
     *
     * @param updateUIOnDo
     */
    public void setUpdateUIOnDo(boolean updateUIOnDo) {
        isUpdateUIOnDo = updateUIOnDo;
    }
}
