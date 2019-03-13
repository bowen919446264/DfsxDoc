package com.ds.xedit.androidexsample;

import android.util.Log;

import com.ds.xedit.jni.ELogLevel;
import com.ds.xedit.jni.ILogReceiver;

public class LogReceiver extends ILogReceiver {

    @Override
    public void Receive(ELogLevel eLevel, String pStrLog) {
        int nLevel = eLevel.swigValue();
        if (nLevel == ELogLevel.ELOG_LEVEL_DEBUG.swigValue()) {
            Log.d("", pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_ERROR.swigValue()) {
            Log.e("", pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_INFO.swigValue()) {
            Log.i("", pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_WARNING.swigValue()) {
            Log.w("", pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_VERBOSE.swigValue()) {
            Log.v("", pStrLog);
        } else {
            Log.i("", pStrLog);
        }
    }
}
