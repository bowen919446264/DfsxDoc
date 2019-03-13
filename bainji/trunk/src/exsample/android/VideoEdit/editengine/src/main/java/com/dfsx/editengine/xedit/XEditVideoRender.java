package com.dfsx.editengine.xedit;

import com.dfsx.editengine.NativeHelper;
import com.dfsx.editengine.bean.Render;
import com.ds.xedit.jni.IRenderer;

public class XEditVideoRender implements Render {

    private NativeHelper.VideoRenderer videoRenderer;

    public XEditVideoRender() {
        videoRenderer = new NativeHelper.VideoRenderer();
    }

    public IRenderer getXRender() {
        return videoRenderer;
    }
}
