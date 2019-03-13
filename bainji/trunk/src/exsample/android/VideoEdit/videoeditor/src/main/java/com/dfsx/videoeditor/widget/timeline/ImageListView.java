package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.graphics.*;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.bumptech.glide.Glide;
import com.dfsx.videoeditor.R;

import java.util.List;

public class ImageListView extends LinearLayout {
    /**
     * 拖拽的位置
     */
    private DragPosition dragPos = DragPosition.none;

    private Context context;
    private List<IFrameImage> frameImageList;
    private RectF leftDragRectF;
    private RectF rightDragRectF;
    private Paint testPaint;

    private int width;
    private int height;

    private int MAXWIDTH;

    private Canvas canvas;

    private OnDragSizeChangeListener changeListener;

    private boolean onTouchDragEnable = false;

    public ImageListView(Context context) {
        this(context, null);
    }

    public ImageListView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public ImageListView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.context = context;
        init();
    }

    private void init() {
        testPaint = new Paint();
        testPaint.setColor(Color.parseColor("#99ed4040"));
        testPaint.setStyle(Paint.Style.FILL);
        leftDragRectF = new RectF();
        rightDragRectF = new RectF();
        setOrientation(HORIZONTAL);
    }

    public void setUp(List<IFrameImage> list, int leftScrollHideWidth, int rightScrollHideWidth) {
        this.frameImageList = list;
        this.leftScrollPadding = -leftScrollHideWidth;
        this.rightScrollPadding = -rightScrollHideWidth;
        MAXWIDTH = getAllWidth();
        width = MAXWIDTH + leftScrollPadding + rightScrollPadding;
        if (width != getWidth() || height != getHeight()) {
            requestLayout();
        }
        post(new Runnable() {
            @Override
            public void run() {
                drawFrameList();
                setPadding(leftScrollPadding, 0, rightScrollPadding, 0);
            }
        });
    }

    @Override
    protected void onDraw(Canvas canvas) {
        this.canvas = canvas;
        super.onDraw(canvas);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        if (frameImageList == null || frameImageList.isEmpty()) {
            super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        } else {
            //Note 注意在调用的时候计算完毕数据
            Log.e("TAG", "image list view width == " + width);
            height = frameImageList.get(0).getFrameHeight();
            super.onMeasure(MeasureSpec.makeMeasureSpec(width, MeasureSpec.EXACTLY),
                    MeasureSpec.makeMeasureSpec(height, MeasureSpec.EXACTLY));
        }
    }

    @Override
    protected void dispatchDraw(Canvas canvas) {
        super.dispatchDraw(canvas);
        //        if (leftDragRectF != null && !leftDragRectF.isEmpty()) {
        //            canvas.drawRect(leftDragRectF, testPaint);
        //        }
        //        if (rightDragRectF != null && !rightDragRectF.isEmpty()) {
        //            canvas.drawRect(rightDragRectF, testPaint);
        //        }
    }

    private int getAllWidth() {
        int w = 0;
        for (IFrameImage frame : frameImageList) {
            w += frame.getFrameWidth();
        }
        return w;
    }

    private void drawFrameList() {
        removeAllViews();
        if (frameImageList == null || width == 0 || height == 0) {
            return;
        }
        for (int i = 0; i < frameImageList.size(); i++) {
            IFrameImage frameImage = frameImageList.get(i);
            ImageView imageView = createImageView(frameImage.getFrameWidth(), frameImage.getFrameHeight());
            addView(imageView);
            Glide.with(context).load(frameImage.getFrameImage())
                    .asBitmap()
                    .placeholder(R.drawable.glide_default_image)
                    .error(R.drawable.glide_default_image)
                    .dontAnimate()
                    .into(imageView);
        }
        postInvalidate();
    }

    private ImageView createImageView(int width, int height) {
        ImageView imageView = new RecyclerImageView(context);
        LayoutParams p = new LayoutParams(width, height);
        imageView.setLayoutParams(p);
        imageView.setScaleType(ImageView.ScaleType.CENTER_CROP);
        return imageView;
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        width = right - left;
        height = bottom - top;
        leftDragRectF.set(0, 0, 100, height);
        rightDragRectF.set(width - 100, 0, width, height);
        postInvalidate();
    }

    private boolean isDrag = false;
    private boolean isLeftDrag = false;
    private boolean isRightDrag = false;
    private float pointX;
    private float pointY;
    //
    //    @Override
    //    public boolean onInterceptTouchEvent(MotionEvent ev) {
    //        int action = ev.getAction();
    //        switch (action) {
    //            case MotionEvent.ACTION_DOWN:
    //                float x = ev.getX();
    //                float y = ev.getY();
    //                isLeftDrag = leftDragRectF.contains(x, y);
    //                isRightDrag = rightDragRectF.contains(x, y);
    //                isDrag = isLeftDrag || isRightDrag;
    //                break;
    //        }
    //        return isDrag || super.onInterceptTouchEvent(ev);
    //    }

    /**
     * 获取这个image list 的显示的第一个图片
     *
     * @return
     */
    public String getFirstImagePath() {
        if (frameImageList == null || width == 0 || height == 0) {
            return null;
        }
        int indexWidth = 0;
        for (IFrameImage frameImage : frameImageList) {
            indexWidth += frameImage.getFrameWidth();
            if (indexWidth >= Math.abs(leftScrollPadding)) {
                return frameImage.getFrameImage();
            }
        }
        return null;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (onTouchDragEnable) {
            int action = event.getAction();
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                    float x = event.getX();
                    float y = event.getY();
                    pointX = x;
                    pointY = y;
                    isLeftDrag = leftDragRectF.contains(x, y);
                    isRightDrag = rightDragRectF.contains(x, y);
                    isDrag = isLeftDrag || isRightDrag;
                    if (isDrag) {
                        dragStart();
                    }
                    break;
                case MotionEvent.ACTION_MOVE:
                    if (isDrag) {
                        float mx = event.getX();
                        float my = event.getY();
                        float dx = mx - pointX;
                        float dy = my - pointY;
                        pointX = mx;
                        pointY = my;
                        Log.e("TAG", "ACTION_MOVE == " + dx);
                        if (Math.abs(dx) > Math.abs(dy)) {
                            if (isLeftDrag) {
                                leftDrag((int) dx);
                            }
                            if (isRightDrag) {
                                rightDrag((int) dx);
                            }
                        }
                    }
                    break;
                case MotionEvent.ACTION_CANCEL:
                case MotionEvent.ACTION_UP:
                    if (isDrag) {
                        onDragEnd();
                    }
                    break;
            }
        }
        return super.onTouchEvent(event);
    }

    /**
     * 右边的拖动
     */
    public int rightDrag(int dx) {
        dragPos = DragPosition.right;
        int changeDx = 0;
        int w = width + dx;
        if (w > MAXWIDTH - Math.abs(leftScrollPadding)) {
            w = MAXWIDTH - Math.abs(leftScrollPadding);
        }
        rightScrollPadding += dx;
        if (rightScrollPadding > 0) {
            rightScrollPadding = 0;
        }
        if (w != width) {
            changeDx = w - width;
            width = w;
            setPadding(leftScrollPadding, 0, rightScrollPadding, 0);
            requestLayout();
            if (changeListener != null) {
                changeListener.onDragSizeChange(this, w, changeDx, Math.abs(leftScrollPadding), Math.abs(rightScrollPadding));
            }
        }

        return changeDx;
    }

    /**
     * 开始拖动
     */
    public void dragStart() {
        if (changeListener != null) {
            changeListener.onDragStart(this);
        }
    }

    /**
     * 左边移除的距离 为负数， 最大值0，
     */
    private int leftScrollPadding;
    /**
     * 右边移除的距离 为负数，最大值0，
     */
    private int rightScrollPadding;

    /**
     * 左边的拖动
     */
    public int leftDrag(int dx) {
        dragPos = DragPosition.left;
        int changeDx = 0;
        int w = width - dx;
        leftScrollPadding -= dx;
        if (w > MAXWIDTH) {
            w = MAXWIDTH;
        }
        if (leftScrollPadding > 0) {
            leftScrollPadding = 0;
        }
        if (w != width) {
            changeDx = w - width;
            width = w;
            setPadding(leftScrollPadding, 0, rightScrollPadding, 0);
            requestLayout();
            if (changeListener != null) {
                changeListener.onDragSizeChange(this, w, changeDx, Math.abs(leftScrollPadding), Math.abs(rightScrollPadding));
            }
        }

        return changeDx;
    }

    public void onDragEnd() {
        if (changeListener != null) {
            changeListener.onDragEnd(this, dragPos, width, Math.abs(leftScrollPadding), Math.abs(rightScrollPadding));
        }
        dragPos = DragPosition.none;
    }

    public void setOnDragSizeChangeListener(OnDragSizeChangeListener listener) {
        this.changeListener = listener;
    }

    public void setOnTouchDragEnable(boolean onTouchDragEnable) {
        this.onTouchDragEnable = onTouchDragEnable;
    }


    public interface IFrameImage {

        int getFrameWidth();

        int getFrameHeight();

        String getFrameImage();
    }


    public interface OnDragSizeChangeListener {

        void onDragStart(View v);

        /**
         * @param v
         * @param viewWidth        变化后的界面宽度
         * @param dWidth           变化的宽度值, 大于0 说明变宽， 小于0变窄
         * @param leftAllHidSize   左边总共隐藏的大小
         * @param rightAllHideSize 右边总共隐藏的大小
         */
        void onDragSizeChange(View v, int viewWidth, int dWidth, int leftAllHidSize, int rightAllHideSize);

        /**
         * 拖拽结束
         *
         * @param v
         * @param dragPos          拖动的位置
         * @param viewWidth
         * @param leftAllHidSize
         * @param rightAllHideSize
         */
        void onDragEnd(View v, DragPosition dragPos, int viewWidth, int leftAllHidSize, int rightAllHideSize);
    }

    public enum DragPosition {
        none,
        left,
        right
    }
}
