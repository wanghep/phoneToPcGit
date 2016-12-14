package com.webservice;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.List;

import android.R.integer;
import android.annotation.SuppressLint;
import android.app.Instrumentation;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.net.Uri;
import android.os.Message;
import android.os.SystemClock;
import android.provider.Settings;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Toast;

import com.alpha.softkeyboard.AlphaIME;
import com.android.ddmlib.Client;
import com.phoneToPc.MainActivity;
import com.phoneToPc.R;
import com.phoneToPc.util;
import com.screencap.FileUtil;

/**
 * 
 * 功能：用于socket的交互 用于新socket连接的读写线程类
 * 
 */

@SuppressLint("NewApi")
public class ThreadReadWriterIOSocket implements Runnable {

	private Socket socket;
	private BufferedOutputStream out;
	private BufferedInputStream in; 
	static public BufferedOutputStream static_out = null;
	static public BufferedInputStream static_in = null;
	private Instrumentation instrumentation;
	private static ctrl_context_callback ctrlcb = null;
	private final static String TAG = "chengli";
	private Context mContext ;
	private int realnum = 0;
	private int site;
	
	private String customInputMethod;
	private String defaultInputMethod = null;

//	private MouseEventSocketClientThread mouseEventSocketClientThread ;
	private String mouseEventMsg = null;
	
	private RatioSocketClientThread alphagentSocketClientThread;
	
	/* 接收socket数据处理 */
	private int MAX_READ_BUFFER_BYTES = 1024;
	private byte[] readDataBuffer = new byte[MAX_READ_BUFFER_BYTES];
	
	private byte[] rcvTotalLen = new byte [4];
	private int rcvSocketDataTotalLen;
	private byte[] rcvCategories = new byte[4];
	private int rcvSocketDataCategories;
	private byte[] rcvCommand = new byte[4];
	private int rcvSocketDataCommand;
	private byte[] keyCodeBytes = new byte[4];
	private int keyCode;
	private byte[] mouseXBytes = new byte[4];
	private int mouseX;
	private byte[] mouseYBytes = new byte[4];
	private int mouseY;
	private int structSize = 1024;
	private int needToRecieve = structSize;
	boolean receiveFlag = true;//false;
	boolean sendFlag = false;
	private ClipboardManager mClipboard = null;
	private Boolean ioThreadFlag = true;
	private String curInputMethod = null;
	private boolean flag;
	private boolean sleep = false;
	
	//记录到本地的坐标
	private int downX;
	private int downY;

	ThreadReadWriterIOSocket(Socket socket) {
		
		this.socket = socket;
		instrumentation = new Instrumentation();
//		mouseEventSocketClientThread = new MouseEventSocketClientThread();
//		new Thread(mouseEventSocketClientThread).start();
		
//		alphagentSocketClientThread = new AlphagentSocketClientThread();
		Log.d(TAG,"go here,socket = "+socket);
		
	}
	
	public void setContext(Context context){
		mContext = context;
	}
	
	public void stop()
	{
		ioThreadFlag = false;
	}

