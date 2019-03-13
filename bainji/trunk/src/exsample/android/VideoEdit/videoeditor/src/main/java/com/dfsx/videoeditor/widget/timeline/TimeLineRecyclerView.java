package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.support.annotation.Nullable;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

public class TimeLineRecyclerView extends RecyclerView {
    private VideoSourceSelectedDecoration selectedDecoration;

    public TimeLineRecyclerView(Context context) {
        super(context);
    }

    public TimeLineRecyclerView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public TimeLineRecyclerView(Context context, @Nullable AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    public void addItemDecoration(ItemDecoration decor, int index) {
        super.addItemDecoration(decor, index);
        if (decor instanceof VideoSourceSelectedDecoration) {
            selectedDecoration = (VideoSourceSelectedDecoration) decor;
        }
    }

    private int latestPointX;
    private int latestPointY;
    boolean isDrag = false;
    boolean isDragLeft = false;
    boolean isDragRight = false;

    @Override
    public boolean onInterceptTouchEvent(MotionEvent e) {
        if (MotionEvent.ACTION_DOWN == e.getAction()) {
            int x = (int) e.getX();
            int y = (int) e.getY();
            latestPointX = x;
            latestPointY = y;
            isDragLeft = selectedDecoration.isInLeftRect(x, y);
            isDragRight = selectedDecoration.inRightRect(x, y);
            isDrag = isDragLeft || isDragRight;
            if (isDrag) {
                selectedDecoration.onDragStart();
            }
        } else if (e.getAction() == MotionEvent.ACTION_CANCEL ||
                e.getAction() == MotionEvent.ACTION_UP) {
            if (isDrag) {
                if (getAdapter() != null) {
                    getAdapter().notifyDataSetChanged();
                    Log.e("TAG", "drag onInterceptTouchEvent -------- notify");
                }
            }
            isDragLeft = false;
            isDragRight = false;
            isDrag = false;
        }
        return super.onInterceptTouchEvent(e);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        if (selectedDecoration != null && selectedDecoration.getSelectedItemView() != null) {
            int action = e.getAction();
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                    int x = (int) e.getX();
                    int y = (int) e.getY();
                    latestPointX = x;
                    latestPointY = y;
                    isDragLeft = selectedDecoration.isInLeftRect(x, y);
                    isDragRight = selectedDecoration.inRightRect(x, y);
                    isDrag = isDragLeft || isDragRight;
                    if (isDrag) {
                        selectedDecoration.onDragStart();
                    }
                    Log.e("TAG", "isDrag ==== " + isDrag);
                    break;
                case MotionEvent.ACTION_MOVE:
                    if (isDrag) {
                        int mx = (int) e.getX();
                        int my = (int) e.getY();
                        int dx = mx - latestPointX;
                        int dy = my - latestPointY;
                        latestPointX = mx;
                        latestPointY = my;
                        Log.e("TAG", "ACTION_MOVE == " + dx);
                        if (Math.abs(dx) > Math.abs(dy)) {
                            if (isDragLeft) {
                                selectedDecoration.leftDragScroll(dx);
                            }
                            if (isDragRight) {
                                selectedDecoration.rightDragScroll(dx);
                            }
                        }
                    }
                    break;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    if (isDrag) {
                        selectedDecoration.onDragScrollEnd();
                    }
                    break;
            }
            if (isDragRight) {//左边同步滚动,右边不要让recyclerView滚动了
                return isDragRight;
            }
        }
        return super.onTouchEvent(e);
    }
}
