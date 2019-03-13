package dfsx.com.videodemo.adapter;

import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import dfsx.com.videodemo.R;

public class EditMediaRecyclerAdapter extends SelectorRecyclerAdapter {

    private int itemWidth;

    public EditMediaRecyclerAdapter(int width) {
        this.itemWidth = width;
    }

    @Override
    public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {
        View view = holder.getView(R.id.item_thumb_container);
        ViewGroup.LayoutParams params = view.getLayoutParams();
        if (params != null) {
            if (itemWidth != params.width) {
                params.width = itemWidth;
                view.setLayoutParams(params);
            }
        } else {
            params = new LinearLayout.LayoutParams(itemWidth, itemWidth);
            view.setLayoutParams(params);
        }
        super.onBindViewHolder(holder, position);
    }
}
