package com.dfsx.editengine.bean;

import com.ds.xedit.jni.ITrack;

public class Track {
    private long id;
    private ITrack track;

    public Track(ITrack track) {
        this.track = track;
        this.id = track.getId();
    }

    public long getId() {
        return id;
    }
}
