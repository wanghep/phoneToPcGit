package com.example.internet;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

public class TransMessage extends AsyncTask<Integer, Integer, String>{
	
	//private final String remoteIP = "192.168.1.104";
	private final int PORT = 1818;
	private Socket sSocket = null;
	private String message = null;
	private String mresult = null;
	
	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public TransMessage(String printInfo){
		Log.e("DEBUG", "创建Socket");
		Log.e("DEBUG", "printInfo：" + printInfo);
		
		setMessage(printInfo);
		//new Thread(runnable).start();
		//sendMess(printInfo);
	}
	/*
	public void sendMess(String mess){
		Log.e("DEBUG", "开始传输");
		
		if(MainActivity.ServerIP_string != null){
			try{
				Log.e("ServerIP+++++",MainActivity.ServerIP_string.toString());
				sSocket = new Socket(MainActivity.ServerIP_string, PORT);
				BufferedWriter writer=new BufferedWriter(new OutputStreamWriter(sSocket.getOutputStream()));
		    	writer.write(mess);
		    	writer.flush();
		    	writer.close();
		    	sSocket.close();
			}
			catch(Throwable e)
			{
				mresult = "网络错误:"+e.getMessage().toString();
			}
		}
		else{
			mresult = "找不到wifi打印服务器";
		}
		
	}
	public String getResult()
	{
		return this.mresult;
	}

	@Override
	protected String doInBackground(Integer... params) {
		// TODO Auto-generated method stub
		Log.e("doInBackground", "start");
		sendMess(this.getMessage());
		Log.e("doInBackground", "end");
		
		return null;
	}
	@Override
	protected void onPostExecute(String result) {
		super.onPostExecute(result);
		if(mresult!=null)
		{
			Toast.makeText(MainActivity.mcontext, mresult,
				     Toast.LENGTH_SHORT).show();
		}
	}
	
*/

	@Override
	protected String doInBackground(Integer... params) {
		// TODO Auto-generated method stub
		return null;
	}
}
