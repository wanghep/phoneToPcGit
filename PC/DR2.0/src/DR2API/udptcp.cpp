#include "./udptcp.h"
#include "./Rstdef.h"
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mf")
#pragma comment(lib, "mfuuid")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "Winmm")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#define USED3D

BOOL	API_DrawDataNew(HDC hDc, LPRECT rtOut, LPBYTE pData, LONG ws, long hs, long nBit);



#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"netapi32.lib")


extern int g_logoutput;
extern char g_logoutputpath[260];
extern LogType g_strlevel;

CMyLog * g_plog = NULL;


#pragma region MyClassLog


CMyLog::CMyLog()
{
	m_threadLog = NULL;
	m_logdatas = new  CMyBufferList(4096, 10);



}
CMyLog::~CMyLog()
{
	StopLog();

	if (m_logdatas)
	{
		delete[] m_logdatas;
		m_logdatas = NULL;
	}
}
void CMyLog::StartLog()
{
	m_threadLog = WorkInThread(LOG_START, NULL, 0, NULL, 0);
}
void CMyLog::StopLog()
{
	api_stopworkex(m_threadLog, LOG_START, 10);
	api_freeworkex(m_threadLog);
}
BOOL CMyLog::DoWork(WorkThreadEx * pWork, DWORD dwIndex)
{
	if (dwIndex == LOG_START)
	{
		doWorkLog(pWork);
	}

	return TRUE;
}
void CMyLog::pushLog1(LogType etype, const CHAR * FuntionName, ...)
{
	if (etype & g_strlevel || g_strlevel == LOG_ALL)
	{
#pragma warning(disable:4996)
		SYSTEMTIME st;
		GetLocalTime(&st);
		char stime[260] = { 0 };
		sprintf(stime, "%04d-%02d-%02d %02d:%02d:%02d %03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds
			);

		if (!FuntionName)
			return;
		va_list vlArgs;
		CHAR tzText[4096];
		CHAR tzText2[4096];
		va_start(vlArgs, FuntionName);
		vsprintf(tzText, FuntionName, vlArgs);
		sprintf(tzText2, "%s %s\n", stime, tzText);
		//printf("%s", tzText2);
		//OutputDebugStringA(tzText2);
		va_end(vlArgs);

		MyBufInfo * pitem = m_logdatas->GetBuffer1();
		if (pitem)
		{
			sprintf((char*)pitem->m_pbdata, "%s", tzText2);
			m_loglist.PushTail(pitem);
			api_releasebuf(pitem);
		}
#pragma warning(default:4996)
	}

}
void CMyLog::pushLog(char *tzText2)
{
	MyBufInfo * pitem = m_logdatas->GetBuffer1();
	if (pitem)
	{
		sprintf((char*)pitem->m_pbdata, "%s", tzText2);
		m_loglist.PushTail(pitem);
		api_releasebuf(pitem);
	}
}

void CMyLog::doWorkLog(WorkThreadEx * pWork)
{

	while (api_isneedwork(pWork, 0))
	{
		MyBufInfo *pitemrtps = (MyBufInfo*)(m_loglist.PopHead());
		if (pitemrtps)
		{

			//OutputDebugStringA((char*)pitemrtps->m_pbdata);

			if (g_logoutput == 1)
			{
				char * tzText2 = (char*)pitemrtps->m_pbdata;
				static FILE * fp = NULL;
				static  char sp[260] = { 0 };
				if (sp[0] == 0)
				{
					SYSTEMTIME st;
					GetLocalTime(&st);
					char stime[260] = { 0 };
					sprintf(stime, "%04d-%02d-%02d %02d:%02d:%02d %03d",
						st.wYear,
						st.wMonth,
						st.wDay,
						st.wHour,
						st.wMinute,
						st.wSecond,
						st.wMilliseconds
						);

					sprintf(sp, "%s\%04d-%02d-%02d-%02d%02d%02d-%03d.txt",
						g_logoutputpath,
						st.wYear,
						st.wMonth,
						st.wDay,
						st.wHour,
						st.wMinute,
						st.wSecond,
						st.wMilliseconds
						);
				}
				else
				{
					static int wtimes = 0; //写入次数
					fp = fopen(sp, "ab");
					if (fp)
					{
						fwrite(tzText2, strlen(tzText2), 1, fp);
						wtimes++;
						if (wtimes > 40)
						{
							wtimes = 0;
							fclose(fp);
							fp = fopen(sp, "ab");
						}
					}
				}
			}
		}
		Sleep(1);
	}
}



void PushLogData(LogType etype, const CHAR * FuntionName, ...)
{
	if (etype & g_strlevel || g_strlevel == LOG_ALL)
	{
#pragma warning(disable:4996)




		SYSTEMTIME st;
		GetLocalTime(&st);
		char stime[260] = { 0 };
		sprintf(stime, "%04d-%02d-%02d %02d:%02d:%02d %03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds
			);

		if (!FuntionName)
			return;
		va_list vlArgs;
		CHAR tzText[4096];
		CHAR tzText2[4096];
		va_start(vlArgs, FuntionName);
		vsprintf(tzText, FuntionName, vlArgs);
		sprintf(tzText2, "%s %s\n", stime, tzText);
		//printf("%s", tzText2);
		//OutputDebugStringA(tzText2);
		va_end(vlArgs);

		if (g_plog == NULL)
		{
			g_plog = new CMyLog();
			g_plog->StartLog();
			OutputDebugStringA("aaaaaaaaaaaaaaaa");
			MyBufInfo * pitem = g_plog->m_logdatas->GetBuffer1();
			if (pitem)
			{
				sprintf((char*)pitem->m_pbdata, "%s", "CMLog is new start\n");
				g_plog->m_loglist.PushTail(pitem);
				api_releasebuf(pitem);
			}
		}

		MyBufInfo * pitem = g_plog->m_logdatas->GetBuffer1();
		if (pitem)
		{
			sprintf((char*)pitem->m_pbdata, "%s", tzText2);
			g_plog->m_loglist.PushTail(pitem);
			api_releasebuf(pitem);
		}
#pragma warning(default:4996)
	}

}


void releaseLogData()
{
	if (g_plog)
	{
		g_plog->StopLog();
		delete g_plog;
		g_plog = NULL;
	}
}


#pragma endregion

CDrawFrame::CDrawFrame()
{
	m_isneedrender = 0;
	m_pIFp3d = NULL;
	m_hWnd = NULL;
	m_pDFCB = 0;
	m_pvData = NULL;
	m_nMaxDataLen = 0;

	m_w = 0;
	m_h = 0;
	m_nDibNum = 0;
	memset(&m_rtDst, 0, sizeof(m_rtDst));
	ResetData(1024 * 768 * 4);

	m_wOut = 0;
	m_hOut = 0;
	m_pvDataOut = 0;
	m_wOutN = 1024;
	m_hOutN = 768;
	m_nMaxDataLenOut = 0;
	ResetDataOut();

}
CDrawFrame::~CDrawFrame()
{
	Exit();
	m_nMaxDataLen = 0;
	SafeDeletes(m_pvData);
}

void CDrawFrame::ResetDataOut()
{
	long nwh = m_wOutN * m_hOutN * 4;
	if (nwh > m_nMaxDataLenOut)
	{
		SafeDeletes(m_pvDataOut);
		m_nMaxDataLenOut = nwh + 0x400;
		m_pvDataOut = new BYTE[m_nMaxDataLenOut];
		memset(m_pvDataOut, 0, m_nMaxDataLenOut);
	}
	m_wOut = m_wOutN;
	m_hOut = m_hOutN;
}
HRESULT CDrawFrame::Init(HWND hWnd, ICallBack * pDFCB)
{
	m_lock.Lock();
	m_isneedrender = 0;
	m_hWnd = hWnd;
	m_pDFCB = pDFCB;

	SafeReleaseP(m_pIFp3d);
#ifdef USED3D
	m_pIFp3d = CreateFpD3D(hWnd);
#endif
	m_lock.Unlock();
	return S_OK;
}
VOID  CDrawFrame::Exit()
{
	m_lock.Lock();
	SafeReleaseP(m_pIFp3d);
	m_hWnd = 0;
	m_pDFCB = NULL;
	m_lock.Unlock();
}

void Resize32(LPVOID pvdst, DWORD wd, DWORD hd, long dxd,
	LPVOID pvsrc, DWORD ws, DWORD hs, long dxs,
	int mode);
HRESULT CDrawFrame::OnRefreshData(void * pvData, long nDibNum, long pitchSrc, RECT * prtSrc, RECT * prtDst, BOOL bDrawNow)
{
	HRESULT hr = E_FAIL;
	long nh = prtSrc->bottom - prtSrc->top;
	long nw = prtSrc->right - prtSrc->left;
	if (nh > 0 && nDibNum > 0 && nw > 0)
	{
		m_w = nw;
		m_h = nh;
		m_nDibNum = nDibNum;
		hr = S_OK;
		LPBYTE pSrc = (LPBYTE)pvData;
		pSrc += prtSrc->top * pitchSrc + prtSrc->left * m_nDibNum / 8;

		m_lock.Lock();
		if (m_pIFp3d)
			hr = m_pIFp3d->OnRefreshData(pvData, m_nDibNum, pitchSrc, m_w, m_h);
		else
		{
			//if(m_nDibNum == 32 && m_pvDataOut)
			//{
			//	ResetDataOut();
			//	long npitch = (m_wOut * nDibNum + 31)/32 * 4;
			//	LPBYTE pDst = (LPBYTE)m_pvDataOut;
			//	pDst += (nh - 1) * npitch; 
			//	long anpitch = -npitch;
			//	Resize32((LPDWORD)pDst,m_wOut,m_hOut,anpitch,
			//		(LPDWORD)pSrc,nw,nh,pitchSrc,
			//		1);
			//}else
			{
				long npitch = (nw * nDibNum + 31) / 32 * 4;
				long nTotalLen = npitch * nh;

				ResetData(nTotalLen);
				LPBYTE pDst = (LPBYTE)m_pvData;

				long minLen = min(npitch, pitchSrc);
				long otherLen = npitch - minLen;


				pDst += (nh - 1) * npitch;
				long anpitch = -npitch;

				for (int i = 0; i < nh; i++)
				{
					memcpy(pDst, pSrc, minLen);
					pDst += anpitch;
					pSrc += pitchSrc;
				}
			}
		}
		if (bDrawNow)
		{
			SIZE sS = { m_w, m_h };
			if (m_pIFp3d)
				m_pIFp3d->RenderData(m_hWnd, 0, 0, &sS, 0, NULL);
			else
			{
				HDC hDC = GetDC(m_hWnd);
				if (hDC)
				{
					//API_DrawDataNew(hDC,NULL,(LPBYTE)m_pvData,m_w,m_h,m_nDibNum);
					::ReleaseDC(m_hWnd, hDC);
				}
			}
		}
		else{
			//InvalidateRect(m_hWnd, NULL, FALSE);
			m_isneedrender = 1;
		}
	}
	m_lock.Unlock();
	return hr;
}
HRESULT CDrawFrame::SetShowMode(long nShowMode)
{
	if (m_pIFp3d)
	{
		m_pIFp3d->OnRefreshData(0, 0, 0, 0, 0);
	}
	return S_OK;
}

HRESULT CDrawFrame::RenderData(HDC hDc, long x, long y, LPSIZE pSizeS, int nmust, int isactive, RECT* rcSrc)
{
	HRESULT hr = E_FAIL;
	if (m_isneedrender == 0 && nmust == 0)
		return S_OK;

	m_lock.Lock();
	m_isneedrender = 0;
	if (m_pIFp3d)
	{
		HWND hWnd = WindowFromDC(hDc);
		hr = m_pIFp3d->RenderData(hWnd, x, y, pSizeS, isactive, NULL);
	}
	else
	{
		RECT rt = { x,
			y,
			(pSizeS ? x + pSizeS->cx : x + m_w),
			(pSizeS ? y + pSizeS->cy : y + m_h)
		};
		//if(m_nDibNum == 32 && m_pvDataOut)
		//{
		//	if(API_DrawDataNew(hDc,&rt,(LPBYTE)m_pvDataOut,m_wOut,m_hOut,m_nDibNum))
		//		hr = S_OK;
		//	m_wOutN = rt.right - rt.left;
		//	m_hOutN = rt.bottom - rt.top;
		//}else
		{
			//if (API_DrawDataNew(hDc, &rt, (LPBYTE)m_pvData, m_w, m_h, m_nDibNum)==isactive)
			hr = S_OK;
		}
	}
	m_lock.Unlock();
	return hr;
}


void CDrawFrame::ResetData(long nMaxData)
{
	if (nMaxData > m_nMaxDataLen)
	{
		LPBYTE pDataNew = new BYTE[nMaxData + 10];
		if (pDataNew)
		{
			LPBYTE pDataTmp = (LPBYTE)m_pvData;
			m_pvData = pDataNew;
			SafeDeletes(pDataTmp);
			m_nMaxDataLen = nMaxData;
		}
	}
}


extern char g_logoutputpath[260];

class CMyUdpData :public MyArrayData
{
public:
	CMyUdpData();
	~CMyUdpData();
	char m_data[3072];
	int m_rsize;
};


class CMyBufferItem
	:public MyArrayData
{
public:
	MyBufInfo m_info;

	LONG	m_cRef1;
	//	DWORD	m_dwLen;
	CMyBufferList * m_PClear;
	//	unsigned long long m_lltime;
	//	unsigned char * m_pbdata;
	//	int m_nw;// = m_myh264->m_nw;
	//	int m_nh;// = m_myh264->m_nh;
	//	int m_nisBlack;// = m_myh264->m_isback;
	//	int m_nbkSize;// = m_myh264->m_isbackSize;
	CMyBufferItem(int ncount);
	virtual ~CMyBufferItem();
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	void SetClear(CMyBufferList * PClear);
};









MyRTPINFO::MyRTPINFO()
{
	m_aid = 0;
	m_prtp = 0;
	m_pts = 0;
	m_ptspat = 0;

	m_dwnidid = 0;
	m_dwpmtid = 0;
	m_vdpid = 0;
	m_adpid = 0;
	m_bvid = 0;
	m_baid = 0;
	m_dww = 0;
	m_dwh = 0;

	m_vl = 0;
	m_vmax = 0;
	m_al = 0;
	m_amax = 0;

	m_ullvpts = 0;
	m_ullapts = 0;

	m_wfe.nChannels = 2;
	m_wfe.nSamplesPerSec = 48000;
	m_wfe.wBitsPerSample = 16;

	m_dwpid = 0;

	m_vp = new unsigned char[MAXVFRAME];
	m_ap = new unsigned char[0x10000];
	m_isidr = 1;
}


MyRTPINFO::~MyRTPINFO(){
	SafeDeletes(m_vp);
	SafeDeletes(m_ap);
};


int MyRTPINFO::dopeshead(LPBYTE pbts, int & iOff, unsigned char & _bid, unsigned long long & _ullpts)
{
	int _npeslen = 0;
	if (pbts[iOff] == 0 && pbts[iOff + 1] == 0 && pbts[iOff + 2] == 1)
	{
		if (_bid == 0)
			_bid = pbts[iOff + 3];
		if (_bid == pbts[iOff + 3])
		{
			iOff += 4;
			_npeslen = TSGETNUM(pbts + iOff);
			iOff += 2;
			int ptsflag = (pbts[iOff + 1] & 0x80);
			int dtsflag = (pbts[iOff + 1] & 0x40);
			iOff += 2;
			int pesheadlen = pbts[iOff];
			iOff += 1;
			if (ptsflag)
				getptsdts(pbts + iOff, &_ullpts);
			iOff += pesheadlen;
			if (_npeslen > (3 + pesheadlen))
			{
				_npeslen -= (3 + pesheadlen);
			}
		}
	}
	return _npeslen;
}




void MyRTPINFO::getptsdts(unsigned char * pb, unsigned long long * pts)
{
	unsigned long long ll32_30 = (pb[0] & 0x0e);
	unsigned long long ll29_22 = pb[1];
	unsigned long long ll21_15 = (pb[2] & 0xfe);
	unsigned long long ll14_7 = pb[3];
	unsigned long long ll6_0 = (pb[4] & 0xfe);
	unsigned long long lpts = ((ll32_30 << 25) | (ll29_22 << 22) | (ll21_15 << 14) | (ll14_7 << 7) | (ll6_0 >> 1));

	*pts = lpts * 100 / 9;
}


int MyRTPINFO::CheckStart(unsigned char * pbts){
	DWORD dwpid = TSGETPID(pbts + 1);
	if (((TS_packet_header *)pbts)->payload_unit_start_indicator){
		if (dwpid == m_vdpid && m_vdpid)
			return 1;
		if (dwpid == m_adpid && m_adpid)
			return 2;
	}
	return 0;
}



bool h264_decode_seq_parameter_set(BYTE * buf, UINT nLen, int &Width, int &Height);
void OnTest();




