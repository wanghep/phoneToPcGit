#pragma  once
#include "windows.h"
#include "stdio.h"



extern int g_limits_Setting;

void MyOutput(const CHAR* ptzFormat, ...);

void MyOutputW( const TCHAR* ptzFormat, ...);




enum LogType{
	LOG_NULL = -1,
	LOG_ZERO = 0 ,
	LOG_ALL = 1,
	LOG_doWorkTCP = (1 << 1),
	LOG_doWorkUDP = (1 << 2),
	LOG_doWorkDecVideo = (1 << 3),
	LOG_doWorkDecAudio = (1 << 4),
	LOG_UDPPUSH = (1<<5) ,
	LOG_LOSTFRAME = (1 << 6),
	LOG_AVT = (1 << 7),

	LOG_DRAW = (1 << 8),
	LOG_UIBC = (1 << 9),

};

void  LogOut(LogType etype, const CHAR * FuntionName, ...);


extern LogType g_strlevel ;