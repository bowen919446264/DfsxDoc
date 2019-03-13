/*
 *
 * SGLView.java
 *
 * Created by Wuwang on 2016/10/15
 * Copyright © 2016年 深圳哎吖科技. All rights reserved.
 */
package com.dfsx.videoeditor.videorender;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import com.dfsx.editengine.bean.VideoFrameBuffer;
import com.dfsx.videoeditor.videorender.filter.AFilter;

import java.io.IOException;

/**
 * Description:
 */
public class SGLView extends GLSurfaceView {

    private SGLRender render;

    public SGLView(Context context) {
        this(context, null);
    }

    public SGLView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        setEGLContextClientVersion(2);
        render = new SGLRender(this);
        setRenderer(render);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

    }

    public void putFrameBuffer(VideoFrameBuffer frameBuffer) {
        render.setFrameBuffer(frameBuffer);
        requestRender();
    }

    public SGLRender getRender() {
        return render;
    }

    public void setFilter(AFilter filter) {
        render.setFilter(filter);
    }

}
