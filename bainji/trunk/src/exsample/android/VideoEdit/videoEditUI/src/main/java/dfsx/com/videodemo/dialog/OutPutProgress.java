package dfsx.com.videodemo.dialog;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v4.content.FileProvider;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import com.dfsx.videoeditor.out.IOut;
import dfsx.com.videodemo.R;

import java.io.File;

public class OutPutProgress extends BaseFullFragmentDialog implements IOut.Listener {

    private TextView tvProgress;
    private ProgressBar progressBar;
    private Button btnCancel;

    private Button btnPlay;

    private OnCancelClickListener clickListener;

    private Handler handler = new Handler();

    @Override
    public int getLayoutRes() {
        return R.layout.dialog_out_put_progress;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        tvProgress = (TextView) view.findViewById(R.id.tv_progress_text);
        progressBar = (ProgressBar) view.findViewById(R.id.progress_bar);
        btnCancel = (Button) view.findViewById(R.id.btn_cancel_output);
        btnPlay = (Button) view.findViewById(R.id.btn_play_output);

        setTvProgress(0);

        btnCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (clickListener != null) {
                    if (TextUtils.equals(btnCancel.getText(), "点击取消")) {
                        clickListener.onCancelClick(v);
                    }
                }
                dismiss();
            }
        });

        btnPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!TextUtils.isEmpty(outputPath)) {
                    String FILE_PROVIDER_AUTHORITY = getActivity().getResources().getString(R.string.app_share_dir_authorities);
                    File file = new File(outputPath);
                    Uri uri = FileProvider.getUriForFile(getActivity(), FILE_PROVIDER_AUTHORITY, file);
                    startActivity(new Intent(Intent.ACTION_VIEW)
                            .setDataAndType(uri, "video/mp4")
                            .setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION));
                }
            }
        });
    }

    public void setOnCancelClickListener(OnCancelClickListener listener) {
        this.clickListener = listener;
    }

    private void setTvProgress(int progress) {
        String text = "正在输出 " + progress + "%";
        setTvProgressText(text);
    }

    private void setButtonText(String text) {
        btnCancel.setText(text);
    }

    private void setTvProgressText(String text) {
        tvProgress.setText(text);
    }


    private String outputPath;

    public void setOutputPath(String path) {
        this.outputPath = path;
    }

    private void deleteOutputFile() {
        if (!TextUtils.isEmpty(outputPath)) {
            File tempFile = new File(outputPath);
            if (tempFile.exists()) {
                tempFile.deleteOnExit();
            }
        }
    }

    @Override
    public void onOutputProgress(final double progress) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                long progressNum = Math.round(progress * 100);
                if (progressNum < 0 || progressNum > 1500) {
                    progressNum = 0;
                }
                if (progressNum > 100) {
                    progressNum = 100;
                    setTvProgressText("文件整理中...");
                } else {
                    setTvProgress((int) progressNum);
                }
                progressBar.setProgress((int) progressNum);
            }
        });
    }

    @Override
    public void onOutputCompleted() {
        handler.post(new Runnable() {
            @Override
            public void run() {
                setTvProgressText("输出成功, 文件保存在：" + outputPath);
                setButtonText("确定");
                btnPlay.setVisibility(View.VISIBLE);
            }
        });
    }

    @Override
    public void onOutputCanceled() {
        deleteOutputFile();
    }

    @Override
    public void onOutputFailed(Exception exception) {
        exception.printStackTrace();
        handler.post(new Runnable() {
            @Override
            public void run() {
                deleteOutputFile();
                setTvProgressText("输出失败");
                setButtonText("确定");
            }
        });
    }


    public interface OnCancelClickListener {
        void onCancelClick(View v);
    }
}
