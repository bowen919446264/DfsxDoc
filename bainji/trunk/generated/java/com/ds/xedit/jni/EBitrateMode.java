/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EBitrateMode {
  EBITRATE_MODE_NONE,
  EBITRATE_MODE_CBR,
  EBITRATE_MODE_VBR;

  public final int swigValue() {
    return swigValue;
  }

  public static EBitrateMode swigToEnum(int swigValue) {
    EBitrateMode[] swigValues = EBitrateMode.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EBitrateMode swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EBitrateMode.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EBitrateMode() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EBitrateMode(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EBitrateMode(EBitrateMode swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

