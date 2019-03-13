package com.dfsx.editengine.xedit;

import com.dfsx.editengine.bean.IActionChangeListener;
import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.editengine.util.EngineUtil;
import com.ds.xedit.jni.IAction;
import com.ds.xedit.jni.*;

public class XEditActionChangeObserver {

    private IActionChangeListener actionChangeListener;
    private ITimeLine timeLine;

    protected XEditActionChangeObserver(ITimeLine timeLine) {
        this.timeLine = timeLine;
    }

    public void setActionChangeListener(IActionChangeListener actionChangeListener) {
        this.actionChangeListener = actionChangeListener;
    }

    /**
     * @param action 执行的动作
     * @param isUndo 是否是撤销动作 true, 表示撤销， false， 表示反撤销
     */
    public void onPushAction(IAction action, boolean isUndo) {
        switch (action.getActionType()) {
            case AT_AddClip: {
                long trackId = Long.valueOf(action.getActionParam(xedit.AP_AddClip_TrackId));
                long mediaId = Long.valueOf(action.getActionParam(xedit.AP_AddClip_MediaId));
                long offsetOnTrack = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_AddClip_OffsetOnTrack)));
                long clipId = Long.valueOf(action.getActionParam(xedit.AP_AddClip_ClipId));
                if (actionChangeListener != null) {
                    if (isUndo) {
                        actionChangeListener.onMediaSourceRemove(clipId);
                    } else {
                        MediaSource mediaSource = new MediaSource(timeLine.getMediaById(mediaId), timeLine.getTrack(trackId).getClipById(clipId),
                                timeLine.getTrack(trackId));
                        actionChangeListener.onAddMediaSource(mediaSource, offsetOnTrack);
                    }
                }
            }
            break;
            case AT_AddMedia: {
                String mediaPath = action.getActionParam(xedit.AP_AddMedia_MediaPath);
                break;
            }
            case AT_ChangeClipDuration: {
                long clipId = Long.valueOf(action.getActionParam(xedit.AP_ChangeClipDuration_ClipId));
                long newDuration = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_ChangeClipDuration_NewDuration)));
                long oldDuration = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_ChangeClipDuration_OldDuration)));
                if (actionChangeListener != null) {
                    if (isUndo) {
                        actionChangeListener.onChangeMediaSourceDuration(clipId, newDuration, oldDuration);
                    } else {
                        actionChangeListener.onChangeMediaSourceDuration(clipId, oldDuration, newDuration);
                    }
                }
                break;
            }
            case AT_MoveClip: {
                long clipId = Long.valueOf(action.getActionParam(xedit.AP_MoveClip_ClipId));
                long newOffsetOnTrack = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_MoveClip_NewOffsetOnTrack)));
                long oldOffsetOnTrack = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_MoveClip_OldOffsetOnTrack)));
                if (actionChangeListener != null) {
                    if (isUndo) {
                        actionChangeListener.onMediaSourceStartTimeChange(clipId, newOffsetOnTrack, oldOffsetOnTrack);
                    } else {
                        actionChangeListener.onMediaSourceStartTimeChange(clipId, oldOffsetOnTrack, newOffsetOnTrack);
                    }
                }
                break;
            }
            case AT_RemoveClip: {
                Long clipId = Long.valueOf(action.getActionParam(xedit.AP_RemoveClip_ClipId));
                if (actionChangeListener != null) {
                    if (isUndo) {
                        //TODO 执行增加操作
                    } else {
                        actionChangeListener.onMediaSourceRemove(clipId);
                    }
                }
                break;
            }
            case AT_RemoveMedia: {
                String mediaId = action.getActionParam(xedit.AP_RemoveMedia_MediaId);
                break;
            }
            case AT_ChangeClipOffsetInMedia: {
                Long clipId = Long.valueOf(action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_ClipId));
                long newOffsetInMedia = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_NewOffsetInMedia)));
                long oldOffsetInMedia = EngineUtil.rationalToTime(EngineUtil.
                        strToRational(action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_OldOffsetInMedia)));
                if (actionChangeListener != null) {
                    if (isUndo) {
                        actionChangeListener.onMediaSourcePlayStartRangeChange(clipId, newOffsetInMedia, oldOffsetInMedia);
                    } else {
                        actionChangeListener.onMediaSourcePlayStartRangeChange(clipId, oldOffsetInMedia, newOffsetInMedia);
                    }
                }
                break;
            }
        }
    }
}
