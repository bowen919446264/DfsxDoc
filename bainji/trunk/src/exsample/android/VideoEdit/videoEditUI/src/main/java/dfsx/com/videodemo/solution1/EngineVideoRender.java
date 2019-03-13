package dfsx.com.videodemo.solution1;

import com.dfsx.editengine.bean.Render;
import com.dfsx.editengine.bean.VideoFrameBuffer;
import com.dfsx.editengine.xedit.XEditVideoFrameBufferRender;
import com.dfsx.videoeditor.videorender.SGLView;
import com.ds.xedit.jni.IRenderer;

public class EngineVideoRender implements Render {

    private VideoFrameRender frameRender;
    private SGLView sglView;

    public EngineVideoRender() {
        frameRender = new VideoFrameRender();
    }

    @Override
    public IRenderer getXRender() {
        return frameRender;
    }

    public void setSglView(SGLView sglView) {
        this.sglView = sglView;
    }

    class VideoFrameRender extends XEditVideoFrameBufferRender {

        @Override
        public void renderFrame(VideoFrameBuffer frameBuffer) {
            if (sglView != null) {
                sglView.putFrameBuffer(frameBuffer);
            }
        }
    }

}
