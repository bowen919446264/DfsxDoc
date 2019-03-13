/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class ICondition {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected ICondition(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ICondition obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_ICondition(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

        /** This constructor creates the proxy which initially does not create nor own any C memory */
        public ICondition() {
            this(0, false);
        }
    
  public int Signal() {
    return xeditJNI.ICondition_Signal(swigCPtr, this);
  }

  public int Wait() {
    return xeditJNI.ICondition_Wait(swigCPtr, this);
  }

  public int WaitTimeout(int nTimeoutMS) {
    return xeditJNI.ICondition_WaitTimeout(swigCPtr, this, nTimeoutMS);
  }

}
