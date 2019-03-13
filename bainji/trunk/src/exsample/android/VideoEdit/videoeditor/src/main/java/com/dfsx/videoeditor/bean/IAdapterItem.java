package com.dfsx.videoeditor.bean;

public interface IAdapterItem<T> {

    enum ItemType {
        TYPE_FRAME(0),
        TYPE_SOURCE_SPLIT(1);

        private int typeCount;

        ItemType(int count) {
            this.typeCount = count;
        }

        public int getTypeCount() {
            return typeCount;
        }

        public static ItemType findType(int typeCount) {
            ItemType type = TYPE_FRAME;
            switch (typeCount) {
                case 0:
                    type = TYPE_FRAME;
                    break;
                case 1:
                    type = TYPE_SOURCE_SPLIT;
                    break;
            }

            return type;
        }
    }

    ItemType getItemType();

    T getItemData();
}
