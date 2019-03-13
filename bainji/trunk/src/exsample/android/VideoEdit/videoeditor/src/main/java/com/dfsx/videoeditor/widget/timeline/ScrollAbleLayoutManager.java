package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.support.v7.widget.LinearLayoutManager;
import android.util.AttributeSet;

public class ScrollAbleLayoutManager extends LinearLayoutManager {

    private boolean canScroll = true;

    public ScrollAbleLayoutManager(Context context) {
        super(context);
    }

    public ScrollAbleLayoutManager(Context context, int orientation, boolean reverseLayout) {
        super(context, orientation, reverseLayout);
    }

    public ScrollAbleLayoutManager(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }


    @Override
    public boolean canScrollHorizontally() {
        return canScroll && super.canScrollHorizontally();
    }

    @Override
    public boolean canScrollVertically() {
        return canScroll && super.canScrollVertically();
    }

    public void setCanScroll(boolean canScroll) {
        this.canScroll = canScroll;
    }
}
