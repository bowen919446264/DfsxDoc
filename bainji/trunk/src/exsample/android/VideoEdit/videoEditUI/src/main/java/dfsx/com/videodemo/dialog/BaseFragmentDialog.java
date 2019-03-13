package dfsx.com.videodemo.dialog;

import android.app.Activity;
import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.DialogFragment;
import android.view.*;

public abstract class BaseFragmentDialog extends DialogFragment {

    protected Context context;
    protected Activity activity;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        context = getContext();
        activity = getActivity();
        return inflater.inflate(getLayoutRes(), container, false);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        if (isFullScreenStyle()) {
            getDialog().getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        }
        super.onActivityCreated(savedInstanceState);
        if (isFullScreenStyle()) {
            getDialog().getWindow().setBackgroundDrawable(new ColorDrawable(0x00000000));
            getDialog().getWindow().setLayout(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT);
        }
    }

    protected boolean isFullScreenStyle() {
        return false;
    }

    public abstract int getLayoutRes();

}
