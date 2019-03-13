package com.dfsx.videoeditor.edit;

import rx.functions.Action1;

/**
 * 编辑操作
 */
public interface IEditAction {

    /**
     * 执行操作 UI线程
     * return true 则加到缓存数据里面
     */
    void onDo(Action1<Boolean> result);

    /**
     * 取消执行操作 UI线程
     * <p>
     * return true 则加到缓存数据里面
     */
    void onCancelDo(Action1<Boolean> result);

    /**
     * 是否可以撤销和反撤销
     *
     * @return
     */
    boolean isCouldBackOrUnBack();
}
