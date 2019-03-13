package dfsx.com.videodemo.adapter;

import android.graphics.Rect;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import com.dfsx.core.common.Util.PixelUtil;

public class ProjectRecyclerItemDecoration extends RecyclerView.ItemDecoration {

    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
        super.getItemOffsets(outRect, view, parent, state);
        int top = PixelUtil.dp2px(parent.getContext(), 10);
        int position = parent.getChildAdapterPosition(view);
        int left = 0;
        int right = 0;
        int bottom = 0;
        if (position % 2 == 0) {//左边
            left = PixelUtil.dp2px(parent.getContext(), 19);
            right = PixelUtil.dp2px(parent.getContext(), 6);
        } else {
            left = PixelUtil.dp2px(parent.getContext(), 6);
            right = PixelUtil.dp2px(parent.getContext(), 19);
        }
        outRect.set(left, top, right, bottom);
    }
}
