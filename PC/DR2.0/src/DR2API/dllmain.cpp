#include "stdafx.h"
#include "shellapi.h"
//#include "./myh264.h"
#include "./udptcp.h"
#include "../../inc/wfdsink.h"
#include "WfdSources.h"

extern HANDLE g_hand;
extern int g_ClickToDisconnect;

extern  int  g_SendTearDownTOPhone;

void printfDebugString(LogType etype, char *  outbuf, char * sstring);

int g_isneedResize = 0;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

HANDLE __stdcall DR2_Create()
{
	return new CWifiCode();
}

void __stdcall DR2_Destroy(HANDLE handle)
{
	CWifiCode * pf = (CWifiCode *)handle;
	if (pf != 0)
	{
		delete pf;
		pf = NULL;
	}
}

int  __stdcall DR2_Start(HANDLE handle, const char* name, void* pState, void * pcall )
{
	CWifiCode * pf = (CWifiCode *)handle;
	if (pf == 0)
		return -1;
	return pf->InitStartWifi(name, (pOnState)pState, (pCallback)pcall);
}

void __stdcall DR2_Stop(HANDLE  handle)
{
	if (handle == NULL)
		return;

	CWifiCode * pf = (CWifiCode *)handle;
	//主动断开
	if (pf)
	{
		pf->InitStopWifi();
		pf->m_onState(NULL, NULL, pf->m_phonename, 1, MYDisconnected);

	}
}


//ximu***
typedef void(*callBackFun) (char* ip);

void __stdcall DR2_registSocketFun(HANDLE  handle, callBackFun pFun)
{
	if (handle == NULL)
		return;

	CWifiCode * pf = (CWifiCode *)handle;
	//主动断开
	if (pf)
	{
		while (true)
		{
			if (pf->m_IpService[0] == 0)
			{
				continue;
			}
			else
			{
				pFun(pf->m_IpService);
				break;
			}
		}

	}
}
//ximu***


void __stdcall DR2_test()
{

}

RECT m_rectin;
RECT m_rectout;

void __stdcall DR2_GetDrawRect(HANDLE handle, RECT *rectin, RECT *rectout)
{
	*rectin = m_rectin;
	*rectout = m_rectout;
}



int  __stdcall DR2_GetISQP(HANDLE  handle)
{
	if (handle == NULL)
		return 0;

	CWifiCode * pf = (CWifiCode *)handle;
	//主动断开
	if (pf)
	{
		return pf->m_isQp;
	}

	return 0;

}




void SetNewWindowRect(RECT * rectin, RECT * rectout, double nw, double nh)
{

	int w = abs(rectin->right - rectin->left);
	int h = abs(rectin->bottom - rectin->top);
	double dw, dh;

	m_rectin = *rectin;

	if (g_isneedResize  )
	{

		if (nw * h > w * nh){
			dw = w;
			dh = dw*nh / nw;
		}
		else{
			dh = h;
			dw = dh*nw / nh;
		}
// 		rectout->left = (w - dw) / 2 + rectin->left;
// 		rectout->top = (h - dh) / 2 + rectin->top;
// 		rectout->right = rectout->left + dw;
// 		rectout->bottom = rectout->top + dh;
// 
// 		rectout->left = (rectout->left + 3) / 4 * 4;
// 		rectout->top = (rectout->top + 3) / 4 * 4;
// 		rectout->right = (rectout->right + 3) / 4 * 4;
//  		rectout->bottom = (rectout->bottom + 3) / 4 * 4;


	//	return;
	}

	else{

		if (w > nw&&h > nh)
		{
			dw = nw;
			dh = nh;
		}
		else
		{
			if (nw * h > w * nh){
				dw = w;
				dh = dw*nh / nw;
			}
			else{
				dh = h;
				dw = dh*nw / nh;
			}
		}
	}

	rectout->left = (w - dw) / 2 + rectin->left;
	rectout->top = (h - dh) / 2 + rectin->top;
	rectout->right = rectout->left + dw;
	rectout->bottom = rectout->top + dh;

	m_rectout = *rectout;

	return;

}









void __stdcall DR2_Draw(HANDLE  hand, HWND hwnd, HDC dc, RECT rt, int isQP) //, char * buffer, int width, int height, int isVertical, int bkSize)
{
	CWifiCode * pwifi = (CWifiCode*)hand;
	if (pwifi)
	{
		pwifi->MyDrawWnd(0);
	}
}



