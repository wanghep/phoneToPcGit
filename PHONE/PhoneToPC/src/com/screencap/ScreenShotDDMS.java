package com.screencap;

import java.io.File;
import java.io.IOException;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

import com.android.ddmlib.AdbCommandRejectedException;
import com.android.ddmlib.AndroidDebugBridge;
import com.android.ddmlib.IDevice;
import com.android.ddmlib.RawImage;
import com.android.ddmlib.TimeoutException;
/**
 * ͨ��DDMS��ȡAndroid��Ļ��ͼ 
 * @author yinxiuyu
 * @version 1.0.0
 */
public class ScreenShotDDMS {

	public Bitmap screenShot() {
		RawImage rawScreen = null;
		byte[] rawScreenData;
		IDevice device;
		AndroidDebugBridge bridge = AndroidDebugBridge.createBridge();
		waitDeviceList(bridge);
		
    /*    // init the lib
        // [try to] ensure ADB is running
        String adbLocation = System.getProperty("com.android.screenshot.bindir"); //$NON-NLS-1$
        if (adbLocation != null && adbLocation.length() != 0) {
            adbLocation += File.separator + "adb"; //$NON-NLS-1$
        } else {
            adbLocation = "adb"; //$NON-NLS-1$
        }

		AndroidDebugBridge.init(false  debugger support );

		AndroidDebugBridge bridge = AndroidDebugBridge.createBridge(
				adbLocation, true  forceNewBridge );

		// we can't just ask for the device list right away, as the internal
		// thread getting
		// them from ADB may not be done getting the first list.
		// Since we don't really want getDevices() to be blocking, we wait here
		// manually.
		int count = 0;
		while (bridge.hasInitialDeviceList() == false) {
			try {
				Thread.sleep(100);
				count++;
			} catch (InterruptedException e) {
				// pass
			}

			// let's not wait > 10 sec.
			if (count > 100) {
				System.err.println("Timeout getting device list!");
				return null;
			}
		}*/

		// now get the devices
		IDevice[] devices = bridge.getDevices();
       
		device = devices[0];
		try {
			rawScreen = device.getScreenshot();
		} catch (TimeoutException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (AdbCommandRejectedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		rawScreenData = rawScreen.data;
		//Bitmap bitmap = BytesToBitmap(rawScreenData);
		//if (rawScreen != null) {

		Matrix matrix = new Matrix();
		matrix.postScale(0.25f, 0.25f); // ���Ϳ�Ŵ���С�ı���
		// ��״̬��������������С
		Bitmap bitmap = Bitmap.createBitmap(bytesToBitmap(rawScreenData), 0, 0,
				rawScreen.width, rawScreen.height, matrix, true);
			
		//}
		return bitmap;
	}

	public Bitmap bytesToBitmap(byte[] b) {
		if (b.length != 0) {
			return BitmapFactory.decodeByteArray(b, 0, b.length);
		} else {
			return null;
		}
	}
	
	private static void waitDeviceList(AndroidDebugBridge bridge) {
		int count = 0;
		while (bridge.hasInitialDeviceList() == false) {
			try {
				Thread.sleep(100); // ���û�л���豸�б?��ȴ�
				count++;
			} catch (InterruptedException e) {
			}
			if (count > 300) {
				// �趨ʱ�䳬��300��100 ms��ʱ��Ϊ���ӳ�ʱ
				System.err.print("Timeout getting device list!");
				return;
			}
		}
	}

}
