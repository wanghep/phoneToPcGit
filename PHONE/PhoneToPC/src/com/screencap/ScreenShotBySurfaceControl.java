package com.screencap;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.SurfaceControl;
import android.view.WindowManager;
  
public class ScreenShotBySurfaceControl{  
	
	private Bitmap mScreenBitmap;
	private WindowManager mWindowManager;
	private DisplayMetrics mDisplayMetrics;
	private Display mDisplay;
	
	@SuppressLint("NewApi")
	public Bitmap takeScreenShot(Activity activity) {
		
		mDisplayMetrics = new DisplayMetrics();
		mWindowManager = activity.getWindowManager();  
		mDisplay = mWindowManager.getDefaultDisplay();
		mDisplay.getRealMetrics(mDisplayMetrics);
		
		float[] dims = { mDisplayMetrics.widthPixels,
				mDisplayMetrics.heightPixels };
		mScreenBitmap = SurfaceControl.screenshot((int) dims[0], (int) dims[1]);

		if (mScreenBitmap == null) {
			Log.d("SurfaceControl.screenshot","ScreenBitmap == null");
			return null;
		}

		Matrix matrix = new Matrix();
		matrix.postScale(0.25f, 0.25f); // ���Ϳ�Ŵ���С�ı���
		// ��״̬��������������С
		return Bitmap.createBitmap(mScreenBitmap, 0, 0,
				(int) dims[0], (int) dims[1], matrix, true);
	}
}  