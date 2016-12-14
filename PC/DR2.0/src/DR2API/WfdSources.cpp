#include "stdafx.h"
#include "../../inc/wfdsink.h"
#include "WfdSources.h"
#include "./udptcp.h"


#include "Rstdef.h"
//nclude "MyClient.h
//nclude "MyService.h 


#include "Wlanapi.h"
#pragma comment(lib,"Wlanapi.lib")

//#include "CMyLog.h"
#include "timeapi.h"

#include "ShlObj.h"
//#include "afxwin.h"

int g_nohp = 0;

#define MAXBSIZE  1024 

HANDLE g_hand = 0;
int g_sneedidr = 0;
LONGLONG MyGetCurTime();
int g_step = 0;
int g_ClickToDisconnect = 0;


extern LogType g_strlevel;
extern int g_logoutput;
extern char g_logoutputpath[260];

extern int g_isneedResize;

int  g_SendTearDownTOPhone = 0;

void MyKeyClickSend();

int g_isnewStartConnect = 0;

//CMyLog * g_plog = NULL;

extern int g_hasDisconnected;












void GetMacAdress(char * info, DOT11_MAC_ADDRESS & pmac)
{
	//char outinfo[256];
	sprintf(info, "%x,%x,%x,%x,%x,%x", pmac[0], pmac[1], pmac[2], pmac[3], pmac[4], pmac[5]);
	//printf(outinfo);

	LogOut(LOG_ALL, "%s", info);
	//OutputDebugString(info);
	//OutputDebugString("\n");
}

void GetIpAdress(char * info, SOCKADDR_STORAGE & pip)
{
	//char outinfo[256];
	unsigned char * pd = (unsigned char *)(pip.__ss_pad1);
	sprintf(info, "%d.%d.%d.%d", pd[2], pd[3], pd[4], pd[5]);
	LogOut(LOG_ALL, "%s", info);
	//printf(outinfo);
	//OutputDebugString(info);
	//OutputDebugString("\n");
}

VOID WINAPI WCheckWifi(PWLAN_NOTIFICATION_DATA x, PVOID y)
{
	LogOut(LOG_ALL, "NotificationSource=%d ", x->NotificationSource);
	//LogOut(LOG_ALL,"  NotificationCode=%d ", x->NotificationCode);
	//LogOut(LOG_ALL,"dwDataSize=%d", x->dwDataSize);
	//LogOut(LOG_ALL,"InterfaceGuid=%f", x->InterfaceGuid);
	switch (x->NotificationSource)
	{
	case  WLAN_NOTIFICATION_SOURCE_NONE:
	{
										   LogOut(LOG_ALL, "WLAN_NOTIFICATION_SOURCE_NONE\n");
										   break;
	}
	case WLAN_NOTIFICATION_SOURCE_ONEX:
	{
										  switch (x->NotificationCode)
										  {
										  case 0:LogOut(LOG_ALL, "OneXPublicNotificationBase\n");
											  break;
										  case 1:LogOut(LOG_ALL, "OneXNotificationTypeResultUpdate\n");
											  break;
										  case 2:LogOut(LOG_ALL, "OneXNotificationTypeAuthRestarted\n");
											  break;
										  case 3:LogOut(LOG_ALL, "OneXNotificationTypeEventInvalid\n");
											  break;
										  case 4:
										  default:LogOut(LOG_ALL, "OneXNumNotifications\n");
											  break;
										  }
										  break;
	}
	case WLAN_NOTIFICATION_SOURCE_ACM:
	{
										 switch (x->NotificationCode)
										 {
										 case 0:LogOut(LOG_ALL, "wlan_notification_acm_start \n");
											 break;
										 case 1:LogOut(LOG_ALL, "wlan_notification_acm_autoconf_enabled \n");
											 break;
										 case 2:LogOut(LOG_ALL, "wlan_notification_acm_autoconf_disabled \n");
											 break;
										 case 3:LogOut(LOG_ALL, "wlan_notification_acm_background_scan_enabled \n");
											 break;
										 case 4:LogOut(LOG_ALL, "wlan_notification_acm_background_scan_disabled \n");
											 break;
										 case 5:LogOut(LOG_ALL, "wlan_notification_acm_bss_type_change \n");
											 break;
										 case 6:LogOut(LOG_ALL, "wlan_notification_acm_power_setting_change \n");
											 break;
										 case 7:LogOut(LOG_ALL, "wlan_notification_acm_scan_complete \n");
											 break;
										 case 8:LogOut(LOG_ALL, "wlan_notification_acm_scan_fail \n");
											 break;
										 case 9:LogOut(LOG_ALL, "wlan_notification_acm_connection_start \n");
											 break;
										 case 10:LogOut(LOG_ALL, "wlan_notification_acm_connection_complete\n");
											 break;
										 case 11:LogOut(LOG_ALL, "wlan_notification_acm_connection_attempt_fail \n");
											 break;
										 case 12:LogOut(LOG_ALL, "wlan_notification_acm_filter_list_change \n");
											 break;
										 case 13:LogOut(LOG_ALL, "wlan_notification_acm_interface_arrival \n");
											 break;
										 case 14:LogOut(LOG_ALL, "wlan_notification_acm_interface_removal \n");
											 break;
										 case 15:LogOut(LOG_ALL, "wlan_notification_acm_profile_change \n");
											 break;
										 case 16:LogOut(LOG_ALL, "wlan_notification_acm_profile_name_change \n");
											 break;
										 case 17:LogOut(LOG_ALL, "wlan_notification_acm_profiles_exhausted \n");
											 break;
										 case 18:LogOut(LOG_ALL, "wlan_notification_acm_network_not_available \n");
											 break;
										 case 19:LogOut(LOG_ALL, "wlan_notification_acm_network_available \n");
											 break;
										 case 20:LogOut(LOG_ALL, "wlan_notification_acm_disconnecting \n");
											 break;
										 case 21:LogOut(LOG_ALL, "wlan_notification_acm_disconnected\n");
											 break;
										 case 22:LogOut(LOG_ALL, "wlan_notification_acm_adhoc_network_state_change\n");
											 break;
										 case 23:LogOut(LOG_ALL, "wlan_notification_acm_profile_unblocked\n");
											 break;
										 case 24:LogOut(LOG_ALL, "wlan_notification_acm_screen_power_change\n");
											 break;
										 case 25:LogOut(LOG_ALL, "wlan_notification_acm_profile_blocked\n");
											 break;
										 case 26:LogOut(LOG_ALL, "wlan_notification_acm_scan_list_refresh\n");
											 break;
										 default:LogOut(LOG_ALL, "wlan_notification_acm_end\n");
											 break;
										 }
										 break;
	}

	case WLAN_NOTIFICATION_SOURCE_MSM:
	{
										 switch (x->NotificationCode)
										 {
										 case 0:LogOut(LOG_ALL, "wlan_notification_msm_start  \n");
											 break;
										 case 1:LogOut(LOG_ALL, "wlan_notification_msm_associating \n");
											 break;
										 case 2:LogOut(LOG_ALL, "wlan_notification_msm_associated \n");
											 break;
										 case 3:LogOut(LOG_ALL, "wlan_notification_msm_authenticating \n");
											 break;
										 case 4:LogOut(LOG_ALL, "wlan_notification_msm_connected \n");
											 break;
										 case 5:LogOut(LOG_ALL, "wlan_notification_msm_roaming_start \n");
											 break;
										 case 6:LogOut(LOG_ALL, "wlan_notification_msm_roaming_end \n");
											 break;
										 case 7:LogOut(LOG_ALL, "wlan_notification_msm_radio_state_change \n");
											 break;
										 case 8:LogOut(LOG_ALL, "wlan_notification_msm_signal_quality_change \n");
											 break;
										 case 9:LogOut(LOG_ALL, "wlan_notification_msm_disassociating \n");
											 break;
										 case 10:LogOut(LOG_ALL, "wlan_notification_msm_disconnected\n");
											 break;
										 case 11:LogOut(LOG_ALL, "wlan_notification_msm_peer_join \n");
											 break;
										 case 12:LogOut(LOG_ALL, "wlan_notification_msm_peer_leave \n");
											 break;
										 case 13:LogOut(LOG_ALL, "wlan_notification_msm_adapter_removal \n");
											 break;
										 case 14:LogOut(LOG_ALL, "wlan_notification_msm_adapter_operation_mode_change \n");
											 break;
										 default:LogOut(LOG_ALL, "wlan_notification_msm_end \n");
											 break;
										 }
										 break;
	}
	case WLAN_NOTIFICATION_SOURCE_SECURITY:
	{
											  LogOut(LOG_ALL, "WLAN_NOTIFICATION_SOURCE_SECURITY\n");
											  break;
	}
	case WLAN_NOTIFICATION_SOURCE_IHV:
	{
										 LogOut(LOG_ALL, "WLAN_NOTIFICATION_SOURCE_IHV\n");
										 break;
	}
	case WLAN_NOTIFICATION_SOURCE_HNWK:
	{
										  switch (x->NotificationCode)
										  {
										  case 0:LogOut(LOG_ALL, "wlan_hosted_network_state_change\n");
											  break;
										  case 1:LogOut(LOG_ALL, "wlan_hosted_network_peer_state_change\n");
											  break;
										  default:LogOut(LOG_ALL, "wlan_hosted_network_radio_state_change\n");
											  break;
										  }
										  break;
	}
	case WLAN_NOTIFICATION_SOURCE_ALL:
	{
										 LogOut(LOG_ALL, "WLAN_NOTIFICATION_SOURCE_ALL\n");
										 break;
	}
	}

}


void printfDebugString(LogType etype, char *  outbuf, char * sstring)
{
	LogOut(etype, "----------%s----------\n", sstring);
	LogOut(etype, "\r\n\r\n%s", outbuf);
	LogOut(etype, "---------- ----------   ----------\n");
}



CWifiCode::CWifiCode()
{
	m_isQp = 0;
	m_udptimeout = 0;
	m_bStartIntime = 0;;

	m_threadTimeout = NULL;
	m_isAudoDecodeClose = 0;
	m_reci = 0;
	m_sendj = 0;
	m_isok = 0;
	m_teardown = 0;


	m_pnData = 0;
	m_onStateIsConnected = 0;
	m_mydcMemory = 0;
	m_hbitmap = 0;
	m_myoldBitmap = 0;
	m_sw = 0;
	m_sh = 0;


	m_skUDP = -1;
	m_skTCP = -1;
	memset(&m_pNotification, 0, sizeof(m_pNotification));

	m_UdpPortStart = 1234;
	m_UdpPortEnd = m_UdpPortStart + 100;
	m_setUdpPort = 0;

	m_macService[0] = 0;
	m_macClient[0] = 0;
	m_IpService[0] = 0;
	m_IpClient[0] = 0;
	m_phonename[0] = 0;
	m_getTcpPort = 0;
	m_RemoteAddress = 0;

	m_myh264 = new CMyh264;
	if (m_myh264)
		m_myh264->Init264Dll();

	m_hand = NULL;
	m_threadTCP = NULL;
	m_threadUDP = NULL;
	m_threadDeadBuff = NULL;
	m_threadA = NULL;
	m_threadV = NULL;

	m_threadUIBC = NULL;

	m_threadCheckWifi = NULL;
	m_threadVD = NULL;

	m_hWndDraw = NULL;
	m_Rect = { 0 };
	m_hdc = NULL;

	m_isfullframe = FALSE;

	m_SystemWidth = GetSystemMetrics(SM_CXSCREEN);
	m_SystemHeight = GetSystemMetrics(SM_CYSCREEN);

	m_rtpdatas = new CMyBufferList(0x10000, 100);
	m_vddatas = new CMyBufferList(MAXVFRAME, 10);
	m_addatas = new CMyBufferList(0x10000, 100);

	m_vDrawdatas = new CMyBufferList(MAXVFRAME, 5);


	m_hrtps = CreateEvent(NULL, 0, 0, NULL);
	m_hv = CreateEvent(NULL, 0, 0, NULL);
	m_ha = CreateEvent(NULL, 0, 0, NULL);

	m_callback = NULL;
	m_onState = NULL;
	m_nTearDown = -1;

	m_startAVT = -1;
	m_startCurT = -1;

	m_isNeedDump = 0;

	m_CurVT = 0;
	m_CurAT = 0;

	m_isNewAduio = 0;

	m_vlistdrawNum = 0;
	m_SetWndChanging = FALSE;;
	m_SetFullscreen = FALSE;

	getSetting();

	m_pnData = new char[MAXBUFFERUSE];//需要进行释放
	memset(m_pnData, 0, MAXBUFFERUSE);

	memset(&m_screenrect, 0, sizeof(m_screenrect));

	m_vft.nw = 1280;
	m_vft.nh = 1024;

	//	m_log = new CMyLog();

	// 	if (g_plog == NULL)
	// 	{
	// 		g_plog = new CMyLog();
	// 		g_plog->StartLog();
	// 	}


	//m_vft.nw = 1920;
	//m_vft.nh = 1080;
}


