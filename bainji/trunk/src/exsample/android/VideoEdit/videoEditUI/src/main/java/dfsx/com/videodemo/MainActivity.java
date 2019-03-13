package dfsx.com.videodemo;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import com.dfsx.core.common.act.DefaultFragmentActivity;
import dfsx.com.videodemo.frag.ProjectListFragment;

public class MainActivity extends AppCompatActivity {
    // Used to load the 'native-lib' library on application startup.

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        Button btn = (Button) findViewById(R.id.start);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //                Intent intent = new Intent(MainActivity.this, TestTimeLineActivity.class);
                //                startActivity(intent);
            }
        });

        findViewById(R.id.start_proj)
                .setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        DefaultFragmentActivity.start(MainActivity.this, ProjectListFragment.class.getName());
                    }
                });

        findViewById(R.id.start_test)
                .setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Intent intent = new Intent(MainActivity.this, TestActivity.class);
                        startActivity(intent);
                    }
                });
    }

}
