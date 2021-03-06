/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class ILogReceiver {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected ILogReceiver(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ILogReceiver obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_ILogReceiver(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() {
    swigCMemOwn = false;
    xeditJNI.ILogReceiver_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    xeditJNI.ILogReceiver_change_ownership(this, swigCPtr, true);
  }

        /** This constructor creates the proxy which initially does not create nor own any C memory */
        public ILogReceiver() {
            this(0, false);
        }
    
  public void Receive(ELogLevel eLevel, String pStrLog) {
    xeditJNI.ILogReceiver_Receive(swigCPtr, this, eLevel.swigValue(), pStrLog);
  }

}
