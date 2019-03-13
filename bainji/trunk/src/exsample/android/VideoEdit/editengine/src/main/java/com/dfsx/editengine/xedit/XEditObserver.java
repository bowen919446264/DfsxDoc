package com.dfsx.editengine.xedit;

import android.util.Log;

import com.dfsx.editengine.bean.IEngineObserver;
import com.dfsx.editengine.bean.Track;
import com.dfsx.editengine.util.EngineUtil;
import com.dfsx.editengine.util.LogUtil;
import com.ds.xedit.jni.ETimeLineStatus;
import com.ds.xedit.jni.ITimeLine;
import com.ds.xedit.jni.ITimeLineObserver;
import com.ds.xedit.jni.ITrack;
import com.ds.xedit.jni.Rational;

import java.util.ArrayList;
import java.util.List;

public class XEditObserver extends ITimeLineObserver {
    private List<IEngineObserver> engineObservers;
    private ITimeLine timeLine;

    private IEngineObserver.PlayStatus latestStatus = IEngineObserver.PlayStatus.None;

    public XEditObserver(ITimeLine timeLine) {
        this.timeLine = timeLine;
        engineObservers = new ArrayList<>();
    }

    public void addEngineObserver(IEngineObserver engineObserver) {
        engineObservers.add(engineObserver);
    }

    public void removeEngineObserver(IEngineObserver engineObserver) {
        engineObservers.remove(engineObserver);
    }

    @Override
    public void onPosDidChanged(Rational rNewPos) {
        Log.e(LogUtil.TAG, "The new position is " + rNewPos.getNNum() + "/" + rNewPos.getNDen());
    }

    public void onTrackCreated(ITrack pTrack) {
        for (IEngineObserver engineObserver : engineObservers) {
            if (engineObserver != null) {
                engineObserver.onTrackCreated(new Track(pTrack));
            }

        }
        Log.e(LogUtil.TAG, "onTrackCreated: track id -  " + pTrack.getId());
    }

    public void onTrackRemoved(long trackId) {
        for (IEngineObserver engineObserver : engineObservers) {
            if (engineObserver != null && timeLine != null) {
                engineObserver.onTrackRemoved(new Track(timeLine.getTrack(trackId)));
            }
        }
        Log.e(LogUtil.TAG, "onTrackRemoved: track id -  " + trackId);
    }

    @Override
    public void onTimeLineStatusChanged(ETimeLineStatus newStatus) {
        IEngineObserver.PlayStatus status = EngineUtil.getPlayStatus(newStatus);
        if (status != latestStatus) {
            latestStatus = status;
            for (IEngineObserver engineObserver : engineObservers) {
                if (engineObserver != null && timeLine != null) {
                    engineObserver.onPlayStatusChanged(status);
                }
            }
        }
        Log.e(LogUtil.TAG, "onTimeLineStatusChanged: -----  " + newStatus.toString());
    }
}
