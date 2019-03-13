package dfsx.com.videodemo.frag;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.CheckedTextView;
import android.widget.FrameLayout;
import android.widget.Toast;

import dfsx.com.videodemo.R;
import dfsx.com.videodemo.widget.HorizontalListBarView;

public class SelectedEditFragment extends BaseBottomFragment {

    private HorizontalListBarView selectedToolBar;
    private View childView;
    private HorizontalListBarView cutToolbar;

    private OnEditActionListener editActionListener;

    private static final int[] selectedTools = new int[]{
            R.drawable.cut_bar,
            R.drawable.voice_bar,
            R.drawable.effect_bar,
            R.drawable.del_bar
    };

    private static final String[] cutTools = new String[]{
            "拆分"
    };

    @Override
    protected void setBottomContentView(FrameLayout contentView) {
        super.setBottomContentView(contentView);
        contentView.addView(LayoutInflater.from(context)
                .inflate(R.layout.frag_selected_edit_layout, null));
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        childView = view.findViewById(R.id.child_view);
        cutToolbar = (HorizontalListBarView) view.findViewById(R.id.child_cut_tool_bar);
        selectedToolBar = (HorizontalListBarView) view.findViewById(R.id.edit_tool_bar);
        selectedToolBar.setUpBarContentView(selectedTools);
        cutToolbar.setUpBarContentView(cutTools);

        selectedToolBar.setOnItemClickListener(new HorizontalListBarView.OnItemClickListener() {
            @Override
            public void onItemClick(View v, int position) {
                if (position == 0) {//cut
                    CheckedTextView tv = (CheckedTextView) v;
                    childView.setVisibility(tv.isChecked() ? View.VISIBLE : View.GONE);
                    cutToolbar.setVisibility(tv.isChecked() ? View.VISIBLE : View.GONE);
                } else if (position == 3) {//delete
                    deleteClick();
                    removeSelef();
                } else {
                    Toast.makeText(context, "暂未开通", Toast.LENGTH_SHORT).show();
                }
            }
        });

        cutToolbar.setOnItemClickListener(new HorizontalListBarView.OnItemClickListener() {
            @Override
            public void onItemClick(View v, int position) {
                switch (position) {
                    case 0://分离
                        if (editActionListener != null) {
                            editActionListener.onSplitActionClick();
                        }
                        removeSelef();
                        break;
                }
            }
        });
    }

    private void deleteClick() {
//        DelEditAction delEditAction = new DelEditAction(context, ProjectManager.getInstance().getTimeLineUI(),
//                ProjectManager.getInstance().getEditTimeLine());
//        ProjectManager.getInstance().getEditTimeLine().dispatchAction(delEditAction);
        if (editActionListener != null) {
            editActionListener.onDeleteActionClick();
        }
    }


    private void removeSelef() {
        getFragmentManager().beginTransaction()
                .setCustomAnimations(R.anim.frag_bottom_in, R.anim.frag_bottom_out)
                .remove(this)
                .commitAllowingStateLoss();
    }

    public void setEditActionListener(OnEditActionListener editActionListener) {
        this.editActionListener = editActionListener;
    }

    public interface OnEditActionListener {

        /**
         * 拆分
         */
        void onSplitActionClick();

        void onDeleteActionClick();
    }
}
