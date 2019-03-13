package dfsx.com.videodemo.frag;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import dfsx.com.videodemo.R;

public class BaseBottomFragment extends Fragment {

    protected Context context;
    protected Activity activity;
    protected View rootView;
    protected FrameLayout contentView;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        context = getContext();
        activity = getActivity();
        rootView = inflater.inflate(R.layout.frag_bottom_layout, null);
        contentView = (FrameLayout) rootView.findViewById(R.id.frag_bottom_content);
        setBottomContentView(contentView);
        return rootView;
    }

    protected void setBottomContentView(FrameLayout contentView) {

    }
}
