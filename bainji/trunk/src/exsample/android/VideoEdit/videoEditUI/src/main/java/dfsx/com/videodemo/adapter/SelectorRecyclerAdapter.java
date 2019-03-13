package dfsx.com.videodemo.adapter;

import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import com.bumptech.glide.Glide;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import com.dfsx.videoeditor.widget.timeline.TimeLineStringUtil;
import dfsx.com.videodemo.R;

public class SelectorRecyclerAdapter extends BaseRecyclerViewDataAdapter<ISelector> {
    @Override
    public BaseRecyclerViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new BaseRecyclerViewHolder(R.layout.adapter_selector_layout, parent, viewType);
    }

    @Override
    public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {
        ISelector selector = list.get(position);
        ImageView imageThumbView = holder.getView(R.id.item_thumb_image);
        TextView tvLength = holder.getView(R.id.item_tv_selector_length);

        Glide.with(imageThumbView.getContext()).load(selector.getSelectorThumb())
                .diskCacheStrategy(DiskCacheStrategy.RESULT)
                .placeholder(R.drawable.glide_default_image)
                .error(R.drawable.glide_default_image)
                .centerCrop()
                .dontAnimate().into(imageThumbView);
        tvLength.setText(TimeLineStringUtil.parseStringSecondTime(selector.getSelectorLength()));

        imageThumbView.setTag(imageThumbView.getId(), selector);
        imageThumbView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    ISelector iSelector = (ISelector) v.getTag(v.getId());
                    if (selectorClickListener != null) {
                        selectorClickListener.onItemClick(v, iSelector);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        });
    }

    private OnSelectorClickListener selectorClickListener;

    public void setOnSelectorClickListener(OnSelectorClickListener listener) {
        this.selectorClickListener = listener;
    }

    public interface OnSelectorClickListener {
        void onItemClick(View v, ISelector iSelector);
    }
}