unsigned char * MyRTPINFO::GetTsData(unsigned char * pbts){
	//TS_packet_header *
	m_nbv = 0;
	m_nba = 0;
	m_isdvr = 0;
	m_pbts = pbts;
	int iOff = 4;

	DWORD dwpid = TSGETPID(pbts + 1);
	m_dwpid = dwpid;

	if (dwpid == 0 || dwpid == m_dwpmtid){
		switch (m_pts->adaption_field_control)
		{
		case 0:
			;// return NULL;
		case 1:// 无调整字段，仅含有效负载 
			iOff += m_pbts[iOff] + 1;
			break;
		case 2:// 仅含调整字段，无有效负载
			iOff += m_pbts[iOff] + 1;
			break;
		case 3:// 调整字段后含有效负载
			iOff += m_pbts[iOff] + 1;
			iOff += m_pbts[iOff] + 1;
			break;
		default:
			break;
		}
	}
	else if (dwpid == m_vdpid || dwpid == m_adpid){
		switch (m_pts->adaption_field_control)
		{
		case 0:
			;// return NULL;
		case 1:// 无调整字段，仅含有效负载 
			//iOff += m_pbts[iOff] + 1;
			break;
		case 2:// 仅含调整字段，无有效负载
			iOff += m_pbts[iOff] + 1;
			break;
		case 3:// 调整字段后含有效负载
			//iOff += m_pbts[iOff] + 1;
			iOff += m_pbts[iOff] + 1;
			break;
		default:
			break;
		}
	}


	m_pbtsdata = pbts + iOff;
	if (dwpid == 0){
		int len = TSGETLEN(pbts + iOff + 1);
		iOff += sizeof(TS_PAT_START);
		for (int j = 0; j < len - 9; j += sizeof(TS_PAT_Program)){
			DWORD dwn = TSGETNUM(pbts + iOff);
			switch (dwn)
			{
			case 0:
				m_dwnidid = TSGETPID(pbts + iOff + 2);
				break;
			case 1:
			default:
				m_dwpmtid = TSGETPID(pbts + iOff + 2);
				break;
			}
			iOff += sizeof(TS_PAT_Program);
		}
	}
	else if (dwpid == m_dwpmtid){
		int len = TSGETLEN(pbts + 1 + iOff);
		iOff += sizeof(TS_PMTSTART);
		iOff += ((pbts + iOff - 2)[1] | (((pbts + iOff - 2)[0] & 0xf) << 8));
		for (int j = 0; j < len - 13;){
			BYTE bst = pbts[iOff];
			switch (bst)
			{
			case 0x1b:
				m_vdpid = TSGETPID(pbts + 1 + iOff);
				break;
			case 0x0f:
			case AUDIOIDUSELPCM:
				m_adpid = TSGETPID(pbts + 1 + iOff);
				m_aid = bst;
				break;
			default:
				break;
			}

			int infolen = TSGETLEN(pbts + 3 + iOff) + sizeof(TS_PMT_Stream);
			iOff += infolen;
			j += infolen;
		}
	}
	else if (dwpid == m_vdpid){
		if (m_pts->payload_unit_start_indicator){
			m_vl = 0;
			m_vmax = 0;

			PESHEAD *  pppes = (PESHEAD *)(pbts + iOff);
			PESHEADOld * pppold = (PESHEADOld *)(pbts + iOff);

			int nvmax = dopeshead(pbts, iOff, m_bvid, m_ullvpts);
			if (nvmax == 0)
				nvmax = MAXVFRAME;
			{
				if (pbts[iOff] == 0 && pbts[iOff + 1] == 0 && pbts[iOff + 2] == 0)
				{
					int n1 = pbts[iOff + 4] & 0xf;
					if (n1 == 0x5)
					{
						m_isdvr = 1;
						m_isidr = 1;
					}

					int  isos = pbts[iOff + 4] & 0xf;
					if (isos == 0x7 || isos == 0x9){

						m_isidr = 1;
						m_isdvr = 1;
						//OnTest();

						if (isos == 0x7)
						{
							//针对 手机 数据流的处理
							int Width, Height;
							h264_decode_seq_parameter_set((BYTE*)(&pbts[iOff + 4]), 11, Width, Height);
							Width = Width;
							m_dww = Width;
							m_dwh = Height;
							LogOut(LOG_ALL, "h264 Get  Width=%d,Height=%d", Width, Height);
						}
						else if (isos == 0x9)
						{
							int  isos7 = pbts[iOff + 4 + 6] & 0xf;   //针对PC数据流的处理
							if (isos7 == 0x7)
							{
								int Width, Height;
								h264_decode_seq_parameter_set((BYTE*)(&pbts[iOff + 4 + 6]), 11, Width, Height);
								Width = Width;
								m_dww = Width;
								m_dwh = Height;
								LogOut(LOG_ALL, "h264 Get 0x7 Width=%d,Height=%d", Width, Height);
							}
						}


						//LogOut(LOG_ALL, "kkkkkkkkkkkkkkkkkk");
					}
					//if (isos == 0x7 || isos == 0x9){
					//	m_dww = (((pbts[iOff + 9] & 0x1f) << 6) + (pbts[iOff + 10] >> 2)) << 4;
					//	m_dwh = (((pbts[iOff + 10] & 0x3) << 9) + (pbts[iOff + 11] << 1) + (pbts[iOff + 12] >> 7)) << 4;
					//}
				}
				if (m_isidr){
					m_vl = 0;
					m_vmax = nvmax;
					if (iOff < 188){
						m_nbv = 188 - iOff;
						int nOff = min(m_nbv, (int)m_vmax);
						if (nOff){
							memcpy(m_vp, pbts + iOff, nOff);
							m_vl = nOff;
						}
					}
				}
			}
		}
		else{
			if (m_isidr && iOff < 188){
				m_nbv = 188 - iOff;
				if (m_vl < m_vmax){
					int noff = m_nbv;
					if (m_vl + m_nbv > m_vmax){
						noff = m_vmax - m_vl;
					}
					if (noff){
						memcpy(m_vp + m_vl, pbts + iOff, noff);
						m_vl += noff;
					}
				}
			}
		}
	}
	else if (dwpid == m_adpid){
		if (m_pts->payload_unit_start_indicator){
			m_al = 0;


			PESHEAD *  pppes = (PESHEAD *)(pbts + iOff);
			PESHEADOld * pppold = (PESHEADOld *)(pbts + iOff);



			m_amax = dopeshead(pbts, iOff, m_baid, m_ullapts);
			if (m_amax == 0)
				m_amax = 0x10000;
			//if (m_amax)
			{
				if (iOff < 188){
					m_nba = 188 - iOff;
					int nOff = min(m_nba, (int)m_amax);
					if (nOff){
						memcpy(m_ap, pbts + iOff, nOff);
						m_al = nOff;
					}
				}
				if (m_aid == AUDIOIDUSELPCM)
				{
					LPCMPAYLOADHEAD * ph = (LPCMPAYLOADHEAD*)(pbts + iOff);

					ph->audio_sampling_frequency;
					ph->number_of_audio_channel;
					ph->quantization_word_length;

					m_wfe.nChannels = 2;
					m_wfe.nSamplesPerSec = 48000;//
					m_wfe.wBitsPerSample = 16;
				}
				else// if (m_aid == 0x0f)
				{
					AACPES * ph = (AACPES*)(pbts + iOff);
					WORD * pw = (WORD*)(pbts + iOff);
					if (((*(WORD*)(pbts + iOff)) & 0xf0ff) == 0xf0ff)
					{
						static int s_sample[16] = {
							96000, 88300, 64000, 48000, 44100, 32000, 24000, 22050,
							16000, 2000, 11025, 8000, 48000, 48000, 48000, 48000
						};
						m_wfe.nChannels = ((((pbts + iOff)[2]) & 1) << 2) + ((((pbts + iOff)[3]) & 0xc0) >> 6);//48000
						m_wfe.nSamplesPerSec = s_sample[((((pbts + iOff)[2]) & 0x3c) >> 2)];//2,4,6,8
						//m_wfe.nBlockAlign = (pbts[iOff + 6] >> 6);
						//m_amax = ((pbts[iOff + 3] & 3) << 11) + (pbts[iOff + 4] << 3) + (pbts[iOff + 5] >> 5);
					}
				}
			}
		}
		else{
			if (iOff < 188){
				m_nba = 188 - iOff;
				memcpy(m_ap + m_al, pbts + iOff, m_nba);
				if (m_al < m_amax){
					int noff = m_nba;
					if (m_al + m_nba > m_amax){
						noff = m_amax - m_al;
					}
					if (noff){
						memcpy(m_ap + m_al, pbts + iOff, noff);
						m_al += noff;
					}
				}
			}
		}
	}
	else{
		//m_dwpid = dwpid;
	}
	return NULL;
}






CMyUdpData::CMyUdpData(){
	m_data[0] = 0;
	m_rsize = 0;
}
CMyUdpData::~CMyUdpData(){
}




CMyBufferItem::CMyBufferItem(int ncount)
{
	m_cRef1 = 1;
	m_PClear = NULL;

	m_info.m_pbdata = new unsigned char[ncount];
	m_info.m_dwLen = 0;
	m_info.m_lltime = 0;
	m_info.m_nw = 0;
	m_info.m_nh = 0;
	m_info.m_nisBlack = 0;
	m_info.m_nbkSize = 0;
	m_info.m_pvthis = this;
}
CMyBufferItem::~CMyBufferItem()
{
	if (m_info.m_pbdata){
		delete[] m_info.m_pbdata;
		m_info.m_pbdata = 0;
	}
	m_info.m_dwLen = 0;
}
HRESULT STDMETHODCALLTYPE CMyBufferItem::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IUnknown)
	{
		AddRef();
		(*ppvObject) = (void*)this;
		return S_OK;
	}
	else
		return E_FAIL;
};
ULONG   CMyBufferItem::AddRef()
{
	LONG lRef = InterlockedIncrement(&m_cRef1);
	return max(ULONG(m_cRef1), 1ul);
}
ULONG CMyBufferItem::Release(){
	LONG lRef = InterlockedDecrement(&m_cRef1);
	if (lRef <= 0)
	{
		if (m_PClear)
			m_PClear->ClearBuffer1(&(this->m_info));
		return ULONG(0);
	}
	else
	{
		return max(ULONG(m_cRef1), 1ul);
	}
}
void CMyBufferItem::SetClear(CMyBufferList * PClear)
{
	m_PClear = PClear;
}


CMyBufferArray::CMyBufferArray()
{
	m_pvlist = new MyUnArray;
	//m_pHandle = CreateEventA(0, 0, 0, 0);
};
CMyBufferArray::~CMyBufferArray()
{
	if (m_pvlist){
		MyUnArray * pvl = (MyUnArray*)m_pvlist;
		m_pvlist = 0;
		delete pvl;
	}
	//if (m_pHandle){
	//	CloseHandle(m_pHandle);
	//	m_pHandle = 0;
	//}
};
MyBufInfo *	CMyBufferArray::PopHead()
{
	CMyBufferItem * pBuffer = (CMyBufferItem *)(((MyUnArray*)m_pvlist)->PopHead());
	if (pBuffer)
		return &(pBuffer->m_info);
	return NULL;
}
MyBufInfo *	CMyBufferArray::GetHead()
{
	CMyBufferItem * pBuffer = (CMyBufferItem *)(((MyUnArray*)m_pvlist)->GetHead());
	if (pBuffer)
		return &(pBuffer->m_info);
	return NULL;
}
void		CMyBufferArray::PushTail(MyBufInfo * pI){
	CMyBufferItem * pBuffer = 0;
	if (pI)
		pBuffer = (CMyBufferItem*)(pI->m_pvthis);
	if (pBuffer)
	{
		((MyUnArray*)m_pvlist)->PushTail(pBuffer);
	}
}
int CMyBufferArray::GetNum()
{
	return ((MyUnArray*)m_pvlist)->GetNum();
}




CMyBufferList::CMyBufferList(DWORD dwMaxBufferSize, DWORD dwdefaultnum) :m_dwMaxBufferSize(dwMaxBufferSize)
{
	m_pvlist = new MyUnArray;
	m_listnum = dwdefaultnum;
	m_listnumtemp = dwdefaultnum;
	for (DWORD i = 0; i < dwdefaultnum; i++)
	{
		CMyBufferItem * pBuffer = new CMyBufferItem(m_dwMaxBufferSize);
		if (pBuffer)
		{
			pBuffer->SetClear(this);
			pBuffer->Release();
		}
	}

}



CMyBufferList::~CMyBufferList()
{
	CMyBufferItem * pData = (CMyBufferItem *)(((MyUnArray*)m_pvlist)->PopHead());
	while (pData)
	{
		delete pData;
		pData = (CMyBufferItem *)(((MyUnArray*)m_pvlist)->PopHead());
	}
	if (m_pvlist){
		MyUnArray * pvl = (MyUnArray*)m_pvlist;
		m_pvlist = 0;
		delete pvl;
	}
}
/*CMyBufferItem * CMyBufferList::GetItem(){
CMyBufferItem * pBuffer = (CMyBufferItem *)(((MyUnArray*)m_pvlist)->PopHead());
if (pBuffer)
{
//pBuffer->AddRef();
}
else
{
pBuffer = new CMyBufferItem(m_dwMaxBufferSize);
m_listnumtemp++;
}
if (pBuffer)
pBuffer->SetClear(this);
return pBuffer;
}*/
MyBufInfo *	CMyBufferList::GetBuffer1()
{
	CMyBufferItem * pBuffer = (CMyBufferItem *)(((MyUnArray*)m_pvlist)->PopHead());
	if (pBuffer)
	{
		//pBuffer->AddRef();
	}
	else
	{
		pBuffer = new CMyBufferItem(m_dwMaxBufferSize);
		m_listnumtemp++;
	}
	if (pBuffer)
		pBuffer->SetClear(this);
	return &(pBuffer->m_info);
}
/*
void		CMyBufferList::ClearItem(CMyBufferItem * pBuffer){
if (pBuffer)
{
pBuffer->SetClear(NULL);
((MyUnArray*)m_pvlist)->PushTail(pBuffer);

if (m_listnumtemp > m_listnum)
{
m_listnumtemp--;
//delete pBuffer;
//pBuffer = NULL;
}
}
}
*/
void		CMyBufferList::ClearBuffer1(MyBufInfo * pI){
	CMyBufferItem * pBuffer = 0;
	if (pI)
		pBuffer = (CMyBufferItem*)(pI->m_pvthis);
	if (pBuffer)
	{
		pBuffer->SetClear(NULL);
		((MyUnArray*)m_pvlist)->PushTail(pBuffer);

		if (m_listnumtemp > m_listnum)
		{
			m_listnumtemp--;
			//delete pBuffer;
			//pBuffer = NULL;
		}
	}
}








void lib_doWorkDealBuff(
	WorkThreadEx * pWork,
	CMyBufferArray *rtpslist,
	CMyBufferArray *alist,
	CMyBufferArray *vlist,
	MyRTPINFO *mr,
	CMyBufferList * vddatas,
	CMyBufferList * addatas,
	HANDLE hrtps,
	HANDLE hv,
	HANDLE ha,
	unsigned long long & startAVT,
	char &isNeedDump,
	int & nohp,
	int & sneedidr,
	int & isnewStartConnect
	)
{

	int nstep = 0;
	while (pWork->IsNeedWork(0))
	{
		MyBufInfo * pitemrtps = (MyBufInfo*)(rtpslist->PopHead());
		if (pitemrtps){

			if (isnewStartConnect == 1)
			{
				isnewStartConnect = 0;
				//mr->m_adpid = 0;
				mr->m_baid = 0;   // 每次新的 链接过来，清零一下声音的 头
				mr->m_bvid = 0;
			}

			//int listnum = vlist.GetNum();
			HANDLE  hd = GetCurrentThread();
			int thd = GetThreadId(hd);
			int islostrtp = 0;

#ifdef TESTUDPWITHFILE
#else 
			if (1)
			{
				RTP_HEAD  * rtp_head = (RTP_HEAD  *)(pitemrtps->m_pbdata);
				char st[260] = { 0 };
				int num = (rtp_head->Sequence_number1 << 8) + rtp_head->Sequence_number0;
				static int last = num;
				if (last != num)
				{

					static int lostnum = 0;
					if (lostnum > 5)
					{
						startAVT = -1;
						lostnum = 0;
					}
					lostnum++;
					sprintf(st, "lost frame seqnum=%d, last=%d\n", num, last);
					LogOut(LOG_LOSTFRAME, st);
					last = num;
					islostrtp = 1;

					//if (sneedidr == 0)
					//	sneedidr = 1;
				}
				last++;
				if (last >= 0x10000)
					last = 0;
			}

			//是否存文件
			static FILE * fp = NULL;
			static char savep[260] = { 0 };
			static int nnn = 0;
			nnn++;
			//static char isd = 0;
			if (isNeedDump == 1){

				// 				CHAR	cModulePath[MAX_PATH] = { 0 };
				// 				CHAR	cDrive[MAX_PATH] = { 0 };
				// 				CHAR	cDir[MAX_PATH] = { 0 };
				// 				CHAR	cED_coder[MAX_PATH] = { 0 };
				// 				CHAR	cED_coderd[MAX_PATH] = { 0 };
				// 				CHAR	cED_aacd[MAX_PATH] = { 0 };
				// 
				// 				GetModuleFileNameA(0, cModulePath, MAX_PATH);
				// 				_splitpath(cModulePath, cDrive, cDir, NULL, NULL);
				// 				_makepath(cModulePath, cDrive, cDir, NULL, NULL);
				// 				_makepath(cED_coder, cDrive, cDir, "dump\\", NULL);

				SYSTEMTIME st;
				GetLocalTime(&st);
				isNeedDump = 2;
				sprintf_s(savep, "%s%04d-%02d-%02d-%02d%02d%02d-%04d.ts",
					g_logoutputpath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
				LogOut(LOG_ALL, "dump file name is: %s", savep);
				fp = fopen(savep, "ab+");
			}
			if (isNeedDump == 2)
			{
				if (fp == NULL)
					fp = fopen(savep, "ab+");
				if (fp)
				{
					fwrite(&pitemrtps->m_dwLen, 1, 4, fp);
					fwrite(pitemrtps->m_pbdata, 1, pitemrtps->m_dwLen, fp);
					if (nnn >= 300)
					{
						nnn = 0;
						fclose(fp);
						fp = NULL;
					}
				}
			}
#endif

			//#define TESTUDPWITHFILE
#ifdef TESTUDPWITHFILE
			for (int i = 0; i < pitemrtps->m_dwLen;)
			{
				unsigned char * ps = ((unsigned char *)(pitemrtps->m_pbdata)) + i;// +(i - 188);
#else 
			for (int i = 12 + 188; i <= (int)(pitemrtps->m_dwLen);)
			{
				unsigned char * ps = ((unsigned char *)(pitemrtps->m_pbdata)) + (i - 188);
#endif

				if (ps[0] != 0x47)
					ps[0] = ps[0];

				TS_packet_header * tps = (TS_packet_header *)ps;
				//int npid = (tps->PID1 << 8) + tps->PID2;
				//LogOut(LOG_ALL, "tspid=%d", npid);

				int needstep = 1;
				int isstartn = mr->CheckStart(ps);
				int isset = 0;
				if (mr->m_vmax > 0)//mr.m_vmax == MAXVFRAME)
				{
					if (1 == isstartn){
						if (mr->m_vmax < mr->m_vl){
							isset = 1;
							//mr.m_vmax = mr.m_vl;
						}
						//needstep = 0;
					}
				}
				if (mr->m_amax > 0)//if (mr.m_amax == 0x10000)
				{
					if (2 == isstartn){
						mr->m_amax = mr->m_al;
						//needstep = 0;
					}
				}
				if (isstartn == 1){
					if (mr->m_vl != mr->m_vmax || isset == 1)
					{
						mr->m_vmax = mr->m_vl;
						if (nohp && islostrtp){
							if (mr->m_isidr == 1)
								mr->m_isidr = 0;
						}

						if (sneedidr == 0)
							sneedidr = 1;
					}
				}

				if (mr->m_vl >= mr->m_vmax && mr->m_vmax > 0)
					needstep = 0;
				if (mr->m_al >= mr->m_amax && mr->m_amax > 0)
					needstep = 0;

				if (needstep)
				{
					mr->GetTsData(ps);
					if (mr->m_isdvr == 1)
						sneedidr = 0;

					i += 188;
				}
				if (mr->m_vl >= mr->m_vmax && mr->m_vmax > 0){
#if 1
					MyBufInfo * pitem = vddatas->GetBuffer1();
					//pitem->Release();
					memcpy(pitem->m_pbdata, mr->m_vp, mr->m_vl);
					pitem->m_dwLen = mr->m_vmax;
					pitem->m_lltime = mr->m_ullvpts;
					//LogOut(LOG_ALL, "vlist ,lltime = %d", pitem->m_lltime);
					if (vlist->GetNum() > 10)
						LogOut(LOG_DEALUDPPUSH, "viedo push list  thd=%d   listnum=%d\n", thd, vlist->GetNum());
					vlist->PushTail(pitem);
					SetEvent(hv);
					api_releasebuf(pitem);
#endif
					mr->m_vl = 0;
					mr->m_vmax = 0;

				}
				if (mr->m_al >= mr->m_amax && mr->m_amax > 0){
#if 1
					MyBufInfo * pitem = addatas->GetBuffer1();
					memcpy(pitem->m_pbdata, mr->m_ap, mr->m_amax);
					pitem->m_dwLen = mr->m_amax;
					pitem->m_lltime = mr->m_ullapts;

					//LogOut(LOG_ALL, "alist ,lltime = %d", pitem->m_lltime);


					pitem->m_wfe = mr->m_wfe;
					pitem->m_aid = mr->m_aid;


					alist->PushTail(pitem);
					SetEvent(ha);
					api_releasebuf(pitem);
#endif
					mr->m_al = 0;
					mr->m_amax = 0;
				}
			}

			api_releasebuf(pitemrtps);
		}
		else{
			Sleep(1);
			WaitForSingleObject(hrtps, 1);
		}
	}
}

/////////////////

void api_stopworkex(WorkThreadEx * pe, DWORD d0, DWORD d1){
	if (pe){
		pe->SetStop(d0, d1);
	}
}
void api_freeworkex(WorkThreadEx * &pe){
	if (pe){
		pe->Release();
		pe = 0;
	}
}
BOOL api_isneedwork(WorkThreadEx * pe, DWORD dwwait){
	return pe->IsNeedWork(dwwait);
}

int api_addrefbuf(MyBufInfo * pI)
{
	CMyBufferItem * pBuffer = 0;
	if (pI)
		pBuffer = (CMyBufferItem*)(pI->m_pvthis);
	if (pBuffer)
	{
		return pBuffer->AddRef();
	}
}
int api_releasebuf(MyBufInfo * pI)
{
	CMyBufferItem * pBuffer = 0;
	if (pI)
		pBuffer = (CMyBufferItem*)(pI->m_pvthis);
	if (pBuffer)
	{
		return pBuffer->Release();
	}
}






UINT Ue(BYTE *pBuff, UINT nLen, UINT &nStartBit)
{
	//计算0bit的个数
	UINT nZeroNum = 0;
	while (nStartBit < nLen * 8)
	{
		if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8))) //&:按位与，%取余
		{
			break;
		}
		nZeroNum++;
		nStartBit++;
	}
	nStartBit++;


	//计算结果
	DWORD dwRet = 0;
	for (UINT i = 0; i < nZeroNum; i++)
	{
		dwRet <<= 1;
		if (pBuff[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
		{
			dwRet += 1;
		}
		nStartBit++;
	}
	return (1 << nZeroNum) - 1 + dwRet;
}


int Se(BYTE *pBuff, UINT nLen, UINT &nStartBit)
{


	int UeVal = Ue(pBuff, nLen, nStartBit);
	double k = UeVal;
	int nValue = ceil(k / 2);//ceil函数：ceil函数的作用是求不小于给定实数的最小整数。ceil(2)=ceil(1.2)=cei(1.5)=2.00
	if (UeVal % 2 == 0)
		nValue = -nValue;
	return nValue;


}


DWORD u(UINT BitCount, BYTE * buf, UINT &nStartBit)
{
	DWORD dwRet = 0;
	for (UINT i = 0; i < BitCount; i++)
	{
		dwRet <<= 1;

		if (buf[nStartBit / 8] & (0x80 >> (nStartBit % 8)))
		{
			dwRet += 1;
		}
		nStartBit++;
	}
	return dwRet;
}


bool h264_decode_seq_parameter_set(BYTE * buf, UINT nLen, int &Width, int &Height)
{
	UINT StartBit = 0;
	int forbidden_zero_bit = u(1, buf, StartBit);
	int nal_ref_idc = u(2, buf, StartBit);
	int nal_unit_type = u(5, buf, StartBit);
	if (nal_unit_type == 7)
	{
		int profile_idc = u(8, buf, StartBit);
		int constraint_set0_flag = u(1, buf, StartBit);//(buf[1] & 0x80)>>7;
		int constraint_set1_flag = u(1, buf, StartBit);//(buf[1] & 0x40)>>6;
		int constraint_set2_flag = u(1, buf, StartBit);//(buf[1] & 0x20)>>5;
		int constraint_set3_flag = u(1, buf, StartBit);//(buf[1] & 0x10)>>4;
		int reserved_zero_4bits = u(4, buf, StartBit);
		int level_idc = u(8, buf, StartBit);

		int seq_parameter_set_id = Ue(buf, nLen, StartBit);

		if (profile_idc == 100 || profile_idc == 110 ||
			profile_idc == 122 || profile_idc == 144)
		{
			int chroma_format_idc = Ue(buf, nLen, StartBit);
			if (chroma_format_idc == 3)
				int residual_colour_transform_flag = u(1, buf, StartBit);
			int bit_depth_luma_minus8 = Ue(buf, nLen, StartBit);
			int bit_depth_chroma_minus8 = Ue(buf, nLen, StartBit);
			int qpprime_y_zero_transform_bypass_flag = u(1, buf, StartBit);
			int seq_scaling_matrix_present_flag = u(1, buf, StartBit);

			int seq_scaling_list_present_flag[8];
			if (seq_scaling_matrix_present_flag)
			{
				for (int i = 0; i < 8; i++) {
					seq_scaling_list_present_flag[i] = u(1, buf, StartBit);
				}
			}
		}
		int log2_max_frame_num_minus4 = Ue(buf, nLen, StartBit);
		int pic_order_cnt_type = Ue(buf, nLen, StartBit);
		if (pic_order_cnt_type == 0)
			int log2_max_pic_order_cnt_lsb_minus4 = Ue(buf, nLen, StartBit);
		else if (pic_order_cnt_type == 1)
		{
			int delta_pic_order_always_zero_flag = u(1, buf, StartBit);
			int offset_for_non_ref_pic = Se(buf, nLen, StartBit);
			int offset_for_top_to_bottom_field = Se(buf, nLen, StartBit);
			int num_ref_frames_in_pic_order_cnt_cycle = Ue(buf, nLen, StartBit);

			int *offset_for_ref_frame = new int[num_ref_frames_in_pic_order_cnt_cycle];
			for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
				offset_for_ref_frame[i] = Se(buf, nLen, StartBit);
			delete[] offset_for_ref_frame;
		}
		int num_ref_frames = Ue(buf, nLen, StartBit);
		int gaps_in_frame_num_value_allowed_flag = u(1, buf, StartBit);
		int pic_width_in_mbs_minus1 = Ue(buf, nLen, StartBit);
		int pic_height_in_map_units_minus1 = Ue(buf, nLen, StartBit);

		Width = (pic_width_in_mbs_minus1 + 1) * 16;
		Height = (pic_height_in_map_units_minus1 + 1) * 16;

		return true;
	}
	else
		return false;
}




void OnTest()
{
	//数据必须把H264的头0x000001去掉


	BYTE bytes[11] = { 0x27, 0x42, 0x40, 0x29, 0x95, 0xa0, 0x14, 0x00, 0x81, 0xb0, 0x11 };
	//BYTE bytes[11] = { 0x67, 0x64, 0x08, 0x1F, 0xAC, 0x34, 0xC1, 0x08, 0x28, 0x0F, 0x64 };
	UINT startbit = 0;
	int Width, Height;
	h264_decode_seq_parameter_set(bytes, 11, Width, Height);

	Width = Width;

}



LONGLONG MyGetCurTime()
{
	LONGLONG llt;
	SYSTEMTIME st;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, (FILETIME*)(&llt));
	return llt;
}


