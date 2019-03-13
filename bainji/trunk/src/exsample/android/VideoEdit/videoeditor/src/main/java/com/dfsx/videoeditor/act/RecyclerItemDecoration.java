package com.dfsx.videoeditor.act;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;
import com.dfsx.videoeditor.adapter.SourceTimeRecyclerAdapter;
import com.dfsx.videoeditor.bean.FrameAdapterData;
import com.dfsx.videoeditor.bean.IAdapterItem;
import com.dfsx.videoeditor.util.EditConstants;

public class RecyclerItemDecoration extends RecyclerView.ItemDecoration {

    private int margin;

    private Paint paint;

    private Rect cacheRect;

    private OnMarginLineListener onMarginLineListener;

    public RecyclerItemDecoration(int margin) {
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
                if (onMarginLineListener != null) {
                    onMarginLineListener.onMarginLineRangeAction(rect, position);
                }
            }
        }
    }

    public int getScrollDxByTime(RecyclerView parent, SourceTimeRecyclerAdapter adapter, long timeLineTime) {
        int childCount = parent.getChildCount();
        for (int i = 0; i < childCount; i++) {
            View itemView = parent.getChildAt(i);
            int position = parent.getChildAdapterPosition(itemView);
            if (position == RecyclerView.NO_POSITION) {
                continue;
            }
            int size = adapter.getData().size();
            if (position - 1 < size && position - 1 >= 0) {
                IAdapterItem item = adapter.getData().get(position - 1);
                if (item instanceof FrameAdapterData) {
                    FrameAdapterData frameAdapterData = (FrameAdapterData) item;
                    if (timeLineTime >= frameAdapterData.getAdapterStartTime() &&
                            timeLineTime < frameAdapterData.getAdapterStartTime() + frameAdapterData.getItemData().durationTime) {
                        long timeD = timeLineTime - frameAdapterData.getAdapterStartTime();
                        double leftDx = itemView.getLeft() + timeD / 1000.0 * EditConstants.ONE_SECOND_VIEW_WIDTH;
                        Log.e("TAFF", "leftDx == " + leftDx);
                        Log.e("TAFF", "left margin === " + margin);
                        return (int) Math.round(leftDx - margin);
                    }
                }
            }
        }
        return 0;
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

    public void setOnMarginLineListener(OnMarginLineListener onMarginLineListener) {
        this.onMarginLineListener = onMarginLineListener;
    }

    public interface OnMarginLineListener {
        void onMarginLineRangeAction(Rect rect, int adapterPos);
    }
}
