package dfsx.com.videodemo;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.ViewGroup;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import com.dfsx.videoeditor.video.FrameThumbInfo;
import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.video.VideoSourceHelper;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;
import com.dfsx.videoeditor.widget.timeline.ImageListView;
import rx.Observable;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

import java.util.ArrayList;
import java.util.List;

public class TestActivity extends Activity {

    RecyclerView recyclerView;
    ImageListView imageListView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.act_test);
        imageListView = (ImageListView) findViewById(R.id.video_source_view);
        recyclerView = (RecyclerView) findViewById(R.id.test_recycler_view);
        recyclerView.setLayoutManager(new LinearLayoutManager(this,
                LinearLayoutManager.HORIZONTAL, false));
        TestLongAdapter adapter = new TestLongAdapter();
        recyclerView.setAdapter(adapter);
        List<String> list = new ArrayList<>();
        for (int i = 0; i < 20; i++) {
            list.add("111111111111");
        }
        adapter.update(list, false);

        String testUrl = "/storage/emulated/0/2.mp4";
//        testUrl = "/storage/emulated/0/相机/video_20171111_170426.mp4";
        setImageListView(testUrl);
    }

    private void setImageListView(String sourceUrl) {
        Observable.just(sourceUrl)
                .observeOn(Schedulers.io())
                .map(new Func1<String, List<ImageListView.IFrameImage>>() {
                    @Override
                    public List<ImageListView.IFrameImage> call(String url) {
                        VideoSourceHelper sourceHelper = new VideoSourceHelper();
                        VideoSource source = sourceHelper.createVideoSource(url);
                        List<ImageListView.IFrameImage> list = new ArrayList<>();
                        if (source != null) {
                            float ratio = source.degree == 90 || source.degree == 270 ? source.height / ((float) source.width)
                                    : source.width / ((float) source.height);
                            //计算显示宽度。按宽高比例显示
                            float imageWidth = ratio * ITimeLineUI.TIMELINE_HEIGHT;
                            float perSecondsWidth = imageWidth / 3000;
                            for (FrameThumbInfo thumbInfo : source.frameThumbInfoList) {
                                int w = Math.round(perSecondsWidth * thumbInfo.durationTime);
                                thumbInfo.viewWidth = w;
                                list.add(thumbInfo);
                            }
                        }
                        return list;
                    }
                })
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<List<ImageListView.IFrameImage>>() {
                    @Override
                    public void call(List<ImageListView.IFrameImage> iFrameImages) {
                        imageListView.setUp(iFrameImages, 0, 0);
                    }
                });

    }


    class TestLongAdapter extends BaseRecyclerViewDataAdapter<String> {

        @Override
        public BaseRecyclerViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new BaseRecyclerViewHolder(R.layout.test_adapter_long, parent, viewType);
        }

        @Override
        public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {

        }
    }
}
