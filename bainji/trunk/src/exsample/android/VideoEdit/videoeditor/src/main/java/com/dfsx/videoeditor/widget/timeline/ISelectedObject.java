package com.dfsx.videoeditor.widget.timeline;

import android.graphics.Rect;

/**
 * 选中类型的数据
 */
public interface ISelectedObject {

    boolean isSelected();

    void setSelected(boolean isSelected);

    /**
     * 获取选中区域的数据信息
     * <p>
     * 选中时会保留选中的数据，未选中数据不可用是初始值。
     * 需要实现类具体实现功能。参照BaseSelectedObject
     *
     * @return
     */
    SelectedInfo getSelectedInfo();

    class SelectedInfo {
        public PositionInfo startPosition;
        public PositionInfo endPosition;

        /**
         * 开始位置的时间线时间
         */
        public long startSelectedTimeLineTime;
        /**
         * 结束位置的时间线时间
         */
        public long endSelectedTimeLineTime;

        public SelectedInfo() {
            startPosition = new PositionInfo();
            endPosition = new PositionInfo();
        }

        public void clear() {
            startPosition.clear();
            endPosition.clear();
            startSelectedTimeLineTime = -1;
            endSelectedTimeLineTime = -1;
        }
    }

    class PositionInfo {
        public int leftPosition = -1;
        public int rightPosition = -1;

        public Rect leftPosRect;
        public Rect rightPosRect;

        public PositionInfo() {
            leftPosRect = new Rect();
            rightPosRect = new Rect();
        }

        public void clear() {
            leftPosition = -1;
            rightPosition = -1;
            leftPosRect.set(0, 0, 0, 0);
            rightPosRect.set(0, 0, 0, 0);
        }

        public Rect hasLeftPosition(int position) {
            if (leftPosition == position) {
                return leftPosRect;
            }
            if (rightPosition == position) {
                return rightPosRect;
            }
            return null;
        }

        public Rect hasRightPosition(int position) {
            if (leftPosition == position) {
                return leftPosRect;
            }
            if (rightPosition == position) {
                return rightPosRect;
            }
            return null;
        }
    }
}
