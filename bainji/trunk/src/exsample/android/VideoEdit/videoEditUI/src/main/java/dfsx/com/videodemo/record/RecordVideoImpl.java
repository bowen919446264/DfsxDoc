package dfsx.com.videodemo.record;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

import java.io.File;

public class RecordVideoImpl implements IRecord {

    private Context context;

    public RecordVideoImpl(Context context) {
        this.context = context;
    }

    @Override
    public void recordVideo(File saveFile) {
        Uri uri = Uri.fromFile(saveFile);
        Intent intent = new Intent(context, SystemRecordActivity.class);
        intent.putExtra(SystemRecordActivity.KEY_RECORD_VIDEO_SAVE_PATH, uri.toString());
        context.startActivity(intent);
    }
}
