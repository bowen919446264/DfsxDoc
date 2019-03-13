package dfsx.com.videodemo.adapter;

import java.io.Serializable;

/**
 * 选择的数据
 * @param <T>
 */
public interface ISelector<T> extends Serializable {

    /**
     * 获取选中的类容
     * @return
     */
    T getSelector();

    /**
     * 获取选择的缩略图
     *
     * @return
     */
    String getSelectorThumb();

    /**
     * 获取选择的长度
     *
     * @return
     */
    long getSelectorLength();

    boolean isSelected();

    void setSelected(boolean isSelected);
}
