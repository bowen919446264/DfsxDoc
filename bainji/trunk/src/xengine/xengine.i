%module(directors="1") xengine

%include <swiginterface.i>
%include <typemaps.i>
%include <arrays_java.i>
%include "../xutil/xutil.i"
%include "swig/pointers.i"
%include "cpointer.i"

%{
#include "xengine/EncodeParam.h"
#include "xengine/GenerateSetting.h"
#include "xengine/IGenerateObserver.h"
#include "xengine/PreviewFrame.h"
#include "xengine/IPreview.h"
#include "xengine/IClip.h"
#include "xengine/IMedia.h"
#include "xengine/ITrackClipInfo.h"
#include "xengine/ITrack.h"
#include "xengine/IProject.h"
#include "xengine/IAction.h"
#include "xengine/ITimeLineObserver.h"
#include "xengine/ITimeLine.h"
#include "xengine/IXEngine.h"
%}

%pointer_class(ID, IDPtr);

%exception xedit::IAVMedia::dynamic_cast(xedit::IMedia *pMedia) {
  $action
    if (!result) {
      jclass excep = jenv->FindClass("java/lang/ClassCastException");
      if (excep) {
        jenv->ThrowNew(excep, "dynamic_cast exception");
      }
    }
}
%extend xedit::IAVMedia {
  static xedit::IAVMedia *dynamic_cast(xedit::IMedia *pMedia) {
    return dynamic_cast<xedit::IAVMedia *>(pMedia);
  }
}

%exception xedit::IAVClip::dynamic_cast(xedit::IClip *pClip) {
  $action
    if (!result) {
      jclass excep = jenv->FindClass("java/lang/ClassCastException");
      if (excep) {
        jenv->ThrowNew(excep, "dynamic_cast exception");
      }
    }
}
%extend xedit::IAVClip {
  static xedit::IAVClip *dynamic_cast(xedit::IClip *pClip) {
    return dynamic_cast<xedit::IAVClip *>(pClip);
  }
}


JAVA_OBJECTOF2POINTER(xedit, IClip, com/ds/xedit/jni)


%feature("director") xedit::IGenerateObserver;
%feature("nodirector") xedit::IGenerateObserver::~IGenerateObserver;

%feature("director") xedit::ITimeLineObserver;
%feature("nodirector") xedit::ITimeLineObserver::~ITimeLineObserver;

%feature("director") xedit::IMedia;
%feature("nodirector") xedit::IMedia::~IMedia;

%feature("director") xedit::IClip;
%feature("nodirector") xedit::IClip::~IClip;

// 打开会导致apk运行报错：java.lang.NoSuchMethodError: no static method
//%interface_impl(xedit::IRenderer)
//%interface_impl(xedit::IAudioRenderer)
//%interface_impl(xedit::IVideoRenderer)
//%interface_impl(xedit::IGenerateobserver)
//%interface_impl(xedit::ITimeLineObserver)

%include "xengine/EncodeParam.h"
%include "xengine/GenerateSetting.h"
%include "xengine/IGenerateObserver.h"
%include "xengine/PreviewFrame.h"
%include "xengine/IPreview.h"
%include "xengine/IClip.h"
%include "xengine/IMedia.h"
%include "xengine/ITrackClipInfo.h"
%include "xengine/ITrack.h"
%include "xengine/IProject.h"
%include "xengine/IAction.h"
%include "xengine/ITimeLineObserver.h"
%include "xengine/ITimeLine.h"
%include "xengine/IXEngine.h"
