/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EActionType {
  AT_Unknown,
  AT_AddMedia,
  AT_RemoveMedia,
  AT_AddClip,
  AT_RemoveClip,
  AT_MoveClip,
  AT_ChangeClipOffsetOnTrack,
  AT_ChangeClipDuration,
  AT_ChangeClipOffsetInMedia,
  AT_GroupAction;

  public final int swigValue() {
    return swigValue;
  }

  public static EActionType swigToEnum(int swigValue) {
    EActionType[] swigValues = EActionType.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EActionType swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EActionType.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EActionType() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EActionType(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EActionType(EActionType swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

