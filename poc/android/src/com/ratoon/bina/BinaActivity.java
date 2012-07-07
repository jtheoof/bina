package com.ratoon.bina;

import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;

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
        webview.loadUrl("file:///android_asset/index.html");
    }
}
