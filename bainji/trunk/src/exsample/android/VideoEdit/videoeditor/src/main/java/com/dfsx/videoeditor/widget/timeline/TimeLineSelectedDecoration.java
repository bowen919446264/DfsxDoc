//package com.dfsx.videoeditor.widget.timeline;
//
//import android.content.Context;
//import android.graphics.Canvas;
//import android.graphics.Rect;
//import android.support.v7.widget.RecyclerView;
//import android.util.Log;
//import android.view.View;
//import com.dfsx.core.common.Util.PixelUtil;
//import com.dfsx.videoeditor.widget.timeline.bean.TimeLineVideoSource;
//
///**
// * 时间线数选择时数据交互， 负责控制选择效果展示。
// */
//public class TimeLineSelectedDecoration extends RecyclerView.ItemDecoration implements SelectedControlView.OnDragListener,
//        SelectedControlView.IDragTimeCallBack {
//
//    private Context context;
//    private SelectedControlView selectedControlView;
//    private Rect selectedBkgRect;
//    private Rect selectedStartMarkRect;
//    private Rect selectedEndMarkRect;
//    private RecyclerView parent;
//
//    private Rect drawStartMarkRect;
//    private Rect drawEndMarkRect;
//
//    private long selectedStartTime;
//    private long selectedEndTime;
//
//    private OnTimeLineSelectedTimeChangeListener onTimeLineSelectedTimeChangeListener;
//
//    public TimeLineSelectedDecoration(Context context, SelectedControlView selectedControlView) {
//        this.context = context;
//        this.selectedControlView = selectedControlView;
//        selectedBkgRect = new Rect();
//        selectedStartMarkRect = new Rect();
//        selectedEndMarkRect = new Rect();
//        drawStartMarkRect = new Rect();
//        drawEndMarkRect = new Rect();
//        this.selectedControlView.setOnDragListener(this);
//        this.selectedControlView.setDragTimeCallBack(this);
//    }
//
//    @Override
//    public void onDraw(Canvas canvas, RecyclerView parent, RecyclerView.State state) {
//        this.parent = parent;
//        super.onDraw(canvas, parent, state);
//        int childCount = parent.getChildCount();
//        RecyclerView.Adapter adapter = parent.getAdapter();
//        if (childCount <= 0 || adapter.getItemCount() <= 0 && adapter instanceof TimeLineRecyclerAdapter) {
//            return;
//        }
//        selectedBkgRect.left = 0;
//        selectedBkgRect.right = 0;
//        boolean hasStart = false, hasEnd = false;
//        boolean hasSelected = false;
//        for (int i = 0; i < childCount; i++) {
//            View itemView = parent.getChildAt(i);
//            int position = parent.getChildAdapterPosition(itemView);
//            if (position == RecyclerView.NO_POSITION) {
//                continue;
//            }
//            TimeLineRecyclerAdapter recyclerAdapter = (TimeLineRecyclerAdapter) adapter;
//            int pos = position - recyclerAdapter.getHeaderViewCount();
//            boolean is = pos >= 0 && recyclerAdapter.getData().size() > pos && recyclerAdapter.getData().get(pos) instanceof TimeLineVideoSource &&
//                    ((TimeLineVideoSource) recyclerAdapter.getData().get(pos)).getSelectedObject().isSelected();
//            if (is) {
//                hasSelected = true;
//                TimeLineVideoSource thumb = (TimeLineVideoSource) recyclerAdapter.getData().get(pos);
//                ISelectedObject selectedObject = thumb.getSelectedObject();
//                if (selectedBkgRect.left == 0 || selectedBkgRect.left > itemView.getLeft()) {
//                    selectedBkgRect.left = itemView.getLeft();
//                }
//                selectedBkgRect.top = itemView.getTop();
//                selectedBkgRect.bottom = itemView.getBottom();
//                if (selectedBkgRect.right == 0 || selectedBkgRect.right < itemView.getRight()) {
//                    selectedBkgRect.right = itemView.getRight();
//                }
//                if (thumb.isStartOfThumbs()) {
//                    hasStart = true;
//                    selectedStartMarkRect.left = itemView.getLeft() - getMarkWidth();
//                    selectedStartMarkRect.right = itemView.getLeft();
//                    selectedStartMarkRect.top = itemView.getTop();
//                    selectedStartMarkRect.bottom = itemView.getBottom();
//
//                    if (selectedObject.getSelectedInfo() == null ||
//                            selectedObject.getSelectedInfo().startPosition.leftPosition == -1 ||
//                            selectedObject.getSelectedInfo().startPosition.leftPosition < position) {//默认没有数据，或者记录数据超出开始位置
//                        drawStartMarkRect.set(selectedStartMarkRect);
//                        selectedStartTime = thumb.getTimeLineStartTime();
//                        if (selectedObject.getSelectedInfo() != null) {
//                            boolean isChange = selectedObject.getSelectedInfo().startSelectedTimeLineTime != selectedStartTime;
//                            selectedObject.getSelectedInfo().startSelectedTimeLineTime = selectedStartTime;
//                            if (onTimeLineSelectedTimeChangeListener != null && isChange) {
//                                onTimeLineSelectedTimeChangeListener.onSelectedTimeChange(thumb, selectedObject.getSelectedInfo());
//                            }
//                        }
//                    }
//                }
//
//                if (thumb.isEndOfThumbs()) {
//                    hasEnd = true;
//                    selectedEndMarkRect.left = itemView.getRight();
//                    selectedEndMarkRect.right = itemView.getRight() + getMarkWidth();
//                    selectedEndMarkRect.top = itemView.getTop();
//                    selectedEndMarkRect.bottom = itemView.getBottom();
//
//                    if (selectedObject.getSelectedInfo() == null ||
//                            selectedObject.getSelectedInfo().endPosition.leftPosition == -1 ||
//                            selectedObject.getSelectedInfo().endPosition.leftPosition > position) {//没有设置位置信息 或者记录结束位置超出了結束位置，就使用結束位置
//                        drawEndMarkRect.set(selectedEndMarkRect);
//                        selectedEndTime =
//                                thumb.getTimeLineStartTime() + thumb.getTimeLineDuration();
//                        if (selectedObject.getSelectedInfo() != null) {
//                            boolean isChange = selectedObject.getSelectedInfo().endSelectedTimeLineTime != selectedEndTime;
//                            selectedObject.getSelectedInfo().endSelectedTimeLineTime = selectedEndTime;
//                            if (onTimeLineSelectedTimeChangeListener != null && isChange) {
//                                onTimeLineSelectedTimeChangeListener.onSelectedTimeChange(thumb, selectedObject.getSelectedInfo());
//                            }
//                        }
//                    }
//                }
//
//                if (selectedObject.getSelectedInfo() != null) {
//                    ISelectedObject.SelectedInfo selectedInfo = selectedObject.getSelectedInfo();
//                    Rect startRect = selectedInfo.startPosition.hasLeftPosition(position);
//                    if (startRect != null) {
//                        hasStart = true;
//                        int left = startRect.left + itemView.getLeft();
//                        drawStartMarkRect.left = left;
//                        drawStartMarkRect.right = left + getMarkWidth();
//                        drawStartMarkRect.top = itemView.getTop();
//                        drawStartMarkRect.bottom = itemView.getBottom();
//                    }
//                    startRect = selectedInfo.startPosition.hasRightPosition(position);
//                    if (startRect != null) {
//                        hasStart = true;
//                        int right = startRect.right + itemView.getLeft();
//                        drawStartMarkRect.left = right - getMarkWidth();
//                        drawStartMarkRect.right = right;
//                        drawStartMarkRect.top = itemView.getTop();
//                        drawStartMarkRect.bottom = itemView.getBottom();
//                    }
//
//                    Rect endRect = selectedInfo.endPosition.hasLeftPosition(position);
//                    if (endRect != null) {
//                        hasEnd = true;
//                        int left = itemView.getLeft() + endRect.left;
//                        drawEndMarkRect.left = left;
//                        drawEndMarkRect.right = left + getMarkWidth();
//                        drawEndMarkRect.top = itemView.getTop();
//                        drawEndMarkRect.bottom = itemView.getBottom();
//                    }
//                    endRect = selectedInfo.endPosition.hasRightPosition(position);
//                    if (endRect != null) {
//                        hasEnd = true;
//                        int right = itemView.getLeft() + endRect.right;
//                        drawEndMarkRect.left = right - getMarkWidth();
//                        drawEndMarkRect.right = right;
//                        drawEndMarkRect.top = itemView.getTop();
//                        drawEndMarkRect.bottom = itemView.getBottom();
//                    }
//                }
//            }
//        }
//        //根据背景来设置限制边界
//        selectedStartMarkRect.left = selectedBkgRect.left - getMarkWidth();
//        selectedStartMarkRect.right = selectedBkgRect.left;
//        selectedEndMarkRect.left = selectedBkgRect.right;
//        selectedEndMarkRect.right = selectedBkgRect.right + getMarkWidth();
//        //设置开始绘制
//        selectedControlView.setDrawLimit(selectedBkgRect, selectedStartMarkRect, selectedEndMarkRect);
//        selectedControlView.postDraw(hasSelected ? selectedBkgRect : null,
//                hasStart ? drawStartMarkRect : null,
//                hasEnd ? drawEndMarkRect : null);
//    }
//
//    public int getMarkWidth() {
//        return PixelUtil.dp2px(context, 25);
//    }
//
//    @Override
//    public void onDragStart(int dragHandle) {
//
//    }
//
//    /**
//     * 拖拽中
//     * 识别当前拖拽的时间并记录下来
//     *
//     * @param dragHandle
//     * @param dx
//     * @param dy
//     * @param startRect
//     * @param endRect
//     */
//    @Override
//    public void onDraging(int dragHandle, int dx, int dy, Rect startRect, Rect endRect) {
//        if (parent == null) {
//            return;
//        }
//        //设置拖动过程中的时间信息
//        int childCount = parent.getChildCount();
//        RecyclerView.Adapter adapter = parent.getAdapter();
//        if (childCount <= 0 || adapter.getItemCount() <= 0 && adapter instanceof TimeLineRecyclerAdapter) {
//            return;
//        }
//        TimeLineRecyclerAdapter recyclerAdapter = (TimeLineRecyclerAdapter) adapter;
//        long startFindTime = -1;
//        long endFindTime = -1;
//        for (int i = 0; i < childCount; i++) {
//            View itemView = parent.getChildAt(i);
//            int position = parent.getChildAdapterPosition(itemView);
//            if (position == RecyclerView.NO_POSITION) {
//                continue;
//            }
//            int listPos = position - recyclerAdapter.getHeaderViewCount();
//            boolean is = listPos >= 0 && recyclerAdapter.getData().size() > listPos &&
//                    recyclerAdapter.getData().get(listPos) instanceof TimeLineVideoSource;
//            boolean isStartRectRightContain = startRect != null && !startRect.isEmpty() &&
//                    startRect.right - itemView.getLeft() >= 0 && startRect.right - itemView.getRight() < 0;
//            boolean isEndRectLeftContain = endRect != null && !endRect.isEmpty() &&
//                    endRect.left - itemView.getLeft() >= 0 && endRect.left - itemView.getRight() < 0;
//            if (is && isStartRectRightContain) {
//                ITimeLineItem lineItem = recyclerAdapter.getData().get(listPos);
//                TimeLineVideoSource lineThumb = (TimeLineVideoSource) lineItem;
//                double per = (startRect.right - itemView.getLeft()) / ((float) lineThumb.getThumbWidth())
//                        * lineThumb.getTimeLineDuration();
//                startFindTime = lineThumb.getTimeLineStartTime() + Math.round(per);
//                boolean isChange = lineThumb.getSelectedObject().getSelectedInfo().startSelectedTimeLineTime != startFindTime;
//                lineThumb.getSelectedObject().getSelectedInfo().startSelectedTimeLineTime = startFindTime;
//                if (onTimeLineSelectedTimeChangeListener != null && isChange) {
//                    onTimeLineSelectedTimeChangeListener.onSelectedTimeChange(lineItem, lineThumb.getSelectedObject().getSelectedInfo());
//                }
//            }
//            if (is && isEndRectLeftContain) {
//                ITimeLineItem lineItem = recyclerAdapter.getData().get(listPos);
//                TimeLineVideoSource lineThumb = (TimeLineVideoSource) lineItem;
//                double per = (endRect.left - itemView.getLeft()) / ((float) lineThumb.getThumbWidth())
//                        * lineThumb.getTimeLineDuration();
//                endFindTime = lineThumb.getTimeLineStartTime() + Math.round(per);
//                boolean isChange = lineThumb.getSelectedObject().getSelectedInfo().endSelectedTimeLineTime != endFindTime;
//                lineThumb.getSelectedObject().getSelectedInfo().endSelectedTimeLineTime = endFindTime;
//                if (onTimeLineSelectedTimeChangeListener != null && isChange) {
//                    onTimeLineSelectedTimeChangeListener.onSelectedTimeChange(lineItem, lineThumb.getSelectedObject().getSelectedInfo());
//                }
//            }
//        }
//        if (startFindTime != -1) {
//            selectedStartTime = startFindTime;
//            Log.e("TAG", "startFindTime == " + startFindTime);
//        }
//        if (endFindTime != -1) {
//            selectedEndTime = endFindTime;
//            Log.e("TAG", "endFindTime == " + endFindTime);
//        }
//    }
//
//    /**
//     * 记录拖拽结束的位置。 和数据集合的位置绑定。 这里绑定的位置是Adapter的位置。和显示的数据集合之间要转换一下
//     * <p>
//     * 用作在拖动RecyclerView时保持和数据的相对位置不变
//     *
//     * @param dragHandle
//     * @param rectLeft
//     * @param rectRight
//     */
//    @Override
//    public void onDragEnd(int dragHandle, int rectLeft, int rectRight) {
//        if (parent == null) {
//            return;
//        }
//        //更新选中的结束时的位置信息
//        int childCount = parent.getChildCount();
//        RecyclerView.Adapter adapter = parent.getAdapter();
//        if (childCount <= 0 || adapter.getItemCount() <= 0 && adapter instanceof TimeLineRecyclerAdapter) {
//            return;
//        }
//        TimeLineRecyclerAdapter recyclerAdapter = (TimeLineRecyclerAdapter) adapter;
//        int startPos = parent.getChildAdapterPosition(parent.getChildAt(0)) - recyclerAdapter.getHeaderViewCount();
//        startPos = startPos >= 0 ? startPos : 0;
//        ISelectedObject.SelectedInfo selectedInfo = findSelectedInfo(startPos, recyclerAdapter);
//        if (selectedInfo == null) {
//            Log.e("TAG", "没有找选中信息对象-----------");
//        }
//        for (int i = 0; i < childCount; i++) {
//            View itemView = parent.getChildAt(i);
//            int position = parent.getChildAdapterPosition(itemView);
//            if (position == RecyclerView.NO_POSITION) {
//                continue;
//            }
//
//            int itemViewLeft = itemView.getLeft();
//            int itemViewRight = itemView.getRight();
//            int top = itemView.getTop();
//            int bottom = itemView.getBottom();
//
//            if (rectLeft - itemViewLeft >= 0 && rectLeft - itemViewRight < 0) {//矩形左边线找到
//                if (selectedInfo != null) {
//                    if (dragHandle == SelectedControlView.HANDLE_START) {
//                        selectedInfo.startPosition.leftPosition = position;
//                        int left = rectLeft - itemViewLeft;
//                        selectedInfo.startPosition.leftPosRect.set(left, top, left + getMarkWidth(), bottom);
//                    } else if (dragHandle == SelectedControlView.HANDLE_END) {
//                        selectedInfo.endPosition.leftPosition = position;
//                        int left = rectLeft - itemViewLeft;
//                        selectedInfo.endPosition.leftPosRect.set(left, top, left + getMarkWidth(), bottom);
//                    }
//
//                }
//            }
//            if (rectRight - itemViewLeft >= 0 && rectRight - itemViewRight < 0) {//矩形右边线找到
//                if (selectedInfo != null) {
//                    if (dragHandle == SelectedControlView.HANDLE_START) {
//                        selectedInfo.startPosition.rightPosition = position;
//                        int right = rectRight - itemViewLeft;
//                        selectedInfo.startPosition.rightPosRect.set(right - getMarkWidth(), top, right, bottom);
//                    } else if (dragHandle == SelectedControlView.HANDLE_END) {
//                        selectedInfo.endPosition.rightPosition = position;
//                        int right = rectRight - itemViewLeft;
//                        selectedInfo.endPosition.rightPosRect.set(right - getMarkWidth(), top, right, bottom);
//                    }
//                }
//            }
//
//        }
//    }
//
//    /**
//     * 从当前界面的位置出发找到第一个是选中数据不为null的数据
//     * <p>
//     * 此前提是保证选中的条目公用一个选中对象
//     *
//     * @param startPos
//     * @param recyclerAdapter
//     * @return
//     */
//    private ISelectedObject.SelectedInfo findSelectedInfo(int startPos, TimeLineRecyclerAdapter recyclerAdapter) {
//        for (int i = startPos; i < recyclerAdapter.getData().size(); i++) {
//            ITimeLineItem item = recyclerAdapter.getData().get(i);
//            if (item instanceof TimeLineVideoSource) {
//                ISelectedObject.SelectedInfo selectedInfo = ((TimeLineVideoSource) item).getSelectedObject().getSelectedInfo();
//                if (selectedInfo != null && ((TimeLineVideoSource) item).getSelectedObject().isSelected()) {
//                    return selectedInfo;
//                }
//            }
//        }
//        return null;
//    }
//
//    @Override
//    public long getDragStartTime() {
//        return selectedStartTime;
//    }
//
//    @Override
//    public long getDragEndTime() {
//        return selectedEndTime;
//    }
//
//
//    /**
//     *
//     */
//    public void setOnTimeLineSelectedTimeChangeListener(OnTimeLineSelectedTimeChangeListener listener) {
//        this.onTimeLineSelectedTimeChangeListener = listener;
//    }
//
//    public interface OnTimeLineSelectedTimeChangeListener {
//        void onSelectedTimeChange(ITimeLineItem timeLineItem, ISelectedObject.SelectedInfo selectedInfo);
//    }
//}
