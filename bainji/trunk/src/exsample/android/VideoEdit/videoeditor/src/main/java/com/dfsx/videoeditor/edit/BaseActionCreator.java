package com.dfsx.videoeditor.edit;

public abstract class BaseActionCreator<T> implements IActionCreator {
    protected T params;

    public BaseActionCreator(T t) {
        this.params = t;
    }
}