CMyh264::CMyh264()
{
	m_P3D = NULL;
	m_myh264D = NULL;
	m_myaacD = NULL;
	m_pdf = NULL;

	m_isback = 1;
	m_isbackold = -1;
	m_isbackSize = 0;
	m_pdata = NULL;

	m_nw = 1920;
	m_nh = 1080;

	// 	m_pYUYV_Float = NULL;
	// 	m_pYUYV_Int = NULL;
	// 	m_pYUYV_RGBTable = NULL;
	// 	m_pYUYV_Table = NULL;
	// 	m_pYUYV_TableEx = NULL;
	// 	m_pYUYV_Common = NULL;
	//m_mr = new MyRTPINFO;



}

CMyh264::~CMyh264()
{

	if (m_pdf)
	{
		delete m_pdf;
		m_pdf = NULL;
	}


	if (m_hDllED_coder)
	{
		FreeLibrary(m_hDllED_coder);
		m_hDllED_coder = 0;
	}
	if (m_hDllED_coderD)
	{
		FreeLibrary(m_hDllED_coderD);
		m_hDllED_coderD = 0;
	}
	if (m_hDllaacd)
	{
		FreeLibrary(m_hDllaacd);
		m_hDllaacd = 0;
	}

}
//错误	1	error C4996 : '_splitpath' : This function or variable may be unsafe.Consider using _splitpath_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.See online help for details.D : \work\WifiDirectN\src\wfdtest\myh264.cpp	24	1	wfdtest




void CMyh264::MyCreateMyH264D()
{
	if (m_hDllED_coderD && m_myh264D == NULL)
	{
		MyAPI_CreateMyH264D pCreateInsD = (MyAPI_CreateMyH264D)GetProcAddress(m_hDllED_coderD, "API_CreateMyH264D");
		if (pCreateInsD)
			m_myh264D = pCreateInsD(50);
	}
}

void CMyh264::MyCreateMyAACD()
{
	if (m_hDllaacd  &&  m_myaacD == NULL)
	{
		MyAPI_CreateMyAACD pCreateInsD = (MyAPI_CreateMyAACD)GetProcAddress(m_hDllaacd, "API_CreateMyAACD");
		if (pCreateInsD)
			m_myaacD = pCreateInsD(1);
	}
}

void CMyh264::InitAadio()
{
	MyCreateMyAACD();
	if (m_myaacD)
	{
		HRESULT hr = m_myaacD->Init();
		if (hr == S_OK)
		{
			OutputDebugString("m_myaacD init\n");
		}
	}

}

void CMyh264::ExitAadio()
{
	if (m_myaacD)
	{
		HRESULT hr = m_myaacD->Exit();
		if (hr == S_OK)
		{
			m_myaacD = NULL;
			OutputDebugString("m_myaacD = NULL\n");
		}
	}
}






void CMyh264::Init264Dll()
{

	CHAR	cModulePath[MAX_PATH] = { 0 };
	CHAR	cDrive[MAX_PATH] = { 0 };
	CHAR	cDir[MAX_PATH] = { 0 };
	CHAR	cED_coder[MAX_PATH] = { 0 };
	CHAR	cED_coderd[MAX_PATH] = { 0 };
	CHAR	cED_aacd[MAX_PATH] = { 0 };
	CHAR	cED_YUV2RGB[MAX_PATH] = { 0 };



	GetModuleFileNameA(0, cModulePath, MAX_PATH);
	_splitpath(cModulePath, cDrive, cDir, NULL, NULL);
	_makepath(cModulePath, cDrive, cDir, NULL, NULL);
	_makepath(cED_coder, cDrive, cDir, "myh264.dll", NULL);
	_makepath(cED_coderd, cDrive, cDir, "myh264d.dll", NULL);
	_makepath(cED_aacd, cDrive, cDir, "myaacd.dll", NULL);


	CoInitialize(NULL);


	m_hDllED_coder = LoadLibraryA(cED_coder);
	//int nn = GetLastError();
	//动态库   不是有效的win32 程序

	m_hDllED_coderD = LoadLibraryA(cED_coderd);

	m_hDllaacd = LoadLibraryA(cED_aacd);

	MyCreateMyH264D();
	MyCreateMyAACD();



	m_pdf = new CDrawFrame;
	m_pout1 = new unsigned char[1920 * 1200 * 4];
	m_pout2 = new unsigned char[1920 * 1200 * 4];
	m_pdata = new char[1920 * 1200 * 4];
	memset(m_pdata, 0x80, 1920 * 1200 * 4);
	m_lltime2 = 0;
	m_lltime1 = 0;
	m_lltime = 0;
	m_lldur = 33333333;

	m_dwoutmax = 1920 * 1080 * 4;



	m_dwout = 0;

	//m_mr;
	m_lltime0 = -99999999999;
	m_llstart = GetTickCount64();
	m_llcur = -99999999999;
	m_llOff = 0;

	memset(&m_mr, 0, sizeof(m_mr));

	m_nw = 1024;
	m_nh = 720;
	//m_myh264D->SetInfo(m_nw, m_nh, 33333333);
}

BOOL CMyh264::SetDecodeWHF(int nw, int nh, int frames)
{

	m_nw = nw;
	m_nh = nh;// (((nh + 15) >> 4) << 4);

	m_lldur = 100000000 / frames;
	m_myh264D->SetInfo(m_nw, m_nh, (int)m_lldur);
	return 0;
}

BOOL  CMyh264::PushInDataA(char*pbuff, int nsize, LONGLONG llTime, LONGLONG * plldtime, WAVEFORMATEX * pfe, int islpcm)
{
	HRESULT hr = S_FALSE;
	BOOL ret = FALSE;

	if (m_myaacD){
		m_myaacD->SetInfo(pfe, islpcm);
		hr = m_myaacD->PushInData((LPBYTE)pbuff, nsize, llTime, plldtime);
	}
	if (hr == S_OK)
		ret = TRUE;
	return ret;
}
BOOL  CMyh264::PushInDataV(char*pbuff, int nsize, LONGLONG llTime)
{

	HRESULT hr = S_FALSE;
	if (m_myh264D){
		hr = m_myh264D->PushInData((LPBYTE)pbuff, nsize, llTime);

	}
	return hr;
}

BOOL  CMyh264::h264Decode(char*pbuff, int nsize)
{
	int nstep = nsize / 188;
	for (int i = 0; i < nstep; i++)
	{
		m_mr.GetTsData((unsigned char *)pbuff);
		pbuff += 188;
		if (m_mr.m_nbv){
			if (m_lltime0 == -99999999999 && m_mr.m_ullvpts != -99999999999)
			{
				m_lltime0 = m_mr.m_ullvpts;
			}
			//m_myh264D->PushInData(pbts, 188, lltime1);
			if (m_mr.m_vmax > 0xffff)
			{
				m_mr.m_vmax = 0;
				m_mr.m_vl = 0;
			}
			if (m_mr.m_vl >= m_mr.m_vmax && m_mr.m_vmax > 0)
			{
				//Sleep(5);
				HRESULT hr = S_FALSE;
				while (hr != S_OK)
				{
					hr = m_myh264D->PushInData(m_mr.m_vp, m_mr.m_vl, m_mr.m_ullvpts);

					if (hr != S_OK)
						hr = hr;
				}

				m_mr.m_vl = 0;
				m_mr.m_vmax = 0;
			}
		}
		//if (m_mr.m_nba){
		//}
	}
	return TRUE;
}



//#define  OLD_BLACKRECT
#ifdef OLD_BLACKRECT

int  isNotBlackRect(unsigned char* pdata, int nw, int nh, int npitch)
{
	int nnn[10] = { nw, nw, nw, nw, nw, nw, nw, nw, nw, nw };
	int nk = 0;


	int nminlen = nw;
	int j, i;
	for (i = 0; i < nh; i++)
	{
		LPBYTE Src = (LPBYTE)pdata;
		for (j = 0; j < nw / 2; j++)
		{
			if (Src[j] > 0x16 || Src[nw - 1 - j] > 0x16)
				break;
		}

		if (j < nminlen)
			nminlen = j;

		for (int tt = 0; tt < 10; tt++){
			if (j < nnn[tt]){
				int kk = j;
				j = nnn[tt];
				nnn[tt] = kk;
				if (nk < tt + 1)
					nk = tt + 1;
			}
		}

		if (j < nminlen)
			nminlen = j;
		pdata += npitch;
	}

	if (nk > 5){
		int nrt = 0;
		for (int tt = 0; tt < nk; tt++){
			nrt += nnn[tt];
		}
		nrt /= nk;
		return nrt;
	}
	else
		return nw;

	return nminlen;
}

#else 


int  isNotBlackRect(unsigned char* pdata, int nw, int nh, int npitch, int & ssnminlen)
{
	int minj = nw / 4;
	int maxj = nw * 2 / 5;

	int * nlens = new int[nh];
	int * njs = new int[nw];
	memset(nlens, 0, nh * sizeof(int));
	memset(njs, 0, nw * sizeof(int));
	int nallj = 0;
	for (int i = 0; i < nh; i++)
	{
		int j = 0;
		for (j = 0; j < nw / 2; j++)
		{
			if (pdata[j] > 0x16 || pdata[nw - 1 - j] > 0x16)
				break;
		}
		nallj += j;
		nlens[i] = j;
		njs[j]++;
		pdata += npitch;
	}
	nallj /= nh;
	if (nallj <= minj || nallj >= maxj){
		return nallj;
	}
	if (ssnminlen > minj && ssnminlen < maxj){
		//if (nallj + 10 < ssnminlen + 10)
		if (nallj + 10 < ssnminlen)
		{
			return minj - 1;//
		}
		else
			return ssnminlen;
	}
	int nummin = 0;
	int nallmin = 0;
	for (int i = 0; i<nw; i++)
	{
		nummin += njs[i];
		nallmin += (i * njs[i]);
		if (nummin > 10)
			break;
	}
	nallmin /= nummin;
	if (nallmin <= minj || nallmin >= maxj){
		return nallmin;
	}
	ssnminlen = nallmin;
	return ssnminlen;
}


#endif 


int getget(void * pv, int nsize){
	int k = 0;
	nsize = 1920 * 1080;
	unsigned char * pb = (unsigned char *)pv;
	for (int i = 0; i < nsize; i++){
		if (pb[i] > 0x15){
			k++;
		}
	}
	if (k > 100)
		k = k;
	return k;
}

char *  CMyh264::ColorChange(char * pin, int nw, int nh)
{

	return m_pdata;
}
BOOL  CMyh264::GetDecodeV()
{
	DWORD dwout = 0;
	m_myh264D->GetOutData(m_pout2, dwout, m_lltime2);
	if (dwout)
	{
		static int nstep = 0;
		nstep++;
		static LONGLONG lasttime = 0;
		LONGLONG nowtime = MyGetCurTime();
		if (nowtime - lasttime > 500)
		{
			lasttime = nowtime;
			//判断 横屏 和 竖屏
#ifdef OLD_BLACKRECT  
			int getbacksize = isNotBlackRect((unsigned char*)m_pout2, m_nw, m_nh, m_nw);

			m_isbackSize = getbacksize;
			if (getbacksize < (m_nw >> 2))
				m_isback = 0; //横屏
			else if (getbacksize < ((m_nw >> 2) << 1)){

				m_isback = 1; //竖屏
			}
			else
			{

			}

#else 

			static int  s_blackSize = 0;

			int getbacksize = isNotBlackRect((unsigned char*)m_pout2, m_nw, m_nh, m_nw, s_blackSize);


			//LogOut(LOG_ALL, "s_blackSize = %d ,getbacksize=%d\n", s_blackSize, getbacksize);

			m_isbackSize = s_blackSize;
			if (getbacksize < (m_nw * 3 / 10))
				m_isback = 0; //横屏
			else if (getbacksize < (m_nw * 2 / 5))
			{
				m_isback = 1; //竖屏
			}
			else
			{

			}


#endif

		}

		m_datasize = m_nw * m_nh * 4;
		m_llcur = (m_lltime2 - m_lltime0) / 1000 + m_llstart;


		static int si = 0;
		if (si == 1){
			si = 0;
			//return FALSE;
		}
		else
			si++;


		return TRUE;
	}
	return FALSE;
}



void    CMyh264::MyCreateP3d(HWND hwnd)
{
	if (m_P3D == NULL)
		m_P3D = CreateFpD3D(hwnd);
}

void		CMyh264::MyRenderData(HWND hWindow, long x, long y, LPSIZE pSizeS, int isactive, RECT* rcSrc)
{
	m_P3D->RenderData(hWindow, x, y, pSizeS, isactive, rcSrc);
}
void		CMyh264::MyOnRefreshData(void * pvData, long DataMode, long lDataLen, LONG nw, LONG nh)
{
	m_P3D->OnRefreshData(pvData, DataMode, lDataLen, nw, nh);
}





