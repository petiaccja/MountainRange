package petiaccja.mountainrange;

import android.content.res.AssetManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.WindowManager;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private NativeSurfaceView terrainView = null;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (BuildConfig.DEBUG) {
            // These flags cause the device screen to turn on (and bypass screen guard if possible) when launching.
            // This makes it easy for developers to test the app launch without needing to turn on the device
            // each time and without needing to enable the "Stay awake" option.
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON
                    | WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD
                    | WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED);
        }


        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(getCompilerVersion());

        terrainView = (NativeSurfaceView)findViewById(R.id.terrain_view);
		JniBridge.Init(this, getAssets());
    }

    @Override
    protected void onPause() {
        if (terrainView != null) {
            terrainView.onPause();
        }
        super.onPause();
    }

    @Override
    protected void onResume() {
        if (terrainView != null) {
            terrainView.onResume();
        }
        super.onResume();
    }


    public native String getCompilerVersion();
}
