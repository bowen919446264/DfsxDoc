/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class EngineSetting {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected EngineSetting(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(EngineSetting obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_EngineSetting(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setCacheDir(String value) {
    xeditJNI.EngineSetting_cacheDir_set(swigCPtr, this, value);
  }

  public String getCacheDir() {
    return xeditJNI.EngineSetting_cacheDir_get(swigCPtr, this);
  }

  public void setLogDir(String value) {
    xeditJNI.EngineSetting_logDir_set(swigCPtr, this, value);
  }

  public String getLogDir() {
    return xeditJNI.EngineSetting_logDir_get(swigCPtr, this);
  }

  public void setPreviewFrameSize(GSize value) {
    xeditJNI.EngineSetting_previewFrameSize_set(swigCPtr, this, GSize.getCPtr(value), value);
  }

  public GSize getPreviewFrameSize() {
    long cPtr = xeditJNI.EngineSetting_previewFrameSize_get(swigCPtr, this);
    return (cPtr == 0) ? null : new GSize(cPtr, false);
  }

  public void setUseGpuToDecode(boolean value) {
    xeditJNI.EngineSetting_useGpuToDecode_set(swigCPtr, this, value);
  }

  public boolean getUseGpuToDecode() {
    return xeditJNI.EngineSetting_useGpuToDecode_get(swigCPtr, this);
  }

  public EngineSetting() {
    this(xeditJNI.new_EngineSetting(), true);
  }

}
