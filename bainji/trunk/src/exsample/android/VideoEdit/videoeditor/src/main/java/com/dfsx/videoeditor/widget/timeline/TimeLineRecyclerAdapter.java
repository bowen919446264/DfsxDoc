package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import com.dfsx.videoeditor.R;
import com.dfsx.videoeditor.test.TimeLineVideo;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineEmptyTime;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import java.util.ArrayList;
import java.util.List;

public class TimeLineRecyclerAdapter extends TimeLineItemAdapter implements ImageListView.OnDragSizeChangeListener {

    private List<ITimeLineEventListener> eventListeners;
    private ISelectedObject latestSelectedObject;

    private long[] playerRange;

    public TimeLineRecyclerAdapter(Context context) {
        super(context);
        playerRange = new long[2];
        eventListeners = new ArrayList<>();
    }

    public void addTimeLineEventListener(ITimeLineEventListener listener) {
        eventListeners.add(listener);
    }

    public boolean removeTimeLineEventListener(ITimeLineEventListener listener) {
        return eventListeners.remove(listener);
    }

    @Override
    public int getItemTypeLayout(ITimeLineItem.ItemType type) {
        int layout = 0;
        switch (type) {
            case TYPE_EMPTY_TIME:
                layout = R.layout.adapter_empty_time_layout;
                break;
            case TYPE_THUMB:
                layout = R.layout.adapter_time_line_thumb_layout;
                break;
            case TYPE_SPLIT:
                layout = R.layout.adapter_time_line_splite_layout;
                break;
        }
        return layout;
    }

    @Override
    public void onBindItemViewData(ITimeLineItem.ItemType itemType, BaseRecyclerViewHolder holder, int position) {
        switch (itemType) {
            case TYPE_EMPTY_TIME:
                setEmptyTimeData(holder, position);
                break;
            case TYPE_THUMB:
                setThumbTypeData(holder, position);
                break;
            case TYPE_SPLIT:
                setSplitTypeData(holder, position);
                break;
        }
    }

