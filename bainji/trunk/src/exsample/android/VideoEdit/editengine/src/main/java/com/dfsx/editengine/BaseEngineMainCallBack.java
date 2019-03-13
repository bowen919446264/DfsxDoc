package com.dfsx.editengine;

/**
 * 扩展可以传递多参数的主线程回调
 *
 * @param <T> 回调类型
 */
public abstract class BaseEngineMainCallBack<T> implements EngineThread.IEngineMainCallback<T> {
    protected Object[] params;

    public BaseEngineMainCallBack(Object... params) {
        this.params = params;
    }
}
