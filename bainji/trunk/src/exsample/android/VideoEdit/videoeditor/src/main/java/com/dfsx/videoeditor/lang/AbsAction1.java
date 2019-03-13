package com.dfsx.videoeditor.lang;

import rx.functions.Action1;

public abstract class AbsAction1<P, T> implements Action1<T> {
    private P param;

    public AbsAction1(P p) {
        param = p;
    }

    @Override
    public void call(T t) {
        call(param, t);
    }

    public abstract void call(P p, T t);
}
