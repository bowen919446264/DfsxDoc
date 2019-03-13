package com.dfsx.videoeditor.edit;

import android.content.Context;
import android.text.TextUtils;

import com.dfsx.editengine.BaseEngineMainCallBack;
import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.video.VideoSourceHelper;
import com.dfsx.videoeditor.widget.timeline.ITimeLineItem;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineSplitAdd;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import java.util.ArrayList;

import rx.functions.Action1;

/**
 * 添加资源动作
 */
public class AddMediaClipAction extends EngineBaseEditAction {

    private String mediaPath;
    private int pos;

    private TimeLineVideoSource addVideoSourceItem;

    public AddMediaClipAction(Context context, ITimeLineUI timeLineUI, EngineProjectManager projectManager) {
        super(context, timeLineUI, projectManager);
    }

    /**
     * @param mediaPath   资源
     * @param timeLinePos 时间线坐标 可以为-1，（-1表示加到末尾）
     */
    public void setAddParams(String mediaPath, int timeLinePos) {
        this.mediaPath = mediaPath;
        this.pos = timeLinePos;
    }

    private void onUIDo(MediaSource source, Action1<Boolean> action1) {
        //设置UI
        VideoSourceHelper helper = new VideoSourceHelper();
        VideoSource videoSource = helper.createVideoSource(source);
        addVideoSourceItem = VideoSourceHelper.sourceToTimeLineItem(videoSource);
        ArrayList<ITimeLineItem> addList = new ArrayList<>();
        if (pos < 0) {
            addList.add(addVideoSourceItem);
            timeLineUI.updateData(addList, true, true);
        } else {
            if (pos > 0) {
                ITimeLineItem beforItem = timeLineUI.getTimeLineItemList().get(pos - 1);
                if (!(beforItem instanceof TimeLineSplitAdd)) {
                    addList.add(new TimeLineSplitAdd());
                }
            }
            addList.add(addVideoSourceItem);
            //验证后一个位置 不需要+1， 因为是插入算法
            if (pos < timeLineUI.getTimeLineItemList().size()) {
                ITimeLineItem afterItem = timeLineUI.getTimeLineItemList().get(pos);
                if (!(afterItem instanceof TimeLineSplitAdd)) {
                    addList.add(new TimeLineSplitAdd());
                }
            }
            timeLineUI.addPositionData(pos, addList);
        }
        action1.call(true);
    }

    @Override
    public void onDo(Action1<Boolean> action1) {
        if (!TextUtils.isEmpty(mediaPath)) {
            //设置引擎
            if (pos < 0) {
                engineProjectManager.addSource(mediaPath, new BaseEngineMainCallBack<MediaSource>(action1) {
                    @Override
                    public void onCallBack(MediaSource data) {
                        Action1<Boolean> action11 = (Action1<Boolean>) params[0];
                        if (data != null) {
                            onUIDo(data, (Action1<Boolean>) params[0]);
                        } else {
                            action11.call(false);
                        }
                    }
                });
            } else {
                long time = pos > 0 ? timeLineUI.getTimeLinePositionEndTime(pos - 1) : 0;
                engineProjectManager.addSourceInTimeLineTime(mediaPath, IEditEngine.TrackType.VIDEO, 0,
                        time, new BaseEngineMainCallBack<MediaSource>(action1) {
                            @Override
                            public void onCallBack(MediaSource data) {
                                Action1<Boolean> action11 = (Action1<Boolean>) params[0];
                                if (data != null) {
                                    onUIDo(data, (Action1<Boolean>) params[0]);
                                } else {
                                    action11.call(false);
                                }
                            }
                        });
            }

        } else {
            action1.call(false);
        }
    }

    @Override
    public void onCancelDo(Action1<Boolean> action) {
        try {
            if (addVideoSourceItem != null) {
                MediaSource mediaSource = addVideoSourceItem.getPlayerSource().getEngineMediaSource();
                //设置引擎
                engineProjectManager.deleteSource(mediaSource, new BaseEngineMainCallBack<Boolean>(action) {
                    @Override
                    public void onCallBack(Boolean data) {
                        Action1<Boolean> action11 = (Action1<Boolean>) params[0];
                        if (data) {
                            //设置UI
                            ArrayList<ITimeLineItem> delList = new ArrayList<>();
                            int index = timeLineUI.getTimeLineItemList().indexOf(addVideoSourceItem);
                            if (index - 1 >= 0) {
                                ITimeLineItem beforeItem = timeLineUI.getTimeLineItemList().get(index - 1);
                                if (beforeItem instanceof TimeLineSplitAdd) {
                                    delList.add(beforeItem);
                                }
                            }
                            delList.add(addVideoSourceItem);
                            timeLineUI.removeTimeLineDataList(delList);
                            action11.call(true);
                        } else {
                            action11.call(false);
                        }
                    }
                });

            }
        } catch (Exception e) {
            e.printStackTrace();
            action.call(false);
        }
    }
}
