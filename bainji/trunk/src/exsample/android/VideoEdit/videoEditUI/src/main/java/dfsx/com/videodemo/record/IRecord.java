package dfsx.com.videodemo.record;

import java.io.File;

public interface IRecord {

    String MSG_RECORD_OK = "IRecord.MSG_RECODE_OK";

    /**
     * 录制视频
     *
     * @param saveFile 视频的输出文件
     */
    void recordVideo(File saveFile);
}
