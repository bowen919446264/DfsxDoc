//package com.dfsx.videoeditor.timeline;
//
//import android.os.Handler;
//import android.os.Looper;
//import android.os.Message;
//import android.util.Log;
//import com.google.android.exoplayer2.ExoPlaybackException;
//import com.google.android.exoplayer2.PlaybackParameters;
//import com.google.android.exoplayer2.Player;
//
//import java.util.concurrent.BlockingQueue;
//import java.util.concurrent.LinkedBlockingQueue;
//import java.util.concurrent.TimeUnit;
//
///**
// * 拖动时间平滑实现画面预览，因由：视频播放器seekTo到画面显示大约耗时100ms，
// * 按1秒20帧计算，实时定位显示不太可能，
// * 目前在播放器的SeekTo机制中，在没有Seek完成前，
// * 重复调用会自动取消上次的seek任务，
// * 而达到总是定位到最后的时间位置，
// * 画面就会突变跳动；因此在快速滑动时，
// * 记录的很多需要定位的时间点需要动态丢弃。
// * 在上次seek完成之前不再进行在次seekTo调用，为了实时，
// * 就需要在seek的所耗时间中丢弃这段时间产生的新的seek任务，
// * 完成时获取当时的任务重新定位。
// */
//public class RealTimeSeekController extends Player.DefaultEventListener implements Runnable {
//
//    private static final int MSG_WHAT_SEEK = 10;
//
//    private Player player;
//
//    private boolean isSeeking = false;
//
//    private boolean isSeekThreadRun = false;
//
//    private Object object = new Object();
//
//    private long startSeekTime;
//
//    /**
//     * 结束位置的seek，
//     * 每次seek完成，必須强制seek到这个点
//     */
//    private SeekInfo endQueueSeek;
//
//    private Handler mainHandler = new Handler(Looper.getMainLooper()) {
//        @Override
//        public void handleMessage(Message msg) {
//            super.handleMessage(msg);
//            if (msg.what == MSG_WHAT_SEEK) {
//                SeekInfo info = (SeekInfo) msg.obj;
//                long time = info.seekTime;
//                int index = info.videoIndex;
//                player.seekTo(index, time);
//            }
//        }
//    };
//    /**
//     * seek 阻塞器
//     */
//    private BlockingQueue<Boolean> seekSyncObject = new LinkedBlockingQueue<>(1);
//
//    /**
//     * 需要seek的队列
//     */
//    private BlockingQueue<SeekInfo> seekTimeQueue;
//
//    public RealTimeSeekController(Player player) {
//        this.player = player;
//        if (this.player != null) {
//            this.player.addListener(this);
//        }
//        seekTimeQueue = new LinkedBlockingQueue<>();
//    }
//
//    public void seekTo(int videoIndex, long timeMs) {
//        SeekInfo seekInfo = new SeekInfo(System.currentTimeMillis(), videoIndex, timeMs);
//        seekTimeQueue.add(seekInfo);
//        endQueueSeek = seekInfo;
//        synchronized (object) {
//            if (!isSeekThreadRun) {
//                new Thread(this).start();
//                isSeekThreadRun = true;
//            }
//        }
//    }
//
//    @Override
//    public void onLoadingChanged(boolean isLoading) {
//    }
//
//    @Override
//    public void onPlayerStateChanged(boolean playWhenReady, int playbackState) {
//        if (playbackState == Player.STATE_READY) {
//            Log.e("TAGG", "playbackState === " + playbackState);
//            if (isSeeking) {
//                Log.e("TAGG", "Seektime == " + (System.currentTimeMillis() - startSeekTime));
//                try {
//                    seekSyncObject.clear();
//                    seekSyncObject.put(true);
//                } catch (InterruptedException e) {
//                    e.printStackTrace();
//                }
//            }
//            isSeeking = false;
//        }
//    }
//
//    @Override
//    public void onRepeatModeChanged(int repeatMode) {
//
//    }
//
//    @Override
//    public void onShuffleModeEnabledChanged(boolean shuffleModeEnabled) {
//
//    }
//
//    @Override
//    public void onPlayerError(ExoPlaybackException error) {
//
//    }
//
//    @Override
//    public void onPositionDiscontinuity(int reason) {
//        Log.e("TAG", "reason === " + reason);
//    }
//
//    @Override
//    public void onPlaybackParametersChanged(PlaybackParameters playbackParameters) {
//
//    }
//
//    @Override
//    public void onSeekProcessed() {
//        Log.e("TAGG", "Seektime == set");
//        startSeekTime = System.currentTimeMillis();
//        isSeeking = true;
//    }
//
//    @Override
//    public void run() {
//        isSeekThreadRun = true;
//        try {
//            SeekInfo seekInfo = getNextSeekInfo(0, 0);
//            while (seekInfo != null && seekInfo.seekTime >= 0) {
//                Message msg = mainHandler.obtainMessage(MSG_WHAT_SEEK);
//                msg.obj = seekInfo;
//                mainHandler.sendMessage(msg);
//                long startSeek = System.currentTimeMillis();
//                try {
//                    seekSyncObject.poll(150, TimeUnit.MILLISECONDS);
//                } catch (Exception e) {
//                    e.printStackTrace();
//                }
//                long seekUseTime = System.currentTimeMillis() - startSeek;
//                Log.e("TAGG", "seek use time == " + seekUseTime);
//                seekInfo = getNextSeekInfo(seekInfo.timestamp, seekUseTime);
//            }
//            //保证最后一个一定是队列的末尾设置的Seek
//            Message msg = mainHandler.obtainMessage(MSG_WHAT_SEEK);
//            msg.obj = endQueueSeek;
//            mainHandler.sendMessage(msg);
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//        isSeekThreadRun = false;
//    }
//
//    private SeekInfo getNextSeekInfo(long latestTime, long skipTime) {
//        SeekInfo info = seekTimeQueue.poll();
//        while (info != null && (info.timestamp - latestTime < skipTime)) {
//            Log.e("TAGG", "drop seek time == " + info.seekTime);
//            info = seekTimeQueue.poll();
//        }
//        return info;
//    }
//
//    class SeekInfo {
//        int videoIndex;
//        long timestamp;
//        Long seekTime;
//
//        public SeekInfo(long sysTime, int videoIndex, Long seekTime) {
//            this.videoIndex = videoIndex;
//            this.timestamp = sysTime;
//            this.seekTime = seekTime;
//        }
//    }
//}