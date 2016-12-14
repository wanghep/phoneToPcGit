package com.phoneToPc;

import android.accessibilityservice.AccessibilityService;
import android.util.Log;
import android.view.accessibility.AccessibilityEvent;

public class ThisAccessibilityService extends AccessibilityService{

	private static final String TAG = ThisAccessibilityService.class.getSimpleName();
	
	@Override
	protected void onServiceConnected() {
		// TODO Auto-generated method stub
		super.onServiceConnected();
		Log.i(TAG, "onServiceConnected");
	}
	
	@Override
	public void onAccessibilityEvent(AccessibilityEvent event) {
		// TODO Auto-generated method stub
		final int eventType = event.getEventType();
	    String eventText = null;
	    switch(eventType) {
	        case AccessibilityEvent.TYPE_VIEW_CLICKED:
	            eventText = "Focused: ";
	            break;
	        case AccessibilityEvent.TYPE_VIEW_FOCUSED:
	            eventText = "Focused: ";
	            break;
	        case AccessibilityEvent.TYPE_TOUCH_EXPLORATION_GESTURE_START:
	        	eventText = "Touch:============";
	        	break;
	    }
	    eventText = eventText + event.getContentDescription();
	    
	    Log.i("Accessibility", "eventText:"+eventText);
	}

	@Override
	public void onInterrupt() {
		// TODO Auto-generated method stub
		
	}

}

