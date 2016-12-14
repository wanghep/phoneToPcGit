package com.webservice;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.Buffer;

import android.util.Log;

import com.screencap.FileUtil;

public class IpServerSocketThread implements Runnable{

	public static final String TAG = "IpServerSocket";
	
	/* 接收socket数据处理 */
	private int MAX_READ_BUFFER_BYTES = 1024;
	private byte[] readDataBuffer = new byte[MAX_READ_BUFFER_BYTES];
	
	private byte[] rcvTotalLen = new byte [4];
	private int rcvSocketDataTotalLen;
	private byte[] rcvCommand = new byte[4];
	private int rcvSocketDataCommand;
	private byte[] rcvData = new byte[4];
	private int rcvSocketDataContent;
	
	private static final int PORT = 36669;
	private  ServerSocket serverSocket = null;
	private  Socket client = null;
	private static IpServerSocketThread instance = null;
	public static BufferedInputStream static_in = null;
	public static BufferedOutputStream static_out = null;
	private static final int SEND_IP = 0x11;
	private static String IP = null;
	
	public static IpServerSocketThread getInstance() {
		// TODO Auto-generated constructor stub
//		IP = ipAddress;
		if(instance == null){
			instance = new IpServerSocketThread();
		}
		return instance; 
	}
	
	public static void setIp(String ipAddress) {
		IP = ipAddress;
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		try {
			if(serverSocket == null ){
//				serverSocket = new ServerSocket(PORT);
				serverSocket = new ServerSocket(PORT, 0, InetAddress.getLocalHost());
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.d(TAG, "error message = " + e.getMessage());
			e.printStackTrace();
		}
		
		while(true){
			try {
				if(client == null){
					//接受客户端的请求
					Log.d(TAG, "client == " + serverSocket.accept());
					client = serverSocket.accept();
					static_in = new BufferedInputStream(client.getInputStream());
					static_out = new BufferedOutputStream(client.getOutputStream());
				}
				if(client != null && client.isConnected()){
					readDataFromSocket(static_in);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				Log.d(TAG, "Socket Connect Exception !!!!");
				client = null;
				e.printStackTrace();
			}
		}
	}
	
	/*读取数据*/
	public void readDataFromSocket(InputStream in){
		
		int numReadedBytes = 0;
		try {
			numReadedBytes = in.read(readDataBuffer);
			Log.d(TAG, "完成数据的读取，开始数据处理.....");
			if(numReadedBytes == -1){
				Log.d(TAG, "numReadedBytes == -1");
				return;
			}
			if(numReadedBytes > 0){
				//长度
				for(int i = 0; i < 4; i++){
					rcvTotalLen[i] = readDataBuffer[i];
				}
				rcvSocketDataTotalLen = FileUtil.bytesToInt(rcvTotalLen);
				//命令
				for(int i = 4;i < 8;i++){
					rcvCommand[i-4] = readDataBuffer[i];
				}
				rcvSocketDataCommand = FileUtil.bytesToInt(rcvCommand);
				Log.d(TAG, "Command = " + rcvSocketDataCommand);
				switch (rcvSocketDataCommand) {
				case 0x11: //Apply IP
					//数据
					for(int i = 8; i < 12; i++){
						rcvData[i-8] = readDataBuffer[i];
					}
					rcvSocketDataContent = FileUtil.bytesToInt(rcvData);
					Log.d(TAG, "Content = " + rcvSocketDataContent);
					if(rcvSocketDataContent == 0x12 && IP != null){
						//发送IP
						writeDataToSocket(getByte());
						return;
					}
					break;
				default:
					break;
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d(TAG, "read 36669 data exception !!!");
			e.printStackTrace();
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
	
	/*發送信息接口*/
	public void writeDataToSocket(byte[] data){
		if(static_out != null){
			try {
				static_out.write(data, 0, data.length);
				static_out.flush();
				Log.d(TAG, "Send IP success !!!!");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				Log.d(TAG, "writeDataToScoket error Exception !!!!");
				e.printStackTrace();
			}
		}
		else{
			Log.d(TAG, "writeDataToSocket error static_out = null");
		}
	}
	
	

}
