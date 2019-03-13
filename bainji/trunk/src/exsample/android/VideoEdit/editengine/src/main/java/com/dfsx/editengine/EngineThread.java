package com.dfsx.editengine;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.support.annotation.NonNull;

/**
 * 引擎的执行线程， 因为引擎的执行。C++库要求 必须同步， 所以直接把他放在一个线程里面
 */
public class EngineThread {

    private HandlerThread engineThread;
    private EngineHandler handler;
    private Handler mainHandler = new Handler(Looper.getMainLooper());

    public EngineThread() {
        engineThread = new HandlerThread("xEdit_thread");
        engineThread.start();
        handler = new EngineHandler(engineThread);
    }

    /**
     * 执行任务， 并且直接回调到主线程
     *
     * @param task     任务
     * @param callback 主线程回调
     * @param <D>      返回值类型
     */
    public <D> void excuteTask(@NonNull IEngineTask<D> task, IEngineMainCallback<D> callback) {
        handler.post(new EngineTask<D>(task, callback));
    }

    public Looper getEngineThreadLooper() {
        return engineThread.getLooper();
    }

    public void stop() {
        engineThread.quit();
    }


    class EngineHandler extends Handler {

        protected EngineHandler(HandlerThread thread) {
            super(thread.getLooper());
        }

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
        }
    }

    class EngineTask<E> implements Runnable {

        private IEngineTask<E> task;

        private IEngineMainCallback<E> callback;

        public EngineTask(@NonNull IEngineTask<E> task, IEngineMainCallback<E> callback) {
            this.task = task;
            this.callback = callback;
        }

        @Override
        public void run() {
            E e = task.run();
            mainHandler.post(new BaseMainRun<E>(callback, e));
        }
    }

    /**
     * @param <R> 回调结果类型
     */
    class BaseMainRun<R> implements Runnable {
        private IEngineMainCallback<R> callback;
        private R r;

        public BaseMainRun(IEngineMainCallback<R> a, R r) {
            this.callback = a;
            this.r = r;
        }

        @Override
        public void run() {
            if (callback != null) {
                callback.onCallBack(r);
            }
        }
    }

    /**
     * 引擎任务
     *
     * @param <D> 任务返回类型
     */
    public interface IEngineTask<D> {
        D run();
    }

    /**
     * 回调
     *
     * @param <T> 回调结果类型
     */
    public interface IEngineMainCallback<T> {

        void onCallBack(T data);
    }
}
