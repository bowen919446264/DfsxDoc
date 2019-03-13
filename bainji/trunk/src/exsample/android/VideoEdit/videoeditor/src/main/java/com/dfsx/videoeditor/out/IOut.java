package com.dfsx.videoeditor.out;

import java.io.File;

public interface IOut {

    void onFileOutPut(File outPutFile);

    void onCancelOutPut();

    public interface Listener {
        void onOutputProgress(double progress);

        void onOutputCompleted();

        /**
         * Called when Output canceled.
         */
        void onOutputCanceled();

        void onOutputFailed(Exception exception);
    }
}
