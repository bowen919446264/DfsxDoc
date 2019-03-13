package dfsx.com.videodemo.solution1;

import rx.Observable;

public abstract class BaseObservableOnSubscribe<T> implements Observable.OnSubscribe<T> {
    protected Object[] params;

    public BaseObservableOnSubscribe(Object... params) {
        this.params = params;
    }
}
