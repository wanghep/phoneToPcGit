package com.webservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.phoneToPc.MainActivity;

public class BootReceiver extends BroadcastReceiver {  
    @Override  
    public void onReceive(Context context, Intent intent) {  
        if(intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {     // boot  
        	Intent activity = new Intent(context, MainActivity.class);  
        	activity.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);  
            context.startActivity(activity);  
            Intent service  = new Intent(context, androidService.class); 
            context.startService(service);  
            Log.d("TAG","开机自动启动！");
        }  
    }  
}  