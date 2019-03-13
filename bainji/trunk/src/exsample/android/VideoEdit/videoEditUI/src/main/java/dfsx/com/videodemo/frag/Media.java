package dfsx.com.videodemo.frag;

import dfsx.com.videodemo.adapter.ISelector;

public abstract class Media implements ISelector<String> {

    private String url;
    private long length;

    private boolean isSelected;

    public Media(String url, long length) {
        this.url = url;
        this.length = length;
    }

    @Override
    public String getSelector() {
        return url;
    }

    public String getUrl() {
        return url;
    }

    @Override
    public long getSelectorLength() {
        return length;
    }

    @Override
    public boolean isSelected() {
        return isSelected;
    }

    @Override
    public void setSelected(boolean isSelected) {
        this.isSelected = isSelected;
    }
}
