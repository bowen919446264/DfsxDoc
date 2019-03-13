package com.dfsx.videoeditor.widget.timeline;

import android.content.Context;
import android.graphics.*;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import com.dfsx.core.CoreApp;
import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.videoeditor.R;

/**
 * TimeLine 的选中效果
 * 包括开始位置，结束位置的图片， 整个选中的背景
 *
 * @author liuwb
 */
public class SelectedControlView extends View {

    public final static int HANDLE_NONE = -1;
    public final static int HANDLE_START = 0;
    public final static int HANDLE_END = 1;

    private final static int MOTION_NONE = 0;
    private final static int MOTION_DRAG_START_HANDLE = 1;
    private final static int MOTION_DRAG_END_HANDLE = 2;

    private static final int TIME_TEXT_SIZE = PixelUtil.dp2px(CoreApp.getInstance(), 10);
    private static final int BKG_STROKEN_WIDTH = 3;

    private OnDragListener dragListener;
    private IDragTimeCallBack dragTimeCallBack;


    private Context context;

    public static final int MARK_RESOURCE = R.drawable.icon_selected_mark;
    private Bitmap markBitmap;
    private Paint wholeRectPaint;
    private Paint markPaint;
    private Paint shadowPaint;
    private Paint timeTextPaint;

    private ConfigData limitConfigData;
    /**
     * 绘制的最终目标
     */
    private ConfigData dstConfigData;
    private ConfigData drawConfigData;

    private Rect startRect, endRect;
    private Rect markBitmapRect;

    private float lastX, lastY;

    private int motionStatus;

    /**
     * 预留显示选择区间的时间文字显示
     */
    private int HEIGHT_OFFSET;
    private int timeTextBaseLine;

    public SelectedControlView(Context context) {
        this(context, null);
    }

