package dfsx.com.videodemo.record;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.util.Log;
import com.dfsx.core.common.act.BaseActivity;
import com.dfsx.core.rx.RxBus;
import dfsx.com.videodemo.frag.RXBusMessage;

public class SystemRecordActivity extends BaseActivity {
    public static final String KEY_RECORD_VIDEO_SAVE_PATH = "SystemRecordActivity_KEY_RECORD_VIDEO_SAVE_PATH";
    private static final int CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE = 10;

    private Uri fileUri;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getIntent() != null) {
            String url = getIntent().getStringExtra(KEY_RECORD_VIDEO_SAVE_PATH);
            if (!TextUtils.isEmpty(url)) {
                fileUri = Uri.parse(url);
            }
        }
        if (fileUri != null) {
            recordVideo();
        } else {
            finish();
        }
    }

    public void recordVideo() {
        Intent intent = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
        intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri);  // set the image file name
        intent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY, 1); // set the video image quality to high
        // start the Video Capture Intent
        startActivityForResult(intent, CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE) {
            if (resultCode == RESULT_OK) {
                // Video captured and saved to fileUri specified in the Intent
                Log.e("TAG", "record video ok === ");
                //                getContentResolver().notifyChange(MediaStore.Video.Media.EXTERNAL_CONTENT_URI, null);
                RxBus.getInstance().post(new RXBusMessage<Uri>(IRecord.MSG_RECORD_OK, fileUri));
            }
        }
        finish();
    }
}
