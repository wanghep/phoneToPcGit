package com.alpha.softkeyboard;

import android.util.Log;

public class AlphaIME {

	public static AlphaIME instance = null;
	private SoftKeyboard mService;
	private static final String TAG = "AlphaIME";

	public AlphaIME() {

	}

	public static AlphaIME getInstance() {
		if (instance == null) {
			Log.e(TAG,"AlphaIME getInstance() instance == NULL");
			synchronized (SoftKeyboard.class) {
				if (instance == null) {
					instance = new AlphaIME();
				}
			}
		}else{
			Log.e(TAG,"AlphaIME getInstance() instance != NULL");
		}
		return instance;
	}

	public void setService(SoftKeyboard listener) {
		mService = listener;
	}

	// refer to other interface
	public void sendKeyEvent(int keyEventCode) {
		if (mService == null) {
			Log.e(TAG, "mSoftKeyboard object error!");
			return;
		}
		if (!mService.getInputFlag()) {
			return;
		}
		mService.alphaKeyDownUp(keyEventCode);
		// mSoftKeyboard.sendDownUpKeyEvents(keyEventCode);
	}

}
