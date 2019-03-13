package com.dfsx.editengine.xedit;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import com.dfsx.editengine.bean.IEngineGenerateListener;
import com.dfsx.editengine.util.LogUtil;
import com.ds.xedit.jni.GenerateSetting;
import com.ds.xedit.jni.IGenerateObserver;
import com.ds.xedit.jni.ITimeLine;
import com.ds.xedit.jni.Rational;

public class XEditGenerateObserver extends IGenerateObserver {

    private static final int MSG_FINISH = 1;
    private static final int MSG_PROCESS = 2;
    private ITimeLine timeLine;

    private IEngineGenerateListener engineGenerateListener;
    private Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case MSG_FINISH:
                    int code = msg.arg1;
                    if (engineGenerateListener != null) {
                        engineGenerateListener.onFinish(code);
                    }
                    break;
                case MSG_PROCESS:
                    double progress = (double) msg.obj;
                    if (engineGenerateListener != null) {
                        engineGenerateListener.onGenerateProcess(progress);
                    }
                    break;
            }
        }
    };

    public XEditGenerateObserver(ITimeLine timeLine) {
        this.timeLine = timeLine;
    }

    public void onFinish(GenerateSetting param, int code) {
        Log.e(LogUtil.TAG, "onFinish code ===== " + code);
        handler.sendMessage(handler.obtainMessage(MSG_FINISH, code, code));

    }

    @Override
    public void onUpdateProcess(GenerateSetting param, Rational rDuration) {
        double value = rDuration != null ? rDuration.doubleValue() : 0;
        Log.e(LogUtil.TAG, "onUpdateProcess ===== " + rDuration.doubleValue());
        double duration = 1;
        if(timeLine != null) {
            duration = timeLine.getDuration().doubleValue();
            if(duration == 0) {
                duration = 1;
            }
        }
        handler.sendMessage(handler.obtainMessage(MSG_PROCESS, value / duration));
    }

    public void setEngineGenerateListener(IEngineGenerateListener engineGenerateListener) {
        this.engineGenerateListener = engineGenerateListener;
    }
}
