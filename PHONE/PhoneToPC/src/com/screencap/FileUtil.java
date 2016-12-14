package com.screencap;

import java.io.File;
import java.io.FileOutputStream;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.Rect;
import android.os.Environment;
import android.util.Log;
import android.view.Display;
import android.view.View;

public class FileUtil {

	/* byte[]תInt */
	public static int bytesToInt(byte[] bytes) {
		int addr = bytes[0] & 0xFF;
		addr |= ((bytes[1] << 8) & 0xFF00);
		addr |= ((bytes[2] << 16) & 0xFF0000);
		addr |= ((bytes[3] << 24) & 0xFF000000);
		return addr;
	}

	/* Intתbyte[] */
	public static byte[] intToByte(int i) {
		byte[] abyte0 = new byte[4];
		abyte0[0] = (byte) (0xff & i);
		abyte0[1] = (byte) ((0xff00 & i) >> 8);
		abyte0[2] = (byte) ((0xff0000 & i) >> 16);
		abyte0[3] = (byte) ((0xff000000 & i) >> 24);
		return abyte0;
	}
	
	/*BytesToBitmap*/
	
	public Bitmap bytesToBitmap(byte[] b) {
		if (b.length != 0) {
			return BitmapFactory.decodeByteArray(b, 0, b.length);
		} else {
			return null;
		}
	}	
	/* 图片转为Bitmap */

	public Bitmap picToBitmap(String url) {
		
		Bitmap bitmap =null;
		/*BitmapFactory.Options options = new BitmapFactory.Options();
		options.inSampleSize = 2;*/
		bitmap = BitmapFactory.decodeFile(url);
		assert( bitmap != null );
		return bitmap;
	}

	/* 保存jpeg图片到SD卡 */

	public void savePic(Bitmap bitmap, String SavePath, String fileName) {
		if (bitmap != null) {
			// 保存Bitmap
			try {
				File path = new File(SavePath);
				// 文件
				String filepath = SavePath + fileName;
				File file = new File(filepath);
				if (!path.exists()) {
					path.mkdirs();
				}
				/*if (file.exists()) {
					file.delete();
				}
				file.createNewFile();*/
				if (!file.exists()) {
					file.createNewFile();
				}
				FileOutputStream fos = null;
				fos = new FileOutputStream(file);
				if (null != fos) {
					bitmap.compress(Bitmap.CompressFormat.JPEG, 50, fos);
					fos.flush();
					fos.close();
					Log.i("yinxiuyu", "savePic(): " + "截屏文件已保存至" + filepath);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/* 获取SDCard的目录路径功能 */

	public String getSDCardPath() {
		File sdcardDir = null;
		// 判断SDCard是否存在
		boolean sdcardExist = Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED);
		if (sdcardExist) {
			sdcardDir = Environment.getExternalStorageDirectory();
		} else {
			Log.i("yinxiuyu", "getSDCardPath():　" + "SD卡不存在");
		}
		return sdcardDir.toString();
	}

	/* 根据Activity自身截屏 */

	public Bitmap screenShot(Activity activity) {
		// 获取windows中最顶层的view
		View view = activity.getWindow().getDecorView();
		view.buildDrawingCache();

		// 获取状态栏高度
		Rect rect = new Rect();
		view.getWindowVisibleDisplayFrame(rect);
		int statusBarHeights = rect.top;
		Display display = activity.getWindowManager().getDefaultDisplay();

		// 获取屏幕宽和高
		int widths = display.getWidth();
		int heights = display.getHeight();
		// Log.i("yinxiuyu","screenWidth= "+widths+"; screenHeight= "+heights);
		// 允许当前窗口保存缓存信息
		view.setDrawingCacheEnabled(true);

		// 去掉状态栏
		/*
		 * Bitmap bitmap = Bitmap.createBitmap(view.getDrawingCache(), 0,
		 * statusBarHeights, widths, heights - statusBarHeights);
		 */
		Matrix matrix = new Matrix();
		matrix.postScale(0.25f, 0.25f); // 长和宽放大缩小的比例
		// 包含状态栏，并按比例缩小
		Bitmap bitmap = Bitmap.createBitmap(view.getDrawingCache(), 0, 0,
				widths, heights, matrix, true);
		// 销毁缓存信息
		view.destroyDrawingCache();

		return bitmap;
	}

}
