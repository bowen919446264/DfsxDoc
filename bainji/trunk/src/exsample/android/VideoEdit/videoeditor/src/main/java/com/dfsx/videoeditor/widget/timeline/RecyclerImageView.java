package com.dfsx.videoeditor.widget.timeline;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.util.AttributeSet;
import android.widget.ImageView;

/**
 * 主要想解决RecyclerView 加载图片显示不出来的问题
 */
@SuppressLint("AppCompatCustomView")
public class RecyclerImageView extends ImageView {

    private BitmapDrawable mRecycleableBitmapDrawable;

    public RecyclerImageView(Context context) {
        super(context);
    }

    public RecyclerImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public RecyclerImageView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public RecyclerImageView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    @Override
    public void setImageBitmap(Bitmap bm) {
        mRecycleableBitmapDrawable = new BitmapDrawable(getResources(), bm);
        setImageDrawable(mRecycleableBitmapDrawable);
    }
}
