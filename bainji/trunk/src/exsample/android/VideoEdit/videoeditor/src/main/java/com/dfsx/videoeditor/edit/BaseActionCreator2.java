package com.dfsx.videoeditor.edit;

public abstract class BaseActionCreator2<T, D> extends BaseActionCreator<T> {
    protected D params2;

    public BaseActionCreator2(T t, D d) {
        super(t);
        this.params2 = d;
    }
}
