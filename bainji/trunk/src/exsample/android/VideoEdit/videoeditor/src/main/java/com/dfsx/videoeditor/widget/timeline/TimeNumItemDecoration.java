package com.dfsx.videoeditor.widget.timeline;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.LinearLayout;

/**
 * 记录滚动的时间数据
 */
public class TimeNumItemDecoration extends RecyclerView.ItemDecoration {

    private int margin;

    private Paint paint;

    private Rect cacheRect;

    private OnTimeLineListener onTimeLineListener;

    public TimeNumItemDecoration(int margin) {
        this.margin = margin;

        cacheRect = new Rect();
        paint = new Paint();
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(4);
        paint.setColor(Color.YELLOW);
    }

    public void setMargin(int margin) {
        this.margin = margin;
    }

    public int getMargin() {
        return margin;
    }

    @Override
    public void onDrawOver(Canvas c, RecyclerView parent, RecyclerView.State state) {
        super.onDrawOver(c, parent, state);
        int childCount = parent.getChildCount();
        RecyclerView.Adapter adapter = parent.getAdapter();
        if (childCount <= 0 || adapter.getItemCount() <= 0) {
            return;
        }
        for (int i = 0; i < childCount; i++) {
            View itemView = parent.getChildAt(i);
            int position = parent.getChildAdapterPosition(itemView);
            if (position == RecyclerView.NO_POSITION) {
                continue;
            }
            Rect rect = getRangeMargin(itemView, getOrientation(parent));
            if (rect != null) {
                //                Log.e("TAG", "onDrawOver rect == " + rect.toString());
                c.drawRect(rect, paint);
                if (onTimeLineListener != null) {
                    onTimeLineListener.onTimeLineRangeAction(itemView, rect, position);
                }
            }
        }
    }

    private Rect getRangeMargin(View view, int orientation) {
        int startSet, endSet;
        if (orientation == LinearLayout.VERTICAL) {
            startSet = view.getTop();
            endSet = view.getBottom();
        } else {
            startSet = view.getLeft();
            endSet = view.getRight();
        }
        boolean hasRange = startSet <= margin && margin < endSet;
        if (hasRange) {
            cacheRect.left = view.getLeft();
            cacheRect.top = view.getTop();
            cacheRect.right = orientation == LinearLayout.VERTICAL ? view.getRight() : margin;
            cacheRect.bottom = orientation == LinearLayout.VERTICAL ? margin : view.getBottom();
            return cacheRect;
        }

        return null;
    }

    public int getOrientation(RecyclerView recyclerView) {
        RecyclerView.LayoutManager layoutManager = recyclerView.getLayoutManager();
        return ((LinearLayoutManager) layoutManager).getOrientation();
    }

    public void setOnTimeLineListener(OnTimeLineListener listener) {
        this.onTimeLineListener = listener;
    }

    public interface OnTimeLineListener {
        /**
         * Item view 矩形区域
         *
         * @param rect
         * @param adapterPos
         */
        void onTimeLineRangeAction(View itemView, Rect rect, int adapterPos);
    }
}

