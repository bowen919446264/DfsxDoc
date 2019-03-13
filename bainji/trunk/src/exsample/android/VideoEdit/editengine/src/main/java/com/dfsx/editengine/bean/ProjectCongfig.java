package com.dfsx.editengine.bean;

import com.ds.xedit.jni.EPixFormat;
import com.ds.xedit.jni.ESampleFormat;
import com.ds.xedit.jni.ProjectSetting;
import com.ds.xedit.jni.Rational;

/**
 * 项目引擎的配置文件
 */
public class ProjectCongfig {
    private EPixFormat pixFormat;
    private ESampleFormat sampleFormat;
    private int sampleRate;
    private int bitsPerSample;
    private int channelCount;
    private int width;
    private int height;
    private int frameRate;
    private boolean interlaced;
    private boolean topFieldFirst;
    private Rational aspectRatio;

    protected ProjectCongfig() {
    }

    public EPixFormat getPixFormat() {
        return pixFormat;
    }

    public void setPixFormat(EPixFormat pixFormat) {
        this.pixFormat = pixFormat;
    }

    public ESampleFormat getSampleFormat() {
        return sampleFormat;
    }

    public void setSampleFormat(ESampleFormat sampleFormat) {
        this.sampleFormat = sampleFormat;
    }

    public int getSampleRate() {
        return sampleRate;
    }

    public void setSampleRate(int sampleRate) {
        this.sampleRate = sampleRate;
    }

    public int getBitsPerSample() {
        return bitsPerSample;
    }

    public void setBitsPerSample(int bitsPerSample) {
        this.bitsPerSample = bitsPerSample;
    }

    public int getChannelCount() {
        return channelCount;
    }

    public void setChannelCount(int channelCount) {
        this.channelCount = channelCount;
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getFrameRate() {
        return frameRate;
    }

    public void setFrameRate(int frameRate) {
        this.frameRate = frameRate;
    }

    public boolean isInterlaced() {
        return interlaced;
    }

    public void setInterlaced(boolean interlaced) {
        this.interlaced = interlaced;
    }

    public boolean isTopFieldFirst() {
        return topFieldFirst;
    }

    public void setTopFieldFirst(boolean topFieldFirst) {
        this.topFieldFirst = topFieldFirst;
    }

    public Rational getAspectRatio() {
        return aspectRatio;
    }

    public void setAspectRatio(Rational aspectRatio) {
        this.aspectRatio = aspectRatio;
    }

    public ProjectSetting getXEditSettings() {
        ProjectSetting setting = new ProjectSetting();
        setting.setEPixFormat(getPixFormat());
        setting.setESampleFormat(getSampleFormat());
        setting.setNSampleRate(getSampleRate());
        setting.setNBitsPerSample(getBitsPerSample());
        setting.setNChannelCount(getChannelCount());
        setting.setNHeight(getHeight());
        setting.setNWidth(getWidth());
        setting.setRFrameRate(new Rational(getFrameRate(), 1));
        setting.setBInterlaced(isInterlaced());
        setting.setBTopFieldFirst(isTopFieldFirst());
        setting.setRAspectRatio(getAspectRatio());
        return setting;
    }

    public static class Builder {

        private ProjectCongfig congfig;

        public Builder() {
            congfig = new ProjectCongfig();
        }

        public void setPixFormat(EPixFormat pixFormat) {
            congfig.setPixFormat(pixFormat);
        }

        public void setSampleFormat(ESampleFormat sampleFormat) {
            congfig.setSampleFormat(sampleFormat);
        }

        public void setSampleRate(int sampleRate) {
            congfig.setSampleRate(sampleRate);
        }

        public void setBitsPerSample(int bitsPerSample) {
            congfig.setBitsPerSample(bitsPerSample);
        }

        public void setChannelCount(int channelCount) {
            congfig.setChannelCount(channelCount);
        }

        public void setWidth(int width) {
            congfig.setWidth(width);
        }

        public void setHeight(int height) {
            congfig.setHeight(height);
        }

        public void setFrameRate(int frameRate) {
            congfig.setFrameRate(frameRate);
        }

        public void setInterlaced(boolean interlaced) {
            congfig.setInterlaced(interlaced);
        }

        public void setTopFieldFirst(boolean topFieldFirst) {
            congfig.setTopFieldFirst(topFieldFirst);
        }

        /**
         * num1 / num2
         *
         * @param num1
         * @param num2
         */
        public void setAspectRatio(long num1, long num2) {
            congfig.setAspectRatio(new Rational(num1, num2));
        }

        public ProjectCongfig build() {
            return congfig;
        }
    }

    public static class DefaultConfigBuilder extends Builder {
        public DefaultConfigBuilder() {
            super();
            setPixFormat(EPixFormat.EPIX_FMT_NV12);
            setSampleFormat(ESampleFormat.EAV_SAMPLE_FMT_S16);
            setSampleRate(48000);
            setBitsPerSample(16);
            setChannelCount(2);
            setWidth(1920);
            setHeight(1080);
            setFrameRate(25);
            setInterlaced(false);
            setTopFieldFirst(false);
            setAspectRatio(16, 9);
        }
    }
}
