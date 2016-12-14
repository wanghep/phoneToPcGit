package com.webservice;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

import android.app.Activity;
import android.app.Service;
import android.app.VKeyboardManager;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import com.phoneToPc.MainActivity;
import com.screencap.FileUtil;


/**
 * 设置：android手机
 * 
 * 
 * */
public class androidService extends Service {
	
	webServiceThread webSocket = null;
	FileUtil fileUtil = new FileUtil();
//	Activity me = MainActivity.GetActivityInstance();
	// 图片存储路径
	String SavePath = fileUtil.getSDCardPath() + "/ScreenshotsImages/";
	String pngFileName = "1.png";
	String jpegFileName = "0.jpeg";
	static public  VKeyboardManager mKeyManager = null;
	ClientSocketThread clientSocketThread = null;
	ServerSocketThread serverSocketThread = null;
	
	@Override
	public void onCreate() {
		super.onCreate();
		mKeyManager = (VKeyboardManager)this.getSystemService("virtual_keyboard");
		
		webSocket = new webServiceThread();	
		new Thread( webSocket ).start();
		
		serverSocketThread = new ServerSocketThread();
		serverSocketThread.setContext(getBaseContext());
		new Thread( serverSocketThread ).start();
		
		new Thread(IpServerSocketThread.getInstance()).start();
		
		/*clientSocketThread = new ClientSocketThread();
		new Thread( clientSocketThread ).start();*/
	}
	
	@SuppressWarnings("static-access")
	@Override
	public void onDestroy() {
		

		if( webSocket != null )
		{
			webSocket.stop();
		}
		//关闭线程
		serverSocketThread.ServerSocketThreadFlag = false;
		//clientSocketThread.ClientSocketThreadFlag = false;
		super.onDestroy();
	}

	@Override
	public void onStart(Intent intent, int startId) {

		super.onStart(intent, startId);
	}

	@Override
	public IBinder onBind(Intent arg0) {

		return null;
	}
}