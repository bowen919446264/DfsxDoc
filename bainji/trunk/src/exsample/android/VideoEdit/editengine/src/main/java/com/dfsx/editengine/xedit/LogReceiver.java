package com.dfsx.editengine.xedit;

import android.util.Log;

import com.dfsx.editengine.util.LogUtil;
import com.ds.xedit.jni.ELogLevel;
import com.ds.xedit.jni.ILogReceiver;

public class LogReceiver extends ILogReceiver {

    @Override
    public void Receive(ELogLevel eLevel, String pStrLog) {
        int nLevel = eLevel.swigValue();
        if (nLevel == ELogLevel.ELOG_LEVEL_DEBUG.swigValue()) {
            Log.d(LogUtil.TAG, pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_ERROR.swigValue()) {
            Log.e(LogUtil.TAG, pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_INFO.swigValue()) {
            Log.i(LogUtil.TAG, pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_WARNING.swigValue()) {
            Log.w(LogUtil.TAG, pStrLog);
        } else if (nLevel == ELogLevel.ELOG_LEVEL_VERBOSE.swigValue()) {
            Log.v(LogUtil.TAG, pStrLog);
        } else {
            Log.i(LogUtil.TAG, pStrLog);
        }
    }
}