	@Override
	public void run() {
		try {
			out = new BufferedOutputStream(socket.getOutputStream());
			in = new BufferedInputStream(socket.getInputStream());
			static_out = out; 
			static_in = in;
			Log.d(TAG,"mContext = "+mContext);
			InputMethodManager imm = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
			List<InputMethodInfo> list = imm.getInputMethodList();
			String str = mContext.getString(R.string.app_name); 
			for(int i=0;i<list.size();i++){
				InputMethodInfo info = list.get(i);
				if(info.loadLabel(mContext.getPackageManager()).equals(str)){
					site = i;
				}
			}
			customInputMethod = list.get(site).getId();
			if(defaultInputMethod == null){
				defaultInputMethod = Settings.Secure.getString(
						mContext.getContentResolver(), 
						Settings.Secure.DEFAULT_INPUT_METHOD);
			}
			
			while ( ioThreadFlag ) {
				curInputMethod = Settings.Secure.getString(
						mContext.getContentResolver(), 
						Settings.Secure.DEFAULT_INPUT_METHOD);
				try {
					if (socket == null || ( !socket.isConnected()) ) {
						break;
					}
					else if(socket != null && socket.isConnected()){
						/**
						 * 当手机与pc端建立连接之后，自动切换为自定义的输入法
						 * */
//						Settings.Secure.putString(
//								mContext.getContentResolver(), 
//								Settings.Secure.DEFAULT_INPUT_METHOD,
//								customInputMethod); 
					}
					
					// 接收服务器端信息
					
					if(in!=null){
						Log.d(TAG,"read form pc,in="+in.toString());
					    readDataFromSocket(in);
					}else {
						break;
					}

					// 向服务器端发送信息 
					/*if(ClientSocketThread.ioThreadFlag==true){
						//writeDataToSocket(readDataBuffer);
					}*/
				
				} catch (Exception e) {

					ioThreadFlag = false;
					Log.e(ClientSocketThread.TAG, Thread.currentThread()
							.getName() + "---->" + "read write error111111");
				}
			}
			out.close();
			in.close();
		} catch (Exception e) {
			ioThreadFlag = false;
			Log.e(ClientSocketThread.TAG, Thread.currentThread().getName()
					+ "---->" + "read write error222222");
			e.printStackTrace();
		} finally {
			/**
			 * 手机和pc端断开连接后，切换为系统默认的输入法
			 * */
//			Settings.Secure.putString(
//					mContext.getContentResolver(),
//					Settings.Secure.DEFAULT_INPUT_METHOD,
//					defaultInputMethod);
		
			//if connect non,send message
			Message message = MainActivity.messageHander.obtainMessage();
			message.what = 2;
			MainActivity.messageHander.sendMessage(message);
			Log.d("ServerSocketThread", "handle message !!!");
//			Toast.makeText(mContext, "catch", Toast.LENGTH_SHORT).show();
			try {
				if (socket != null) {
					
					Log.v(ClientSocketThread.TAG, Thread.currentThread()
							.getName() + "---->" + "client.close()");
					socket.close();
					socket = null;
//					MainActivity.GetActivityInstance().showSocket("socket connect failed ! socket client was closed !");
					Log.d(TAG, "socket connect failed !  socket client was closed !");
				}
			} catch (IOException e) {
				Log.e(ClientSocketThread.TAG, Thread.currentThread().getName()
						+ "---->" + "read write error333333");
				e.printStackTrace();
			}
			ioThreadFlag = false;
		}

	}

	/* 读取命令 */
	public void readDataFromSocket(InputStream in) {

		System.out.println("读取数据...");
        Log.d(TAG,"read data from pc,in = "+in.toString());
		int numReadedBytes = 0;
		try {
			/*numReadedBytes = in.read(readDataBuffer,
					(structSize - needToRecieve), needToRecieve);
			Log.d(TAG,"111111111111111,numReadedBytes = "+numReadedBytes+",needToRecieve = "+needToRecieve);*/
			flag = true;
			numReadedBytes = in.read(readDataBuffer);
			if (numReadedBytes == -1) {
				ioThreadFlag = false;
				return;
			}
			Log.d(ClientSocketThread.TAG,"完成一次数据读取，开始处理数据！"); 
			if (numReadedBytes > 0) {
				rcvDataHandle(); // 接收完毕处理数据
			} else {
				
			}

			/*needToRecieve -= numReadedBytes;

			if (needToRecieve <= 0) 
			{
				System.out.printlnLog.d(TAG,"完成一次数据读取，开始处理数据！");
				realnum=numReadedBytes;
				rcvDataHandle(); // 接收完毕处理数据
				//receiveFlag = false;
				needToRecieve = structSize;
			}*/

		} catch (Exception e) {
//			ioThreadFlag = false;
			Log.v(ClientSocketThread.TAG, Thread.currentThread().getName()
					+ "---->" + "readFromSocket error1");
			e.printStackTrace();
		}
	}

