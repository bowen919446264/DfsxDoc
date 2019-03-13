package com.dfsx.videoeditor.bean;

public class SplitData implements IAdapterItem {
    @Override
    public ItemType getItemType() {
        return ItemType.TYPE_SOURCE_SPLIT;
    }

    @Override
    public Object getItemData() {
        return null;
    }
}
