package com.dfsx.videoeditor.widget.timeline;

import com.dfsx.core.CoreApp;
import com.dfsx.videoeditor.R;

import java.util.List;

public interface ITimeLineUI {

    /**
     * 时间线的高度
     */
    int TIMELINE_HEIGHT = (int) CoreApp.getInstance().getResources().getDimension(R.dimen.time_line_height);
    /**
     * 选中状态界面的高度，影响显示选中区间的时间显示可用的大小
     */
    int TIMELINE_SELECT_VIEW_HEIGHT = (int) CoreApp.getInstance().getResources().getDimension(R.dimen.time_line_select_view_height);

    /**
     * 获取时间线上 的时间
     *
     * @return
     */
    long getLineTime();

    ITimeLineItem getLineItem();

    /**
     * 获取时间线 对应的位置的video时间
     *
     * @return
     */
    long getLineSourceTime();

    /**
     * 获取时间线上位置的时间
     *
     * @return
     */
    long getTimeLinePositionEndTime(int listPos);

    /**
     * 获取时间线上的最终时间
     *
     * @return
     */
    long getTimeLineEndTime();

    /**
     * 获取时间线上的所有Item
     *
     * @return
     */
    List<ITimeLineItem> getTimeLineItemList();

    /**
     * 获取当前最后位置的Item数据
     *
     * @return
     */
    ITimeLineItem getTimeLineEndItem();

    ITimeLineItem getSelectedItem();

    ISelectedObject getSelectedObject();

    /**
     * 获取时间线上的首帧图片
     *
     * @return
     */
    String getTimeLineFirstFrame();


    void addTimeLineEventListener(ITimeLineEventListener listener);

    void removeTimeLineEventListener(ITimeLineEventListener listener);

    void updateData(List<ITimeLineItem> data, boolean isAdd);

    /**
     * 更新数据
     *
     * @param data           按时间线数据排序
     * @param isAdd          是否添加
     * @param isAutoAddSplit 是否自动判断添加点是否添加分割（#ITimeLineItem.ItemType.TYPE_SPLIT）类型的数据
     */
    void updateData(List<ITimeLineItem> data, boolean isAdd, boolean isAutoAddSplit);

    /**
     * 在时间线里对应位置增加数据
     *
     * @param listIndex 数据位置
     * @param data      数据（包含时间顺序的列表）
     */
    void addPositionData(int listIndex, List<ITimeLineItem> data);

    /**
     * 删除一些时间线上的数据
     *
     * @param data 时间线上的数据。(按时间线时间排序)
     * @return
     */
    boolean removeTimeLineDataList(List<ITimeLineItem> data);

    /**
     * 滚动到时间线的指定时间
     * <p>
     * 这个比较消耗计算，不要高頻率調用
     *
     * @param toTime
     */
    void scrollToTimeLineTime(long toTime);

    /**
     * 从当前时间线时间滚动到时间。
     *
     * @param toTime 不能超出界面的时间
     */
    void scrollCurrentTimeToTime(long toTime);

    void sortByTime();

    void updateUI();
}
