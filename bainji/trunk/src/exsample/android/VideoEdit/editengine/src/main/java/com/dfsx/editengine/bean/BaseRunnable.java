package com.dfsx.editengine.bean;

public abstract class BaseRunnable<T> implements Runnable {

    private T data;

    public BaseRunnable(T data) {
        this.data = data;
    }

    @Override
    public void run() {
        run(data);
    }

    public abstract void run(T data);
}
