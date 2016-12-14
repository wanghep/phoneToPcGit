package com.screencap;
/**
 * ScreenShotFb.java
 * ��Ȩ����(C) 2014
 * ������:cuiran 2014-4-3 ����4:55:23
 */


import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.PixelFormat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;

/**
 * FrameBuffer�л�ȡAndroid��Ļ��ͼ 
 * @author yinxiuyu
 * @version 1.0.0
 */
public class ScreenShotFb {

	private static final String TAG = "ScreenShotFb";
	final static String FB0FILE1 = "/dev/graphics/fb0";
	final static String RAWFILE = "/sdcard/ScreenshotsImages/screencap.raw";
	final static String RAWFILE1 = "/sdcard/ScreenshotsImages/inputStreamFile.raw";
	static File fbFile;
	static DataInputStream dStream = null;
	static byte[] piex = null;
	static int[] colors = null;
	static int screenWidth;
	static int screenHeight;
	 

	public  synchronized Bitmap getScreenShotBitmap(Activity activity) {
		
		try {
			
			DisplayMetrics dm = new DisplayMetrics();
			Display display = activity.getWindowManager().getDefaultDisplay();
			display.getMetrics(dm);
			screenWidth = dm.widthPixels; // ��Ļ�?���أ��磺480px��
			screenHeight = dm.heightPixels; // ��Ļ�ߣ����أ��磺800p��
			int pixelformat = display.getPixelFormat();
			PixelFormat localPixelFormat1 = new PixelFormat();
			PixelFormat.getPixelFormatInfo(pixelformat, localPixelFormat1);
			int deepth = localPixelFormat1.bytesPerPixel;// λ��
			Log.i(TAG, "deepth="+deepth);
			piex = new byte[screenHeight * screenWidth*deepth] ;// ����
			colors = new int[screenHeight * screenWidth];
		}catch(Exception e){
			Log.e(TAG, "Exception error",e);
		}
		
		FileInputStream fi = null;
		try {
			//fbFile = new File(FB0FILE1);
			fbFile = new File(RAWFILE1);
			if(!fbFile.exists()){
				Log.e(TAG, "FileNotFoundException error");
			}
			
			long fileSize = fbFile.length();
			if (fileSize > Integer.MAX_VALUE) {
				System.out.println("file too big...");
				return null;
			}
			
			fi = new FileInputStream(fbFile);// ��ȡ�ļ�����
			
			/*dStream=new DataInputStream(fi);
			dStream.readFully(piex);
			dStream.close();*/
			
			byte[] buffer = new byte[(int) fileSize];
			int offset = 0;
			int numRead = 0;
			while (offset < buffer.length
					&& (numRead = fi.read(buffer, offset, buffer.length
							- offset)) >= 0) {
				offset += numRead;
			}
			// ȷ��������ݾ��ȡ
			if (offset != buffer.length) {
				throw new IOException("Could not completely read file "
						+ fbFile.getName());
			}
			
			
		/*	// ��rgbתΪɫֵ
			for (int i = 0; i < piex.length; i += 2) {
				colors[i / 2] = (int) 0xff000000
						+ (int) (((piex[i + 1]) << (16)) & 0x00f80000)
						+ (int) (((piex[i + 1]) << 13) & 0x0000e000)
						+ (int) (((piex[i]) << 5) & 0x00001A00)
						+ (int) (((piex[i]) << 3) & 0x000000f8);
			}
			// �õ���Ļbitmap
			return Bitmap.createBitmap(colors, screenWidth, screenHeight,
    					Bitmap.Config.RGB_565);*/
			/*
			for (int i = 0; i < piex.length; i += 4) {
				int r = (piex[i] & 0xFF);
				int g = (piex[i + 1] & 0xFF);
				int b = (piex[i + 2] & 0xFF);
				int a = (piex[i + 3] & 0xFF);
				colors[i / 4] = (a << 24) + (r << 16) + (g << 8) + b;
				//Log.d("HciServlet","rgba = "+ " "+r + " "+ g +  " "+b + " "+ a);
			}
			// �õ���Ļbitmap
			return Bitmap.createBitmap(colors, screenWidth, screenHeight,
    					Bitmap.Config.ARGB_8888);*/
			
			
		/*	for (int j = 0; j < piex.length; j += 4) {
				int b = piex[j] & 0xff;
				int g = piex[j + 1] & 0xff;
				int r = piex[j + 2] & 0xff;
				colors[j / 4] = (r << 16) | (g << 8) | b | (0xff000000);
			}
			return Bitmap.createBitmap(colors, screenWidth, screenHeight,
					Bitmap.Config.ARGB_8888);*/
			
			
			Matrix matrix = new Matrix();
			matrix.postScale(0.25f, 0.25f); // ���Ϳ�Ŵ���С�ı���
			// ��״̬��������������С
			return Bitmap.createBitmap(bytesToBitmap(buffer), 0, 0,
					screenWidth, screenHeight, matrix, true);
			
		} catch (FileNotFoundException e) {
			Log.e(TAG, "FileNotFoundException error",e);
		} catch (IOException e) {
			Log.e(TAG, "IOException error",e);
		}catch (Exception e) {
			Log.e(TAG, "Exception error",e);
		}  
		finally {
			if(fi!=null){
				try {
					fi.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return null;
	}
	
	
	public Bitmap bytesToBitmap(byte[] b) {
		if (b.length != 0) {
			return BitmapFactory.decodeByteArray(b, 0, b.length);
		} else {
			return null;
		}
	}
  
}
