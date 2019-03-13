package com.dfsx.lzcms.liveroom.model;

/**
 * 广告对象
 */
public class AD {

    private long id;
    private String ADLink;
    private long duration;
    private String name;
    /**
     * 跳过时长 0，表示不能跳過， 其他表示在skipTime之后能跳过
     */
    private long skipTime;
    /**
     * 是否执行结束
     */
    private boolean isRunOver;


    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getADLink() {
        return ADLink;
    }

    public void setADLink(String ADLink) {
        this.ADLink = ADLink;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public boolean isRunOver() {
        return isRunOver;
    }

    public void setRunOver(boolean runOver) {
        isRunOver = runOver;
    }

    public long getSkipTime() {
        return skipTime;
    }

    public void setSkipTime(long skipTime) {
        this.skipTime = skipTime;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
