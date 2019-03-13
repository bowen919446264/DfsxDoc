package com.ds.xedit.androidexsample;

import android.widget.TextView;
import android.widget.Toast;

import com.ds.xedit.jni.GenerateSetting;
import com.ds.xedit.jni.IGenerateObserver;
import com.ds.xedit.jni.Rational;

public class GenerateObserver extends IGenerateObserver {
    private MainActivity mainActivity;

    public GenerateObserver(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }

    @Override
    public void onFinish(GenerateSetting param, int code) {
        final int resultCode = code;
        mainActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (resultCode < 0) {
                    Toast.makeText(mainActivity.getApplicationContext(), "生成失败！", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(mainActivity.getApplicationContext(), "生成成功", Toast.LENGTH_SHORT).show();
                }
            }
        });

    }

    @Override
    public void onUpdateProcess(GenerateSetting param, Rational rDuration) {
        final TextView currentFrameTextView = mainActivity.findViewById(R.id.currentFrameTextView);
        final long currentFrame = new Rational(rDuration.getNNum() * 25, rDuration.getNDen()).integerValue() + 1;
        currentFrameTextView.post(new Runnable() {
            @Override
            public void run() {
                currentFrameTextView.setText(currentFrame + "");
            }
        });
    }

}
