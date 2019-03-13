package com.dfsx.videoeditor.edit;

import android.support.annotation.NonNull;

import com.dfsx.videoeditor.lang.AbsAction1;

import java.util.Stack;

import rx.functions.Action1;

/**
 * 操作管理器
 */
public final class ActionManager {

    private static ActionManager instance;

    /**
     * 撤销栈
     */
    private Stack<IEditAction> backStack;
    /**
     * 反撤销栈
     */
    private Stack<IEditAction> unBackStack;


    private OnActionStackChangeListener stackChangeListener;

    private static Object object = new Object();

    private ActionManager() {
        backStack = new Stack<>();
        unBackStack = new Stack<>();
    }

    public static ActionManager getInstance() {
        synchronized (object) {
            if (instance == null) {
                instance = new ActionManager();
            }
        }
        return instance;
    }

    /**
     * 调度执行动作
     *
     * @param creator
     */
    public void dispatchAction(@NonNull IActionCreator creator) {
        IEditAction action = creator.createAction();
        action.onDo(new AbsAction1<IEditAction, Boolean>(action) {
            @Override
            public void call(IEditAction editAction, Boolean aBoolean) {
                if (aBoolean && editAction.isCouldBackOrUnBack()) {
                    addBackStack(editAction);
                }
            }
        });
    }

    /**
     * 添加可以测校操作的动作
     *
     * @param editAction
     */
    public void addBackStack(IEditAction editAction) {
        backStack.push(editAction);
        callBackStackChange();
    }

    /**
     * 添加反侧小操作
     *
     * @param editAction
     */
    public void addUnBackStack(IEditAction editAction) {
        unBackStack.push(editAction);
        callUnBackStackChange();
    }

    /**
     * 是否可以撤销
     *
     * @return
     */
    public boolean isCanBack() {
        return backStack != null && !backStack.isEmpty();
    }

    /**
     * 是否可以反撤销
     *
     * @return
     */
    public boolean isCanUnBack() {
        return unBackStack != null && !unBackStack.isEmpty();
    }

    /**
     * 撤销
     */
    public boolean onBackAction() {
        if (isCanBack()) {
            IEditAction action = backStack.pop();
            callBackStackChange();
            if (action != null) {
                action.onCancelDo(new AbsAction1<IEditAction, Boolean>(action) {
                    @Override
                    public void call(IEditAction editAction, Boolean aBoolean) {
                        if (editAction.isCouldBackOrUnBack()) {
                            addUnBackStack(editAction);
                        }
                    }
                });
                return true;
            }
        }
        return false;
    }

    /**
     * 取消撤销
     */
    public boolean onUnBackAction() {
        if (isCanUnBack()) {
            IEditAction action = unBackStack.pop();
            callUnBackStackChange();
            if (action != null) {
                action.onDo(new AbsAction1<IEditAction, Boolean>(action) {
                    @Override
                    public void call(IEditAction editAction, Boolean aBoolean) {
                        if (editAction.isCouldBackOrUnBack()) {
                            addBackStack(editAction);
                        }
                    }
                });
                return true;
            }
        }
        return false;
    }

    public void destory() {
        backStack.clear();
        unBackStack.clear();
        instance = null;
    }

    public void setActionStackChangeListener(OnActionStackChangeListener stackChangeListener) {
        this.stackChangeListener = stackChangeListener;
    }


    private void callBackStackChange() {
        if (stackChangeListener != null) {
            stackChangeListener.onBackStackChange();
        }
    }

    private void callUnBackStackChange() {
        if (stackChangeListener != null) {
            stackChangeListener.onUnBackStackChange();
        }
    }

    /**
     * 内部栈 变化通知
     */
    public interface OnActionStackChangeListener {

        /**
         * 撤销栈 变化
         */
        void onBackStackChange();

        /**
         * 反撤销栈变化
         */
        void onUnBackStackChange();
    }
}
