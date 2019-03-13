package dfsx.com.videodemo.frag;

import android.Manifest;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;
import com.gun0912.tedpermission.PermissionListener;
import com.gun0912.tedpermission.TedPermission;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.adapter.ItemOffsetDecoration;
import dfsx.com.videodemo.adapter.SelectorRecyclerAdapter;

import java.util.ArrayList;
import java.util.List;

public class SelectedGridFragment extends Fragment {

    protected RecyclerView recyclerView;
    protected SelectorRecyclerAdapter adapter;

    protected OnSelectedSelectorChangeListener selectorChangeListener;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        TedPermission.with(getContext()).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                onUserPermissionGranted();
            }


            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(getContext(), "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(getContext().getResources().getString(com.dfsx.videoeditor.R.string.denied_message)).
                setPermissions(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .check();

        return inflater.inflate(R.layout.frag_selected_grid_layout, null);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        recyclerView = (RecyclerView) view.findViewById(R.id.selected_recycler_view);
        setRecyclerView();
    }

    protected void onUserPermissionGranted() {

    }

    protected void setRecyclerView() {
        recyclerView.setLayoutManager(new GridLayoutManager(getActivity(), 4,
                LinearLayoutManager.VERTICAL, false));
        recyclerView.addItemDecoration(new ItemOffsetDecoration(2));
        recyclerView.setClipToPadding(false);
        adapter = new SelectorRecyclerAdapter();
        recyclerView.setAdapter(adapter);
    }

    public void setOnSelectedSelectorChangeListener(OnSelectedSelectorChangeListener listener) {
        this.selectorChangeListener = listener;
    }

    public interface OnSelectedSelectorChangeListener {
        void onSelected(List<ISelector> iSelectors);
    }
}