CWifiCode::~CWifiCode()
{
	api_stopworkex(m_threadTCP, WIFI_TCP, 10);
	api_stopworkex(m_threadUDP, WIFI_UDP, 10);
	api_stopworkex(m_threadDeadBuff, WIFI_DEALBUFF, 10);
	api_stopworkex(m_threadV, WIFI_VIDEODEC, 10);
	api_stopworkex(m_threadA, WIFI_AUDIODEC, 10);
	api_stopworkex(m_threadCheckWifi, WIFI_CHECKWIFI, 10);
	api_stopworkex(m_threadVPush, WIFI_VIDEODECPUSH, 10);
	api_stopworkex(m_threadVD, WIFI_VIDEODECDRAW, 10);

	api_freeworkex(m_threadTCP);
	api_freeworkex(m_threadUDP);
	api_freeworkex(m_threadDeadBuff);
	api_freeworkex(m_threadV);
	api_freeworkex(m_threadA);
	api_freeworkex(m_threadCheckWifi);
	api_freeworkex(m_threadVPush);
	api_freeworkex(m_threadVD);

	SafeDeletes(m_pnData);


	//releaseLogData();

	if (m_myh264)
		delete m_myh264;
	if (g_hand){
		WFDDisplaySinkCloseSession(g_hand);
		g_hand = 0;
	}

	WFDDisplaySinkStop();
}




int CWifiCode::getSetting()
{
	char spath[260] = { 0 };
	Api_GetMyIniPath(spath);

	m_isNeedDump = GetPrivateProfileIntA("main", "isdump", 0, spath);
	g_nohp = GetPrivateProfileIntA("main", "nohp", 0, spath);

	//int defaultn = 0xffff;// -(1 << 3) - (1 << 4) - (1 << 5);
	int defaultn = 0xffff - (1 << 5) - (1 << 4) - (1 << 8) - (1 << 9);

	int nlevel = GetPrivateProfileIntA("main", "loglevel", defaultn, spath);
	g_strlevel = (LogType)nlevel;

	g_logoutput = GetPrivateProfileIntA("main", "logoutput", 0, spath);

	char sspath[260] = { 0 };

// 	CHAR	cModulePath[MAX_PATH] = { 0 };
// 	CHAR	cDrive[MAX_PATH] = { 0 };
// 	CHAR	cDir[MAX_PATH] = { 0 };
// 	CHAR	cED_coder[MAX_PATH] = { 0 };
// 	CHAR	cED_coderd[MAX_PATH] = { 0 };
// 	CHAR	cED_aacd[MAX_PATH] = { 0 };
// 
// 	GetModuleFileNameA(0, cModulePath, MAX_PATH);
// 	_splitpath(cModulePath, cDrive, cDir, NULL, NULL);
// 	_makepath(cModulePath, cDrive, cDir, NULL, NULL);
// 	_makepath(cED_coder, cDrive, cDir, "dump\\", NULL);

	GetPrivateProfileStringA("main", "logoutputpath", "", g_logoutputpath, 260, spath);

	g_isneedResize = GetPrivateProfileIntA("main", "isneedResize", 0, spath);


	// 把 输出 信息写入 C:\Users\KT\AppData\Local\ScreenCast
	char cLocalAppDir[260];
	char  cAppDir[260];
	WIN32_FIND_DATA wfd;
	::SHGetSpecialFolderPathA(NULL, cLocalAppDir, CSIDL_LOCAL_APPDATA, false);
	sprintf(cAppDir, "%s\\%s", cLocalAppDir, "ScreenCast");

	if (INVALID_HANDLE_VALUE == ::FindFirstFileA(cAppDir, &wfd)){
		BOOL isCreateSuc = ::CreateDirectoryA(cAppDir, NULL);
		if (isCreateSuc)
			sprintf(g_logoutputpath, "%s\\", cAppDir);
	}
	else 
		sprintf(g_logoutputpath, "%s\\", cAppDir);


	return 0;

}




///////////////////////////////////////////// class DealDataCode 


void  CWifiCode::SetWfdSink(_WFD_DISPLAY_SINK_NOTIFICATION*pNotification)
{
	if (pNotification){
		m_pNotification = *pNotification;
		char macService[260] = { 0 };
		char macClient[260] = { 0 };
		char IpService[260] = { 0 };
		char IpClient[260] = { 0 };
		int  pPort = 0;
		GetMacAdress(macService, pNotification->RemoteDeviceAddress);
		GetIpAdress(IpService, pNotification->ConnectCompleteInfo.RemoteAddress);
		GetMacAdress(macClient, pNotification->ConnectCompleteInfo.GroupID.DeviceAddress);
		GetIpAdress(IpClient, pNotification->ConnectCompleteInfo.LocalAddress);
		pPort = pNotification->ConnectCompleteInfo.uRTSPPort;

		sprintf(m_macService, "%s", macService); //远程mac地址
		sprintf(m_macClient, "%s", macClient); //本地mac地址
		sprintf(m_IpService, "%s", IpService); // 远程ip地址
		sprintf(m_IpClient, "%s", IpClient); // 本地ip地址
		m_getTcpPort = pPort; //tcp 连接 端口
		m_RemoteAddress = inet_addr(IpService);

		//m_hand = pNotification->ConnectCompleteInfo.hSessionHandle;
	}
	api_stopworkex(m_threadTCP, WIFI_TCP, 10);
	api_freeworkex(m_threadTCP);

	//pNotification->ConnectCompleteInfo.guidSessionInterface
	//pNotification->ConnectCompleteInfo.strProfile;
	//WFDDisplaySinkSetPersistedGroupIDList(1, &(pNotification->ConnectCompleteInfo.GroupID));


	if (m_threadTCP == NULL)
		m_threadTCP = WorkInThread(WIFI_TCP, NULL, 0, NULL, 0);
	//return m_pNotification;
}



int  myRecvMsg(
	SOCKET s,
	char * buf,
	int len,
	int flags)
{
	return recv(s, buf, len, 0);
}

int  mySendMsg(
	SOCKET s,
	const char  * buf,
	int len,
	int flags)
{
	return send(s, buf, len, 0);
}










char* returnNextString(char* buf, char* packetbuf)
{
	char* prn = NULL;
	char* pLen = NULL;
	int LengthNum = 0;
	prn = strstr(buf, "\r\n\r\n");
	if (prn)
	{
		int nlen1 = strlen(buf);
		int nlen2 = strlen(prn);
		int nlenOnePack = nlen1 - nlen2 + 4;

		memcpy(packetbuf, buf, nlenOnePack);

		char * prn2 = strstr(packetbuf, "Content-Length");
		if (prn2 != NULL)
		{
			int nlength = 0;
			sscanf(prn2, "Content-Length: %d", &nlength);
			memcpy(packetbuf, buf, nlenOnePack + nlength);
			return prn + 4 + nlength;
		}
		return prn + 4;
	}
	return NULL;
}


