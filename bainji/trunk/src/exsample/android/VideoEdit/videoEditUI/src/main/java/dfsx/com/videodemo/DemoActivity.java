package dfsx.com.videodemo;

import android.Manifest;
import android.os.Bundle;
import android.widget.Toast;
import com.dfsx.core.common.act.DefaultFragmentActivity;
import com.gun0912.tedpermission.PermissionListener;
import com.gun0912.tedpermission.TedPermission;
import dfsx.com.videodemo.frag.ProjectListFragment;

import java.util.ArrayList;

public class DemoActivity extends DefaultFragmentActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        getIntent().putExtra(KEY_FRAGMENT_NAME, ProjectListFragment.class.getName());
        super.onCreate(savedInstanceState);

        TedPermission.with(this).setPermissionListener(new PermissionListener() {
            @Override
            public void onPermissionGranted() {
            }


            @Override
            public void onPermissionDenied(ArrayList<String> arrayList) {
                Toast.makeText(DemoActivity.this, "没有权限", Toast.LENGTH_SHORT).show();
            }
        }).setDeniedMessage(DemoActivity.this.getResources().getString(com.dfsx.videoeditor.R.string.denied_message)).
                setPermissions(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                .check();
    }
}