	/* 处理接收到数据 */
	public void rcvDataHandle() {
		Log.d(TAG,"rcvDataHandle,readDataBuffer = "+readDataBuffer.toString());
		Log.d(TAG,"rcvDataHandle,realnum = "+realnum);
		//if(realnum>0 && realnum<4 ||realnum>8)
		//	doReceiveDataFromPc(readDataBuffer);
		//else
		{
		for (int i = 0; i < 4; i++) {
			rcvTotalLen[i] = readDataBuffer[i];
		}
		rcvSocketDataTotalLen = FileUtil.bytesToInt(rcvTotalLen);
		
		for (int i = 4; i < 8; i++) {
			rcvCategories[i - 4] = readDataBuffer[i];
		} 
		rcvSocketDataCategories = FileUtil.bytesToInt(rcvCategories);
        Log.d(ClientSocketThread.TAG,"rcvSocketDataCategories = "+rcvSocketDataCategories);    

        switch (rcvSocketDataCategories) {
		case 1: // 1: MouseEvent
			
			Log.d(ClientSocketThread.TAG,  "readDataBuffer length == "+readDataBuffer.length);
			
			for (int i = 8; i < 12; i++) {
				rcvCommand[i - 8] = readDataBuffer[i];
			}
			rcvSocketDataCommand = FileUtil.bytesToInt(rcvCommand);
			Log.d(ClientSocketThread.TAG, "rcvCommand === "+rcvSocketDataCommand);
			
			for (int i = 12; i < 16; i++) {
				mouseXBytes[i - 12] = readDataBuffer[i];
			}
			mouseX = FileUtil.bytesToInt(mouseXBytes);
			Log.d(ClientSocketThread.TAG, "mouseX ==== " + mouseX);
			
			for (int i = 16; i < 20; i++) {
				mouseYBytes[i - 16] = readDataBuffer[i];
			}
			mouseY = FileUtil.bytesToInt(mouseYBytes);
			Log.d(ClientSocketThread.TAG, "mouseY ==== " + mouseY);
			
			// actionSwtich == 0 MouseEvent.ACTION_DOWN
			if (rcvSocketDataCommand == 0) {
				/*instrumentation.sendPointerSync(MotionEvent.obtain(
						SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
						MotionEvent.ACTION_DOWN, mouseX, mouseY, 0));*/
				downX = mouseX;
				downY = mouseY;

			// actionSwtich == 1 MouseEvent.ACTION_UP
			} else {
//				mouseEventSocketClientThread.sendMsg(null);
				/*instrumentation.sendPointerSync(MotionEvent.obtain(
						SystemClock.uptimeMillis(), SystemClock.uptimeMillis(),
						MotionEvent.ACTION_UP, mouseX, mouseY, 0));*/
				//down时候
				mouseEventMsg = "touch down " + downX +" " + downY;
				MouseEventSocketClientThread.sendMsg(mouseEventMsg);
				Log.d(ClientSocketThread.TAG, "Monkey Down=========================");
				Log.d(ClientSocketThread.TAG, "mouseX="+downX+","+"mouseY="+downY);

				//move时候
				mouseEventMsg = "touch move " + mouseX + " " + mouseY;
				MouseEventSocketClientThread.sendMsg(mouseEventMsg);
				Log.d(ClientSocketThread.TAG, "Monkey Move==========================");
//				SystemClock.sleep(10);
				
				//up时候
				mouseEventMsg = "touch up "+ mouseX + " " + mouseY;;
				MouseEventSocketClientThread.sendMsg(mouseEventMsg);
				Log.d(ClientSocketThread.TAG, "Monkey Up============================");
				Log.d(ClientSocketThread.TAG,"mouseX = " + mouseX + "; mouseY= " + mouseY);  

//				mouseEventMsg = "touch down 504 1000";
//				mouseEventSocketClientThread.sendMsg(mouseEventMsg);
//				Log.d(ClientSocketThread.TAG, "Monkey Down================");
//				SystemClock.sleep(1000);
//				
//				mouseEventMsg = "touch move 1242 1000";
//				mouseEventSocketClientThread.sendMsg(mouseEventMsg);
//				SystemClock.sleep(1000);
//				
//				mouseEventMsg = "touch up 1242 1000";
//				mouseEventSocketClientThread.sendMsg(mouseEventMsg);
//				Log.d(ClientSocketThread.TAG, "Monkey Up==================");
			
			}

			
			break;

		case 2: // 2: KeyEvent

			for (int i = 8; i < 12; i++) {
				rcvCommand[i - 8] = readDataBuffer[i];
			}
			rcvSocketDataCommand = FileUtil.bytesToInt(rcvCommand);

			for (int i = 12; i < 16; i++) {
				keyCodeBytes[i - 12] = readDataBuffer[i];
			}
			keyCode = FileUtil.bytesToInt(keyCodeBytes);

			// actionSwtich == 0 KeyEvent.ACTION_DOWN
			if (rcvSocketDataCommand == 0) {
				//收到pc敲击键盘的命令，切换为自定义的输入法
				if(curInputMethod != customInputMethod){
					Settings.Secure.putString(
							mContext.getContentResolver(), 
							Settings.Secure.DEFAULT_INPUT_METHOD,
							customInputMethod);
				}
				flag = false;
				if(!sleep){
					sleep = true;
				}else{
					sleep = false;
				}
				/*instrumentation.sendKeyDownUpSync(util
						.PCKeyCodeToPhoneKeyCode(keyCode));*/
				Log.d(ClientSocketThread.TAG, "custom = " + customInputMethod);
				AlphaIME.getInstance().sendKeyEvent(util.PCKeyCodeToPhoneKeyCode(keyCode));
			// actionSwtich == 1 KeyEvent.ACTION_UP
			} else {
				/*instrumentation.sendKeyDownUpSync(util
						.PCKeyCodeToPhoneKeyCode(keyCode));*/
			}
			
			Log.d(ClientSocketThread.TAG,"KeyEvent_PC_keyCode = " + keyCode + "; KeyEvent_PHONE_keyCode = " + util
					.PCKeyCodeToPhoneKeyCode(keyCode)); 
			break;
		case 3: // 3: Ctrl信息
		Log.d(TAG,"33333333,ctrl按键");

			for (int i = 8; i < 12; i++) {
				rcvCommand[i - 8] = readDataBuffer[i];
			}
			rcvSocketDataCommand = FileUtil.bytesToInt(rcvCommand);
			char c = (char) rcvSocketDataCommand;
			Log.d(TAG,"传来的组合字符是：rcvSocketDataCommand = "+ c);
			keyCode= rcvSocketDataCommand-36;
			
			Log.d(TAG,"keyCode = "+keyCode);
			//if (rcvSocketDataCommand == 0) {
				//KeyEvent singleEvent = new KeyEvent(KeyEvent.ACTION_DOWN,util.PCKeyCodeToPhoneKeyCode(keyCode));
				//KeyEvent compEvent = new KeyEvent(0,0,KeyEvent.ACTION_DOWN,keyCode,0,0x1000);
				//instrumentation.sendKeySync(new KeyEvent(0,0,KeyEvent.ACTION_DOWN,keyCode,0,0x1000));
				//instrumentation.sendKeySync(new KeyEvent(0,0,KeyEvent.ACTION_UP,keyCode,0,0x1000));
			//} else {

			//}		
			if(keyCode==31)
			{
			    //获取剪切板的内容
			 	String msg=MainActivity.getTextDataFromClipBoard();
//			 	writeDataToSocket(msg.getBytes());
			}
			break;
			
		case 5: // 5: Socket信息
			byte readSocketHeardBeatMassage = readDataBuffer[8];
			Log.d(ClientSocketThread.TAG,"ReadSocketHeardBeatMassage = " + readSocketHeardBeatMassage);
			ClientSocketThread.HeardBeatConnect = true;
			break;
			
		case 7: //接受PC端傳輸的Ctrl信息，放到剪切板中
			byte[] rcvClipData = new byte[rcvSocketDataTotalLen-4];			
			for (int i = 8; i < rcvSocketDataTotalLen+4; i++) {
				rcvClipData[i-8] = readDataBuffer[i];
			}
			String textClipData = new String(rcvClipData);
			Log.d(ClientSocketThread.TAG, "Ctrl Message = " + textClipData);
			MainActivity.GetActivityInstance().showSocket(textClipData);
			MainActivity.putTextDatatoClipBoard(textClipData);
			break;
			
		default:
//			Log.d(TAG,"44444,default,data from PC放入android,rcvSocketDataTotalLen = "+rcvSocketDataTotalLen);
//			byte[] RawData = new byte[rcvSocketDataTotalLen];			
//			for (int i = 0; i < rcvSocketDataTotalLen; i++) {
//				RawData[i] = readDataBuffer[i+4];
//			}
//			doReceiveDataFromPc(RawData);	
			break;
		}
		}

		if (sleep) {
			new Thread(){
				public void run() {
					try {
						sleep(3000);
						Log.d(ClientSocketThread.TAG, "sleep 3000=======");
							if(flag == true){
								Settings.Secure.putString(
										mContext.getContentResolver(), 
										Settings.Secure.DEFAULT_INPUT_METHOD,
										defaultInputMethod);
								sleep = false;
								Log.d(ClientSocketThread.TAG, "sleep 200ms");
							}
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
						Log.d(ClientSocketThread.TAG, "sleep exception !!!");
					}
				};
			}.start();
		}
		Log.d(TAG,"buffer清空，等待下一次数据");
//		readDataBuffer= null;
	}

