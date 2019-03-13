package com.dfsx.editengine.bean;

import com.ds.xedit.jni.AudioCodecParam;
import com.ds.xedit.jni.EBitrateMode;
import com.ds.xedit.jni.ECodecID;
import com.ds.xedit.jni.ECodecLevel;
import com.ds.xedit.jni.ECodecProfile;
import com.ds.xedit.jni.EMuxerType;
import com.ds.xedit.jni.EPixFormat;
import com.ds.xedit.jni.ESampleFormat;
import com.ds.xedit.jni.EncodeParam;
import com.ds.xedit.jni.VideoCodecParam;

public class GenerateConfig {
    //Video
    private boolean isInterlaced;
    private boolean isTopFieldFirst;
    private int width;
    private int height;
    private int bitrate;

    //Audio
    private int bitsPerSample;
    private int channels;
    private int sampleRate;
    private int audioBitrate;

    protected GenerateConfig() {

    }

    public boolean isInterlaced() {
        return isInterlaced;
    }

    private void setInterlaced(boolean interlaced) {
        isInterlaced = interlaced;
    }

    public boolean isTopFieldFirst() {
        return isTopFieldFirst;
    }

    private void setTopFieldFirst(boolean topFieldFirst) {
        isTopFieldFirst = topFieldFirst;
    }

    public int getWidth() {
        return width;
    }

    private void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    private void setHeight(int height) {
        this.height = height;
    }

    public int getBitrate() {
        return bitrate;
    }

    private void setBitrate(int bitrate) {
        this.bitrate = bitrate;
    }

    public int getBitsPerSample() {
        return bitsPerSample;
    }

    private void setBitsPerSample(int bitsPerSample) {
        this.bitsPerSample = bitsPerSample;
    }

    public int getChannels() {
        return channels;
    }

    private void setChannels(int channels) {
        this.channels = channels;
    }

    public int getSampleRate() {
        return sampleRate;
    }

    private void setSampleRate(int sampleRate) {
        this.sampleRate = sampleRate;
    }

    public int getAudioBitrate() {
        return audioBitrate;
    }

    private void setAudioBitrate(int audioBitrate) {
        this.audioBitrate = audioBitrate;
    }

    public EncodeParam getEngineEncodeParams() {
        EncodeParam encodeParam = new EncodeParam();
        encodeParam.setEMuxerType(EMuxerType.EMUXER_TYPE_MOV);

        VideoCodecParam videoCodecParam = new VideoCodecParam();
        videoCodecParam.setBInterlaced(isInterlaced());
        videoCodecParam.setBTopFieldFirst(isTopFieldFirst());
        videoCodecParam.setEPixFormat(EPixFormat.EPIX_FMT_NV12);
        videoCodecParam.setNWidth(getWidth());
        videoCodecParam.setNHeight(getHeight());
        videoCodecParam.setEBitrateMode(EBitrateMode.EBITRATE_MODE_VBR);
        videoCodecParam.setECodecID(ECodecID.ECODEC_ID_H264);
        videoCodecParam.setECodecLevel(ECodecLevel.ECODEC_LEVEL_NONE);
        videoCodecParam.setECodecProfile(ECodecProfile.ECODEC_PROFILE_NONE);
        videoCodecParam.setNBitrate(getBitrate());

        encodeParam.setVideoParam(videoCodecParam);

        AudioCodecParam audioCodecParam = new AudioCodecParam();
        audioCodecParam.setESampleFmt(ESampleFormat.EAV_SAMPLE_FMT_S16);
        audioCodecParam.setNBitsPerSample(getBitsPerSample());
        audioCodecParam.setNChannels(getChannels());
        audioCodecParam.setNSampleRate(getSampleRate());
        audioCodecParam.setEBitrateMode(EBitrateMode.EBITRATE_MODE_VBR);
        audioCodecParam.setECodecID(ECodecID.ECODEC_ID_MP3);
        audioCodecParam.setECodecLevel(ECodecLevel.ECODEC_LEVEL_NONE);
        audioCodecParam.setECodecProfile(ECodecProfile.ECODEC_PROFILE_NONE);
        audioCodecParam.setNBitrate(getAudioBitrate());

        encodeParam.setAudioParam(audioCodecParam);

        return encodeParam;
    }


    public static class Builder {

        private GenerateConfig config;

        public Builder() {
            config = new GenerateConfig();
        }

        public Builder setInterlaced(boolean interlaced) {
            config.setInterlaced(interlaced);
            return this;
        }

        public Builder setTopFieldFirst(boolean topFieldFirst) {
            config.setTopFieldFirst(topFieldFirst);
            return this;
        }

        public Builder setWidth(int width) {
            config.setWidth(width);
            return this;
        }

        public Builder setHeight(int height) {
            config.setHeight(height);
            return this;
        }

        public Builder setBitrate(int bitrate) {
            config.setBitrate(bitrate);
            return this;
        }


        public Builder setBitsPerSample(int bitsPerSample) {
            config.setBitsPerSample(bitsPerSample);
            return this;
        }

        public Builder setChannels(int channels) {
            config.setChannels(channels);
            return this;
        }

        public Builder setSampleRate(int sampleRate) {
            config.setSampleRate(sampleRate);
            return this;
        }

        public Builder setAudioBitrate(int audioBitrate) {
            config.setAudioBitrate(audioBitrate);
            return this;
        }

        public GenerateConfig build() {
            return config;
        }
    }

    public static class DefaultBuild extends Builder {
        public DefaultBuild() {
            super();
            setInterlaced(false);
            setTopFieldFirst(false);
            setWidth(1280);
            setHeight(720);
            setBitrate(8 * 1000000);

            setBitsPerSample(16);
            setChannels(2);
            setSampleRate(48000);
            setAudioBitrate(128000);
        }
    }
}
