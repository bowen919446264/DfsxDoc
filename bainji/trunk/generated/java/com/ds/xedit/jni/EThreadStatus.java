/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EThreadStatus {
  ETHREAD_STATUS_NOT_START,
  ETHREAD_STATUS_RUNNING,
  ETHREAD_STATUS_SUSPENDED,
  ETHREAD_STATUS_STOPPED;

  public final int swigValue() {
    return swigValue;
  }

  public static EThreadStatus swigToEnum(int swigValue) {
    EThreadStatus[] swigValues = EThreadStatus.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EThreadStatus swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EThreadStatus.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EThreadStatus() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EThreadStatus(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EThreadStatus(EThreadStatus swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

