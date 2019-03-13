package com.dfsx.videoeditor.widget.timeline;

public class TimeLineStringUtil {

    public static String parseStringTime(long time) {
        String formatStr = "%02d:%02d:%02d.%03d"; //HOURS:MM:SS.MICROSECONDS
        int hour = (int) (time / 3600 / 1000);
        int mm = (int) (time / 1000 % 3600 / 60);
        int ss = (int) (time / 1000 % 3600 % 60);
        int mil = (int) (time % 1000);
        return String.format(formatStr, hour, mm, ss, mil);
    }

    public static String parseStringSecondTime(long time) {
        String formatStr = "%02d:%02d:%02d"; //HOURS:MM:SS
        int hour = (int) (time / 3600 / 1000);
        int mm = (int) (time / 1000 % 3600 / 60);
        int ss = (int) (time / 1000 % 3600 % 60);
        return String.format(formatStr, hour, mm, ss);
    }
}
