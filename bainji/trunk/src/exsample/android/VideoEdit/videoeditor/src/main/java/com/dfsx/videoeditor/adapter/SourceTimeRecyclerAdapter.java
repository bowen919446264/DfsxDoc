package com.dfsx.videoeditor.adapter;

import android.content.Context;
import android.graphics.Color;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import com.bumptech.glide.Glide;
import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import com.dfsx.videoeditor.R;
import com.dfsx.videoeditor.bean.FrameAdapterData;
import com.dfsx.videoeditor.bean.IAdapterItem;
import com.dfsx.videoeditor.bean.SplitData;
import com.dfsx.videoeditor.video.FrameThumbInfo;
import com.dfsx.videoeditor.video.VideoSource;

import java.util.ArrayList;
import java.util.List;

public class SourceTimeRecyclerAdapter extends BaseRecyclerViewDataAdapter<IAdapterItem> {

    public static final int HEADER_TYPE = 10001;

    private Context context;
    private List<VideoSource> videoSources;

    private int headerViewWidth;

    public SourceTimeRecyclerAdapter(Context context) {
        this.context = context;
    }

    public void setHeaderViewWidth(int width) {
        this.headerViewWidth = width;
    }

    public void setVideoSourceData(List<VideoSource> data, boolean isAdd) {
        if (!isAdd || videoSources == null) {
            calculateSourceTimeLineStartTime(0, data);
            videoSources = data;
        } else {
            long startOff = videoSources.get(videoSources.size() - 1).timeLineStartTime
                    + videoSources.get(videoSources.size() - 1).sourceDuration;
            calculateSourceTimeLineStartTime(startOff, data);
            videoSources.addAll(data);
        }
        update(createAdapterData(data), isAdd);
    }

    private void calculateSourceTimeLineStartTime(long startOffTime, List<VideoSource> data) {
        if (data != null) {
            long startTime = startOffTime;
            for (VideoSource source : data) {
                source.timeLineStartTime = startTime;
                startTime += source.sourceDuration;
            }
        }
    }

    private List<IAdapterItem> createAdapterData(List<VideoSource> data) {
        List<IAdapterItem> itemList = new ArrayList<>();
        if (data != null) {
            for (int i = 0; i < data.size(); i++) {
                VideoSource source = data.get(i);
                long startOffSet = source.timeLineStartTime;
                itemList.addAll(createSourceItemList(startOffSet, source));
                if (i != data.size() - 1) {
                    itemList.add(new SplitData());
                }
            }
        }

        return itemList;
    }

    private List<IAdapterItem> createSourceItemList(long startTime, VideoSource sourceData) {
        List<IAdapterItem> list = new ArrayList<>();
        if (sourceData != null && sourceData.frameThumbInfoList != null) {
            long start = startTime;
            for (FrameThumbInfo imageInfo : sourceData.frameThumbInfoList) {
                list.add(new FrameAdapterData(imageInfo, sourceData, start));
                start += imageInfo.durationTime;
            }
        }
        return list;
    }

    public List<VideoSource> getVideoSources() {
        return videoSources;
    }


    @Override
    public BaseRecyclerViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        BaseRecyclerViewHolder holder = new BaseRecyclerViewHolder(getViewTypeRes(viewType), parent, viewType);
        return holder;
    }

    @Override
    public int getItemCount() {
        if (super.getItemCount() > 0) {
            return super.getItemCount() + 2;
        }
        return super.getItemCount();
    }

    private int getViewTypeRes(int viewType) {
        if (viewType == HEADER_TYPE) {
            return R.layout.adapter_header_layout;
        }
        IAdapterItem.ItemType type = IAdapterItem.ItemType.findType(viewType);
        int layout = 0;
        switch (type) {
            case TYPE_FRAME:
                layout = R.layout.adapter_video_frame;
                break;
            case TYPE_SOURCE_SPLIT:
                layout = R.layout.adapter_source_split;
                break;
        }
        return layout;
    }

    @Override
    public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {
        if (position == 0 || position == getItemCount() - 1) {//首尾单独处理
            setHeaderViewData(holder, position);
            return;
        }
        position = position - 1;
        IAdapterItem.ItemType itemType = list.get(position).getItemType();
        switch (itemType) {
            case TYPE_FRAME:
                setVideoFrameData(holder, position);
                break;
            case TYPE_SOURCE_SPLIT:
                setSourceSplitData(holder, position);
                break;
        }
    }

    private void setHeaderViewData(BaseRecyclerViewHolder holder, int position) {
        View emptyView = holder.getView(R.id.item_empty_view);
        setViewWidth(emptyView, headerViewWidth);
    }

    private void setSourceSplitData(BaseRecyclerViewHolder holder, int position) {
        //nothing
    }

    private void setVideoFrameData(BaseRecyclerViewHolder holder, int position) {
        View frameBkgView = holder.getView(R.id.frame_back_ground);
        ImageView imageView = holder.getView(R.id.frame_image);
        FrameAdapterData item = (FrameAdapterData) list.get(position);
        FrameThumbInfo frameInfo = item.getItemData();
        String url = frameInfo.imagePath;
        setViewWidth(imageView, frameInfo.viewWidth);
        Glide.with(context)
                .load(url)
                .asBitmap().into(imageView);
        VideoSource source = (VideoSource) item.getTag();
        int res = source.tagSelected ? Color.YELLOW : Color.TRANSPARENT;
        frameBkgView.setBackgroundColor(res);
        frameBkgView.setTag(item);
        frameBkgView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                FrameAdapterData frameAdapterData = (FrameAdapterData) v.getTag();
                ((VideoSource) frameAdapterData.getTag()).tagSelected = !((VideoSource) frameAdapterData.getTag())
                        .tagSelected;
                notifyDataSetChanged();
            }
        });
    }

    private void setViewWidth(View view, int width) {
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