#pragma comment(lib, "d3d9.lib")


#define D3DFVF_FPCUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
struct FPCUSTOMVERTEX
{
	FLOAT       x, y, z;
	DWORD		color;    // The color
	FLOAT       tu, tv;   // The texture coordinates
};
class Fp3d
	:public CDDUnknown<IFp3d>
{
public:
	Fp3d()
	{
		m_bDeviceLost = FALSE;
		m_pD3D = NULL;
		m_pd3dDevice = NULL;
		m_pBufffer = NULL;
		m_pTexture = NULL;
		m_pyv12 = NULL;
		ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
		m_dwColorBk = 0xff000000;
		m_w = 0;
		m_h = 0;
		m_nDibNum = 0;
		MatrixIdentity(&m_matInit);


		FPCUSTOMVERTEX	Vertices[4] =
		{
			{ -1.0f, -1.0f, 0.0f, 0xffffffff, 0.0f, 1.0f },
			{ -1.0f, 1.0f, 0.0f, 0xffffffff, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 0.0f, 0xffffffff, 1.0f, 0.0f },
			{ 1.0f, -1.0f, 0.0f, 0xffffffff, 1.0f, 1.0f },
		};
		memcpy(m_Vertices, Vertices, sizeof(Vertices));
	}
	~Fp3d()
	{
		Cleanup(TRUE);
	}


	ARCAPI			RenderData(HWND hWnd, long x, long y, LPSIZE pSizeS, int isactive, RECT* rcSrc);
	ARCAPI			OnRefreshData(void * pvData, long DataMode, long lDataLen, LONG nw, LONG nh);


	HRESULT DoforLost()
	{
		HRESULT hr = E_FAIL;
		if (!m_pd3dDevice)
			return hr;
		if (m_d3dpp.BackBufferWidth != GetSystemMetrics(SM_CXSCREEN) ||
			m_d3dpp.BackBufferHeight != GetSystemMetrics(SM_CYSCREEN) ||
			m_bDeviceLost)
		{
			SafeReleaseP(m_pTexture);
			SafeReleaseP(m_pBufffer);
			SafeReleaseP(m_pyv12);
			hr = m_pd3dDevice->TestCooperativeLevel();
			if (hr == D3DERR_DEVICELOST)
			{
				SafeReleaseP(m_pTexture);
				SafeReleaseP(m_pBufffer);
				SafeReleaseP(m_pyv12);
			}
			else if (hr == D3DERR_DEVICENOTRESET)
			{
				SafeReleaseP(m_pTexture);
				SafeReleaseP(m_pBufffer);
				SafeReleaseP(m_pyv12);
				hr = ResetD3D();
			}
			if (SUCCEEDED(hr))
			{
				//GetBackData(FALSE);
				m_bDeviceLost = FALSE;
				InitDataD3D(FALSE);
			}
		}
		else
			hr = S_OK;

		return hr;
	}

	HRESULT InitDataD3D(BOOL bCheckStretch)
	{
		HRESULT hr = E_FAIL;
		if (m_pd3dDevice)
			hr = m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		if (SUCCEEDED(hr))
			hr = m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		if (SUCCEEDED(hr))
			hr = m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		if (SUCCEEDED(hr))
		{
			if (bCheckStretch)
			{
				if (SUCCEEDED(hr))
					hr = InitBuffer(512, 512, 32);
				IDirect3DSurface9 * pRt = NULL;
				hr = m_pd3dDevice->GetRenderTarget(0, &pRt);
				if (SUCCEEDED(hr))
				{
					hr = m_pd3dDevice->StretchRect(m_pBufffer, NULL, pRt, NULL, D3DTEXF_LINEAR);
					SafeReleaseP(pRt);
				}
			}
		}

		return hr;
	}
	HRESULT InitD3D(HWND hWnd)
	{
		HRESULT hr = E_FAIL;
		m_lk.Lock();
		Cleanup(FALSE);
		ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
		m_d3dpp.Windowed = TRUE;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;



		m_d3dpp.EnableAutoDepthStencil = FALSE;
		//m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


		m_d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
		m_d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
		m_d3dpp.BackBufferCount = 1;


		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (m_pD3D)
			hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice);

		hr = InitDataD3D(TRUE);
		if (FAILED(hr))
			Cleanup(FALSE);

		m_bDeviceLost = FALSE;
		m_lk.Unlock();



		return hr;
	}
	VOID Cleanup(BOOL bLock)
	{
		if (bLock)
			m_lk.Lock();
		SafeReleaseP(m_pTexture);
		SafeReleaseP(m_pBufffer);
		SafeReleaseP(m_pyv12);
		SafeReleaseP(m_pd3dDevice);
		SafeReleaseP(m_pD3D);
		if (bLock)
			m_lk.Unlock();
	}

	HRESULT ResetD3D()
	{
		SafeReleaseP(m_pTexture);
		SafeReleaseP(m_pBufffer);
		SafeReleaseP(m_pyv12);

		m_d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
		m_d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
		HRESULT hr = m_pd3dDevice->Reset(&m_d3dpp);
		return hr;
	}




	HRESULT InitBuffer(LONG w, LONG h, LONG nDibNum)
	{
		HRESULT	hr = E_FAIL;
		if (m_pd3dDevice)
		{
			if (m_w != w || m_h != h || nDibNum != m_nDibNum)
			{
				SafeReleaseP(m_pTexture);
				SafeReleaseP(m_pBufffer);
				SafeReleaseP(m_pyv12);
				m_w = w;
				m_h = h;
				m_nDibNum = nDibNum;
			}
			hr = S_OK;
#ifdef USETEXTURE
			if (!m_pTexture)
			{
				if (m_nDibNum == 32)
				{
					hr = m_pd3dDevice->CreateTexture(m_w, m_h,
						1, D3DUSAGE_DYNAMIC,
						D3DFMT_X8R8G8B8,
						D3DPOOL_MANAGED,
						&m_pTexture, NULL);
				}
			}

#endif
			if (!m_pyv12)
			{
				hr = m_pd3dDevice->CreateOffscreenPlainSurface(
					m_w, m_h,
					(D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2'),
					D3DPOOL_DEFAULT,
					&m_pyv12, NULL);
			}
			if (!m_pTexture)
			if (!m_pBufffer)
			{
				if (m_nDibNum == 32)
				{
					hr = m_pd3dDevice->CreateRenderTarget(m_w, m_h,
						D3DFMT_X8R8G8B8,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&m_pBufffer, NULL);
				}
				else if (m_nDibNum == 16)
				{
					hr = m_pd3dDevice->CreateRenderTarget(m_w, m_h,
						D3DFMT_X1R5G5B5,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&m_pBufffer, NULL);
				}
				if (m_pBufffer)
					hr = m_pd3dDevice->ColorFill(m_pBufffer, NULL, m_dwColorBk);
			}
			//m_pd3dDevice->CreateTexture(1024,768,0,
			//	D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
			//	&m_pTexture,NULL);
		}
		return hr;
	}


	HRESULT Refresh(RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
	{
		m_lk.Lock();
		HRESULT hr = DoforLost();
		if (SUCCEEDED(hr))
			hr = m_pd3dDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		m_lk.Unlock();
		return hr;
	}


	HRESULT Render(RECT* pSourceRect, CONST RECT* pDestRect0, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, int isactive)
	{
		m_lk.Lock();
		HRESULT hr = DoforLost();
		RECT rt;
		LPRECT  pDestRect = NULL;
		if (pDestRect0)
		{
			rt = *pDestRect0;
			pDestRect = &rt;
			//使下面修改不会改掉输入值
		}


		if (SUCCEEDED(hr))
		{
			//hr =m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
			hr = m_pd3dDevice->BeginScene();
			if (SUCCEEDED(hr))
			{
				if (m_pTexture)
				{
					//RECT rtc;
					m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
					m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
					m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

					m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matInit);
					m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matInit);
					m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matInit);
					m_pd3dDevice->SetTexture(0, m_pTexture);
					m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
					m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
					m_pd3dDevice->SetFVF(D3DFVF_FPCUSTOMVERTEX);
					m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_Vertices, sizeof(FPCUSTOMVERTEX));
				}
				else if (m_pBufffer || m_pyv12)
				{
					IDirect3DSurface9 * pRt = NULL;
					hr = m_pd3dDevice->GetRenderTarget(0, &pRt);
					if (pRt)
					{
						D3DSURFACE_DESC decD;
						pRt->GetDesc(&decD);
						if (pDestRect)
						{
							if (pDestRect->left < 0)
								pDestRect->left = 0;
							if (pDestRect->top < 0)
								pDestRect->top = 0;
							if (pDestRect->right > decD.Width)
								pDestRect->right = decD.Width;
							if (pDestRect->bottom > decD.Height)
								pDestRect->bottom = decD.Height;
						}
						RECT rtDD;
						RECT rtD;
						if (hDestWindowOverride)
						{
							//HWND hwout = GetParent(hDestWindowOverride);
							//if (hwout == 0)hwout = hDestWindowOverride;
							//GetWindowRect(hDestWindowOverride, &rtDD);
							GetClientRect(hDestWindowOverride, &rtD);
						}
						else{
							rtD.left = 0;
							rtD.top = 0;
							rtD.right = decD.Width;
							rtD.bottom = decD.Height;
						}

						//hr = m_pd3dDevice->ColorFill(m_pyv12, NULL, D3DCOLOR_XRGB(255, 255, 0));


						if (m_nDibNum == 12){
							hr = m_pd3dDevice->StretchRect(m_pyv12, pSourceRect, pRt, pDestRect, D3DTEXF_LINEAR);
						}
						else
							hr = m_pd3dDevice->StretchRect(m_pBufffer, pSourceRect, pRt, pDestRect, D3DTEXF_LINEAR);


						//						hr = m_pd3dDevice->StretchRect(m_pBufffer,NULL,pRt,pDestRect,D3DTEXF_LINEAR);
						if (rtD.right == decD.Width && rtD.bottom == decD.Height)
							isactive = 0;
						if (isactive){
							int nn = 6;
							RECT rt0 = { rtD.left, rtD.top, rtD.right, nn };
							RECT rt1 = { rtD.left, rtD.bottom - nn, rtD.right, rtD.bottom };

							RECT rt2 = { rtD.left, rtD.top, nn, rtD.bottom };
							RECT rt3 = { rtD.right - nn, rtD.top, rtD.right, rtD.bottom };
							m_pd3dDevice->ColorFill(pRt, &rt0, D3DCOLOR_XRGB(255, 255, 0));
							m_pd3dDevice->ColorFill(pRt, &rt1, D3DCOLOR_XRGB(255, 255, 0));
							m_pd3dDevice->ColorFill(pRt, &rt2, D3DCOLOR_XRGB(255, 255, 0));
							m_pd3dDevice->ColorFill(pRt, &rt3, D3DCOLOR_XRGB(255, 255, 0));
						}
						pRt->Release();
					}

				}
				hr = m_pd3dDevice->EndScene();
			}
			if (SUCCEEDED(hr))
			{
				hr = m_pd3dDevice->Present(pDestRect, pDestRect, hDestWindowOverride, pDirtyRegion);
			}
		}
		m_lk.Unlock();
		if (D3DERR_DEVICELOST == hr)
			m_bDeviceLost = true;
		return hr;
	}

private:
	void MatrixIdentity(D3DMATRIX *pOut)
	{
		pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
			pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
			pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
			pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
		pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
	}


	BOOL					m_bDeviceLost;

	D3DMATRIX m_matInit;
	LPDIRECT3D9             m_pD3D;
	D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3DDEVICE9       m_pd3dDevice;
	IDirect3DSurface9	*   m_pBufffer;
	IDirect3DTexture9	*	m_pTexture;

	IDirect3DSurface9 *m_pyv12;

	FPCUSTOMVERTEX	m_Vertices[4];



	LONG	m_w;
	LONG	m_h;
	LONG	m_nDibNum;
	DWORD   m_dwColorBk;
	MyLock	m_lk;
};





IFp3d * CreateFpD3D(HWND hWnd)
{
	HRESULT hr = E_FAIL;
	Fp3d * pD3D = new Fp3d();
	if (pD3D)
	{
		hr = pD3D->InitD3D(hWnd);
		if (FAILED(hr))
			SafeReleaseP(pD3D);
	}
	return pD3D;
}

//////////////////////////////////////////////////////////
HRESULT	Fp3d::RenderData(HWND hWnd, long x, long y, LPSIZE pSizeS, int isactive, RECT* rcSrc)
{
	if (pSizeS)
	{
		RECT rt = { x, y, pSizeS->cx + x, pSizeS->cy + y };

		if (rcSrc)
			Render(rcSrc, &rt, hWnd, NULL, isactive);
		else
			Render(NULL, &rt, hWnd, NULL, isactive);
	}
	else
	{
		Render(NULL, NULL, hWnd, NULL, isactive);
	}

	return S_OK;
}
HRESULT	Fp3d::OnRefreshData(void * pvData, long DataMode, long pitchSrc, LONG nw, LONG nh)
{
	m_lk.Lock();
	HRESULT hr = S_OK;
	if (pvData == 0)
	{
		nw = m_w;
		nh = m_h;
		DataMode = m_nDibNum;
	}
	else
		hr = InitBuffer(nw, nh, DataMode);


	if (SUCCEEDED(hr))
	{
		hr = E_FAIL;
		D3DLOCKED_RECT d3dRt = { 0, 0 };
		if (DataMode == 12)
		{

			hr = m_pyv12->LockRect(&d3dRt, NULL, 0);

			LPBYTE pSrc = (LPBYTE)pvData;
			LPBYTE pDst = (LPBYTE)(d3dRt.pBits);
			long npitch = d3dRt.Pitch;
			long minLen = min(npitch, pitchSrc);
			long otherLen = npitch - minLen;


			if (pSrc)
			{
				for (int i = 0; i < nh; i++)
				{
					memcpy(pDst, pSrc, minLen);
					pDst += npitch;
					pSrc += pitchSrc;
				}
				for (int i = 0; i < nh / 2; i++)
				{
					memcpy(pDst, pSrc, minLen / 2);
					pDst += npitch / 2;
					pSrc += pitchSrc / 2;
				}
				for (int i = 0; i < nh / 2; i++)
				{
					memcpy(pDst, pSrc, minLen / 2);
					pDst += npitch / 2;
					pSrc += pitchSrc / 2;
				}
			}
			else
			{
				for (int i = 0; i < nh; i++)
				{
					memset(pDst, 0, npitch);
					pDst += npitch;
				}
				for (int i = 0; i < nh / 2; i++)
				{
					memset(pDst, 0x80, npitch / 2);
					pDst += npitch / 2;
				}
				for (int i = 0; i < nh / 2; i++)
				{
					memset(pDst, 0x80, npitch / 2);
					pDst += npitch / 2;
				}
			}





			m_pyv12->UnlockRect();

		}
		else{
			if (m_pTexture)
			{
				hr = m_pTexture->LockRect(0, &d3dRt, NULL, 0);
			}
			else if (m_pBufffer)
				hr = m_pBufffer->LockRect(&d3dRt, NULL, 0);
			if (SUCCEEDED(hr))
			{
				LPBYTE pSrc = (LPBYTE)pvData;
				LPBYTE pDst = (LPBYTE)(d3dRt.pBits);
				long npitch = d3dRt.Pitch;
				long minLen = min(npitch, pitchSrc);
				long otherLen = npitch - minLen;


				pDst += (nh - 1) * npitch;
				long anpitch = -npitch;


				if (pSrc)
				{
					for (int i = 0; i < nh; i++)
					{
						memcpy(pDst, pSrc, minLen);
						pDst += anpitch;
						pSrc += pitchSrc;
					}
				}
				else
				{
					for (int i = 0; i < nh; i++)
					{
						memset(pDst, 0, npitch);
						pDst += anpitch;
					}
				}
				if (m_pTexture)
					m_pTexture->UnlockRect(0);
				else if (m_pBufffer)
					m_pBufffer->UnlockRect();
			}
		}
	}
	m_lk.Unlock();
	return hr;
}

MyLK::MyLK(){
#ifdef USEWINDOW
	InitializeCriticalSection(&lk_mtx);
#else
#endif
}
MyLK::~MyLK()
{
#ifdef USEWINDOW
	DeleteCriticalSection(&lk_mtx);
#else
#endif
}
BOOL MyLK::lk(){
#ifdef USEWINDOW
	EnterCriticalSection(&lk_mtx);
#else
#endif
	return TRUE;
}
void MyLK::ulk(){
#ifdef USEWINDOW
	LeaveCriticalSection(&lk_mtx);
#else
#endif
}

MyArray::MyArray()
{
	m_pFirst = 0;
	m_pLast = 0;
	m_dwNum = 0;

}

MyArray::~MyArray()
{
	ClearArray();
}
/*
MyArrayData *	MyArray::PopIndex(INT nIndex,BOOL bLock)
{
if(nIndex >= (INT)m_dwNum)
return NULL;

INT nI = 0;
MyArrayData * pSB = 0;
_MYLOCKITN(pSB)
if(nIndex == (INT)(m_dwNum-1))
pSB = PopTail(FALSE);
else if(nIndex == 0)
pSB = PopHead(FALSE);
else
{
pSB = m_pFirst;
while(pSB)
{
if(nI== nIndex)
{
MyArrayData * pSB0 = pSB->GetPrev();
MyArrayData * pSB1 = pSB->GetNext();
if(pSB0)
pSB0->SetNext(pSB1);
if(pSB1)
pSB1->SetPrev(pSB0);
m_dwNum --;
break;
}
pSB = pSB->GetNext();
}
}
_MYUNLOCKIT;
return pSB;

}
*/
/*
MyArrayData *	MyArray::PopDataN(MyArrayData * pTo,BOOL bLock)
{
MyArrayData * pSB = NULL;
if(pTo){
_MYLOCKITN(pSB)
if(m_pLast == pTo)
pSB = PopTail(FALSE);
else if(m_pFirst == pTo)
pSB = PopHead(FALSE);
else
{
pSB = pTo;
MyArrayData * pSB0 = pSB->GetPrev();
MyArrayData * pSB1 = pSB->GetNext();
if(pSB0)
pSB0->SetNext(pSB1);
if(pSB1)
pSB1->SetPrev(pSB0);
pSB->SetPrev(NULL);
pSB->SetNext(NULL);
m_dwNum --;
}
_MYUNLOCKIT;
}
return pSB;
}
*/

MyArrayData *	MyArray::PopDataN(MyArrayData * pTo, BOOL bLock)
{
	MyArrayData * pSB = 0;
	_MYLOCKITN(pSB)
	if (m_pLast == pTo)
		pSB = PopTail1(FALSE);
	else if (m_pFirst == pTo)
		pSB = PopHead(FALSE);
	else
	{
		pSB = pTo;//m_pFirst;
		//while(pSB)
		{
			//if(pSB == pTo && pSB)
			{
				MyArrayData * pSB0 = pSB->GetPrev();
				MyArrayData * pSB1 = pSB->GetNext();
				if (pSB0)
					pSB0->SetNext(pSB1);
				if (pSB1)
					pSB1->SetPrev(pSB0);
				m_dwNum--;
				//break;
			}
			//pSB = pSB->GetNext();
		}
	}
	_MYUNLOCKIT;
	return pSB;

}

