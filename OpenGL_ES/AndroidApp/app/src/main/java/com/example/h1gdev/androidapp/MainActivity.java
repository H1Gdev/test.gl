package com.example.h1gdev.androidapp;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {

    private GLSurfaceView glSurfaceView = null;
    private final GLSurfaceView.Renderer renderer = new GLSurfaceView.Renderer() {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // do not use gl...
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            // do not use gl...
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            // do not use gl...
        }
    };

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // For use GLThread.
        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(renderer);
        ((ViewGroup)((ViewGroup)findViewById(android.R.id.content)).getChildAt(0)).addView(glSurfaceView);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        tv.setText(stringFromAAsset(getAssets()));

        outputGLInfo();
    }

    @Override
    protected void onResume() {
        super.onResume();

        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();

        glSurfaceView.onPause();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String stringFromAAsset(AssetManager assetManager);
    public native void outputGLInfo();
}
