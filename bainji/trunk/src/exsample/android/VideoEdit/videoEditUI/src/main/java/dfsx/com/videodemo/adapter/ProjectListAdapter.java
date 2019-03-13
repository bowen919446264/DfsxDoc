package dfsx.com.videodemo.adapter;

import android.content.Context;
import android.widget.ImageView;
import android.widget.TextView;
import com.bumptech.glide.Glide;
import com.dfsx.core.common.adapter.BaseListViewAdapter;
import com.dfsx.core.common.adapter.BaseViewHodler;
import com.dfsx.core.img.GlideRoundTransform;
import com.dfsx.videoeditor.widget.timeline.TimeLineStringUtil;
import dfsx.com.videodemo.R;

import java.text.SimpleDateFormat;
import java.util.Date;

public class ProjectListAdapter extends BaseListViewAdapter<IProjectInfo> {
    public ProjectListAdapter(Context context) {
        super(context);
    }

    @Override
    public int getItemViewLayoutId() {
        return R.layout.adapter_proj_layout;
    }

    @Override
    public void setItemViewData(BaseViewHodler holder, int position) {
        TextView titleText = holder.getView(R.id.item_title_tv);
        TextView projTimeText = holder.getView(R.id.item_tv_proj_time);
        TextView mediaTimeText = holder.getView(R.id.item_tv_media_time);
        ImageView thumbImage = holder.getView(R.id.item_thumb_image);

        IProjectInfo projectInfo = list.get(position);

        titleText.setText(projectInfo.getProjTitle());
        projTimeText.setText(parseTime(projectInfo.getProjTime()));
        mediaTimeText.setText(TimeLineStringUtil.parseStringSecondTime(projectInfo.getProjMediaDuration()));

        String imagePath = projectInfo.getProjThumbPath();
        Glide.with(context)
                .load(imagePath)
                .asBitmap()
                .transform(new GlideRoundTransform(context, 10))
                .placeholder(R.drawable.glide_default_image)
                .error(R.drawable.glide_default_image)
                .into(thumbImage);
    }

    private String parseTime(long time) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd-HH:MM");
        return sdf.format(new Date(time));
    }
}
