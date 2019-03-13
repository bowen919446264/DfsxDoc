/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class IThread {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected IThread(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(IThread obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_IThread(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public int TStart() {
    return xeditJNI.IThread_TStart(swigCPtr, this);
  }

  public int TStop() {
    return xeditJNI.IThread_TStop(swigCPtr, this);
  }

  public int TSuspend() {
    return xeditJNI.IThread_TSuspend(swigCPtr, this);
  }

  public int TResume() {
    return xeditJNI.IThread_TResume(swigCPtr, this);
  }

  public EThreadStatus GetThreadStatus() {
    return EThreadStatus.swigToEnum(xeditJNI.IThread_GetThreadStatus(swigCPtr, this));
  }

}
