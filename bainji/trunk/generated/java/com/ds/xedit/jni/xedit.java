/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class xedit implements xeditConstants {
  public static String AVSampleFormatToString(ESampleFormat eSampleFormat) {
    return xeditJNI.AVSampleFormatToString(eSampleFormat.swigValue());
  }

  public static ESampleFormat AVSampleFormatFromString(String fmtStr) {
    return ESampleFormat.swigToEnum(xeditJNI.AVSampleFormatFromString(fmtStr));
  }

  public static Rational AVMakeRational(long nNum, long nDen) {
    return new Rational(xeditJNI.AVMakeRational(nNum, nDen), true);
  }

  public static Rational AVRationalMultiply(Rational rA, Rational rB) {
    return new Rational(xeditJNI.AVRationalMultiply(Rational.getCPtr(rA), rA, Rational.getCPtr(rB), rB), true);
  }

  public static Rational AVRationalDivision(Rational rA, Rational rB) {
    return new Rational(xeditJNI.AVRationalDivision(Rational.getCPtr(rA), rA, Rational.getCPtr(rB), rB), true);
  }

  public static Rational AVRationalScale(long a, Rational r) {
    return new Rational(xeditJNI.AVRationalScale(a, Rational.getCPtr(r), r), true);
  }

  public static int AVCreateCondition(SWIGTYPE_p_p_libav__ICondition ppOutCond, String name) {
    return xeditJNI.AVCreateCondition__SWIG_0(SWIGTYPE_p_p_libav__ICondition.getCPtr(ppOutCond), name);
  }

  public static int AVCreateCondition(SWIGTYPE_p_p_libav__ICondition ppOutCond) {
    return xeditJNI.AVCreateCondition__SWIG_1(SWIGTYPE_p_p_libav__ICondition.getCPtr(ppOutCond));
  }

  public static int AVCreateMutex(SWIGTYPE_p_p_libav__IMutex ppOutMutex) {
    return xeditJNI.AVCreateMutex(SWIGTYPE_p_p_libav__IMutex.getCPtr(ppOutMutex));
  }

  public static java.math.BigInteger AVGetCurrentTime() {
    return xeditJNI.AVGetCurrentTime();
  }

  public static DateTime AVGetCurrentDate() {
    return new DateTime(xeditJNI.AVGetCurrentDate(), true);
  }

  public static int AVGetCurrentClock() {
    return xeditJNI.AVGetCurrentClock();
  }

  public static int AVClocksPerSecond() {
    return xeditJNI.AVClocksPerSecond();
  }

  public static float AVClockToTime(int clock) {
    return xeditJNI.AVClockToTime(clock);
  }

  public static int AVTimeToClock(float dTimeDuration) {
    return xeditJNI.AVTimeToClock(dTimeDuration);
  }

  public static void AVSleep(int msec) {
    xeditJNI.AVSleep(msec);
  }

  public static void AVSetLibDirectory(String pLibDir) {
    xeditJNI.AVSetLibDirectory(pLibDir);
  }

  public static SWIGTYPE_p_void AVLoadDynamicLib(String pLibFile) {
    long cPtr = xeditJNI.AVLoadDynamicLib(pLibFile);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static SWIGTYPE_p_void AVFindFunction(SWIGTYPE_p_void pLibAddr, String pFuncName) {
    long cPtr = xeditJNI.AVFindFunction(SWIGTYPE_p_void.getCPtr(pLibAddr), pFuncName);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static boolean AVFreeDynamicLib(SWIGTYPE_p_void pLibAddr) {
    return xeditJNI.AVFreeDynamicLib(SWIGTYPE_p_void.getCPtr(pLibAddr));
  }

  public static void AVGetCurrentModulePath(String pOutPath) {
    xeditJNI.AVGetCurrentModulePath(pOutPath);
  }

  public static SWIGTYPE_p_void AVGetModuleHandle(String pFuncName) {
    long cPtr = xeditJNI.AVGetModuleHandle(pFuncName);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static void AVGetModulePath(SWIGTYPE_p_void pHandle, String pOutPath) {
    xeditJNI.AVGetModulePath(SWIGTYPE_p_void.getCPtr(pHandle), pOutPath);
  }

  public static SWIGTYPE_p_void AVMemcopy(SWIGTYPE_p_void pDst, SWIGTYPE_p_void pSrc, int ui32Count) {
    long cPtr = xeditJNI.AVMemcopy(SWIGTYPE_p_void.getCPtr(pDst), SWIGTYPE_p_void.getCPtr(pSrc), ui32Count);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static void AVTrace(String format) {
    xeditJNI.AVTrace(format);
  }

  public static int AVGetSampleBits(ESampleFormat eSampleFmt) {
    return xeditJNI.AVGetSampleBits(eSampleFmt.swigValue());
  }

  public static int AVGetCurrentProcessID() {
    return xeditJNI.AVGetCurrentProcessID();
  }

  public static int AVGetCurrentThreadID() {
    return xeditJNI.AVGetCurrentThreadID();
  }

  public static boolean AVHasAlphaChannel(EPixFormat ePixFmt) {
    return xeditJNI.AVHasAlphaChannel(ePixFmt.swigValue());
  }

  public static int AVGetFillColor(EPixFormat ePixFmt) {
    return xeditJNI.AVGetFillColor(ePixFmt.swigValue());
  }

  public static int AVGetPlaneCount(EPixFormat ePixFmt) {
    return xeditJNI.AVGetPlaneCount(ePixFmt.swigValue());
  }

  public static boolean AVFileOrDirExist(String pStrPath) {
    return xeditJNI.AVFileOrDirExist(pStrPath);
  }

  public static int AVMakeDir(String pStrDir) {
    return xeditJNI.AVMakeDir(pStrDir);
  }

  public static long AVGetFileModifyTime(String pStrFile) {
    return xeditJNI.AVGetFileModifyTime(pStrFile);
  }

  public static boolean AVDumpBinary(String filename, SWIGTYPE_p_unsigned_char buffer, int size) {
    return xeditJNI.AVDumpBinary(filename, SWIGTYPE_p_unsigned_char.getCPtr(buffer), size);
  }

  public static int AVGetCPUCoreNumber() {
    return xeditJNI.AVGetCPUCoreNumber();
  }

  public static String AVGetSystemCodeString(long nCode) {
    return xeditJNI.AVGetSystemCodeString(nCode);
  }

  public static long GCD(long m, long n) {
    return xeditJNI.GCD(m, n);
  }

  public static long LCM(long m, long n) {
    return xeditJNI.LCM(m, n);
  }

  public static long Scale(long a, long b, long c) {
    return xeditJNI.Scale(a, b, c);
  }

  public static SWIGTYPE_p_void AVMalloc(int size) {
    long cPtr = xeditJNI.AVMalloc(size);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public static void AVFree(SWIGTYPE_p_p_void ptr) {
    xeditJNI.AVFree(SWIGTYPE_p_p_void.getCPtr(ptr));
  }

  public static IAVGlobal AVGetGlobal() {
    long cPtr = xeditJNI.AVGetGlobal();
    return (cPtr == 0) ? null : new IAVGlobal(cPtr, false);
  }

  public static int AVCreatePicture(IPicture ppOutPic) {
    return xeditJNI.AVCreatePicture(ppOutPic);
  }

  public static int AVCreateFrame(EFrameType eType, IFrame ppOutFrame) {
    return xeditJNI.AVCreateFrame(eType.swigValue(), ppOutFrame);
  }

  public static int AVCreatePacket(IPacket ppOutPkt) {
    return xeditJNI.AVCreatePacket(ppOutPkt);
  }

  public static int AVCreateStream(IStream ppOutStrm, EStreamType eStrmType) {
    return xeditJNI.AVCreateStream(ppOutStrm, eStrmType.swigValue());
  }

  public static void AVLogAddReceiver(ILogReceiver pLogReceiver) {
    xeditJNI.AVLogAddReceiver(ILogReceiver.getCPtr(pLogReceiver), pLogReceiver);
  }

  public static void AVLogRemoveReceiver(ILogReceiver pLogReceiver) {
    xeditJNI.AVLogRemoveReceiver(ILogReceiver.getCPtr(pLogReceiver), pLogReceiver);
  }

  public static void AVLogSetLevel(ELogLevel eLevel) {
    xeditJNI.AVLogSetLevel(eLevel.swigValue());
  }

  public static ELogLevel AVLogGetLevel() {
    return ELogLevel.swigToEnum(xeditJNI.AVLogGetLevel());
  }

  public static void AVLOG(ELogLevel eLevel, String format) {
    xeditJNI.AVLOG(eLevel.swigValue(), format);
  }

  public static void AVLOGS(ELogLevel eLevel, int code, String format) {
    xeditJNI.AVLOGS(eLevel.swigValue(), code, format);
  }

  public static int AVCreateLogReceiver(ILogReceiver ppOutReceiver) {
    return xeditJNI.AVCreateLogReceiver(ppOutReceiver);
  }

  public static long idGenerateOne() {
    return xeditJNI.idGenerateOne();
  }

  public static IDictionary createDictionary() {
    long cPtr = xeditJNI.createDictionary();
    return (cPtr == 0) ? null : new IDictionary(cPtr, false);
  }

  public static int xSaveImage(String path, SWIGTYPE_p_unsigned_char pBuffer, EPixFormat srcFormat, int nSize, int nSrcLineSize, GSize srcSize, GSize dstSize, EImageType dstImageType) {
    return xeditJNI.xSaveImage(path, SWIGTYPE_p_unsigned_char.getCPtr(pBuffer), srcFormat.swigValue(), nSize, nSrcLineSize, GSize.getCPtr(srcSize), srcSize, GSize.getCPtr(dstSize), dstSize, dstImageType.swigValue());
  }

  public static int xScaleImage(String src, String dst, GSize dstSize, EImageType dstImageType) {
    return xeditJNI.xScaleImage__SWIG_0(src, dst, GSize.getCPtr(dstSize), dstSize, dstImageType.swigValue());
  }

  public static int xScaleImage(String src, String dst, GSize dstSize) {
    return xeditJNI.xScaleImage__SWIG_1(src, dst, GSize.getCPtr(dstSize), dstSize);
  }

}
