/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EThreadPriority {
  ThreadPriority_Low,
  ThreadPriority_Normal,
  ThreadPriority_High,
  ThreadPriority_Default(ThreadPriority_Normal);

  public final int swigValue() {
    return swigValue;
  }

  public static EThreadPriority swigToEnum(int swigValue) {
    EThreadPriority[] swigValues = EThreadPriority.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EThreadPriority swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EThreadPriority.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EThreadPriority() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EThreadPriority(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EThreadPriority(EThreadPriority swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

