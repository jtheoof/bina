package com.ratoon.bina;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.RelativeLayout;

public class BinaActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        WebView webview = new WebView(this);
        setContentView(webview);
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.main);
        webview.getSettings().setJavaScriptEnabled(true);
        webview.getSettings().setRenderPriority(WebSettings.RenderPriority.HIGH);
        //webview.onTouchEvent(this);
        //webview.setVerticalScrollBarEnable(false);
        webview.loadUrl("file:///android_asset/index.html");
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        super.onTouchEvent(event);
        return true;
    }
}
