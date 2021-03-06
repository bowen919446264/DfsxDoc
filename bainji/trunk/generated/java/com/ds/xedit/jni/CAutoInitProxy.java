/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class CAutoInitProxy {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected CAutoInitProxy(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(CAutoInitProxy obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_CAutoInitProxy(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public CAutoInitProxy(SWIGTYPE_p_f___void initFunc, SWIGTYPE_p_f___void deinitFunc) {
    this(xeditJNI.new_CAutoInitProxy__SWIG_0(SWIGTYPE_p_f___void.getCPtr(initFunc), SWIGTYPE_p_f___void.getCPtr(deinitFunc)), true);
  }

  public CAutoInitProxy(SWIGTYPE_p_f___void initFunc) {
    this(xeditJNI.new_CAutoInitProxy__SWIG_1(SWIGTYPE_p_f___void.getCPtr(initFunc)), true);
  }

}
