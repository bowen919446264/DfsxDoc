package dfsx.com.videodemo.test;

import dfsx.com.videodemo.adapter.IProjectInfo;

public class TestProjectInfo implements IProjectInfo {
    @Override
    public long getProjId() {
        return 0;
    }

    @Override
    public String getProjTitle() {
        return "我是标题";
    }

    @Override
    public long getProjTime() {
        return System.currentTimeMillis();
    }

    @Override
    public long getProjMediaDuration() {
        return 50300;
    }

    @Override
    public String getProjThumbPath() {
        return "https://ss1.baidu.com/6ONXsjip0QIZ8tyhnq/it/u=2271299185,4081567485&fm=173&app=25&f=JPEG?w=630&h=419&s=E6F3C07E4F3572110468C15F00004073";
    }

    @Override
    public String getProjConfigFilePath() {
        return null;
    }
}
