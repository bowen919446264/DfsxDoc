/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public enum EMuxerType {
  EMUXER_TYPE_NONE,
  EMUXER_TYPE_AAC,
  EMUXER_TYPE_AC3,
  EMUXER_TYPE_EAC3,
  EMUXER_TYPE_AIFF,
  EMUXER_TYPE_AMR,
  EMUXER_TYPE_APE,
  EMUXER_TYPE_ASF,
  EMUXER_TYPE_AVI,
  EMUXER_TYPE_AVS,
  EMUXER_TYPE_DSHOW,
  EMUXER_TYPE_DV,
  EMUXER_TYPE_DV1394,
  EMUXER_TYPE_FLV,
  EMUXER_TYPE_GXF,
  EMUXER_TYPE_MOV,
  EMUXER_TYPE_MP3,
  EMUXER_TYPE_MPEGVIDEO,
  EMUXER_TYPE_MPEGPS,
  EMUXER_TYPE_MPEGTS,
  EMUXER_TYPE_MPEGTS_RAW,
  EMUXER_TYPE_MXF,
  EMUXER_TYPE_PCM,
  EMUXER_TYPE_WAV,
  EMUXER_TYPE_UNKONWN(0x10000);

  public final int swigValue() {
    return swigValue;
  }

  public static EMuxerType swigToEnum(int swigValue) {
    EMuxerType[] swigValues = EMuxerType.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (EMuxerType swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + EMuxerType.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private EMuxerType() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private EMuxerType(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private EMuxerType(EMuxerType swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

