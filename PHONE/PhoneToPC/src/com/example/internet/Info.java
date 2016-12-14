package com.example.internet;


import java.util.ArrayList;
import java.util.List;


public class Info {
	private List<String> numList = new ArrayList<String>();
	public String strInfo = null;
	public Info(String num, String sum){
		//ªÒ»°List
		strInfo = num;
	}
	
	public String info2String(String type) {
		if(strInfo != null)
		{
			return strInfo;
		}else
			return null;
	}
}
