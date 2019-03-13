package com.dfsx.editengine;

/**
 * 扩展可以传递多参数的任务
 *
 * @param <D> 任务返回值 类型
 */
public abstract class BaseEngineTask<D> implements EngineThread.IEngineTask<D> {
    private Object[] params;

    public BaseEngineTask(Object... objects) {
        this.params = objects;
    }

    @Override
    public D run() {
        return run(params);
    }

    public abstract D run(Object... params);
}
