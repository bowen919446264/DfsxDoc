package com.dfsx.videoeditor.widget.timeline;

/**
 * 选中对象的基础实现类。其他实现尽量继承
 * @param <T>
 */
public class BaseSelectedObject<T> implements ISelectedObject {
    private T data;
    private boolean isSelected;
    private SelectedInfo selectedInfo;

    public BaseSelectedObject(T data) {
        this.data = data;
        selectedInfo = new SelectedInfo();
    }

    @Override
    public boolean isSelected() {
        return isSelected;
    }

    @Override
    public void setSelected(boolean isSelected) {
        if (isSelected != this.isSelected) {//切换的时候把数据清除
            selectedInfo.clear();
        }
        this.isSelected = isSelected;
    }

    @Override
    public SelectedInfo getSelectedInfo() {
        return selectedInfo;
    }

    public T getData() {
        return data;
    }
}
