package com.phoneToPc;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;

import android.R.integer;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipData.Item;
import android.content.ClipboardManager;
import android.content.ClipboardManager.OnPrimaryClipChangedListener;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.os.SystemClock;
import android.text.Html;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.CommandParser.CommandE;
import com.CommandParser.Property;
import com.screencap.FileUtil;
import com.screencap.ScreenShotBySurfaceControl;
import com.screencap.ScreenShotDDMS;
import com.screencap.ScreenShotFb;
import com.tmac.adapter.ListViewAdapter;
import com.webservice.ClientSocketThread;
import com.webservice.ThreadReadWriterIOSocket;
import com.webservice.androidService;
import com.zxing.activity.CaptureActivity;

public class MainActivity extends Activity implements OnClickListener {
	private final static int SCANNIN_GREQUEST_CODE = 10;
	private final static String TAG = "MainActivity";
	private ImageView startScan = null;
	public static String IpAddress = "";
	public HandlerThread pHttpThread = null;
	public Handler handler = null;
	public static MainActivity instance = null;
	private static final int SHOW_SOCKTE = 0;
	private static final int CONNECT_ON = 1;
	private static final int CONNECT_NON = 2;
	private EditText editText = null;

	FileUtil fileUtil = new FileUtil();
	ScreenShotDDMS sS = new ScreenShotDDMS();
	ScreenShotFb sSFB = new ScreenShotFb();
	ScreenShotBySurfaceControl sSBSC = new ScreenShotBySurfaceControl();
	Timer timer;
	int count = 0;
	String SavePath = fileUtil.getSDCardPath() + "/ScreenshotsImages/";
	String picture0 = "0.jpeg";
	String picture1 = "1.png";
	String rawFile = "inputStreamFile.raw";
	Bitmap pictureToBitmap;
	Bitmap pictureToBitmapForDecode = null;
	boolean exit = false;
	File path = new File(SavePath);

	private Process process;
	private DataInputStream in;
	private InputStream inStream;
	private BufferedReader reader = null;
	// 将数据按照文本输出到文件
	// private PrintWriter writer = null;
	private FileWriter writer = null;
	private Context mContext;
	private static ClipboardManager mClipboard;
	
	private byte[] sendTotalLen = new byte [4];
	private int sendSocketDataTotalLen;
	private byte[] sendCategories = new byte[4];
	private int sendSocketDataCategories;

	// xutao
	private static final int SCAN_BACK_CODE = 6;
	private TextView wifiIpState;
	private RelativeLayout deviceConLatest;
	//shell命令
//	private String exe_path = "data/data/com.phoneToPc/mutilshotagent";
	private String exe_path = "data/local/tmp/mutilshotagent";
	private String exe_monkey_path = "data/data/com.phoneToPc/monkey";
	private String exe_monkey = "data/data/com.phoneToPc/monkey --port 8688";  
	private String exe_chomd_dir = "chmod -R 777 /data/data/com.phoneToPc";
	private String exe_chomd_mutilShotAgent = "chmod -R 777 /data/data/com.phoneToPc/mutilshotagent";
	private String exe_chown_mutilShotAgent = "chown shell mutilshotagent";
	private String exe_chown_monkey = "chown shell monkey";
	private String exe_chgrp_mutilshotagent = "chgrp shell mutilshotagent";
	private File exe_file;
	private int mutilshotagent = 0;
	//Layout
	private List<String> list;
	private ListViewAdapter adapter;
	private MyListView listView;
	public static TextView title_title;
	
	//接受连接WiFi的广播，显示ip
	private BroadcastReceiver wifiReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub

