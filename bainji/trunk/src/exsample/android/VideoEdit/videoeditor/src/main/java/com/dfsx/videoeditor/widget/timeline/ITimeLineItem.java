package com.dfsx.videoeditor.widget.timeline;

public interface ITimeLineItem<T> {
    enum ItemType {
        TYPE_EMPTY_TIME(0),//表示空白的时间间隔（有时间参数）
        TYPE_THUMB(1),//表示资源
        TYPE_SPLIT(2);//资源分割线(没有时间)

        private int typeCount;

        ItemType(int count) {
            this.typeCount = count;
        }

        public int getTypeCount() {
            return typeCount;
        }

        public static ItemType findType(int typeCount) {
            ItemType type = TYPE_THUMB;
            switch (typeCount) {
                case 0:
                    type = TYPE_EMPTY_TIME;
                    break;
                case 1:
                    type = TYPE_THUMB;
                    break;
                case 2:
                    type = TYPE_SPLIT;
                    break;
            }
            return type;
        }
    }

    /**
     * 获取组装的数据
     *
     * @return
     */
    T getItemData();

    /**
     * 获取当前Item在时间线的时间。一个时间标尺。（null标识没有时间）
     *
     * @return
     */
    long[] getTimeLineTime();

    /**
     * 设置当前Item的时间刻度偏移量
     * 带有符号的数据， 正数表示增加， 负数表示减少
     *
     * @param timeOffSet
     */
    void setTimeLineTimeOffSet(long timeOffSet);

    ItemType getItemType();
}
