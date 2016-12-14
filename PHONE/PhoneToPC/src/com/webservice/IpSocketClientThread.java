package com.webservice;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

import com.phoneToPc.ThisAccessibilityService;
import com.screencap.FileUtil;

import android.R.integer;
import android.util.Log;
import android.widget.EditText;

public class IpSocketClientThread implements Runnable{
	
	private static final String HOST = "127.0.0.1";
	private static final int PORT = 36669;
	//10(十进制) = 1*16^1+0*16^0 = 16(十进制)
//	private static int LENGTH;
	private static final int SEND_IP = 0x11;
	private static String IP = null;
	private Socket socket = null;
	private Socket client;
	public static BufferedOutputStream static_out = null;
	public static BufferedInputStream static_in = null;
	private boolean isConnect = true;
	private static IpSocketClientThread instance = null;
	
	public IpSocketClientThread(){

	}
	
	
	public static IpSocketClientThread getInstance(String ipAdress){
		
		IP = ipAdress;
		if(instance == null){
			instance = new IpSocketClientThread();
		}
		return instance;
		
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		while(true){
			try {
	//			IP = ipAddress;
				Log.d(ClientSocketThread.TAG, "ip === "+IP);
				if(( socket == null) || (!socket.isConnected()))
				{	
					socket = new Socket(HOST,PORT);
					static_out = new BufferedOutputStream(socket.getOutputStream());
					static_in = new BufferedInputStream(socket.getInputStream());
				}
				if(socket != null && socket.isConnected()){
					Log.d(ClientSocketThread.TAG, "ip socket connect success !!!");
					writeDataToSocket(getByte());
					return;
				}else{
					
					Log.d(ClientSocketThread.TAG, "ip socket send fail!!!!");
				}
				
			}catch (Exception e) {
				// TODO Auto-generated catch block
				
				e.printStackTrace();
				socket = null;
				Log.d(ClientSocketThread.TAG, "ip send Exception!!!");
//				if(socket != null && static_in != null && static_out != null){
//					isConnect = false;
//					close();  
//				}
			}
		}
	}
	
	
	public byte[] getByte(){
		int IPlength = IP.getBytes().length;
		int LENGTH = 8 + IPlength;
		byte[] length = new byte[4];
		length = FileUtil.intToByte(LENGTH);
		byte[] cmd = new byte[4];
		cmd = FileUtil.intToByte(SEND_IP);
		byte[] ip = new byte[IPlength];  
		ip = IP.getBytes();
		int lengthAll = length.length + cmd.length + ip.length;
		byte[] bytes = new byte[lengthAll];
		System.arraycopy(length, 0, bytes, 0, length.length);
		System.arraycopy(cmd, 0, bytes, length.length, cmd.length);
		System.arraycopy(ip, 0, bytes, length.length+cmd.length, ip.length);
		return bytes;
	}
	
	
	public void close(){
		try {
			socket.close();
			static_in.close();
			static_out.close();
			Log.e("IpSocketClientThread","socket is closed!");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/* 发送信息接口 */
	public static void writeDataToSocket(byte data[]) {
		if (static_out != null) {
			try {
				static_out.write(data, 0, data.length);// 暂未处理发送数据，有待完成
				static_out.flush();
				Log.d(ClientSocketThread.TAG, "ip length = "+data.length);
				Log.d(ClientSocketThread.TAG, "ip socket send success!!!");
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
