package dfsx.com.videodemo.adapter;

import java.io.Serializable;

/**
 * 项目的界面信息
 */
public interface IProjectInfo extends Serializable {

    long getProjId();

    String getProjTitle();

    long getProjTime();

    long getProjMediaDuration();

    String getProjThumbPath();

    String getProjConfigFilePath();
}