			if (intent.getAction().equals(
					WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
				NetworkInfo info = intent
						.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
				if (info.getState().equals(NetworkInfo.State.DISCONNECTED)) {
					wifiIpState.setText(getResources().getString(
							R.string.connect_state_non));
					wifiIpState.setTextColor(getResources().getColor(
							R.color.red));
				} else if (info.getState().equals(NetworkInfo.State.CONNECTED)) {
					if (Utils.isWiFiActive(getApplicationContext())) {
						wifiIpState.setText(Utils
								.getWiFiIp(getApplicationContext()));
						wifiIpState.setTextColor(getResources().getColor(
								R.color.black));
					}
				}
			}
		}
	};
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		startService(new Intent(this, androidService.class));
		// ComponentName svr = startService(new Intent(this,
		// ThisAccessibilityService.class));
		IpAddress = util.getLocalIP_Address();
		Log.d(this.getClass().getName(), "IP: " + IpAddress);
		// init();
		instance = this;
		// Start socket data monitoring!
		Log.d(TAG, "Start socket data monitoring!");
		// ctrl_context_callback cb = null;
		// ThreadReadWriterIOSocket.registCtrlContextCallBack(cb);
		mContext = this.getBaseContext();
		mClipboard = (ClipboardManager) mContext
				.getSystemService(Context.CLIPBOARD_SERVICE);
		mClipboard.addPrimaryClipChangedListener(new OnPrimaryClipChangedListener() {
			@Override
			public void onPrimaryClipChanged() {
				// TODO Auto-generated method stub
				ClipData data = mClipboard.getPrimaryClip();
				Item item = data.getItemAt(0);
				Log.e(ClientSocketThread.TAG, "复制文字========:"+item.getText());	
 				sendSocketDataTotalLen = item.getText().toString().getBytes().length+4;
				sendTotalLen = FileUtil.intToByte(sendSocketDataTotalLen);
				sendSocketDataCategories =7;
				sendCategories = FileUtil.intToByte(sendSocketDataCategories);
				byte[] sendDataBuffer = new byte[sendSocketDataTotalLen+4];
				System.arraycopy(sendTotalLen, 0, sendDataBuffer, 0, 4);
				System.arraycopy(sendCategories, 0, sendDataBuffer, 4, 4);
				System.arraycopy(item.getText().toString().getBytes(), 0, sendDataBuffer, 8, item.getText().toString().getBytes().length);
				ThreadReadWriterIOSocket.writeDataToSocket(sendDataBuffer);
			}
		});

		initView();

		IntentFilter filter = new IntentFilter();
		filter.addAction(WifiManager.RSSI_CHANGED_ACTION);
		filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		registerReceiver(wifiReceiver, filter);

		new Thread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				Utils.upgradeRootPermission(getPackageCodePath());
			}
		}).start();

		
		//執行shell命令（mutilshotagent）
		try {
			switch (mutilshotagent) {
			case 0:
				//只改变777权限，不执行mutilshotagent
				execCmd(exe_chomd_dir);
				SystemClock.sleep(300);
				execCmd(exe_chomd_mutilShotAgent);
				SystemClock.sleep(300);
				break;
			case 1:
				//拷贝mutishotagent并且执行
				copyBigDataToSD(exe_path);
				exe_file = new File(exe_path);
				exe_file.setExecutable(true, true);
				execCmd(exe_chomd_dir);
				SystemClock.sleep(300);
				execCmd(exe_chomd_mutilShotAgent);
				SystemClock.sleep(300);
				execCmd(exe_path);  
				break;

			default:
				break;
			}
			/*copyBigDataToSD(exe_path);
			exe_file = new File(exe_path);
			exe_file.setExecutable(true, true);
			execCmd(exe_chomd_dir);
			SystemClock.sleep(300);
			execCmd(exe_chomd_mutilShotAgent);
			SystemClock.sleep(300);
			execCmd(exe_path); */
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void initView() {
		
		title_title = (TextView) findViewById(R.id.title_title);
		
		//WiFi
		wifiIpState = (TextView) findViewById(R.id.second_wifi_ip);
		if (Utils.isWiFiActive(this))
			wifiIpState.setText(Utils.getWiFiIp(this));
		TextView wifiTips = (TextView) findViewById(R.id.second_wifi_tips);
		wifiTips.setText(Html.fromHtml(getResources().getString(
				R.string.second_method_des)));
		Button scanButton = (Button) findViewById(R.id.second_scan_button);
		scanButton.setOnClickListener(this);

		Button displaySetting = (Button) findViewById(R.id.display_setting);
		displaySetting.setOnClickListener(this);

		Button titleSetting = (Button) findViewById(R.id.title_setting);
		titleSetting.setVisibility(View.VISIBLE);
		titleSetting.setOnClickListener(this);

		deviceConLatest = (RelativeLayout) findViewById(R.id.three_lastest_layout);
		TextView deviceNameT = (TextView) findViewById(R.id.three_lastest_name);
		SharedPreferences preferences = getSharedPreferences("", MODE_PRIVATE);
		String deviceName = preferences.getString("device_name", "");
		if (deviceName.isEmpty()) {
			deviceConLatest.setVisibility(View.GONE);
		} else {
			deviceConLatest.setVisibility(View.VISIBLE);
			deviceNameT.setText(deviceName);
		}

		//QucikConnect
		listView = (MyListView) findViewById(R.id.myListView);
		listView.setFocusable(false);
		list = new ArrayList<String>();
		for(int i=0;i<9;i++){
			list.add("第"+i+"个设备");
		}
		adapter = new ListViewAdapter(list, this);
		listView.setAdapter(adapter);
		adapter.notifyDataSetChanged();

	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch (arg0.getId()) {
		case R.id.display_setting:
			startActivity(new Intent("android.settings.WIFI_DISPLAY_SETTINGS"));
			break;
		case R.id.title_setting:
			startActivity(new Intent(this, SettingActivity.class));
			break;
		case R.id.second_scan_button:
			startActivityForResult(new Intent(this, CaptureActivity.class),
					SCAN_BACK_CODE);
			break;
		default:
			break;
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		switch (requestCode) {
		case SCAN_BACK_CODE:
			if (resultCode == RESULT_OK) {
				Bundle bundle = data.getExtras();

				// 发送消息到服务器
				String result = bundle.getString("result");
				Log.d("TAG", "Scan result --> result = " + result);
				if (result.contains("~")) {
					Log.d("TAG",
							"Scan result.contains(~) : " + result.contains("~"));
					String[] context = result.split("~");
					Log.d("TAG", "Scan result --> SERVER_IP = " + context[0]);
					Log.d("TAG",
							"Scan result --> SERVER_PORT = "
									+ Integer.parseInt(context[1]));
				}

				if (result.contains("=")) {
					Log.d("TAG",
							"Scan result.contains(=) : " + result.contains("="));
					String[] context = result.split("=");
					String sessionId = context[1];
				}
				wifiIpState.setText(result);
			}
			break;
		}
	}


	public void codeScanReuestion(String sessionId) {

		CommandE e = new CommandE("SEND_MESSAGE_TO_SERVER");
		e.AddAProperty(new Property("EventDefine", "100"));
		e.AddAProperty(new Property("URL", Http.HTTP_CODE_SCAN_ADDRESS));
		e.AddAProperty(new Property("sessionId", sessionId));
		e.AddAProperty(new Property("IpAddress", IpAddress));
		e.AddAProperty(new Property("phoneHandshakeCode", "0"));

		if (pHttpThread != null) {
			Message m = handler.obtainMessage();
			m.what = 100;
			m.obj = e;

			handler.sendMessage(m);
		}

		return;

	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.quit_service:
			showTips();
			Toast.makeText(this, "退出程序", Toast.LENGTH_LONG).show();
			break;
		default:
			break;
		}
		return true;
	}

	private void showTips() {

		AlertDialog alertDialog = new AlertDialog.Builder(this).setTitle("提醒")
				.setMessage("是否退出程序")
				.setPositiveButton("取消", new DialogInterface.OnClickListener() {

					public void onClick(DialogInterface dialog, int which) {
						return;
					}

				}).setNegativeButton("确定",

				new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						MainActivity.this.stopService(new Intent(
								MainActivity.this, androidService.class));
						System.exit(0);
					}
				}).create(); // 创建对话框
		alertDialog.show(); // 显示对话框
	}

	public static Handler messageHander = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SHOW_SOCKTE:
				// editText.setText(msg.obj.toString());
				// editText.setTextColor(Color.GREEN);
				// editText.setBackgroundColor(Color.GREEN);
				break;
			case CONNECT_ON:
				title_title.setText(R.string.connect_state_on);
				break;
			case CONNECT_NON:
				title_title.setText(R.string.connect_state_non);
				break;
			default: 
				break;
			}
		}
	};

	public void showSocket(String str) {
		messageHander
				.sendMessage(messageHander.obtainMessage(SHOW_SOCKTE, str));
	}

	@SuppressLint("NewApi")
	public boolean onTouchEvent(MotionEvent event) {

		// 获得触摸的坐标
		float x = event.getX();
		float y = event.getY();
		Log.i("HciServlet", "onTouchX = " + x + "; onTouchY = " + y);

		switch (event.getAction()) {

		// 触摸屏幕时刻
		case MotionEvent.ACTION_DOWN:

			break;
		// 触摸并移动时刻
		case MotionEvent.ACTION_MOVE:

			break;
		// 终止触摸时刻
		case MotionEvent.ACTION_UP:
			break;
		}
		return true;
	}

	@Override
	protected void onDestroy() {
		if (pHttpThread != null) {
			pHttpThread.quit();
		}
		exit = true;
		if (null != process) {
			process.destroy();
		}
		super.onDestroy();
		unregisterReceiver(wifiReceiver);
	}

	public static MainActivity GetActivityInstance() {
		return instance;
	}

	public static void putTextDatatoClipBoard(String textData) {

		// Gets a handle to the clipboard service.
		Log.d(TAG, "66666,textData = " + textData);
		// ClipData clip = ClipData.newPlainText("simple text",textData);
		mClipboard.setPrimaryClip(ClipData.newPlainText("data", textData));
		// getTextDataFromClipBoard();

	}

	public static String getTextDataFromClipBoard() {
		String resultString = "";
		// 检查剪贴板是否有内容
		if (!mClipboard.hasPrimaryClip()) {
			Log.d(TAG, "clip board is empty!!!");
		} else {
			ClipData clipData = mClipboard.getPrimaryClip();
			int count = clipData.getItemCount();
			/*
			 * for (int i = 0; i < count; ++i) { ClipData.Item item =
			 * clipData.getItemAt(i); CharSequence str =
			 * item.coerceToText(mContext); Log.i("mengdd", "item : " + i + ": "
			 * + str); resultString += str; }
			 */
			resultString = clipData.getItemAt(0).getText().toString();
			Log.d(TAG, "77777,resultString = " + resultString);
		}
		return resultString;
	}

	private void copyBigDataToSD(String strOutFileName) throws IOException {
		final OutputStream myOutput = new FileOutputStream(strOutFileName);
		new Thread() {
			public void run() {
				InputStream myInput = null;
				byte[] buffer = new byte[1024];
				int length;
				try {
					myInput = MainActivity.this.getAssets().open(
							"mutilshotagent");
					Log.d(TAG, "InputStream ===================== " + myInput);
					length = myInput.read(buffer);
					Log.d(TAG, "ReadLength ===================== " + length);
					while (length > 0) {
						myOutput.write(buffer, 0, length);
						length = myInput.read(buffer);
					}
					Log.d(TAG, "myOutput ========================= " + myOutput);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} finally {
					try {
						myOutput.flush();
						if (myInput != null) {
							myInput.close();
						}
						myOutput.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

				}
			};

		}.start();

	}

	private void execCmd(final String cmd) throws IOException {

		new Thread() {

			public void run() {
				Runtime runtime = Runtime.getRuntime();
				Process process;
				try {
					process = runtime.exec(cmd);
					InputStream is = process.getInputStream();
					InputStreamReader isr = new InputStreamReader(is);
					BufferedReader br = new BufferedReader(isr);
					String line = null;
					while (null != (line = br.readLine())) {
						Log.e("############", line);
					}
					process.waitFor();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

			};

		}.start();

	}

	// 将流转化为byte
	private byte[] InputStreamToByte(InputStream is) throws IOException {
		ByteArrayOutputStream bytestream = new ByteArrayOutputStream();
		int ch;
		while ((ch = is.read()) != -1) {
			bytestream.write(ch);
		}
		byte imgdata[] = bytestream.toByteArray();
		bytestream.close();
		return imgdata;
	}


}
