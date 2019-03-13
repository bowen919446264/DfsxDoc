package dfsx.com.videodemo.frag;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.text.TextUtils;

import com.dfsx.core.common.act.DefaultFragmentActivity;
import com.dfsx.core.common.act.WhiteTopBarActivity;

import java.io.Serializable;
import java.util.List;

import dfsx.com.videodemo.adapter.IProjectInfo;
import dfsx.com.videodemo.adapter.ISelector;
import dfsx.com.videodemo.solution1.EngineEditVideoFragment;

public class FragUtil {

    public static LocalVideoSelectedGridFragment addVideoSelectedFragent(FragmentManager fragmentManager,
                                                                         int contentId, String tag) {
        Fragment frag = fragmentManager.findFragmentByTag(tag);
        LocalVideoSelectedGridFragment selectedMiediaFrag;
        if (frag == null) {
            selectedMiediaFrag = new LocalVideoSelectedGridFragment();
            frag = selectedMiediaFrag;
            fragmentManager.beginTransaction().add(contentId, frag, tag)
                    .commitAllowingStateLoss();
        } else {
            selectedMiediaFrag = (LocalVideoSelectedGridFragment) frag;
            fragmentManager.beginTransaction().show(frag)
                    .commitAllowingStateLoss();
        }
        return selectedMiediaFrag;
    }


    public static void startSingleMediaSelect(Context context) {
        startSingleMediaSelect(context, null);
    }

    /**
     * 选择单个媒体
     *
     * @param context
     */
    public static void startSingleMediaSelect(Context context, String returnAction) {
        if (TextUtils.isEmpty(returnAction)) {
            WhiteTopBarActivity.startAct(context, LocalMediaSelectFragment.class.getName(),
                    "本地视频", "完成");
        } else {
            Bundle bundle = new Bundle();
            bundle.putString(LocalMediaSelectFragment.KEY_RETURN_ACTION, returnAction);
            WhiteTopBarActivity.startAct(context, LocalMediaSelectFragment.class.getName(),
                    "本地视频", "完成", bundle);
        }

    }

    /**
     * 启动编辑界面
     *
     * @param context
     * @param editSelectors 选中的编辑素材
     */
    public static void startEdit(Context context, List<ISelector> editSelectors) {
        Bundle bundle = new Bundle();
        if (editSelectors != null) {
            bundle.putSerializable("edit_media_source", (Serializable) editSelectors);
        }
        DefaultFragmentActivity.start(context, EngineEditVideoFragment.class.getName(), bundle);
    }

    /**
     * 启动编辑界面
     *
     * @param context
     * @param editProject 编辑项目
     */
    public static void startEditProject(Context context, IProjectInfo editProject) {
        Bundle bundle = new Bundle();
        if (editProject != null) {
            bundle.putSerializable("edit_project", editProject);
        }
        DefaultFragmentActivity.start(context, EngineEditVideoFragment.class.getName(), bundle);
    }
}