MyArrayData *	MyArray::PopData(MyArrayData * pTo, BOOL bLock)
{
	MyArrayData * pSB = 0;
	_MYLOCKITN(pSB)
	if (m_pLast == pTo)
		pSB = PopTail1(FALSE);
	else if (m_pFirst == pTo)
		pSB = PopHead(FALSE);
	else
	{
		pSB = m_pFirst;
		while (pSB)
		{
			if (pSB == pTo && pSB)
			{
				MyArrayData * pSB0 = pSB->GetPrev();
				MyArrayData * pSB1 = pSB->GetNext();
				if (pSB0)
					pSB0->SetNext(pSB1);
				if (pSB1)
					pSB1->SetPrev(pSB0);
				m_dwNum--;
				break;
			}
			pSB = pSB->GetNext();
		}
	}
	check();
	_MYUNLOCKIT;
	return pSB;

}

MyArrayData *	MyArray::PopTail1(BOOL bLock)
{
	MyArrayData * pSB = 0;
	_MYLOCKITN(pSB)
	if (m_pFirst && m_pLast)
	{
		pSB = m_pLast;
		m_pLast = m_pLast->GetPrev();
		if (!m_pLast){
			m_dwNum = 0;
			m_pLast = m_pFirst = 0;
		}
		else{
			m_dwNum--;
			m_pLast->SetNext(0);
		}
	}
	else{
		m_pFirst = m_pLast = 0;
		m_dwNum = 0;
	}
	_MYUNLOCKIT;
	return pSB;

}

LONG MyArray::check(){
	DWORD i = 0;
	MyArrayData * pSB = m_pFirst;
	for (; i < m_dwNum; i++){
		if (pSB != m_pLast){
			pSB = pSB->GetNext();
		}
		else{
			pSB = pSB;
			i++;
			break;
		}
	}
	if (i == 0 && m_dwNum == 0)
	{
		if (m_pLast == NULL && m_pFirst == NULL)
			return 1;
		else
			return 0;

	}
	else{
		if (pSB == m_pLast && i == m_dwNum)
			return 1;
		else
			return 0;
	}
}


MyArrayData *	MyArray::PopHead(BOOL bLock)
{
	MyArrayData * pSB = 0;
	_MYLOCKITN(pSB)
	if (m_pFirst && m_pLast)
	{
		pSB = m_pFirst;
		m_pFirst = m_pFirst->GetNext();
		if (!m_pFirst){
			m_pFirst = m_pLast = 0;
			m_dwNum = 0;
		}
		else{
			m_pFirst->SetPrev(0);
			m_dwNum--;
		}
	}
	else{
		m_pFirst = m_pLast = 0;
		m_dwNum = 0;
	}
	check();
	_MYUNLOCKIT;
	return pSB;
}

MyArrayData *	MyArray::GetHead(BOOL bLock)
{
	MyArrayData * pSB = 0;
	_MYLOCKITN(pSB)
		pSB = m_pFirst;
	_MYUNLOCKIT;
	return pSB;
}

//MyArrayData *	MyArray::GetSub(int index,BOOL bLock)
//{
//	MyArrayData * pSB = 0;
//	_MYLOCKITN(pSB)
//	pSB = m_pFirst;
//	for(int i = 0;i<index && pSB;i++)
//	{
//		pSB = pSB->GetNext();
//	}
//	_MYUNLOCKIT;
//	return pSB;
//}
///////////////////////////////

void	MyArray::PushHead1(MyArrayData * pSB, BOOL bLock)
{
	_MYLOCKIT
		pSB->SetPrev(0);
	pSB->SetNext(m_pFirst);
	if (m_pFirst)
	{
		m_pFirst->SetPrev(pSB);
		m_pFirst = pSB;
		m_dwNum++;
	}
	else
	{
		m_pFirst = m_pLast = pSB;
		m_dwNum = 0;
	}
	_MYUNLOCKIT;
}



void	MyArray::PushTail(MyArrayData * pSB, BOOL bLock)
{
	_MYLOCKIT
		pSB->SetPrev(m_pLast);
	pSB->SetNext(0);
	if (m_pLast)
	{
		m_pLast->SetNext(pSB);
		m_pLast = pSB;
		m_dwNum++;
	}
	else
	{
		m_pFirst = m_pLast = pSB;
		m_dwNum = 1;
	}
	check();
	_MYUNLOCKIT;
}

/*
void	MyArray::Insert(MyArrayData * pSB,INT nIndex,BOOL bLock)
{
_MYLOCKIT
int i = 0;
MyArrayData * p0 = 0;
MyArrayData * p1 = m_pFirst;
for(;i<nIndex && p1;i++)
{
p0 = p1;
p1 = p1->GetNext();
}
pSB->SetPrev(p0);
pSB->SetNext(p1);
if(p0)
p0->SetNext(pSB);
else
m_pFirst = pSB;
if(p1)
p1->SetPrev(pSB);
else
m_pLast = pSB;
m_dwNum++;
_MYUNLOCKIT;
}
*/
void	MyArray::ClearArray(BOOL bLock)
{
	_MYLOCKIT
		m_pFirst = m_pLast = 0;
	m_dwNum = 0;
	_MYUNLOCKIT;
}



//MyArrayData *	MyUnArray::PopIndex(INT nIndex,BOOL bLock)
//{
//	MyArrayData *	pData = MyArray::PopIndex(nIndex,bLock);
//	//if(pData)
//	//	((CBaseUN*)pData)->AddRef();
//	return pData;
//}
//MyArrayData *	MyUnArray::PopDataN(MyArrayData * pTo,BOOL bLock)
//{
//	MyArrayData *	pData = MyArray::PopDataN(pTo,bLock);
//	//if(pData)
//	//	((CBaseUN*)pData)->AddRef();
//	return pData;
//}
//MyArrayData *	MyUnArray::PopData(MyArrayData * pTo,BOOL bLock)
//{
//	MyArrayData *	pData = MyArray::PopData(pTo,bLock);
//	//if(pData)
//	//	((CBaseUN*)pData)->AddRef();
//	return pData;
//}
//MyArrayData *	MyUnArray::PopHead(BOOL bLock)
//{
//	MyArrayData *	pData = MyArray::PopHead(bLock);
//	//if(pData)
//	//	((CBaseUN*)pData)->AddRef();
//	return pData;
//}
MyArrayData *	MyUnArray::GetHead(BOOL bLock)
{
	MyArrayData * pData = 0;
	_MYLOCKITN(pData)
		pData = MyArray::GetHead(FALSE);
	if (pData)
		((CBaseUN*)pData)->AddRef();
	_MYUNLOCKIT;
	return pData;
}
//MyArrayData *	MyUnArray::PopTail(BOOL bLock)
//{
//	MyArrayData *	pData = MyArray::PopTail(bLock);
//	//if(pData)
//	//	((CBaseUN*)pData)->AddRef();
//	return pData;
//}
//void	MyUnArray::Insert(MyArrayData * pData,INT nIndex,BOOL bLock)
//{
//	if(pData)
//	{
//		((CBaseUN*)pData)->AddRef();
//		MyArray::Insert(pData,nIndex,bLock);
//	}
//}
void	MyUnArray::PushHead1(MyArrayData * pData, BOOL bLock)
{
	if (pData)
	{
		((CBaseUN*)pData)->AddRef();
		MyArray::PushHead1(pData, bLock);
	}
}
void	MyUnArray::PushTail(MyArrayData * pData, BOOL bLock)
{
	if (pData)
	{
		((CBaseUN*)pData)->AddRef();
		MyArray::PushTail((MyArrayData *)pData, bLock);
	}
}
void	MyUnArray::ClearArray(BOOL bLock)
{
	_MYLOCKIT
		MyArrayData * pData = m_pFirst;
	while (pData)
	{
		MyArrayData * pData0 = pData->GetNext();
		((CBaseUN*)pData)->Release();
		pData = pData0;
	}
	m_pFirst = m_pLast = 0;
	m_dwNum = 0;
	_MYUNLOCKIT;
}

//MyArrayData *	MyUnArray::GetSub(int index,BOOL bLock)
//{
//	MyArrayData * pData = 0;
//	_MYLOCKITN(pData)
//		pData = MyArray::GetSub(index,FALSE);
//	if(pData)
//		((CBaseUN*)pData)->AddRef();
//	_MYUNLOCKIT;
//	return pData;
//}




MyAtomic::MyAtomic()
{
	m_n = 0;
#ifdef USEWINDOW
	InitializeCriticalSection(&m_mtx);
#else
#endif
}
MyAtomic::~MyAtomic()
{
#ifdef USEWINDOW
	DeleteCriticalSection(&m_mtx);
#else
#endif
}

void MyAtomic::Set(INT n){
	m_n = n;
}

INT MyAtomic::Inc(){
	INT nRt;
	//nRt = InterlockedIncrement(&m_n);
#ifdef USEWINDOW
	EnterCriticalSection(&m_mtx);
#else
#endif
	nRt = (++m_n);
#ifdef USEWINDOW
	LeaveCriticalSection(&m_mtx);
#else
#endif
	return nRt;
}
INT MyAtomic::Dec(){
	INT nRt;
	//nRt = InterlockedDecrement( &m_n);
#ifdef USEWINDOW
	EnterCriticalSection(&m_mtx);
#else
#endif
	nRt = (--m_n);
#ifdef USEWINDOW
	LeaveCriticalSection(&m_mtx);
#else
#endif
	return nRt;
}

// CLSID_CMSH264DecoderMFT
static GUID clsid_h264dec = { 0x62CE7E72, 0x4C71, 0x4d20, { 0xB1, 0x5D, 0x45, 0x28, 0x31, 0xA8, 0x7D, 0x9D } };
HRESULT CreateMediaSample(DWORD cbData, IMFSample **ppSample)
{
	HRESULT hr = S_OK;

	IMFSample *pSample = NULL;
	IMFMediaBuffer *pBuffer = NULL;

	hr = MFCreateSample(&pSample);

	if (SUCCEEDED(hr))
	{
		hr = MFCreateMemoryBuffer(cbData, &pBuffer);
	}

	if (SUCCEEDED(hr))
	{
		hr = pSample->AddBuffer(pBuffer);
	}

	if (SUCCEEDED(hr))
	{
		*ppSample = pSample;
		(*ppSample)->AddRef();
	}

	SafeRelease(&pSample);
	SafeRelease(&pBuffer);
	return hr;
}

