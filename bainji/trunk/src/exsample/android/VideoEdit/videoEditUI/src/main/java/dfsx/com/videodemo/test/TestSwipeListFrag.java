package dfsx.com.videodemo.test;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import com.baoyz.swipemenulistview.SwipeMenu;
import com.baoyz.swipemenulistview.SwipeMenuCreator;
import com.baoyz.swipemenulistview.SwipeMenuItem;
import com.baoyz.swipemenulistview.SwipeMenuListView;
import com.dfsx.core.common.Util.PixelUtil;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.adapter.ProjectListAdapter;

import java.util.ArrayList;

public class TestSwipeListFrag extends Fragment {
    SwipeMenuListView swipeMenuListView;
    Context context;
    ProjectListAdapter adapter;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        context = getActivity();
        return inflater
                .inflate(R.layout.frag_proj_layout, null);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        swipeMenuListView = (SwipeMenuListView) view.findViewById(R.id.proj_list_view);

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
}
