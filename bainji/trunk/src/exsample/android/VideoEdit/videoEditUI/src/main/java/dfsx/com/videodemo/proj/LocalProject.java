package dfsx.com.videodemo.proj;

import android.content.SharedPreferences;
import android.text.TextUtils;

import com.dfsx.core.CoreApp;
import com.dfsx.videoeditor.util.EditConstants;
import com.dfsx.videoeditor.video.VideoSource;

import dfsx.com.videodemo.adapter.IProjectInfo;

import java.util.ArrayList;
import java.util.List;

public class LocalProject implements IProjectInfo {
    private long projectId;
    private String title;
    private long createTime;
    private long latestChangeTime;
    private long projectContentLength;
    private String projectThumbPath;


    public LocalProject() {

    }

    public LocalProject(List<VideoSource> videoSources) {
        this(makeLocalProjectId(), null, videoSources);
    }

    public LocalProject(String title, long contenLength, String projImage) {
        projectId = makeLocalProjectId();
        String defaultTitle = "没有题目" + (projectId > 0 ? " " + projectId : "");
        this.title = TextUtils.isEmpty(title) ? defaultTitle : title;
        createTime = System.currentTimeMillis();
        latestChangeTime = createTime;
        projectContentLength = contenLength;
        projectThumbPath = projImage;
    }

    public LocalProject(long id, String title, List<VideoSource> videoSources) {
        projectId = id;
        String defaultTitle = "没有题目" + (projectId > 0 ? " " + projectId : "");
        this.title = TextUtils.isEmpty(title) ? defaultTitle : title;
        createTime = System.currentTimeMillis();
        latestChangeTime = createTime;
        if (videoSources == null) {
            projectContentLength = 0;
        } else {
            long temp = 0;
            for (VideoSource source : videoSources) {
                temp += source.getTimeLineDuration();
            }
            projectContentLength = temp;
        }
        projectThumbPath = videoSources != null && videoSources.size() > 0 ? videoSources.get(0).getSourceUrl()
                : null;
    }

    public void setProjectId(long projectId) {
        latestChangeTime = System.currentTimeMillis();
        this.projectId = projectId;
    }

    public void setTitle(String title) {
        latestChangeTime = System.currentTimeMillis();
        this.title = title;
    }

    public void setCreateTime(long createTime) {
        this.createTime = createTime;
    }

    public void setLatestChangeTime(long latestChangeTime) {
        this.latestChangeTime = latestChangeTime;
    }

    public void setProjectContentLength(long projectContentLength) {
        this.projectContentLength = projectContentLength;
    }

    public void updateChangeTime() {
        latestChangeTime = System.currentTimeMillis();
    }

    public void setProjectThumbPath(String projectThumbPath) {
        this.projectThumbPath = projectThumbPath;
    }

    @Override
    public long getProjId() {
        return projectId;
    }

    @Override
    public String getProjTitle() {
        return title;
    }

    @Override
    public long getProjTime() {
        return latestChangeTime;
    }

    @Override
    public long getProjMediaDuration() {
        return projectContentLength;
    }

    @Override
    public String getProjThumbPath() {
        return projectThumbPath;
    }

    @Override
    public String getProjConfigFilePath() {
        String dir = EditConstants.getProjectXeprojDir();
        return dir + title + ".xeproj";
    }


    /**
     * 生成本地的项目ID
     *
     * @return
     */
    public static long makeLocalProjectId() {
        SharedPreferences sp = CoreApp.getInstance().getApplicationContext()
                .getSharedPreferences("videodemo.proj", 0);
        long curUseId = sp.getLong("videodemo.proj.id", 0L);
        long nextId = curUseId + 1;
        sp.edit().putLong("videodemo.proj.id", nextId).commit();
        return nextId;
    }

    public static void clearLocalProjectId() {
        SharedPreferences sp = CoreApp.getInstance().getApplicationContext()
                .getSharedPreferences("videodemo.proj", 0);
        sp.edit().clear().commit();
    }
}
