package com.dfsx.videoeditor.edit;

import android.content.Context;

import com.dfsx.editengine.BaseEngineMainCallBack;
import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineSplitAdd;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import java.util.ArrayList;
import java.util.Observable;

import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;

public class DeleteMediaClipAction extends EngineBaseEditAction {
    private TimeLineVideoSource deletedVideoSource;
    private int timeLinePos = -1;

    public DeleteMediaClipAction(Context context, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        this(context, null, timeLineUI, projectManager);
    }

    public DeleteMediaClipAction(Context context, TimeLineVideoSource deletedVideoSource, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        super(context, timeLineUI, projectManager);
        this.deletedVideoSource = deletedVideoSource;
        try {
            if (this.deletedVideoSource == null) {
                this.deletedVideoSource = (TimeLineVideoSource) timeLineUI.getSelectedItem();
            }
            timeLinePos = timeLineUI.getTimeLineItemList().indexOf(this.deletedVideoSource);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void onUIDo(Action1<Boolean> result) {
        //设置UI
        ArrayList<ITimeLineItem> deleteList = new ArrayList<>();
        if (timeLinePos > 0) {
            ITimeLineItem beforeItem = timeLineUI.getTimeLineItemList().get(timeLinePos - 1);
            if (beforeItem instanceof TimeLineSplitAdd) {
                deleteList.add(beforeItem);
            }
        }
        deleteList.add(deletedVideoSource);
        deletedVideoSource.getSelectedObject().setSelected(false);//清除选中状态
        boolean is = timeLineUI.removeTimeLineDataList(deleteList);
        if (is) {//删除成功之后， 重新记录位置
            timeLinePos = timeLinePos - (deleteList.size() - 1);
            if (timeLinePos < 0) {
                timeLinePos = 0;
            }
        }
        result.call(is);
    }

    @Override
    public void onDo(Action1<Boolean> result) {
        try {
            if (deletedVideoSource != null) {
                //设置引擎
                MediaSource mediaSource = deletedVideoSource.getPlayerSource().getEngineMediaSource();
                engineProjectManager.deleteSource(mediaSource, new BaseEngineMainCallBack<Boolean>(result) {
                    @Override
                    public void onCallBack(Boolean data) {
                        Action1<Boolean> result = (Action1<Boolean>) params[0];
                        if (data) {
                            onUIDo(result);
                        } else {
                            result.call(false);
                        }
                    }
                });
            }
        } catch (Exception e) {
            e.printStackTrace();
            result.call(false);
        }
    }

    private void onUICancelDo(Action1<Boolean> result) {
        //设置UI
        ArrayList<ITimeLineItem> addList = new ArrayList<>();
        if (timeLinePos > 0) {
            ITimeLineItem beforeItem = timeLineUI.getTimeLineItemList().get(timeLinePos - 1);
            if (!(beforeItem instanceof TimeLineSplitAdd) || timeLinePos >= timeLineUI.getTimeLineItemList().size()) {
                addList.add(new TimeLineSplitAdd());
            }
        }
        addList.add(deletedVideoSource);
        timeLineUI.addPositionData(timeLinePos, addList);
        //增加成功之后重新记录位置
        timeLinePos = timeLinePos + addList.indexOf(deletedVideoSource);
        result.call(true);
    }


    @Override
    public void onCancelDo(Action1<Boolean> result) {
        if (deletedVideoSource != null && timeLinePos != -1) {
            //设置引擎
            MediaSource mediaSource = deletedVideoSource.getPlayerSource().getEngineMediaSource();
            long timeOfTimeLine = 0;
            if (timeLinePos > 0) {
                timeOfTimeLine = timeLineUI.getTimeLinePositionEndTime(timeLinePos - 1);
            }
            engineProjectManager.
                    addSourceInTimeLineTime(mediaSource, IEditEngine.TrackType.VIDEO, 0, timeOfTimeLine, new BaseEngineMainCallBack<MediaSource>(result) {
                        @Override
                        public void onCallBack(MediaSource data) {
                            rx.Observable.just(data)
                                    .observeOn(AndroidSchedulers.from(engineProjectManager.getEngineThreadLooper()))
                                    .map(new Func1<MediaSource, MediaSource>() {
                                        @Override
                                        public MediaSource call(MediaSource mediaSource) {
                                            long[] oldRange = deletedVideoSource.getPlayerSource().getPlayTimeRange();
                                            if (oldRange != null && oldRange.length == 2) {
                                                mediaSource.setPlayRange(oldRange);
                                            }
                                            return mediaSource;
                                        }
                                    })
                                    .observeOn(AndroidSchedulers.mainThread())
                                    .subscribe(new Action1<MediaSource>() {
                                        @Override
                                        public void call(MediaSource mediaSource) {
                                            Action1<Boolean> result = (Action1<Boolean>) params[0];
                                            if (mediaSource != null) {
                                                deletedVideoSource.getPlayerSource().getEngineMediaSource().update(mediaSource);
                                                onUICancelDo(result);
                                            } else {
                                                result.call(false);
                                            }
                                        }
                                    });

                        }
                    });
        } else {
            result.call(false);
        }
    }

}
