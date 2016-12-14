package com.webservice;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Point;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;
import com.phoneToPc.R.string;
import com.screencap.FileUtil;

public class RatioSocketClientThread {

	private static final String HOST = "127.0.0.1";
	public static final String TAG = "RatioSocketClientThread";
	private static final int PORT = 36669;
	private static final int LENGTH = 0x10;
	private static final int TYPE = 0;
	private static final int SCAN_RATIO = 0;
	public static BufferedOutputStream static_out = null;
	public static BufferedInputStream static_in = null;
	private Socket socket = null;
	private PrintWriter out = null;
	private boolean isConnect = true;
	private int WIDTH;
	private int HEIGHT;
	private Context mContext;
	private String scan_ratio;
	
	public RatioSocketClientThread(Context context,Socket socket) {
		// TODO Auto-generated constructor stub
		try {
			this.socket = socket;
			static_out = new BufferedOutputStream(socket.getOutputStream());
			static_in = new BufferedInputStream(socket.getInputStream());
			mContext = context;
			getPhoneSize();
			if(socket != null && socket.isConnected()){
				Log.d(ClientSocketThread.TAG, "WIDTH ==== " + WIDTH);
				Log.d(ClientSocketThread.TAG, "HEIGHT ==== " + HEIGHT);
				writeDataToSocket(getByte());
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			isConnect = false;
			e.printStackTrace();
		}
	}
	
	public byte[] getByte(){
		
		byte[] length = new byte[4];
		//length = FileUtil.intToByte(LENGTH);
		length = FileUtil.intToByte(16);
		byte[] type = new byte[4];
		type = FileUtil.intToByte(TYPE);
		byte[] cmd = new byte[4];
		cmd = FileUtil.intToByte(SCAN_RATIO);
		byte[] width = new byte[4];
		width = FileUtil.intToByte(WIDTH);
		byte[] height = new byte[4];
		height = FileUtil.intToByte(HEIGHT);
		int lengthAll = length.length + type.length + cmd.length + width.length + height.length;
		byte[] bytes = new byte[lengthAll];
		System.arraycopy(length, 0, bytes, 0, length.length);
		System.arraycopy(type, 0, bytes, length.length, type.length);
		System.arraycopy(cmd, 0, bytes, length.length + type.length, cmd.length);
		System.arraycopy(width, 0, bytes, length.length + type.length + cmd.length, width.length);
		System.arraycopy(height, 0, bytes, length.length + type.length + cmd.length + width.length, height.length);
		
		Log.d(ClientSocketThread.TAG, "lengthAll === "+lengthAll);
		return bytes;
	}
	
	
	
	public void close() {
		try {
			socket.close();
			Log.e("AlphagentSocketClientThread","socket is closed!");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	//获取当前手机的屏幕分辨率
	@SuppressLint("NewApi")
	public void getPhoneSize(){
		Point size = new Point();
		WindowManager wm = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
		Display display = wm.getDefaultDisplay();
		display.getSize(size);
		WIDTH = size.x;
		HEIGHT = size.y;
//		for(int i=0; i<9;i++){
//			Log.d(ClientSocketThread.TAG, "WIDTH ==== " + WIDTH);
//			Log.d(ClientSocketThread.TAG, "HEIGHT ==== " + HEIGHT);
//		}
	}
	
	/* 发送信息接口 */
	public static void writeDataToSocket(byte[] data) {
		if (static_out != null) {
			try {
				static_out.write(data, 0, data.length);// 暂未处理发送数据，有待完成
				static_out.flush();
			} catch (Exception e) {
				// TODO Auto-generated catch block  
				Log.e( "SEND" ," writeDataToSocket error Exception:  " + e.getMessage() );
				e.printStackTrace();
			}
		}
		else
		{
			Log.e( "SEND" ," writeDataToSocket error static_out = null ");
		}
	}
	
}
