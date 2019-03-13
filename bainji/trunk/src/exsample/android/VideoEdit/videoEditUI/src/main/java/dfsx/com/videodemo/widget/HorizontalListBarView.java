package dfsx.com.videodemo.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckedTextView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import com.dfsx.core.common.Util.PixelUtil;
import dfsx.com.videodemo.R;

import java.util.ArrayList;
import java.util.List;

public class HorizontalListBarView extends LinearLayout implements View.OnClickListener {

    private Context context;
    private int itemViewWidth;

    public HorizontalListBarView(Context context) {
        super(context);
        init(null);
    }

    public HorizontalListBarView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs);
    }

    public HorizontalListBarView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(attrs);
    }

    private void init(AttributeSet attrs) {
        context = getContext();
        setOrientation(LinearLayout.HORIZONTAL);
        if (attrs != null) {
            TypedArray ta = context.obtainStyledAttributes(attrs, R.styleable.HorizontalListBarView);
            itemViewWidth = (int) ta.getDimension(R.styleable.HorizontalListBarView_item_content_width, 0f);
        }

    }

    public void setUpBarContentView(String... strings) {
        List<IBarContent> list = new ArrayList<>();
        for (String item : strings) {
            list.add(new DefaultBarContent(0, item));
        }
        setUpBarContentView(list);
    }

    public void setUpBarContentView(int... imageResourceArr) {
        List<IBarContent> list = new ArrayList<>();
        for (int res : imageResourceArr) {
            list.add(new DefaultBarContent(res, null));
        }
        setUpBarContentView(list);
    }

    public void setUpBarContentView(List<IBarContent> list) {
        removeAllViews();
        if (list != null) {
            for (int i = 0; i < list.size(); i++) {
                IBarContent content = list.get(i);
                View item = createTimeView(i);
                addView(item);
                if (!TextUtils.isEmpty(content.getBarContentText())) {
                    setItemContent(item, content.getBarContentText());
                }
                if (content.getContentImageResource() != 0) {
                    setItemContent(item, content.getContentImageResource());
                }
            }
        }
    }


    private View createTimeView(int pos) {
        RelativeLayout itemView = new RelativeLayout(context);
        itemView.setId(pos);
        LayoutParams params = null;
        if (itemViewWidth == 0) {
            params = new LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.MATCH_PARENT);
            params.weight = 1;
        } else {
            params = new LayoutParams(itemViewWidth, ViewGroup.LayoutParams.MATCH_PARENT);
        }
        itemView.setLayoutParams(params);
        itemView.setGravity(Gravity.CENTER);
        CheckedTextView itemContentView = new CheckedTextView(context);
        RelativeLayout.LayoutParams p = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT);
        itemContentView.setId(R.id.tv_bar_content);
        itemContentView.setLayoutParams(p);
        itemView.addView(itemContentView);
        itemContentView.setMinHeight(PixelUtil.dp2px(context, 50));
        itemContentView.setGravity(Gravity.CENTER);
        itemContentView.setPadding(PixelUtil.dp2px(context, 10), 0, PixelUtil.dp2px(context, 10), 0);
        itemContentView.setTextColor(Color.WHITE);
        itemContentView.setTextSize(TypedValue.COMPLEX_UNIT_DIP, 15);
        itemContentView.setTag(pos);
        itemContentView.setOnClickListener(this);
        return itemView;
    }

    public void setItemContent(View itemView, int imgRes) {
        TextView textView = (TextView) itemView.findViewById(R.id.tv_bar_content);
        textView.setCompoundDrawablesWithIntrinsicBounds(imgRes, 0, 0, 0);
    }

    public void setItemContent(View itemView, String text) {
        TextView textView = (TextView) itemView.findViewById(R.id.tv_bar_content);
        textView.setText(text);
    }

    @Override
    public void onClick(View v) {
        if (v instanceof CheckedTextView) {
            CheckedTextView ctv = (CheckedTextView) v;
            ctv.setChecked(!ctv.isChecked());

            if (itemClickListener != null) {
                itemClickListener.onItemClick(v, (Integer) v.getTag());
            }
        }
    }

    private OnItemClickListener itemClickListener;

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.itemClickListener = listener;
    }

    public interface OnItemClickListener {
        void onItemClick(View v, int position);
    }

    public interface IBarContent {
        int getContentImageResource();

        String getBarContentText();
    }

    public class DefaultBarContent implements IBarContent {

        private int imgRes;
        private String text;

        public DefaultBarContent(int img, String text) {
            this.imgRes = img;
            this.text = text;
        }

        @Override
        public int getContentImageResource() {
            return imgRes;
        }

        @Override
        public String getBarContentText() {
            return text;
        }
    }
}
