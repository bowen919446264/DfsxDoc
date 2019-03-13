package dfsx.com.videodemo.frag;

import android.database.Cursor;
import android.graphics.Color;
import android.media.MediaExtractor;
import android.media.MediaFormat;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.Nullable;
import android.view.View;
import com.dfsx.core.common.view.CustomeProgressDialog;
import com.dfsx.videoeditor.util.MediaExtractorUtils;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.adapter.SelectedDecoration;
import dfsx.com.videodemo.adapter.SelectorRecyclerAdapter;
import rx.Observable;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

import java.util.ArrayList;

public class LocalVideoSelectedGridFragment extends SelectedGridFragment {
    private final static Uri MEDIA_EXTERNAL_CONTENT_URI = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
    private final static String MEDIA_DATA = MediaStore.Video.Media.DATA;

    private ISelector latestSelector;
    private ArrayList<ISelector> selectors;

    private boolean isSingleSelected = false;
    private CustomeProgressDialog progressDialog;


    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        selectors = new ArrayList<>();
        recyclerView.addItemDecoration(new SelectedDecoration(getContext(),
                Color.parseColor("#4c000000"), R.mipmap.icon_location_video_selected));
        adapter.setOnSelectorClickListener(new SelectorRecyclerAdapter.OnSelectorClickListener() {
            @Override
            public void onItemClick(View v, ISelector iSelector) {
                onSelectorItemClick(iSelector);
            }
        });
    }

    @Override
    protected void onUserPermissionGranted() {
        super.onUserPermissionGranted();
        //有权限的情况下获取数据
        initVideos();
    }

    protected void onSelectorItemClick(ISelector iSelector) {
        if (iSelector != null) {
            boolean isSelected = iSelector.isSelected();
            iSelector.setSelected(!isSelected);
            int pos = adapter.getData().indexOf(iSelector);
            int latestPos = -1;
            if (isSingleSelected) {
                if (latestSelector != null) {
                    latestSelector.setSelected(false);
                    selectors.remove(latestSelector);
                    latestPos = adapter.getData().indexOf(latestSelector);
                }
            }
            if (iSelector.isSelected()) {//选中
                latestSelector = iSelector;
                selectors.add(iSelector);
            } else {
                selectors.remove(iSelector);
                latestSelector = null;
            }
            adapter.notifyItemChanged(pos);
            if (isSingleSelected && latestPos != -1 && latestPos != pos) {
                adapter.notifyItemChanged(latestPos);
            }
            if (selectorChangeListener != null) {
                selectorChangeListener.onSelected(selectors);
            }
        }
    }

    public void setSingleSelected(boolean isSingleSelected) {
        this.isSingleSelected = isSingleSelected;
    }

    public ArrayList<ISelector> getSelectedList() {
        return selectors;
    }

    public void initVideos() {
        try {
            final String orderBy = MediaStore.Video.Media.DATE_TAKEN;
            //Here we set up a string array of the thumbnail ID column we want to get back

            String[] proj = {MediaStore.Video.Media.DATA, MediaStore.Video.Media._ID, MediaStore.Video.Media.LANGUAGE, MediaStore.Video.Media.LONGITUDE, MediaStore.Video.Media.LANGUAGE, MediaStore.Video.Media.DURATION};

            Cursor cursor = getActivity().getContentResolver().query(MEDIA_EXTERNAL_CONTENT_URI, proj, null, null, orderBy + " DESC");
            updateAdapterData(cursor);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void showProgress() {
        progressDialog = CustomeProgressDialog.show(getContext(), "加载中...");
    }

    private void updateAdapterData(Cursor cursor) {
        showProgress();
        int count = cursor.getCount();
        if (count > 0) {
            int mDataColumnIndex = cursor.getColumnIndex(MEDIA_DATA);
            int latfColumnIndex = cursor.getColumnIndex(MediaStore.Video.Media.LATITUDE);
            int longfColumnIndex = cursor.getColumnIndex(MediaStore.Video.Media.LONGITUDE);
            int durationColumnIndex = cursor.getColumnIndex(MediaStore.Video.Media.DURATION);
            //move position to first element
            cursor.moveToFirst();

            ArrayList<ISelector> mediaList = new ArrayList<>();
            for (int i = 0; i < count; i++) {
                cursor.moveToPosition(i);
                String url = cursor.getString(mDataColumnIndex);
                int length = 0;
                if (durationColumnIndex != -1)
                    length = cursor.getInt(durationColumnIndex);
                mediaList.add(new VideoMedia(url, length));
            }
            filterMediaList(mediaList, new Action1<ArrayList<ISelector>>() {
                @Override
                public void call(ArrayList<ISelector> iSelectors) {
                    adapter.update(iSelectors, false);
                    if (progressDialog != null) {
                        progressDialog.dismiss();
                    }
                }
            });

        }
    }

    protected void filterMediaList(ArrayList<ISelector> mediaList, Action1<ArrayList<ISelector>> afterFilterAction) {
        Observable.just(mediaList)
                .observeOn(Schedulers.io())
                .map(new Func1<ArrayList<ISelector>, ArrayList<ISelector>>() {
                    @Override
                    public ArrayList<ISelector> call(ArrayList<ISelector> iSelectors) {
                        ArrayList<ISelector> afterList = new ArrayList<>();
                        try {
                            for (ISelector selector : iSelectors) {
                                MediaExtractor extractor = null;
                                try {
                                    VideoMedia videoMedia = ((VideoMedia) selector);
                                    if (videoMedia.getSelectorLength() > 0) {
                                        extractor = new MediaExtractor();
                                        extractor.setDataSource(videoMedia.getUrl());
                                        MediaExtractorUtils.TrackResult trackResult = MediaExtractorUtils.getFirstVideoAndAudioTrack(extractor);
                                        if (isSelectedMediaFormate(trackResult.mAudioTrackFormat)) {
                                            afterList.add(selector);
                                        }
                                    }
                                } catch (Exception e) {
                                    e.printStackTrace();
                                } finally {
                                    if (extractor != null) {
                                        extractor.release();
                                    }
                                }
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        return afterList;
                    }
                })
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(afterFilterAction);
    }


    protected boolean isSelectedMediaFormate(MediaFormat audioFormat) {
//        int inputChannel = audioFormat.getInteger(MediaFormat.KEY_CHANNEL_COUNT);
//        int sampleRate = audioFormat.getInteger(MediaFormat.KEY_SAMPLE_RATE);
//        if (sampleRate != 44100) {
//            return false;
//        }
//        if (inputChannel != 1 && inputChannel != 2) {
//            return false;
//        }

        return true;
    }
}