void lib_doWorkTCP(void *pthis ,
	SOCKET sk,
	VideoFromatTable  * vft,
	CWfdStringFormat *  psf,
	WorkThreadEx * pWork,
	char * recvbuf,
	char * sendbuf,
	char * sessionid,
	int &isok,
	int &reci,
	int &sendj,
	int &teardown,
	int & nTearDown,
	int & sneedidr
	)
{
	CWifiCode *  pWifi = (CWifiCode*)pthis;

	const char * RTSPM1 = "OPTIONS * RTSP/1.0";
	const char * RTSPM2 = "RTSP/1.0 200 OK";
	const char * RTSPM2_EX0 = "Public: org.wfa.wfd1.0";
	const char * RTSPM2_EX1 = "Transport: RTP/AVP/UDP";
	const char * RTSPM3 = "GET_PARAMETER";
	const char * RTSPM4 = "SET_PARAMETER";
	const char * RTSPM5 = "SET_PARAMETER";
	const char * RTSPM6 = "SETUP rtsp://";
	const char * RTSPM7 = "PLAY rtsp://";
	const char * RTSPM8 = "";
	const char * RTSPM9 = "";
	const char * RTSPM10 = "";


	int nfirst = 0;

	int isplaystart = 0;


	while (isok == 1 && sk != INVALID_SOCKET && api_isneedwork(pWork, 0))
	{


		static int cishu = 0;
		cishu++;

		if ((isplaystart > 0 && isplaystart < 6) || (sneedidr && cishu > 200))
		{
			Sleep(1000);
			isplaystart++;
			LogOut(LOG_ALL, "send idr times = %d", isplaystart);

			cishu = 0;
			sneedidr = 0;

			if (isok){//M13
				isok = 0;
				psf->SinkToSource13(sendbuf, sendj);
				sendj++;
				long nlensend = strlen(sendbuf);
				if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0)){
					printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send13");
					isok = 1;
				}
			}
		}


		if (nTearDown == 1 || g_SendTearDownTOPhone == 1)
		{

			if (isok){//M13
				isok = 0;
				psf->SinkToSource8(sendbuf, sendj);
				sendj++;
				long nlensend = strlen(sendbuf);
				if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0)){
					printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send8");

				}
			}
			nTearDown = 0;
			g_SendTearDownTOPhone = 0;
		}




		memset(recvbuf, 0, 4096);
		int rsize = API_RecDataInTime(sk, recvbuf, MAX_BUFF_LEN, 100, NULL, NULL);
		if (rsize > 0)
		{
			recvbuf[rsize] = 0;

			char * sbuff = recvbuf;

			while (sbuff != NULL)//sbuff[0])
			{
				char OnePack[4096] = { 0 };
				sbuff = returnNextString(sbuff, OnePack);

				if (strncmp(OnePack, RTSPM1, strlen(RTSPM1)) == 0)
				{
					isok = 0;
					printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv1");
					char * pCSeq = strstr(OnePack, "CSeq:");
					if (pCSeq)
					{
						sscanf(pCSeq, "CSeq: %d", &reci);
						if (reci >= 0)
						{
							psf->SinkToSource1(sendbuf, reci);//return m1
							reci++;
							long nlensend = strlen(sendbuf);
							if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
							{
								printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send1");
								isok = 1;
							}
						}

						if (isok)
						{//M2
							isok = 0;
							psf->SinkToSource2(sendbuf, sendj);
							sendj++;
							long nlensend = strlen(sendbuf);
							if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
							{
								printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send2");
								isok = 1;
							}
						}
					}
				}
				else if (strncmp(OnePack, RTSPM2, strlen(RTSPM2)) == 0)
				{

					if (strstr(OnePack, RTSPM2_EX1) != NULL)
					{
						isok = 0;

						printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv6");
						char * nstep = strstr(recvbuf, "Session: ");
						if (nstep){
							sscanf(nstep, "Session: %s;", sessionid);
							sprintf(psf->m_sessionid, "%s", sessionid);

							nstep = strchr(sessionid, ';');
							if (nstep)
								nstep[0] = 0;
							isok = 1;


							if (isok)
							{
								isok = 0;
								psf->SinkToSource7(sendbuf, sendj);
								sendj++;
								long nlensend = strlen(sendbuf);
								if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0)){
									printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send7");
									isok = 1;
									isplaystart = 1;
								}
							}
						}
					}
					else
					{
						printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv  RTSP OK");
					}
				}
				else if (strncmp(OnePack, RTSPM3, strlen(RTSPM3)) == 0)
				{
					printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv3");

					char * pCSeq = strstr(OnePack, "CSeq:");
					if (pCSeq)
					{
						int reciget = 0;
						sscanf(pCSeq, "CSeq: %d", &reciget);
						if (reciget == reci)
						{
							psf->SinkToSource3(sendbuf, reci, recvbuf);//return m1
							reci++;
							long nlensend = strlen(sendbuf);
							if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
							{
								printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send3");
								isok = 1;
							}
						}
					}
					char * pIntel = strstr(OnePack, "intel_");
					char * pMicrosoft = strstr(OnePack, "microsoft_");
					if (pMicrosoft || pIntel)
					{
						// PC 链接过来的数据  给上层数据 判断是 全屏
						LogOut(LOG_ALL, "this is PC connect to PC");
						pWifi->m_isQp = 1;
					}
					else{
						pWifi->m_isQp = 0;
						LogOut(LOG_ALL, "this is Phone connect to PC");
					}

				}
				else if (strncmp(OnePack, RTSPM4, strlen(RTSPM4)) == 0)
				{
					char * sp = strstr(OnePack, "wfd_video_formats");
					if (sp)
					{
						printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv4");

						MyWfd_video_fromat wfd_video_format;
						sscanf(sp, "wfd_video_formats: %02x %02x %02x %02x %08x %08x %08x %02x %04x %04x %02x none none",
							&wfd_video_format.native,
							&wfd_video_format.preferred_display_mode_supported,
							&wfd_video_format.profile,
							&wfd_video_format.level,
							&wfd_video_format.CEA_Support,
							&wfd_video_format.VESA_Support,
							&wfd_video_format.HH_Support,
							&wfd_video_format.latency,
							&wfd_video_format.min_slice_size,
							&wfd_video_format.slice_enc_params,
							&wfd_video_format.frame_rate_control_support
							);
						int native = wfd_video_format.native;

						int nindex = 0;
						int tableid = 0;
						int ci = 0;

						if (nindex == 0)
						{
							nindex = wfd_video_format.CEA_Support;
							tableid = 0;
						}
						if (nindex == 0)
						{
							nindex = wfd_video_format.VESA_Support;
							tableid = 1;
						}

						if (nindex == 0)
						{
							nindex = wfd_video_format.HH_Support;
							tableid = 2;
						}

						while (nindex > 0)
						{
							nindex = (nindex >> 1);
							ci++;
						}
						if (ci >= 1)
							ci--;

						GetVideoFromtTable(vft, tableid, ci);
						LogOut(LOG_doWorkTCP, "index=%d,type=%d,nw=%d,nh=%d,frames=%d",
							vft->index,
							vft->type,
							vft->nw,
							vft->nh,
							vft->frames);

						char * pCSeq = strstr(OnePack, "CSeq:");
						if (pCSeq)
						{
							int reciget = 0;
							sscanf(pCSeq, "CSeq: %d", &reciget);
							if (reciget == reci)
							{
								psf->SinkToSource4(sendbuf, reci);//return m1
								reci++;
								long nlensend = strlen(sendbuf);
								if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
								{
									printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send4");
									isok = 1;
								}
							}
						}
					}
					else if (strstr(OnePack, "wfd_trigger_method: SETUP") != NULL)
					{
						printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv5");
						char * pCSeq = strstr(OnePack, "CSeq:");
						if (pCSeq){
							int reciget = 0;
							sscanf(pCSeq, "CSeq: %d", &reciget);
							if (reciget == reci){
								psf->SinkToSource5(sendbuf, reci);//return m1
								reci++;
								long nlensend = strlen(sendbuf);
								if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
								{
									printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send5");
									isok = 1;

									if (isok){//M6
										isok = 0;
										psf->SinkToSource6(sendbuf, sendj);
										sendj++;
										long nlensend = strlen(sendbuf);
										if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0)){
											printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send6");
											isok = 1;
										}
									}
								}
							}
						}
					}
					else if (strstr(OnePack, "wfd_trigger_method: TEARDOWN") != NULL)
					{
						printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv TEARDOWN");
						isok = 0;
						teardown = 1;
						//LogOut(LOG_ALL , "wfd_trigger_method: TEARDOWN")
					}
					else if (strstr(OnePack, "wfd_trigger_method: PLAY") != NULL)// 针对一些手机 锁屏之后 不能继续播放 ，需要发送 一个 SinkToSource7
					{
						psf->SinkToSource16(sendbuf, reci);
						reci++;
						long nlensend = strlen(sendbuf);
						if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
						{
							printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send16");
							isok = 1;
						}
						if (isok == 1)
						{
							psf->SinkToSource7(sendbuf, sendj);
							sendj++;
							nlensend = strlen(sendbuf);
							if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
							{
								printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "SinkToSource7");
								isok = 1;
							}
						}
					}

					else {
						printfDebugString(LOG_doWorkTCP, (char*)OnePack, "recv4_ex");
						char * pCSeq = strstr(OnePack, "CSeq:");
						if (pCSeq)
						{
							int reciget = 0;
							sscanf(pCSeq, "CSeq: %d", &reciget);
							if (reciget == reci)
							{
								psf->SinkToSource4(sendbuf, reci);//return m1
								reci++;
								long nlensend = strlen(sendbuf);
								if (nlensend == mySendMsg(sk, sendbuf, nlensend, 0))
								{
									printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send_ex");
									isok = 1;
								}
							}
						}
					}
				}
			}

		}
		else
		{
			if (rsize < 0)
			{
				LogOut(LOG_ALL, "rsize < 0");
				break;
			}
		}

		Sleep(2);
	}
}
//char* sendbuff(char* rebuf, int &teardown);


void CWifiCode::doWorkTCPRecv(WorkThreadEx * pWork)
{

}
void CWifiCode::doWorkTCPSend(WorkThreadEx * pWork)
{

}

void CWifiCode::doWorkTCP(WorkThreadEx * pWork)
{

	sprintf(m_CWfdStringFormat.m_IpService, "%s", m_IpService);

	m_needToLostAudio = 0;

	LogOut(LOG_doWorkTCP, "doWorkTCP  start");

	m_skTCP = INVALID_SOCKET;

	SOCKET  sk = INVALID_SOCKET;
	char * recvbuf = new char[MAX_BUFF_LEN];
	char * sendbuf = new char[MAX_BUFF_LEN];
	struct sockaddr_in saServer; //服务器地址信息
	saServer.sin_family = AF_INET; //地址家族
	saServer.sin_port = htons(m_getTcpPort); //注意转化为网络节序
	saServer.sin_addr.S_un.S_addr = m_RemoteAddress;


	m_reci = 0;
	m_sendj = 0;
	m_isok = 0;
	m_teardown = 0;

	sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 	int reci = 0;
	// 	int sendj = 0;
	// 	int isok = 0;
	// 	int teardown = 0;

	int connectTimes = 0;
	while (connectTimes < 15 && api_isneedwork(pWork, 0))
	{
		connectTimes++;
		Sleep(100);
		LogOut(LOG_ALL, "connect times = %d", connectTimes);

		if (sk != INVALID_SOCKET){
			if (SOCKET_ERROR == connect(sk, (struct sockaddr *)&saServer, sizeof(saServer))){
				closesocket(sk);
				sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				LogOut(LOG_ALL, "connect error");
			}
			else{
				m_skTCP = sk;
				m_isok = 1;
				break;
			}
		}
	}


	lib_doWorkTCP(this ,
		sk,
		&m_vft,
		&m_CWfdStringFormat,
		pWork,
		recvbuf,
		sendbuf,
		m_sessionid,
		m_isok,
		m_reci,
		m_sendj,
		m_teardown,
		m_nTearDown,
		g_sneedidr
		);



	// 	if (pf->m_skTCP != INVALID_SOCKET)
	// 		{
	// 			char sendbuf[4096] = { 0 };
	// 			char recvbuf[4096] = { 0 };
	// 	
	// 			m_CWfdStringFormat.SinkToSource8(sendbuf, m_sendj);
	// 			m_sendj++;
	// 			long nlensend = strlen(sendbuf);
	// 			if (nlensend == send(m_skTCP, sendbuf, nlensend, 0))
	// 			{
	// 				printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send8");
	// 				int rsize = recv(m_skTCP, recvbuf, MAX_BUFF_LEN, 0);
	// 				if (rsize > 0)
	// 				{
	// 					recvbuf[rsize] = 0;
	// 					printfDebugString(LOG_doWorkTCP, (char*)recvbuf, "recv8");
	// 				}
	// 			}
	// 		}



	LogOut(LOG_ALL, " tcpout one\n");

	if (sk != INVALID_SOCKET){
		closesocket(sk);
	}
	SafeDeletes(recvbuf);
	SafeDeletes(sendbuf);
	m_skTCP = sk;

	if (m_pNotification.ConnectCompleteInfo.hSessionHandle){
		if (g_hand != m_pNotification.ConnectCompleteInfo.hSessionHandle){
			WFDDisplaySinkCloseSession(m_pNotification.ConnectCompleteInfo.hSessionHandle);
			m_pNotification.ConnectCompleteInfo.hSessionHandle = 0;
		}
	}

	LogOut(LOG_ALL, " tcpout\n");


	InitStopWifi();

	if (m_teardown)
	{
		m_onState(NULL, NULL, m_phonename, 1, Disconnected);
	}
	else
	{
		m_onState(NULL, NULL, m_phonename, 1, MYDisconnected);
	}
	m_SetFullscreen = FALSE;


}

