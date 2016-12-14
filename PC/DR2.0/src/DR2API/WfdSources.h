#pragma once 
#include "./Rstdef.h"
#include "./udptcp.h"
#include "./WfdStringFormat.h"


#define WIFI_TCP 1111
#define WIFI_UDP 1112
#define WIFI_DEALBUFF  1113
#define WIFI_VIDEODEC  1114
#define WIFI_AUDIODEC  1115
#define WIFI_UIBC  1116
#define WIFI_CHECKWIFI  1117
#define WIFI_VIDEODECDRAW  1118
#define WIFI_VIDEODECPUSH 1119
#define WIFI_HDCP 1120
#define WIFI_TCP_SEND 1121
#define WIFI_TCP_RECV 1122
#define WIFI_TCP_TIMEOUT 1123


#define MAXVFRAME 0x1000000
#define MAXBUFFERUSE  0x10000000

class CMyh264;


typedef void(__stdcall *pOnState )(HANDLE handle, HANDLE devHandle, const char* name, bool mobile, DeviceState state);
typedef void(__stdcall *pCallback) (char * buf,int nw ,int nh ,int  isBlack , int nbkSize);
typedef void(__stdcall *pClickCallback) (int clickmsg);


class CWifiCode :public Work_ThreadS
{
public:
	CWifiCode();
	~CWifiCode();
	
	void  SetWfdSink(_WFD_DISPLAY_SINK_NOTIFICATION*pNotification);
	void InitVideoFromatTable();
	int InitStartWifi(const char* name, pOnState onState, pCallback callback);
	void   InitStopWifi();
	ARCAPI_(BOOL) DoWork(WorkThreadEx * pWork, DWORD dwIndex);
	void doWorkTCP(WorkThreadEx * pWork);
	void doWorkTCPRecv(WorkThreadEx * pWork);
	void doWorkTCPSend(WorkThreadEx * pWork);
	void doWorkUDP(WorkThreadEx * pWork);
	void doWorkRecvBuff(WorkThreadEx * pWork,char * src, int nsize);
	void doWorkDealBuff(WorkThreadEx * pWork);
	void doWorkDecAudio(WorkThreadEx * pWork);
	void doWorkDecVideo(WorkThreadEx * pWork);
	void doWorkUIBC(WorkThreadEx * pWork);
	void doWorkHDCP(WorkThreadEx * pWork);
	void doWorkDecVideoDraw(WorkThreadEx * pWork);
	void doWorkCheckTimeout(WorkThreadEx * pWork);


	int getSetting();
	void setStartAVTime(unsigned long long lltime);
	void doWorkCheckWifi(WorkThreadEx * pWork);
	void doWorkDecVideoPush(WorkThreadEx * pWork);
	void MyDrawWnd(BOOL isrela);// 自己进行绘制
public:
	CWfdStringFormat m_CWfdStringFormat;
	HANDLE m_hand;
	_WFD_DISPLAY_SINK_NOTIFICATION m_pNotification;

	WorkThreadEx * m_threadTCP;
	WorkThreadEx * m_threadUDP;
	WorkThreadEx * m_threadDeadBuff;
	WorkThreadEx * m_threadV;
	WorkThreadEx * m_threadA;
	WorkThreadEx * m_threadUIBC;
	WorkThreadEx * m_threadCheckWifi;
	WorkThreadEx * m_threadVPush;
	WorkThreadEx * m_threadVD;

	WorkThreadEx * m_threadTimeout;

	pCallback m_callback;
	pOnState m_onState;
	char m_onStateIsConnected;
	pClickCallback m_clickCall;
	HWND m_hWndDraw;
	BOOL m_SetWndChanging;
	BOOL m_SetFullscreen;

	RECT m_Rect;

	SOCKET m_skTCP;
	SOCKET m_skUDP;

	//得到的设备的信息  mac  ip  port
	char m_macService[260];// = { 0 };
	char m_macClient[260];// = { 0 };
	char m_IpService[260];// = { 0 };
	char m_IpClient[260];//= { 0 };
	char m_phonename[260];// = { 0 };


	DWORD m_RemoteAddress;
	int m_getTcpPort;// 得到的tcp 端口 进行连接
	int m_getTcpPortUIBC;// 得到的UIBC tcp 端口 进行连接

	int m_setUdpPort; // 需要设置的UDP 端口 

	int m_UdpPortStart;
	int m_UdpPortEnd;


	char  m_sessionid[260];

	CMyh264 * m_myh264;
	BOOL m_isfullframe;

	HDC m_hdc;


	VideoFromatTable m_vft; //获取返回的 video数据的格式

	int m_SystemWidth;//屏幕分辨率大小
	int m_SystemHeight;




	CMyBufferArray m_rtpslist;
	HANDLE m_hrtps;


	CMyBufferArray m_vlist;
	HANDLE m_hv; 
	
	CMyBufferArray m_alist;
	HANDLE m_ha;

	HANDLE m_hvPush;

	CMyBufferArray m_vlistdraw;
	HANDLE m_hvDraw;
	int m_vlistdrawNum;

	
	int m_nTearDown; //断开连接
	
	int m_needToLostAudio; //audio 丢帧计数

	MyRTPINFO m_mr;

	class CMyBufferList * m_rtpdatas;
	//class CMyBufferList * m_pedatas;
	class CMyBufferList * m_vddatas;
	class CMyBufferList * m_addatas;

	class CMyBufferList * m_vDrawdatas;

	//unsigned long long m_audioPlaytime ;
	//unsigned long long m_startAudiotime;
	unsigned long long m_startAVT;
	unsigned long long m_startCurT;

	unsigned long long m_CurVT; //当前 video 时间戳
	unsigned long long m_CurAT;// 当前 Audio 时间戳


	char m_isNewAduio;
	char m_isNeedDump; // 是否进行 dump 文件   从setting读取


	HDC  m_mydcMemory;
	HBITMAP m_hbitmap;
	HBITMAP m_myoldBitmap;
	int m_sh;
	int m_sw;

	MyLK m_lklk;

	//UIBC_STRUCT  m_myuibc;


	char * m_pnData;

	RECT m_screenrect;

	int m_reci ;
	int m_sendj ;
	int m_isok ;
	int m_teardown ;

	int m_isAudoDecodeClose;

	int m_bStartIntime;

	LONGLONG m_udptimeout;
	int m_isQp;
	//CMyLog  * m_log;
	//void pushLog(LogType etype, const CHAR * FuntionName, ...);
};