HRESULT MyH264D::Init()
{
	HRESULT hr = S_OK;
	Exit();
	hr = CoCreateInstance(clsid_h264dec, NULL, CLSCTX_INPROC_SERVER, IID_IMFTransform, (void**)&m_pDecoder);
	if (SUCCEEDED(hr)){
		IMFAttributes * pat = 0;
		m_pDecoder->GetAttributes(&pat);
		if (pat){
			//m_osc = 0;
			pat->SetUINT32(MF_SA_MINIMUM_OUTPUT_SAMPLE_COUNT, (UINT32)m_osc);
			pat->SetUINT32(MFT_DECODER_EXPOSE_OUTPUT_TYPES_IN_NATIVE_ORDER, (UINT32)(m_subtype.Data1));
			//			pat->GetUINT32(MF_SA_D3D_AWARE, (UINT32*)&m_d3da);

			//			pat->GetUINT32(MFT_SUPPORT_DYNAMIC_FORMAT_CHANGE, (UINT32*)&m_dfc);
			pat->Release();
			pat = 0;
		}

#if 0
		SetInOutFmt();
#else

		ICodecAPI * pCodec = 0;
		hr = m_pDecoder->QueryInterface(__uuidof(ICodecAPI), (void**)&pCodec);
		if (SUCCEEDED(hr)){
			m_lowlat = 1;

			hr = pCodec->SetValue(&CODECAPI_AVDecVideoAcceleration_H264, &(_variant_t((ULONG)m_DXVA)));
			hr = pCodec->SetValue(&CODECAPI_AVLowLatencyMode, &(_variant_t((ULONG)m_lowlat)));

			hr = pCodec->SetValue(&CODECAPI_AVDecVideoMaxCodedWidth, &(_variant_t((ULONG)m_nmaxw)));
			hr = pCodec->SetValue(&CODECAPI_AVDecVideoMaxCodedHeight, &(_variant_t((ULONG)m_nmaxh)));
			hr = pCodec->SetValue(&CODECAPI_AVDecVideoThumbnailGenerationMode, &(_variant_t((ULONG)m_tgm)));
			pCodec->Release();
			pCodec = 0;
		}
		UINT32 nrate = 0;
		UINT32 drate = 0;
		hr = MFAverageTimePerFrameToFrameRate(m_ndur, &nrate, &drate);
		if (SUCCEEDED(hr)){
			IMFMediaType * pmtdi = 0;
			MFCreateMediaType(&pmtdi);
			pmtdi->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
			pmtdi->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
			hr = pmtdi->SetUINT32(MF_MT_MPEG2_PROFILE, (UINT32)m_epf);
			hr = pmtdi->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
			hr = MFSetAttributeSize(pmtdi, MF_MT_FRAME_SIZE, m_nw, m_nh);
			hr = MFSetAttributeRatio(pmtdi, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
			hr = MFSetAttributeRatio(pmtdi, MF_MT_FRAME_RATE, nrate, drate);
			hr = pmtdi->SetUINT32(MF_MT_AVG_BITRATE, m_nbufsizeavg);
			hr = m_pDecoder->SetInputType(0, pmtdi, 0);
			SafeRelease(&pmtdi);
		}
		if (SUCCEEDED(hr)){
			IMFMediaType * pmtdo = 0;
			UINT32 nsize = 0;
			UINT32 ndx = 0;
			hr = MFGetStrideForBitmapInfoHeader(m_subtype.Data1, m_nw, (LONG*)&ndx);
			hr = MFCalculateImageSize(m_subtype, m_nw, m_nh, &nsize);
			MFCreateMediaType(&pmtdo);
			hr = pmtdo->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
			hr = pmtdo->SetGUID(MF_MT_SUBTYPE, m_subtype);
			hr = pmtdo->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
			hr = MFSetAttributeSize(pmtdo, MF_MT_FRAME_SIZE, m_nw, m_nh);
			hr = pmtdo->SetUINT32(MF_MT_DEFAULT_STRIDE, UINT32(ndx));
			hr = pmtdo->SetUINT32(MF_MT_SAMPLE_SIZE, nsize);
			hr = pmtdo->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
			hr = MFSetAttributeRatio(pmtdo, MF_MT_FRAME_RATE, nrate, drate);
			hr = MFSetAttributeRatio(pmtdo, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
			hr = m_pDecoder->SetOutputType(0, pmtdo, 0);
			SafeRelease(&pmtdo);
		}
#endif
		if (SUCCEEDED(hr)){
			//for (int i = 0; i < m_insamplelen; i++)
			//	CreateMediaSample(m_maxsamplesizei, m_ppInSample + i);
			CreateMediaSample(m_maxsamplesizeo, &m_pOutSample);

		}
		if (SUCCEEDED(hr)){

			hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
		}
	}
	return hr;
}

HRESULT MyH264D::Exit()
{
	HRESULT hr = S_OK;
	if (m_pDecoder){
		hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, 0);
		hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
		while (hr == S_OK)
		{
			DWORD dwStatus = 0;
			MFT_OUTPUT_DATA_BUFFER mdb;
			mdb.dwStreamID = 0;
			mdb.dwStatus = 0;
			mdb.pEvents = NULL;
			mdb.pSample = m_pOutSample;
			hr = m_pDecoder->ProcessOutput(0, 1, &mdb, &dwStatus);
			if (hr == S_OK){
				//f_OnGetSample();
			}
		}
		m_pDecoder->Release();
		m_pDecoder = 0;
	}
	SafeRelease(&m_pDecoder);
	return hr;
}



HRESULT MyH264D::PushData(IMFSample * pSample, int ischange, ITakeOut * pOut)
{
	HRESULT hr = E_FAIL;
	if (m_pDecoder){
		if (m_dfc == 0 && ischange){
			hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
		}
		while (1)
		{
			DWORD dwStatus = 0;
			MFT_OUTPUT_DATA_BUFFER mdb;
			mdb.dwStreamID = 0;
			mdb.dwStatus = 0;
			mdb.pEvents = NULL;
			mdb.pSample = m_pOutSample;
			hr = m_pDecoder->ProcessOutput(0, 1, &mdb, &dwStatus);
			if (hr == S_OK){
				m_pDstSample = mdb.pSample;
				if (pOut){
					IMFMediaBuffer * pBuf = 0;
					mdb.pSample->GetBufferByIndex(0, &pBuf);
					if (pBuf){
						BYTE * dst = 0;
						DWORD dwmax = 0;
						DWORD dwcur = 0;
						hr = pBuf->Lock(&dst, &dwmax, &dwcur);
						if (SUCCEEDED(hr)){
							pOut->OnGetOut(dst, dwcur);
							pBuf->Unlock();
							pBuf->SetCurrentLength(0);
						}
						pBuf->Release();
					}
				}
			}
			else
				break;
		}
		if (ischange){
			ICodecAPI * pCodec = 0;
			hr = m_pDecoder->QueryInterface(__uuidof(ICodecAPI), (void**)&pCodec);
			if (SUCCEEDED(hr)){
				hr = pCodec->SetValue(&CODECAPI_AVEncCommonRateControlMode, &(_variant_t((ULONG)m_em)));
				hr = pCodec->SetValue(&CODECAPI_AVEncCommonQuality, &(_variant_t((ULONG)m_nQuality)));
				hr = pCodec->SetValue(&CODECAPI_AVEncAdaptiveMode, &(_variant_t((ULONG)eAVEncAdaptiveMode_FrameRate)));

				//hr = pCodec->GetValue(&CODECAPI_AVEncCommonBufferSize, &(vi));
				hr = pCodec->SetValue(&CODECAPI_AVEncCommonBufferSize, &(_variant_t((ULONG)m_nbufsizeavg)));//eAVEncCommonRateControlMode_CBR
				hr = pCodec->SetValue(&CODECAPI_AVEncCommonMaxBitRate, &(_variant_t((ULONG)m_nbufsizemax)));//eAVEncCommonRateControlMode_PeakConstrainedVBR
				hr = pCodec->SetValue(&CODECAPI_AVEncCommonMeanBitRate, &(_variant_t((ULONG)m_nbufsizeavg)));//not eAVEncCommonRateControlMode_Quality
				hr = pCodec->SetValue(&CODECAPI_AVEncCommonQualityVsSpeed, &(_variant_t((ULONG)m_ncpu)));//quality/speed 0 - 100
				hr = pCodec->SetValue(&CODECAPI_AVEncH264CABACEnable, &(_variant_t((bool)m_iscabac)));

				//hr = pCodec->SetValue(&CODECAPI_AVEncH264SPSID, &(_variant_t((ULONG)seq_parameter_set_id)));// seq_parameter_set_id
				hr = pCodec->SetValue(&CODECAPI_AVEncMPVDefaultBPictureCount, &(_variant_t((ULONG)m_nBFrame)));
				hr = pCodec->SetValue(&CODECAPI_AVEncMPVGOPSize, &(_variant_t((ULONG)m_nGOPSize)));

				//hr = pCodec->SetValue(&CODECAPI_AVEncVideoContentType, &(_variant_t((ULONG)nctype)));//the type of video content
				//		hr = pCodec->SetValue(&CODECAPI_AVEncVideoEncodeQP, &(_variant_t((ULONGLONG)nQP)));
				////hr = pCodec->SetValue(&CODECAPI_AVEncVideoForceKeyFrame, &(_variant_t((ULONG)iskeyframe)));
				//		hr = pCodec->SetValue(&CODECAPI_AVEncVideoMinQP, &(_variant_t((ULONG)nMinQP)));
				//hr = pCodec->SetValue(&CODECAPI_AVLowLatencyMode, &(_variant_t((ULONG)islowlate)));//low-latency mode
				pCodec->Release();
				pCodec = 0;
			}
			UINT32 nrate = 0;
			UINT32 drate = 0;
			hr = MFAverageTimePerFrameToFrameRate(m_ndur, &nrate, &drate);
			if (SUCCEEDED(hr)){
				IMFMediaType * pmtdi = 0;
				MFCreateMediaType(&pmtdi);
				pmtdi->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
				pmtdi->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
				hr = pmtdi->SetUINT32(MF_MT_MPEG2_PROFILE, (UINT32)m_epf);
				hr = pmtdi->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
				hr = MFSetAttributeSize(pmtdi, MF_MT_FRAME_SIZE, m_nw, m_nh);
				hr = MFSetAttributeRatio(pmtdi, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
				hr = MFSetAttributeRatio(pmtdi, MF_MT_FRAME_RATE, nrate, drate);
				hr = pmtdi->SetUINT32(MF_MT_AVG_BITRATE, m_nbufsizeavg);
				hr = m_pDecoder->SetOutputType(0, pmtdi, 0);
				SafeRelease(&pmtdi);
			}
			if (SUCCEEDED(hr)){
				IMFMediaType * pmtdo = 0;
				UINT32 nsize = 0;
				UINT32 ndx = 0;
				hr = MFGetStrideForBitmapInfoHeader(m_subtype.Data1, m_nw, (LONG*)&ndx);
				hr = MFCalculateImageSize(m_subtype, m_nw, m_nh, &nsize);
				MFCreateMediaType(&pmtdo);
				hr = pmtdo->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
				hr = pmtdo->SetGUID(MF_MT_SUBTYPE, m_subtype);
				hr = pmtdo->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
				hr = MFSetAttributeSize(pmtdo, MF_MT_FRAME_SIZE, m_nw, m_nh);
				hr = pmtdo->SetUINT32(MF_MT_DEFAULT_STRIDE, UINT32(ndx));
				hr = pmtdo->SetUINT32(MF_MT_SAMPLE_SIZE, nsize);
				hr = pmtdo->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
				hr = MFSetAttributeRatio(pmtdo, MF_MT_FRAME_RATE, nrate, drate);
				hr = MFSetAttributeRatio(pmtdo, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
				hr = m_pDecoder->SetInputType(0, pmtdo, 0);
				SafeRelease(&pmtdo);
			}
		}
		else{
			if (hr == MF_E_TRANSFORM_STREAM_CHANGE){
				IMFMediaType * pmtdi = 0;
				hr = m_pDecoder->GetOutputAvailableType(0, 0, &pmtdi);
				if (pmtdi == 0)
					hr = MFCreateMediaType(&pmtdi);
				if (pmtdi){
					pmtdi->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
					pmtdi->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
					hr = pmtdi->SetUINT32(MF_MT_MPEG2_PROFILE, (UINT32)m_epf);
					hr = pmtdi->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
					hr = MFSetAttributeSize(pmtdi, MF_MT_FRAME_SIZE, m_nw, m_nh);
					hr = MFSetAttributeRatio(pmtdi, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
					//hr = MFSetAttributeRatio(pmtdi, MF_MT_FRAME_RATE, nrate, drate);
					hr = pmtdi->SetUINT32(MF_MT_AVG_BITRATE, m_nbufsizeavg);
					hr = m_pDecoder->SetOutputType(0, pmtdi, 0);
					SafeRelease(&pmtdi);
				}
			}
		}
		if (pSample){
			hr = m_pDecoder->ProcessInput(0, pSample, 0);
		}
	}
	return hr;
}

HRESULT MyH264D::Config(PEncConfig pConfig){
	if (
		pConfig->Width != m_nw
		|| pConfig->Height != m_nh
		|| pConfig->dwCpuSpeed != m_ncpu
		|| pConfig->dwThrad != m_nthread
		|| pConfig->dwQuality != m_nQuality
		|| pConfig->dwDur != m_ndur
		//|| pConfig->dwFPSN != m_dwFPSN
		//|| pConfig->dwBitRateN != dwBitRateN
		){
		m_nw = pConfig->Width;
		m_nh = pConfig->Height;
		m_ncpu = pConfig->dwCpuSpeed;
		m_nthread = pConfig->dwThrad;
		m_nQuality = pConfig->dwQuality;
		m_ndur = pConfig->dwDur;
		return PushData(NULL, 1, NULL);
	}
	return S_OK;
}

HRESULT MyH264D::Encode(void * pv, int nw, int nh, int ndx, const LONGLONG & llstarts,
	int nheadlen, long & nAll, DWORD & nOldMax, unsigned char * & pOld, LONGLONG & llstartd){
	m_pDstSample = 0;
	HRESULT hr = PushYV12(pv, nw, nh, ndx, m_ndur, NULL, llstarts, m_ndur, nAll);
	nAll = 0;
	if (m_pDstSample){
		IMFMediaBuffer * pBuf = 0;
		m_pDstSample->GetBufferByIndex(0, &pBuf);
		if (pBuf){
			BYTE * dst = 0;
			DWORD dwmax = 0;
			DWORD dwcur = 0;
			hr = pBuf->Lock(&dst, &dwmax, &dwcur);
			if (SUCCEEDED(hr)){
				nAll = dwcur + nheadlen;
				if (nAll > nOldMax){
					//if (pOld)
					//	delete[] pOld;
					//pOld = new BYTE[nAll + 100];
					//nOldMax = nAll + 100;
					//memset(pOld, 0, nheadlen);
				}
				else
					memcpy(pOld + nheadlen, dst, dwcur);
				pBuf->Unlock();
				pBuf->SetCurrentLength(0);
				m_pDstSample->GetSampleTime(&(llstartd));
			}
			pBuf->Release();
		}
	}

	return hr;
}

HRESULT MyH264D::PushYV12(void * pv, int nw, int nh, int ndx, int ndur, ITakeOut * pOut, LONGLONG llTime, LONGLONG LLDur, int ncursize){
	HRESULT hr = E_FAIL;
	int ischange = 0;
	IMFSample * pSample = 0;
	if (pv){
		pSample = FindFreeSample();
		if (pSample == 0)
			return hr;
		if (nw != m_nw
			|| nh != m_nh
			|| ndur != m_ndur
			|| m_subtype != MFVideoFormat_YV12
			){
			m_nw = nw;
			m_nh = nh;
			m_ndur = ndur;
			m_subtype = MFVideoFormat_YV12;
			ischange = 1;
		}
		{
			int nsize = ncursize;
			IMFMediaBuffer * pBuf = 0;
			pSample->Release();
			pSample->AddRef();
			pSample->GetBufferByIndex(0, &pBuf);
			if (pBuf){
				BYTE * dst = 0;
				DWORD dwmax = 0;
				DWORD dwcur = 0;
				hr = pBuf->Lock(&dst, &dwmax, &dwcur);
				memcpy(dst, pv, nsize);
				pBuf->Unlock();
				pBuf->SetCurrentLength(nsize);
				pBuf->Release();
			}
			pSample->SetSampleTime(llTime);
			pSample->SetSampleDuration(LLDur);
			pSample->Release();
			pSample->AddRef();
			//pSample->SetSampleFlags(0);
		}
	}
	hr = PushData(pSample, ischange, pOut);
	if (pSample){
		pSample->Release();
		pSample = 0;
	}
	return hr;
}
IMFSample * MyH264D::FindFreeSample(){
	for (int i = 0; i < m_insamplenum; i++){
		if (m_ppInSample[i] == 0){
			CreateMediaSample(m_maxsamplesizei, m_ppInSample + i);
		}
		if (m_ppInSample[i]){
			ULONG ul = m_ppInSample[i]->AddRef();
			if (ul == 2)
				return m_ppInSample[i];
			m_ppInSample[i]->Release();
		}
	}
	return NULL;
}
IMyH264D * __stdcall API_CreateMyH264D(int ninlen){
	MyH264D * phs = new MyH264D(ninlen);
	if (SUCCEEDED(phs->Init()))
		return (IMyH264D*)phs;
	else
	{
		delete phs;
	}
	return 0;
}

void __stdcall API_FreeMyH264D(IMyH264D * pv){
	MyH264D * phs = (MyH264D*)(pv);
	if (phs){
		delete phs;
	}
}






















HRESULT MyH264D::Reset(){
	HRESULT hr = 0;

	//IMFMediaType * pmtdi = 0;
	//hr = m_pDecoder->GetInputAvailableType(0, 0, &pmtdi);
	//hr = m_pDecoder->SetInputType(0, pmtdi, 0);
	//SafeRelease(&pmtdi);

	//IMFMediaType * pmtdo = 0;
	//hr = m_pDecoder->GetOutputAvailableType(0, 0, &pmtdo);
	//hr = m_pDecoder->SetOutputType(0, pmtdo, 0);
	//SafeRelease(&pmtdo);

	if (m_pOutSample){
		//m_pOutSample->AddRef();
		//int nn = m_pOutSample->Release();
		//if (nn > 1){
		//	m_pOutSample->Release();
		//	m_pOutSample = 0;
		//}

		//m_pOutSample->SetSampleTime(0);
		//m_pOutSample->SetSampleDuration(0);


		//IMFMediaBuffer * pBuf = 0;

		//MFCreateMemoryBuffer(m_maxsamplesizeo, &pBuf);
		//m_pOutSample->RemoveAllBuffers();
		//m_pOutSample->AddBuffer(pBuf);
		//pBuf->Release();


		//m_pOutSample->GetBufferByIndex(0, &pBuf);
		//pBuf->SetCurrentLength(0);
		//pBuf->Release();
	}
	//if (m_pOutSample==0)
	//CreateMediaSample(m_maxsamplesizeo, &m_pOutSample);
	return S_OK;
}

HRESULT MyH264D::SetInfo(int nw, int nh, int ndur)
{
	HRESULT hr = E_FAIL;
	if (m_pDecoder){
		if (nw != m_nw
			|| nh != m_nh
			|| ndur != m_ndur
			|| m_subtype != MFVideoFormat_YV12
			){
			m_nw = nw;
			m_nh = nh;
			m_ndur = ndur;
			m_subtype = MFVideoFormat_YV12;
			if (m_dfc == 0){
				hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
			}
			while (hr == S_OK)
			{
				DWORD dwStatus = 0;
				MFT_OUTPUT_DATA_BUFFER mdb;
				mdb.dwStreamID = 0;
				mdb.dwStatus = 0;
				mdb.pEvents = NULL;
				mdb.pSample = m_pOutSample;
				hr = m_pDecoder->ProcessOutput(0, 1, &mdb, &dwStatus);
			}
			return SetInOutFmt();
		}
		else
			hr = S_OK;
	}
	return hr;
}

HRESULT MyH264D::SetInOutFmt()
{
	ICodecAPI * pCodec = 0;
	HRESULT hr = m_pDecoder->QueryInterface(__uuidof(ICodecAPI), (void**)&pCodec);
	if (SUCCEEDED(hr)){
		hr = pCodec->SetValue(&CODECAPI_AVEncCommonRateControlMode, &(_variant_t((ULONG)m_em)));
		hr = pCodec->SetValue(&CODECAPI_AVEncCommonQuality, &(_variant_t((ULONG)m_nQuality)));
		hr = pCodec->SetValue(&CODECAPI_AVEncAdaptiveMode, &(_variant_t((ULONG)eAVEncAdaptiveMode_FrameRate)));

		//hr = pCodec->GetValue(&CODECAPI_AVEncCommonBufferSize, &(vi));
		hr = pCodec->SetValue(&CODECAPI_AVEncCommonBufferSize, &(_variant_t((ULONG)m_nbufsizeavg)));//eAVEncCommonRateControlMode_CBR
		hr = pCodec->SetValue(&CODECAPI_AVEncCommonMaxBitRate, &(_variant_t((ULONG)m_nbufsizemax)));//eAVEncCommonRateControlMode_PeakConstrainedVBR
		hr = pCodec->SetValue(&CODECAPI_AVEncCommonMeanBitRate, &(_variant_t((ULONG)m_nbufsizeavg)));//not eAVEncCommonRateControlMode_Quality
		hr = pCodec->SetValue(&CODECAPI_AVEncCommonQualityVsSpeed, &(_variant_t((ULONG)m_ncpu)));//quality/speed 0 - 100
		hr = pCodec->SetValue(&CODECAPI_AVEncH264CABACEnable, &(_variant_t((bool)m_iscabac)));

		//hr = pCodec->SetValue(&CODECAPI_AVEncH264SPSID, &(_variant_t((ULONG)seq_parameter_set_id)));// seq_parameter_set_id
		hr = pCodec->SetValue(&CODECAPI_AVEncMPVDefaultBPictureCount, &(_variant_t((ULONG)m_nBFrame)));
		hr = pCodec->SetValue(&CODECAPI_AVEncMPVGOPSize, &(_variant_t((ULONG)m_nGOPSize)));

		//hr = pCodec->SetValue(&CODECAPI_AVEncVideoContentType, &(_variant_t((ULONG)nctype)));//the type of video content
		//		hr = pCodec->SetValue(&CODECAPI_AVEncVideoEncodeQP, &(_variant_t((ULONGLONG)nQP)));
		////hr = pCodec->SetValue(&CODECAPI_AVEncVideoForceKeyFrame, &(_variant_t((ULONG)iskeyframe)));
		//		hr = pCodec->SetValue(&CODECAPI_AVEncVideoMinQP, &(_variant_t((ULONG)nMinQP)));
		//hr = pCodec->SetValue(&CODECAPI_AVLowLatencyMode, &(_variant_t((ULONG)islowlate)));//low-latency mode
		pCodec->Release();
		pCodec = 0;
	}
	UINT32 nrate = 0;
	UINT32 drate = 0;
	hr = MFAverageTimePerFrameToFrameRate(m_ndur, &nrate, &drate);
	if (SUCCEEDED(hr)){
		IMFMediaType * pmtdi = 0;
		MFCreateMediaType(&pmtdi);
		pmtdi->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		pmtdi->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
		hr = pmtdi->SetUINT32(MF_MT_MPEG2_PROFILE, (UINT32)m_epf);
		hr = pmtdi->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
		hr = MFSetAttributeSize(pmtdi, MF_MT_FRAME_SIZE, m_nw, m_nh);
		hr = MFSetAttributeRatio(pmtdi, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
		hr = MFSetAttributeRatio(pmtdi, MF_MT_FRAME_RATE, nrate, drate);
		hr = pmtdi->SetUINT32(MF_MT_AVG_BITRATE, m_nbufsizeavg);
		hr = m_pDecoder->SetOutputType(0, pmtdi, 0);
		SafeRelease(&pmtdi);
	}
	if (SUCCEEDED(hr)){
		IMFMediaType * pmtdo = 0;
		UINT32 nsize = 0;
		UINT32 ndx = 0;
		hr = MFGetStrideForBitmapInfoHeader(m_subtype.Data1, m_nw, (LONG*)&ndx);
		hr = MFCalculateImageSize(m_subtype, m_nw, m_nh, &nsize);
		MFCreateMediaType(&pmtdo);
		hr = pmtdo->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		hr = pmtdo->SetGUID(MF_MT_SUBTYPE, m_subtype);
		hr = pmtdo->SetUINT32(MF_MT_INTERLACE_MODE, m_interlace);
		hr = MFSetAttributeSize(pmtdo, MF_MT_FRAME_SIZE, m_nw, m_nh);
		hr = pmtdo->SetUINT32(MF_MT_DEFAULT_STRIDE, UINT32(ndx));
		hr = pmtdo->SetUINT32(MF_MT_SAMPLE_SIZE, nsize);
		hr = pmtdo->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
		hr = MFSetAttributeRatio(pmtdo, MF_MT_FRAME_RATE, nrate, drate);
		hr = MFSetAttributeRatio(pmtdo, MF_MT_PIXEL_ASPECT_RATIO, m_raitox, m_raitoy);
		hr = m_pDecoder->SetInputType(0, pmtdo, 0);
		SafeRelease(&pmtdo);
	}
	return hr;
}


HRESULT MyH264D::PushInData(LPBYTE pbin, DWORD dwin, LONGLONG llTime)
{
	HRESULT hr = E_FAIL;
	if (m_pDecoder){
		DWORD dwFlags = 0;
		while ((dwFlags & MFT_INPUT_STATUS_ACCEPT_DATA) == 0){
			m_pDecoder->GetInputStatus(0, &dwFlags);
			Sleep(1);
		}

		IMFSample * pSample = FindFreeSample();
		if (pSample){
			IMFMediaBuffer * pBuf = 0;
			pSample->GetBufferByIndex(0, &pBuf);
			if (pBuf){
				BYTE * dst = 0;
				DWORD dwmax = 0;
				DWORD dwcur = 0;
				hr = pBuf->Lock(&dst, &dwmax, &dwcur);
				if (dst){
					memcpy(dst, pbin, dwin);
					pBuf->Unlock();
					pBuf->SetCurrentLength(dwin);
					pBuf->Release();
					pSample->SetSampleTime(llTime);
					pSample->SetSampleDuration(m_ndur);
					hr = m_pDecoder->ProcessInput(0, pSample, 0);
				}
			}
			pSample->Release();
		}
	}
	return hr;
}


HRESULT MyH264D::GetOutData(LPBYTE pbout, DWORD & dwout, LONGLONG & llTime)
{
	dwout = 0;
	HRESULT hr = E_FAIL;
	if (m_pDecoder){
		DWORD dwStatus = 0;
		MFT_OUTPUT_DATA_BUFFER mdb;
		mdb.dwStreamID = 0;
		mdb.dwStatus = 0;
		mdb.pEvents = NULL;
		mdb.pSample = m_pOutSample;
		hr = m_pDecoder->ProcessOutput(0, 1, &mdb, &dwStatus);

		//hr = m_pDecoder->ProcessOutput(MFT_PROCESS_OUTPUT_REGENERATE_LAST_OUTPUT, 1, &mdb, &dwStatus);


		if (hr == S_OK){
			IMFMediaBuffer * pBuf = 0;
			hr = mdb.pSample->GetBufferByIndex(0, &pBuf);
			if (pBuf){
				BYTE * dst = 0;
				DWORD dwmax = 0;
				DWORD dwcur = 0;
				hr = pBuf->Lock(&dst, &dwmax, &dwcur);
				if (SUCCEEDED(hr)){
					memcpy(pbout, dst, dwcur);
					dwout = dwcur;
					pBuf->Unlock();
					pBuf->SetCurrentLength(0);
					mdb.pSample->GetSampleTime(&llTime);
				}
				pBuf->Release();
			}
		}
	}
	if (hr == MF_E_TRANSFORM_STREAM_CHANGE){
		SetInOutFmt();
	}
	return hr;
}

MyEvent::MyEvent(BOOL bManualReset, BOOL bInitialState)
{
#ifdef USEWINDOW
	//	m_HEvent = CreateEvent(0,bManualReset,bInitialState,0);
	InitializeCriticalSection(&m_mtx);
#else
#endif
	m_bmanule = bManualReset;
	m_bOut = FALSE;
	m_we.bSet = bInitialState;
	m_we.pPrev = 0;
	m_we.pNext = 0;
	m_pwe = &m_we;
}

MyEvent::~MyEvent()
{
	m_bOut = TRUE;
	Sleep(10);
#ifdef USEWINDOW
	DeleteCriticalSection(&m_mtx);
#else

#endif
}

void MyEvent::Set(){
	//SetEvent(m_HEvent);
#ifdef USEWINDOW
	EnterCriticalSection(&m_mtx);
#else
	pthread_mutex_lock(&m_mtx);
#endif
	if (m_bmanule == FALSE)
		m_pwe->bSet = TRUE;
	else{
		PMYENVENTWAIT pwe = &m_we;
		while (pwe){
			pwe->bSet = TRUE;
			pwe = pwe->pNext;
		}
	}
#ifdef USEWINDOW
	LeaveCriticalSection(&m_mtx);
#else
	pthread_mutex_unlock(&m_mtx);
#endif
}
void MyEvent::Reset(){
	//ResetEvent(m_HEvent);
#ifdef USEWINDOW
	EnterCriticalSection(&m_mtx);
#else

#endif
	m_we.bSet = FALSE;
#ifdef USEWINDOW
	LeaveCriticalSection(&m_mtx);
#else

#endif
}


DWORD MyEvent::Wait(INT nwait){
	//#ifdef USEWINDOW
	//return WaitForSingleObject(m_HEvent,(DWORD)nwait);
	//#else
	MYENVENTWAIT ew = { 0, 0, 0 };
	PMYENVENTWAIT pew = &ew;
	//	DWORD dwwaitm;
#ifdef USEWINDOW
	EnterCriticalSection(&m_mtx);
#else

#endif
	pew->pPrev = &m_we;
	pew->pNext = m_we.pNext;
	if (m_we.pNext)
		m_we.pNext->pPrev = pew;
	else
		m_pwe = pew;
	m_we.pNext = pew;
	pew->bSet = m_we.bSet;
	if (m_bmanule == FALSE)
		m_we.bSet = FALSE;
#ifdef USEWINDOW
	LeaveCriticalSection(&m_mtx);
#else

#endif

	DWORD dwRt = WAIT_FAILED;
	while (m_bOut == FALSE){
		if (pew->bSet)
			dwRt = WAIT_OBJECT_0;
		else if (nwait == 0)
			dwRt = WAIT_TIMEOUT;
		else
			nwait--;
		if (dwRt != WAIT_FAILED)
			break;
		Sleep(1);
	}


#ifdef USEWINDOW
	EnterCriticalSection(&m_mtx);
#else

#endif
	if (pew->pNext)
		pew->pNext->pPrev = pew->pPrev;
	else
		m_pwe = pew->pPrev;
	pew->pPrev->pNext = pew->pNext;
#ifdef USEWINDOW
	LeaveCriticalSection(&m_mtx);
#else

#endif
	return dwRt;
	//#endif
}



MyLock::MyLock(const char * lpName)
{
	m_pEvent = new MyEvent(FALSE, TRUE);

}

MyLock::~MyLock()
{
	if (m_pEvent){
		Unlock();
		Lock();
		SafeDelete(m_pEvent);
	}
}


BOOL	MyLock::Lock(DWORD dwTime)
{
	return m_pEvent->Wait(dwTime) == WAIT_OBJECT_0;
}

void	MyLock::Unlock()
{
	m_pEvent->Set();
}






void * MyThread_F_Excute(void * pv)
{
	MyThread * pThis = (MyThread*)pv;
	if (pThis){
		pThis->m_lkThread.Lock();
		pThis->m_bStart = TRUE;
		pThis->m_bEnd = FALSE;
		pThis->m_lkThread.Unlock();
		pThis->ThreadExcute();
		pThis->m_lkThread.Lock();
		if (pThis->m_bStart)
			pThis->m_bEnd = TRUE;
		pThis->m_lkThread.Unlock();
	}
	return pv;
};
void MyThread::ThreadExcute()
{
	printf("old ThreadExcute");
	m_eStop->Wait(-1);
}
MyThread::MyThread()
{
	m_bStart = FALSE;
	m_bEnd = FALSE;

	m_eStop = new MyEvent(TRUE, TRUE);
	m_eThread = new MyEvent(TRUE, TRUE);

#ifdef USEWINDOW
	m_hThread = 0;
	m_dwThreadID = 0;
#else
#endif
}
BOOL MyThread::IsWork(){
	BOOL bRt = FALSE;
	m_lkThread.Lock();
	bRt = (m_bStart == TRUE && m_bEnd == FALSE);
	m_lkThread.Unlock();
	return bRt;
}
WorkThreadEx::WorkThreadEx()
{
	m_pMyWork = NULL;
	m_bkPm = 0;
	m_bkPmLenMax = 0;
	m_inPm = 0;
	m_inPmLenMax = 0;
	m_dwCmd = 0;
	m_inPmLen = 0;
	m_bkPmLen = 0;


	m_bWork = FALSE;
	StartThread();
}

WorkThreadEx::~WorkThreadEx()
{
	SetStop(0, 1000);
	CloseThread(1000);
	SafeDeletes(m_inPm);
	SafeDeletes(m_bkPm);
}


BOOL WorkThreadEx::IsNeedWork(DWORD dwWaitTime){
	return (m_eStop->Wait(dwWaitTime) == WAIT_TIMEOUT && m_bWork);
}


void WorkThreadEx::ThreadExcute()
{
	//LogW("WorkThreadEx is exec\n");
	while (m_eStop->Wait(2) == WAIT_TIMEOUT)
	{
		//LogW("WorkThreadEx is loop\n");
		if (m_lk.Lock(0)){
			if (m_pMyWork){
				//			LogW("WorkThreadEx is work\n");
				//m_pMyWork->OnStart(m_dwData);
				m_pMyWork->DoWork(this, m_dwParam);
				//m_pMyWork->OnStop(m_dwData);
				m_bWork = FALSE;
				m_pMyWork = NULL;
				//			LogW("WorkThreadEx work end\n");
			}
			else
				;//LogW("WorkThreadEx is not work\n");
			m_lk.Unlock();
		}
		else
			;//LogW("WorkThreadEx is lock error\n");
	}
}

BOOL WorkThreadEx::SetWorkParam(DWORD	dwCmd,
	LPVOID	inPm,
	DWORD	inPmLen,
	LPVOID	bkPm,
	DWORD	bkPmLen)
{
	m_dwCmd = dwCmd;
	if (inPm && inPmLen)
	{
		if (inPmLen > m_inPmLenMax){
			SafeDeletes(m_inPm);
			m_inPmLenMax = inPmLen;
			m_inPm = new BYTE[m_inPmLenMax];
		}
		memcpy(m_inPm, inPm, inPmLen);
		m_inPmLen = inPmLen;
	}
	else
		m_inPmLen = 0;
	if (bkPm && bkPmLen)
	{
		if (bkPmLen > m_bkPmLenMax){
			SafeDeletes(m_bkPm);
			m_bkPmLenMax = bkPmLen;
			m_bkPm = new BYTE[m_bkPmLenMax];
		}

		memcpy(m_bkPm, bkPm, bkPmLen);
		m_bkPmLen = bkPmLen;
	}
	else
		m_bkPmLen = 0;
	return TRUE;
}

BOOL WorkThreadEx::AddMyWork(IMyWork * pMyWork, DWORD dwParam, DWORD dwData,
	DWORD	dwCmd,
	LPVOID	inPm,
	DWORD	inPmLen,
	LPVOID	bkPm,
	DWORD	bkPmLen
	, DWORD dwWait)
{
	BOOL brt = FALSE;
	if (pMyWork && m_lk.Lock(dwWait)){
		if (m_pMyWork == NULL)
		{
			m_pMyWork = pMyWork;
			m_dwParam = dwParam;
			m_bWork = TRUE;
			SetWorkParam(dwCmd, inPm, inPmLen, bkPm, bkPmLen);
			pMyWork->OnSetStart(this, m_dwParam, dwData);
			//			LogW("thread star\n");
			brt = TRUE;
		}
		m_lk.Unlock();
	}
	return brt;
}
BOOL	WorkThreadEx::SetStop(DWORD dwData, DWORD dwWait)
{
	IMyWork * pMyWork = m_pMyWork;
	if (pMyWork){
		pMyWork->OnSetStop(this, m_dwParam, dwData);
	}
	m_bWork = FALSE;
	while (IsFree() == FALSE&&dwWait > 0){
		dwWait--;
		Sleep(1);
	}

	return TRUE;
}

WorkThreadEx * API_NewWorkThread(MyUnArray * pList, BOOL bCreate, IMyWork * pMyWork, DWORD dwParam, DWORD dwData,
	DWORD	dwCmd,
	LPVOID	inPm,
	DWORD	inPmLen,
	LPVOID	bkPm,
	DWORD	bkPmLen)
{
	pList->lk();
	WorkThreadEx * pCur = (WorkThreadEx *)(pList->GetF());
	while (pCur)
	{
		if (pCur->AddMyWork(pMyWork, dwParam, dwData, dwCmd, inPm, inPmLen, bkPm, bkPmLen, 0))
		{
			pCur->AddRef();
			break;
		}
		else
			pCur = (WorkThreadEx *)(pCur->GetNext());
	}
	if (!pCur && bCreate)
	{
		//		LogW("new thread\n");
		pCur = new WorkThreadEx;
		pList->PushTail(pCur, FALSE);
		pCur->AddMyWork(pMyWork, dwParam, dwData, dwCmd, inPm, inPmLen, bkPm, bkPmLen, 0);
	}
	pList->ulk();
	return pCur;
}


Work_ThreadS::Work_ThreadS()
{
	m_pvlists = new MyUnArray;
}

Work_ThreadS::~Work_ThreadS()
{
	if (m_pvlists){
		MyUnArray * pua = (MyUnArray*)m_pvlists;
		m_pvlists = 0;
		pua->ClearArray(TRUE);
		delete pua;

	}
	//m_ListWorkThread.ClearArray(TRUE);
}

WorkThreadEx * Work_ThreadS::WorkInThread(DWORD	dwCmd,
	LPVOID	inPm,
	DWORD	inPmLen,
	LPVOID	bkPm,
	DWORD	bkPmLen)
{
	WorkThreadEx * pWork = NULL;
	lk();
	pWork = API_NewWorkThread((MyUnArray*)m_pvlists, TRUE, this, dwCmd, 0, dwCmd, inPm, inPmLen, bkPm, bkPmLen);
	ulk();
	return pWork;
}


MyThread::~MyThread()
{
	m_eStop->Set();
#ifdef USEWINDOW
	CloseThread();
	if (m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = 0;
	}
#else
#endif
	SafeDelete(m_eStop);
}

BOOL MyThread::StartThread()
{
	BOOL bRt = FALSE;
	CloseThread();
	m_lkThread.Lock();
	m_eStop->Reset();
#ifdef USEWINDOW
	//CREATE_SUSPENDED
	if (!m_hThread)
		m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)(MyThread_F_Excute), (void*)this, CREATE_SUSPENDED, &m_dwThreadID);
	if (m_hThread)
		ResumeThread(m_hThread);
	bRt = (m_hThread != 0);
#else
#endif
	m_lkThread.Unlock();
	if (bRt)
		Sleep(1);
	return bRt;
}
BOOL MyThread::CloseThread(INT dwWait)
{
	BOOL bRt = FALSE;
	m_eStop->Set();
#ifdef USEWINDOW
	if (m_hThread)
	{
		if (WaitForSingleObject(m_hThread, dwWait) == WAIT_TIMEOUT){
			bRt = FALSE;
		}
		CloseHandle(m_hThread);
		m_hThread = 0;
		m_dwThreadID = 0;
	}
#else
	
#endif
	m_lkThread.Lock();
	m_bStart = m_bEnd = FALSE;
	m_lkThread.Unlock();
	return bRt;
}


static GUID clsid_AACdec = { 0x32d186a7, 0x218f, 0x4c75, { 0x88, 0x76, 0xdd, 0x77, 0x27, 0x3a, 0x89, 0x99 } };
HRESULT CreateMediaSample(DWORD cbData, IMFSample **ppSample);

HRESULT MyAACD::Init()
{
	HRESULT hr = E_FAIL;
	Exit();

	BYTE bdata[] = { 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xb0 };
	hr = CoCreateInstance(clsid_AACdec, NULL, CLSCTX_INPROC_SERVER, IID_IMFTransform, (void**)&m_pDecoder);
	if (SUCCEEDED(hr)){
		IMFAttributes * pat = 0;
		m_pDecoder->GetAttributes(&pat);
		if (pat){
			pat->Release();
			pat = 0;
		}
		if (SUCCEEDED(hr)){
			IMFMediaType * pmtdi = 0;
			//MFCreateMediaType(&pmtdi);
			m_pDecoder->GetInputAvailableType(0, 0, &pmtdi);
			hr = pmtdi->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 0);
			hr = pmtdi->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 0);
			hr = pmtdi->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, 1);
			hr = pmtdi->SetUINT32(MF_MT_AUDIO_CHANNEL_MASK, 0);
			hr = pmtdi->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, m_wfe.nChannels);
			hr = pmtdi->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, m_wfe.nSamplesPerSec);
			hr = m_pDecoder->SetInputType(0, pmtdi, 0);
			SafeRelease(&pmtdi);
		}
		if (SUCCEEDED(hr)){
			IMFMediaType * pmtdo = 0;
			m_pDecoder->GetOutputAvailableType(0, 0, &pmtdo);
			MFCreateMediaType(&pmtdo);
			hr = pmtdo->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
			hr = pmtdo->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
			hr = pmtdo->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, m_wfe.wBitsPerSample);
			hr = pmtdo->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, m_wfe.nChannels);
			hr = pmtdo->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, m_wfe.nSamplesPerSec);
			hr = pmtdo->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, m_wfe.nBlockAlign);
			hr = pmtdo->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, m_wfe.nAvgBytesPerSec);
			hr = pmtdo->SetUINT32(MF_MT_ORIGINAL_WAVE_FORMAT_TAG, WAVE_FORMAT_PCM);
			hr = m_pDecoder->SetOutputType(0, pmtdo, 0);
			SafeRelease(&pmtdo);
		}
		if (SUCCEEDED(hr)){
			//for (int i = 0; i < m_insamplelen; i++)
			//	CreateMediaSample(m_maxsamplesizei, m_ppInSample + i);
			CreateMediaSample(m_maxsamplesizeo, &m_pOutSample);
		}
		if (SUCCEEDED(hr)){
			hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
		}
	}
	return hr;
}

