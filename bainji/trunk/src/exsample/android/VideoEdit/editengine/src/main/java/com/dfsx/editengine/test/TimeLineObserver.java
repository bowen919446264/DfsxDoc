package com.dfsx.editengine.test;

import android.util.Log;

import com.ds.xedit.jni.ETimeLineStatus;
import com.ds.xedit.jni.ITimeLineObserver;
import com.ds.xedit.jni.ITrack;
import com.ds.xedit.jni.Rational;

/**
 * 时间线观察者
 */
public class TimeLineObserver extends ITimeLineObserver {
    private MainActivity mainActivity;

    public TimeLineObserver(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }

    @Override
    public void onPosDidChanged(Rational rNewPos) {
        Log.i("TimeLineObserver", "The new position is " + rNewPos.getNNum() + "/" + rNewPos.getNDen());
    }

    @Override
    public void onTrackCreated(ITrack pTrack) {
        Log.i("TimeLineObserver", "onTrackCreated: track id -  " + pTrack.getId());
    }

    @Override
    public void onTrackRemoved(long trackId) {
        Log.i("TimeLineObserver", "onTrackRemoved: track id -  " + trackId);
    }

    @Override
    public void onTimeLineStatusChanged(ETimeLineStatus newStatus) {
        Log.i("TimeLineObserver", "onTimeLineStatusChanged - " + newStatus);
    }
}
