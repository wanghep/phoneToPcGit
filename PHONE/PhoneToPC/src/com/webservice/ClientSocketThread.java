package com.webservice;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;

import android.os.Handler;
import android.util.Log;

import com.phoneToPc.MainActivity;


public class ClientSocketThread implements Runnable {

	public static final String TAG = "ClientSocketThread";
	//public static Boolean IsSokcetConnect = false;
	public static Boolean ClientSocketThreadFlag = false;
	//private final String SERVER_IP = "10.106.9.144";   // le-wifi
	//private final String SERVER_IP = "172.18.18.68";     // simon
	//private final String SERVER_IP = "172.18.18.66";     // orange
	//private final String SERVER_IP = "10.4.64.55";     //本机测试
	public static String SERVER_IP = null; 
	public static int SERVER_PORT =8888;
	private InetSocketAddress inetSocketAddress;
	private Socket clientSocket = null;
	private ThreadReadWriterIOSocket rwiosocket = null;
	static public boolean HeardBeatConnect = false;
	private boolean tryToReconnect =true;

	/** 心跳检测时间  */
	private static final long HEART_BEAT_RATE = 6 * 1000;
	private long sendTime = 0L;
	private static byte HEART_BEAT_NUM = 1;

	public void run() {
		ClientSocketThreadFlag = true;
		//SERVER_IP = "192.168.49.233";
		SERVER_IP = "10.4.64.55";     //本机测试
		/*while (SERVER_IP == null) {
			if (false == mainThreadFlag) {
				break;
			}
		}*/
		inetSocketAddress = new InetSocketAddress(SERVER_IP, SERVER_PORT);
		Log.d(TAG, "Try to connect the socket...( SERVER_IP = " + SERVER_IP + " )");
		while (ClientSocketThreadFlag) {

			if (tryToReconnect) {
				ThreadReadWriterIOSocket.static_out = null;
				ThreadReadWriterIOSocket.static_in = null;

				if (clientSocket != null) {
					if (rwiosocket != null) {
						rwiosocket.stop();
					}
					try {
						clientSocket.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}

				// if( clientSocket == null )
				{
					clientSocket = new Socket();
				}

				if (!clientSocket.isConnected() || clientSocket.isClosed()) {
					try {
//						MainActivity.GetActivityInstance().showSocket(
//								"try to reconnect the socket...  HEART_BEAT_NUM = "
//										+ HEART_BEAT_NUM);
						Log.d(TAG,
								"try to reconnect the socket...  HEART_BEAT_NUM = "
										+ HEART_BEAT_NUM);
						clientSocket.connect(inetSocketAddress);
						HeardBeatConnect = true;
						tryToReconnect = false;
					} catch (Exception e) {
						// TODO Auto-generated catch block
						if (clientSocket != null) {
							try {
								clientSocket.close();
							} catch (IOException e1) {
								// TODO Auto-generated catch block
								e1.printStackTrace();
							}
						}
						e.printStackTrace();
//						MainActivity
//								.GetActivityInstance()
//								.showSocket(
//										"socket connect failed !try to reconnect the socket...");
						Log.d(TAG,
								"socket connect failed !try to reconnect the socket...");
						// clientSocket = new Socket();
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
						tryToReconnect = true;
						continue;
					}
//					MainActivity.GetActivityInstance().showSocket(
//							"socket connect success !");
					Log.d(TAG, "socket connect success !");
					rwiosocket = new ThreadReadWriterIOSocket(clientSocket);
					new Thread(rwiosocket).start();
					mHandler.postDelayed(heartBeatRunnable, HEART_BEAT_RATE);// 初始化成功后，就准备发送心跳包
				}
			}

		}
		if (this.rwiosocket != null) {
			rwiosocket.stop();
		}
	}
	
	/**
	 * Socket发送心跳包，判断连接
	 * Socket中有一个方法void sendUrgentData(0xFF)当对方SO_OOBINLINE属性没有打开时，
	 * 就会自动舍弃这个字节，而SO_OOBINLINE属性默认情况下就是关闭的，
	 * 但是只要能够发送到对方Socket，sendUrgentData(0xFF)方法就不会报错，
	 * 只要catch一下IOException就OK了，当对方关掉了Socket之后就会进入catch
	 *//*
	private boolean isSocketConnect(Socket socket){
		 try {  
		        socket.sendUrgentData(0xFF);  
		        return true;
		    } catch (Exception e) {  
		        //网络断开  
		        e.printStackTrace();  
		        return false;
		    }  
		
	}*/
	
	// 发送心跳包
	private int errorPacket = 0;
	private Handler mHandler = new Handler();
	private Runnable heartBeatRunnable = new Runnable() {
		@Override
		public void run() {
			
			if( HeardBeatConnect )
			{
//				MainActivity.GetActivityInstance().showSocket("packet ok: " + (HEART_BEAT_NUM -1)+"  errTotal(" +  errorPacket + ")" );
				
				byte heardBeat[] = new byte[9];
				
				heardBeat[0] = 1;
				heardBeat[1] = 0;
				heardBeat[2] = 0;
				heardBeat[3] = 0;
				heardBeat[4] = 4;
				heardBeat[5] = 0;
				heardBeat[6] = 0;
				heardBeat[7] = 0;
				heardBeat[8] = HEART_BEAT_NUM++;
				          
				ThreadReadWriterIOSocket.writeDataToSocket(heardBeat);// 就发送一个\r\n过去, 如果发送失败，就重新初始化一个socket
				mHandler.postDelayed(this, HEART_BEAT_RATE);
				HeardBeatConnect = false;
			}
			else
			{
				errorPacket++;
				
//				MainActivity.GetActivityInstance().showSocket("packet missing: " + (HEART_BEAT_NUM -1) +"  errTotal(" +  errorPacket + ")");
				HEART_BEAT_NUM++;
				mHandler.postDelayed(this, HEART_BEAT_RATE);
				/*
				if( rwiosocket != null )
				{
					rwiosocket.stop();
				}
				rwiosocket = null;
				
				if(clientSocket != null ) 
				{
					MainActivity.GetActivityInstance().showSocket("socket disconnected!");
					try {
						clientSocket.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				//tryToReconnect = true;
				 * */
				 
				
			}
			
		}
	};
	
	public boolean sendMsg(byte []msg) {
		if (null == clientSocket) {
			return false;
		}
		try {
			if (!clientSocket.isClosed() && !clientSocket.isOutputShutdown()) {
				OutputStream os = clientSocket.getOutputStream();
				os.write(msg);
				os.flush();
				sendTime = System.currentTimeMillis();// 每次发送成功数据，就改一下最后成功发送的时间，节省心跳间隔时间
				Log.i(TAG, "发送成功的时间：" + sendTime);
			} else {
				return false;
			}
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}
}