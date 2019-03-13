package dfsx.com.videodemo.adapter;

import android.content.Context;
import android.graphics.*;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;

public class SelectedDecoration extends RecyclerView.ItemDecoration {

    private int selectedColor;
    private Bitmap selectedCenterBitmap;

    private Rect bitmapRect;
    private Rect bitmapPosRect;
    private Paint bitmapPaint;

    private Rect bkgRect;
    private Paint bkgPaint;


    public SelectedDecoration(Context context, int selectColor, int selectorBitmapRes) {
        selectedColor = selectColor;
        bkgRect = new Rect();
        bkgPaint = new Paint();
        bkgPaint.setStyle(Paint.Style.FILL);
        bkgPaint.setColor(selectColor);
        selectedCenterBitmap = BitmapFactory.decodeResource(context.getResources(), selectorBitmapRes);
        bitmapRect = new Rect();
        bitmapPosRect = new Rect();
        bitmapRect.set(0, 0, selectedCenterBitmap.getWidth(), selectedCenterBitmap.getHeight());
        bitmapPaint = new Paint();
    }

    @Override
    public void onDrawOver(Canvas c, RecyclerView parent, RecyclerView.State state) {
        super.onDrawOver(c, parent, state);
        int childCount = parent.getChildCount();
        RecyclerView.Adapter recyleradapter = parent.getAdapter();
        if (childCount <= 0 || recyleradapter.getItemCount() <= 0 && recyleradapter instanceof BaseRecyclerViewDataAdapter) {
            return;
        }
        BaseRecyclerViewDataAdapter<ISelector> adapter = (BaseRecyclerViewDataAdapter) recyleradapter;
        for (int i = 0; i < childCount; i++) {
            View itemView = parent.getChildAt(i);
            int position = parent.getChildAdapterPosition(itemView);
            if (position == RecyclerView.NO_POSITION) {
                continue;
            }
            ISelector iSelector = adapter.getData().get(position);
            if (iSelector.isSelected()) {

                bkgRect.set(itemView.getLeft(), itemView.getTop(), itemView.getRight(), itemView.getBottom());
                c.drawRect(bkgRect, bkgPaint);

                if (selectedCenterBitmap != null) {
                    int left = itemView.getLeft() + itemView.getWidth() / 2 - selectedCenterBitmap.getWidth() / 2;
                    int top = itemView.getTop() + itemView.getHeight() / 2 - selectedCenterBitmap.getHeight() / 2;
                    int right = left + bitmapRect.width();
                    int bottom = top + bitmapRect.height();
                    bitmapPosRect.set(left, top, right, bottom);
                    c.drawBitmap(selectedCenterBitmap, bitmapRect, bitmapPosRect, bitmapPaint);
                }
            }
        }
    }
}
