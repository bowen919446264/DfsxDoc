/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class ITimeLineObserver {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected ITimeLineObserver(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ITimeLineObserver obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_ITimeLineObserver(swigCPtr);
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
    xeditJNI.ITimeLineObserver_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    xeditJNI.ITimeLineObserver_change_ownership(this, swigCPtr, true);
  }

  public void onPosDidChanged(Rational rNewPos) {
    xeditJNI.ITimeLineObserver_onPosDidChanged(swigCPtr, this, Rational.getCPtr(rNewPos), rNewPos);
  }

  public void onTrackCreated(ITrack pTrack) {
    xeditJNI.ITimeLineObserver_onTrackCreated(swigCPtr, this, ITrack.getCPtr(pTrack), pTrack);
  }

  public void onTrackRemoved(long trackId) {
    xeditJNI.ITimeLineObserver_onTrackRemoved(swigCPtr, this, trackId);
  }

  public void onTimeLineStatusChanged(ETimeLineStatus newStatus) {
    xeditJNI.ITimeLineObserver_onTimeLineStatusChanged(swigCPtr, this, newStatus.swigValue());
  }

  public ITimeLineObserver() {
    this(xeditJNI.new_ITimeLineObserver(), true);
    xeditJNI.ITimeLineObserver_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

}
