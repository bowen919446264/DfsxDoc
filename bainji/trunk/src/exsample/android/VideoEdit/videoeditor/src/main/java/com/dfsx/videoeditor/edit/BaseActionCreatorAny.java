package com.dfsx.videoeditor.edit;

/**
 * 同种类型的多参数传递
 *
 * @param <P>
 */
public abstract class BaseActionCreatorAny<P> implements IActionCreator {
    private P[] params;

    public BaseActionCreatorAny(P... params) {
        this.params = params;
    }

    @Override
    public IEditAction createAction() {
        return createAction(params);
    }

    public abstract IEditAction createAction(P... params);
}
