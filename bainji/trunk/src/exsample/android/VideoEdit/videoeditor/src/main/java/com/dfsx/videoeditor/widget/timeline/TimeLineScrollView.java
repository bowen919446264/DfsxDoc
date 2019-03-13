package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.graphics.Color;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.TextView;
import com.dfsx.videoeditor.R;

public class TimeLineScrollView extends FrameLayout implements View.OnLayoutChangeListener {

    public static final int LINE_COLOR = Color.WHITE;

    protected Context context;

    private View timeIndexLine;
    protected RecyclerView timeRecyclerView;
    private TextView timeLineTextView;
    private TextView timeAllTextView;

    protected int timeLineLeftMargin;

    public TimeLineScrollView(Context context) {
        super(context);
        this.context = context;
        init();
    }

    public TimeLineScrollView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.context = context;
        init();
    }

    public TimeLineScrollView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.context = context;
        init();
    }

    protected void init() {
        LayoutInflater.from(context).inflate(R.layout.time_line_view, this);
        timeIndexLine = findViewById(R.id.item_time_index_line);
        timeRecyclerView = (RecyclerView) findViewById(R.id.item_hor_recycler);
        timeLineTextView = (TextView) findViewById(R.id.time_tv);
        timeAllTextView = (TextView) findViewById(R.id.all_time_tv);
        timeRecyclerView.setLayoutManager(new ScrollAbleLayoutManager(context,
                LinearLayoutManager.HORIZONTAL, false));
        timeIndexLine.addOnLayoutChangeListener(this);
    }

    public void setRecyclerViewAdapter(RecyclerView.Adapter adapter) {
        timeRecyclerView.setAdapter(adapter);
    }


    public String parseStringTime(long time) {
        return TimeLineStringUtil.parseStringTime(time);
    }

    public void setTimeLineText(CharSequence sequence) {
        timeLineTextView.setText(sequence);
    }

    public void setAllTimeText(CharSequence sequence) {
        timeAllTextView.setText(sequence);
    }

    public void setTimeLineText(long time) {
        timeLineTextView.setText(parseStringTime(time));
    }

    public void setAllTimeText(long time) {
        timeAllTextView.setText(parseStringTime(time));
    }

    protected void onLineViewLayoutReset(int leftMargin) {

    }

    @Override
    public void onLayoutChange(View v, int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom) {
        if (v == timeIndexLine) {
            if (left != timeLineLeftMargin) {
                timeLineLeftMargin = left;
                onLineViewLayoutReset(left);
            }
        }
    }
}
