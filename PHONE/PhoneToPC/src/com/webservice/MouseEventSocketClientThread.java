package com.webservice;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import com.android.ddmlib.log.LogReceiver.LogEntry;

import android.R.bool;
import android.os.SystemClock;
import android.util.Log;

public class MouseEventSocketClientThread implements Runnable {

	private static final String HOST = "127.0.0.1";
	private static final int PORT = 36667;
	private static Socket socket = null;
	private static BufferedReader in = null;
	private static PrintWriter out = null;
	
	private boolean isRunning = true;
	
	private static MouseEventSocketClientThread mouseEventSocketClientThread = null;

	MouseEventSocketClientThread() {
		
	}
	
	public static MouseEventSocketClientThread getInstance(){
		
		if(mouseEventSocketClientThread == null){
			mouseEventSocketClientThread = new MouseEventSocketClientThread();
			Log.d(ClientSocketThread.TAG, " mouseEventSocket == "+mouseEventSocketClientThread);
		}
		return mouseEventSocketClientThread;
		
	}

	@Override
	public void run() {
		// TODO Auto-generated method stub 
		while (isRunning) {
			try {
				if (socket == null) {
					socket = new Socket(HOST, PORT);
					in = new BufferedReader(new InputStreamReader(
							socket.getInputStream()));
					out = new PrintWriter(socket.getOutputStream(), true);
					Log.e(ClientSocketThread.TAG, "monkey socket connect success!!!");
					// break;
//					return;
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				if (socket != null && in != null && out != null) {
					close();
				}
				socket = null;
				Log.d(ClientSocketThread.TAG, "monkey socket connect fail!!!!!");
			}
		}
	}

	public static void sendMsg(String msg) {
		try {
			if (socket.isConnected() && socket != null) {
				out.println(msg);
				Log.d(ClientSocketThread.TAG,
						"Monkey Send OK=========================");
			} 
		} catch (Exception e) {
			Log.e(ClientSocketThread.TAG, "socket == " + socket);
			e.printStackTrace();
			if (socket != null) {
				close();
			}
		}
	}

	public static void close() {
		try {
			out.close();
			in.close();
			socket.close();
			Log.e("MouseEventSocketClientThread", "socket is closed!");
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}