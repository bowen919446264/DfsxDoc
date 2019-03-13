package com.dfsx.pullrefresh;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewStub;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import in.srain.cube.views.ptr.PtrClassicFrameLayout;
import in.srain.cube.views.ptr.PtrDefaultHandler2;
import in.srain.cube.views.ptr.PtrFrameLayout;
import in.srain.cube.views.ptr.PtrHandler;

public abstract class BasePullRefreshFragment extends Fragment {
    protected Activity activity;
    protected Context context;

    protected View rootView;
    protected PtrClassicFrameLayout refreshLayout;
    protected LinearLayout refreshTopContainer, refreshBottomContainer;
    protected View contentView;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        activity = getActivity();
        context = getContext();
        View v = inflater.inflate(R.layout.frag_base_refresh_layout, container, false);
        rootView = v;
        refreshLayout = (PtrClassicFrameLayout) v.findViewById(R.id.refresh_ptr_frame);
        refreshTopContainer = (LinearLayout) v.findViewById(R.id.top_pull_refresh_container);
        refreshBottomContainer = (LinearLayout) v.findViewById(R.id.bottom_pull_refresh_container);

        setRefreshTopView(refreshTopContainer);
        setRefreshBottomView(refreshBottomContainer);

        contentView = getPullRefreshContentView();
        if (contentView != null) {
            ((FrameLayout) v.findViewById(R.id.refresh_container)).addView(contentView);
        }
        setUpRefreshLayout();
        return v;
    }

    protected void setRefreshTopView(LinearLayout refreshTopContainer) {

    }

    protected void setRefreshBottomView(LinearLayout refreshBottomContainer) {

    }

    public abstract View getPullRefreshContentView();

    private void setUpRefreshLayout() {
        refreshLayout.setLastUpdateTimeHeaderRelateObject(this);
        refreshLayout.setMode(getRefreshMode());
        refreshLayout.setPtrHandler(getPtrHandler());
        // the following are default settings
        refreshLayout.setResistance(1.7f);
        refreshLayout.setRatioOfHeaderHeightToRefresh(1.2f);
        refreshLayout.setDurationToClose(200);
        refreshLayout.setDurationToCloseHeader(1000);
        // default is false
        refreshLayout.setPullToRefresh(false);
        // default is true
        refreshLayout.setKeepHeaderWhenRefresh(true);
    }

    protected PtrFrameLayout.Mode getRefreshMode() {
        return PtrFrameLayout.Mode.BOTH;
    }

    protected PtrHandler getPtrHandler() {
        return new DefaultPtrHandler();
    }

    public void onRefreshComplete() {
        if (refreshLayout != null) {
            refreshLayout.refreshComplete();
        }
    }

    /**
     * 下拉刷新
     *
     * @param ptrFrameLayout
     */
    public void onRefreshBegin(PtrFrameLayout ptrFrameLayout) {

    }

    /**
     * 加载更多
     *
     * @param ptrFrameLayout
     */
    public void onLoadMoreBegin(PtrFrameLayout ptrFrameLayout) {

    }

    class DefaultPtrHandler extends PtrDefaultHandler2 {

        @Override
        public void onLoadMoreBegin(PtrFrameLayout ptrFrameLayout) {
            BasePullRefreshFragment.this.onLoadMoreBegin(ptrFrameLayout);
        }

        @Override
        public void onRefreshBegin(PtrFrameLayout ptrFrameLayout) {
            BasePullRefreshFragment.this.onRefreshBegin(ptrFrameLayout);
        }

        @Override
        public boolean checkCanDoRefresh(PtrFrameLayout frame, View content, View header) {
            if (contentView == null) {
                return false;
            }
            return checkContentCanBePulledDown(frame, contentView, header);
        }

        public boolean checkCanDoLoadMore(PtrFrameLayout frame, View content, View footer) {
            if (contentView == null) {
                return false;
            }
            return checkContentCanBePulledUp(frame, contentView, footer);
        }
    }

}
