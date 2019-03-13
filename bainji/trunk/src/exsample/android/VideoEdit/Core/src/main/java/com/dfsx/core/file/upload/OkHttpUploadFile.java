package com.dfsx.core.file.upload;

import android.util.Log;
import cn.edu.zafu.coreprogress.listener.impl.UIProgressRequestListener;
import com.dfsx.core.network.HttpUtil;
import org.json.JSONObject;

import java.util.ArrayList;

public class OkHttpUploadFile implements IUpload {

    private UploadPercentListener uploadPercentListener;

    private DataUIProgressRequestListener uiProgressRequestListener;

    public OkHttpUploadFile() {
        uiProgressRequestListener = new DataUIProgressRequestListener();
    }

    private boolean uploadData(UploadFileData data, int pos) {
        try {
            uiProgressRequestListener.setIndex(pos);
            String res = HttpUtil.uploadFileSynchronized(data.getUploadServiceUrl(),
                    data.getFile(), uiProgressRequestListener);
            Log.e("TAG", "upfile " + data.getFile().getPath() + "--- res == " + res);
            boolean isOk = false;
            String servicePath = "";
            JSONObject json = new JSONObject(res);
            isOk = json.optInt("isOK") > 0;
            String name = json.optString("name");
            String dir = json.optString("relativepath");
            servicePath = dir + name;
            data.setResult(new UploadFileData.UploadResult(isOk, servicePath, name));
            return isOk;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    @Override
    public boolean upload(ArrayList<UploadFileData> uploadList) {
        if (uploadList != null) {
            uiProgressRequestListener.setAllCount(uploadList.size());
            for (int i = 0; i < uploadList.size(); i++) {
                UploadFileData data = uploadList.get(i);
                if (data.getResult() == null || !data.getResult().isSuccess()) {
                    boolean isOk = uploadData(data, i);
                    if (!isOk) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    public void setUploadPercentListener(UploadPercentListener uploadPercentListener) {
        this.uploadPercentListener = uploadPercentListener;
    }

    public interface UploadPercentListener {

        /**
         * 上的进度 100%
         * 大小为 0-100
         *
         * @param percent
         * @param done    上传流程结束 这个并不意味着成功
         */
        void onUploadPercent(int percent, boolean done);
    }

    class DataUIProgressRequestListener extends UIProgressRequestListener {

        private int index;
        private int allCount;

        private int currentPercent;

        public void setIndex(int index) {
            this.index = index;
        }

        @Override
        public void onUIRequestProgress(long bytesWrite, long contentLength, boolean done) {
            float p = (float) bytesWrite / contentLength * 100;
            currentPercent = (int) (index * 100 / allCount + p / allCount);
            boolean isDone = (index == (allCount - 1)) && done;
            if (uploadPercentListener != null) {
                uploadPercentListener.onUploadPercent(currentPercent, isDone);
            }
        }

        public void setAllCount(int allCount) {
            this.allCount = allCount;
        }
    }
}
