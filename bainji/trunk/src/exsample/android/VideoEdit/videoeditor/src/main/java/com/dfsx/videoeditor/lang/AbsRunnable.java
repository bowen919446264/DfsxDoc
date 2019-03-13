package com.dfsx.videoeditor.lang;

/**
 * 基础带参数的线程
 * @param <T>
 */
public abstract class AbsRunnable<T> implements Runnable {
    private T data;

    public AbsRunnable(T data) {
        this.data = data;
    }

    @Override
    public void run() {
        run(data);
    }

    public abstract void run(T data);
}
