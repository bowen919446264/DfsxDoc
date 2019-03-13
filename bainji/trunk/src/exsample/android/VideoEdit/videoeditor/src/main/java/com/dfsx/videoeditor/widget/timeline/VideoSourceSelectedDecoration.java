package com.dfsx.videoeditor.widget.timeline;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.dfsx.videoeditor.R;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

public class VideoSourceSelectedDecoration extends RecyclerView.ItemDecoration {

    private Rect leftRect;
    private Rect rightRect;
    private Rect topRect;
    private Rect bottomRect;
    private Paint rectPaint;

    private int leftWidth;
    private int rightWidth;
    private int topHeight;
    private int bottomHeight;

    private View selectedItemView;


    public VideoSourceSelectedDecoration(int leftWidth, int rghtWidth, int topHeight, int bottomHeight, int color) {
        leftRect = new Rect();
        rightRect = new Rect();
        topRect = new Rect();
        bottomRect = new Rect();

        rectPaint = new Paint();
        rectPaint.setStyle(Paint.Style.FILL);
        rectPaint.setColor(color);

        this.leftWidth = leftWidth;
        this.rightWidth = rghtWidth;
        this.topHeight = topHeight;
        this.bottomHeight = bottomHeight;
    }

    @Override
    public void onDrawOver(Canvas c, RecyclerView parent, RecyclerView.State state) {
        super.onDrawOver(c, parent, state);
        selectedItemView = null;
        int childCount = parent.getChildCount();
        RecyclerView.Adapter adapter = parent.getAdapter();
        if (childCount <= 0 || adapter.getItemCount() <= 0 && adapter instanceof TimeLineRecyclerAdapter) {
            return;
        }
        for (int i = 0; i < childCount; i++) {
            View itemView = parent.getChildAt(i);
            int position = parent.getChildAdapterPosition(itemView);
            if (position == RecyclerView.NO_POSITION) {
                continue;
            }
            TimeLineRecyclerAdapter recyclerAdapter = (TimeLineRecyclerAdapter) adapter;
            int pos = position - recyclerAdapter.getHeaderViewCount();
            boolean is = recyclerAdapter.getData() != null && pos >= 0 && recyclerAdapter.getData().size() > pos &&
                    recyclerAdapter.getData().get(pos) instanceof TimeLineVideoSource &&
                    ((TimeLineVideoSource) recyclerAdapter.getData().get(pos)).getSelectedObject().isSelected();
            if (is) {
                selectedItemView = itemView;
                int left = itemView.getLeft();
                int top = itemView.getTop();
                int right = itemView.getRight();
                int bottom = itemView.getBottom();
                leftRect.set(left - leftWidth, top - topHeight, left, bottom + bottomHeight);
                rightRect.set(right, top - topHeight, right + rightWidth, bottom + bottomHeight);
                topRect.set(left, top - topHeight, right, top);
                bottomRect.set(left, bottom, right, bottom + bottomHeight);

                c.drawRect(leftRect, rectPaint);
                c.drawRect(rightRect, rectPaint);
                c.drawRect(topRect, rectPaint);
                c.drawRect(bottomRect, rectPaint);
            }
        }
    }


    public boolean isInLeftRect(int x, int y) {
        return leftRect.contains(x, y);
    }

    public boolean inRightRect(int x, int y) {

        return rightRect.contains(x, y);
    }

    public void onDragStart() {
        if (selectedItemView != null) {
            ImageListView imageListView = (ImageListView) selectedItemView.findViewById(R.id.frame_image_list_view);
            imageListView.dragStart();
        }
    }

    /**
     * 左边拖动
     *
     * @param dx
     * @return
     */
    public int leftDragScroll(int dx) {
        if (selectedItemView != null) {
            ImageListView imageListView = (ImageListView) selectedItemView.findViewById(R.id.frame_image_list_view);
            return imageListView.leftDrag(dx);
        }
        return 0;
    }


    /**
     * 右边拖动
     *
     * @param dx 返回滚动的距离
     * @return
     */
    public int rightDragScroll(int dx) {
        if (selectedItemView != null) {
            ImageListView imageListView = (ImageListView) selectedItemView.findViewById(R.id.frame_image_list_view);
            return imageListView.rightDrag(dx);
        }
        return 0;
    }

    public void onDragScrollEnd() {
        if (selectedItemView != null) {
            ImageListView imageListView = (ImageListView) selectedItemView.findViewById(R.id.frame_image_list_view);
            imageListView.onDragEnd();
        }
    }

    public View getSelectedItemView() {
        return selectedItemView;
    }
}
