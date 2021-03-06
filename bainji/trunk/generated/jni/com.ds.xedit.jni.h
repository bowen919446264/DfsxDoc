/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_xedit_WRAP_H_
#define SWIG_xedit_WRAP_H_

class SwigDirector_ILogReceiver : public libav::ILogReceiver, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_ILogReceiver(JNIEnv *jenv);
    virtual void Receive(libav::ELogLevel eLevel, char const *pStrLog);
public:
    bool swig_overrides(int n) {
      return (n < 1 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<1> swig_override;
};

class SwigDirector_IBuffer : public xedit::IBuffer, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IBuffer(JNIEnv *jenv);
    virtual xedit::EBufferType getType() const;
public:
    bool swig_overrides(int n) {
      return (n < 1 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<1> swig_override;
};

class SwigDirector_IVideoBuffer : public xedit::IVideoBuffer, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IVideoBuffer(JNIEnv *jenv);
    virtual xedit::EBufferType getType() const;
    virtual xedit::GSize getVideoRect() const;
    virtual libav::EPixFormat getPixelFormat() const;
    virtual xedit::EBufferLocation getLocation() const;
    virtual int getPlaneCount() const;
    virtual uint8_t *getPlanePointer(int nPlane) const;
    virtual int getPlaneLineSize(int nPlane) const;
    virtual int getSize(int nPlane) const;
public:
    bool swig_overrides(int n) {
      return (n < 8 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<8> swig_override;
};

class SwigDirector_IRenderer : public xedit::IRenderer, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IRenderer(JNIEnv *jenv);
    virtual StatusCode render(xedit::IBuffer *pBuffer);
public:
    bool swig_overrides(int n) {
      return (n < 1 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<1> swig_override;
};

class SwigDirector_IVideoRenderer : public xedit::IVideoRenderer, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IVideoRenderer(JNIEnv *jenv);
    virtual StatusCode render(xedit::IBuffer *pBuffer);
    virtual StatusCode init(int nWidth, int nHeight, libav::EPixFormat ePixFormat);
public:
    bool swig_overrides(int n) {
      return (n < 2 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<2> swig_override;
};

class SwigDirector_IAudioRenderer : public xedit::IAudioRenderer, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IAudioRenderer(JNIEnv *jenv);
    virtual StatusCode render(xedit::IBuffer *pBuffer);
    virtual StatusCode init(int nChannel, int nSampleRate, int nBitsPerSample, libav::ESampleFormat eSampleFormat);
public:
    bool swig_overrides(int n) {
      return (n < 2 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<2> swig_override;
};

class SwigDirector_IInputStream : public xedit::IInputStream, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IInputStream(JNIEnv *jenv);
    virtual int read(uint8_t *buffer, int count);
    virtual int64_t seek(int64_t pos);
    virtual int64_t available();
    virtual int64_t skip(int64_t n);
    virtual StatusCode close();
public:
    bool swig_overrides(int n) {
      return (n < 5 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<5> swig_override;
};

class SwigDirector_IObserver : public xedit::IObserver, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IObserver(JNIEnv *jenv);
public:
    bool swig_overrides(int n) {
      return false;
    }
};

class SwigDirector_IObservable : public xedit::IObservable, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IObservable(JNIEnv *jenv);
    virtual void addObserver(xedit::IObserver *pObserver);
    virtual void deleteObserver(xedit::IObserver *pObserver);
    virtual void deleteAllObservers();
    virtual int getObserverCount();
    virtual xedit::IObserver *getObserver(int index);
    virtual void notifyAllObservers();
public:
    bool swig_overrides(int n) {
      return (n < 6 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<6> swig_override;
};

class SwigDirector_IOutputStream : public xedit::IOutputStream, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IOutputStream(JNIEnv *jenv);
    virtual int write(uint8_t const *buffer, int count);
    virtual StatusCode flush();
    virtual StatusCode close();
    virtual int64_t seek(int64_t pos);
public:
    bool swig_overrides(int n) {
      return (n < 4 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<4> swig_override;
};

class SwigDirector_IGenerateObserver : public xedit::IGenerateObserver, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IGenerateObserver(JNIEnv *jenv);
    virtual void onFinish(xedit::GenerateSetting const &param, StatusCode code);
    virtual void onUpdateProcess(xedit::GenerateSetting const &param, libav::Rational rDuration);
public:
    bool swig_overrides(int n) {
      return (n < 2 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<2> swig_override;
};

class SwigDirector_IClip : public xedit::IClip, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IClip(JNIEnv *jenv);
    virtual int64_t getId() const;
    virtual xedit::EClipType getType() const;
    virtual int64_t getRefMediaId() const;
    virtual libav::Rational getOffsetOnTrack() const;
    virtual libav::Rational getDuration() const;
    virtual libav::Rational getOffsetInMedia() const;
public:
    bool swig_overrides(int n) {
      return (n < 6 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<6> swig_override;
};

class SwigDirector_IMedia : public xedit::IMedia, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_IMedia(JNIEnv *jenv);
    virtual int64_t getId() const;
    virtual char const *getPath() const;
    virtual xedit::EMediaType getMediaType() const;
    virtual xedit::IClip *newClip() const;
    virtual xedit::IClip *newClip(libav::Rational rDuration) const;
    virtual xedit::IPreview *getPreview() const;
public:
    bool swig_overrides(int n) {
      return (n < 6 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<6> swig_override;
};

class SwigDirector_ITimeLineObserver : public xedit::ITimeLineObserver, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_ITimeLineObserver(JNIEnv *jenv);
    virtual void onPosDidChanged(libav::Rational rNewPos);
    virtual void onTrackCreated(xedit::ITrack *pTrack);
    virtual void onTrackRemoved(int64_t trackId);
    virtual void onTimeLineStatusChanged(xedit::ETimeLineStatus newStatus);
public:
    bool swig_overrides(int n) {
      return (n < 4 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<4> swig_override;
};


#endif
