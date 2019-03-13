package dfsx.com.videodemo.frag;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;
import com.dfsx.core.common.act.DefaultFragmentActivity;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.EditMediaRecyclerAdapter;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.adapter.ItemOffsetDecoration;

import java.io.Serializable;
import java.util.List;

/**
 * 选择本地视频进行编辑
 */
public class CreateNewProjectFragment extends Fragment implements SelectedGridFragment.OnSelectedSelectorChangeListener {

    private Context context;

    private RecyclerView selectedRecycler;
    private View btnStartEdit;

    private int screenWidth;

    private LocalVideoSelectedGridFragment selectedMiediaFrag;
    private List<ISelector> selectedSelectors;
    private EditMediaRecyclerAdapter selectedAdapter;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        context = getContext();
        screenWidth = getActivity().getWindowManager().getDefaultDisplay().getWidth();
        return inflater.inflate(R.layout.frag_create_new_proj_layout, null);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        btnStartEdit = view.findViewById(R.id.btn_start_edit);
        selectedRecycler = (RecyclerView) view.findViewById(R.id.edit_media_recycler);
        ViewGroup.LayoutParams params = selectedRecycler.getLayoutParams();
        if (params != null) {
            params.height = screenWidth / 4;
            selectedRecycler.setLayoutParams(params);
        }
        selectedRecycler.addItemDecoration(new ItemOffsetDecoration(2));
        selectedRecycler.setLayoutManager(new LinearLayoutManager(context, LinearLayoutManager.HORIZONTAL, false));
        selectedAdapter = new EditMediaRecyclerAdapter(screenWidth / 4);
        selectedRecycler.setAdapter(selectedAdapter);
        addSelectedFrag();


        btnStartEdit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startEditClick();
            }
        });
    }

    private void startEditClick() {
        if (selectedSelectors == null || selectedSelectors.isEmpty()) {
            Toast.makeText(context, "还没有选择视频", Toast.LENGTH_SHORT).show();
        } else {
            FragUtil.startEdit(context, selectedSelectors);
            getActivity().finish();
        }
    }

    private void addSelectedFrag() {
        FragmentManager fragmentManager = getChildFragmentManager();
        selectedMiediaFrag = FragUtil.addVideoSelectedFragent(fragmentManager, R.id.media_select_view,
                "Selected_Media_Frag");
        selectedMiediaFrag.setOnSelectedSelectorChangeListener(this);
    }

    @Override
    public void onSelected(List<ISelector> iSelectors) {
        selectedSelectors = iSelectors;
        selectedAdapter.update(iSelectors, false);
    }
}
