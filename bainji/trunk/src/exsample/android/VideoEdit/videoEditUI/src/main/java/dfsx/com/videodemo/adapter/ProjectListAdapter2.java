package dfsx.com.videodemo.adapter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import com.bumptech.glide.Glide;
import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import com.dfsx.core.img.GlideRoundTransform;
import com.dfsx.videoeditor.widget.timeline.TimeLineStringUtil;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.proj.ProjectManager;

import java.text.SimpleDateFormat;
import java.util.Date;

public class ProjectListAdapter2 extends BaseRecyclerViewDataAdapter<IProjectInfo> {

    private Activity activity;

    private OnEventListener eventListener;

    public ProjectListAdapter2(Activity activity) {
        this.activity = activity;
    }

    @Override
    public BaseRecyclerViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new BaseRecyclerViewHolder(R.layout.adapter_project_2_layout, parent, viewType);
    }

    @Override
    public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {
        ImageView thumbImageView = holder.getView(R.id.project_thumb);
        View projInfoView = holder.getView(R.id.project_info_view);
        TextView tvTitle = holder.getView(R.id.tv_project_title);
        TextView tvProjTime = holder.getView(R.id.project_time);
        TextView tvContentTime = holder.getView(R.id.tv_project_content_time);


        if (position < list.size()) {
            IProjectInfo projectInfo = list.get(position);
            thumbImageView.setTag(thumbImageView.getId(), projectInfo);
            Glide.with(activity)
                    .load(projectInfo.getProjThumbPath())
                    .asBitmap()
                    .transform(new GlideRoundTransform(activity, 2))
                    .placeholder(R.drawable.glide_default_image)
                    .error(R.drawable.glide_default_image)
                    .into(thumbImageView);
            projInfoView.setVisibility(View.VISIBLE);
            tvTitle.setText(projectInfo.getProjTitle());
            tvProjTime.setText(parseTime(projectInfo.getProjTime()));
            tvContentTime.setText(TimeLineStringUtil.parseStringSecondTime(projectInfo.getProjMediaDuration()));
        } else {
            projInfoView.setVisibility(View.GONE);
            thumbImageView.setTag(thumbImageView.getId(), null);
            thumbImageView.setImageResource(R.drawable.create_proj_bg);
        }

        thumbImageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Object tag = v.getTag(v.getId());
                if (tag != null && tag instanceof IProjectInfo) {
                    if (eventListener != null) {
                        eventListener.onProjectClick(v, (IProjectInfo) tag);
                    }
                } else {
                    if (eventListener != null) {
                        eventListener.onAddProjectClick(v);
                    }
                }
            }
        });

        thumbImageView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                Object tag = v.getTag(v.getId());
                if (tag != null && tag instanceof IProjectInfo) {
                    delProject((IProjectInfo) tag);
                    return true;
                } else {
                }
                return false;
            }
        });
    }

    private void delProject(final IProjectInfo projectInfo) {
        AlertDialog alertDialog = new AlertDialog.Builder(activity, android.R.style.Theme_Material_Light_Dialog_Alert)
                .setPositiveButton("确定", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        ProjectManager.delProject(activity, projectInfo);
                    }
                })
                .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                })
                .setTitle("提示")
                .setMessage("删除项目？")
                .create();
        if (!activity.isFinishing()) {
            alertDialog.show();
        }

    }

    private String parseTime(long time) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:MM");
        return sdf.format(new Date(time));
    }

    @Override
    public int getItemCount() {
        return super.getItemCount() + 1;
    }


    public void setOnEventListener(OnEventListener listener) {
        this.eventListener = listener;
    }

    public interface OnEventListener {
        void onProjectClick(View v, IProjectInfo projectInfo);

        void onAddProjectClick(View v);
    }
}
