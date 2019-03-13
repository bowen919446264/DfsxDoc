/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EClipType {
  EClipType_Invalid,
  EClipType_Video,
  EClipType_Audio,
  EClipType_Image,
  EClipType_SVG;

  public final int swigValue() {
    return swigValue;
  }

  public static EClipType swigToEnum(int swigValue) {
    EClipType[] swigValues = EClipType.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EClipType swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EClipType.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EClipType() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EClipType(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EClipType(EClipType swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

