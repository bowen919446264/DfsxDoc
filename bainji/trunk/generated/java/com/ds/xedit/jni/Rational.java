/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.ds.xedit.jni;

public class Rational {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected Rational(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Rational obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        xeditJNI.delete_Rational(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public static Rational getZero() {
    long cPtr = xeditJNI.Rational_zero_get();
    return (cPtr == 0) ? null : new Rational(cPtr, false);
  }

  public void setNNum(long value) {
    xeditJNI.Rational_nNum_set(swigCPtr, this, value);
  }

  public long getNNum() {
    return xeditJNI.Rational_nNum_get(swigCPtr, this);
  }

  public void setNDen(long value) {
    xeditJNI.Rational_nDen_set(swigCPtr, this, value);
  }

  public long getNDen() {
    return xeditJNI.Rational_nDen_get(swigCPtr, this);
  }

  public Rational() {
    this(xeditJNI.new_Rational__SWIG_0(), true);
  }

  public Rational(long nInNum, long nInDen) {
    this(xeditJNI.new_Rational__SWIG_1(nInNum, nInDen), true);
  }

  public long integerValue() {
    return xeditJNI.Rational_integerValue(swigCPtr, this);
  }

  public double doubleValue() {
    return xeditJNI.Rational_doubleValue(swigCPtr, this);
  }

  public Rational absValue() {
    return new Rational(xeditJNI.Rational_absValue(swigCPtr, this), true);
  }

  public Rational reverseValue() {
    return new Rational(xeditJNI.Rational_reverseValue(swigCPtr, this), true);
  }

  public Rational add(Rational r) {
    return new Rational(xeditJNI.Rational_add(swigCPtr, this, Rational.getCPtr(r), r), true);
  }

  public Rational subtract(Rational r) {
    return new Rational(xeditJNI.Rational_subtract(swigCPtr, this, Rational.getCPtr(r), r), true);
  }

  public Rational multiply(Rational r) {
    return new Rational(xeditJNI.Rational_multiply__SWIG_0(swigCPtr, this, Rational.getCPtr(r), r), true);
  }

  public Rational multiply(long a) {
    return new Rational(xeditJNI.Rational_multiply__SWIG_1(swigCPtr, this, a), true);
  }

  public Rational divide(Rational r) {
    return new Rational(xeditJNI.Rational_divide__SWIG_0(swigCPtr, this, Rational.getCPtr(r), r), true);
  }

  public Rational divide(long a) {
    return new Rational(xeditJNI.Rational_divide__SWIG_1(swigCPtr, this, a), true);
  }

  public void assign(Rational r) {
    xeditJNI.Rational_assign(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public void selfAdd(Rational r) {
    xeditJNI.Rational_selfAdd(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public void selfSubtract(Rational r) {
    xeditJNI.Rational_selfSubtract(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public void selfMultiply(Rational r) {
    xeditJNI.Rational_selfMultiply(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public void selfDivide(Rational r) {
    xeditJNI.Rational_selfDivide(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public boolean isEqualTo(Rational r) {
    return xeditJNI.Rational_isEqualTo(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public boolean isLessThan(Rational r) {
    return xeditJNI.Rational_isLessThan(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public boolean isLessThanOrEqual(Rational r) {
    return xeditJNI.Rational_isLessThanOrEqual(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public boolean isGreaterThan(Rational r) {
    return xeditJNI.Rational_isGreaterThan(swigCPtr, this, Rational.getCPtr(r), r);
  }

  public boolean isGreaterThanOrEqual(Rational r) {
    return xeditJNI.Rational_isGreaterThanOrEqual(swigCPtr, this, Rational.getCPtr(r), r);
  }

}