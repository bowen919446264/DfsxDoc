package dfsx.com.videodemo.frag;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Size;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckedTextView;
import android.widget.TextView;
import android.widget.Toast;
import com.dfsx.core.common.Util.PixelUtil;
import com.dfsx.core.common.adapter.BaseRecyclerViewDataAdapter;
import com.dfsx.core.common.adapter.BaseRecyclerViewHolder;
import dfsx.com.videodemo.R;
import dfsx.com.videodemo.adapter.ItemOffsetDecoration;

import java.util.ArrayList;

/**
 * 输出配置选择界面
 */
public class OutputConfigFragment extends Fragment {
    private RecyclerView recyclerView;
    private OutPutConfigAdapter adapter;

    private IOutputConfig latestConfig;

    private Button btnOkOutput;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.frag_output_congfig_layout, null);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        recyclerView = (RecyclerView) view.findViewById(R.id.output_recycler);
        btnOkOutput = (Button) view.findViewById(R.id.btn_ok_output);
        recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        int leftRight = PixelUtil.dp2px(getContext(), 20);
        int top = PixelUtil.dp2px(getContext(), 6);
        int bottom = PixelUtil.dp2px(getContext(), 6);
        recyclerView.setClipToPadding(true);
        ItemOffsetDecoration offsetDecoration = new ItemOffsetDecoration(leftRight, top, leftRight, bottom);
        offsetDecoration.setStartOffet(PixelUtil.dp2px(getContext(), 13));
        recyclerView.addItemDecoration(offsetDecoration);
        adapter = new OutPutConfigAdapter();
        recyclerView.setAdapter(adapter);

        ArrayList<IOutputConfig> list = new ArrayList<>();
        list.add(new SizeConfig(1920, 1080));
        list.add(new SizeConfig(1280, 720));
        list.add(new SizeConfig(720, 480));

        adapter.update(list, false);

        btnOkOutput.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (getSelectedConfig() != null) {
                    if (outputClickListener != null) {
                        outputClickListener.onOutputClick(v, getSelectedConfig());
                    }
                    getFragmentManager().beginTransaction()
                            .setCustomAnimations(R.anim.frag_bottom_in, R.anim.frag_bottom_out)
                            .remove(OutputConfigFragment.this)
                            .commitAllowingStateLoss();
                } else {
                    Toast.makeText(getContext(), "请选择输出类型", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    /**
     * 获取选中的Config
     *
     * @return
     */
    public IOutputConfig getSelectedConfig() {
        return latestConfig;
    }

    private void onConfigClick(IOutputConfig config) {
        if (config != null) {
            config.setSelected(!config.isSelected());
            if (latestConfig != null) {
                latestConfig.setSelected(false);
            }
            if (config.isSelected()) {
                latestConfig = config;
            } else {
                latestConfig = null;
            }
            adapter.notifyDataSetChanged();
        }
    }

    public class SizeConfig implements IOutputConfig {

        private int width;
        private int heght;

        private boolean isSelected;

        public SizeConfig(int w, int h) {
            this.width = w;
            this.heght = h;
        }

        public Size getSize() {
            return new Size(width, heght);
        }

        @Override
        public String getConfigName() {
            return heght + "P";
        }

        @Override
        public boolean isSelected() {
            return isSelected;
        }

        @Override
        public void setSelected(boolean isSelected) {
            this.isSelected = isSelected;
        }
    }

    public interface IOutputConfig {
        String getConfigName();

        boolean isSelected();

        void setSelected(boolean isSelected);
    }


    class OutPutConfigAdapter extends BaseRecyclerViewDataAdapter<IOutputConfig> {

        @Override
        public BaseRecyclerViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new BaseRecyclerViewHolder(R.layout.adapter_output_config_layout, parent, viewType);
        }

        @Override
        public void onBindViewHolder(BaseRecyclerViewHolder holder, int position) {
            View itemView = holder.getView(R.id.item_layout_view);
            TextView itemConfigName = holder.getView(R.id.tv_name);
            CheckedTextView itemSelectedCheck = holder.getView(R.id.selected_check_tv);

            IOutputConfig config = list.get(position);
            int bkgres = config.isSelected() ? R.drawable.output_bkg_selected : R.drawable.output_bkg_unselected;
            itemView.setBackgroundResource(bkgres);
            itemConfigName.setText(config.getConfigName());
            itemSelectedCheck.setChecked(config.isSelected());

            itemView.setTag(config);
            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        IOutputConfig config1 = (IOutputConfig) v.getTag();
                        onConfigClick(config1);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                }
            });
        }
    }

    private OnOutputClickListener outputClickListener;

    public void setOnOutputClickListener(OnOutputClickListener listener) {
        this.outputClickListener = listener;
    }

    public interface OnOutputClickListener {
        void onOutputClick(View v, IOutputConfig outputConfig);
    }
}
