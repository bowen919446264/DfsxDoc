package com.dfsx.videoeditor.util;

import android.os.Environment;

import com.dfsx.core.CoreApp;
import com.dfsx.core.common.Util.PixelUtil;

import java.io.File;

public class EditConstants {

    public static final int ONE_SECOND_VIEW_WIDTH = PixelUtil.dp2px(CoreApp.getInstance(), 25);
    public static final int SOURCE_DIVIDE_VIEW_WIDTH = PixelUtil.dp2px(CoreApp.getInstance(), 20);


    public static final String ROOT_DIR = Environment.getExternalStorageDirectory().getPath() + "/videoEdit/";

    /**
     * 解码视频帧的保存地址
     */
    public static final String FRAME_IMAGE_CACHE_DIR = ROOT_DIR;
    /**
     * 视频路劲保存地址
     */
    public static final String VIDEO_CACHE_DIR = ROOT_DIR + "video/";

    public static final String PROJ_CACHE_DIR = ROOT_DIR + "file/cache/";

    public static final String PROJ_LOG_DIR = ROOT_DIR + "file/log/";

    public static final String VIDEO_OUTPUT_DIR = ROOT_DIR + "videoOutput/";

    public static final String VIDEO_RECORD_DIR = ROOT_DIR + "videoRecord/";

    public static final String PROJECT_INFO_DIR = ROOT_DIR + "project_info/";

    public static final String PROJECT_XEPROJ_DIR = PROJECT_INFO_DIR + "project_xeproj/";


    public static String getProjCacheDir() {
        checkAndmkDir(ROOT_DIR);
        checkAndmkDir(ROOT_DIR + "file/");
        checkAndmkDir(PROJ_CACHE_DIR);
        return PROJ_CACHE_DIR;
    }

    public static String getProjLogDir() {
        checkAndmkDir(ROOT_DIR);
        checkAndmkDir(ROOT_DIR + "file/");
        checkAndmkDir(PROJ_LOG_DIR);
        return PROJ_LOG_DIR;
    }

    public static String getVideoOutputDir() {
        checkAndmkDir(ROOT_DIR);
        checkAndmkDir(VIDEO_OUTPUT_DIR);
        return VIDEO_OUTPUT_DIR;
    }

    private static void checkAndmkDir(String path) {
        File dir = new File(path);
        if (!dir.exists()) {
            dir.mkdirs();
        }
    }

    public static String getVideoRecordDir() {
        checkAndmkDir(ROOT_DIR);
        checkAndmkDir(VIDEO_RECORD_DIR);
        return VIDEO_RECORD_DIR;
    }

    public static String getProjectDir() {
        checkAndmkDir(ROOT_DIR);
        checkAndmkDir(PROJECT_INFO_DIR);
        return PROJECT_INFO_DIR;
    }

    public static String getProjectXeprojDir() {
        checkAndmkDir(ROOT_DIR);
        checkAndmkDir(PROJECT_INFO_DIR);
        checkAndmkDir(PROJECT_XEPROJ_DIR);
        return PROJECT_XEPROJ_DIR;
    }
}
