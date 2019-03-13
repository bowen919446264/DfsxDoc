package dfsx.com.videodemo.solution1;

import rx.functions.Action1;

public abstract class BaseParamsAction<T> implements Action1<T> {
    protected Object[] params;

    public BaseParamsAction(Object... params) {

    }
}