    private void setEmptyTimeData(BaseRecyclerViewHolder holder, int position) {
        View emptyDataView = holder
                .getView(R.id.empty_data_time_view);
        ITimeLineItem<TimeLineEmptyTime> itemData = list.get(position);
        TimeLineEmptyTime emptyTime = itemData.getItemData();
        ViewGroup.LayoutParams params = emptyDataView.getLayoutParams();
        if (params == null) {
            params = new RecyclerView.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, R.dimen.time_line_height);
        }
        params.width = emptyTime.getViewWidth();
        emptyDataView.setLayoutParams(params);
    }

    private void setThumbTypeData(BaseRecyclerViewHolder holder, int position) {
        /*FrameLayout thumbSizeView = holder.getView(R.id.thumb_size_view);*/
        ImageListView videoFrameListView = holder.getView(R.id.frame_image_list_view);
        ITimeLineItem itemData = list.get(position);
        TimeLineVideo videoSource = null;
        int left = 0;
        int right = 0;
        List<ImageListView.IFrameImage> frameList = null;
        if (itemData instanceof TimeLineVideo) {
            videoSource = (TimeLineVideo) itemData;
            frameList = videoSource.getFrameList();
            if (videoSource.getPlayerSource().getPlayTimeRange() != null &&
                    videoSource.getPlayerSource().getPlayTimeRange().length == 2) {
                long sourceStartTime = videoSource.getPlayerSource().getSourceStartTime();
                long sourceDuration = videoSource.getPlayerSource().getSourceDuration();
                float onMsWidth = videoSource.getOneTimeMSWidthRatio();
                left = Math.round(onMsWidth * (videoSource.getPlayerSource().getPlayTimeRange()[0] -
                        sourceStartTime));
                right = Math.round(onMsWidth * (sourceStartTime + sourceDuration - videoSource.getPlayerSource()
                        .getPlayTimeRange()[1]));
            }
        }
        videoFrameListView.setUp(frameList, left, right);
        videoFrameListView.setTag(videoSource);
        videoFrameListView.setOnDragSizeChangeListener(this);
        videoFrameListView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    ITimeLineItem itemData = (ITimeLineItem) v.getTag();
                    setSelectedSource((TimeLineVideoSource) itemData);
                    for (ITimeLineEventListener lineEventListener : eventListeners) {
                        lineEventListener.onThumbImageClick(v, itemData);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private void setSelectedSource(TimeLineVideoSource itemData) {
        ISelectedObject selectedObject = itemData.getSelectedObject();
        if (latestSelectedObject != null && latestSelectedObject.isSelected()) {
            latestSelectedObject.setSelected(false);
        } else {
            if (selectedObject != null) {
                selectedObject.setSelected(!selectedObject.isSelected());
            }
            latestSelectedObject = selectedObject.isSelected() ? selectedObject : null;
        }
        notifyDataSetChanged();
    }

    /**
     * 清除选中
     */
    public void clearSelected() {
        if (latestSelectedObject != null) {
            latestSelectedObject.setSelected(false);
            latestSelectedObject = null;
            notifyDataSetChanged();
        }
    }

    public ISelectedObject getLatestSelectedObject() {
        return latestSelectedObject;
    }

    private void setSplitTypeData(BaseRecyclerViewHolder holder, int position) {
        ImageView addImageView = holder.getView(R.id.add_image_view);
        ITimeLineItem itemData = list.get(position);
        addImageView.setTag(addImageView.getId(), itemData);
        addImageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    ITimeLineItem itemData = (ITimeLineItem) v.getTag(v.getId());
                    for (ITimeLineEventListener lineEventListener : eventListeners) {
                        lineEventListener.onAddSourceClick(v, itemData);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        });
    }

    private ImageListView.OnDragSizeChangeListener imageListDragSizeChangeListener;

    public void setOnDragSizeChangeListener(ImageListView.OnDragSizeChangeListener listener) {
        this.imageListDragSizeChangeListener = listener;
    }

    @Override
    public void onDragStart(View v) {
        if (imageListDragSizeChangeListener != null) {
            imageListDragSizeChangeListener.onDragStart(v);
        }
    }

    @Override
    public void onDragSizeChange(View v, int viewWidth, int dWidth, int leftHidSize, int rightHideSize) {
        if (v.getTag() instanceof TimeLineVideoSource) {
            TimeLineVideoSource source = (TimeLineVideoSource) v.getTag();
            Log.e("TAG", "onDragSizeChange source == " + source.getPlayerSource().getSourceUrl());
            Log.e("TAG", "leftHidSize == " + leftHidSize);
            float oneMsW = source.getOneTimeMSWidthRatio();
            long duration = source.getTimeLineDuration();
            long startSourceTime = source.getPlayerSource().getSourceStartTime();
            long startCutTime = Math.round(leftHidSize / oneMsW);
            long dTime = Math.round(dWidth / oneMsW);
            long newDuration = duration + dTime;
            source.setTimeLineDuration(newDuration);
            playerRange[0] = startSourceTime + startCutTime;
            playerRange[1] = playerRange[0] + source.getTimeLineDuration();
            source.getPlayerSource().setPlayTimeRange(playerRange);
            int index = list.indexOf(source);
            if (index >= 0) {
                for (int i = index + 1; i < list.size(); i++) {
                    list.get(i).setTimeLineTimeOffSet(dTime);
                }
            }
        }
        if (imageListDragSizeChangeListener != null) {
            imageListDragSizeChangeListener.onDragSizeChange(v, viewWidth, dWidth, leftHidSize, rightHideSize);
        }
    }

    @Override
    public void onDragEnd(View v, ImageListView.DragPosition dragPos, int viewWidth, int leftAllHidSize, int rightAllHideSize) {
//        if (v.getTag() instanceof TimeLineVideoSource) {
//            //矫正视频时间为关键帧
//            TimeLineVideoSource source = (TimeLineVideoSource) v.getTag();
//            long[] timeRange = source.getPlayerSource().getPlayTimeRange();
//            if (timeRange != null && timeRange.length == 2) {
//                ExtractVideoInfoHelper videoInfoHelper = new ExtractVideoInfoHelper(source.getPlayerSource().getSourceUrl());
//                long startTime = videoInfoHelper.getKeyFrameTime(timeRange[0]);
//                long endTime = videoInfoHelper.getKeyFrameTime(timeRange[1]);
//                if (startTime != timeRange[0]) {
//                    Log.e("TAG", "矫正视频时间 ---- work------------");
//                    timeRange[0] = startTime;
//                }
//                if (endTime != timeRange[1]) {
//                    Log.e("TAG", "矫正视频时间 ---- work------------");
//                    timeRange[1] = endTime;
//                }
//                videoInfoHelper.release();
//            }
//        }
        if (imageListDragSizeChangeListener != null) {
            imageListDragSizeChangeListener.onDragEnd(v, dragPos, viewWidth, leftAllHidSize, rightAllHideSize);
        }
    }
}
