package com.dfsx.videoeditor.widget;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import com.bumptech.glide.Glide;

public class DrawImageSurface extends SurfaceView implements SurfaceHolder.Callback {

    private Handler drawImageHander;

    private Paint paint = new Paint();

    public DrawImageSurface(Context context) {
        super(context);
        init();
    }

    public DrawImageSurface(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public DrawImageSurface(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }


    private void init() {
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        DrawImageHandlerThread thread = new DrawImageHandlerThread("image+hander");
        thread.start();
        drawImageHander = new Handler(thread.getLooper(), thread);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        drawImageHander.removeCallbacksAndMessages(null);
    }

    public void drawImage(String image, int w, int h) {
        if (!TextUtils.isEmpty(image)) {
            Message msg = drawImageHander.obtainMessage(1011);
            msg.obj = image;
            drawImageHander.sendMessage(msg);
        }
    }

    class DrawImageHandlerThread extends HandlerThread implements Handler.Callback {

        public DrawImageHandlerThread(String name) {
            super(name);
        }

        @Override
        public boolean handleMessage(Message msg) {
            //打印线程的名称
            if (msg.what == 1011) {
                try {
                    long time = System.currentTimeMillis();
                    String imagepath = (String) msg.obj;
                    Canvas canvas = getHolder().lockCanvas(); //获取画布
                    Bitmap bitmap = Glide.with(getContext())
                            .load(imagepath)
                            .asBitmap()
                            .into(-1, -1).get();
                    if (bitmap != null) {
                        canvas.drawBitmap(bitmap, 0, 0, paint);
                    }
                    getHolder().unlockCanvasAndPost(canvas);//解锁画布，提交画好的图像
                    Log.e("TAG", "draw time == " + (System.currentTimeMillis() - time));
                } catch (Exception e) {
                    // TODO: handle exception
                    Log.e("TAG", "DrawSurfaceView：绘制失败...");
                }
            }
            return true;
        }
    }
}
