package com.CommandParser;

import java.util.ArrayList;


//
public class CommandE
{
	
	
	String Cmd;
	
	ArrayList<Property> PropertyList;
	
	//String Packet;
	
	public CommandE()
	{
		PropertyList = new ArrayList<Property>();
	}
	
	public CommandE( String Command )
	{
		SetCommand( Command );
		PropertyList = new ArrayList<Property>();
	}
	
	public CommandE( String Command , String P_Name1 , String P_Context1)
	{
		SetCommand( Command );
		PropertyList = new ArrayList<Property>();
		
		Property	CmdProperty = new Property(P_Name1,P_Context1);
		PropertyList.add(CmdProperty);
	}
	
	public CommandE( String Command , String P_Name1 , String P_Context1 ,  String P_Name2 , String P_Context2 )
	{
		SetCommand( Command );
		PropertyList = new ArrayList<Property>();
		
		Property	CmdProperty = new Property(P_Name1,P_Context1);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name2,P_Context2);
		PropertyList.add(CmdProperty);
	}
	
	public CommandE( String Command , String P_Name1 , String P_Context1 ,  String P_Name2 , String P_Context2,String P_Name3 , String P_Context3 )
	{
		SetCommand( Command );
		PropertyList = new ArrayList<Property>();
		
		Property	CmdProperty = new Property(P_Name1,P_Context1);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name2,P_Context2);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name3,P_Context3);
		PropertyList.add(CmdProperty);
	}
	
	public CommandE( String Command , String P_Name1 , String P_Context1 ,  String P_Name2 , String P_Context2,String P_Name3 , String P_Context3 ,String P_Name4 , String P_Context4 )
	{
		SetCommand( Command );
		PropertyList = new ArrayList<Property>();
		
		Property	CmdProperty = new Property(P_Name1,P_Context1);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name2,P_Context2);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name3,P_Context3);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name4,P_Context4);
		PropertyList.add(CmdProperty);
	}
	
	public CommandE( String Command , String P_Name1 , String P_Context1 ,  String P_Name2 , String P_Context2,String P_Name3 , String P_Context3 ,String P_Name4 , String P_Context4,String P_Name5 , String P_Context5 )
	{
		SetCommand( Command );
		PropertyList = new ArrayList<Property>();
		
		Property	CmdProperty = new Property(P_Name1,P_Context1);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name2,P_Context2);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name3,P_Context3);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name4,P_Context4);
		PropertyList.add(CmdProperty);
		
		CmdProperty = new Property(P_Name5,P_Context5);
		PropertyList.add(CmdProperty);
	}
	
	public void SetCommand(String Command )
	{
		Cmd = Command;
	}

	
	public String GetCommand()
	{
		return Cmd;
	}
	
	public int GetPropertyNum()
	{
		return PropertyList.size();
	}
	
	public Property GetProperty(int index )
	{
		if( index < PropertyList.size())
		{
			return PropertyList.get(index);
		}
		else
		{
			return null;
		}
	}
	public Property GetProperty( String ProtertyName)
	{
		for( int i = 0 ;i < PropertyList.size() ; i++ )
		{
			 if( PropertyList.get( i ).GetPropertyName().equals(ProtertyName) )
			 {
				 return PropertyList.get( i );				 
			 }
			
		}
		
		return null;
	}
	public String GetPropertyContext(String ProtertyName )
	{
		for( int i = 0 ;i < PropertyList.size() ; i++ )
		{
			 if( PropertyList.get( i ).GetPropertyName().equals(ProtertyName) )
			 {
				 return PropertyList.get( i ).GetPropertyContext();				 
			 }
			
		}
		
		return null;
		
	}
	
	public void AddAProperty( Property CmdProperty)
	{
		PropertyList.add(CmdProperty);
	}
	
	public void AddAProperty( int index ,Property CmdProperty)
	{
		if( PropertyList.size() > index )
		{
			PropertyList.set(index, CmdProperty );
		}
		else
		{
			PropertyList.add(CmdProperty);
		}
	}
	
	public void CopyPropertysTo( CommandE to )
	{
		for( int i = 0 ;i < PropertyList.size() ; i++ )
		{
			to.AddAProperty( new Property( this.GetProperty(i).PropertyName ,this.GetProperty(i).Context  ) );
		}
	}


	
}

