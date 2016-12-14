package com.phoneToPc;

import java.io.DataOutputStream;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

public class Utils {
	/**
	 * 当前是否是wifi连接
	 * @param inContext
	 * @return
	 */
	public static boolean isWiFiActive(Context inContext) {
		Context context = inContext.getApplicationContext();
		ConnectivityManager connectivity = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity != null) {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getTypeName().equals("WIFI")
							&& info[i].isConnected()) {
						return true;
					}
				}
			}
		}
		return false;
	}
	/**
	 * 获取wifi连接状态的IP
	 * @param context
	 * @return
	 */
	public static String getWiFiIp(Context context) {
		WifiManager wifimanage = (WifiManager) context
				.getSystemService(Context.WIFI_SERVICE);// 获取WifiManager

		WifiInfo wifiinfo = wifimanage.getConnectionInfo();

		int i = wifiinfo.getIpAddress(); // "."
		return (i & 0xFF) + "." + ((i >> 8) & 0xFF) + "." + ((i >> 16) & 0xFF)
				+ "." + (i >> 24 & 0xFF);

	}
	/**
	 * 获取root权限
	 * @param pkgCodePath
	 * @return
	 */
	public static boolean upgradeRootPermission(String pkgCodePath) {  
	    Process process = null;  
	    DataOutputStream os = null;  
	    try {  
	        String cmd="chmod 777 " + pkgCodePath;  
	        process = Runtime.getRuntime().exec("su"); //切换到root帐号  
	        os = new DataOutputStream(process.getOutputStream());  
	        os.writeBytes(cmd + "\n");  
	        os.writeBytes("exit\n");  
	        os.flush();  
	        process.waitFor();  
	    } catch (Exception e) {  
	        return false;  
	    } finally {  
	        try {  
	            if (os != null) {  
	                os.close();  
	            }  
	            process.destroy();  
	        } catch (Exception e) {  
	        }  
	    }  
	    return true;  
	}
}

