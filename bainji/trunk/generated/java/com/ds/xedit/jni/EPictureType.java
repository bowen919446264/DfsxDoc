/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EPictureType {
  EPICTURE_TYPE_NONE,
  EPICTURE_TYPE_I,
  EPICTURE_TYPE_P,
  EPICTURE_TYPE_B,
  EPICTURE_TYPE_S,
  EPICTURE_TYPE_SI,
  EPICTURE_TYPE_SP,
  EPICTURE_TYPE_BI;

  public final int swigValue() {
    return swigValue;
  }

  public static EPictureType swigToEnum(int swigValue) {
    EPictureType[] swigValues = EPictureType.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EPictureType swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EPictureType.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EPictureType() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EPictureType(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EPictureType(EPictureType swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}
