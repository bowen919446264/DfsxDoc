package com.dfsx.editengine.bean;

import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.dfsx.editengine.IEditEngine;
import com.dfsx.editengine.NativeHelper;
import com.dfsx.editengine.util.EngineUtil;
import com.dfsx.editengine.util.LogUtil;
import com.ds.xedit.jni.AVMediaInfo;
import com.ds.xedit.jni.EMediaType;
import com.ds.xedit.jni.IAVClip;
import com.ds.xedit.jni.IAVMedia;
import com.ds.xedit.jni.IClip;
import com.ds.xedit.jni.IMedia;
import com.ds.xedit.jni.IPreview;
import com.ds.xedit.jni.ITrack;
import com.ds.xedit.jni.PreviewFrame;
import com.ds.xedit.jni.Rational;
import com.ds.xedit.jni.VideoStream;

import java.io.Serializable;

public class MediaSource {
    private IMedia media;
    private IClip sourceClip;
    /**
     * 当前资源所在的线路
     */
    private ITrack track;

    public MediaSource(IMedia media, IClip sourceClip, ITrack track) {
        this.media = media;
        this.sourceClip = sourceClip;
        this.track = track;
    }

    /**
     * 跟新数据
     *
     * @param source
     */
    public void update(MediaSource source) {
        if (source != null) {
            if (source.media != null) {
                this.media = source.media;
            }
            if (source.sourceClip != null) {
                this.sourceClip = source.sourceClip;
            }
            if (source.track != null) {
                this.track = source.track;
            }
        }
    }

    public IClip getSourceClip() {
        return sourceClip;
    }

    public IMedia getMedia() {
        return media;
    }

    /**
     * 设置当前资源的 播放区间
     *
     * @param startSourceTime 相对于原始视频的 开始位置
     */
    public void setPlayRangeStart(long startSourceTime) {
        if (sourceClip != null && track != null) {
            //设置开始位置
            Rational startRational = EngineUtil.timeToRational(startSourceTime);
            if (!startRational.isEqualTo(getOffsetInMedia())) {
                track.changeClipOffsetInMedia(sourceClip.getId(), startRational);
            }
            Log.e(LogUtil.TAG, "clip play range start success --- " +
                    EngineUtil.rationalToString(startRational));
        } else {
            Log.e(LogUtil.TAG, "资源 切片是null， 不能操作开始位置和结束位置");
        }
    }


    /**
     * @param playDuration 可播放视频的长度
     */
    public void setPlayRangeDuration(long playDuration) {
        if (sourceClip != null) {
            Rational durationRational = EngineUtil.timeToRational(playDuration);
            track.changeClipDuration(sourceClip.getId(), durationRational);
            Log.e(LogUtil.TAG, "clip play range duration success --- " +
                    EngineUtil.rationalToString(durationRational));
        } else {
            Log.e(LogUtil.TAG, "资源 切片是null， 不能操作开始位置和结束位置");
        }

    }

    /**
     * 设置资源的播放区间
     *
     * @param playRange 相对于原始视频的时间刻度
     */
    public void setPlayRange(long[] playRange) {
        if (playRange != null && playRange.length == 2) {
            setPlayRangeStart(playRange[0]);
            setPlayRangeDuration(playRange[1] - playRange[0]);
        }
    }


    private Rational getOffsetInMedia() {
        if (sourceClip != null) {
            IAVClip avClip = IAVClip.dynamic_cast(sourceClip);
            Rational offsetInMedia = avClip.getOffsetInMedia();
            return offsetInMedia;
        }
        return null;
    }

    /**
     * 返回当前资源的播放区间 相对于原始视频的区间
     *
     * @return
     */
    public long[] getPlayRange() {
        if (sourceClip != null) {
            Rational offsetInMedia = getOffsetInMedia();
            long[] arr = new long[2];
            arr[0] = EngineUtil.rationalToTime(offsetInMedia);
            arr[1] = EngineUtil.rationalToTime(sourceClip.getDuration().add(offsetInMedia));
            return arr;
        }
        return null;
    }

    public long getId() {
        if (sourceClip != null) {
            return sourceClip.getId();
        }
        return -1;
    }

    public long getDuration() {
        if (sourceClip != null) {
            return EngineUtil.rationalToTime(sourceClip.getDuration());
        }
        return 0;
    }

    public String getSourcePath() {
        return media.getPath();
    }

    /**
     * 创建预览帧
     *
     * @param time 帧时间
     * @return
     */
    public String createPreFrameImage(long time) {
        String frameImage = getFrameImage(time);
        if (TextUtils.isEmpty(frameImage)) {
            if (media.getMediaType() == EMediaType.EMediaType_AV) {
                Rational offsetInMedia = EngineUtil.timeToRational(time);
                IAVMedia avMedia = IAVMedia.dynamic_cast(media);
                avMedia.openPreviewSession();
                // 创建预览帧
                AVMediaInfo mediaInfo = new AVMediaInfo();
                avMedia.getMediaInfo(mediaInfo);
                PreviewFrame previewFrame = avMedia.createPreviewFrame(0, offsetInMedia);
                String framePath = null;
                if (previewFrame != null) {
                    framePath = previewFrame.getPath();
                }
                avMedia.closePreviewSession();
                return framePath;
            } else {
                return null;
            }
        } else {
            return frameImage;
        }
    }

    public String getFrameImage(long time) {
        Rational offsetInMedia = EngineUtil.timeToRational(time);
        IPreview preview = media.getPreview();
        if (media.getMediaType() == EMediaType.EMediaType_AV) {
            if (preview != null && preview.getPreviewFrameNearBy(offsetInMedia) != null) {
                return preview.getPreviewFrameNearBy(offsetInMedia).getPath();
            }
        } else if (media.getMediaType() == EMediaType.EMediaType_Image) {
            if (preview.getPreviewFrameCount() > 0) {
                PreviewFrame previewFrame = preview.getPreviewFrame(0);
                return previewFrame.getPath();
            }
        }
        return null;
    }

    public ITrack getTrack() {
        return track;
    }
}
