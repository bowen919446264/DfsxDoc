package com.ds.xedit.androidexsample;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.ds.xedit.jni.*;
import com.gun0912.tedpermission.PermissionListener;
import com.gun0912.tedpermission.TedPermission;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback, SeekBar.OnSeekBarChangeListener {

    private LogReceiver     logReceiver     = new LogReceiver();    // 日志接收者
    private IXEngine        engine          = IXEngine.getSharedInstance();
    private ITimeLine       timeLine        = engine.getTimeLine();
    private String          projectName     = "current";
    private NativeHelper.VideoRenderer videoRenderer   = new NativeHelper.VideoRenderer();
    private SurfaceHolder   surfaceHolder;
    private SeekBar         playProcessBar;
    private IMedia          currentMedia;
    private ITrack          currentTrack;
    private IClip           currentClip;
    private ITimeLineObserver timeLineObserver = new TimeLineObserver(this);
    private GenerateObserver generateObserver = new GenerateObserver(this);
    private ProjectSetting  setting = new ProjectSetting();

    private static final int REQUEST_MEDIA_CODE = 0x1000;
    private static final int REQUEST_OPEN_PROJECT_CODE = REQUEST_MEDIA_CODE + 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        playProcessBar = findViewById(R.id.playProcessBar);
        playProcessBar.setOnSeekBarChangeListener(this);
        initializeEngine();
    }

    /**
     * 提取assets中的文件到缓存目录
     * @param context
     * @param fileName
     * @return
     */
    public String getAssetsCacheFile(Context context, String fileName)   {
        File cacheFile = new File(context.getCacheDir(), fileName);
        try {
            java.io.InputStream inputStream = context.getAssets().open(fileName);
            try {
                FileOutputStream outputStream = new FileOutputStream(cacheFile);
                try {
                    byte[] buf = new byte[1024];
                    int len;
                    while ((len = inputStream.read(buf)) > 0) {
                        outputStream.write(buf, 0, len);
                    }
                } finally {
                    outputStream.close();
                }
            } finally {
                inputStream.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return cacheFile.getAbsolutePath();
    }

    /**
     * 引擎初始化
     */
    private void initializeEngine() {
        // 检查CPU和GPU性能，看哪一种解码更快
        String samplePath = getAssetsCacheFile(this, "sample.mp4");
        boolean useGpu = IXEngine.checkIfGpuFasterThanCpu(samplePath);

        // 工程设置
        setting.setEPixFormat(EPixFormat.EPIX_FMT_NV12);
        setting.setESampleFormat(ESampleFormat.EAV_SAMPLE_FMT_S16);
        setting.setNSampleRate(48000);
        setting.setNBitsPerSample(16);
        setting.setNChannelCount(2);
        setting.setNHeight(720);
        setting.setNWidth(1280);
        setting.setRFrameRate(new Rational(25, 1));
        setting.setBInterlaced(false);
        setting.setBTopFieldFirst(false);
        setting.setRAspectRatio(new Rational(16, 9));

        // 引擎设置
        EngineSetting engineSetting = new EngineSetting();
        engineSetting.setCacheDir(Environment.getExternalStorageDirectory().getAbsolutePath() + "/xeproj/cache");
        engineSetting.setLogDir(Environment.getExternalStorageDirectory().getAbsolutePath() + "/xeproj/log");
        //engineSetting.setUseGpuToDecode(useGpu);  目前GPU下行颜色转换比CPU更慢，先使用CPU解码
        engineSetting.setUseGpuToDecode(false);
//        GSize size = new GSize();
//        size.setNWidth(240);
//        size.setNHeight(135);
//        engineSetting.setPreviewFrameSize(size);

        // 引擎初始化
        engine.initialize(engineSetting);

        // 添加一个日志接收者
        xedit.AVLogAddReceiver(logReceiver);

        // 添加一个时间线观察者
        timeLine.addObserver(timeLineObserver);

        // 添加renderer
        // 暂时不用添加音频renderer，因为底层会自动播放音频
        timeLine.addRenderer(videoRenderer);

        // 初始化播放窗口
        SurfaceView surfaceView = findViewById(R.id.surfaceView);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
    }

    /**
     * 测试保存工程
     */
    private void testSaveProject() {
        try {
            // 创建工程目录
            String projDir = Environment.getExternalStorageDirectory().getAbsolutePath() +  "/xeproj";
            File projDirFile = new File(projDir);
            if (!projDirFile.exists()) {
                if (!projDirFile.mkdir()) {
                    throw new Exception("创建工程目录 " + projDir + " 失败!");
                }
            }

            // 打开输出流
            COutputFileStream outputStream = new COutputFileStream();
            String url = projDir + "/" + projectName + ".xeproj";
            long code = outputStream.open(url);
            if (code < 0) {
                throw new Exception("打开输出流" + url + "失败!");
            }

            // 保存工程
            code = engine.saveProject(outputStream);
            if (code < 0) {
                throw new Exception("保存工程失败!");
            }

            outputStream.flush();
            outputStream.close();

            Toast.makeText(getApplicationContext(),"保存工程成功。路径: " + url, Toast.LENGTH_SHORT).show();
        } catch (Exception ex) {
            Toast.makeText(getApplicationContext(),"保存工程失败！", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 保存工程
     * @param view
     */
    public void onSaveProject(View view) {
        TedPermission.with(MainActivity.this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                testSaveProject();
            }
            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(MainActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(MainActivity.this.getResources().getString(R.string.denied_message))
                .setPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .check();
    }

    /**
     * 测试新建工程
     */
    private void testNewProject() {
        try {
            // 新建工程
            int code = engine.newProject(projectName, setting);
            if (code < 0) {
                throw new Exception("新建工程失败!");
            }

            Toast.makeText(getApplicationContext(),"新建工程成功", Toast.LENGTH_SHORT).show();
        } catch(Exception ex) {
            Toast.makeText(getApplicationContext(),"新建工程失败！" + ex.getMessage(), Toast.LENGTH_SHORT).show();
            Log.e("XEngine", ex.getMessage());
        }
    }

    /**
     * 新建工程
     * @param view
     */
    public void onNewProject(View view) {
        TedPermission.with(MainActivity.this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                testNewProject();
            }
            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(MainActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(MainActivity.this.getResources().getString(R.string.denied_message))
                .setPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .check();
    }

    /**
     * 载入demo工程
     * @param view
     */
    public void onOpenProject(View view) {
        TedPermission.with(MainActivity.this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                try {
//                    File file = new File(Environment.getExternalStorageDirectory(), "xeproj");
//                    if(null == file || !file.exists()){
//                        return;
//                    }
                    Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                    //Uri uri = FileProvider.getUriForFile(getApplicationContext(),BuildConfig.APPLICATION_ID + ".provider", file);
                    //intent.setDataAndType(uri, "*/*");
                    intent.setType("*/*");
                    intent.addCategory(Intent.CATEGORY_OPENABLE);

                    startActivityForResult(intent, REQUEST_OPEN_PROJECT_CODE);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(MainActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(MainActivity.this.getResources().getString(R.string.denied_message))
                .setPermissions(Manifest.permission.READ_EXTERNAL_STORAGE)
                .check();
    }

    /**
     * 关闭当前工程
     * @param view
     */
    public void onCloseProject(View view) {
        int code = engine.closeCurrentProject();
        if (code >= 0) {
            Toast.makeText(MainActivity.this, "工程已关闭", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(MainActivity.this, "关闭工程失败!", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 测试添加图片
     */
    private void testAddImage() {
        try {
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("image/*");
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            startActivityForResult(intent, REQUEST_MEDIA_CODE);
        } catch (Exception ex) {
            Toast.makeText(getApplicationContext(),"引入文件失败！" + ex.getMessage(), Toast.LENGTH_SHORT).show();
            Log.e("XEngine", ex.getMessage());
        }
    }

    /**
     * 添加图片
     * @param view
     */
    public void onAddImage(View view) {
        TedPermission.with(MainActivity.this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                testAddImage();
            }
            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(MainActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(MainActivity.this.getResources().getString(R.string.denied_message))
                .setPermissions(Manifest.permission.READ_EXTERNAL_STORAGE)
                .check();
    }

    /**
     * 测试添加视频
     */
    private void testAddVideo() {
        try {
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("video/*");
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            startActivityForResult(intent, REQUEST_MEDIA_CODE);
        } catch (Exception ex) {
            Toast.makeText(getApplicationContext(),"引入文件失败！" + ex.getMessage(), Toast.LENGTH_SHORT).show();
            Log.e("XEngine", ex.getMessage());
        }
    }

    /**
     * 添加视频
     * @param view
     */
    public void onAddVideo(View view) {
        TedPermission.with(MainActivity.this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                testAddVideo();
            }
            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(MainActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(MainActivity.this.getResources().getString(R.string.denied_message))
                .setPermissions(Manifest.permission.READ_EXTERNAL_STORAGE)
                .check();
    }

    /**
     * 在轨道指定位置添加切片(该位置不存在切片)
     * @param offsetOnTrack
     */
    private void addClip(Rational offsetOnTrack) {
        if (currentMedia == null) {
            Toast.makeText(MainActivity.this, "请先引入媒体!", Toast.LENGTH_SHORT).show();
            return;
        }

        try {
            if (currentMedia.getMediaType() == EMediaType.EMediaType_AV) {
                IAVMedia avMedia = IAVMedia.dynamic_cast(currentMedia);
                AVMediaInfo mediaInfo = new AVMediaInfo();
                avMedia.getMediaInfo(mediaInfo);
                if (mediaInfo.getNVideoCount() > 0) {
                    ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
                    IClip clip = track.addClip(currentMedia.getId(), offsetOnTrack);
                    if (clip != null) {
                        currentTrack = track;
                        currentClip = clip;
                        Toast.makeText(MainActivity.this, "创建切片成功", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(MainActivity.this, "创建切片失败!", Toast.LENGTH_SHORT).show();
                    }
                } else if (mediaInfo.getNAudioCount() > 0) {
//                    ITrack track = timeLine.getTrack(ETrackType.ETrackType_Audio, 0);
//                    AudioStream[] audioStreams = mediaInfo.getAStreams();
//                    IClip clip = track.addAVStreamClip(currentMedia.getId(), offsetOnTrack, audioStreams[0].getNIndex());
//                    if (clip != null) {
//                        currentClip = clip;
//                        Toast.makeText(MainActivity.this, "创建切片成功", Toast.LENGTH_SHORT).show();
//                    } else {
//                        Toast.makeText(MainActivity.this, "创建切片失败!", Toast.LENGTH_SHORT).show();
//                    }
                }
            } else if (currentMedia.getMediaType() == EMediaType.EMediaType_Image) {
                ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
                IClip clip = track.addClip(currentMedia.getId(), offsetOnTrack);
                if (clip != null) {
                    currentClip = clip;
                    Toast.makeText(MainActivity.this, "创建切片成功", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(MainActivity.this, "创建切片失败!", Toast.LENGTH_SHORT).show();
                }
            }
        } catch (Exception e) {
            Log.e("onAddClip", e.toString());
        }
    }

    /**
     * 创建切片
     * @param view
     */
    public void onAddClip(View view) {
        addClip(timeLine.getDuration());
    }

    /**
     * 在第一个切片后添加切片
     * @param view
     */
    public void addClipAfterFirstClip(View view) {
        if (currentClip == null) {
            Toast.makeText(MainActivity.this, "请先创建切片!", Toast.LENGTH_SHORT).show();
            return;
        }

        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        IClip firstClip = track.getClip(0);
        addClip(firstClip.getDuration());
    }
    /**
     * 删除切片
     * @param view
     */
    public void onRemoveClip(View view) {
        if (currentClip == null) {
            Toast.makeText(MainActivity.this, "请先创建切片!", Toast.LENGTH_SHORT).show();
            return;
        }

        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        int code = track.removeClipById(currentClip.getId());
        if (code < 0) {
            Toast.makeText(MainActivity.this, "删除切片失败!", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(MainActivity.this, "删除切片成功!", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 第一种情况，移动后，切片的相对位置不变(序号不变)
     */
    private void moveClipCase1() {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        track.removeAllClips();
        currentClip = null;

        IAVMedia iavMedia = IAVMedia.dynamic_cast(currentMedia);
        AVMediaInfo mediaInfo = new AVMediaInfo();
        iavMedia.getMediaInfo(mediaInfo);

        // 构造数据
        IClip firstClip = track.addClip(currentMedia.getId(), new Rational());
        IClip secondClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration());
        IClip thirdClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(3));

        // 移动中间切片
        IDPtr idPtr = new IDPtr();
        IClip leftClip = new IClip();
        IClip rightClip = new IClip();
        int code = track.moveClip(secondClip.getId(), secondClip.getOffsetOnTrack().add(secondClip.getDuration().divide(2)), idPtr.cast(), leftClip, rightClip);
        if (code < 0) {
            Toast.makeText(MainActivity.this, "移动切片失败!", Toast.LENGTH_SHORT).show();
            return;
        }

        long splitedClipId = idPtr.value();
        if (splitedClipId > 0) {// 移动过程中，有切片被拆分了
            long leftClipId = leftClip.getId();     // 被拆分切片的左侧部分
            long rightClipId = rightClip.getId();   // 被拆分切片的右侧部分
        }
    }

    /**
     * 第二种情况，移动后，切片的相对位置变小(序号变小)
     */
    private void moveClipCase2() {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        track.removeAllClips();
        currentClip = null;

        IAVMedia iavMedia = IAVMedia.dynamic_cast(currentMedia);
        AVMediaInfo mediaInfo = new AVMediaInfo();
        iavMedia.getMediaInfo(mediaInfo);

        // 构造数据
        IClip firstClip = track.addClip(currentMedia.getId(), new Rational());
        IClip secondClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration());
        IClip thirdClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(2));
        IClip fourthClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(3));

        // 将第三个切片移动到第一个切片末尾
        IDPtr idPtr = new IDPtr();
        IClip leftClip = new IClip();
        IClip rightClip = new IClip();
        int code = track.moveClip(thirdClip.getId(), firstClip.getOffsetOnTrack().add(firstClip.getDuration()), idPtr.cast(), leftClip, rightClip);
        if (code < 0) {
            Toast.makeText(MainActivity.this, "移动切片失败!", Toast.LENGTH_SHORT).show();
            return;
        }

        long splitedClipId = idPtr.value();
        if (splitedClipId > 0) {// 移动过程中，有切片被拆分了
            long leftClipId = leftClip.getId();     // 被拆分切片的左侧部分
            long rightClipId = rightClip.getId();   // 被拆分切片的右侧部分
        }
    }

    /**
     * 第三种情况，移动后，切片的相对位置变大(序号变大)
     */
    private void moveClipCase3() {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        track.removeAllClips();
        currentClip = null;

        IAVMedia iavMedia = IAVMedia.dynamic_cast(currentMedia);
        AVMediaInfo mediaInfo = new AVMediaInfo();
        iavMedia.getMediaInfo(mediaInfo);

        // 构造数据
        IClip firstClip = track.addClip(currentMedia.getId(), new Rational());
        IClip secondClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration());
        IClip thirdClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(2));
        //IClip fourthClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(3));

        // 将第二个切片移动到第三个切片末尾
        IDPtr idPtr = new IDPtr();
        IClip leftClip = new IClip();
        IClip rightClip = new IClip();
        int code = track.moveClip(secondClip.getId(), thirdClip.getOffsetOnTrack().add(thirdClip.getDuration()), idPtr.cast(), leftClip, rightClip);
        if (code < 0) {
            Toast.makeText(MainActivity.this, "移动切片失败!", Toast.LENGTH_SHORT).show();
            return;
        }

        long splitedClipId = idPtr.value();
        if (splitedClipId > 0) {// 移动过程中，有切片被拆分了
            long leftClipId = leftClip.getId();     // 被拆分切片的左侧部分
            long rightClipId = rightClip.getId();   // 被拆分切片的右侧部分
        }
    }

    /**
     * 第四种情况，移动目标位置已有切片
     */
    private void moveClipCase4() {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        track.removeAllClips();
        currentClip = null;

        IAVMedia iavMedia = IAVMedia.dynamic_cast(currentMedia);
        AVMediaInfo mediaInfo = new AVMediaInfo();
        iavMedia.getMediaInfo(mediaInfo);

        // 构造数据
        IClip firstClip = track.addClip(currentMedia.getId(), new Rational());
        IClip secondClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration());
        IClip thirdClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(2));
        IClip fourthClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(4));

        // 将第2个切片移动到第3个切片中间
        IDPtr idPtr = new IDPtr();
        IClip leftClip = new IClip();
        IClip rightClip = new IClip();
        int code = track.moveClip(secondClip.getId(), thirdClip.getOffsetOnTrack().add(thirdClip.getDuration().divide(2)), idPtr.cast(), leftClip, rightClip);
        if (code < 0) {
            Toast.makeText(MainActivity.this, "移动切片失败!", Toast.LENGTH_SHORT).show();
            return;
        }

        long splitedClipId = idPtr.value();
        if (splitedClipId > 0) {// 移动过程中，有切片被拆分了
            long leftClipId = leftClip.getId();     // 被拆分切片的左侧部分
            long rightClipId = rightClip.getId();   // 被拆分切片的右侧部分
        }
    }

    /**
     * 移动切片
     * 假设目标位置为p
     * 1）移动后切片序号不变，移动后切片的位置为p
     * 2) 移动后切片的序号变小（即向左移动），移动后切片的位置为p
     * 3) 移动后切片的序号变大（即向右移动），移动后切片的位置小于p（因为考虑到手机编辑实际情况，移动后不改变整个时间线长度）
     * @param view
     */
    public void onMoveClip(View view) {
        moveClipCase1();
        moveClipCase2();
        moveClipCase3();
        moveClipCase4();
    }

    /**
     * 添加切片预览
     * @param view
     */
    public void onAddPreviewFrame(View view) {
        if (currentMedia == null) {
            Toast.makeText(MainActivity.this, "请先引入媒体!", Toast.LENGTH_SHORT).show();
            return;
        }

        if (currentMedia.getMediaType() == EMediaType.EMediaType_AV) {
            IAVMedia avMedia = IAVMedia.dynamic_cast(currentMedia);
            avMedia.openPreviewSession();

            // 创建预览帧
            AVMediaInfo mediaInfo = new AVMediaInfo();
            avMedia.getMediaInfo(mediaInfo);
            VideoStream videoStream = mediaInfo.getVStreams()[0];
            for (int i = 0; i < videoStream.getNFrameCount(); i += 10000) {
                Rational offsetInMedia = new Rational(i, 25);
                PreviewFrame previewFrame = avMedia.createPreviewFrame(0, offsetInMedia);
                if(previewFrame != null) {
                    Log.e("TAG", "cretae ok ==== "  + previewFrame.getPath());
                }
                if (previewFrame == null) {
                    Toast.makeText(MainActivity.this, "添加切片预览失败!", Toast.LENGTH_SHORT).show();
                    avMedia.closePreviewSession();
                    return;
                }
            }
            Toast.makeText(MainActivity.this, "添加切片预览成功", Toast.LENGTH_SHORT).show();
            avMedia.closePreviewSession();

            // 遍历预览帧
            IPreview preview = avMedia.getPreview(0);
            if (preview != null) {
                Rational offsetInMedia = new Rational(5, 1);
                PreviewFrame previewFrame = preview.getPreviewFrameNearBy(offsetInMedia);
                if (previewFrame != null) {
                    ImageView previewImageView = findViewById(R.id.previewFrame);
                    try {
                        Uri uri = Uri.fromFile(new File(previewFrame.getPath()));
                        previewImageView.setImageURI(uri);
                    } catch (Exception e) {
                        Log.e("", e.toString());
                    }
                }
            }
        } else if (currentMedia.getMediaType() == EMediaType.EMediaType_Image) {
            IPreview preview = currentMedia.getPreview();
            if (preview == null) {
                Toast.makeText(MainActivity.this, "获取图片预览失败!", Toast.LENGTH_SHORT).show();
                return;
            }
            if (preview.getPreviewFrameCount() > 0) {
                PreviewFrame previewFrame = preview.getPreviewFrame(0);
                ImageView previewImageView = findViewById(R.id.previewFrame);
                try {
                    Uri uri = Uri.fromFile(new File(previewFrame.getPath()));
                    previewImageView.setImageURI(uri);
                } catch (Exception e) {
                    Log.e("", e.toString());
                }
            }
        } else {
            Toast.makeText(MainActivity.this, "只有视频或图片才能创建预览帧!", Toast.LENGTH_SHORT).show();
            return;
        }
    }

    /**
     * 拆分切片(将一个切片拆分成2个)
     * @param view
     */
    public void onSplitClip(View view) {
        if (currentClip == null) {
            Toast.makeText(MainActivity.this, "请先创建切片!", Toast.LENGTH_SHORT).show();
            return;
        }

        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video,0);
        Rational splitOffsetInClip = currentClip.getDuration().divide(new Rational(2, 1));
        IClip leftClip = new IClip();
        IClip rightClip = new IClip();
        int code = track.splitClip(currentClip.getId(), splitOffsetInClip, leftClip, rightClip);
        if (code < 0) {
            Toast.makeText(MainActivity.this, "拆分切片失败!", Toast.LENGTH_SHORT).show();
            return;
        } else {
            Toast.makeText(MainActivity.this, "拆分切片成功", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 设置切片在时间线上的偏移量
     * @param view
     */
    public void onSetClipOffset(View view) {
//        if (currentClip == null) {
//            Toast.makeText(MainActivity.this, "请先创建切片!", Toast.LENGTH_SHORT).show();
//            return;
//        }
//
//        Rational offset = currentClip.getOffsetOnTrack();
//        offset.add(new Rational(5, 1));
//        ITrack videoTrack = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
//        IDPtr idPtr = new IDPtr();
//        IClip leftClip = new IClip();
//        IClip rightClip = new IClip();
//        int code = videoTrack.moveClip(currentClip.getId(), offset, idPtr.cast(), leftClip, rightClip);
//        Toast.makeText(MainActivity.this, "设置切片在时间线上的偏移量成功", Toast.LENGTH_SHORT).show();
    }

    private void setClipDurationCase1() {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        track.removeAllClips();
        currentClip = null;

        IAVMedia iavMedia = IAVMedia.dynamic_cast(currentMedia);
        AVMediaInfo mediaInfo = new AVMediaInfo();
        iavMedia.getMediaInfo(mediaInfo);

        // 构造数据
        IClip firstClip = track.addClip(currentMedia.getId(), new Rational());
        IClip secondClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration());
        IClip thirdClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(2));

        int code = track.changeClipDuration(secondClip.getId(), mediaInfo.getRDuration().divide(2));
        if (code < 0) {
            Toast.makeText(MainActivity.this, "设置切片时长失败！", Toast.LENGTH_SHORT).show();
            return;
        }

        code = track.changeClipDuration(secondClip.getId(), mediaInfo.getRDuration());
        if (code < 0) {
            Toast.makeText(MainActivity.this, "设置切片时长失败！", Toast.LENGTH_SHORT).show();
            return;
        }

        Toast.makeText(MainActivity.this, "设置切片时长成功", Toast.LENGTH_SHORT).show();
    }

    /**
     * 设置切片时长
     * @param view
     */
    public void onSetClipDuration(View view) {
        setClipDurationCase1();
    }

    private void setClipOffsetInMediaCase1() {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        track.removeAllClips();
        currentClip = null;

        IAVMedia iavMedia = IAVMedia.dynamic_cast(currentMedia);
        AVMediaInfo mediaInfo = new AVMediaInfo();
        iavMedia.getMediaInfo(mediaInfo);

        // 构造数据
        IClip firstClip = track.addClip(currentMedia.getId(), new Rational());
        IClip secondClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration());
        IClip thirdClip = track.addClip(currentMedia.getId(), mediaInfo.getRDuration().multiply(2));

        int code = track.changeClipOffsetInMedia(secondClip.getId(), mediaInfo.getRDuration().divide(2));
        if (code < 0) {
            Toast.makeText(MainActivity.this, "设置视音频切片在文件内的偏移量失败！", Toast.LENGTH_SHORT).show();
            return;
        }

        code = track.changeClipOffsetInMedia(secondClip.getId(), Rational.getZero());
        if (code < 0) {
            Toast.makeText(MainActivity.this, "设置视音频切片在文件内的偏移量失败！", Toast.LENGTH_SHORT).show();
            return;
        }

        Toast.makeText(MainActivity.this, "设置视音频切片在文件内的偏移量成功", Toast.LENGTH_SHORT).show();
    }

    /**
     * 设置视音频切片在文件内的偏移量
     * @param view
     */
    public void onSetAVClipOffsetInMedia(View view) {
        setClipOffsetInMediaCase1();
    }

    /**
     * 获取切片在文件内的偏移量
     * @param view
     */
    public void onGetAVClipOffsetInMedia(View view) {
        IAVClip avClip = IAVClip.dynamic_cast(currentClip);
        Rational offsetInMedia = avClip.getOffsetInMedia();
        Toast.makeText(MainActivity.this, "切片在文件内的偏移量是: " + offsetInMedia.getNNum() + "/" + offsetInMedia.getNDen(), Toast.LENGTH_SHORT).show();
    }

    /**
     * 通过时间查找切片
     * @param view
     */
    public void onFindClipByTime(View view) {
        ITrack track = timeLine.getTrack(ETrackType.ETrackType_Video, 0);
        if (track == null) {
            Toast.makeText(MainActivity.this, "没有轨道!", Toast.LENGTH_SHORT).show();
            return;
        }

        IClip clip = track.findClip(new Rational(5, 1));
        if (clip == null) {
            Toast.makeText(MainActivity.this, "没有在轨道上找到对应的切片!", Toast.LENGTH_SHORT).show();
            return;
        }

        Toast.makeText(MainActivity.this, "成功找到切片", Toast.LENGTH_SHORT).show();
    }

    /**
     * 添加视频轨道
     * @param view
     */
    public void onCreateVideoTrack(View view) {
        ITrack track = timeLine.newTrack(ETrackType.ETrackType_Video);
        if (track != null) {
            Toast.makeText(getApplicationContext(),"创建视频轨道成功", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(getApplicationContext(), "创建视频轨道失败!", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 添加音频轨道
     * @param view
     */
    public void onCreateAudioTrack(View view) {
        ITrack track = timeLine.newTrack(ETrackType.ETrackType_Audio);
        if (track != null) {
            Toast.makeText(getApplicationContext(),"创建音频轨道成功", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(getApplicationContext(), "创建音频轨道失败!", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 撤销操作
     * @param view
     */
    public void onUnDo(View view) {
        if (timeLine.canUndo()) {
            IAction action = timeLine.undo();
            if (action == null) {
                Toast.makeText(getApplicationContext(), "撤销动作失败!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(getApplicationContext(), "撤销动作成功", Toast.LENGTH_SHORT).show();

                switch (action.getActionType()) {
                    case AT_AddClip: {
                        String trackId = action.getActionParam(xedit.AP_AddClip_TrackId);
                        String mediaId = action.getActionParam(xedit.AP_AddClip_MediaId);
                        String offsetOnTrack = action.getActionParam(xedit.AP_AddClip_OffsetOnTrack);
                        String clipId = action.getActionParam(xedit.AP_AddClip_ClipId);
                        break;
                    }
                    case AT_AddMedia: {
                        String mediaPath = action.getActionParam(xedit.AP_AddMedia_MediaPath);
                        break;
                    }
                    case AT_ChangeClipDuration: {
                        String clipId = action.getActionParam(xedit.AP_ChangeClipDuration_ClipId);
                        String newDuration = action.getActionParam(xedit.AP_ChangeClipDuration_NewDuration);
                        String oldDuration = action.getActionParam(xedit.AP_ChangeClipDuration_OldDuration);
                        break;
                    }
                    case AT_MoveClip: {
                        String clipId = action.getActionParam(xedit.AP_MoveClip_ClipId);
                        String newOffsetOnTrack = action.getActionParam(xedit.AP_MoveClip_NewOffsetOnTrack);
                        String oldOffsetOnTrack = action.getActionParam(xedit.AP_MoveClip_OldOffsetOnTrack);
                        break;
                    }
                    case AT_RemoveClip: {
                        String clipId = action.getActionParam(xedit.AP_RemoveClip_ClipId);
                        break;
                    }
                    case AT_RemoveMedia: {
                        String mediaId = action.getActionParam(xedit.AP_RemoveMedia_MediaId);
                        break;
                    }
                    case AT_ChangeClipOffsetInMedia: {
                        String clipId = action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_ClipId);
                        String newOffsetInMedia = action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_NewOffsetInMedia);
                        String oldOffsetInMedia = action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_OldOffsetInMedia);
                        break;
                    }
                }
            }
        } else {
            Toast.makeText(getApplicationContext(), "没有动作可以撤销", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 重做操作
     * @param view
     */
    public void onReDo(View view) {
        if (timeLine.canRedo()) {
            IAction action = timeLine.redo();
            if (action == null) {
                Toast.makeText(getApplicationContext(), "重做动作失败!", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(getApplicationContext(), "重做动作成功", Toast.LENGTH_SHORT).show();

                switch (action.getActionType()) {
                    case AT_AddClip: {
                        String trackId = action.getActionParam(xedit.AP_AddClip_TrackId);
                        String mediaId = action.getActionParam(xedit.AP_AddClip_MediaId);
                        String offsetOnTrack = action.getActionParam(xedit.AP_AddClip_OffsetOnTrack);
                        String clipId = action.getActionParam(xedit.AP_AddClip_ClipId);
                    }
                    break;
                    case AT_AddMedia: {
                        String mediaPath = action.getActionParam(xedit.AP_AddMedia_MediaPath);
                        break;
                    }
                    case AT_ChangeClipDuration: {
                        String clipId = action.getActionParam(xedit.AP_ChangeClipDuration_ClipId);
                        String newDuration = action.getActionParam(xedit.AP_ChangeClipDuration_NewDuration);
                        String oldDuration = action.getActionParam(xedit.AP_ChangeClipDuration_OldDuration);
                        break;
                    }
                    case AT_MoveClip: {
                        String clipId = action.getActionParam(xedit.AP_MoveClip_ClipId);
                        String newOffsetOnTrack = action.getActionParam(xedit.AP_MoveClip_NewOffsetOnTrack);
                        String oldOffsetOnTrack = action.getActionParam(xedit.AP_MoveClip_OldOffsetOnTrack);
                        break;
                    }
                    case AT_RemoveClip: {
                        String clipId = action.getActionParam(xedit.AP_RemoveClip_ClipId);
                        break;
                    }
                    case AT_RemoveMedia: {
                        String mediaId = action.getActionParam(xedit.AP_RemoveMedia_MediaId);
                        break;
                    }
                    case AT_ChangeClipOffsetInMedia: {
                        String clipId = action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_ClipId);
                        String newOffsetInMedia = action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_NewOffsetInMedia);
                        String oldOffsetInMedia = action.getActionParam(xedit.AP_ChangeClipOffsetInMedia_OldOffsetInMedia);
                        break;
                    }
                }
            }
        } else {
            Toast.makeText(getApplicationContext(), "没有动作可以重做", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 测试打开工程
     * @param projectPath
     */
    private void testOpenProject(String projectPath) {
        try {
            CInputStream inputStream = new CInputStream();
            int code = inputStream.open(projectPath);
            if (code < 0) {
                throw new Exception("打开文件 " + projectPath + " 失败!");
            }

            code = engine.openProject(inputStream);
            if (code < 0) {
                throw new Exception("打开工程失败!");
            }

            Toast.makeText(getApplicationContext(),"载入工程成功。路径: " + projectPath, Toast.LENGTH_SHORT).show();
        } catch (Exception ex) {
            Toast.makeText(getApplicationContext(),"载入工程失败！" + ex.getMessage(), Toast.LENGTH_SHORT).show();
            Log.e("XEngine", ex.getMessage());
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        try {
            if (resultCode == RESULT_OK && null != data) {
                if (requestCode == REQUEST_MEDIA_CODE) {
                    Uri uri = data.getData();
                    final String mediaPath = FileUtil.getPath(MainActivity.this, uri);
                    IMedia media = timeLine.addMedia(mediaPath);
                    if (media == null) {
                        Toast.makeText(getApplicationContext(), "添加媒体失败!" + timeLine.getLastErrorMessage(), Toast.LENGTH_SHORT).show();
                    } else {
                        currentMedia = media;
                        Toast.makeText(getApplicationContext(), "添加媒体成功", Toast.LENGTH_SHORT).show();
                    }
                } else if (requestCode == REQUEST_OPEN_PROJECT_CODE) {
                    // 媒体路径
                    Uri uri = data.getData();
                    final String projectPath = FileUtil.getPath(MainActivity.this, uri);
                    testOpenProject(projectPath);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    /**
     * 播放
     * @param view
     */
    public void onPlay(View view) {
        timeLine.play();
    }

    /**
     * 暂停
     * @param view
     */
    public void onPause(View view) {
        timeLine.pause();
    }

    /**
     * 测试时间线生成
     */
    public void testGenerate() {
        String projDir = Environment.getExternalStorageDirectory().getAbsolutePath() +  "/xeproj/generated";
        File projDirFile = new File(projDir);
        if (!projDirFile.exists()) {
            if (!projDirFile.mkdir()) {
                Toast.makeText(getApplicationContext(), "创建生成目录 " + projDir + " 失败!", Toast.LENGTH_SHORT).show();
            }
        }

        GenerateSetting setting = new GenerateSetting();
        setting.setStrDestDir(projDir);
        setting.setStrDestName(projectName + ".mp4");
        setting.setRStartTime(new Rational(0, 1));
        setting.setRDuration(timeLine.getDuration());

        EncodeParam encodeParam = new EncodeParam();
        encodeParam.setEMuxerType(EMuxerType.EMUXER_TYPE_MOV);

        VideoCodecParam videoCodecParam = new VideoCodecParam();
        videoCodecParam.setBInterlaced(false);
        videoCodecParam.setBTopFieldFirst(false);
        videoCodecParam.setEPixFormat(EPixFormat.EPIX_FMT_NV12);
        videoCodecParam.setNWidth(1280);
        videoCodecParam.setNHeight(720);
        videoCodecParam.setEBitrateMode(EBitrateMode.EBITRATE_MODE_VBR);
        videoCodecParam.setECodecID(ECodecID.ECODEC_ID_H264);
        videoCodecParam.setECodecLevel(ECodecLevel.ECODEC_LEVEL_NONE);
        videoCodecParam.setECodecProfile(ECodecProfile.ECODEC_PROFILE_NONE);
        videoCodecParam.setNBitrate(8*1000000);
        encodeParam.setVideoParam(videoCodecParam);

        AudioCodecParam audioCodecParam = new AudioCodecParam();
        audioCodecParam.setESampleFmt(ESampleFormat.EAV_SAMPLE_FMT_S16);
        audioCodecParam.setNBitsPerSample(16);
        audioCodecParam.setNChannels(2);
        audioCodecParam.setNSampleRate(48000);
        audioCodecParam.setEBitrateMode(EBitrateMode.EBITRATE_MODE_VBR);
        audioCodecParam.setECodecID(ECodecID.ECODEC_ID_MP3);
        audioCodecParam.setECodecLevel(ECodecLevel.ECODEC_LEVEL_NONE);
        audioCodecParam.setECodecProfile(ECodecProfile.ECODEC_PROFILE_NONE);
        audioCodecParam.setNBitrate(128000);
        encodeParam.setAudioParam(audioCodecParam);

        setting.setEncodeParam(encodeParam);

        TextView totalFrameView = findViewById(R.id.totalFramesTextView);
        long totalFrames = new Rational(timeLine.getDuration().getNNum() * 25, timeLine.getDuration().getNDen()).integerValue();
        totalFrameView.setText(totalFrames + "");

        int code = timeLine.generate(setting, generateObserver);
        if (code < 0) {
            Toast.makeText(MainActivity.this, "生成失败!", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * 时间线生成
     * @param view
     */
    public void onGenerate(View view) {
        TedPermission.with(MainActivity.this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
                testGenerate();
            }
            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(MainActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(MainActivity.this.getResources().getString(R.string.denied_message))
                .setPermissions(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .check();
    }

    /**
     * 取消生成
     * @param view
     */
    public void onCancelGenerate(View view) {
        timeLine.cancelGenerate();
        Toast.makeText(MainActivity.this, "已取消生成", Toast.LENGTH_SHORT).show();
    }

    /**
     * 清理缓存
     * @param view
     */
    public void onClearCaches(View view) {
        engine.clearCaches();
    }

    /**
     * 清理日志
     * @param view
     */
    public void onClearLogs(View view) {
        engine.clearLogs();
    }

    public void surfaceCreated(final SurfaceHolder surfaceHolder) {
        NativeHelper.setupNativeWindow(surfaceHolder.getSurface(), setting.getNWidth(), setting.getNHeight());
    }

    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        Rational duration = timeLine.getDuration();
        Rational offset = new Rational(duration.getNNum() * progress / 100, duration.getNDen());
        timeLine.seek(offset);
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }

    static {
        try {
            System.loadLibrary("native-lib");
        } catch(Throwable ex){
            ex.printStackTrace();
            Log.e("LoadNativeLibraries", ex.getMessage());
        }
    }
}
