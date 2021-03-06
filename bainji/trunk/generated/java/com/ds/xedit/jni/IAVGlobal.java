/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class IAVGlobal {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected IAVGlobal(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(IAVGlobal obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_IAVGlobal(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void AVSetLanguage(ELanguage eLang) {
    xeditJNI.IAVGlobal_AVSetLanguage(swigCPtr, this, eLang.swigValue());
  }

  public int AVCodecGetCount() {
    return xeditJNI.IAVGlobal_AVCodecGetCount(swigCPtr, this);
  }

  public int AVCodecEnumerate(int[] pOutArray, int nArraySize, SWIGTYPE_p_int pOutCount) {
    return xeditJNI.IAVGlobal_AVCodecEnumerate(swigCPtr, this, pOutArray, nArraySize, SWIGTYPE_p_int.getCPtr(pOutCount));
  }

  public int AVCodecGetDescription(ECodecID eCodec, Description pOutDesc) {
    return xeditJNI.IAVGlobal_AVCodecGetDescription(swigCPtr, this, eCodec.swigValue(), Description.getCPtr(pOutDesc), pOutDesc);
  }

  public int AVMuxerGetCount() {
    return xeditJNI.IAVGlobal_AVMuxerGetCount(swigCPtr, this);
  }

  public int AVMuxerEnumerate(int[] pOutArray, int nArraySize, SWIGTYPE_p_int pOutCount) {
    return xeditJNI.IAVGlobal_AVMuxerEnumerate(swigCPtr, this, pOutArray, nArraySize, SWIGTYPE_p_int.getCPtr(pOutCount));
  }

  public int AVMuxerGetDescription(EMuxerType eMuxer, Description pOutDesc) {
    return xeditJNI.IAVGlobal_AVMuxerGetDescription(swigCPtr, this, eMuxer.swigValue(), Description.getCPtr(pOutDesc), pOutDesc);
  }

  public int AVPixelFormatGetCount() {
    return xeditJNI.IAVGlobal_AVPixelFormatGetCount(swigCPtr, this);
  }

  public int AVPixelFormatEnumerate(int[] pOutArray, int nArraySize, SWIGTYPE_p_int pOutCount) {
    return xeditJNI.IAVGlobal_AVPixelFormatEnumerate(swigCPtr, this, pOutArray, nArraySize, SWIGTYPE_p_int.getCPtr(pOutCount));
  }

  public int AVPixelFormatGetDescription(EPixFormat ePixFmt, Description pOutDesc) {
    return xeditJNI.IAVGlobal_AVPixelFormatGetDescription(swigCPtr, this, ePixFmt.swigValue(), Description.getCPtr(pOutDesc), pOutDesc);
  }

  public int AVBitrateModeGetCount() {
    return xeditJNI.IAVGlobal_AVBitrateModeGetCount(swigCPtr, this);
  }

  public int AVBitrateModeEnumerate(int[] pOutArray, int nArraySize, SWIGTYPE_p_int pOutCount) {
    return xeditJNI.IAVGlobal_AVBitrateModeEnumerate(swigCPtr, this, pOutArray, nArraySize, SWIGTYPE_p_int.getCPtr(pOutCount));
  }

  public int AVBitrateModeGetDescription(EBitrateMode eMode, Description pOutDesc) {
    return xeditJNI.IAVGlobal_AVBitrateModeGetDescription(swigCPtr, this, eMode.swigValue(), Description.getCPtr(pOutDesc), pOutDesc);
  }

  public int AVStandardGetCount() {
    return xeditJNI.IAVGlobal_AVStandardGetCount(swigCPtr, this);
  }

  public int AVStandardEnumerate(int[] pOutArray, int nArraySize, SWIGTYPE_p_int pOutCount) {
    return xeditJNI.IAVGlobal_AVStandardEnumerate(swigCPtr, this, pOutArray, nArraySize, SWIGTYPE_p_int.getCPtr(pOutCount));
  }

  public int AVStandardGetDescription(EStandard eStandard, Description pOutDesc) {
    return xeditJNI.IAVGlobal_AVStandardGetDescription(swigCPtr, this, eStandard.swigValue(), Description.getCPtr(pOutDesc), pOutDesc);
  }

  public int AVSampleFormatGetCount() {
    return xeditJNI.IAVGlobal_AVSampleFormatGetCount(swigCPtr, this);
  }

  public int AVSampleFormatEnumerate(int[] pOutArray, int nArraySize, SWIGTYPE_p_int pOutCount) {
    return xeditJNI.IAVGlobal_AVSampleFormatEnumerate(swigCPtr, this, pOutArray, nArraySize, SWIGTYPE_p_int.getCPtr(pOutCount));
  }

  public int AVSampleFormatGetDescription(ESampleFormat eSampleFmt, Description pOutDesc) {
    return xeditJNI.IAVGlobal_AVSampleFormatGetDescription(swigCPtr, this, eSampleFmt.swigValue(), Description.getCPtr(pOutDesc), pOutDesc);
  }

}
