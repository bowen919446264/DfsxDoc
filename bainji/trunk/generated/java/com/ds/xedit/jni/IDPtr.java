/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class IDPtr {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected IDPtr(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(IDPtr obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_IDPtr(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public IDPtr() {
    this(xeditJNI.new_IDPtr(), true);
  }

  public void assign(long value) {
    xeditJNI.IDPtr_assign(swigCPtr, this, value);
  }

  public long value() {
    return xeditJNI.IDPtr_value(swigCPtr, this);
  }

  public SWIGTYPE_p_long_long cast() {
    long cPtr = xeditJNI.IDPtr_cast(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_long_long(cPtr, false);
  }

  public static IDPtr frompointer(SWIGTYPE_p_long_long t) {
    long cPtr = xeditJNI.IDPtr_frompointer(SWIGTYPE_p_long_long.getCPtr(t));
    return (cPtr == 0) ? null : new IDPtr(cPtr, false);
  }

}