HRESULT MyAACD::Exit()
{
	HRESULT hr = S_OK;

	m_isinithac = 0;
	m_outbuffer = 0;
	m_outlen = 0;
	if (m_pDecoder){
		hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, 0);
		hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
		while (hr == S_OK)
		{
			DWORD dwStatus = 0;
			MFT_OUTPUT_DATA_BUFFER mdb;
			mdb.dwStreamID = 0;
			mdb.dwStatus = 0;
			mdb.pEvents = NULL;
			mdb.pSample = m_pOutSample;
			hr = m_pDecoder->ProcessOutput(0, 1, &mdb, &dwStatus);
			if (hr == S_OK){
				//f_OnGetSample();
			}
		}
		m_pDecoder->Release();
		m_pDecoder = 0;
	}
	SafeRelease(&m_pDecoder);
	return hr;
}


IMFSample * MyAACD::FindFreeSample(){
	for (int i = 0; i < m_insamplenum; i++){
		if (m_ppInSample[i] == 0){
			CreateMediaSample(m_maxsamplesizei, m_ppInSample + i);
		}
		if (m_ppInSample[i]){
			ULONG ul = m_ppInSample[i]->AddRef();
			if (ul == 2)
				return m_ppInSample[i];
			m_ppInSample[i]->Release();
		}
	}
	return NULL;
}
IMyAACD * __stdcall API_CreateMyAACD(int ninlen){
	MyAACD * phs = new MyAACD(ninlen);
	if (SUCCEEDED(phs->Init()))
		return (IMyAACD*)phs;
	else
	{
		delete phs;
	}
	return 0;
}

void __stdcall API_FreeMyAACD(IMyAACD * pv){
	MyAACD * phs = (MyAACD*)(pv);
	if (phs){
		delete phs;
	}
}






















HRESULT MyAACD::Reset(){
	return S_OK;
}

HRESULT MyAACD::SetInfo(WAVEFORMATEX * pfe, int islpcm)
{
	HRESULT hr = E_FAIL;
	if (m_decmode == 0){
		if (islpcm)
		{
			if (pfe->wBitsPerSample != m_wfe.wBitsPerSample
				|| pfe->nChannels != m_wfe.nChannels
				|| pfe->nSamplesPerSec != m_wfe.nSamplesPerSec
				|| m_subtype != MFAudioFormat_PCM
				)
			{
				//m_wfe = *pfe;
				m_wfe.nChannels = pfe->nChannels;
				m_wfe.nSamplesPerSec = pfe->nSamplesPerSec;
				m_wfe.wBitsPerSample = pfe->wBitsPerSample;
				m_wfe.nBlockAlign = m_wfe.wBitsPerSample * m_wfe.nChannels / 8;
				m_wfe.nAvgBytesPerSec = m_wfe.nSamplesPerSec * m_wfe.nBlockAlign;


				m_subtype = MFAudioFormat_PCM;
				m_isinithac = 0;
			}
		}
		else{
			if (pfe->wBitsPerSample != m_wfe.wBitsPerSample
				|| pfe->nChannels != m_wfe.nChannels
				|| pfe->nSamplesPerSec != m_wfe.nSamplesPerSec
				|| m_subtype != MFAudioFormat_AAC
				){
				//m_wfe = *pfe;
				m_wfe.nChannels = pfe->nChannels;
				m_wfe.nSamplesPerSec = pfe->nSamplesPerSec;
				m_wfe.wBitsPerSample = pfe->wBitsPerSample;
				m_wfe.nBlockAlign = m_wfe.wBitsPerSample * m_wfe.nChannels / 8;
				m_wfe.nAvgBytesPerSec = m_wfe.nSamplesPerSec * m_wfe.nBlockAlign;
				m_subtype = MFAudioFormat_AAC;
				m_isinithac = 0;
			}
		}
	}
	if (m_pDecoder){
		if (pfe->wBitsPerSample != m_wfe.wBitsPerSample
			|| pfe->nChannels != m_wfe.nChannels
			|| pfe->nSamplesPerSec != m_wfe.nSamplesPerSec
			|| m_subtype != MFAudioFormat_AAC
			){
			m_wfe = *pfe;
			m_subtype = MFAudioFormat_AAC;
			if (m_dfc == 0){
				hr = m_pDecoder->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
			}
			while (hr == S_OK)
			{
				DWORD dwStatus = 0;
				MFT_OUTPUT_DATA_BUFFER mdb;
				mdb.dwStreamID = 0;
				mdb.dwStatus = 0;
				mdb.pEvents = NULL;
				mdb.pSample = m_pOutSample;
				hr = m_pDecoder->ProcessOutput(0, 1, &mdb, &dwStatus);
			}
			return SetInOutFmt();
		}
		else
			hr = S_OK;
	}
	return hr;
}

