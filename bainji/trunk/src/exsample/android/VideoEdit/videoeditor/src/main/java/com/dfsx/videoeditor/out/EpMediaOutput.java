package com.dfsx.videoeditor.out;

/*import VideoHandle.EpEditor;
import VideoHandle.EpVideo;
import VideoHandle.OnEditorListener;*/

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Size;

import com.dfsx.videoeditor.video.VideoSource;

import java.io.File;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

public class EpMediaOutput implements IOut {

    private Handler handler = new Handler(Looper.getMainLooper());
    private Context context;
    private Size outputSize;
    private List<VideoSource> videoSourceList;
    private AtomicReference<IOut.Listener> listenerAtomicReference;

    private boolean isCanceled;

    public EpMediaOutput(Context context, Size outputSize,
                         List<VideoSource> list, IOut.Listener editorListener) {
        this.context = context;
        this.outputSize = outputSize;
        this.videoSourceList = list;
        listenerAtomicReference = new AtomicReference<>();
        listenerAtomicReference.set(editorListener);
    }

    @Override
    public void onFileOutPut(File outPutFile) {
        isCanceled = false;
        if (videoSourceList == null || videoSourceList.size() <= 0) {
            return;
        }
/*        Observable.just(outPutFile)
                .observeOn(Schedulers.io())
                .map(new Func1<File, Object>() {
                    @Override
                    public Object call(File outFile) {
                        ArrayList<EpVideo> epVideos = new ArrayList<>();
                        for (VideoSource videoSource : videoSourceList) {
                            EpVideo epVideo = new EpVideo(videoSource.sourcePath);
                            if (videoSource.getPlayTimeRange() != null) {
                                long startMs = videoSource.getPlayTimeRange()[0];
                                long endMs = videoSource.getPlayTimeRange()[1];
                                long duration = (endMs - startMs) / 1000;
                                epVideo.clip(startMs / 1000, duration);
                            }
                            if (videoSource.degree == 90 || videoSource.degree == 270) {
                                epVideo.rotation(videoSource.degree, false);
                            }
                            epVideos.add(epVideo);
                        }
                        EpEditor.OutputOption outputOption = new EpEditor.OutputOption(outFile.getPath());
                        outputOption.setWidth(outputSize.getWidth());
                        outputOption.setHeight(outputSize.getHeight());
                        outputOption.frameRate = 30;
                        //                        outputOption.bitRate = 4 * 1000;
                        if (epVideos.size() > 1) {
                            EpEditor.merge(epVideos, outputOption, new OnEditorListener() {
                                @Override
                                public void onSuccess() {
                                    handler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            listenerAtomicReference.get().onOutputCompleted();
                                        }
                                    });
                                }

                                @Override
                                public void onFailure() {
                                    handler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            if (!isCanceled) {
                                                listenerAtomicReference.get().onOutputFailed(new Exception("not know exception"));
                                            }
                                        }
                                    });
                                }

                                @Override
                                public void onProgress(final float v) {
                                    handler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            listenerAtomicReference.get().onOutputProgress(v);
                                        }
                                    });
                                }
                            });
                        } else if (epVideos.size() == 1) {
                            EpEditor.exec(epVideos.get(0), outputOption, new OnEditorListener() {
                                @Override
                                public void onSuccess() {
                                    listenerAtomicReference.get().onOutputCompleted();
                                }

                                @Override
                                public void onFailure() {
                                    if (!isCanceled) {
                                        listenerAtomicReference.get().onOutputFailed(new Exception("not know exception"));
                                    }
                                }

                                @Override
                                public void onProgress(final float progress) {
                                    //这里获取处理进度
                                    handler.post(new Runnable() {
                                        @Override
                                        public void run() {
                                            listenerAtomicReference.get().onOutputProgress(progress);
                                        }
                                    });
                                }
                            });
                        }
                        return null;
                    }
                })
                .subscribe();*/
    }

    @Override
    public void onCancelOutPut() {
        isCanceled = true;
        handler.post(new Runnable() {
            @Override
            public void run() {
                listenerAtomicReference.get().onOutputCanceled();
            }
        });
    }
}
