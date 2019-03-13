package dfsx.com.videodemo.frag;

public class VideoMedia extends Media {
    public VideoMedia(String url, long length) {
        super(url, length);
    }

    @Override
    public String getSelectorThumb() {
        return getUrl();
    }
}
