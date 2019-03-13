package com.dfsx.editengine.util;

import android.text.TextUtils;

import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.bean.IEngineObserver;
import com.ds.xedit.jni.ETimeLineStatus;
import com.ds.xedit.jni.ETrackType;
import com.ds.xedit.jni.Rational;

public class EngineUtil {

    /**
     * @param time 时间  单位ms
     * @return
     */
    public static Rational timeToRational(long time) {
        return new Rational(time, 1000);
    }

    /**
     * @param str 格式为 2/3;
     * @return
     */
    public static Rational strToRational(String str) {
        try {
            if (!TextUtils.isEmpty(str)) {
                String[] arr = str.split("\\/");

                return new Rational(Long.valueOf(arr[0].replace(" ", "")),
                        Long.valueOf(arr[1].replace(" ", "")));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return new Rational(0, 1);
    }

    /**
     * 返回时间ms
     *
     * @param rational
     * @return
     */
    public static long rationalToTime(Rational rational) {
        return (long) (rational.doubleValue() * 1000);
    }

    public static String rationalToString(Rational r) {
        return String.format(" %d / %d", r.getNNum(), r.getNDen());
    }

    public static ETrackType getETrackType(IEditEngine.TrackType trackType) {
        ETrackType type;
        switch (trackType) {
            case AUDIO:
                type = ETrackType.ETrackType_Audio;
                break;
            case VIDEO:
                type = ETrackType.ETrackType_Video;
                break;
            default:
                type = ETrackType.ETrackType_Invalid;
                break;
        }
        return type;
    }

    public static IEditEngine.TrackType getTrackType(ETrackType trackType) {
        IEditEngine.TrackType type;
        switch (trackType) {
            case ETrackType_Audio:
                type = IEditEngine.TrackType.AUDIO;
                break;
            case ETrackType_Video:
                type = IEditEngine.TrackType.VIDEO;
                break;
            default:
                type = IEditEngine.TrackType.VIDEO;
                break;
        }
        return type;
    }

    public static IEngineObserver.PlayStatus getPlayStatus(ETimeLineStatus status) {
        IEngineObserver.PlayStatus playStatus;
        switch (status) {
            case ETimeLineStatus_None:
                playStatus = IEngineObserver.PlayStatus.None;
                break;
            case ETimeLineStatus_Playing:
                playStatus = IEngineObserver.PlayStatus.Playing;
                break;
            case ETimeLineStatus_Pause:
                playStatus = IEngineObserver.PlayStatus.Pause;
                break;
            case ETimeLineStatus_PlayEnd:
                playStatus = IEngineObserver.PlayStatus.PlayEnd;
                break;
            case ETimeLineStatus_DropFrame:
                playStatus = IEngineObserver.PlayStatus.DropFrame;
                break;
            case ETimeLineStatus_PlayFailed:
                playStatus = IEngineObserver.PlayStatus.PlayFailed;
                break;
            case ETimeLineStatus_Error:
                playStatus = IEngineObserver.PlayStatus.Error;
                break;
            case ETimeLineStatus_Generating:
                playStatus = IEngineObserver.PlayStatus.Generating;
                break;
            case ETimeLineStatus_GenerateFinish:
                playStatus = IEngineObserver.PlayStatus.GenerateFinish;
                break;
            case ETimeLineStatus_GenerateFailed:
                playStatus = IEngineObserver.PlayStatus.GenerateFailed;
                break;
            default:
                playStatus = IEngineObserver.PlayStatus.None;
                break;
        }
        return playStatus;
    }
}
