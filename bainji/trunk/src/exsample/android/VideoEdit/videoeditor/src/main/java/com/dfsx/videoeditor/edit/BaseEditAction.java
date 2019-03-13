package com.dfsx.videoeditor.edit;

import android.content.Context;

import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;

public abstract class BaseEditAction implements IEditAction {

    protected Context context;
    protected ITimeLineUI timeLineUI;
    private boolean isCouldBackOrUnBack = true;

    public BaseEditAction(Context context, ITimeLineUI timeLineUI) {
        this.context = context;
        this.timeLineUI = timeLineUI;
    }

    @Override
    public boolean isCouldBackOrUnBack() {
        return isCouldBackOrUnBack;
    }

    public void setCouldBackOrUnBack(boolean couldBackOrUnBack) {
        isCouldBackOrUnBack = couldBackOrUnBack;
    }
}
