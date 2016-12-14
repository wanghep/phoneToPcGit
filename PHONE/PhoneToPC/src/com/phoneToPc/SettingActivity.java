package com.phoneToPc;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.provider.Settings;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

public class SettingActivity extends Activity implements OnClickListener {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_setting);
		
		initView();
	}
	
	public void initView() {
		ImageView back = (ImageView) findViewById(R.id.title_back);
		back.setVisibility(View.VISIBLE);
		back.setOnClickListener(this);
		
		TextView title = (TextView) findViewById(R.id.title_title);
		title.setText(R.string.action_settings);
		
		Button messageSet = (Button) findViewById(R.id.message_setting_set);
		messageSet.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.title_back:
			finish();
			break;
		case R.id.message_setting_set:
			startActivity(new Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS));
			break;
		default:
			break;
		}
	}
}

