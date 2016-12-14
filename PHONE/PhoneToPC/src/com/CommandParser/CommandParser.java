package com.CommandParser;


public class CommandParser {

	
	//coding rule example 
	// Command:propert1#value1:propert2#value2:propert3#value3
	static public String EncodeA_Cmd( CommandE ExternOrder )
	{
		if( ExternOrder != null )
		{
			String CodingStr;
			
			CodingStr = ExternOrder.GetCommand();
			
			int PropertyNum = ExternOrder.GetPropertyNum();

			//CodingStr += ":" + ExternOrder.GetPacket();
			for( int i = 0 ; i < PropertyNum ; i++ )
			{
				Property CmdProperty = ExternOrder.GetProperty(i);
				
				CodingStr += ":" ;
				
				CodingStr += CmdProperty.GetPropertyName();
				CodingStr += "#";
				CodingStr += CmdProperty.GetPropertyContext();
				
			}
			
			return CodingStr;
		}
		else
		{
			assert(false);
		}
		
		return null;
	}
	
	static public CommandE DecodeA_Cmd( String EncodedData )
	{
		CommandE cmd = new CommandE();
		
		String[]  PropertyArray = EncodedData.split(":");
		
		//the first string is command
		assert( PropertyArray.length > 0 );
		cmd.SetCommand(PropertyArray[0] );

              //the second string is packet
		//assert( PropertyArray.length > 1 );
	    //    cmd.SetPacket(PropertyArray[1] );
		   
		//the followed string is property
		for( int i = 1 ; i < PropertyArray.length; i++ )
		{
			String[] PropertyPara = PropertyArray[i].split("#");
			
			assert(PropertyPara.length == 2 );
			
			Property NewProperty = new Property();
			
			NewProperty.SetProperytName( PropertyPara[0] );
			NewProperty.SetPropertyContext( PropertyPara[1] );
			cmd.AddAProperty(NewProperty );
		}
		
		return cmd;
	}
	
}
