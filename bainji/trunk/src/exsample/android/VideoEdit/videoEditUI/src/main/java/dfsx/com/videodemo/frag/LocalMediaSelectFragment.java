package dfsx.com.videodemo.frag;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.view.View;
import android.widget.Toast;
import com.dfsx.core.common.act.WhiteTopBarActivity;
import com.dfsx.core.rx.RxBus;
import dfsx.com.videodemo.adapter.ISelector;

import java.util.List;

/**
 * 选择Media
 * 通過RXBUs返回数据 MSG_SELECTED_MEDIA_OK
 */
public class LocalMediaSelectFragment extends LocalVideoSelectedGridFragment {

    public static final String MSG_SELECTED_MEDIA_OK = "dfsx.com.videodemo.SELECTED_MEDIA_OK";
    public static final String KEY_RETURN_ACTION = "dfsx.com.videodemo.SELECTED_MEDIA_return_action";
    private String action = null;

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        initReturnAction();
        initTopBar();
        super.onViewCreated(view, savedInstanceState);
        setSingleSelected(true);
    }

    private void initReturnAction() {
        if (getArguments() != null) {
            action = getArguments().getString(KEY_RETURN_ACTION, null);
        }
        if (TextUtils.isEmpty(action)) {
            action = MSG_SELECTED_MEDIA_OK;
        }
    }

    private void initTopBar() {
        Activity act = getActivity();
        if (act instanceof WhiteTopBarActivity) {
            WhiteTopBarActivity activity = (WhiteTopBarActivity) act;
            activity.getTopBarRightText().setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    onSelectedOkCallBack();
                }
            });
        }
    }

    private void onSelectedOkCallBack() {
        if (getSelectedList() == null || getSelectedList().isEmpty()) {
            Toast.makeText(getContext(), "请先选择资源", Toast.LENGTH_SHORT).show();
            return;
        }
        RxBus.getInstance().post(new RXBusMessage<List<ISelector>>(action, getSelectedList()));
        Intent intent = new Intent(action);
        intent.putExtra(MSG_SELECTED_MEDIA_OK, getSelectedList());
        getActivity().setResult(Activity.RESULT_OK, intent);
        getActivity().finish();
    }
}
