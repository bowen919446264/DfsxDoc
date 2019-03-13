package com.dfsx.videoeditor.out;

import android.util.Log;

import com.dfsx.editengine.EngineProjectManager;
import com.dfsx.editengine.bean.GenerateConfig;
import com.dfsx.editengine.bean.IEngineGenerateListener;

import java.io.File;

public class EngineOutPut implements IOut, IEngineGenerateListener {

    private EngineProjectManager engineProjectManager;
    private GenerateConfig config;
    private IOut.Listener outListener;
    private long startTime = -1;
    private long durationTime = -1;

    public EngineOutPut(EngineProjectManager engineProjectManager, GenerateConfig outputConfig, IOut.Listener outListener) {
        this.engineProjectManager = engineProjectManager;
        this.config = outputConfig;
        this.outListener = outListener;
    }

    @Override
    public void onFileOutPut(File outPutFile) {
        if (outPutFile != null && config != null && engineProjectManager != null) {
            engineProjectManager.output(outPutFile.getPath(), config, startTime, durationTime, this);
        } else {
            Log.e("TAG", "out参数异常--------------");
        }
    }

    @Override
    public void onCancelOutPut() {
        if (engineProjectManager != null) {
            engineProjectManager.cancelGenerate();
            if (outListener != null) {
                outListener.onOutputCanceled();
            }
        }
    }

    public void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    public void setDurationTime(long durationTime) {
        this.durationTime = durationTime;
    }

    @Override
    public void onFinish(int statusCode) {
        if (outListener != null) {
            boolean isOk = statusCode >= 0;
            if (isOk) {
                outListener.onOutputCompleted();
            } else {
                outListener.onOutputFailed(new Exception("输出失败!"));
            }
        }
    }

    @Override
    public void onGenerateProcess(double progress) {
        if (outListener != null) {
            outListener.onOutputProgress(progress);
        }
    }
}
