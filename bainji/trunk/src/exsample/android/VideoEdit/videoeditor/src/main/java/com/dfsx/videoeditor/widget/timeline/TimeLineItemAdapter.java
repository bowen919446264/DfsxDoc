package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.content.res.Resources;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import com.dfsx.videoeditor.R;

/**
 * 设置开始位置和结束位置
 */
public abstract class TimeLineItemAdapter extends BaseRecyclerViewDataAdapter<ITimeLineItem> {

    public static final int HEADER_TYPE = 10001;

    protected Context context;

    private int headerViewWidth;

    private int screenWidth;

    private int defaultWidth;

    public TimeLineItemAdapter(Context context) {
        this.context = context;
        Resources resources = context.getResources();
        DisplayMetrics dm = resources.getDisplayMetrics();
        float density = dm.density;
        int width = dm.widthPixels;
        screenWidth = width;
        defaultWidth = (int) ((width - resources.getDimension(R.dimen.time_line_ver_line_width)) / 2);
    }

    public void setHeaderViewWidth(int width) {
        this.headerViewWidth = width;
    }

    @Override
    public BaseRecyclerViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        BaseRecyclerViewHolder holder = new BaseRecyclerViewHolder(getViewTypeRes(viewType), parent, viewType);
        return holder;
    }

    @Override
    public int getItemCount() {
        return super.getItemCount() + 2;
    }

    public int getHeaderViewCount() {
        return 1;
    }

    public int getFooterCount() {
        return 1;
    }

    private int getViewTypeRes(int viewType) {
        if (viewType == HEADER_TYPE) {
            return R.layout.adapter_header_layout;
        }
        ITimeLineItem.ItemType type = ITimeLineItem.ItemType.findType(viewType);
        return getItemTypeLayout(type);
    }

    /**
     * 获取类型的布局文件
     *
     * @param type
     * @return
     */
    public abstract int getItemTypeLayout(ITimeLineItem.ItemType type);

    @Override
    public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {
        if (position == 0 || position == getItemCount() - 1) {//首尾单独处理
            setHeaderViewData(holder, position);
            return;
        }
        position = position - 1;
        ITimeLineItem.ItemType itemType = list.get(position).getItemType();
        onBindItemViewData(itemType, holder, position);
    }

    /**
     * @param itemType 当前的item类型
     * @param holder
     * @param position list中的下标
     */
    public abstract void onBindItemViewData(ITimeLineItem.ItemType itemType, BaseRecyclerViewHolder holder, int position);

    private void setHeaderViewData(BaseRecyclerViewHolder holder, int position) {
        View emptyView = holder.getView(R.id.item_empty_view);
        if (headerViewWidth == 0) {
            headerViewWidth = defaultWidth;
        }
        setRelativeLayoutChildViewWidth(emptyView, headerViewWidth);
    }

    private void setRelativeLayoutChildViewWidth(View view, int width) {
        ViewGroup.LayoutParams params = view.getLayoutParams();
        if (params == null) {
            params = new RelativeLayout.LayoutParams(width, PixelUtil.dp2px(context, 70));
        } else {
            params.width = width;
        }
        view.setLayoutParams(params);
    }

    @Override
    public int getItemViewType(int position) {
        if (position == 0 || position == getItemCount() - 1) {
            return HEADER_TYPE;
        }
        position -= 1;
        return list != null ? list.get(position).getItemType().getTypeCount() : 0;
    }
}
