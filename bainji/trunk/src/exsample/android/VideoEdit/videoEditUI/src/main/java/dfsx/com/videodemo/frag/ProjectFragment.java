package dfsx.com.videodemo.frag;

import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.LinearLayout;
import com.baoyz.swipemenulistview.SwipeMenu;
import com.baoyz.swipemenulistview.SwipeMenuCreator;
import com.baoyz.swipemenulistview.SwipeMenuItem;
import com.baoyz.swipemenulistview.SwipeMenuListView;
import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.core.common.act.WhiteTopBarActivity;
import com.dfsx.core.rx.RxBus;
import com.dfsx.pullrefresh.BasePullRefreshFragment;
import com.dfsx.videoeditor.util.EditConstants;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.adapter.ProjectListAdapter;
import dfsx.com.videodemo.record.IRecord;
import dfsx.com.videodemo.record.RecordVideoImpl;
import dfsx.com.videodemo.test.TestProjectInfo;
import in.srain.cube.views.ptr.PtrFrameLayout;
import rx.Subscription;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;

import java.io.File;
import java.util.ArrayList;

/**
 * 我的项目页面
 */
public class ProjectFragment extends BasePullRefreshFragment {

    private SwipeMenuListView swipeMenuListView;
    private ProjectListAdapter adapter;
    private Handler handler = new Handler();
    private IRecord record;

    private Subscription recordSub;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return super.onCreateView(inflater, container, savedInstanceState);
    }

    @Override
    public View getPullRefreshContentView() {
        return LayoutInflater.from(context)
                .inflate(R.layout.frag_proj_layout, null);
        //        return null;
    }

    private void initHelperInstance() {
        record = new RecordVideoImpl(context);
    }

    @Override
    protected void setRefreshTopView(LinearLayout refreshTopContainer) {
        super.setRefreshTopView(refreshTopContainer);
        refreshTopContainer.addView(LayoutInflater.from(context)
                .inflate(R.layout.proj_header_layout, null));
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        initHelperInstance();
        iniTitleBar();
        initRegister();
        swipeMenuListView = (SwipeMenuListView) rootView.findViewById(R.id.proj_list_view);

        SwipeMenuCreator creator = new SwipeMenuCreator() {

            @Override
            public void create(SwipeMenu menu) {
                // create "open" item
                SwipeMenuItem editItem = new SwipeMenuItem(
                        context.getApplicationContext());
                // set item background
                editItem.setBackground(new ColorDrawable(Color.rgb(0xC9, 0xC9,
                        0xCE)));
                // set item width
                editItem.setWidth(PixelUtil.dp2px(context, 90));
                // set item title
                editItem.setIcon(R.mipmap.icon_edit_proj);
                // add to menu
                menu.addMenuItem(editItem);

                // create "delete" item
                SwipeMenuItem deleteItem = new SwipeMenuItem(
                        context.getApplicationContext());
                // set item background
                deleteItem.setBackground(new ColorDrawable(Color.rgb(0xF9,
                        0x3F, 0x25)));
                // set item width
                deleteItem.setWidth(PixelUtil.dp2px(context, 90));
                // set a icon
                deleteItem.setIcon(R.mipmap.icon_del_proj);
                // add to menu
                menu.addMenuItem(deleteItem);
            }
        };
        swipeMenuListView.setMenuCreator(creator);
        swipeMenuListView.setSwipeDirection(SwipeMenuListView.DIRECTION_LEFT);
        swipeMenuListView.setOnMenuItemClickListener(new SwipeMenuListView.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(int i, SwipeMenu swipeMenu, int index) {
                switch (index) {
                    case 0:
                        FragUtil.startEdit(context, null);
                        // edit
                        break;
                    case 1:
                        // delete
                        break;
                }
                // false : close the menu; true : not close the menu
                return false;
            }
        });

        swipeMenuListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

            }
        });

        adapter = new ProjectListAdapter(context);
        swipeMenuListView.setAdapter(adapter);
        setTestData();
    }

    @Override
    protected PtrFrameLayout.Mode getRefreshMode() {
        return PtrFrameLayout.Mode.BOTH;
    }

    private void iniTitleBar() {
        View goRecordView = rootView.findViewById(R.id.image_go_record);
        goRecordView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                File saveFile = new File(EditConstants.getVideoRecordDir(),
                        System.currentTimeMillis() + ".mp4");
                record.recordVideo(saveFile);
            }
        });
        View createNewProj = rootView.findViewById(R.id.right_image);
        createNewProj.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                WhiteTopBarActivity.startAct(context,
                        CreateNewProjectFragment.class.getName(), "本地视频");
            }
        });
    }

    private void initRegister() {
        recordSub = RxBus.getInstance().toObserverable(RXBusMessage.class)
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<RXBusMessage>() {
                    @Override
                    public void call(RXBusMessage rxBusMessage) {
                        if (TextUtils.equals(rxBusMessage.action, IRecord.MSG_RECORD_OK)) {
                            Uri uri = (Uri) rxBusMessage.getData();
                            if (uri != null) {
                                ArrayList<ISelector> list = new ArrayList<>();
                                Media media = new VideoMedia(uri.getPath(), 0);
                                list.add(media);
                                FragUtil.startEdit(context, list);
                            }
                        }
                    }
                });
    }

    private void unRegister() {
        if (recordSub != null) {
            recordSub.unsubscribe();
        }
    }

    private void setTestData() {
        ArrayList<IProjectInfo> list = getTestData(4);
        adapter.update(list, false);

    }

    private ArrayList<IProjectInfo> getTestData(int size) {
        ArrayList<IProjectInfo> list = new ArrayList();
        for (int i = 0; i < size; i++) {
            list.add(new TestProjectInfo());
        }
        return list;
    }

    @Override
    public void onRefreshBegin(PtrFrameLayout ptrFrameLayout) {
        super.onRefreshBegin(ptrFrameLayout);
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                setTestData();
                onRefreshComplete();
            }
        }, 2000);
    }

    @Override
    public void onLoadMoreBegin(PtrFrameLayout ptrFrameLayout) {
        super.onLoadMoreBegin(ptrFrameLayout);
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                adapter.update(getTestData(10), true);
                onRefreshComplete();
            }
        }, 2000);
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        unRegister();
    }
}
