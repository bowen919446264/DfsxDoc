package com.dfsx.videoeditor.widget.timeline;

import android.view.View;

public interface ITimeLineEventListener {
    void onAddSourceClick(View v, ITimeLineItem item);

    void onThumbImageClick(View v, ITimeLineItem item);

    /**
     * @param v
     * @param timeLineItem
     * @param time
     * @param recyclerViewState RecyclerView.SCROLL_STATE_IDLE, ecyclerView.SCROLL_STATE_DRAGGING} or RecyclerView.SCROLL_STATE_SETTLING
     */
    void onTimeLineTimeChangeListener(View v, ITimeLineItem timeLineItem, long time, int recyclerViewState);
}