HRESULT MyAACD::SetInOutFmt()
{
	HRESULT hr = S_OK;
	if (SUCCEEDED(hr)){
		IMFMediaType * pmtdi = 0;
		MFCreateMediaType(&pmtdi);
		pmtdi->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		pmtdi->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_AAC);
		hr = pmtdi->SetUINT32(MF_MT_AAC_AUDIO_PROFILE_LEVEL_INDICATION, 0);
		//hr = pmtdi->SetUINT32(MF_MT_AAC_PAYLOAD_TYPE, 0);
		hr = pmtdi->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, m_wfe.wBitsPerSample);
		//hr = pmtdi->SetUINT32(MF_MT_AUDIO_CHANNEL_MASK, m_wfe.nChannels);
		hr = pmtdi->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, m_wfe.nChannels);
		hr = pmtdi->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, m_wfe.nSamplesPerSec);
		//hr = pmtdi->SetUINT32(MF_MT_USER_DATA,0);
		hr = m_pDecoder->SetInputType(0, pmtdi, 0);
		SafeRelease(&pmtdi);
	}
	if (SUCCEEDED(hr)){
		IMFMediaType * pmtdo = 0;
		UINT32 nsize = 0;
		UINT32 ndx = 0;
		hr = MFGetStrideForBitmapInfoHeader(m_subtype.Data1, m_nw, (LONG*)&ndx);
		hr = MFCalculateImageSize(m_subtype, m_nw, m_nh, &nsize);
		MFCreateMediaType(&pmtdo);
		hr = pmtdo->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		hr = pmtdo->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		hr = pmtdo->SetUINT32(MF_MT_AAC_AUDIO_PROFILE_LEVEL_INDICATION, 0);
		//hr = pmtdo->SetUINT32(MF_MT_AAC_PAYLOAD_TYPE, 0);
		hr = pmtdo->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, m_wfe.wBitsPerSample);
		//hr = pmtdo->SetUINT32(MF_MT_AUDIO_CHANNEL_MASK, m_wfe.nChannels);
		hr = pmtdo->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, m_wfe.nChannels);
		hr = pmtdo->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, m_wfe.nSamplesPerSec);
		//hr = pmtdo->SetUINT32(MF_MT_USER_DATA,0);
		hr = m_pDecoder->SetOutputType(0, pmtdo, 0);
		SafeRelease(&pmtdo);
	}
	return hr;
}


HRESULT MyAACD::PushInData(LPBYTE pbin, DWORD dwin, LONGLONG llTime, LONGLONG * plldtime)
{
	HRESULT hr = E_FAIL;
	if (m_pDecoder){
		IMFSample * pSample = FindFreeSample();
		if (pSample){
			IMFMediaBuffer * pBuf = 0;
			pSample->GetBufferByIndex(0, &pBuf);
			if (pBuf){
				BYTE * dst = 0;
				DWORD dwmax = 0;
				DWORD dwcur = 0;
				hr = pBuf->Lock(&dst, &dwmax, &dwcur);
				if (dst){
					memcpy(dst, pbin, dwin);
					pBuf->Unlock();
					pBuf->SetCurrentLength(dwin);
					pBuf->Release();
					hr = m_pDecoder->ProcessInput(0, pSample, 0);
				}
			}
			pSample->Release();
		}
	}
	return hr;
}


HRESULT MyAACD::GetOutData(LPBYTE pbout, DWORD & dwout, LONGLONG & llTime)
{
	dwout = 0;
	HRESULT hr = E_FAIL;
	if (m_pDecoder){
		DWORD dwStatus = 0;
		if (S_OK == m_pDecoder->GetOutputStatus(&dwStatus)){
			if (dwStatus & MFT_OUTPUT_STATUS_SAMPLE_READY){

			}
			else
				return hr;
		}
		MFT_OUTPUT_DATA_BUFFER mdb;
		mdb.dwStreamID = 0;
		mdb.dwStatus = 0;
		mdb.pEvents = NULL;
		mdb.pSample = m_pOutSample;
		hr = m_pDecoder->ProcessOutput(1, 1, &mdb, &dwStatus);
		if (hr == S_OK){
			IMFMediaBuffer * pBuf = 0;
			hr = mdb.pSample->GetBufferByIndex(0, &pBuf);
			if (pBuf){
				BYTE * dst = 0;
				DWORD dwmax = 0;
				DWORD dwcur = 0;
				hr = pBuf->Lock(&dst, &dwmax, &dwcur);
				if (SUCCEEDED(hr)){
					memcpy(pbout, dst, dwcur);
					dwout = dwcur;
					pBuf->Unlock();
					pBuf->SetCurrentLength(0);
					mdb.pSample->GetSampleTime(&llTime);
				}
				pBuf->Release();
			}
		}
	}
	return hr;
}



#define  _OUT_PUT_MYSTRING "myout: "

int g_limits_Setting = 0;
LogType g_strlevel = (LogType)0xffff;

int g_logoutput = 0;
char g_logoutputpath[260] = { 0 };


void  LogOut(LogType etype, const CHAR * FuntionName, ...)
{

	if (etype & g_strlevel || g_strlevel == LOG_ALL)
	{
#pragma warning(disable:4996)
		SYSTEMTIME st;
		GetLocalTime(&st);
		char stime[260] = { 0 };
		sprintf(stime, "%04d-%02d-%02d %02d:%02d:%02d %03d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond,
			st.wMilliseconds
			);

		if (!FuntionName)
			return;
		va_list vlArgs;
		CHAR tzText[4096] = { 0 };
		CHAR tzText2[4096] = { 0 };
		va_start(vlArgs, FuntionName);
		vsprintf(tzText, FuntionName, vlArgs);
		sprintf(tzText2, "%s %s\n", stime, tzText);
		printf("%s", tzText2);
		OutputDebugStringA(tzText2);
		va_end(vlArgs);

		if (g_logoutput == 1)
		{
			static FILE * fp = NULL;
			static  char sp[260] = { 0 };
			if (sp[0] == 0)
			{
				sprintf(sp, "%s\%04d-%02d-%02d-%02d%02d%02d-%03d.txt",
					g_logoutputpath,
					st.wYear,
					st.wMonth,
					st.wDay,
					st.wHour,
					st.wMinute,
					st.wSecond,
					st.wMilliseconds
					);
			}
			else
			{
				fp = fopen(sp, "ab");
				if (fp)
				{
					fwrite(tzText2, strlen(tzText2), 1, fp);
					fclose(fp);
				}
			}
		}
#pragma warning(default:4996)
	}

}



void  MyOutput(const CHAR * FuntionName, ...)
{
#pragma warning(disable:4996)


	LogOut(LOG_ALL, FuntionName);
	/*
	if (!FuntionName)
	return;
	va_list vlArgs;
	CHAR tzText[1024];
	CHAR tzText2[1024];
	va_start(vlArgs, FuntionName);
	vsprintf(tzText, FuntionName, vlArgs);
	sprintf(tzText2, "\n%s\n", tzText);
	printf("%s", tzText2);
	OutputDebugStringA(tzText2);
	va_end(vlArgs);
	*/
#pragma warning(default:4996)
}



void MyOutputW(const TCHAR* ptzFormat, ...)
{
	//if( n <= g_limits_Setting)
	{
#pragma warning(disable:4996)
		if (!ptzFormat)
		return;
		va_list vlArgs;

		TCHAR tzText[1024];
		TCHAR tzText2[1024];
		va_start(vlArgs, ptzFormat);
		_vstprintf(tzText, ptzFormat, vlArgs);

		_stprintf(tzText2, _T("myoutW : %s\n"), tzText);

		OutputDebugString(tzText2);
		va_end(vlArgs);
#pragma warning(default:4996)
	}
}


#ifdef USEWINDOW
WSADATA     wsaData;
#endif
void API_ExitSocket()
{
#ifdef USEWINDOW
	WSACleanup();
#endif
}
int API_InitSocket()
{
	int    retval = 0;
#ifdef USEWINDOW
	//API_ExitSocket();
	// Load Winsock
	if ((retval = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		WSACleanup();
		return -1;
	}
#endif
	return retval;
}
int API_BindSocket(SOCKET sk, WORD wPort, BOOL bSwap)
{
	sockaddr_in sin1;
	sin1.sin_addr.s_addr = INADDR_ANY;
	sin1.sin_family = AF_INET;
	if (bSwap)
		sin1.sin_port = htons(wPort);
	else
		sin1.sin_port = wPort;
	return bind(sk, (struct sockaddr*)&sin1, sizeof(sin1));
};

SOCKET API_CreateBaseSocket(int nType)
{
	return socket(AF_INET, nType, IPPROTO_IP);
}

SOCKET API_CreateSocket(WORD wPort, BOOL bTcp, BOOL bswap)
{
	SOCKET s1;
	if (bTcp)
		s1 = API_CreateBaseSocket(SOCK_STREAM);
	else
		s1 = API_CreateBaseSocket(SOCK_DGRAM);
	if (INVALID_SOCKET != s1 && wPort)
	{
		if (API_BindSocket(s1, wPort, bswap) == SOCKET_ERROR)
		{
			closesocket(s1);
			s1 = INVALID_SOCKET;
		}
	}
	return s1;
};

int API_UdpSendToIp(SOCKET s1, char * chip, WORD wPort, const LPBYTE pData, DWORD dwLen)
{
	sockaddr_in remote;
	remote.sin_addr.s_addr = inet_addr(chip);
	remote.sin_family = AF_INET;
	remote.sin_port = htons(wPort);
	return sendto(s1, (const char*)pData, dwLen, 0, (const sockaddr*)&remote, sizeof(remote));
}


SOCKET	API_CreateSocketEx(WORD & wPort0, BOOL bTcp, BOOL bSwap, DWORD dwTryNum)
{
	SOCKET sk = INVALID_SOCKET;
	if (wPort0 == 0)
		dwTryNum = 1;
	//if(INVALID_SOCKET != sk)
	{
		WORD wPort = wPort0;
		INT NTestNUm = dwTryNum;
		while (NTestNUm > 0)
		{
			if (bTcp)
				sk = API_CreateBaseSocket(SOCK_STREAM);
			else
				sk = API_CreateBaseSocket(SOCK_DGRAM);
			if (INVALID_SOCKET == sk)
				break;

			if (wPort >= 0xffff)wPort = 0x401;
			if (SOCKET_ERROR != API_BindSocket(sk, wPort, bSwap))
			{
				sockaddr_in si;
				int addLen = sizeof(sockaddr_in);
				if (getsockname(sk, (sockaddr *)&si, (socklen_t*)(&addLen)) == 0)
				{
					if (bSwap)
						wPort0 = htons(si.sin_port);
					else
						wPort0 = si.sin_port;
				}
				else
					wPort0 = wPort;
				break;
			}
			else
			{
				closesocket(sk);
				sk = INVALID_SOCKET;
			}
			wPort++;
			NTestNUm--;
		}
		////if(NTestNUm<=0)
		////{
		////	closesocket(sk);
		////	sk = INVALID_SOCKET;
		////}
	}
	return sk;
}
int API_RecDataInTime(SOCKET sk,
	LPVOID	pvData,
	int		nLen,
	DWORD	dwWaitTime,
	sockaddr_in* padd,
	int * pnStart)
{
	int nAddLen = sizeof(sockaddr_in);
	int ni = 0;
	int nStart = pnStart ? *pnStart : 0;
	if (nStart < nLen)
	{
		char * buf = (char *)pvData + nStart;
		INT   len = (INT)(nLen - nStart);
		timeval timeout1;
		timeout1.tv_sec = dwWaitTime / 1000;
		timeout1.tv_usec = (dwWaitTime - timeout1.tv_sec * 1000) * 1000;
		fd_set	fdset;
		FD_ZERO(&fdset);
		FD_SET(sk, &fdset);
		INT maxfdp = sk + 1;
#ifdef USEWINDOW
		maxfdp = 0;
#endif
		ni = select(maxfdp, &fdset, 0, 0, &timeout1);
		while (ni > 0)
		{
			int nGet;
			if (padd)
				nGet = recvfrom(sk, buf, len, 0, (sockaddr *)padd, (socklen_t*)(&nAddLen));
			else
				nGet = recv(sk, buf, len, 0);
			//int nr = GetLastError();
			if (nGet > 0)
			{
				len -= nGet;
				buf += nGet;
				nStart += nGet;
			}
			else
			{
				ni = -1;
				break;
			}
			if (padd)
				return nStart;

			//if(len > 0)
			//{
			//	fdset.fd_count = 1;
			//	fdset.fd_array[0] = sk;
			//	timeout1.tv_sec = 0;
			//	timeout1.tv_usec = 0;
			//	ni = select(0,&fdset,0,0,&timeout1);
			//}
			//else
			break;
		}
		if (pnStart)*pnStart = nStart;
	}
	if (ni >= 0)
		return nStart;
	else
	{
		//if(WSAGetLastError() == 0)return nStart;
		return ni;
	}
}
int API_SendDataInTime(SOCKET sk,
	LPVOID pvData,
	int nLen,
	DWORD dwWaitTime,
	sockaddr_in* padd,
	int * pnStart)
{
	int ni = 0;
	int nStart = pnStart ? *pnStart : 0;
	if (nStart < nLen)
	{
		char * buf = (char *)pvData + nStart;
		INT   len = (INT)(nLen - nStart);
		timeval timeout1;
		timeout1.tv_sec = dwWaitTime / 1000;
		timeout1.tv_usec = (dwWaitTime - timeout1.tv_sec * 1000) * 1000;
		fd_set	fdset;
		FD_ZERO(&fdset);
		FD_SET(sk, &fdset);
		INT maxfdp = sk + 1;
#ifdef USEWINDOW
		maxfdp = 0;
#endif
		int ni = select(maxfdp, 0, &fdset, 0, &timeout1);
		while (ni > 0)
		{
			int nGet;
			if (padd)
				nGet = sendto(sk, buf, len, 0, (sockaddr*)padd, sizeof(sockaddr_in));
			else
				nGet = send(sk, buf, len, 0);
			if (nGet > 0)
			{
				len -= nGet;
				buf += nGet;
				nStart += nGet;
			}
			else
				break;
			if (len > 0)
			{
				FD_ZERO(&fdset);
				FD_SET(sk, &fdset);
				ni = select(maxfdp, 0, &fdset, 0, &timeout1);
			}
			else
				break;
		}
		if (pnStart)*pnStart = nStart;
	}
	if (ni >= 0)
		return nStart;
	else
		return ni;
}

BOOL API_IsSocketOK(SOCKET sk)
{
	if (sk != INVALID_SOCKET)
	{
		timeval timeout = { 0, 0 };
		fd_set	fdset;
		FD_ZERO(&fdset);
		FD_SET(sk, &fdset);
		INT maxfdp = sk + 1;
#ifdef USEWINDOW
		maxfdp = 0;
#endif
		int ni = select(maxfdp, 0, 0, &fdset, &timeout);
		if (ni >= 0)
		{
			//int nGet = sendto(sk,NULL,0,0,0,0);
			//if(nGet < 0)
			//	ni = -1;
		}
		if (ni >= 0)
			return TRUE;
	}
	return FALSE;
}

///////////////////////
char * API_GetIpFromeYM(char * YM)
{
	char * pIpt = NULL;
	struct hostent *hostf = gethostbyname(YM);
	if (hostf)
		pIpt = (char *)inet_ntoa(*(struct in_addr *)(hostf->h_addr));
	return pIpt;
}


#if 0
//主要 API GetNetworkParams() 和 GetAdaptersInfo()
void fff()
{
	DWORD Err;
	PFIXED_INFO pFixedInfo;
	DWORD FixedInfoSize = 0;
	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	DWORD AdapterInfoSize;
	PIP_ADDR_STRING pAddrStr;
	if ((Err = GetNetworkParams(NULL, &FixedInfoSize)) != 0)
	{
		if (Err != ERROR_BUFFER_OVERFLOW)
		{
			// printf("GetNetworkParams sizing failed with error %d/n", Err);
			return;
		}
	}
	// Allocate memory from sizing information
	if ((pFixedInfo = (PFIXED_INFO)GlobalAlloc(GPTR, FixedInfoSize)) == NULL)
	{
		// printf("Memory allocation error/n");
		return;
	}
	AdapterInfoSize = 0;
	if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
	{
		if (Err != ERROR_BUFFER_OVERFLOW)
		{
			// printf("GetAdaptersInfo sizing failed with error %d/n", Err);
			return;
		}
	}
	if ((pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize)) == NULL)
	{
		// printf("Memory allocation error/n");
		return;
	}
	// Get actual adapter information
	if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
	{
		// printf("GetAdaptersInfo failed with error %d/n", Err);
		return;
	}
	pAdapt = pAdapterInfo;
	//ShowMessage(pAdapt->Description);
	// ShowMessage(pAdapt->AdapterName);
	pAddrStr = &(pAdapt->IpAddressList);
	while (pAddrStr)
	{
		//ShowMessage(pAddrStr->IpAddress.String);
		//ShowMessage(pAddrStr->IpMask.String);
		pAddrStr = pAddrStr->Next;
	}
}
#endif

DWORD API_GetLoaclIp(DWORD * pOut, DWORD dwMaxNum)
{
	DWORD dwIndex = 0;
	char szHostName[MAX_PATH] = "";
	gethostname(szHostName, MAX_PATH);
	struct hostent *pHostent = gethostbyname(szHostName);
	if (pHostent && pHostent->h_addr_list)
	{
		struct in_addr  ** ppaddr = (struct in_addr  **)(pHostent->h_addr_list);
		while (dwIndex < dwMaxNum && *ppaddr)
		{
			pOut[dwIndex++] = (*ppaddr)->s_addr;
			if (dwIndex >= dwMaxNum)
				break;
			ppaddr++;
		}
	}
	return dwIndex;
}
/*
DWORD _GetAddrIndex()
{
DWORD nRt = 0;
PMIB_IPADDRTABLE pIPAddrTable = NULL;
DWORD dwSize = 0;
if (GetIpAddrTable(NULL, &dwSize, 0) ==	ERROR_INSUFFICIENT_BUFFER)
{
pIPAddrTable = (MIB_IPADDRTABLE *) malloc(dwSize);
}
if(pIPAddrTable)
{
if(GetIpAddrTable(pIPAddrTable, &dwSize, 0) == NO_ERROR)
{
// Save the interface index to use for adding an IP address
nRt = pIPAddrTable->table[0].dwIndex;
}
free(pIPAddrTable);
pIPAddrTable = NULL;
}
return nRt;
}
DWORD API_AddAddr(DWORD dwIp,DWORD dwMsk)
{
DWORD NTEContext = 0;
DWORD NTEInstance = 0;
DWORD dwIndex = _GetAddrIndex();
DWORD dwRetVal = AddIPAddress(dwIp,dwMsk,dwIndex,&NTEContext, &NTEInstance);
if(dwRetVal == 0x00001392)
NTEContext = dwIp;
return NTEContext;
}
VOID API_DelAddr(DWORD dwIp,DWORD dwMsk)
{
DeleteIPAddress(dwIp);
}
#endif

*/

__forceinline void FpCloseSocket(SOCKET sk, char * pData)
{
	if (sk != INVALID_SOCKET)
	{
		if (pData)
			;// LogW("sk 0x%x is closed %s", sk, pData);
		else
			;// LogW("sk 0x%x is closed: XXXX", sk);

		closesocket(sk);
	}
}





























