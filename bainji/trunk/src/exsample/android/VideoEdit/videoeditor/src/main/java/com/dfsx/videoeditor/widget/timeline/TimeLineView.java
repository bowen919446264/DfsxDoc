package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.view.View;

import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineSplitAdd;
import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class TimeLineView extends TimeLineScrollView implements ITimeLineUI {

    private TimeLineRecyclerAdapter adapter;
    private TimeNumItemDecoration timeNumItemDecoration;
    private VideoSourceSelectedDecoration selectedDecoration;
    private List<ITimeLineEventListener> eventListeners;

    private ITimeLineItem currentLineItem;
    private long currentLineTime;
    private ImageListView.OnDragSizeChangeListener dragSizeChangeListener;

    public TimeLineView(Context context) {
        super(context);
    }

    public TimeLineView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public TimeLineView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    protected void init() {
        super.init();
        adapter = new TimeLineRecyclerAdapter(context);
        timeRecyclerView.getRecycledViewPool()
                .setMaxRecycledViews(ITimeLineItem.ItemType.TYPE_THUMB.getTypeCount(), 5);
        setRecyclerViewAdapter(adapter);
        timeNumItemDecoration = new TimeNumItemDecoration(0);
        timeRecyclerView.addItemDecoration(timeNumItemDecoration);
        selectedDecoration = new VideoSourceSelectedDecoration(PixelUtil.dp2px(context, 12),
                PixelUtil.dp2px(context, 12),
                PixelUtil.dp2px(context, 4),
                PixelUtil.dp2px(context, 4), Color.parseColor("#ffc107"));
        timeRecyclerView.addItemDecoration(selectedDecoration);

        timeNumItemDecoration.setOnTimeLineListener(new TimeNumItemDecoration.OnTimeLineListener() {
            @Override
            public void onTimeLineRangeAction(View itemView, Rect rect, int adapterPos) {
                int listPos = adapterPos - adapter.getHeaderViewCount();
                if (adapter.getData() != null && listPos >= 0 && listPos < adapter.getData().size()) {
                    View thumbSizeView = itemView;
                    ITimeLineItem itemData = adapter.getData().get(listPos);
                    currentLineItem = itemData;
                    if (thumbSizeView != null && itemData instanceof TimeLineVideoSource) {
                        float itemThumbRange = rect.right - rect.left;
                        double per = itemThumbRange / thumbSizeView.getWidth() *
                                ((TimeLineVideoSource) itemData).getTimeLineDuration();
                        currentLineTime = ((TimeLineVideoSource) itemData).getTimeLineStartTime() + Math.round(per);
                        setTimeLineText(currentLineTime);
                        if (eventListeners != null) {
                            for (ITimeLineEventListener eventListener : eventListeners) {
                                eventListener.onTimeLineTimeChangeListener(TimeLineView.this, itemData, currentLineTime,
                                        timeRecyclerView.getScrollState());
                            }
                        }
                    }
                }
            }
        });


        adapter.setOnDragSizeChangeListener(new ImageListView.OnDragSizeChangeListener() {
            @Override
            public void onDragStart(View v) {
                if (dragSizeChangeListener != null) {
                    dragSizeChangeListener.onDragStart(v);
                }
            }

            @Override
            public void onDragSizeChange(View v, int viewWidth, int dWidth, int leftAllHidSize, int rightAllHideSize) {
                updateEndTime();
                if (dragSizeChangeListener != null) {
                    dragSizeChangeListener.onDragSizeChange(v, viewWidth, dWidth, leftAllHidSize, rightAllHideSize);
                }
            }

            @Override
            public void onDragEnd(View v, ImageListView.DragPosition dragPos, int viewWidth, int leftAllHidSize, int rightAllHideSize) {
                if (dragSizeChangeListener != null) {
                    dragSizeChangeListener.onDragEnd(v, dragPos, viewWidth, leftAllHidSize, rightAllHideSize);
                }
            }
        });
    }

    public void setOnDragSizeChangeListener(ImageListView.OnDragSizeChangeListener listener) {
        dragSizeChangeListener = listener;
    }

    public void addRecyclerViewOnScrollListener(RecyclerView.OnScrollListener onScrollListener) {
        timeRecyclerView.addOnScrollListener(onScrollListener);
    }

    public void removeRecyclerViewOnScrollListener(RecyclerView.OnScrollListener onScrollListener) {
        timeRecyclerView.addOnScrollListener(onScrollListener);
    }

    public void addRecyclerViewOnItemTouchListener(RecyclerView.OnItemTouchListener onItemTouchListener) {
        timeRecyclerView.addOnItemTouchListener(onItemTouchListener);
    }

    public void removeRecyclerViewOnItemTouchListener(RecyclerView.OnItemTouchListener onItemTouchListener) {
        timeRecyclerView.removeOnItemTouchListener(onItemTouchListener);
    }

    @Override
    public long getLineTime() {
        return currentLineTime;
    }

    @Override
    public ITimeLineItem getLineItem() {
        return currentLineItem;
    }

    @Override
    public long getLineSourceTime() {
        return 0;
    }

    @Override
    public long getTimeLinePositionEndTime(int listPos) {
        List<ITimeLineItem> curList = getTimeLineItemList();
        if (curList != null ) {
            int pos = listPos < curList.size() ? listPos : curList.size() - 1;
            for (int i = pos; i >= 0; i--) {
                if (curList.get(i).getTimeLineTime() != null) {
                    return curList.get(i).getTimeLineTime()[1];
                }
            }
        }
        return -1;
    }

    @Override
    public long getTimeLineEndTime() {
        if (adapter != null && adapter.getData() != null) {
            List<ITimeLineItem> list = adapter.getData();
            for (int i = list.size() - 1; i >= 0; i--) {
                if (list.get(i).getTimeLineTime() != null) {
                    return list.get(i).getTimeLineTime()[1];
                }
            }
        }
        return 0;
    }

    @Override
    public List<ITimeLineItem> getTimeLineItemList() {
        return adapter != null ? adapter.getData() : null;
    }

    @Override
    public ITimeLineItem getTimeLineEndItem() {
        if (getTimeLineItemList() != null) {
            int size = getTimeLineItemList().size();
            int endIndex = size - 1;
            if (endIndex >= 0) {
                return getTimeLineItemList().get(endIndex);
            }
        }
        return null;
    }

    @Override
    public ITimeLineItem getSelectedItem() {
        if (getTimeLineItemList() != null) {
            for (ITimeLineItem item : getTimeLineItemList()) {
                if (item instanceof TimeLineVideoSource) {
                    if (((TimeLineVideoSource) item).getSelectedObject().isSelected()) {
                        return item;
                    }
                }
            }
        }
        return null;
    }

    @Override
    public ISelectedObject getSelectedObject() {
        ITimeLineItem item = getSelectedItem();
        if (item != null) {
            if (item instanceof TimeLineVideoSource) {
                if (((TimeLineVideoSource) item).getSelectedObject().isSelected()) {
                    return ((TimeLineVideoSource) item).getSelectedObject();
                }
            }
        }
        return null;
    }

    @Override
    public String getTimeLineFirstFrame() {
        List<ITimeLineItem> list = getTimeLineItemList();
        if (list == null || list.isEmpty()) {
            return null;
        }
        TimeLineVideoSource videoSource = null;
        int index = 0;
        while (videoSource == null && index < list.size()) {
            ITimeLineItem item = list.get(index);
            if (item != null && item instanceof TimeLineVideoSource) {
                videoSource = (TimeLineVideoSource) item;
            }
            index++;
        }
        if (videoSource != null) {
            return videoSource.getFirstFrame();
        }
        return null;
    }

    @Override
    protected void onLineViewLayoutReset(int leftMargin) {
        super.onLineViewLayoutReset(leftMargin);
        timeNumItemDecoration.setMargin(leftMargin);
        adapter.setHeaderViewWidth(leftMargin);
    }

    @Override
    public void addTimeLineEventListener(ITimeLineEventListener listener) {
        if (eventListeners == null) {
            eventListeners = new ArrayList<>();
        }
        eventListeners.add(listener);
        adapter.addTimeLineEventListener(listener);
    }

    @Override
    public void removeTimeLineEventListener(ITimeLineEventListener listener) {
        adapter.removeTimeLineEventListener(listener);
        if (eventListeners != null) {
            eventListeners.remove(listener);
        }
    }

    @Override
    public void updateData(List<ITimeLineItem> data, boolean isAdd) {
        updateData(data, isAdd, true);
    }

    @Override
    public void updateData(List<ITimeLineItem> data, boolean isAdd, boolean isAutoAddSplit) {
        if (data != null && data.size() > 0) {
            long startTime = isAdd ? getTimeLineEndTime() : 0;
            long listStartTime = getTimeLineItemListStartTime(data);
            long dtime = startTime - listStartTime;
            for (ITimeLineItem item : data) {//矫正列表的时间
                item.setTimeLineTimeOffSet(dtime);
            }
            if (isAdd && isAutoAddSplit) {
                boolean isNeedAddSplit = getTimeLineEndItem() != null &&
                        getTimeLineEndItem().getItemType() != ITimeLineItem.ItemType.TYPE_SPLIT &&
                        data.get(0).getItemType() != ITimeLineItem.ItemType.TYPE_SPLIT;
                if (isNeedAddSplit) {
                    data.add(0, new TimeLineSplitAdd());
                }
            }
            adapter.update(data, isAdd);

            updateEndTime();
        }
    }

    @Override
    public void addPositionData(int listIndex, List<ITimeLineItem> data) {
        List<ITimeLineItem> allList = getTimeLineItemList();
        if (data != null && data.size() > 0 && allList != null) {
            if (listIndex <= allList.size() && listIndex >= 0) {
                long startIndexTime = getTimeLinePositionEndTime(listIndex);
                long dataStartTime = getTimeLineItemListStartTime(data);
                long dTime = startIndexTime - dataStartTime;
                for (ITimeLineItem item : data) {//矫正列表的时间
                    item.setTimeLineTimeOffSet(dTime);
                }
                long addAfterTime = getTimeLineItemListEndTime(data);
                long addDTime = addAfterTime - startIndexTime;
                //矫正时间线列表时间
                if (addDTime > 0) {
                    for (int i = listIndex; i < allList.size(); i++) {
                        allList.get(i).setTimeLineTimeOffSet(addDTime);
                    }
                }
                allList.addAll(listIndex, data);
            } else {
                allList.addAll(data);
            }
            updateUI();
        }
    }

    /**
     * 获取时间线上某一段数据的开始时间
     *
     * @param list 时间线上的一段数据列表
     * @return
     */
    public static long getTimeLineItemListStartTime(List<ITimeLineItem> list) {
        if (list != null && list.size() > 0) {
            for (int i = 0; i < list.size(); i++) {
                ITimeLineItem item = list.get(i);
                if (item.getTimeLineTime() != null && item.getTimeLineTime().length == 2) {
                    return item.getTimeLineTime()[0];
                }
            }
        }
        return 0;
    }

    /**
     * 获取时间线上某一段数据的结束时间
     *
     * @param list 时间线上的一段数据列表
     * @return
     */
    public static long getTimeLineItemListEndTime(List<ITimeLineItem> list) {
        if (list != null && list.size() > 0) {
            for (int i = list.size() - 1; i >= 0; i--) {
                ITimeLineItem item = list.get(i);
                if (item.getTimeLineTime() != null && item.getTimeLineTime().length == 2) {
                    return item.getTimeLineTime()[1];
                }
            }
        }
        return 0;
    }

    @Override
    public boolean removeTimeLineDataList(List<ITimeLineItem> data) {
        if (data == null || data.size() <= 0) {
            return false;
        }
        List<ITimeLineItem> allList = getTimeLineItemList();
        if (allList == null) {
            return false;
        }
        int index = allList.indexOf(data.get(data.size() - 1));
        ITimeLineItem afaterItem = index != -1 && index + 1 < allList.size() ? allList.get(index + 1) : null;
        long removeDtime = getTimeLineListEndTime(data) - getTimeLineListStartTime(data);
        boolean isRemoveOk = allList.removeAll(data);
        if (isRemoveOk) {
            if (afaterItem != null) {
                int AfterRemovePos = allList.indexOf(afaterItem);
                for (int i = AfterRemovePos; i < allList.size(); i++) {
                    allList.get(i).setTimeLineTimeOffSet(-removeDtime);
                }
            }

            updateUI();
        }
        return isRemoveOk;
    }

    @Override
    public void scrollToTimeLineTime(long toTime) {
        int listPos = getListIndexByTime(toTime);
        if (listPos != -1) {
            boolean hasView = hasShowedInView(listPos);
            if (hasView) {
                int dx = getCurrentViewScrollDx(toTime);
                timeRecyclerView.smoothScrollBy(dx, 0);
            } else {
                int adapterPos = listPos + adapter.getHeaderViewCount();
                timeRecyclerView.smoothScrollToPosition(adapterPos);
                int dx = getCurrentViewScrollDx(toTime);
                timeRecyclerView.smoothScrollBy(dx, 0);
            }
        }
    }

    @Override
    public void scrollCurrentTimeToTime(long toTime) {
        int dx = getCurrentViewScrollDx(toTime);
        timeRecyclerView.smoothScrollBy(dx, 0);
    }

    @Override
    public void sortByTime() {
        List<ITimeLineItem> allList = getTimeLineItemList();
        if (allList != null) {
            List<ITimeLineItem> spliteList = new ArrayList<>();
            List<ITimeLineItem> dataList = new ArrayList<>();
            for (ITimeLineItem item : allList) {
                if (item.getItemType() == ITimeLineItem.ItemType.TYPE_SPLIT) {
                    spliteList.add(item);
                } else {
                    dataList.add(item);
                }
            }
            Collections.sort(dataList, new Comparator<ITimeLineItem>() {
                @Override
                public int compare(ITimeLineItem o1, ITimeLineItem o2) {
                    if (o1.getTimeLineTime()[0] < o2.getTimeLineTime()[0]) {
                        return -1;
                    } else if (o1.getTimeLineTime()[0] > o2.getTimeLineTime()[0]) {
                        return 1;
                    } else {
                        return 0;
                    }
                }
            });
            allList.clear();
            for (int i = 0; i < dataList.size(); i++) {
                allList.add(dataList.get(i));
                if (i < spliteList.size()) {
                    allList.add(spliteList.get(i));
                }
            }
            updateUI();
        }
    }

    @Override
    public void updateUI() {
        adapter.notifyDataSetChanged();
        updateEndTime();
    }

    /**
     * 获取这个列表的开始时间
     *
     * @param list
     * @return
     */
    private long getTimeLineListStartTime(List<ITimeLineItem> list) {
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getTimeLineTime() != null) {
                return list.get(i).getTimeLineTime()[0];
            }
        }
        return 0;
    }

    /**
     * 获取这个列表的结尾时间
     *
     * @param list
     * @return
     */
    private long getTimeLineListEndTime(List<ITimeLineItem> list) {
        for (int i = list.size() - 1; i >= 0; i--) {
            if (list.get(i).getTimeLineTime() != null) {
                return list.get(i).getTimeLineTime()[1];
            }
        }
        return 0;
    }

    private void updateEndTime() {
        setAllTimeText(getTimeLineEndTime());
    }

    private boolean hasShowedInView(int listPos) {
        LinearLayoutManager layoutManager = (LinearLayoutManager) timeRecyclerView.getLayoutManager();
        int firstPos = layoutManager.findFirstVisibleItemPosition();
        int endPos = layoutManager.findLastVisibleItemPosition();

        return listPos + adapter.getHeaderViewCount() >= firstPos && listPos + adapter.getHeaderViewCount() <= endPos;
    }

    private int getCurrentViewScrollDx(long toTime) {
        int childCount = timeRecyclerView.getChildCount();
        for (int i = 0; i < childCount; i++) {
            View itemView = timeRecyclerView.getChildAt(i);
            int position = timeRecyclerView.getChildAdapterPosition(itemView);
            if (position == RecyclerView.NO_POSITION) {
                continue;
            }
            int size = adapter.getData().size();
            if (position - 1 < size && position - 1 >= 0) {
                ITimeLineItem item = adapter.getData().get(position - 1);
                if (item instanceof TimeLineVideoSource) {
                    TimeLineVideoSource lineThumb = (TimeLineVideoSource) item;
                    if (toTime >= lineThumb.getTimeLineStartTime() &&
                            toTime < lineThumb.getTimeLineStartTime() + lineThumb.getTimeLineDuration()) {
                        long timeD = toTime - lineThumb.getTimeLineStartTime();
                        double leftDx = timeD / ((double) lineThumb.getTimeLineDuration()) * itemView.getWidth();
                        int timeLeft = (int) (itemView.getLeft() + Math.round(leftDx));
                        int dx = timeLeft - timeLineLeftMargin;
                        return dx;
                    }
                }
            }
        }
        return 0;
    }

    private int getListIndexByTime(long time) {
        if (adapter == null || adapter.getData() == null) {
            return -1;
        }

        for (int i = 0; i < adapter.getData().size(); i++) {
            ITimeLineItem item = adapter.getData().get(i);
            if (item instanceof TimeLineVideoSource) {
                TimeLineVideoSource timeLineVideoSource = (TimeLineVideoSource) item;
                boolean is = timeLineVideoSource.getTimeLineDuration() <= time && time > timeLineVideoSource.getTimeLineStartTime()
                        + timeLineVideoSource.getTimeLineDuration();
                if (is) {
                    return i;
                }
            }
        }

        return -1;
    }
}
