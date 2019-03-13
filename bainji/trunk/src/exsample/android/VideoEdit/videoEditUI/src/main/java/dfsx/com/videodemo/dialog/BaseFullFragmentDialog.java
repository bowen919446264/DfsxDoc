package dfsx.com.videodemo.dialog;

public class BaseFullFragmentDialog extends BaseFragmentDialog {

    @Override
    protected boolean isFullScreenStyle() {
        return true;
    }

    @Override
    public int getLayoutRes() {
        return 0;
    }
}
