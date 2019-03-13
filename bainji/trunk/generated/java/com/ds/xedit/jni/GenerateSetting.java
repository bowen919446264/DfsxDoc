/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class GenerateSetting {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected GenerateSetting(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(GenerateSetting obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_GenerateSetting(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setStrDestDir(String value) {
    xeditJNI.GenerateSetting_strDestDir_set(swigCPtr, this, value);
  }

  public String getStrDestDir() {
    return xeditJNI.GenerateSetting_strDestDir_get(swigCPtr, this);
  }

  public void setStrDestName(String value) {
    xeditJNI.GenerateSetting_strDestName_set(swigCPtr, this, value);
  }

  public String getStrDestName() {
    return xeditJNI.GenerateSetting_strDestName_get(swigCPtr, this);
  }

  public void setEncodeParam(EncodeParam value) {
    xeditJNI.GenerateSetting_encodeParam_set(swigCPtr, this, EncodeParam.getCPtr(value), value);
  }

  public EncodeParam getEncodeParam() {
    long cPtr = xeditJNI.GenerateSetting_encodeParam_get(swigCPtr, this);
    return (cPtr == 0) ? null : new EncodeParam(cPtr, false);
  }

  public void setRStartTime(Rational value) {
    xeditJNI.GenerateSetting_rStartTime_set(swigCPtr, this, Rational.getCPtr(value), value);
  }

  public Rational getRStartTime() {
    long cPtr = xeditJNI.GenerateSetting_rStartTime_get(swigCPtr, this);
    return (cPtr == 0) ? null : new Rational(cPtr, false);
  }

  public void setRDuration(Rational value) {
    xeditJNI.GenerateSetting_rDuration_set(swigCPtr, this, Rational.getCPtr(value), value);
  }

  public Rational getRDuration() {
    long cPtr = xeditJNI.GenerateSetting_rDuration_get(swigCPtr, this);
    return (cPtr == 0) ? null : new Rational(cPtr, false);
  }

  public GenerateSetting() {
    this(xeditJNI.new_GenerateSetting(), true);
  }

}