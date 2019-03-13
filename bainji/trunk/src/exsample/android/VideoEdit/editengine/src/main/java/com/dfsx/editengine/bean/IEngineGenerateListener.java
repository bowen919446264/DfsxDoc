package com.dfsx.editengine.bean;

public interface IEngineGenerateListener {

    /**
     * 結束
     *
     * @param statusCode
     */
    void onFinish(int statusCode);

    /**
     * 进度
     *
     * @param progress 0 到 1
     */
    void onGenerateProcess(double progress);
}
