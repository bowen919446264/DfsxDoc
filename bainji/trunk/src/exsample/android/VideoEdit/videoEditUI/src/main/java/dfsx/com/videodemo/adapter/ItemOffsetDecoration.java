package dfsx.com.videodemo.adapter;

import android.content.Context;
import android.graphics.Rect;
import android.support.annotation.DimenRes;
import android.support.annotation.NonNull;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;

public class ItemOffsetDecoration extends RecyclerView.ItemDecoration {

    private int mItemOffset;
    private Rect offSetRect;
    private int startOffet;

    public ItemOffsetDecoration(int itemOffset) {
        mItemOffset = itemOffset;
    }

    public ItemOffsetDecoration(int left, int top, int right, int bottom) {
        mItemOffset = -1;
        offSetRect = new Rect(left, top, right, bottom);
    }

    public ItemOffsetDecoration(@NonNull Context context, @DimenRes int itemOffsetId) {
        this(context.getResources().getDimensionPixelSize(itemOffsetId));
    }

    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent,
                               RecyclerView.State state) {
        super.getItemOffsets(outRect, view, parent, state);
        boolean isHorizontal = ((LinearLayoutManager) parent.getLayoutManager())
                .getOrientation() == LinearLayoutManager.HORIZONTAL;
        int startOff = parent.getChildAdapterPosition(view) != 0 ? 0 : startOffet;
        if (isHorizontal) {
            if (mItemOffset != -1) {
                outRect.set(mItemOffset + startOff, mItemOffset, mItemOffset, mItemOffset);
            } else {
                outRect.set(offSetRect.left + startOff, offSetRect.top, offSetRect.right, offSetRect.bottom);
            }
        } else {
            if (mItemOffset != -1) {
                outRect.set(mItemOffset, mItemOffset + startOff, mItemOffset, mItemOffset);
            } else {
                outRect.set(offSetRect.left, offSetRect.top + startOff, offSetRect.right, offSetRect.bottom);
            }
        }
    }

    public void setStartOffet(int startOffet) {
        this.startOffet = startOffet;
    }
}