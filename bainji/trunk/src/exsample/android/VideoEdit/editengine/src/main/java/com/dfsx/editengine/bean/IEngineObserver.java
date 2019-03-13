package com.dfsx.editengine.bean;

public interface IEngineObserver {

    enum PlayStatus {
        None,
        Playing,
        Pause,
        PlayEnd,
        DropFrame,
        PlayFailed,
        Error,
        Generating,
        GenerateFinish,
        GenerateFailed
    }

    void onPlayStatusChanged(PlayStatus status);

    /**
     * 有track 创建
     *
     * @param track
     */
    void onTrackCreated(Track track);

    /**
     * 有track 移除
     *
     * @param track
     */
    void onTrackRemoved(Track track);
}
