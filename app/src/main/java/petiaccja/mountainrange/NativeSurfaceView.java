package petiaccja.mountainrange;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class NativeSurfaceView extends GLSurfaceView {
    private static final String TAG = "GLES3JNI";
    private static final boolean DEBUG = true;
    private AssetManager assetManagerForInit;
    private Activity activityForInit;

    public NativeSurfaceView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
    }

    /** This function literally exists only because OpenGL is a useless pile of shit.
     * Anyhow, this is horrible design and should be incinerated in a fucking furnace
     * (alongside OpenGL). */
    public void initCrapGl(Activity activityForInit, AssetManager assetManagerForInit) {
        setRenderer(new Renderer(activityForInit, assetManagerForInit));
    }

    private static class Renderer implements GLSurfaceView.Renderer {
        private AssetManager assetManagerForInit; // See the rant above about CrapGL.
        private Activity activityForInit; // See the rant above about CrapGL.
        private boolean isInit = false;

        Renderer() {
            this.activityForInit = null;
            this.assetManagerForInit = null;
        }
        Renderer(Activity activityForInit, AssetManager assetManagerForInit) {
            this.activityForInit = activityForInit;
            this.assetManagerForInit = assetManagerForInit;
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            if (!isInit) {
                JniBridge.Init(activityForInit, assetManagerForInit);
                isInit = true;
            }
            if (isInit) {
                JniBridge.OnDrawFrame();
            }
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            // GLES3JNILib.resize(width, height);
        }

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // GLES3JNILib.init();
        }
    }
}