    public SelectedControlView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public SelectedControlView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        this.context = context;
        init();
    }

    private void init() {
        HEIGHT_OFFSET = ITimeLineUI.TIMELINE_SELECT_VIEW_HEIGHT - ITimeLineUI.TIMELINE_HEIGHT;

        limitConfigData = new ConfigData(true);
        markBitmap = BitmapFactory.decodeResource(context.getResources(), MARK_RESOURCE);
        markBitmapRect = new Rect(0, 0, markBitmap.getWidth(), markBitmap.getHeight());
        wholeRectPaint = new Paint();
        wholeRectPaint.setColor(Color.RED);
        wholeRectPaint.setStyle(Paint.Style.STROKE);
        wholeRectPaint.setStrokeWidth(BKG_STROKEN_WIDTH);

        markPaint = new Paint();

        shadowPaint = new Paint();
        shadowPaint.setStyle(Paint.Style.FILL);
        shadowPaint.setColor(Color.parseColor("#66000000"));

        timeTextPaint = new Paint();
        timeTextPaint.setStrokeWidth(2);
        timeTextPaint.setColor(Color.WHITE);
        timeTextPaint.setTextAlign(Paint.Align.CENTER);
        timeTextPaint.setTextSize(TIME_TEXT_SIZE);
        timeTextBaseLine = HEIGHT_OFFSET - (HEIGHT_OFFSET - TIME_TEXT_SIZE) / 2;

        startRect = new Rect();
        endRect = new Rect();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (drawConfigData != null) {
            Rect bkgRect = drawConfigData.bkgRect;
            canvas.drawRect(bkgRect, wholeRectPaint);
            Log.e("TAG", "bkgRect == " + bkgRect.toString());

            Rect startMarkRect = drawConfigData.startMarkRect;
            if (startMarkRect != null && !startMarkRect.isEmpty()) {
                canvas.drawBitmap(markBitmap,
                        markBitmapRect, startMarkRect, markPaint);
                int startW = startMarkRect.left - bkgRect.left;
                if (startW > 0) {
                    startRect.left = bkgRect.left;
                    startRect.top = bkgRect.top;
                    startRect.right = startMarkRect.left;
                    startRect.bottom = bkgRect.bottom;
                    canvas.drawRect(startRect, shadowPaint);
                }
                int x = startMarkRect.left + startMarkRect.width() / 2;
                //绘制时间文字
                drawStartTimeText(canvas, x, timeTextBaseLine);
                Log.e("TAG", "startMarkRect == " + startMarkRect.toString());
            }
            Rect endMarkRect = drawConfigData.endMarkRect;
            if (endMarkRect != null && !endMarkRect.isEmpty()) {
                canvas.drawBitmap(markBitmap,
                        markBitmapRect, endMarkRect, markPaint);
                int endW = bkgRect.right - endMarkRect.right;
                if (endW > 0) {
                    endRect.left = endMarkRect.right;
                    endRect.top = bkgRect.top;
                    endRect.right = bkgRect.right;
                    endRect.bottom = bkgRect.bottom;
                    canvas.drawRect(endRect, shadowPaint);
                }
                int x = endMarkRect.left + endMarkRect.width() / 2;
                drawEndTimeText(canvas, x, timeTextBaseLine);
            }
        }

    }

    private void drawStartTimeText(Canvas canvas, int x, int y) {
        String text = getStartTimeText();
        drawTimeText(canvas, text, x, y);
    }

    private void drawTimeText(Canvas canvas, String text, int x, int y) {
        if (!TextUtils.isEmpty(text)) {
            canvas.drawText(text, x, y, timeTextPaint);
        }
    }

    private void drawEndTimeText(Canvas canvas, int x, int y) {
        String text = getEndTimeText();
        drawTimeText(canvas, text, x, y);
    }


    private String getStartTimeText() {
        if (dragTimeCallBack != null) {
            long startTime = dragTimeCallBack.getDragStartTime();
            return TimeLineStringUtil.parseStringTime(startTime);
        }
        return null;
    }

    private String getEndTimeText() {
        if (dragTimeCallBack != null) {
            long endTime = dragTimeCallBack.getDragEndTime();
            return TimeLineStringUtil.parseStringTime(endTime);
        }
        return null;
    }


    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = event.getAction();
        float x = event.getX();
        float y = event.getY();

        switch (action) {
            case MotionEvent.ACTION_DOWN:
                lastX = x;
                lastY = y;

                int handle = hitHandle((int) x, (int) y);
                if (handle == HANDLE_START) {
                    motionStatus = MOTION_DRAG_START_HANDLE;
                    if (dragListener != null) {
                        dragListener.onDragStart(HANDLE_START);
                    }
                    return true;
                } else if (handle == HANDLE_END) {
                    motionStatus = MOTION_DRAG_END_HANDLE;
                    if (dragListener != null) {
                        dragListener.onDragStart(HANDLE_END);
                    }
                    return true;
                } else {
                    motionStatus = MOTION_NONE;
                }
                break;
            case MotionEvent.ACTION_MOVE:
                float dx = x - lastX;
                float dy = y - lastY;

                lastX = x;
                lastY = y;

                if (motionStatus == MOTION_DRAG_START_HANDLE || motionStatus == MOTION_DRAG_END_HANDLE) {
                    dragHandle(motionStatus, dx, dy);
                    if (dragListener != null) {
                        int handleIndex = motionStatus == MOTION_DRAG_START_HANDLE ? HANDLE_START : HANDLE_END;
                        dragListener.onDraging(handleIndex, (int) dx, (int) dy, drawConfigData.startMarkRect, drawConfigData.endMarkRect);
                    }
                    return true;
                }

                break;
            case MotionEvent.ACTION_UP:
                if (dragListener != null) {
                    if (motionStatus == MOTION_DRAG_START_HANDLE) {
                        dragListener.onDragEnd(HANDLE_START, drawConfigData.startMarkRect.left, drawConfigData.startMarkRect.right);
                    } else if (motionStatus == MOTION_DRAG_END_HANDLE) {
                        dragListener.onDragEnd(HANDLE_END, drawConfigData.endMarkRect.left, drawConfigData.endMarkRect.right);
                    }
                }
                break;
        }
        return super.onTouchEvent(event);
    }

    private int hitHandle(int x, int y) {
        if (drawConfigData != null) {
            boolean start = drawConfigData.startMarkRect != null && drawConfigData.startMarkRect.contains(x, y);
            if (start) {
                return HANDLE_START;
            }
            boolean end = drawConfigData.endMarkRect != null && drawConfigData.endMarkRect.contains(x, y);
            if (end) {
                return HANDLE_END;
            }
        }

        return HANDLE_NONE;
    }

    private void dragHandle(int motionStatus, float dx, float dy) {
        if (motionStatus == MOTION_DRAG_START_HANDLE) {
            if (drawConfigData.startMarkRect != null) {
                int left = (int) (drawConfigData.startMarkRect.left + dx);
                int right = (int) (drawConfigData.startMarkRect.right + dx);
                if (left < limitConfigData.startMarkRect.left && limitConfigData.startMarkRect.left != 0) {
                    left = limitConfigData.startMarkRect.left;
                    right = limitConfigData.startMarkRect.right;
                }
                if (drawConfigData.endMarkRect != null &&
                        right > drawConfigData.endMarkRect.left &&
                        drawConfigData.endMarkRect.left != 0) {
                    right = drawConfigData.endMarkRect.left;
                    left = right - (drawConfigData.startMarkRect.width());
                }
                if (right > limitConfigData.endMarkRect.left && limitConfigData.endMarkRect.left != 0) {
                    right = limitConfigData.endMarkRect.left;
                    left = right - (drawConfigData.startMarkRect.width());
                }
                drawConfigData.startMarkRect.left = left;
                drawConfigData.startMarkRect.right = right;
                postInvalidate();
            }
        } else if (motionStatus == MOTION_DRAG_END_HANDLE) {
            if (drawConfigData.endMarkRect != null) {
                int left = (int) (drawConfigData.endMarkRect.left + dx);
                int right = (int) (drawConfigData.endMarkRect.right + dx);
                if (left > limitConfigData.endMarkRect.left && limitConfigData.endMarkRect.left != 0) {
                    left = limitConfigData.endMarkRect.left;
                    right = limitConfigData.endMarkRect.right;
                }
                if (drawConfigData.startMarkRect != null &&
                        left < drawConfigData.startMarkRect.right && drawConfigData.startMarkRect.right != 0) {
                    left = drawConfigData.startMarkRect.right;
                    right = left + drawConfigData.endMarkRect.width();
                }
                if (left < limitConfigData.startMarkRect.right && limitConfigData.endMarkRect.left != 0) {
                    left = limitConfigData.startMarkRect.right;
                    right = left + drawConfigData.endMarkRect.width();
                }
                drawConfigData.endMarkRect.left = left;
                drawConfigData.endMarkRect.right = right;
                postInvalidate();
            }
        }
    }

    /**
     * @param bkgRect       为null直接清除数据
     * @param startMarkRect
     * @param endMarkRect
     */
    public void postDraw(Rect bkgRect, Rect startMarkRect, Rect endMarkRect) {
        if (bkgRect == null) {
            drawConfigData = null;
            dstConfigData = null;
            postInvalidate();
        } else {
            if (dstConfigData == null) {
                dstConfigData = new ConfigData(true);
            }
            dstConfigData.bkgRect.set(bkgRect);
            if (startMarkRect != null) {
                dstConfigData.startMarkRect.set(startMarkRect);
            } else {
                dstConfigData.startMarkRect.set(0, 0, 0, 0);
            }
            if (endMarkRect != null) {
                dstConfigData.endMarkRect.set(endMarkRect);
            } else {
                dstConfigData.endMarkRect.set(0, 0, 0, 0);
            }
            if (drawConfigData == null) {
                drawConfigData = new ConfigData(true);
            }
            drawConfigData.bkgRect.set(dstConfigData.bkgRect);
            drawConfigData.bkgRect.offset(0, HEIGHT_OFFSET);
            drawConfigData.bkgRect.top += BKG_STROKEN_WIDTH / 2;//预留出来边线的位置
            drawConfigData.bkgRect.bottom -= BKG_STROKEN_WIDTH / 2;//预留出来边线的位置
            drawConfigData.startMarkRect.set(dstConfigData.startMarkRect);
            drawConfigData.startMarkRect.offset(0, HEIGHT_OFFSET);
            drawConfigData.endMarkRect.set(dstConfigData.endMarkRect);
            drawConfigData.endMarkRect.offset(0, HEIGHT_OFFSET);
            postInvalidate();
        }

    }

    public void setDrawLimit(Rect bkgRect, Rect startMarkRect, Rect endMarkRect) {
        limitConfigData.bkgRect.set(bkgRect);
        limitConfigData.startMarkRect.set(startMarkRect);
        limitConfigData.endMarkRect.set(endMarkRect);
    }

    public void setDragTimeCallBack(IDragTimeCallBack dragTimeCallBack) {
        this.dragTimeCallBack = dragTimeCallBack;
    }

    class ConfigData {
        Rect bkgRect;
        Rect startMarkRect;
        Rect endMarkRect;

        public ConfigData() {

        }

        public ConfigData(boolean init) {
            if (init) {
                bkgRect = new Rect();
                startMarkRect = new Rect();
                endMarkRect = new Rect();
            }
        }

        public void offSet(int dx, int dy) {
            if (bkgRect != null) {
                bkgRect.offset(dx, dy);
            }
            if (startMarkRect != null) {
                startMarkRect.offset(dx, dy);
            }
            if (endMarkRect != null) {
                endMarkRect.offset(dx, dy);
            }
        }
    }

    public interface OnDragListener {
        void onDragStart(int dragHandle);

        void onDraging(int dragHandle, int dx, int dy, Rect startRect, Rect endRect);

        void onDragEnd(int dragHandle, int rectLeft, int rectRight);
    }

    public interface IDragTimeCallBack {
        long getDragStartTime();

        long getDragEndTime();
    }

    public void setOnDragListener(OnDragListener listener) {
        this.dragListener = listener;
    }
}
