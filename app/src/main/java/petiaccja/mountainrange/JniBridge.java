package petiaccja.mountainrange;

import android.app.Activity;
import android.content.res.AssetManager;

public class JniBridge {
    public static native void Init(Activity activity, AssetManager assetManager);
    public static native void OnDrawFrame();
}