void  CWifiCode::doWorkUDP(WorkThreadEx * pWork)
{
	MyBufInfo * pitem = m_rtpdatas->GetBuffer1();
	m_skUDP = INVALID_SOCKET;
	m_setUdpPort = 0;
	int udpportget = 0;
	SOCKET  sk = INVALID_SOCKET;
	//CMyUdpData * pUdpData = new CMyUdpData;
	char * recvBuf = new char[MAX_BUFF_LEN];
	struct sockaddr_in saServer; //服务器地址信息
	saServer.sin_family = AF_INET; //地址家族
	saServer.sin_addr.S_un.S_addr = 0;// inet_addr(m_IpClient);
	LogOut(LOG_doWorkUDP, " udpin\n");





	while (api_isneedwork(pWork, 0))
	{

#ifdef TESTUDPWITHFILE
		if (1)
		{	
			//static FILE * fp = fopen("D:\\work\\WifiDirect\\bin\\Debug\\dump\\old\\dataAAC.ts", "rb");
			static FILE * fp = fopen("D:\\work\\WifiDirect\\bin\\Debug\\dump\\dataV.ts", "rb");
			//static FILE * fp = fopen("D:\\work\\WifiDirect\\bin\\Debug\\dump\\old\\dataVPC.ts", "rb");
			//static FILE * fp = fopen("D:\\work\\WifiDirect\\bin\\Debug\\dump\\old\\dataVPC2.ts", "rb");

			int nnn = 0;
			if (fp)
			{
				nnn++;
				if (nnn > 1)
				{
					nnn = 0;
					Sleep(25);
					//Sleep(10);
				}
				memset(pitem->m_pbdata, 0, 0x10000);
				int ns = fread(pitem->m_pbdata, 1, 188, fp);
				if (ns != 188)
				{
					fclose(fp);
					fp = 0;

					fp = fopen("D:\\work\\WifiDirect\\bin\\Debug\\dump\\old\\dataLPCM.ts", "rb");
				}
				else
				{
					pitem->m_dwLen = 188;
					m_rtpslist.PushTail(pitem);
					//pitem->Release();
					api_releasebuf(pitem);
					SetEvent(m_hrtps);
					//pitem = m_rtpdatas->GetBuffer();
					pitem = m_rtpdatas->GetBuffer1();
					LogOut(LOG_UDPPUSH, " udp rec outdata %d  m_rtpslist.num=%d \n", 188, m_rtpslist.GetNum());
				}
			}
			Sleep(2);
			continue;		
		}

#endif 


		if (sk == INVALID_SOCKET){
			int udpportlast = udpportget == 0 ? m_UdpPortStart : udpportget;
			udpportget = 0;
			sk = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (sk != INVALID_SOCKET){
				for (int udpport = udpportlast; udpport <= m_UdpPortEnd; udpport++){
					saServer.sin_port = htons(udpport); //注意转化为网络节序
					if (bind(sk, (struct sockaddr*)&saServer, sizeof(saServer)) == 0){
						udpportget = udpport;
						m_skUDP = sk;
						m_setUdpPort = udpportlast;
						m_CWfdStringFormat.m_setUdpPort = m_setUdpPort;

						break;
					}
				}
				if (udpportget == 0)
				{
					for (int udpport = 0; udpport < udpportlast; udpport++){
						saServer.sin_port = htons(udpport); //注意转化为网络节序
						if (bind(sk, (struct sockaddr*)&saServer, sizeof(saServer)) == 0){
							udpportget = udpport;
							break;
						}
					}
				}
				if (udpportget == 0){
					closesocket(sk);
					sk = INVALID_SOCKET;
					//logerr(udp can not create and bind  %d--%d, m_UdpPortStart, m_UdpPortEnd)
				}
			}
		}
		if (sk != INVALID_SOCKET){
			int rsize = API_RecDataInTime(sk, pitem->m_pbdata, m_rtpdatas->m_dwMaxBufferSize, 10, &saServer, NULL);
			//check addr is phone if (saServer.sin_addr.S_un.S_addr == pcli)
			//Sleep(2);
			if (1)
			{
				if (rsize > 12){

					m_udptimeout = MyGetCurTime();

					pitem->m_dwLen = rsize;
					m_rtpslist.PushTail(pitem);
					api_releasebuf(pitem);
					SetEvent(m_hrtps);
					pitem = m_rtpdatas->GetBuffer1();
					int nnum = m_rtpslist.GetNum();
					if (nnum > 1)
						LogOut(LOG_UDPPUSH, " udp rec outdata %d  m_rtpslist.num=%d \n", rsize, nnum);
				}
				else if (rsize < 0){
					closesocket(sk);
					sk = INVALID_SOCKET;
					//logerr(udp rec data error)
					LogOut(LOG_doWorkUDP, " udpout break  one\n");
				}
			}
		}
		if (sk == INVALID_SOCKET){
			m_setUdpPort = 0;
			LogOut(LOG_doWorkUDP, " udpout break\n");
			break;
		}
	}
	if (sk != INVALID_SOCKET){
		closesocket(sk);
	}
	m_skUDP = sk;


	LogOut(LOG_doWorkUDP, "udpout\n");

	if (m_skTCP != INVALID_SOCKET)
	{
		closesocket(m_skTCP);
		m_skTCP = INVALID_SOCKET;
		Sleep(100);
	}


	SafeDeletes(recvBuf);
	api_releasebuf(pitem);


}




void CWifiCode::doWorkRecvBuff(WorkThreadEx * pWork, char * recvBuf, int nsize)
{

}




void CWifiCode::doWorkDealBuff(WorkThreadEx * pWork)
{

	lib_doWorkDealBuff(
		pWork,
		&m_rtpslist,
		&m_alist,
		&m_vlist,
		&m_mr,
		m_vddatas,
		m_addatas,
		m_hrtps,
		m_hv,
		m_ha,
		m_startAVT,
		m_isNeedDump,
		g_nohp,
		g_sneedidr,
		g_isnewStartConnect
		);

}
void CWifiCode::doWorkDecVideo(WorkThreadEx * pWork)
{
	//m_vft.nw = 1920;
	//m_vft.nh = 1080;
	//m_vft.frames = 25;

	int nstep = 0;
	MyBufInfo * pitemrtps = 0;
	while (api_isneedwork(pWork, 0))
	{
		if (pitemrtps == 0)
			pitemrtps = m_vlist.PopHead();
		BOOL ret = 0;

		if (pitemrtps){
			//int listnum = m_vlist.GetNum();
			//if (listnum < 5)
			//	Sleep(30);
			//else
			//	LogOut(LOG_ALL, "m_vlist list num =%d", listnum);
			//if (m_DataVNum > 2)
			//	Sleep(30);
			if (m_myh264)
			{

				//setStartAVTime(pitemrtps->m_lltime);

				//判断ts流获取的流宽高是不是与RTSP取得的一样
				int nw = m_vft.nw;
				int nh = (((m_vft.nh + 15) >> 4) << 4);
				if (nw != m_mr.m_dww || nh != m_mr.m_dwh)
				{
					nw = m_mr.m_dww;
					nh = m_mr.m_dwh;
					m_vft.nw = nw;
					m_vft.nh = nh;

					LogOut(LOG_ALL, "SetDecodeWHF nw=%d ,nh=%d", nw,nh);
				}
				m_myh264->SetDecodeWHF(nw, nh, m_vft.frames);
				ret = m_myh264->PushInDataV((char*)(pitemrtps->m_pbdata), pitemrtps->m_dwLen, pitemrtps->m_lltime);
				SetEvent(m_hvPush);
				//m_DataVNum++;
				//if (listnum > 0)
				//	LogOut(LOG_ALL, "PushInDataV  end    num=%d\n", listnum);
			}

			if (ret != (0xC00D36B5L))
			{
				api_releasebuf(pitemrtps);
				pitemrtps = NULL;
			}
			else
				Sleep(1);
		}
		else
		{
			Sleep(1);
			WaitForSingleObject(m_hv, 1);
		}
	}
}


void CWifiCode::doWorkDecVideoPush(WorkThreadEx * pWork)
{

	int nstep = 0;
	while (api_isneedwork(pWork, 0))
	{





		//int listnum = m_vlist.GetNum();
		//HANDLE  hd = GetCurrentThread();
		//int thd = GetThreadId(hd);
#if 1
		if (m_myh264)
		{
			m_isfullframe = m_myh264->GetDecodeV();
			static int isfirstss = 0;
			while (m_isfullframe)
			{

				// 				static int iii = 0;
				// 				iii++;
				// 				char sss[260] = { 0 };
				// 				sprintf(sss, "iii=%d", iii);
				// 				OutputDebugStringA(sss);


				// push  到解码之后的list 
				if (isfirstss == 0)
				{
					isfirstss = 1;
					LogOut(LOG_ALL, "first frame is decode out");
				}
				if (1)
				{
					MyBufInfo * pitem = m_vDrawdatas->GetBuffer1();
					//pitem->Release();
					//memcpy(pitem->m_pbdata, m_mr.m_vp, m_mr.m_vmax);

					if (1){
						int nlen = m_myh264->m_nw * m_myh264->m_nh * 3 / 2;
						memcpy(pitem->m_pbdata, m_myh264->m_pout2, nlen);

						// 						static FILE * fp = fopen("d:\\test.yuv", "ab+");
						// 						static int nii = 0;
						// 						nii++;
						// 						if (fp)
						// 						{
						// 							fwrite(m_myh264->m_pout2, 1, nlen, fp);
						// 							if (nii > 4)
						// 							{
						// 								fclose(fp);
						// 								fp = fopen("d:\\test.yuv", "ab+");
						// 							}
						// 
						// 
						// 						}



						pitem->m_dwLen = nlen;
						//m_pconv->CCV_ColorChange((unsigned char*)m_pdata, m_nw * 4, 32, m_pout2, m_nw, 12, m_nw, m_nh, 1);
					}
					else{
						memcpy(pitem->m_pbdata, m_myh264->m_pdata, m_myh264->m_datasize);
						pitem->m_dwLen = m_myh264->m_datasize;
					}



					pitem->m_lltime = m_myh264->m_lltime2;//s MyGetCurTime();

					pitem->m_nw = m_myh264->m_nw;
					pitem->m_nh = m_myh264->m_nh;
					pitem->m_nisBlack = m_myh264->m_isback;
					pitem->m_nbkSize = m_myh264->m_isbackSize;

					int nlistnum = m_vlistdraw.GetNum();



					if (nlistnum < 30)
					{

					
						//m_vlistdrawNum++;
						m_vlistdraw.PushTail(pitem);

					
					}
					else {
						LogOut(LOG_ALL, "nlistnum=%d>6", nlistnum);
					}
					SetEvent(m_hvDraw);
					api_releasebuf(pitem);
				}


				m_isfullframe = m_myh264->GetDecodeV();
			}
		}

#endif
		if (m_isfullframe == 0)
		{
			Sleep(1);
			WaitForSingleObject(m_hvPush, 1);
		}

	}
}



#pragma region MyRegionD3DDraw



void CWifiCode::doWorkDecVideoDraw(WorkThreadEx * pWork)
{
	int nstep = 0;

	int sss = 0;
	int frames = 10000000 / m_vft.frames;
	while (api_isneedwork(pWork, 0))
	{
		if (m_isAudoDecodeClose == 1)
		{
			m_myh264->InitAadio();
			m_threadA = WorkInThread(WIFI_AUDIODEC, NULL, 0, NULL, 0);
			LogOut(LOG_ALL, "audio thread start new");
			m_isAudoDecodeClose = 0;
		}


		Sleep(1);
		if (m_vft.frames == 0)
			m_vft.frames = 30;
		int oneFrameTime = 10000000 / (m_vft.frames + 5); 


		static LONGLONG lasttime = 0;
		LONGLONG nowtime = MyGetCurTime();
		if (nowtime - lasttime > oneFrameTime){

		}
		else{
			continue;
		}
		int nlistnum = m_vlistdraw.GetNum();
		if (nlistnum > 0)
		{
			MyBufInfo * pitemrtps = (MyBufInfo*)(m_vlistdraw.GetHead());
			if (pitemrtps)
			{
				if (g_ClickToDisconnect == 0)
				{
					if (m_onStateIsConnected == 0)
					{
						m_onStateIsConnected = 1;
						m_onState(this, NULL, m_phonename, 1, Connected);
					}

					if (nlistnum > 3)
						LogOut(LOG_doWorkDecVideo, "list m_vlistdraw  num is = %d\n", nlistnum);

					m_CurVT = pitemrtps->m_lltime;
					int nw = pitemrtps->m_nw;
					int nh = pitemrtps->m_nh;
					int nisBlack = pitemrtps->m_nisBlack;
					int nbkSize = pitemrtps->m_nbkSize;
					api_releasebuf(pitemrtps);

					if (m_callback)
					{
						m_callback(NULL, nw, nh, nisBlack, nbkSize);
						static int sback = -1;
						if (sback != nisBlack){
							sback = nisBlack;
							Sleep(100);
						}
					}
					MyDrawWnd(TRUE);
					lasttime = MyGetCurTime();
					//isr = 1;
				}
				else {

					MyBufInfo * 	pitemrtps = (MyBufInfo*)(m_vlistdraw.PopHead());
					api_releasebuf(pitemrtps);
					LogOut(LOG_ALL, "after close list m_vlistdraw  num is = %d\n", nlistnum);
				}
			}
		}
		else
		{
			if (nowtime - lasttime > oneFrameTime)
			{
				MyDrawWnd(FALSE);
			}
		}
	}
}




