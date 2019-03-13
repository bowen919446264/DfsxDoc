package dfsx.com.videodemo.proj;

import android.content.Context;

import com.dfsx.core.CoreApp;
import com.dfsx.core.file.FileUtil;
import com.dfsx.core.rx.RxBus;
import com.dfsx.videoeditor.util.EditConstants;
import com.dfsx.videoeditor.video.VideoSource;
import com.dfsx.videoeditor.widget.timeline.ITimeLineUI;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.frag.RXBusMessage;
import rx.Observable;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

public class ProjectManager {

    public static final String MSG_PROJECT_CHANGE = "dfsx.com.videodemo.proj.change";
    public static final String ProjectListFileName = "dfsx_edit_video_android.txt";

    private static ProjectManager instance = new ProjectManager();

    private IProjectInfo currentProjectInfo;

    private ITimeLineUI timeLineUI;
    private List<VideoSource> videoSourceList;

    public static ProjectManager getInstance() {
        return instance;
    }

    private ProjectManager() {

    }


    /**
     * 保存項目
     *
     * @param context
     * @param iProjectInfo
     */
    public static void saveProject(Context context, IProjectInfo iProjectInfo) {
        if (iProjectInfo != null && iProjectInfo.getProjId() != 0) {
            ArrayList<IProjectInfo> projectInfos = getSaveProjectList(context);
            int containIndex = hasContainProject(projectInfos, iProjectInfo);
            if (containIndex == -1) {
                projectInfos.add(iProjectInfo);
            } else {
                projectInfos.remove(containIndex);
                projectInfos.add(containIndex, iProjectInfo);
            }
            FileUtil.saveFile(EditConstants.getProjectDir(), ProjectListFileName, projectInfos);
            notifyProjectChange();
        }
    }

    public static void delProject(Context context, IProjectInfo iProjectInfo) {
        if (iProjectInfo != null && iProjectInfo.getProjId() != 0) {
            ArrayList<IProjectInfo> projectInfos = getSaveProjectList(context);
            int containIndex = hasContainProject(projectInfos, iProjectInfo);
            if (containIndex != -1) {
                projectInfos.remove(containIndex);
                FileUtil.saveFile(EditConstants.getProjectDir(), ProjectListFileName, projectInfos);
                notifyProjectChange();
            }
        }
    }

    public static void clearProjectInfo() {
        FileUtil.delAllFile(getProjectFilePath(CoreApp.getInstance()));
        LocalProject.clearLocalProjectId();
    }

    private static int hasContainProject(List<IProjectInfo> list, IProjectInfo projectInfo) {
        for (int i = 0; i < list.size(); i++) {
            if (list.get(i).getProjId() == projectInfo.getProjId()) {
                return i;
            }
        }
        return -1;
    }


    private static String getProjectFilePath(Context context) {
        String filePath = EditConstants.getProjectDir() + File.separator + ProjectListFileName;
        return filePath;
    }

    /**
     * 获取所用的项目
     *
     * @param context
     * @return
     */
    public static ArrayList<IProjectInfo> getSaveProjectList(Context context) {
        String filePath = getProjectFilePath(context);
        Object object = FileUtil.getFile(filePath);
        if (object != null && object instanceof ArrayList) {
            return (ArrayList<IProjectInfo>) object;
        }
        return new ArrayList<>();
    }

    public static void notifyProjectChange() {
        RxBus.getInstance().post(new RXBusMessage<String>(MSG_PROJECT_CHANGE, ""));
    }

    public IProjectInfo getCurrentProjectInfo() {
        return currentProjectInfo;
    }

    public void setCurrentProjectInfo(IProjectInfo currentProjectInfo) {
        this.currentProjectInfo = currentProjectInfo;
    }


    public void clearCurrentInfo() {
        this.currentProjectInfo = null;
        this.timeLineUI = null;
        this.videoSourceList = null;
    }

    public void save() {
        if (currentProjectInfo != null) {
            Observable.just(currentProjectInfo)
                    .observeOn(Schedulers.io())
                    .map(new Func1<IProjectInfo, Object>() {
                        @Override
                        public Object call(IProjectInfo iProjectInfo) {
                            saveProject(CoreApp.getInstance().getApplicationContext(), iProjectInfo);
                            return null;
                        }
                    }).subscribe();

        }
    }

    public ITimeLineUI getTimeLineUI() {
        return timeLineUI;
    }

    public void setTimeLineUI(ITimeLineUI timeLineUI) {
        this.timeLineUI = timeLineUI;
    }

    public List<VideoSource> getVideoSourceList() {
        return videoSourceList;
    }

    public void setVideoSourceList(List<VideoSource> videoSourceList) {
        this.videoSourceList = videoSourceList;
    }
}
