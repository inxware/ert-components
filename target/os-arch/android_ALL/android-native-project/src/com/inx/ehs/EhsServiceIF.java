

//package com.inx.ehs;
package com.inx.ehs;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.Gravity;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.app.Activity;
import android.app.NativeActivity;
import 	android.content.Context;
import android.content.res.Configuration;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.webkit.WebSettings;
import android.webkit.WebViewClient;
import android.widget.TextView;
import android.widget.Toast;
//import android.app.Activity;


public class EhsServiceIF  extends NativeActivity /*implements SurfaceHolder.Callback2*/ {
	/*Constructor if needed 
	EhsServiceIF() { }*/

	WindowManager mWM;
	WebView mView; /* todo - we need an expandable array of these */
	int mX;
	int mY ;
	int mHeight;
	int mWidth;
	int mAlpha;
	int mTimeoutms;
	boolean mViewParamsSet=false;
	boolean mVisible=false;
	boolean mCreated=false;
	String mBaseUrl;
	
	/* Use thread  DrawingThread mDrawingThread; */

	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		//Log.i("EHS","Create!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
	/* Threaded UI bits needed? :
	 public void surfaceCreated(SurfaceHolder holder) {
	      // Tell the drawing thread that a surface is available.
		  synchronized (mDrawingThread) {
	       		mDrawingThread.mSurface = holder;
	        	mDrawingThread.notify();
	      }
	 }*/

	@Override
	protected void onStart() {
		super.onStart();
	}
	
	public void setViewParameters(final int x, final int y,final int width, final int height, final int alpha,final int  timeoutms ) {
		mX= x;
		mY = y;
		mHeight=height;
		mWidth=width;
		mAlpha=alpha;
		mTimeoutms=timeoutms;
		mViewParamsSet=true;
		Log.i("EHS","In setViewParameters!!!");
	}
	

	public int setFRameVisible(boolean visible) {
		if (visible & !mVisible) {

		}
		mVisible = visible;
		return 0;
	}
	/*   String baseUrl - This is for cross domain checking - can be null or "data" if bDataUri is true
	 *   String Uri 	- This can be a URL or data (use bDataUri to notify if actual data)
	 *   String mimeType- This is effective if the Uri is data rather than a link , 
	 *   boolean bDataUri - set ii the Uri is data rather than a URL */
	
	public int CreateWebView(final String baseUrl, final String Uri,final boolean bDataUri, final String mimeType  ) {
		this.runOnUiThread(new Runnable() {
			public void run() {
				String data = "";
				//String mimeType;
				String encoding = "";
				String historyUrl = "null";
				
				//if (hasWindowFocus()) { - don't think this is necessary??
				Context context = EhsServiceIF.this;
				if (mView != null) mView = null; /* force garbage collection */
				mView = new WebView(context); /* todo: we want a parent view for all Java overlays - Webview can be nexted in here  */
				
				mView.setLayerType(View.LAYER_TYPE_SOFTWARE, null); /* todo is this necessary */
				
				//mView.setBackgroundColor(Color.WHITE);
				//mView.invalidate();
				WebSettings settings = mView.getSettings();
				settings.setJavaScriptEnabled(true);
				settings.setAllowFileAccessFromFileURLs(true); 
				settings.setAllowUniversalAccessFromFileURLs(true);
				if (bDataUri ) {
					data = Uri;
				}
				else {
					try {
						data = URLEncoder.encode(Uri, "UTF-8");
					} catch (UnsupportedEncodingException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				LayoutParams mParams = new LayoutParams();
				mWM = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
				if (mViewParamsSet) {
					mParams.gravity = Gravity.START | Gravity.TOP;
					mParams.x = mX;
					mParams.y = mY;
					mParams.height=mHeight;
					mParams.width= mWidth;
					//mParams.verticalMargin = 200; //mVerticalMargin;
					//mParams.horizontalMargin =200; // mHorizontalMargin;
					mParams.alpha=(float)mAlpha/255.0f;
					//mParams.format = PixelFormat.RGBA_8888;
					Log.i("EHS",String.format("x=%d,y=%d, width = %d,height=%d alpha=%f",mParams.x,mParams.y,mParams.height,mParams.width,mParams.alpha));
					
					mWM.addView(mView, mParams);
					//mView.loadDataWithBaseURL (baseUrl,  "<!DOCTYPE HTML><html><head><link href=\"www.google.com\"><script>function move(elem) {var left = 0;function frame() {left++;elem.style.left = left + 'px';  if (left == 100) clearInterval(id);} }</script></head><body><div onclick=\"move(this.children[0])\" class=\"example_path\"><div class=\"example_block\"></div></div></body></html>", "text/html"/*mimeType*/,  encoding,  historyUrl);
					//mView.loadDataWithBaseURL (baseUrl,  "<!DOCTYPE HTML><html><head><script>function move(elem) {var left = 0;function frame() {left++;elem.style.left = left + 'px';  if (left == 100) clearInterval(id);} }</script></head><body><div onclick=\"move(this.children[0])>\"<div>Hello in a div</div></div></body></html>", "text/html"/*mimeType*/,  encoding,  historyUrl);
					
					if (bDataUri) {
						mView.loadDataWithBaseURL (baseUrl, data , "text/html"/*mimeType*/,  encoding,  historyUrl);
					}
					else {
						mView.setWebViewClient(new WebViewClient()); /* this is to stop launching a full screen browser to handle forwards etc. */
						mView.loadUrl(data);
					}
					mCreated = true;
					if (mTimeoutms > 0) {
						Log.i("EHS",String.format("Setting Webview timeout to %d",mTimeoutms));
						new Timer().schedule( 
								new TimerTask() {
									@Override
									public void run() {
										runOnUiThread(new Runnable() {
											public void run() {
												mView.destroy();
												mWM.removeView(mView);
												//myCycle();
											}
										});
									}
								}, mTimeoutms );
					}
					//mView. perhaps add listerners here
				}
				//else {
				//	Log.i("EHS","No Window Focus");
				//}
				//Toast.makeText(EhsServiceIF.this, "This is Toast!!!", Toast.LENGTH_SHORT).show();	
				Log.i("EHS","In CreateWebView - 10");
			}
			
		});

		return 0;
	}
	
	public int RunFunction(String funcName, String jsonarg) {
		if (mCreated) {
		mView.loadDataWithBaseURL (mBaseUrl, "javascript:"+funcName+"('" + jsonarg + "')", "text/javascript",  null,  null);
		//mView.loadUrl("javascript:disableSection('" + surveyId + "');");
		}
		return 0;
	}
	
	public int Destroy() {
		if (mCreated) {
			mView.destroy();
			mWM.removeView(mView);
		}
		mWM = null;
		mCreated = false;
		mVisible = false;
		return 0;
	}
/*todo need to add destroy */

};