void CWifiCode::MyDrawWnd(BOOL isrealdraw)
{
	int isneedsetrgn = 0;
	if (m_hWndDraw &&  m_SetWndChanging == 0)
	{

		static int sIsQP = -1;
		int  isQP = m_SetFullscreen;
		if (sIsQP != isQP)
		{
			sIsQP = isQP;
			isneedsetrgn = 1;
		}

		HWND hparent = NULL;
		RECT rt;
		{
			hparent = GetParent(GetParent(m_hWndDraw));
			if (hparent){
				GetClientRect(hparent, &rt);
				if (m_screenrect.bottom != rt.bottom || m_screenrect.right != rt.right)
				{
					m_screenrect.bottom = rt.bottom;
					m_screenrect.right = rt.right;
					isneedsetrgn = 1;
				}
			}
		}

		GetClientRect(m_hWndDraw, &rt);
		HDC dc = GetDC(m_hWndDraw);

		if (m_mydcMemory){
			if (m_sw != m_SystemWidth || m_SystemHeight != m_sh)
			{
				SelectObject(m_mydcMemory, m_myoldBitmap);
				DeleteObject(m_hbitmap);
				DeleteDC(m_mydcMemory);
				m_mydcMemory = 0; m_hbitmap = 0; m_myoldBitmap = 0;
				isneedsetrgn = 1;
			}
		}
		if (m_mydcMemory == 0){
			m_mydcMemory = CreateCompatibleDC(dc);
			m_hbitmap = CreateCompatibleBitmap(dc, m_SystemWidth, m_SystemHeight);
			m_myoldBitmap = (HBITMAP)(SelectObject(m_mydcMemory, m_hbitmap));
			m_sh = m_SystemHeight;
			m_sw = m_SystemWidth;
			SetStretchBltMode(m_mydcMemory, STRETCH_HALFTONE);
			::BitBlt(m_mydcMemory, 0, 0, m_sw, m_sh, 0, 0, 0, BLACKNESS);
			isneedsetrgn = 1;
		}

		static  RECT s_rt = { 0, 0, 0, 0 };
		if (s_rt.bottom != rt.bottom || s_rt.right != rt.right)
		{
			s_rt.bottom = rt.bottom;
			s_rt.right = rt.right;

			::BitBlt(m_mydcMemory, 0, 0, m_SystemWidth, m_SystemHeight, 0, 0, 0, BLACKNESS);

			isneedsetrgn = 1;
		}

		static MyBufInfo * s_pitemrtps = 0;
		MyBufInfo * pitemrtps = 0;
		if (isrealdraw)
			pitemrtps = (MyBufInfo*)(m_vlistdraw.PopHead());
		if (pitemrtps == 0){
			//if (1) // isneedsetrgn)
			if (s_pitemrtps)
			{
				pitemrtps = s_pitemrtps;
				if (pitemrtps)
					api_addrefbuf(pitemrtps);//pitemrtps->AddRef();
			}
			else {
				//
			}
		}
		else{
			
			if (s_pitemrtps)
				api_releasebuf(s_pitemrtps);
			s_pitemrtps = pitemrtps;
			api_addrefbuf(s_pitemrtps); 
		}


		if (pitemrtps)
		{
			if (g_hasDisconnected == 0)
				g_hasDisconnected = 2;
			g_step++;
			
			char  * pdata = (char*)pitemrtps->m_pbdata;// pwifi->m_myh264->m_pdata;

			int nw = m_vft.nw;
			int nh = m_vft.nh;

			int isBack = pitemrtps->m_nisBlack;
			int ndx = nw * 4;
			int nlen = ((nh*nh / nw) + 3) / 4 * 4;
			int nbs = (nw - nlen) / 2;
			int nBlackSize = pitemrtps->m_nbkSize;

			nBlackSize = (nBlackSize + 3) / 4 * 4;

			m_myh264->MyCreateP3d(m_hWndDraw);

			//pitemrtps->m_pbdata =(unsigned char * ) pdata;
			// 进行窗口 大小进行 判断
			if (isQP == -1)//最小化
			{
				//pwifi->m_myh264->m_isbackold = -1;
			}
			else if (isQP == 1) //全屏
			{
				RECT nRect = { 0 };
				SetNewWindowRect(&rt, &nRect, nw, nh);

				m_myh264->MyOnRefreshData(pitemrtps->m_pbdata, 12, pitemrtps->m_nw, pitemrtps->m_nw, pitemrtps->m_nh);
				
				SIZE si = { nRect.right - nRect.left, nRect.bottom - nRect.top };
				m_myh264->MyRenderData(m_hWndDraw, nRect.left, nRect.top, &si, 0, NULL);
		
			}
			else if (isQP == 0)// 窗口模式
			{
				RECT nn = { 0 };
				//int isBack = pwifi->m_myh264->m_isback;
				if (1 && isBack == 1 && nbs * 2 < nw)
				{

					if (m_pnData)
					{
						if (pdata)
							memset(m_pnData, 0, m_sw * m_sh * 4);
						char * pone = m_pnData;
						for (int i = 0; i < nh; i++)
						{
							char * psrc = pone;
							memcpy(psrc, pdata + nbs * 4, nlen * 4);
							pdata += nw * 4;
							pone += nlen * 4;
						}
					}

					RECT nRect = { 0 };
					SetNewWindowRect(&rt, &nRect, nlen, nh);

					RECT nSrc = { 0 };
					nSrc.left = nBlackSize;
					nSrc.top = 0;
					nSrc.bottom = nh;
					nSrc.right = nlen + nBlackSize;
					m_myh264->MyOnRefreshData(pitemrtps->m_pbdata, 12, pitemrtps->m_nw, pitemrtps->m_nw, pitemrtps->m_nh);
					SIZE si = { nRect.right - nRect.left, nRect.bottom - nRect.top };
					m_myh264->MyRenderData(m_hWndDraw, nRect.left, nRect.top, &si, 0, &nSrc);

				}
				else
				{
					RECT nRect = { 0 };
					SetNewWindowRect(&rt, &nRect, nw, nh);

					m_myh264->MyOnRefreshData(pitemrtps->m_pbdata, 12, pitemrtps->m_nw, pitemrtps->m_nw, pitemrtps->m_nh);
					SIZE si = { nRect.right - nRect.left, nRect.bottom - nRect.top };
					m_myh264->MyRenderData(m_hWndDraw, nRect.left, nRect.top, &si, 0, NULL);

				}
			}
			api_releasebuf(pitemrtps);
		}
		else{
			if (isQP == -1)//最小化
			{
			}
			else if (isQP == 1) //全屏
			{
			}
			else
			{
			}
		}
		::ReleaseDC(m_hWndDraw, dc);

		if (isneedsetrgn)
		{
			int nrt = -1;
			if (isQP != 1){
				HRGN hrgn = CreateRoundRectRgn(0, 0, m_screenrect.right, m_screenrect.bottom, 50, 50);
				nrt = ::SetWindowRgn(hparent, hrgn, TRUE);
				if (nrt == 0){
					Sleep(200);
				}
			}
			else{
				nrt = ::SetWindowRgn(hparent, 0, TRUE);
				//isneedrgn = 0;
			}
		}
	}
}
#pragma endregion



void CWifiCode::setStartAVTime(unsigned long long lltime)
{
	if (m_startAVT == -1)
	{
		m_startAVT = lltime;
		m_startCurT = MyGetCurTime();
		char strtime[260] = { 0 };
		sprintf(strtime, "A stime = %lld,curt=%lld\n", m_startAVT, m_startCurT);
		LogOut(LOG_ALL, strtime);
	}
}

void CWifiCode::doWorkDecAudio(WorkThreadEx * pWork)
{
	int nstep = 0;
	int nRetnum = 0;
	while (api_isneedwork(pWork, 0))
	{
		MyBufInfo * pitemrtps = (MyBufInfo*)(m_alist.PopHead());
		if (pitemrtps){
			if (m_myh264)
			{
				m_CurAT = pitemrtps->m_lltime;
				//setStartAVTime(pitemrtps->m_lltime);

				//m_myh264->SetDecodeA(m_vft.nw, m_vft.nh, m_vft.frames);
				int alistnum = m_alist.GetNum();
				if (alistnum > 0)
					LogOut(LOG_doWorkDecAudio, "doWorkDecAudio end listSize=%d\n", alistnum);

				LONGLONG  difvt = m_CurVT - m_CurAT;
				if (difvt > 200000)
				{
					m_needToLostAudio = 0;
					char strtime[260] = { 0 };
					sprintf(strtime, "ATime m_CurAT-m_CurVT=%lld  , m_CurAT=%lld,m_CurVT=%lld\n", m_CurAT - m_CurVT, m_CurAT, m_CurVT);
					LogOut(LOG_ALL, strtime);
				}
				else
				{
					//char strtime[260] = { 0 };
					//sprintf(strtime, "ATime m_CurAT-m_CurVT=%lld  , m_CurAT=%lld,m_CurVT=%lld\n", m_CurAT - m_CurVT, m_CurAT, m_CurVT);
					//LogOut(LOG_ALL, strtime);
				}

				m_isNewAduio = 0;
				int lostnum = 0;
				if (m_isNewAduio == 0)
					lostnum = 100;
				else
					lostnum = 50;

				m_needToLostAudio = 100;
				m_needToLostAudio++;
				if (m_needToLostAudio > lostnum)
				{
					m_isNewAduio = 1;
					m_needToLostAudio = lostnum;
					LONGLONG lld = ((LONGLONG)m_CurVT - (LONGLONG)m_CurAT) / 1000;
					BOOL ret = m_myh264->PushInDataA((char*)(pitemrtps->m_pbdata), pitemrtps->m_dwLen, pitemrtps->m_lltime, &lld, &(pitemrtps->m_wfe), pitemrtps->m_aid == AUDIOIDUSELPCM);
					if (ret == 0)
						nRetnum++;
					else
						nRetnum = 0;

					//LogOut(LOG_ALL, "ret=%d", ret);
					//char strtime[260] = { 0 };
					//sprintf(strtime, "ATime in  curat=%lld,curvt=%lld  diff=%lld", m_CurAT, m_CurVT, m_CurAT - m_CurVT);
				}
				else  {
					//char strtime[260] = { 0 };
					//unsigned long long fdd = m_CurVT - m_CurAT;
					//sprintf(strtime, "ATime m_needToLostAudio > 230 curat=%lld,curvt=%lld" , m_CurAT ,m_CurVT);
					//OutputDebugString(strtime);
				}

				if (nRetnum > 50)
				{
					nRetnum = 0;

					m_myh264->ExitAadio();

					api_stopworkex(m_threadA, WIFI_AUDIODEC, 10);
					api_freeworkex(m_threadA);
					LogOut(LOG_ALL, "audio decod error");

					m_isAudoDecodeClose = 1;
				}

			}
			api_releasebuf(pitemrtps);

			// audio 解码失败
			//	if (ret == FALSE)
			//	{
			//			api_stopworkex(m_threadA, WIFI_AUDIODEC, 10);
			//			api_freeworkex(m_threadA);
			//			LogOut(LOG_ALL, "audio decod error");
			//	}
		}
		else{
			Sleep(1);
			WaitForSingleObject(m_ha, 1);
		}
	}
}





