package com.dfsx.videoeditor.video;

import android.text.TextUtils;
import android.util.Log;

import com.dfsx.editengine.bean.MediaSource;
import com.dfsx.videoeditor.test.TimeLineVideo;
import com.dfsx.videoeditor.util.EditConstants;
import com.dfsx.videoeditor.widget.timeline.BaseSelectedObject;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.ImageListView;

import java.io.File;
import java.util.ArrayList;

public class VideoSourceHelper {

    private static final boolean isUseMediaCodecFrame = true;

    public VideoSourceHelper() {

    }


    private float calculateOneTimeMSWidth(VideoSource sourceData, long widthTime) {
        float ratio = sourceData.degree == 90 || sourceData.degree == 270 ? sourceData.height / ((float) sourceData.width)
                : sourceData.width / ((float) sourceData.height);
        //计算显示宽度。按宽高比例显示
        float imageWidth = ratio * ITimeLineUI.TIMELINE_HEIGHT;
        float perMSWidth = imageWidth / widthTime;

        return perMSWidth;
    }

    /**
     * 通过MediaSource
     *
     * @param source
     * @return
     */
    public VideoSource createVideoSource(MediaSource source) {
        try {
            VideoSource source1 = new VideoSource();
            ExtractVideoInfoHelper infoHelper = null;
            try {
                String videoFilePath = source.getSourcePath();
                infoHelper = new ExtractVideoInfoHelper(videoFilePath);

                source1.sourcePath = videoFilePath;
                source1.degree = infoHelper.getVideoDegree();
                source1.videoDuration = Long.valueOf(infoHelper.getVideoLength());
                source1.mintype = infoHelper.getMimetype();
                source1.width = infoHelper.getVideoWidth();
                source1.height = infoHelper.getVideoHeight();
                source1.sourceDuration = source1.videoDuration;

                long frameAddIndex = 3 * 1000;//ms
                long startTime = 0;
                String nameDir = new File(videoFilePath).getName();
                String saveDir = EditConstants.FRAME_IMAGE_CACHE_DIR;
                File dir = new File(saveDir);
                if (!dir.exists()) {
                    dir.mkdirs();
                }
                saveDir += nameDir;

                ArrayList<FrameThumbInfo> frameThumbInfos = new ArrayList<>();
                for (int i = 0; i < source1.sourceDuration; i += frameAddIndex) {
                    if (isUseMediaCodecFrame) {
                        FrameData frameData = infoHelper.extractFrame(i);
                        startTime = frameData.time;
                        String saveFile = PictureUtils.saveImageToSDForEdit(frameData.frameBitmap, saveDir,
                                frameData.time + ".jpeg");
                        frameData.release();
                        FrameThumbInfo thumbInfo = new FrameThumbInfo();
                        thumbInfo.imagePath = saveFile;
                        thumbInfo.startTime = startTime;
                        frameThumbInfos.add(thumbInfo);
                    } else {
                        String framePath = source.createPreFrameImage(i);
                        if (!TextUtils.isEmpty(framePath)) {
                            startTime = i;
                            FrameThumbInfo thumbInfo = new FrameThumbInfo();
                            thumbInfo.imagePath = framePath;
                            thumbInfo.startTime = startTime;
                            frameThumbInfos.add(thumbInfo);
                        } else {
                            Log.e("TAG", "时间 == " + i + " create Frame image === null");
                        }
                    }

                }
                for (int j = 0; j < frameThumbInfos.size() - 1; j++) {
                    FrameThumbInfo thumbInfo1 = frameThumbInfos.get(j);
                    FrameThumbInfo thumbInfo2 = frameThumbInfos.get(j + 1);
                    thumbInfo1.durationTime = thumbInfo2.startTime - thumbInfo1.startTime;
                    thumbInfo1.durationDataChanged();
                }
                FrameThumbInfo endThumbInfo = frameThumbInfos.get(frameThumbInfos.size() - 1);
                endThumbInfo.durationTime = source1.videoDuration - endThumbInfo.startTime;
                endThumbInfo.durationDataChanged();
                source1.frameThumbInfoList = frameThumbInfos;

                source1.setOneTimeMSWidthRatio(calculateOneTimeMSWidth(source1, frameAddIndex));

                source1.setEngineMediaSource(source);
                source1.setPlayTimeRange(source.getPlayRange());
                source1.setTimeLineDuration(source.getDuration());
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (infoHelper != null) {
                    infoHelper.release();
                }
            }
            return source1;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * Note: run child thread
     *
     * @param videoFilePath
     * @return
     */
    public VideoSource createVideoSource(String videoFilePath) {
        VideoSource source = new VideoSource();
        ExtractVideoInfoHelper infoHelper = null;
        try {
            infoHelper = new ExtractVideoInfoHelper(videoFilePath);

            source.sourcePath = videoFilePath;
            source.degree = infoHelper.getVideoDegree();
            source.videoDuration = Long.valueOf(infoHelper.getVideoLength());
            source.mintype = infoHelper.getMimetype();
            source.width = infoHelper.getVideoWidth();
            source.height = infoHelper.getVideoHeight();
            source.sourceDuration = source.videoDuration;

            long frameAddIndex = 3 * 1000;//ms
            long thumbAddIndex = 3 * 1000;//ms

            long lastestTime = 0; //ms

            String nameDir = new File(videoFilePath).getName();
            String saveDir = EditConstants.FRAME_IMAGE_CACHE_DIR;
            File dir = new File(saveDir);
            if (!dir.exists()) {
                dir.mkdirs();
            }
            saveDir += nameDir;

            long startTime = 0;

            ArrayList<FrameThumbInfo> frameThumbInfos = new ArrayList<>();
            source.frameThumbInfoList = frameThumbInfos;
            ArrayList<FrameImageInfo> frameImageInfos = new ArrayList<>();
            for (int i = 0; i < source.sourceDuration; i += frameAddIndex) {
                FrameData frameData = infoHelper.extractFrame(i);
                startTime = frameData.time;
                String saveFile = PictureUtils.saveImageToSDForEdit(frameData.frameBitmap, saveDir,
                        frameData.time + ".jpeg");
                frameData.release();
                frameImageInfos.add(new FrameImageInfo(startTime, saveFile));
            }
            FrameThumbInfo thumbInfo = null;
            int thumbCount = 0;
            for (FrameImageInfo imageInfo : frameImageInfos) {
                lastestTime = thumbInfo != null ? thumbInfo.startTime : 0;
                long curTime = imageInfo.time;
                long dtime = curTime - lastestTime;
                if (dtime >= thumbAddIndex || thumbCount == 0) {
                    if (thumbInfo != null) {
                        thumbInfo.durationTime = dtime;
                        thumbInfo.durationDataChanged();
                    }
                    long startIndex = thumbCount == 0 ? 0 : curTime;
                    thumbInfo = new FrameThumbInfo(startIndex, 0, imageInfo.imagePath);
                    ArrayList<FrameImageInfo> list = new ArrayList<>();
                    thumbInfo.includeFrameInfoList = list;
                    frameThumbInfos.add(thumbInfo);
                    thumbCount++;
                } else {
                    thumbInfo.includeFrameInfoList.add(imageInfo);
                }
            }
            if (thumbInfo != null && thumbInfo.durationTime <= 0) {
                thumbInfo.durationTime = source.sourceDuration - thumbInfo.startTime;
                thumbInfo.durationDataChanged();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (infoHelper != null) {
                infoHelper.release();
            }
        }
        return source;
    }
    public static TimeLineVideo sourceToTimeLineItem(VideoSource sourceData) {
        float perSecondsWidth = sourceData.getOneTimeMSWidthRatio();
        TimeLineVideo videoFrameThumb = new TimeLineVideo(sourceData);
        BaseSelectedObject<VideoSource> sourceSelectedObject = new BaseSelectedObject<>(sourceData);
        videoFrameThumb.setSelectedObject(sourceSelectedObject);
        videoFrameThumb.setTimeLineStartTime(sourceData.timeLineStartTime);
        videoFrameThumb.setTimeLineDuration(sourceData.getTimeLineDuration());
        videoFrameThumb.setPlayerSource(sourceData);
        videoFrameThumb.setOneTimeMSWidthRatio(perSecondsWidth);
        ArrayList<ImageListView.IFrameImage> frameImages = new ArrayList<>();
        videoFrameThumb.setFrameList(frameImages);
        for (int i = 0; i < sourceData.frameThumbInfoList.size(); i++) {
            FrameThumbInfo imageInfo = sourceData.frameThumbInfoList.get(i);
            imageInfo.viewWidth = Math.round(perSecondsWidth * imageInfo.durationTime);
            frameImages.add(imageInfo);
        }
        return videoFrameThumb;
    }
}
