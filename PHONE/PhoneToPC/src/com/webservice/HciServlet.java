package com.webservice;

import java.io.IOException;
import java.util.Map;
import java.util.Set;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Instrumentation;
import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;
import android.view.MotionEvent;

import com.phoneToPc.util;

@SuppressLint("NewApi")
public class HciServlet extends HttpServlet {
	
	private int downtime = 2;
	private Handler mhandler = null;
	private Instrumentation instrumentation;
	public HciServlet()
	{
		instrumentation = new Instrumentation();
	}
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		//super.doGet(request, response);
		Map map = request.getParameterMap();  
        Set<String> keySet = map.keySet();  
        String[] keyCodeValues = null;
        String[] actionSwitchValues = null;
        String[] mouseXValues = null;
        String[] mouseYValues = null;
        
        for (String key : keySet) {  
        	//Log.i( this.getClass().getSimpleName().toString(), "key= "+ key + " and value= " + map.get(key));
        	if(key.equals("keyCode")){
        		keyCodeValues = (String[]) map.get(key);  
        	}
        	if(key.equals("keyAction")){
        		actionSwitchValues = (String[]) map.get(key);  
        	}
        	if(key.equals("mouseX")){
        		mouseXValues = (String[]) map.get(key);  
        	}
        	if(key.equals("mouseY")){
        		mouseYValues = (String[]) map.get(key);  
        	}
        }  
        if( keyCodeValues != null && actionSwitchValues != null ){
	        Log.i( this.getClass().getSimpleName().toString(), "keyCode_PC = "+ Integer.valueOf(keyCodeValues[0]) + "; actionSwitch = " +Integer.valueOf(actionSwitchValues[0]));
	        messageDispatchKeyEvent(  util.pcKeyCodeToPhoneKeyCode(Integer.valueOf(keyCodeValues[0])),Integer.valueOf(actionSwitchValues[0]));
        }
        if( mouseXValues != null && mouseXValues != null ){
        	//Log.i( this.getClass().getSimpleName().toString(), "mouseX = "+ Integer.valueOf(mouseXValues[0]) + "; mouseY = " +Integer.valueOf(mouseYValues[0]));
        	messageDispatchMouseEvent(Integer.valueOf(mouseXValues[0]),Integer.valueOf(mouseYValues[0]));
        }
		response.setContentType("application/json");
		response.setHeader("Access-Control-Allow-Origin", "*");
		response.setStatus(HttpServletResponse.SC_OK);
		response.setCharacterEncoding("UTF-8");
		JSONObject json = new JSONObject();
		try {
			json.put("status", "0");
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		response.getWriter().write(json.toString());
		
		/*response.setContentType("image/jpeg");
		response.setHeader("Access-Control-Allow-Origin", "*");
		response.setStatus(HttpServletResponse.SC_OK);
		response.setCharacterEncoding("UTF-8");
		sendImage(response.getOutputStream());*/
	}

	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doGet(request, response);
	}
	
	@SuppressLint("NewApi")
	public void messageDispatchKeyEvent( int keyCode , int actionSwtich )
	{
		// actionSwtich == 1   KeyEvent.ACTION_DOWN
		if(actionSwtich == 1){
			
			instrumentation.sendKeyDownUpSync(keyCode);
		// actionSwtich == 0   KeyEvent.ACTION_UP	
		}else{
			
			//instrumentation.sendCharacterSync(keyCode);
		}
		
		
		
		/*final int  keyCodeFinal = keyCode;
		// 必需在线程中运行,否者报错  
		new Thread(new Runnable() {

			@Override
			public void run() {
				instrumentation.sendKeyDownUpSync(keyCodeFinal);
			}
		}).start();*/
		/*
		long curentTime = SystemClock.uptimeMillis();
		
		// actionSwtich == 1   KeyEvent.ACTION_DOWN
		if(actionSwtich == 1){ 
			Log.i( this.getClass().getSimpleName().toString(), "keyCodeName : "+KeyEvent.keyCodeToString(keyCode)+"  keyCode_PHONE = " + keyCode );
			androidService.mKeyManager.inputKeyevent(2, curentTime-100, KeyEvent.ACTION_DOWN, keyCode, 0, 0, 0);
		
		// actionSwtich == 0   KeyEvent.ACTION_UP	
		}else{
			Log.i( this.getClass().getSimpleName().toString(), "keyCodeName : "+KeyEvent.keyCodeToString(keyCode)+"  keyCode_PHONE = " + keyCode );
			androidService.mKeyManager.inputKeyevent(2, curentTime, KeyEvent.ACTION_UP, keyCode, 0, 0, 0);
		}
		*/
	}
	
	public void messageDispatchMouseEvent( int x , int y )
	{
		
		instrumentation.sendPointerSync(MotionEvent.obtain(SystemClock.uptimeMillis(),SystemClock.uptimeMillis(), MotionEvent.ACTION_DOWN, x, y, 0));
		instrumentation.sendPointerSync(MotionEvent.obtain(SystemClock.uptimeMillis(),SystemClock.uptimeMillis(), MotionEvent.ACTION_UP, x, y, 0));
		/*final int  X = x;
		final int  Y = y;
		// 必需在线程中运行,否者报错  
		new Thread(new Runnable() {

			@Override
			public void run() {
				instrumentation.sendPointerSync(MotionEvent.obtain(SystemClock.uptimeMillis(),SystemClock.uptimeMillis(), MotionEvent.ACTION_DOWN, X, Y, 0));
				instrumentation.sendPointerSync(MotionEvent.obtain(SystemClock.uptimeMillis(),SystemClock.uptimeMillis(), MotionEvent.ACTION_UP, X, Y, 0));
			}
		}).start();*/
		/*
		androidService.mKeyManager.clickTouch(x, y, false);
		Log.i( this.getClass().getSimpleName().toString(), "mouseClickX = "+ x + "; mouseClickY = " + y);
		*/
	}
	
	/*private void sendImage(OutputStream out) {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		Bitmap bitmap = screenShots(me);
		bitmap.compress(Bitmap.CompressFormat.PNG, 100, baos);
		try {

			out.write(baos.toByteArray());

		} catch (IOException e) {

			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}*/
}