void CWifiCode::doWorkCheckWifi(WorkThreadEx * pWork)
{

	//HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;      //    
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;


	DWORD dw = 0;
	DWORD dwv = 0;
	HANDLE hwlan = 0;
	DWORD da = 0;
	DWORD dwrt = WlanOpenHandle(2, NULL, &dwv, &hwlan);

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;

	PWLAN_AVAILABLE_NETWORK_LIST pAvailableNetworkList = NULL;
	DWORD ht = 0;
	dwrt = WlanEnumInterfaces(hwlan, NULL, &pIfList);
	if (pIfList == NULL)
	{

		LogOut(LOG_ALL, "WlanEnumInterfaces  error\n");
		return;
	}

	int i;
	for (i = 0; i < pIfList->dwNumberOfItems; i++)
	{
		da = WlanGetAvailableNetworkList(hwlan, &pIfList->InterfaceInfo[i].InterfaceGuid, 2, NULL, &pAvailableNetworkList);
		for (int j = 0; j < pAvailableNetworkList->dwNumberOfItems; j++)
		{
			LogOut(LOG_ALL, "%s\n", pAvailableNetworkList->Network[j].dot11Ssid.ucSSID);
		}

	}
	dw = WlanRegisterNotification(hwlan, WLAN_NOTIFICATION_SOURCE_ALL, TRUE, WCheckWifi, NULL, NULL, &ht);
	if (0){
		if (dw == ERROR_SUCCESS)
		{
			printf("ERROR_SUCCESS\n");
		}
		else
		{
			printf("error = %d \n", dw);
		}
	}
	//while (1){
	//	Sleep(5000);
	//}

	while (api_isneedwork(pWork, 0))
	{
		Sleep(100);
	}


	if (pAvailableNetworkList != NULL) {
		WlanFreeMemory(pAvailableNetworkList);
		pAvailableNetworkList = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

}


BOOL	CWifiCode::DoWork(WorkThreadEx * pWork, DWORD dwIndex)
{
	WorkThreadEx * pWorkEx = (WorkThreadEx *)pWork;
	if (dwIndex == WIFI_TCP)
	{
		//OutputDebugString("wifi do work");
		LogOut(LOG_ALL, "wifi do work");
		m_setUdpPort = 0;
		m_threadUDP = WorkInThread(WIFI_UDP, NULL, 0, NULL, 0);
		if (m_setUdpPort == 0)
			Sleep(10);

		//m_setUdpPort = 1234;
		//Sleep(10000);
		doWorkTCP(pWorkEx);
	}
	else if (dwIndex == WIFI_UDP)
	{
		//m_threadDeadBuff = WorkInThread(WIFI_DEALBUFF, NULL, 0, NULL, 0);
		doWorkUDP(pWorkEx);
	}
	else if (dwIndex == WIFI_DEALBUFF)
	{
		//doWorkRecvBuff();
		doWorkDealBuff(pWorkEx);
	}
	else if (dwIndex == WIFI_AUDIODEC){
		doWorkDecAudio(pWorkEx);
	}
	else if (dwIndex == WIFI_VIDEODEC){
		doWorkDecVideo(pWorkEx);
	}
	else if (dwIndex == WIFI_UIBC)
	{
		doWorkUIBC(pWorkEx);
	}
	else if (dwIndex == WIFI_CHECKWIFI)
	{
		doWorkCheckWifi(pWorkEx);
	}
	else if (dwIndex == WIFI_VIDEODECPUSH)
	{
		doWorkDecVideoPush(pWorkEx);
	}
	else if (dwIndex == WIFI_VIDEODECDRAW)
	{
		doWorkDecVideoDraw(pWorkEx);
	}
	else if (dwIndex == WIFI_HDCP)
	{
		doWorkHDCP(pWorkEx);
	}
	else if (dwIndex == WIFI_TCP_RECV)
	{
		doWorkTCPRecv(pWorkEx);
	}
	else if (dwIndex == WIFI_TCP_SEND)
	{
		doWorkTCPSend(pWorkEx);
	}
	else if (dwIndex == WIFI_TCP_TIMEOUT)
	{
		doWorkCheckTimeout(pWork);
	}


	return TRUE;
}



void outputmacadress(DOT11_MAC_ADDRESS & pmac){
	char outinfo[256];
	sprintf_s(outinfo, "\n{%x,%x,%x,%x,%x,%x}", pmac[0], pmac[1], pmac[2], pmac[3], pmac[4], pmac[5]);
	printf(outinfo);
	//OutputDebugString(outinfo);
	LogOut(LOG_ALL, outinfo);
}

void outputipadress(SOCKADDR_STORAGE & pip){
	char outinfo[256];
	unsigned char * pd = (unsigned char *)(pip.__ss_pad1);
	sprintf_s(outinfo, "\n%d.%d.%d.%d", pd[2], pd[3], pd[4], pd[5]);
	printf(outinfo);
	//OutputDebugString(outinfo);
	LogOut(LOG_ALL, outinfo);
}

int g_hasDisconnected = 0;
LONGLONG g_hasDisconnectedtime = 0;



void testprintfinfo(PVOID Context,
	PWFD_DISPLAY_SINK_NOTIFICATION pNotification,
	PWFD_DISPLAY_SINK_NOTIFICATION_RESULT pNotificationResult)

{

#if 0
#ifdef JJJJJ

	static int n = 0;
	n++;
	char fpath[260] = { 0 };
	sprintf(fpath, "d:\\%d.txt", n);

	FILE * fp = fopen(fpath, "wb");
	if (fp)
	{
		if (pNotification)
		{
			int nlen = sizeof(WFD_DISPLAY_SINK_NOTIFICATION);
			fwrite(pNotification, 1, nlen, fp);
		}
		if (pNotificationResult)
		{
			int nlen2 = sizeof(WFD_DISPLAY_SINK_NOTIFICATION_RESULT);
			fwrite(pNotificationResult, 1, nlen2, fp);
		}
		fclose(fp);
	}
#else 

	pNotification = new WFD_DISPLAY_SINK_NOTIFICATION;
	pNotificationResult = new WFD_DISPLAY_SINK_NOTIFICATION_RESULT;
	FILE * fp = fopen("d:\\2.txt", "rb");
	if (fp)
	{
		if (pNotification)
		{
			int nlen = sizeof(WFD_DISPLAY_SINK_NOTIFICATION);
			fread(pNotification, 1, nlen, fp);
		}
		if (pNotificationResult)
		{
			int nlen2 = sizeof(WFD_DISPLAY_SINK_NOTIFICATION_RESULT);
			fread(pNotificationResult, 1, nlen2, fp);
		}
		fclose(fp);
	}
#endif 

#endif 
	//char pbuff[260] = { 0 };
	//sprintf(pbuff, "%s , %s , %d", pNotification->Header.Type, pNotification->Header.Revision, pNotification->Header.Length);
	//LogOut(LOG_ALL, pbuff);



	LogOut(LOG_ALL, "Header.Type=%d", pNotification->Header.Type);
	LogOut(LOG_ALL, "Header.Revision=%d", pNotification->Header.Revision);
	LogOut(LOG_ALL, "Header.Length=%d", pNotification->Header.Length);

	LogOut(LOG_ALL, "strRemoteDeviceName = %s", pNotification->strRemoteDeviceName);

	//LogOutW(LOG_ALL, "strRemoteDeviceName = %s", pNotification->strRemoteDeviceName);

	OutputDebugStringW(pNotification->strRemoteDeviceName);
	OutputDebugString("\n");

	LogOut(LOG_ALL, "RemoteDeviceAddress=%x,%x,%x,%x,%x,%x",
		pNotification->RemoteDeviceAddress[0],
		pNotification->RemoteDeviceAddress[1],
		pNotification->RemoteDeviceAddress[2],
		pNotification->RemoteDeviceAddress[3],
		pNotification->RemoteDeviceAddress[4],
		pNotification->RemoteDeviceAddress[5]);


	if (pNotification->type == ProvisioningRequestNotification)
	{
		LogOut(LOG_ALL, "ProvisioningRequestNotification");
		LogOut(LOG_ALL, "session=%x", pNotification->ProvisioningRequestInfo.hSessionHandle);
		LogOut(LOG_ALL, "PossibleConfigMethods=%x", pNotification->ProvisioningRequestInfo.PossibleConfigMethods);
		LogOut(LOG_ALL, "------------------------------------");
	}
	else if (pNotification->type == ReconnectRequestNotification)
	{
		LogOut(LOG_ALL, "ReconnectRequestNotification");
		LogOut(LOG_ALL, "session=%x", pNotification->ReconnectRequestInfo.hSessionHandle);
		//LogOut(LOG_ALL, "GroupID=%d", pNotification->ReconnectRequestInfo.GroupID);

		LogOut(LOG_ALL, "GroupID.DeviceAddress=%x,%x,%x,%x,%x,%x",
			pNotification->ReconnectRequestInfo.GroupID.DeviceAddress[0],
			pNotification->ReconnectRequestInfo.GroupID.DeviceAddress[1],
			pNotification->ReconnectRequestInfo.GroupID.DeviceAddress[2],
			pNotification->ReconnectRequestInfo.GroupID.DeviceAddress[3],
			pNotification->ReconnectRequestInfo.GroupID.DeviceAddress[4],
			pNotification->ReconnectRequestInfo.GroupID.DeviceAddress[5]);

		LogOut(LOG_ALL, "uSSIDLength=%d", pNotification->ReconnectRequestInfo.GroupID.SSID.uSSIDLength);
		LogOut(LOG_ALL, "ucSSID=%d", pNotification->ReconnectRequestInfo.GroupID.SSID.ucSSID);
		LogOut(LOG_ALL, "------------------------------------");
	}
	else if (pNotification->type == ConnectCompleteNotification)
	{
		LogOut(LOG_ALL, "ConnectCompleteNotification");
		LogOut(LOG_ALL, "session=%x", pNotification->ConnectCompleteInfo.hSessionHandle);

		// GUID of the Wi Fi Direct network interface
		LogOut(LOG_ALL, "guidSessionInterface=%x-%x-%x-%x%x-%x%x%x%x%x%x",
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data1,
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data2,
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data3,
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[0],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[1],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[2],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[3],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[4],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[5],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[6],
			pNotification->ConnectCompleteInfo.guidSessionInterface.Data4[7]
			);

		// Group ID of the group use for the connection.
		//DOT11_WFD_GROUP_ID GroupID;        
		LogOut(LOG_ALL, "GroupID.DeviceAddress=%x,%x,%x,%x,%x,%x",
			pNotification->ConnectCompleteInfo.GroupID.DeviceAddress[0],
			pNotification->ConnectCompleteInfo.GroupID.DeviceAddress[1],
			pNotification->ConnectCompleteInfo.GroupID.DeviceAddress[2],
			pNotification->ConnectCompleteInfo.GroupID.DeviceAddress[3],
			pNotification->ConnectCompleteInfo.GroupID.DeviceAddress[4],
			pNotification->ConnectCompleteInfo.GroupID.DeviceAddress[5]);
		LogOut(LOG_ALL, "uSSIDLength=%d", pNotification->ConnectCompleteInfo.GroupID.SSID.uSSIDLength);
		LogOut(LOG_ALL, "ucSSID=%d", pNotification->ConnectCompleteInfo.GroupID.SSID.ucSSID);



		// If a new profile was generated, this pointer is set to a null terminated string. When the callback returns
		// this pointer will be freed and hence can not be accessed.

		//PWSTR strProfile; ->
		if (pNotification->ConnectCompleteInfo.strProfile)
			LogOut(LOG_ALL, "strProfile=%s", pNotification->ConnectCompleteInfo.strProfile);


		// Local IP address
		//SOCKADDR_STORAGE LocalAddress;

		char outinfo[256];
		unsigned char * pd = (unsigned char *)(pNotification->ConnectCompleteInfo.LocalAddress.__ss_pad1);
		sprintf_s(outinfo, "LocalAddress=%d.%d.%d.%d", pd[2], pd[3], pd[4], pd[5]);
		LogOut(LOG_ALL, outinfo);

		// Remote IP address
		//SOCKADDR_STORAGE RemoteAddress;

		char outinfo2[256];
		unsigned char * pd2 = (unsigned char *)(pNotification->ConnectCompleteInfo.RemoteAddress.__ss_pad1);
		sprintf_s(outinfo2, "RemoteAddress=%d.%d.%d.%d", pd2[2], pd2[3], pd2[4], pd2[5]);
		LogOut(LOG_ALL, outinfo2);



		// RTSP port to be used to setup RTSP session (in host byte order)
		USHORT uRTSPPort;
		LogOut(LOG_ALL, "uRTSPPort = %d", pNotification->ConnectCompleteInfo.uRTSPPort);


		LogOut(LOG_ALL, "------------------------------------");
	}
	else if (pNotification->type == ConnectFailedNotification)
	{
		LogOut(LOG_ALL, "ConnectFailedNotification");
		//HANDLE hSessionHandle;             // Handle to the session
		LogOut(LOG_ALL, "session=%d", pNotification->ConnectFailedInfo.hSessionHandle);

		//DWORD dwError;                     // Set to Win32 error code indicating reason for failure
		LogOut(LOG_ALL, "dwError=%d", pNotification->ConnectFailedInfo.dwError);


		//DWORD dwReasonCode;                // Set to a WLAN specific reason code when a detailed reason for failure is known
		LogOut(LOG_ALL, "dwReasonCode=%d", pNotification->ConnectFailedInfo.dwReasonCode);

		LogOut(LOG_ALL, "------------------------------------");
	}
	else if (pNotification->type == DisconnectedNotification)
	{
		LogOut(LOG_ALL, "DisconnectedNotification");

		//HANDLE hSessionHandle;             // Handle to the session just disconnected
		LogOut(LOG_ALL, "session=%d", pNotification->DisconnectedInfo.hSessionHandle);

		LogOut(LOG_ALL, "------------------------------------");
	}




}


void CWifiCode::doWorkCheckTimeout(WorkThreadEx * pWork)
{

	char spath[260] = { 0 };
	Api_GetMyIniPath(spath);

	int stimeout = GetPrivateProfileIntA("main", "timeout", 120, spath);


	LONGLONG timenow = MyGetCurTime();
	if (m_udptimeout == 0)
		m_udptimeout = timenow;
	
	LONGLONG dift = 0;
	while (api_isneedwork(pWork, 0))
	{

		timenow = MyGetCurTime(); 

		dift = abs(timenow - m_udptimeout) / 10000;
		LogOut(LOG_TIMEOUT, "timeout = %lld timenow=%lld  , m_udptimeout=%lld", dift, timenow, m_udptimeout);
		dift = dift / 1000;

		Sleep(1000);

		if (dift >= stimeout)
		{
			//if (m_bStartIntime == 1)
			{
				InitStopWifi();
				m_onState(NULL, NULL, m_phonename, 1, ConnectOuttime);
			}
			//api_stopworkex(m_threadTimeout, WIFI_TCP_TIMEOUT, 10);
		}
		
	}
	if (dift >= stimeout)
		LogOut(LOG_ALL, "doWorkCheckTimeout thread is exit  after 120 s");
	else 
		LogOut(LOG_ALL, "doWorkCheckTimeout thread is exit  normal");

}


//call back
DWORD WINAPI wfdsinkfun(
	_In_opt_ PVOID Context,
	_In_ const PWFD_DISPLAY_SINK_NOTIFICATION pNotification,
	_Inout_opt_ PWFD_DISPLAY_SINK_NOTIFICATION_RESULT pNotificationResult
	)
{

	testprintfinfo(Context, pNotification, pNotificationResult);


	CWifiCode * pWifi = (CWifiCode *)Context;
	g_ClickToDisconnect = 0;
	int nrrt = 0;

	LONGLONG nowtime = MyGetCurTime();

	//if ()


	//WFDDisplaySinkSetPersistedGroupIDList();
	LogOut(LOG_ALL, "wfdsinkfun come in\n");



	//if (pWifi)
	//	sprintf(pWifi->m_phonename, "%s", (CHAR*)(pNotification->ConnectCompleteInfo.GroupID.SSID.ucSSID));



	DWORD dwlen = sizeof(WFD_DISPLAY_SINK_NOTIFICATION);
	if (pNotification->Header.Type == WFD_DISPLAY_SINK_NOTIFICATION_OBJECT_TYPE_DEFAULT){};
	if (pNotification->Header.Revision == WFD_DISPLAY_SINK_NOTIFICATION_OBJECT_REVISION_VERSION_1){};
	pNotification->strRemoteDeviceName;
	pNotification->RemoteDeviceAddress;

	char outinfo[1024] = { 0 };
	memset(outinfo, 0, 1024);



	if (g_hand == 0 && pNotification->type != ProvisioningRequestNotification)
	{
		LogOut(LOG_ALL, "WFDDisplaySink get info wihtout  ProvisioningRequestNotification\n");
		if (pNotification->ProvisioningRequestInfo.hSessionHandle)
			WFDDisplaySinkCloseSession(pNotification->ProvisioningRequestInfo.hSessionHandle);
		g_hand = 0;
		return -1;

	}
	else if (g_hand != 0 && g_hand != pNotification->ProvisioningRequestInfo.hSessionHandle)
	{
		LogOut(LOG_ALL, "WFDDisplaySinkCloseSession \n");
		pWifi->InitStopWifi();
		if (pNotification->ProvisioningRequestInfo.hSessionHandle)
			WFDDisplaySinkCloseSession(pNotification->ProvisioningRequestInfo.hSessionHandle);
		return -1;
	}
	else
	{
		g_hand = pNotification->ProvisioningRequestInfo.hSessionHandle;
	}


	MyOutput("\n_________________\n");



	if (pWifi)
	{
		WideCharToMultiByte(CP_ACP, 0, pNotification->strRemoteDeviceName, 32, pWifi->m_phonename, 260, NULL, NULL);

		pWifi->m_onState(NULL, NULL, pWifi->m_phonename, 1, Connecting);
		//sprintf(pWifi->m_phonename, "%s", pNotification->strRemoteDeviceName);



	}

	OutputDebugStringW(pNotification->strRemoteDeviceName);
	wprintf_s(pNotification->strRemoteDeviceName);
	if (pNotification->type == ProvisioningRequestNotification)
	{
		g_isnewStartConnect = 1;  //每次开始进来

		g_hand = pNotification->ProvisioningRequestInfo.hSessionHandle;
		sprintf_s(outinfo, "ProvisioningRequestNotification methds = 0x%x", pNotification->ProvisioningRequestInfo.PossibleConfigMethods);
		if (pWifi)
		{

			//pWifi->m_bStartIntime = 1;

			//api_stopworkex(pWifi->m_threadTimeout, WIFI_TCP_TIMEOUT, 10);
			pWifi->m_threadTimeout = pWifi->WorkInThread(WIFI_TCP_TIMEOUT, NULL, 0, NULL, 0);

		}
		MyOutput(outinfo);

	}
	else if (pNotification->type == ConnectFailedNotification)
	{
		pWifi->m_onState(NULL, NULL, pWifi->m_phonename, 1, Connecting);
		
		//pWifi->m_bStartIntime = 0;
		//api_stopworkex(pWifi->m_threadTimeout, WIFI_TCP_TIMEOUT, 10);

		sprintf_s(outinfo, "ConnectFailedNotification err = %d", pNotification->ConnectFailedInfo.dwError);
		MyOutput(outinfo);
	}
	else if (pNotification->type == ReconnectRequestNotification)
	{
		MyOutput("ReconnectRequestNotification");

	}
	else if (pNotification->type == ConnectCompleteNotification)
	{

		if (pWifi)
		{
			//pWifi->m_bStartIntime = 0;
			//api_stopworkex(pWifi->m_threadTimeout, WIFI_TCP_TIMEOUT, 10);
			


		}
		MyOutput("connect ok");

		MyOutput("mac adress is:");
		outputmacadress(pNotification->RemoteDeviceAddress);

		MyOutput("ip adress is:");
		outputipadress(pNotification->ConnectCompleteInfo.RemoteAddress);

		MyOutput("group mac adress is:");
		outputmacadress(pNotification->ConnectCompleteInfo.GroupID.DeviceAddress);


		//char * ucssid = (CHAR*)(pNotification->ConnectCompleteInfo.GroupID.SSID.ucSSID);
		char ucssid[1024] = { 0 };
		sprintf(ucssid, "%s", (CHAR*)(pNotification->ConnectCompleteInfo.GroupID.SSID.ucSSID));
		MyOutput("group ssid is: %s", ucssid);
		//printf((CHAR*)(pNotification->ConnectCompleteInfo.GroupID.SSID.ucSSID));

		MyOutput("local ip is:");
		outputipadress(pNotification->ConnectCompleteInfo.LocalAddress);


		sprintf_s(outinfo, "\nuRTSPPort is = %d\n", pNotification->ConnectCompleteInfo.uRTSPPort);

		MyOutput(outinfo);

		int pp = pNotification->ConnectCompleteInfo.uRTSPPort;

		if (pWifi)
		{
			OutputDebugString("pwifi is exist\n");
			//sprintf(pWifi->m_phonename, "%s", (CHAR*)(pNotification->ConnectCompleteInfo.GroupID.SSID.ucSSID));
			//pWifi->m_onState(NULL, NULL, pWifi->m_phonename, 1, Connecting);

			//Sleep(500);
			pWifi->SetWfdSink(pNotification);

		}
		//SOCKET sk = socket(,)
	}
	else if (pNotification->type == DisconnectedNotification){
		if (g_hasDisconnected == 2){
			g_hasDisconnected = 1;
			g_hasDisconnectedtime = nowtime;
		}

		pWifi->m_onState(NULL, NULL, pWifi->m_phonename, 1, Disconnected);
		MyOutput("DisconnectedNotification");
	}
	else{
		MyOutput("other methds");
	}
	MyOutput("\n");

	if (pNotification->type == ProvisioningRequestNotification){

		int isreturnerr = 0;
		if (g_hasDisconnected == 0)
		{
			g_hasDisconnectedtime = nowtime;
		}
		else if (g_hasDisconnected == 1)
		{
			LONGLONG  dift = (nowtime - g_hasDisconnectedtime) / 10000;
			if (dift > 0)
			{
				g_hasDisconnected = 0;
				g_hasDisconnectedtime = nowtime;
			}
			else
			{
				LogOut(LOG_ALL, "wfdsinkfun comeing in 20 sec\n");
				pWifi->InitStopWifi();
				return  -1;
			}
		}


		//pWifi->m_onState(NULL, NULL,NULL, 1, Connecting);


		//Sleep(3000);
		if (isreturnerr == 0){
			//pNotification->ProvisioningRequestInfo.hSessionHandle;
			//pNotification->ProvisioningRequestInfo.PossibleConfigMethods;
			if (pNotificationResult)
			{
				memset(pNotificationResult, 0, sizeof(WFD_DISPLAY_SINK_NOTIFICATION_RESULT));
				pNotificationResult->type = ProvisioningRequestNotification;
				pNotificationResult->Header.Type = WFD_DISPLAY_SINK_NOTIFICATION_RESULT_OBJECT_TYPE_DEFAULT;
				pNotificationResult->Header.Length = sizeof(WFD_DISPLAY_SINK_NOTIFICATION_RESULT);
				pNotificationResult->Header.Revision = WFD_DISPLAY_SINK_NOTIFICATION_RESULT_OBJECT_REVISION_VERSION_1;

				// 0x80
				//pNotificationResult->ProvisioningData.ConfigMethod = pNotification->ProvisioningRequestInfo.PossibleConfigMethods;// ChooseLocalConfigMethodFromLocalSupported(pNotification->ProvisioningRequestInfo.PossibleConfigMethods);

				pNotificationResult->ProvisioningData.ConfigMethod = DOT11_WPS_CONFIG_METHOD_PUSHBUTTON;

				if (pNotificationResult->ProvisioningData.ConfigMethod == DOT11_WPS_CONFIG_METHOD_PUSHBUTTON)
				{
					pNotificationResult->ProvisioningData.uPassKeyLength = WFD_SINK_WPS_INFO_MAX_PASSKEY_LENGTH;
					pNotificationResult->ProvisioningData.Passkey[0] = '0';
					pNotificationResult->ProvisioningData.Passkey[1] = '1';
					pNotificationResult->ProvisioningData.Passkey[2] = '2';
					pNotificationResult->ProvisioningData.Passkey[3] = '3';
					pNotificationResult->ProvisioningData.Passkey[4] = '4';
					pNotificationResult->ProvisioningData.Passkey[5] = '5';
					pNotificationResult->ProvisioningData.Passkey[6] = '6';
					pNotificationResult->ProvisioningData.Passkey[7] = '7';
				};
				if (pNotificationResult->ProvisioningData.ConfigMethod == DOT11_WPS_CONFIG_METHOD_DISPLAY)
				{
					pNotificationResult->ProvisioningData.uPassKeyLength = 8;
					pNotificationResult->ProvisioningData.Passkey[0] = '0';
					pNotificationResult->ProvisioningData.Passkey[1] = '1';
					pNotificationResult->ProvisioningData.Passkey[2] = '2';
					pNotificationResult->ProvisioningData.Passkey[3] = '3';
					pNotificationResult->ProvisioningData.Passkey[4] = '4';
					pNotificationResult->ProvisioningData.Passkey[5] = '5';
					pNotificationResult->ProvisioningData.Passkey[6] = '6';
					pNotificationResult->ProvisioningData.Passkey[7] = '7';
				}
				if (pNotificationResult->ProvisioningData.ConfigMethod == DOT11_WPS_CONFIG_METHOD_KEYPAD)
				{
					pNotificationResult->ProvisioningData.uPassKeyLength = WFD_SINK_WPS_INFO_MAX_PASSKEY_LENGTH;
					pNotificationResult->ProvisioningData.Passkey[0] = '0';
					pNotificationResult->ProvisioningData.Passkey[1] = '1';
					pNotificationResult->ProvisioningData.Passkey[2] = '2';
					pNotificationResult->ProvisioningData.Passkey[3] = '3';
					pNotificationResult->ProvisioningData.Passkey[4] = '4';
					pNotificationResult->ProvisioningData.Passkey[5] = '5';
					pNotificationResult->ProvisioningData.Passkey[6] = '6';
					pNotificationResult->ProvisioningData.Passkey[7] = '7';
				}
			}
		}
		else{
			if (pNotificationResult)
				memset(pNotificationResult, 0, sizeof(WFD_DISPLAY_SINK_NOTIFICATION_RESULT));
			//pNotificationResult->ProvisioningData.uPassKeyLength = 0;// WFD_SINK_WPS_INFO_MAX_PASSKEY_LENGTH;
		}
	}
	DWORD dwerr = 0;
	if ((int)(pNotification->type) == ConnectFailedNotification)
	{
		DeviceState state = Disconnected;// = 4
		switch (pNotification->ConnectFailedInfo.dwError)
		{
		case ERROR_SERVICE_NOT_ACTIVE:
			dwerr = pNotification->ConnectFailedInfo.dwError;
			state = ServiceNotActive;
			break;
		case ERROR_TIMEOUT:
			dwerr = pNotification->ConnectFailedInfo.dwError;
			state = ConnectOuttime;
			break;
		case ERROR_INVALID_DATA:
			dwerr = pNotification->ConnectFailedInfo.dwError;
			//state = ReInvalidData;
			state = ReInvalidData;
			break;
		case ERROR_INVALID_PARAMETER:
			dwerr = pNotification->ConnectFailedInfo.dwError;
			state = ReInvalidParameter;
			break;
		case  ERROR_GEN_FAILURE:
			dwerr = pNotification->ConnectFailedInfo.dwError;
			state = GenFailure;
			break;

		default:
			dwerr = pNotification->ConnectFailedInfo.dwError;
			state = GenFailure;
			break;
		}
		pNotification->ConnectFailedInfo.hSessionHandle;
		dwerr = pNotification->ConnectFailedInfo.dwError;
		pNotification->ConnectFailedInfo.dwReasonCode;
		pWifi->InitStopWifi();
		nrrt = -1;

		pWifi->m_onState(NULL, NULL, pWifi->m_phonename, 1, state);
	}
	if ((int)(pNotification->type) == DisconnectedNotification || g_hand == 0)
	{
		pWifi->InitStopWifi();
		nrrt = -1;
	}
	return nrrt;
}


//stop wifidisplay
void   CWifiCode::InitStopWifi()
{
	m_lklk.lk();
	g_step = 0;
	OutputDebugString("InitStopWifi  InitStopWifi\n");
	closesocket(m_skTCP);
	closesocket(m_skUDP);
	m_skTCP = -1;
	m_skUDP = -1;


	api_stopworkex(m_threadTCP, WIFI_TCP, 10);
	api_stopworkex(m_threadUDP, WIFI_UDP, 10);

	api_freeworkex(m_threadTCP);
	api_freeworkex(m_threadUDP);

	// thread  timeout
	api_stopworkex(m_threadTimeout, WIFI_TCP_TIMEOUT, 10);
	api_freeworkex(m_threadTimeout);
	m_udptimeout = 0;


	m_myh264->ExitAadio();


	if (g_hand){
		WFDDisplaySinkCloseSession(g_hand);
		g_hand = 0;
	}
	m_startAVT = -1;
	m_isNeedDump = 0;
	m_isNewAduio = 0;
	m_onStateIsConnected = 0;

	g_SendTearDownTOPhone = 0;
	//m_onState(NULL, NULL, m_phonename, 1, ConnectOuttime);
	if (g_hasDisconnected == 2){
		g_hasDisconnected = 1;
		g_hasDisconnectedtime = MyGetCurTime();
	}


		/*
	DWORD sstop = WFDDisplaySinkStop();
	LogOut(LOG_ALL, "sstop=%d\n", sstop);

	Sleep(100);
	int ret5 = WFDDisplaySinkStart(0, 0, 1, this, wfdsinkfun);
	if (ret5 == 0){
		OutputDebugString("\n\n\n\n\n\n");
		OutputDebugString("the wfdisplay istart SUCCESS\n");
		OutputDebugString("\n\n\n\n\n\n");
		//break;
	}
	else{
		OutputDebugString("\n\n\n\n\n\n");
		OutputDebugString("the wfdisplay iserror  ERROR\n");
		OutputDebugString("the wfdisplay iserror  ERROR\n");
		OutputDebugString("exe exit , please do something\n");
		OutputDebugString("\n\n\n\n\n\n");
		//MessageBox(NULL, "程序退出 !", "MSG", MB_OK | MB_SETFOREGROUND);
		//exit(0);
	}

	*/





	m_lklk.ulk();
}



void CWifiCode::InitVideoFromatTable()
{
	SetTheVideoFromat5_10(g_table5_10);
	SetTheVideoFromat5_11(g_table5_11);
	SetTheVideoFromat5_12(g_table5_12);
}


//thread do hdcp 
void CWifiCode::doWorkHDCP(WorkThreadEx * pWork)
{

}

//thread do uibc
void CWifiCode::doWorkUIBC(WorkThreadEx * pWork)
{

}


//start wifidisplay
int CWifiCode::InitStartWifi(const char* name, pOnState onState, pCallback callback)
{
	//CMyLog * mlog = new CMyLog();

	//testprintfinfo(NULL, NULL, NULL);
	//SHSTDAPI_(BOOL) SHGetSpecialFolderPathA(_Reserved_ HWND hwnd, _Out_writes_(MAX_PATH) LPSTR pszPath, _In_ int csidl, _In_ BOOL fCreate)
	

	


	m_callback = callback;
	m_onState = onState;
	m_threadDeadBuff = WorkInThread(WIFI_DEALBUFF, NULL, 0, NULL, 0);
	m_threadV = WorkInThread(WIFI_VIDEODEC, NULL, 0, NULL, 0);
	m_threadA = WorkInThread(WIFI_AUDIODEC, NULL, 0, NULL, 0);
	m_threadVPush = WorkInThread(WIFI_VIDEODECPUSH, NULL, 0, NULL, 0);
	m_threadVD = WorkInThread(WIFI_VIDEODECDRAW, NULL, 0, NULL, 0);
#ifdef  TESTUDPWITHFILE
	m_threadUDP = WorkInThread(WIFI_UDP, NULL, 0, NULL, 0);
#endif //  TESTUDPWITHFILE



	InitVideoFromatTable();
	API_InitSocket();
	int ret5 = WFDDisplaySinkStart(0, 0, 1, this, wfdsinkfun);
	if (ret5 == 0){
		OutputDebugString("\n\n\n\n\n\n");
		OutputDebugString("the wfdisplay istart SUCCESS\n");
		OutputDebugString("\n\n\n\n\n\n");
		//break;
	}
	else{
		OutputDebugString("\n\n\n\n\n\n");
		OutputDebugString("the wfdisplay iserror  ERROR\n");
		OutputDebugString("the wfdisplay iserror  ERROR\n");
		OutputDebugString("exe exit , please do something\n");
		OutputDebugString("\n\n\n\n\n\n");
		//MessageBox(NULL, "程序退出 !", "MSG", MB_OK | MB_SETFOREGROUND);
		//exit(0);
		return 1;
	}
	return 0;
}





void MyKeyClickSend()
{
	INPUT inputs[2];
	memset(&inputs, 0, sizeof(inputs));
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_DOWN;
	inputs[0].ki.dwFlags = 0;
	inputs[0].ki.time = GetTickCount();
	inputs[0].ki.dwExtraInfo = GetMessageExtraInfo();

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_UP;
	inputs[1].ki.time = GetTickCount();
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
	inputs[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(1, &inputs[0], sizeof(INPUT));
	SendInput(1, &inputs[1], sizeof(INPUT));

}
