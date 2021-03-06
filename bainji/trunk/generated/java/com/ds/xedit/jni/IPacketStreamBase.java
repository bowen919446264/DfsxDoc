/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class IPacketStreamBase extends IStream {
  private transient long swigCPtr;

  protected IPacketStreamBase(long cPtr, boolean cMemoryOwn) {
    super(xeditJNI.IPacketStreamBase_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(IPacketStreamBase obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_IPacketStreamBase(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public int GetItem(IPacket ppOutItem) {
    return xeditJNI.IPacketStreamBase_GetItem(swigCPtr, this, ppOutItem);
  }

  public void PopItem() {
    xeditJNI.IPacketStreamBase_PopItem(swigCPtr, this);
  }

  public int GetItemCount() {
    return xeditJNI.IPacketStreamBase_GetItemCount(swigCPtr, this);
  }

  public void Reset() {
    xeditJNI.IPacketStreamBase_Reset(swigCPtr, this);
  }

  public void Extend(int count) {
    xeditJNI.IPacketStreamBase_Extend(swigCPtr, this, count);
  }

}