	/* 发送信息接口 */
	public static void writeDataToSocket(byte data[]) {
		if (static_out != null) {
			try {
				static_out.write(data, 0, data.length);// 暂未处理发送数据，有待完成
				static_out.flush();
				Log.d(ClientSocketThread.TAG, "Send Message Success!!!");
			} catch (Exception e) {
				// TODO Auto-generated catch block  
				Log.e( TAG ," writeDataToSocket error Exception:  " + e.getMessage() );
				e.printStackTrace();
			}
		}
		else
		{
			Log.e( TAG ," writeDataToSocket error static_out = null ");
		}
	}

	
	/* Ctrl信息接口 */
	public interface ctrl_context_callback {
		public void ctrl_context_comein(byte context[]);
	}

	/* Ctrl接口注册 */
	public static void registCtrlContextCallBack(ctrl_context_callback cb) {
		ctrlcb = cb;
	}

	
	public void doReceiveDataFromPc(byte [] data)
	{
		String textData = new String(data);
		Log.d(TAG,"333333,doReceiveDataFromPc,textData = "+textData+",receiveFlag = "+receiveFlag);
		if(true)
		{
			Log.d(TAG,"put text to clipboard");
			MainActivity.putTextDatatoClipBoard(textData);
		}
		else if (sendFlag)
		{
			Log.d(TAG,"55555,put text to clipboard");
			String phoneData=MainActivity.getTextDataFromClipBoard();
			Log.d(TAG,"55555,send phoneData  to PC");
//			writeDataToSocket(phoneData.getBytes());
		}
		else if (textData=="0" )
		{
			Log.d(TAG,"44444,from PC copy to android");
		    receiveFlag = true; 
		}
		else if (textData=="1")
		{
			Log.d(TAG,"from android copy to PC");
		    sendFlag = true; 
        }
	}

}

