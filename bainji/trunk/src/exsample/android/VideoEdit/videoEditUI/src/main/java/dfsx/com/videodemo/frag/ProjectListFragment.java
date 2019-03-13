package dfsx.com.videodemo.frag;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import com.dfsx.core.common.act.WhiteTopBarActivity;
import com.dfsx.core.rx.RxBus;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.adapter.ProjectListAdapter2;
import dfsx.com.videodemo.adapter.ProjectRecyclerItemDecoration;
import dfsx.com.videodemo.proj.ProjectManager;
import rx.Subscription;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;

import java.util.ArrayList;

public class ProjectListFragment extends Fragment {


    private View btnCloseImage;
    private RecyclerView recyclerView;

    private ProjectListAdapter2 adapter;
    private Subscription projectSubscrip;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.frag_project_list, null);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initView(view);
        initAction();

        setData();
        initRegister();

    }

    private void initRegister() {
        projectSubscrip = RxBus.getInstance().toObserverable(RXBusMessage.class)
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<RXBusMessage>() {
                    @Override
                    public void call(RXBusMessage rxBusMessage) {
                        if (TextUtils.equals(rxBusMessage.action, ProjectManager.MSG_PROJECT_CHANGE)) {
                            updateData();
                        }
                    }
                });
    }

    private void initAction() {
        btnCloseImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getActivity().finish();
            }
        });

        adapter.setOnEventListener(new ProjectListAdapter2.OnEventListener() {
            @Override
            public void onProjectClick(View v, IProjectInfo projectInfo) {
                FragUtil.startEditProject(getContext(), projectInfo);
            }

            @Override
            public void onAddProjectClick(View v) {
                WhiteTopBarActivity.startAct(getContext(),
                        CreateNewProjectFragment.class.getName(), "本地视频");
            }
        });
    }

    private void initView(View view) {
        btnCloseImage = view.findViewById(R.id.btn_close);
        recyclerView = (RecyclerView) view.findViewById(R.id.project_list_recycler);
        recyclerView.setLayoutManager(new GridLayoutManager(getContext(),
                2, GridLayoutManager.VERTICAL, false));
        recyclerView.addItemDecoration(new ProjectRecyclerItemDecoration());
        adapter = new ProjectListAdapter2(getActivity());
        recyclerView.setAdapter(adapter);
    }


    private void setData() {
        ArrayList<IProjectInfo> list = ProjectManager.getSaveProjectList(getContext());
        adapter.update(list, false);
    }

    private void updateData() {
        ArrayList<IProjectInfo> list = ProjectManager.getSaveProjectList(getContext());
        adapter.update(list, false);
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        if (projectSubscrip != null) {
            projectSubscrip.unsubscribe();
        }
    }
}
