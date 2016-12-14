package com.tmac.adapter;

import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.phoneToPc.R;

public class ListViewAdapter extends MyAdapter{

	private List<String> list;
	private Context context;
	
	public ListViewAdapter(List<String> list, Context context) {
		// TODO Auto-generated constructor stub
		super(list, context);
	}
	



	@Override
	public View getView(final int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder holder;
		if(convertView == null){
			holder = new ViewHolder();
			convertView = LayoutInflater.from(mContext).inflate(R.layout.listview_item, null);
			holder.tv_name = (TextView) convertView.findViewById(R.id.tv_quick_name);
			holder.tv_way = (TextView) convertView.findViewById(R.id.tv_quick_way);
			convertView.setTag(holder);
		}else{
			holder = (ViewHolder) convertView.getTag();
		}
		holder.tv_name.setText(mList.get(position)+"");
		//QuickConnect Item Click 
		holder.tv_name.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Toast.makeText(mContext, "点击了第"+position+"项", Toast.LENGTH_SHORT).show();
				mContext.startActivity(new Intent("android.settings.WIFI_DISPLAY_SETTINGS"));
				
			}
		});
		
		return convertView;
	}
	
	public class ViewHolder{
		
		private TextView tv_name;
		private TextView tv_way;
		
		public ViewHolder(){
			
		}
		
	}


}
