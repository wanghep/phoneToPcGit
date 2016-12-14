package com.CommandParser;


public class Property 
{
	/**
	 * 
	 */
	String PropertyName;
	String Context;
	
	public Property()
	{
		
	}
	public Property( String P_Name , String P_Cotext )
	{
		PropertyName = P_Name;
		Context = P_Cotext;
	}
	public void SetProperytName(String Name )
	{
		PropertyName = Name;
	}
	public String GetPropertyName()
	{
		return PropertyName;		
	}
	
	public void SetPropertyContext(String Text )
	{
		Context = Text;
		
	}
	
	public String GetPropertyContext()
	{
		return Context;		
	}
	
}