void __stdcall DR2_Click(HANDLE handle, int ClickCmd, char *  sLang  )
{
	CWifiCode * pf = (CWifiCode *)handle;
	if (pf)
	{
		if (ClickCmd == 1)
		{
			if ( stricmp(sLang , "Zh-cn") == 0)
			//打开网页？
				ShellExecute(NULL, "open", ".\\help\\WifiDirect2.0_Help_CN.chm", NULL, NULL, SW_SHOWMAXIMIZED);
			else if (stricmp(sLang, "en-US") == 0)
				ShellExecute(NULL, "open", ".\\help\\WifiDirect2.0_Help_US.chm", NULL, NULL, SW_SHOWMAXIMIZED);
		}
		else if (ClickCmd == 2)
		{
			g_ClickToDisconnect = 1;
			//Sleep(50);
			//断开连接？
			if (g_hand)
			{
				if (pf)
				{
					//主动断开
// 					if (pf->m_skTCP != INVALID_SOCKET)
// 					{
// 						char sendbuf[4096] = { 0 };
// 						char recvbuf[4096] = { 0 };
// 
// 						pf->m_CWfdStringFormat.SinkToSource8(sendbuf, pf->m_sendj);
// 						pf->m_sendj++;
// 						long nlensend = strlen(sendbuf);
// 						if (nlensend == send(pf->m_skTCP, sendbuf, nlensend, 0))
// 						{
// 							printfDebugString(LOG_doWorkTCP, (char*)sendbuf, "send8");
// 							int rsize = recv(pf->m_skTCP, recvbuf, MAX_BUFF_LEN, 0);
// 							if (rsize > 0)
// 							{
// 								recvbuf[rsize] = 0;
// 								printfDebugString(LOG_doWorkTCP, (char*)recvbuf, "recv8");
// 							}
// 						}
// 					}

					g_SendTearDownTOPhone = 1;

					for (int i = 0; i < 100; i++)
					{
						if (g_SendTearDownTOPhone == 0)
						{
							LogOut(LOG_ALL, "send teardwon success") ;
							break;
						}
						else
							Sleep(10);
					}


					pf->InitStopWifi();
					pf->m_onState(NULL, NULL, pf->m_phonename, 1, MYDisconnected);
				}
				else
					WFDDisplaySinkCloseSession(g_hand);
				g_hand = 0;
				pf->m_onState(NULL, NULL, NULL, 1, Disconnected);
			}
		}
		else if (ClickCmd == 3)
		{
			//断开连接？
			pf->m_nTearDown = 1;
		}

		else if (ClickCmd == 4)
		{
			//打开网页？
			if (stricmp(sLang, "Zh-cn") == 0)
				ShellExecute(NULL, "open", ".\\help\\WifiDirect2.0_Connect_CN.chm", NULL, NULL, SW_SHOWMAXIMIZED);
			else if (stricmp(sLang, "en-US") == 0)
				ShellExecute(NULL, "open", ".\\help\\WifiDirect2.0_Connect_US.chm", NULL, NULL, SW_SHOWMAXIMIZED);
		}
	}
}

void __stdcall DR2_SetDrawWnd(void* handle, HWND hwnd)
{

	LogOut(LOG_ALL, "DR2_SetDrawWnd \n");
	CWifiCode * pwifi = (CWifiCode*)handle;
	if (pwifi)
	{
		pwifi->m_hWndDraw = hwnd;
	}
}

void __stdcall DR2_SetWndChanging(void* handle, bool changing)//表示窗口正在切换最大化，或者还原。这个时候你最好不要绘制
{
	
	LogOut(LOG_ALL, "DR2_SetWndChanging \n");
	CWifiCode * pwifi = (CWifiCode*)handle;
	if (pwifi)
	{
		pwifi->m_SetWndChanging = changing;
	}
}

void __stdcall DR2_SetFullscreen(void* handle, bool fullscreen) //设置为false的时候，你再绘制
{
	LogOut(LOG_ALL, "DR2_SetFullscreen \n");
	CWifiCode * pwifi = (CWifiCode*)handle;
	if (pwifi)
	{
		pwifi->m_SetFullscreen = fullscreen;
		if (fullscreen == TRUE){

			pwifi->m_SystemWidth = GetSystemMetrics(SM_CXSCREEN);
			pwifi->m_SystemHeight = GetSystemMetrics(SM_CYSCREEN);

			if (pwifi->m_mydcMemory)
				::BitBlt(pwifi->m_mydcMemory, 0, 0, pwifi->m_SystemWidth, pwifi->m_SystemHeight, 0, 0, 0, BLACKNESS);
		}

	}
}

#pragma region UIBC

void __stdcall DR2_OnText(HANDLE handle, const wchar_t* text)
{

	//wchar_t wstr[260] = { 0 };

	//LogOut(LOG_ALL, "DR2_OnText \n");
}
/**
* @brief 鼠标左键点击事件
* @param handle
* @param clickCount 点击次数, 1为单击;2为双击
* @param width 显示控件的宽度
* @param height 显示控件的高度
* @param x 鼠标相对于显示控件的x偏移
* @param y 鼠标相对于显示控件的y偏移
*/
void __stdcall DR2_OnMouseLeftButtonDown(HANDLE handle, int clickCount, int width, int height, int x, int y)
{
	//LogOut(LOG_ALL, "DR2_OnMouseLeftButtonDown \n");
}
void __stdcall DR2_OnMouseLeftButtonUp(HANDLE handle, int width, int height, int x, int y)
{
	//LogOut(LOG_ALL, "DR2_OnMouseLeftButtonUp \n");
}
void __stdcall DR2_OnMouseMove(HANDLE handle, int width, int height, int x, int y)
{
	//LogOut(LOG_ALL, "DR2_OnMouseMove x=%d,y=%d\n" ,x, y);
}
void __stdcall DR2_OnMouseRightButtonDown(HANDLE handle, int width, int height, int x, int y)
{
	//LogOut(LOG_ALL, "DR2_OnMouseRightButtonDown \n");
}
void __stdcall DR2_OnMouseRightButtonUp(HANDLE handle, int width, int height, int x, int y)
{
	//LogOut(LOG_ALL, "DR2_OnMouseRightButtonUp \n");
}
#pragma endregion
