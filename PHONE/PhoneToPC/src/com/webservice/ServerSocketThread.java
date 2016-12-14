package com.webservice;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import android.content.Context;
import android.os.Message;
import android.util.Log;

import com.phoneToPc.MainActivity;
import com.phoneToPc.R;
import com.phoneToPc.R.layout;


public class ServerSocketThread implements Runnable {
	
	private String TAG = "ServerSocketThread";
	public static Boolean ServerSocketThreadFlag = false;
	private ServerSocket serverSocket;
	private final int SERVER_PORT = 8888;
	private ThreadReadWriterIOSocket rwiosocket = null;
	private Context context;
	
	public void setContext(Context context){
		this.context = context;
	}
	
	public void run() {
		ServerSocketThreadFlag = true;
		serverSocket = null;
		try {
			serverSocket = new ServerSocket(SERVER_PORT);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		while (true) {
			try {	
				Log.d(TAG, "serverSocket wait connect !!!!");
				/*MainActivity.GetActivityInstance().showSocket(
				"wait socket connect ... ");
				Log.d(TAG, "socket wait client connect ... ");*/
				// 接受客户端请求
				Socket client = serverSocket.accept();
				//send connect on message
				Message message = MainActivity.messageHander.obtainMessage();
				message.what = 1;
				MainActivity.messageHander.sendMessage(message);
				//获取pc端的IP地址
				String remoteIp = client.getInetAddress().getHostAddress();
				Log.d(ClientSocketThread.TAG, "ip:="+remoteIp);
//				new Thread(IpServerSocketThread.getInstance()).start();
				IpServerSocketThread.getInstance().setIp(remoteIp);
//				new Thread(MouseEventSocketClientThread.getInstance()).start();
				//发送分辨率给PC端
				if(client != null && client.isConnected() ){
					new RatioSocketClientThread(context, client);
					Log.d(TAG, "socket connect success !"); 
				}
				rwiosocket = new ThreadReadWriterIOSocket(client);
				rwiosocket.setContext(context);
				new Thread(rwiosocket).start();
			} catch (Exception e) {
				e.printStackTrace();
				System.out.println(e.getMessage());
			} finally {
//				try {
//					if (serverSocket != null) {
// 						serverSocket.close();
//						Log.d(TAG, "---->  serverSocket closed.");
//					}
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
			}
		}
	}	
}
