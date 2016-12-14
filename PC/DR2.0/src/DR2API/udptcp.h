#pragma once 
#include <winsock2.h>
#include <winsock.h>
#include <SDKDDKVer.h>
#pragma   warning(disable:4996)
#if _WIN32_WINNT < 0x0500
#define _WIN32_WINNT 0x0500 
#endif
#include "time.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "stdlib.h"
#include "math.h"

#include <new>
#include <iostream>
#include <windows.h>
//#include <windowsx.h>
#include <d3d9.h>
#include <mfapi.h>
#include <Mfidl.h>
#include <shlwapi.h>
//#include <mfreadwrite.h>
#include <mferror.h>

//#include <strsafe.h>
//#include <assert.h>
//#include <ks.h>
//#include <ksmedia.h>
//#include <Dbt.h>
#include <Dshow.h>
#include <Codecapi.h>

#include <comutil.h>
#include "d3d9.h"


#define USE_TEST


void	API_ExitSocket();
int		API_InitSocket();

SOCKET API_CreateBaseSocket(int nType);
int		API_BindSocket(SOCKET sk, WORD wPort, BOOL bSwap);
SOCKET	API_CreateSocket(WORD wPort, BOOL bTcp, BOOL bSwap);
SOCKET	API_CreateSocketEx(WORD & wPort, BOOL bTcp, BOOL bSwap, DWORD dwTryNum);

int		API_UdpSendToIp(SOCKET s1, char * chip, WORD wPort, const LPBYTE pData, DWORD dwLen);


int API_RecDataInTime(SOCKET sk,
	LPVOID pvData,
	int nLen,
	DWORD dwWaitTime,
	sockaddr_in* padd = NULL,
	int * pnStart = NULL);
int API_SendDataInTime(SOCKET sk,
	LPVOID pvData,
	int nLen,
	DWORD dwWaitTime,
	sockaddr_in* padd = NULL,
	int * pnStart = NULL);


char *	API_GetIpFromeYM(char * YM);

BOOL	API_IsSocketOK(SOCKET sk);

DWORD API_GetLoaclIp(DWORD * pOut, DWORD dwMaxNum);



//#ifdef USE_TEST
void	testUdpClient(char *, unsigned short);
void	testUdpServer(unsigned short);
void	testTcpClient(char *, unsigned short);
void	testTcpServer(unsigned short);
//#endif


INT API_CheckSend(SOCKET sk);
INT API_CheckRecv(SOCKET sk);


#define SETLINK(SSK) {linger li;li.l_onoff = 0;li.l_linger = 0;setsockopt((SSK), SOL_SOCKET, SO_LINGER, (char*)&li,sizeof(li));}
#define SETLINK1(SSK) {linger li;li.l_onoff = 1;li.l_linger = 200;setsockopt((SSK), SOL_SOCKET, SO_LINGER, (char*)&li,sizeof(li));}


int API_get_mac(char* pMac, int nbuffsz);




////////////////////////////////////////////////////////////////////







typedef   struct   tag_ip_hdr
{
	unsigned   char   ip_verlen;
	unsigned   char   ip_tos;
	unsigned   short   ip_totallength;
	unsigned   short   ip_id;
	unsigned   short   ip_offset;
	unsigned   char   ip_ttl;
	unsigned   char   ip_protocol;
	unsigned   short   ip_checksum;
	unsigned   int   ip_srcaddr;
	unsigned   int   ip_destaddr;
}   IP_HDR;
typedef struct ip_hdr //定义IP首部
{
	unsigned char h_verlen; //4位首部长度,4位IP版本号
	unsigned char tos; //8位服务类型TOS
	unsigned short total_len; //16位总长度（字节）
	unsigned short ident; //16位标识
	unsigned short frag_and_flags; //3位标志位
	unsigned char ttl; //8位生存时间 TTL
	unsigned char proto; //8位协议 (TCP, UDP 或其他)
	unsigned short checksum; //16位IP首部校验和
	unsigned int sourceIP; //32位源IP地址
	unsigned int destIP; //32位目的IP地址
}IPHEADER;

typedef struct tsd_hdr //定义TCP伪首部
{
	DWORD saddr; //源地址
	DWORD daddr; //目的地址
	char mbz;
	char ptcl; //协议类型
	unsigned short tcpl; //TCP长度
}PSDHEADER;

typedef struct tcp_hdr //定义TCP首部
{
	BYTE th_sport; //16位源端口
	BYTE th_dport; //16位目的端口
	unsigned int th_seq; //32位序列号
	unsigned int th_ack; //32位确认号
	unsigned char th_lenres; //4位首部长度/6位保留字
	unsigned char th_flag; //6位标志位
	BYTE th_win; //16位窗口大小
	BYTE th_sum; //16位校验和
	BYTE th_urp; //16位紧急数据偏移量
}TCPHEADER;




typedef   struct   tag_udp_hdr
{
	unsigned   short   src_portno;
	unsigned   short   dst_portno;
	unsigned   short   udp_length;
	unsigned   short   udp_checksum;
}   UDP_HDR;
typedef struct udp_hdr //定义TCP首部
{
	BYTE th_sport; //16位源端口
	BYTE th_dport; //16位目的端口
	BYTE th_win; //16位窗口大小
	BYTE th_sum; //16位校验和
}UDPHEADER;

//CheckSum:计算校验和的子函数
BYTE checksum(BYTE *buffer, int size);



int		tryRaw_SocketUdp(char * ipDst, WORD wDstPort,
	char * ipSrc, WORD wSrcPort,
	char * pData, DWORD dwLen);


int tryRaw_SocketTcp(char * ipDst, WORD wDstPort,
	char * ipSrc, WORD wSrcPort,
	char * pData, DWORD dwLen);





#ifdef _DEBUG
#define _TRACK
#endif

#ifndef WINAPI
#define WINAPI	__stdcall
#endif

#ifndef ARCAPI
#define ARCAPI virtual HRESULT WINAPI
#endif

#ifndef ARCAPI_
#define ARCAPI_(x) virtual x WINAPI
#endif

#ifndef ARCAPIN_
#define ARCAPIN_(x) x WINAPI
#endif
#ifndef _OUTPUT_STRHEADERA
#define _OUTPUT_STRHEADERA	"test "
#endif
#if 0
#ifdef _TRACK
__inline void TracklogU(const TCHAR* ptzFormat, ...)
{
#pragma warning(disable:4996)
	if (!ptzFormat)
		return;
	va_list vlArgs;

	TCHAR tzText[1024];
	TCHAR tzText2[1024];
	va_start(vlArgs, ptzFormat);
	_vstprintf(tzText, ptzFormat, vlArgs);
	_stprintf(tzText2, _T("%s%s\n"), _OUTPUT_STRHEADER, tzText);
	OutputDebugString(tzText2);
#ifdef _Print_To_Console
	_tprintf(tzText2);
#endif
	va_end(vlArgs);
#pragma warning(default:4996)
}

__inline void TracklogA(const CHAR* ptzFormat, ...)
{
#pragma warning(disable:4996)
	if (!ptzFormat)
		return;
	va_list vlArgs;

	CHAR tzText[1024];
	CHAR tzText2[1024];
	va_start(vlArgs, ptzFormat);
	vsprintf(tzText, ptzFormat, vlArgs);
	sprintf(tzText2, "%s%s\n", _OUTPUT_STRHEADERA, tzText);
	OutputDebugStringA(tzText2);
#ifdef _Print_To_Console
	printf(tzText2);
#endif
	va_end(vlArgs);
#pragma warning(default:4996)
}

#else
__inline void TracklogU(const TCHAR* ptzFormat, ...){}
__inline void TracklogA(const  CHAR* ptzFormat, ...){}
#endif
#endif

enum SYSTYPE
{
	WIN2K = 0,
	WINXP,
	WINVISTA,
	WIN7,
	INVALID_VERSION
};
#ifndef _SafeFreeLibrary
#define _SafeFreeLibrary(x) if (x){ FreeLibrary(x); (x) = NULL; }
#endif
#ifndef _SafeFreeBSTR
#define _SafeFreeBSTR(x) if (x) { ::SysFreeString(x); (x) = NULL; }
#endif

#ifndef SafeDelete
#define SafeDelete(p)\
if (p){ delete (p); (p) = NULL; }
#endif

#ifndef SafeDeletes
#define SafeDeletes(p)\
if (p){ delete[](p); (p) = NULL; }
#endif

#ifndef SafeCloseHandle
#define SafeCloseHandle(h)\
if (h){ CloseHandle(h); (h) = NULL; }
#endif

#ifndef SafeCloseFile
#define SafeCloseFile(pf)\
if (pf){ fclose(pf); (pf) = NULL; }
#endif

#ifndef WaitCloseThread
#define WaitCloseThread(hhh, ttt)\
if (hhh && ttt != 0){ WaitForSingleObject(hhh, ttt); }\
	SafeCloseHandle(hhh);
#endif

#ifndef SafeFreeLibrary
#define SafeFreeLibrary(h)\
if (h){ FreeLibrary(h); (h) = NULL; }
#endif

#ifndef SafeReleaseP
#define SafeReleaseP(p)\
{if (p){ (p)->Release(); (p) = NULL; }}
#endif


#ifndef SafeAddRef
#define SafeAddRef(p)\
{if (p){ p->AddRef(); }}
#endif

#define CheckNull(p)\
if (!p){ return E_ParameterNull; }

#ifndef CheckFailP
#define CheckFailP(hr, p)\
if (FAILED(hr) || !(p)){ return hr; }
#endif


#ifndef MAILSETBSTR
#define MAILSETBSTR(pIn, p)\
if (pIn){ (p) = SysAllocString(pIn); }
#endif

#ifndef SAFEFRREBSTR
#define SAFEFRREBSTR(p)\
if (p){ SysFreeString(p); (p) = NULL; }
#endif

#ifndef CheckPointer
#define CheckPointer(p,ret) { if((p)==NULL) return (ret);}
#endif

#define MyClearPath(szPath)\
{\
	int nCl1 = (int)_tcslen(szPath) - 1; \
while (nCl1 >= 0)\
{\
if (szPath[nCl1] == '\\')\
	nCl1--; \
		else\
		break; \
	}\
	szPath[nCl1 + 1] = '\0'; \
	}

#define MyParentPath(szFullPath)\
{\
	int nnnlen = (int)_tcslen(szFullPath) - 1; \
while (nnnlen >= 0)\
{\
if (szFullPath[nnnlen] == '\\')\
{\
	szFullPath[nnnlen] = '\0'; \
	break; \
	}\
	nnnlen--; \
	}\
}

#define _MyLLPrintf(szStr, llData)\
if (szStr)\
{\
	DWORD high = llData >> 32; \
	DWORD low = llData & 0xFFFFFFFF; \
	_stprintf(szStr, _T("%08X%08X"), high, low); \
	}


#define _AddXGanString(szXString)\
if (szXString && (szXString[_tcslen(szXString) - 1] != '\\'))\
{\
	_tcscat(szXString, _T("\\")); \
	}

#define SafeMemcpy(pcpdst, pcpsrc, cpsize)\
if (pcpdst && pcpsrc)\
{\
	memcpy(pcpdst, pcpsrc, cpsize); \
	}

#define  EXIT_ON_ERROR(hr) \
if (FAILED(hr)){ goto Exit; }



//////////////////////////////////////////////////////////////////////////
//for ExportFriend
#define SAFEFREEBSTR(bstr) \
if (bstr){ SysFreeString(bstr); }

#define  SAFE_DEL_HTTPFILE(p) \
if (p) { (p)->Close(); delete (p); (p) = 0; }

#define  EXIT_ON_NULL(p,hr) \
if ((p) == NULL) { (hr) = E_FAIL; goto Exit; }

#define  SAFE_CLOSE_HTTPCONN(p) \
if (p) { (p)->Close(); }

#define  EXIT_ON_NULL2(p) \
if ((p) == NULL) { goto Exit; }





//#define USECLIENTNEW

#ifndef SAFERELEASE
#define SAFERELEASE(p)	{if(p){(p)->Release();(p) = 0;}}
#endif

#ifndef ___STR
#define ___STR(x) #x
#endif

#define DEFAULTTIMETCPWAIT		30000
#define DEFAULTTIMETCPDIFF		500

#if _MSC_VER <= 1200
#ifndef __in
#define __in
#define __out
#endif
#endif // _MSC_VER > 1200

#ifndef _OUTPUT_STRHEADER
#define _OUTPUT_STRHEADER "test"
#endif

#ifndef _OUTPUT_STRHEADERA
#define _OUTPUT_STRHEADERA "test"
#endif

#ifndef VMBEGIN
#define  VMBEGIN {\
{__asm _emit 0xEB }; \
{__asm _emit 0x10 }; \
{__asm _emit 0x56 }; \
{__asm _emit 0x4D }; \
{__asm _emit 0x50 }; \
{__asm _emit 0x72 }; \
{__asm _emit 0x6F }; \
{__asm _emit 0x74 }; \
{__asm _emit 0x65 }; \
{__asm _emit 0x63 }; \
{__asm _emit 0x74 }; \
{__asm _emit 0x20 }; \
{__asm _emit 0x62 }; \
{__asm _emit 0x65 }; \
{__asm _emit 0x67 }; \
{__asm _emit 0x69 }; \
{__asm _emit 0x6E }; \
{__asm _emit 0x00 }; \
}
#define  VMEND {\
{__asm _emit 0xEB }; \
{__asm _emit 0x0E }; \
{__asm _emit 0x56 }; \
{__asm _emit 0x4D }; \
{__asm _emit 0x50 }; \
{__asm _emit 0x72 }; \
{__asm _emit 0x6F }; \
{__asm _emit 0x74 }; \
{__asm _emit 0x65 }; \
{__asm _emit 0x63 }; \
{__asm _emit 0x74 }; \
{__asm _emit 0x20 }; \
{__asm _emit 0x65 }; \
{__asm _emit 0x6E }; \
{__asm _emit 0x64 }; \
{__asm _emit 0x00 }; \
}
#endif



#ifndef ARCAPI
#define ARCAPI virtual HRESULT WINAPI
#endif

#ifndef ARCAPI_
#define ARCAPI_(x) virtual x WINAPI
#endif

#ifndef ARCAPIN_
#define ARCAPIN_(x) x WINAPI
#endif





#ifndef WAIT_TIMEOUT /* also in winerror.h */
#define WAIT_TIMEOUT 258
#endif
#ifndef WAIT_OBJECT_0
#define WAIT_OBJECT_0 0
#endif
#ifndef WAIT_FAILED
#define WAIT_IO_COMPLETION 0xC0
#define WAIT_ABANDONED 128
#define WAIT_FAILED ((DWORD)0xFFFFFFFF)
#endif

typedef struct tagMYENVENTWAIT{
	BOOL	bSet;
	tagMYENVENTWAIT * pNext;
	tagMYENVENTWAIT * pPrev;
}MYENVENTWAIT, *PMYENVENTWAIT;
class MyEvent
{
public:
	MyEvent(BOOL bSet, BOOL bManuleReset);
	virtual ~MyEvent();

	void Set();
	void Reset();
	DWORD Wait(INT dwWait);

#ifdef USEWINDOW
	//	void *		m_HEvent;
	CRITICAL_SECTION m_mtx;

#else
#endif

	BOOL	m_bmanule;
	BOOL	m_bOut;
	MYENVENTWAIT m_we;
	PMYENVENTWAIT m_pwe;
};

class MyAtomic
{
public:
	MyAtomic();
	virtual ~MyAtomic();
	INT Inc();
	INT Dec();
	void Set(INT nset);
#ifdef USEWINDOW
	CRITICAL_SECTION m_mtx;
#else
#endif
	INT m_n;
};

class MyLK
{
public:
	MyLK();
	virtual ~MyLK();
	BOOL	lk();
	void	ulk();
#ifdef USEWINDOW
	CRITICAL_SECTION lk_mtx;
#else
#endif
};

#define _MYLOCKIT	if(bLock){if(!lk())return;}
#define _MYLOCKITN(RTN)	if(bLock){if(!lk())return (RTN);}
#define _MYUNLOCKIT	if(bLock){ulk();}



#define MYLOCKIT	if(bLock){if(!Lock())return;}
#define MYLOCKITN(RTN)	if(bLock){if(!Lock())return (RTN);}
#define MYUNLOCKIT	if(bLock){Unlock();}


class MyLock
{
public:
	MyLock(const char * lpName = 0);
	virtual ~MyLock();
	BOOL	Lock(DWORD dwTime = -1);
	void	Unlock();
protected:
	MyEvent	* m_pEvent;
};



#ifndef USEWINDOW
#ifdef WIN32 
#define USEWINDOW
#endif
#ifndef _WINDOWS
#define USEWINDOW
#endif
#endif

#ifdef USEWINDOW
//#define WINVER 0x0500
#pragma warning(disable:4996)
#endif
#ifdef USEWINDOW
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define mystricmp stricmp

typedef int socklen_t;

#else
#endif

#ifndef E_POINTER
#define E_POINTER -2
#define E_NOINTERFACE 0x80000004L
#define NOERROR 0
#ifndef __unknwn_h__
const GUID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
#endif
#else
#ifndef __unknwn_h__
const GUID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
#endif
#endif

#ifndef PURE
#define PURE =0
#endif




#ifndef MAX_PATH
#define MAX_PATH 260
#endif
#if 0
#define mystdcall __stdcall
#else
#define mystdcall
#endif
#ifndef ARCAPI
#define ARCAPI	virtual HRESULT mystdcall
#endif
#ifndef ARCAPI_
#define ARCAPI_(x)	virtual x mystdcall
#endif

#ifndef SafeDelete
#define SafeDelete(p)\
if (p){ delete (p); (p) = NULL; }
#endif

#ifndef SafeDeletes
#define SafeDeletes(p)\
if (p){ delete[](p); (p) = NULL; }
#endif

#ifndef DEFFUN_ZLIB_COMPRESS
#define DEFFUN_ZLIB_COMPRESS
typedef INT(__stdcall *FUN_ZLIB_COMPRESS)(LPBYTE pDst, LPDWORD pDstLen, LPBYTE	source, DWORD dwSrcLen);
typedef INT(__stdcall *FUN_ZLIB_UNCOMPRESS)(LPBYTE pDst, LPDWORD pDstLen, LPBYTE	source, DWORD dwSrcLen);
#endif

int LogW(const char* pFormat, ...);

#ifndef MYSTRNCPY
#ifdef USEWINDOW
#define  MYSTRNCPY strncpy
#else
#define  MYSTRNCPY strncpy
#endif
#endif



//#define LPUNKNOWN IUnknown *

#ifndef CheckPointer
#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}
#endif
//typedef struct { volatile int counter; } atomic_t;
class CBaseUN
	:public MyAtomic
{
public:
	CBaseUN()
	{
		Set(0);
		AddRef();
	}
	virtual ~CBaseUN()
	{
	}
	inline static ULONG ourmax(const ULONG & a, const ULONG & b)
	{
		return a > b ? a : b;
	}
	BOOL IsEqualGUIDN(const GUID & rguid1, const GUID & rguid2)
	{
		return (
			((DWORD *)&rguid1)[0] == ((DWORD *)&rguid2)[0] &&
			((DWORD *)&rguid1)[1] == ((DWORD *)&rguid2)[1] &&
			((DWORD *)&rguid1)[2] == ((DWORD *)&rguid2)[2] &&
			((DWORD *)&rguid1)[3] == ((DWORD *)&rguid2)[3]);
	}


	ARCAPI QueryInterface(const GUID & riid, void **ppvObject)
	{
		CheckPointer(ppvObject, E_POINTER);
		if (IsEqualGUIDN(riid, IID_IUnknown)){
			*ppvObject = (void*)((CBaseUN*) this);
			AddRef();
			return NOERROR;
		}
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	ARCAPI_(ULONG) AddRef()
	{
		Inc();
		return ourmax((ULONG)(m_n), 1ul);
	}
	ARCAPI_(ULONG) Release()
	{
		LONG lRef = Dec();
		if (lRef <= 0)
		{
			Set(1);
			delete this;
			return (ULONG)(0);
		}
		else {
			return ourmax((ULONG)(m_n), 1ul);
		}
	}
protected:

};


#ifndef SafeReleaseP
#define SafeReleaseP(p)\
{if (p){ (p)->Release(); (p) = NULL; }}
#endif


#ifndef SafeAddRef
#define SafeAddRef(p)\
{if (p){ (p)->AddRef(); }}
#endif





class MyArrayData
	: public CBaseUN
{
public:
	MyArrayData(){ m_pNext = 0; m_pPrev = 0; }
	virtual ~MyArrayData(){};
	ARCAPI_(MyArrayData*) GetNext(){ return m_pNext; }
	ARCAPI_(void)SetNext(MyArrayData * pNext){ m_pNext = pNext; }
	ARCAPI_(MyArrayData*) GetPrev(){ return m_pPrev; }
	ARCAPI_(void)SetPrev(MyArrayData * pPrev){ m_pPrev = pPrev; }
protected:
	MyArrayData *		m_pNext;
	MyArrayData *		m_pPrev;
};

class MyArray : public MyLK
{
public:
	MyArray();
	virtual ~MyArray();
	ARCAPI_(MyArrayData*)	PopData(MyArrayData * pTo, BOOL bLock = TRUE);
	ARCAPI_(MyArrayData*)	PopHead(BOOL bLock = TRUE);
	ARCAPI_(void)			PushHead1(MyArrayData * pSB, BOOL bLock = TRUE);
	ARCAPI_(MyArrayData*)	PopTail1(BOOL bLock = TRUE);
	ARCAPI_(void)			PushTail(MyArrayData * pSB, BOOL bLock = TRUE);
	ARCAPI_(void)			ClearArray(BOOL bLock = TRUE);
	ARCAPI_(MyArrayData*)	GetHead(BOOL bLock = TRUE);
	ARCAPI_(MyArrayData*)	PopDataN(MyArrayData * pTo, BOOL bLock = TRUE);

	DWORD	GetNum(){ return m_dwNum; }
	MyArrayData	* GetF(){ return m_pFirst; }
	MyArrayData	* GetL(){ return m_pLast; }

	LONG check();
protected:
	MyArrayData		*	m_pFirst;
	MyArrayData		*	m_pLast;
	DWORD						m_dwNum;
};


class MyUnArray : public MyArray
{
public:
	ARCAPI_(void)				PushHead1(MyArrayData * pSB, BOOL bLock = TRUE);
	ARCAPI_(void)				PushTail(MyArrayData * pSB, BOOL bLock = TRUE);
	ARCAPI_(void)			ClearArray(BOOL bLock = TRUE);
	ARCAPI_(MyArrayData*)	GetHead(BOOL bLock = TRUE);
};


extern int g_limits_Setting;

void MyOutput(const CHAR* ptzFormat, ...);

void MyOutputW(const TCHAR* ptzFormat, ...);




enum LogType{
	LOG_NULL = -1,
	LOG_ZERO = 0,
	LOG_ALL = 1,
	LOG_doWorkTCP = 2,
	LOG_doWorkUDP = 4,
	LOG_doWorkDecVideo = 8,
	LOG_doWorkDecAudio = 16,
	LOG_UDPPUSH = 32,
	LOG_LOSTFRAME = 64,
	LOG_AVT = 128,

	LOG_DRAW = 256,
	LOG_UIBC = 512,
	LOG_TIMEOUT = 1024,

	LOG_DEALUDPPUSH = 2048,
};

void  LogOut(LogType etype, const CHAR * FuntionName, ...);


extern LogType g_strlevel;




interface IFp3d
	:public IUnknown
{
	ARCAPI			RenderData(HWND hWindow, long x, long y, LPSIZE pSizeS, int isactive, RECT* rcSrc) PURE;
	ARCAPI			OnRefreshData(void * pvData, long DataMode, long lDataLen, LONG nw, LONG nh) PURE;
};

IFp3d * CreateFpD3D(HWND hWnd);



enum enDataMod
{
	enD_X8R8G8B8 = 0,
	enD_R8G8B8
};

class CDrawFrame
	//:public CDDUnknown<IDrawFrame>
{
public:
	CDrawFrame();
	~CDrawFrame();


	virtual HRESULT __stdcall         Init(HWND hWnd, interface ICallBack * pDFCB);
	virtual VOID __stdcall  Exit();

	virtual HRESULT __stdcall         OnRefreshData(void * pvData, long DataMode, long lDataLen, RECT * prtSrc, RECT * prtDst, BOOL bDrawNow);
	virtual HRESULT __stdcall         SetShowMode(long nShowMode);

	virtual HRESULT __stdcall 			RenderData(HDC hDc, long x, long y, LPSIZE pSizeS, int nm, int isactive, RECT* rcSrc);
private:
	void ResetData(long nMaxData);
	MyLock	 m_lock;


	HWND m_hWnd;
	interface ICallBack * m_pDFCB;

	void * m_pvData;
	long   m_nMaxDataLen;

	long	m_nDibNum;
	long	m_w;
	long	m_h;



	RECT m_rtDst;


	interface IFp3d * m_pIFp3d;



	void ResetDataOut();
	long m_wOut;
	long m_hOut;
	void * m_pvDataOut;
	long m_nMaxDataLenOut;

	long m_wOutN;
	long m_hOutN;


	int m_isneedrender;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif

	// Encoder Type
#define ENC_X264		1

	// PicCode
#define ENC_YUV			1
#define ENC_YV12		2

	// Enc Mode
#define ENC_DEFAULT 0
#define ENC_320_240_8K 1
#define ENC_640_480_8K 2
#define ENC_640_480_15K 3
#define ENC_1280_720_50K 4
#define ENC_720_576_80K	5
#define ENC_320_240_15K 6
#define ENC_352_288_20K 7
#define ENC_640_480_30K 8
#define ENC_1280_720_100K 9
#define ENC_640_480_50K 10
#define ENC_640_480_80K 11
#define ENC_640_480_Half 12
#define ENC_1920_1080_200K 0x10

	//Frame Type
	enum FrameType
	{
		ENC_FRAME_AUTO = 0,
		ENC_FRAME_IDR = 1,
		ENC_FRAME_B = 5,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef struct
	{
		DWORD	dwSize;
		DWORD	EncType;
		DWORD	PicCode;

		DWORD	Width;
		DWORD	Height;

		DWORD	dwFPS;
		DWORD   dwBitRate;

		DWORD	dwEncMode;


		DWORD	dwMaxt;
		DWORD	dwMint;
		DWORD	dwFPSN;
		DWORD	dwBitRateN;
		DWORD	dwMH;
		DWORD	dwCPU;

		DWORD	dwThrad;
		DWORD	dwQuality;
		DWORD	dwDur;
		DWORD	dwCpuSpeed;
	}EncConfig, *PEncConfig;

	typedef struct
	{
		DWORD	dwSize;

		//
		// Picture specific
		//
		DWORD	PicCode;			// YUV

		LONGLONG rtStart;
		LONGLONG rtStop;

		PVOID   pbYUV[3];			// Do not modify this frame buffer and would be referenced;
		DWORD	dwStride;

		DWORD	frameidx;
	}InSample, *PInSample;

	typedef struct
	{
		DWORD dwSize;
		PBYTE pbSample;
		DWORD dwSampleLen;
		FrameType	frametype;

		LONGLONG rtStart;			// -1 means invalid
		LONGLONG rtStop;

		DWORD	frameidx;
	}OutSample, *POutSample;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	interface IEncoder : public IUnknown
	{
	public:
		virtual HRESULT	Config(PEncConfig pConfig) = 0;
		virtual HRESULT	Reset() = 0;
		virtual HRESULT Destory() = 0;

		virtual HRESULT Encode(PInSample pIn, POutSample* pOut) = 0;
		virtual HRESULT Flush(POutSample pOut) = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	class ITakeOut
	{
	public:
		virtual HRESULT __stdcall OnGetOut(void * pvout, int nout)PURE;
	};

#ifdef __cplusplus
}
#endif

class IMyH264D
{
public:
	virtual HRESULT __stdcall Init() = 0;
	//HRESULT SetVideoFormat(UINT fmt, UINT nw, UINT nh, UINT ndx, UINT ndur, UINT raitox, UINT raitoy, MFVideoInterlaceMode    m_interlace);
	virtual HRESULT __stdcall Exit() = 0;
	virtual HRESULT __stdcall PushYV12(void * pv, int nw, int nh, int ndx, int ndur, ITakeOut * pOut, LONGLONG llTime, LONGLONG LLDur, int ncur) = 0;

	virtual HRESULT __stdcall Encode(void * pv, int nw, int nh, int ndx, const LONGLONG & llstarts,
		int nheadlen, long & nAll, DWORD & nOldMax, unsigned char * & pOld, LONGLONG & llstartd) = 0;

	virtual HRESULT __stdcall Config(PEncConfig pConfig) = 0;

	virtual HRESULT __stdcall SetInfo(int nw, int nh, int ndur) = 0;
	virtual HRESULT __stdcall PushInData(LPBYTE pbin, DWORD dwin, LONGLONG llTime) = 0;
	virtual HRESULT __stdcall GetOutData(LPBYTE pbout, DWORD & dwout, LONGLONG & llTime) = 0;

	virtual HRESULT __stdcall Reset() = 0;
};
void __stdcall API_FreeMyH264D(IMyH264D * pv);
IMyH264D * __stdcall API_CreateMyH264D(int num);

typedef void(__stdcall *MyAPI_FreeMyH264D)(IMyH264D * pv);
typedef IMyH264D * (__stdcall *MyAPI_CreateMyH264D)(int num);


class IMyAACD
{
public:
	virtual HRESULT __stdcall Init() = 0;
	//HRESULT SetVideoFormat(UINT fmt, UINT nw, UINT nh, UINT ndx, UINT ndur, UINT raitox, UINT raitoy, MFVideoInterlaceMode    m_interlace);
	virtual HRESULT __stdcall Exit() = 0;

	virtual HRESULT __stdcall SetInfo(WAVEFORMATEX * pfe, int islpcm) = 0;
	virtual HRESULT __stdcall PushInData(LPBYTE pbin, DWORD dwin, LONGLONG llTime, LONGLONG * plldtime) = 0;
	virtual HRESULT __stdcall GetOutData(LPBYTE pbout, DWORD & dwout, LONGLONG & llTime) = 0;

	virtual HRESULT __stdcall Reset() = 0;
};
void __stdcall API_FreeMyAACD(IMyAACD * pv);
IMyAACD * __stdcall API_CreateMyAACD(int num);

typedef void(__stdcall *MyAPI_FreeMyAACD)(IMyAACD * pv);
typedef IMyAACD * (__stdcall *MyAPI_CreateMyAACD)(int num);




#define TSGETPID(pb)  ((pb)[1]|(((pb)[0]&0x1f)<<8))
//((*(DWORD*)(pv))& 0x00FF1F00)
#define TSGETLEN(pb) ((pb)[1]|(((pb)[0]&0xf)<<8)) 
//(((*(DWORD*)(pv)) & 0x00FF0F00)>>8)
#define TSGETNUM(pb)  ((pb)[1]|(((pb)[0])<<8))
//(*(WORD*)(pv))



#pragma pack(1)

#define MAXVFRAME 0x1000000
// Transport packet header
typedef struct tagTS_packet_header
{
	unsigned char sync_byte : 8;

	unsigned char PID1 : 5;
	unsigned char transport_priority : 1;
	unsigned char payload_unit_start_indicator : 1;
	unsigned char transport_error_indicator : 1;

	unsigned char PID2;

	unsigned char continuity_counter : 4;
	unsigned char adaption_field_control : 2;
	unsigned char transport_scrambling_control : 2;
} TS_packet_header;

typedef struct tagTS_PAT_START
{
	unsigned char table_id : 8; //固定为0x00 ，标志是该表是PAT表  

	unsigned char section_length1 : 4;
	unsigned char reserved_1 : 2; // 保留位  
	unsigned char zero : 1; //0  
	unsigned char section_syntax_indicator : 1; //段语法标志位，固定为1  

	unsigned char section_length2 : 8; //表示从下一个字段开始到CRC32(含)之间有用的字节数  

	unsigned char transport_stream_id1; //该传输流的ID，区别于一个网络中其它多路复用的流  

	unsigned char transport_stream_id2; //该传输流的ID，区别于一个网络中其它多路复用的流  

	unsigned char current_next_indicator : 1; //发送的PAT是当前有效还是下一个PAT有效  
	unsigned char version_number : 5; //范围0-31，表示PAT的版本号  
	unsigned char reserved_2 : 2;// 保留位  

	unsigned char section_number : 8; //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段  

	unsigned char last_section_number : 8;  //最后一个分段的号码  

}TS_PAT_START;


typedef struct tagTS_PAT_Program

{
	unsigned char program_number1;  //节目号  
	unsigned char program_number2;  //节目号  
	unsigned char program_map_PID1 : 5; // 节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个  
	unsigned char program_map_res : 3; // 节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个  
	unsigned char program_map_PID2 : 8; // 节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个  

}TS_PAT_Program;


//	TS_PAT_Program  program[1];
typedef struct tagTS_PAT_END
{
	//unsigned char network_PID1 : 5; //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID  
	//unsigned char reserved_3 : 3; // 保留位  
	//unsigned char network_PID2 : 8; //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID  
	unsigned int CRC_32 : 32;  //CRC32校验码  
} TS_PAT_END;



typedef struct  tagTS_PMT_Stream
{
	unsigned char stream_type : 8; //指示特定PID的节目元素包的类型。该处PID由elementary PID指定  

	unsigned char elementary_PID1 : 5; //该域指示TS包的PID值。这些TS包含有相关的节目元素  
	unsigned char res1 : 3; //该域指示TS包的PID值。这些TS包含有相关的节目元素  

	unsigned char elementary_PID2 : 8; //该域指示TS包的PID值。这些TS包含有相关的节目元素  

	unsigned char ES_info_length1 : 4; //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数  
	unsigned char res2 : 4; //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数  
	unsigned char ES_info_length2 : 8; //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数  

	//unsigned char descriptor;
}TS_PMT_Stream;
// PMT table
// Program Map Table
typedef struct tagTS_PMTSTART
{
	unsigned char table_id : 8;  //固定为0x02,表示PMT表


	unsigned char section_length1 : 4;//首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。
	unsigned char reserved_1 : 2;//0x03	
	unsigned char zero : 1;
	unsigned char section_syntax_indicator : 1;//固定为0x01

	unsigned char section_length2 : 8;

	unsigned char program_number1 : 8; //指出该节目对应于可应用的Program map PID
	unsigned char program_number2 : 8;

	unsigned char current_next_indicator : 1;//当该位置1时，当前传送的Programmap section可用；/当该位置0时，指示当前传送的Program map section不可用，下一个TS流的Program map section有效。
	unsigned char version_number : 5; //指出TS流中Program map section的版本号
	unsigned char reserved_2 : 2;

	unsigned char section_number : 8;//固定为0x00

	unsigned char last_section_number : 8;//固定为0x00

	unsigned char PCR_PID1 : 5;//指明TS包的PID值，该TS包含有PCR域，//该PCR值对应于由节目号指定的对应节目。
	//如果对于私有数据流的节目定义与PCR无关，这个域的值将为0x1FFF。
	unsigned char reserved_3 : 3; //0x07


	unsigned char PCR_PID2 : 8;


	unsigned char program_info_length1 : 4; //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。
	unsigned char reserved_4 : 4;//预留为0x0F

	unsigned char program_info_length2 : 8;

	//unsigned reserved_5 : 3; //0x07
	//unsigned reserved_6 : 4; //0x0F
	//unsigned CRC_32 : 32; 
}TS_PMTSTART;



typedef struct tagTS_PMTEND
{
	unsigned int  CRC_32 : 32;
} TS_PMTEND;



typedef struct  tagRTP_HEAD
{
	unsigned char csrc : 4; //CC:CSRC计数 4bits
	unsigned char X : 1; //X：扩展 1bits
	unsigned char P : 1; //P：填充 1bits
	unsigned char version : 2; //V：版本号，始终为2 2bits

	unsigned char Payload_type : 7; //Payload type(PT) : 负载类型 7 bits
	unsigned char M : 1; //M：标志 1bits


	unsigned char Sequence_number1; //Sequence number(SN) : 序列号 16 bits
	unsigned char Sequence_number0; //Sequence number(SN) : 序列号 16 bits

	unsigned char  Timestamp3; //时间戳 32 bits
	unsigned char  Timestamp2; //时间戳 32 bits
	unsigned char  Timestamp1; //时间戳 32 bits
	unsigned char  Timestamp0; //时间戳 32 bits

	unsigned char  synchronization_source3; //同步源标志
	unsigned char  synchronization_source2; //同步源标志
	unsigned char  synchronization_source1; //同步源标志
	unsigned char  synchronization_source0; //同步源标志


}RTP_HEAD;


struct AACPES
{
	unsigned char syncword1;

	unsigned char protection_absent : 1;
	unsigned char layer : 2;
	unsigned char id : 1;
	unsigned char syncword2 : 4;

	unsigned long channel_configuration1 : 1;
	unsigned long private_bit : 1;
	unsigned long sampling_frequency_index : 4;
	unsigned long profile : 2;

	unsigned char channel_configuration2 : 2;
	unsigned char original_copy : 1;
	unsigned char home : 1;
	unsigned char copyright_identification_bit : 1;
	unsigned char copyright_identification_start : 1;
	unsigned char frame_length1 : 2;

	unsigned char frame_length2;

	unsigned char frame_length3 : 3;
	unsigned char adts_buffer_fullness1 : 5;

	unsigned char adts_buffer_fullness2 : 6;
	unsigned char number_of_raw_data_blocks_in_frame : 2;
};



typedef struct tagPESHEAD
{
	unsigned long 	stream_id : 8; //1 0b10111101[private stream1]
	unsigned long packet_start_code_prefix : 24;//3 0x00 00 01


	unsigned short 	PES_packet_length; //2 0x07 8E : if HDCP 2.x is not used	//0x07 9F : if HDCP 2.x is used


	unsigned char original_or_copy : 1;// 0b1[original] or 0b0[copy]
	unsigned char copyright : 1;// 0b0[not defined]
	unsigned char data_alignment_indicator : 1;// 0b0[not defined]
	unsigned char PES_priority : 1;// 0b0[no priority]
	unsigned char PES_scrambling_control : 2;// 0b00[not scrambled]
	unsigned char Bex10 : 2;//0b10



	unsigned char PES_extension_flag : 1;// 0b0 : if HDCP 2.x is not used//0b1 : if HDCP 2.x is used
	unsigned char PES_CRC_flag : 1;// 0b0[not present]
	unsigned char additional_copy_info_flag : 1;// 0b0[not present]
	unsigned char DSM_trick_mode_flag : 1;// 0b0[not present]
	unsigned char ES_rate_flag : 1;// 0b0[not present]
	unsigned char ESCR_flag : 1;// 0b0[not present]
	unsigned char PTS_DTS_flag : 2;// 10b[present]








	unsigned  char PES_header_data_length;// 1 0x07 : if HDCP 2.x is not used//0x18 : if HDCP 2.x is used

	unsigned  char marker_bit32_30 : 1;// 0b1
	unsigned  char PTS32_30 : 3;// shall be set to correct value
	unsigned  char Bex0010 : 4; //0b0010


	unsigned short marker_bit29_15 : 1;// 0b1
	unsigned short PTS29_15 : 15;// shall be set to correct value


	unsigned short marker_bit14_0 : 1;// 0b1
	unsigned short PTS14_0 : 15;// shall be set to correct value



	unsigned char PES_extension_flag_2 : 1;// 0b0[not present](note - 1)
	unsigned char Reserved : 3;// 0b111 (note - 1)
	unsigned char P_STD_buffer_flag : 1;// 0b0[not present](note - 1)
	unsigned char program_packet_sequence_counter_flag : 1;// 0b0[not present](note - 1)
	unsigned char pack_header_field_flag : 1;// 0b0[not present](note - 1)
	unsigned char PES_private_data_flag : 1;//0b1[present](note - 1)


	unsigned char PES_private_data[16];// 16 HDCP 2.x counter values and marker_bit.See[21]. (note - 1)
	unsigned char stuffing_bytes[2]; // 2 0xFF F

}PESHEAD;



typedef struct tagPESHEADold
{
	unsigned long byte000001 : 24;
	unsigned long stream_id : 8;
	unsigned short len;

	unsigned char b10 : 2;
	unsigned char PES_scrambling_control : 2;
	unsigned char PES_priority : 1;
	unsigned char data_alignment_indicator : 1;
	unsigned char copyright : 1;
	unsigned char original_or_copy : 1;

	unsigned char PTS_DTS_flag : 2;
	unsigned char ESCR_flag : 1;
	unsigned char ES_rate_flag : 1;
	unsigned char DSM_trick_mode_flag : 1;
	unsigned char additional_copy_info_flag : 1;
	unsigned char PES_CRC_flag : 1;
	unsigned char PES_extension_flag : 1;

	unsigned char PES_header_data_length;

	unsigned char bit0010 : 4;
	unsigned char pts32_30 : 3;
	unsigned char ptsmsk1 : 1;
	unsigned short pts29_15 : 15;
	unsigned short ptsmsk2 : 1;
	unsigned short pts14_0 : 15;
	unsigned short ptsmsk3 : 1;


	unsigned char PES_private_data_flag : 1;
	unsigned char pack_header_field_flag : 1;
	unsigned char program_packet_sequence : 1;
	unsigned char P_STD_buffer_flag : 1;
	unsigned char Reserved1 : 3;
	unsigned char PES_extension_flag_2 : 1;

	unsigned char PES_private_data[16];

	unsigned char stuffingbytes[2];
}PESHEADOld;

struct LPCMPAYLOADHEAD
{
	unsigned char sub_stream_id;
	unsigned char number_of_frame_header;

	unsigned char reserved : 7;
	unsigned char audio_emphasis_flag : 1;

	unsigned char quantization_word_length : 2;
	unsigned char audio_sampling_frequency : 3;
	unsigned char number_of_audio_channel : 3;
};

struct H264SPS
{
	unsigned char ui0;//0x67 sps  0x68 pps
	unsigned char profile_idc;//0x42 = baseprofile??//66 (PROFILE_IDC_Baseline)（01000010）

	unsigned char 	reserved_zero_4bits : 4;// = 0
	unsigned char 	constraint_set3_flag : 1;// = 0
	unsigned char 	constraint_set2_flag : 1;// = 0
	unsigned char 	constraint_set1_flag : 1;// = 0
	unsigned char 	constraint_set0_flag : 1;// = 0


	unsigned char 	level_idc;// = 0x1f



	unsigned char 	seq_parameter_set_id : 1;// = 0 ue_v
	unsigned char 	log2_max_frame_num_minus4 : 1;// = 0 ue_v
	unsigned char 	pic_order_cnt_type : 1;// = 0 ue_v
	unsigned char 	log2_max_pic_order_cnt_lsb_minus4 : 1;// = 0 ue_v
	unsigned char 	num_ref_frames1 : 4;// = 5 ue_v

	unsigned char 	num_ref_frames2 : 1;// = 5 ue_v
	unsigned char   gaps_in_frame_num_value_allowed_flag : 1;// = 0
	unsigned char   un11 : 1;// = 0
	unsigned char   pic_width_in_mbs_minus1 : 5;// = 10 (176) ue_v

	unsigned char   pic_width_in_mbs_minus2 : 6;// = 10 (176) ue_v
	unsigned char   pic_height_in_map_units_minus1 : 2;// = 8 (144) ue_v

	unsigned char   pic_height_in_map_units_minus2;// = 8 (144) ue_v

	unsigned char   pic_height_in_map_units_minus3 : 1;// = 8 (144) ue_v


	unsigned char   frame_mbs_only_flag : 1;// = 1
	unsigned char   direct_8x8_inference_flag : 1;// = 1
	unsigned char   frame_cropping_flag : 1;// = 0
	unsigned char   vui_parameters_present_flag : 1;// = 0
	unsigned char   un : 3;// = 0
};



typedef struct   tagVideoFromatTable
{
	//char  tableid; // 0 table 5-10 , 1 table 5-11 , 2 table 5-12
	char  index; // 0-16             0-30                     0-12
	char type; //0  p 帧     1 i 帧
	int nw;// 宽
	int nh;//高
	int frames;//帧数

	DWORD indexand;

}VideoFromatTable, *pVideoFromatTable;

#pragma pack()

class MyRTPINFO
{
public:
	MyRTPINFO();
	~MyRTPINFO();

	union
	{
		RTP_HEAD *  m_prtp;
		unsigned char *		m_pbrtp;
	};
	union
	{
		TS_packet_header *  m_pts;
		unsigned char *		m_pbts;
	};
	union
	{
		TS_PAT_START * m_ptspat;
		TS_PMTSTART  * m_ptspmt;
		unsigned  char * m_pbtsdata;

	};
	int m_isidr;
	DWORD m_dwnidid;
	DWORD m_dwpmtid;
	DWORD m_vdpid;
	DWORD m_adpid;
	DWORD m_aid;


	unsigned char m_bvid;
	unsigned char m_baid;

	long m_noff;

	unsigned char * m_vp;
	unsigned int m_vl;
	unsigned int m_vmax;
	int m_nbv;
	unsigned long long m_ullvpts;

	unsigned char * m_ap;
	unsigned int m_al;
	unsigned int m_amax;
	int m_nba;
	unsigned long long m_ullapts;

	DWORD m_dww;
	DWORD m_dwh;
	WAVEFORMATEX m_wfe;
	DWORD m_dwpid;

	int m_isdvr;

	int dopeshead(LPBYTE pbts, int & iOff, unsigned char & _bid, unsigned long long & _ullpts);
	void getptsdts(unsigned char * pb, unsigned long long * pts);
	int CheckStart(unsigned char * pbts);
	unsigned char * GetTsData(unsigned char * pbts);

};


#define MAX_BUFF_LEN 0x1000
#define MAX_TABLE_LEN 32
extern VideoFromatTable g_table5_10[MAX_TABLE_LEN];
extern VideoFromatTable g_table5_11[MAX_TABLE_LEN];
extern VideoFromatTable g_table5_12[MAX_TABLE_LEN];
void SetTheVideoFromat5_10(VideoFromatTable * vft);
void SetTheVideoFromat5_11(VideoFromatTable * vft);
void SetTheVideoFromat5_12(VideoFromatTable * vft);
void GetVideoFromtTable(VideoFromatTable *  pvft, int tableid, int index);

int getSettingTable5_10(char * sPath, VideoFromatTable * tableget);
int getSettingTable5_11(char * sPath, VideoFromatTable * tableget);
int getSettingTable5_12(char * sPath, VideoFromatTable * tableget);
int gettableuse(char * sPath);



//apifree(MyBufInfo)
#define AUDIOIDUSELPCM 0x83
struct MyBufInfo
{
public:
	DWORD	m_dwLen;
	unsigned long long m_lltime;
	unsigned char * m_pbdata;
	union
	{
		struct
		{
			int m_nw;
			int m_nh;
			int m_nisBlack;
			int m_nbkSize;
		};

		struct
		{
			int m_aid;
			WAVEFORMATEX  m_wfe;
		};
	};
	void * m_pvthis;
};

class CMyBufferList
{
public:
	CMyBufferList(DWORD dwMaxBufferSize, DWORD dwdefaultnum);// :m_dwMaxBufferSize(dwMaxBufferSize);
	virtual ~CMyBufferList();
	MyBufInfo *	GetBuffer1();
	void		ClearBuffer1(MyBufInfo * pBuffer);


	//class CMyBufferItem * GetItem();
	//void		ClearItem(class CMyBufferItem * pitem);



	DWORD		m_dwMaxBufferSize;
	int				m_listnum;
	int				m_listnumtemp;
	void *			m_pvlist;
};



class CMyBufferArray
{
public:
	CMyBufferArray();
	virtual ~CMyBufferArray();

	MyBufInfo *	PopHead();
	MyBufInfo *	GetHead();
	void		PushTail(MyBufInfo * pBuffer);
	int GetNum();

	HANDLE		m_pHandle;
	void *			m_pvlist;
};


int api_addrefbuf(MyBufInfo * pI);
int api_releasebuf(MyBufInfo * pI);

void lib_doWorkDealBuff(
class WorkThreadEx * pWork,
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
	);



#ifdef USEWINDOW
#else
#endif

class MyThread
{
public:
	MyThread();
	virtual ~MyThread();
	BOOL IsWork();
	void F_Excute0();

	BOOL StartThread();
	BOOL CloseThread(INT dwWait = 0);
#ifdef USEWINDOW
	HANDLE	m_hThread;
	DWORD	m_dwThreadID;
	//HANDLE m_hThreadStop;
#else
	//sigset_t m_set;
	pthread_t m_tid;
#endif
	MyEvent	* m_eThread;
	MyEvent	* m_eStop;

	MyLock	m_lkThread;
	BOOL	m_bStart;
	BOOL	m_bEnd;

	virtual void ThreadExcute();

};


class WorkThreadEx;
class IMyWork
{
public:
	virtual BOOL __stdcall OnSetStop(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData) = 0;
	virtual BOOL __stdcall 	OnSetStart(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData) = 0;
	virtual BOOL __stdcall DoWork(WorkThreadEx * pWork, DWORD dwIndex) = 0;
};

class WorkThreadEx
	:public MyArrayData
	, public MyThread
{
public:
	WorkThreadEx();
	virtual ~WorkThreadEx();
	virtual BOOL __stdcall 	SetStop(DWORD dwData, DWORD dwWait);
	BOOL IsFree(){ return m_pMyWork == NULL; }
	BOOL	IsNeedWork(DWORD dwWaitTime = 0);
	BOOL AddMyWork(IMyWork * pMyWork, DWORD dwParam, DWORD dwData,
		DWORD	dwCmd,
		LPVOID	inPm,
		DWORD	inPmLen,
		LPVOID	bkPm,
		DWORD	bkPmLen
		, DWORD dwWait);
	virtual BOOL __stdcall SetWorkParam(DWORD	dwCmd,
		LPVOID	inPm,
		DWORD	inPmLen,
		LPVOID	bkPm,
		DWORD	bkPmLen);
	void						ThreadExcute();
	MyLock					m_lk;
	IMyWork *			m_pMyWork;
	DWORD				m_dwParam;
	BOOL					m_bWork;
	DWORD				m_dwCmd;
	LPBYTE					m_bkPm;
	DWORD				m_bkPmLen;
	DWORD				m_bkPmLenMax;
	LPBYTE					m_inPm;
	DWORD				m_inPmLen;
	DWORD				m_inPmLenMax;

};

class Work_ThreadS
	:public IMyWork
	, public MyLK
{
public:
	Work_ThreadS();
	virtual ~Work_ThreadS();

	virtual BOOL __stdcall 			OnSetStop(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData){ return TRUE; };
	virtual BOOL __stdcall 			OnSetStart(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData){ return TRUE; };
	virtual BOOL __stdcall 			DoWork(WorkThreadEx * pWork, DWORD dwIndex){ return TRUE; };



	WorkThreadEx * 					WorkInThread(DWORD	dwCmd,
		LPVOID	inPm,
		DWORD	inPmLen,
		LPVOID	bkPm,
		DWORD	bkPmLen);


private:
	void	* m_pvlists;
};

WorkThreadEx * API_NewWorkThread(MyUnArray * pList, BOOL bCreate, IMyWork * pMyWork, DWORD dwParam, DWORD dwData,
	DWORD	dwCmd,
	LPVOID	inPm,
	DWORD	inPmLen,
	LPVOID	bkPm,
	DWORD	bkPmLen);

#ifndef defSafeRelease
#define defSafeRelease
template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
#endif




#define MAXPARAM 500
class IMyAudioPlay;
class MyAACD :public IMyAACD
{
private:
	union
	{
		char m_Param[MAXPARAM];
		struct
		{
			int m_decmode;
			void * m_hAac;
			int m_isinithac;
			void * m_outbuffer;
			int m_outlen;





			int					m_maxsamplesizei;
			int					m_maxsamplesizeo;
			int					m_insamplenum;
			IMFSample		**	m_ppInSample;
			IMFSample		*	m_pOutSample;


			IMFTransform	*	m_pDecoder;
			IMFMediaType * m_pmto;
			IMFMediaType * m_pmti;

			UINT32			m_dfc;


			GUID					m_subtype;
			WAVEFORMATEX			m_wfe;

			UINT32                   m_nw;
			UINT32                   m_nh;
			UINT32					m_ndur;
			UINT32					m_raitox;
			UINT32					m_raitoy;
			MFVideoInterlaceMode    m_interlace;

			UINT32		m_epf;
			eAVEncCommonRateControlMode m_em;
			ULONG m_ncpu;
			bool m_iscabac;
			ULONG m_nBFrame;
			ULONG m_nGOPSize;
			ULONG m_nthread;
			ULONG m_nQuality;
			ULONG m_nQP;
			ULONG m_nMinQP;
			ULONG m_iskeyframe;
			ULONG m_nbufsizemax;
			ULONG m_nbufsizeavg;

			IMFSample *m_pDstSample;


			UINT32					m_DXVA;//use dxva
			UINT32					m_osc;//out sample count
			UINT32					m_d3da;

			ULONG					m_lowlat;//LowLatencyMode
			ULONG					m_nmaxw;//maxw
			ULONG					m_nmaxh;//maxh
			ULONG					m_tgm;
		};
	};
public:

	MyAACD(int innum){
		memset(m_Param, 0, MAXPARAM);
		m_insamplenum = innum;
		m_ppInSample = new IMFSample *[m_insamplenum];
		memset(m_ppInSample, 0, sizeof(IMFSample*)*m_insamplenum);
		m_pOutSample = 0;


		m_maxsamplesizei = 1920 * 1080 * 4 * 2;
		m_maxsamplesizeo = m_maxsamplesizei;
		m_raitox = 1;
		m_raitoy = 1;


		m_subtype = MFVideoFormat_YV12;
		m_nw = 1280;
		m_nh = 720;
		m_ndur = 33333333;
		m_raitox = 1;
		m_raitoy = 1;
		m_interlace = MFVideoInterlace_Progressive;

		m_epf = 0;
		m_em = eAVEncCommonRateControlMode_Quality;
		m_ncpu = 1;
		m_iscabac = 0;
		m_nBFrame = 0;
		m_nGOPSize = 0;
		m_nthread = 1;
		m_nQuality = 45;
		m_nQP = 24;
		m_nMinQP = 0;
		m_iskeyframe = 0;
		m_nbufsizemax = 1000000;
		m_nbufsizeavg = 1000000;

		m_nmaxw = 1920;
		m_nmaxh = 1080;
		m_osc = 1;

		m_wfe.wFormatTag = WAVE_FORMAT_PCM;
		m_wfe.nChannels = 2;
		m_wfe.nSamplesPerSec = 48000;
		m_wfe.wBitsPerSample = 16;
		m_wfe.nBlockAlign = m_wfe.nChannels * m_wfe.wBitsPerSample / 8;
		m_wfe.nAvgBytesPerSec = m_wfe.nBlockAlign * m_wfe.nSamplesPerSec;

	};
	virtual ~MyAACD(){
		Exit();
		m_insamplenum = 0;
		if (m_ppInSample != NULL){
			delete[] m_ppInSample;
			m_ppInSample = 0;
		}
	};

	virtual HRESULT __stdcall Init();
	virtual HRESULT __stdcall Exit();


	virtual HRESULT __stdcall SetInfo(WAVEFORMATEX * pfe, int islpcm);
	virtual HRESULT __stdcall PushInData(LPBYTE pbin, DWORD dwin, LONGLONG llTime, LONGLONG * plldtime);
	virtual HRESULT __stdcall GetOutData(LPBYTE pbout, DWORD & dwout, LONGLONG & llTime);

	virtual HRESULT __stdcall Reset();

	HRESULT SetInOutFmt();

	HRESULT SetVideoFormat(UINT fmt, UINT nw, UINT nh, UINT ndx, UINT ndur, UINT raitox, UINT raitoy, MFVideoInterlaceMode    m_interlace);

	HRESULT PushData(IMFSample * pSample, int ischange, ITakeOut * pOut);

	IMFSample * FindFreeSample();
};

#define MAXPARAM 500
class MyH264D :public IMyH264D
{
private:
	union
	{
		char m_Param[MAXPARAM];
		struct
		{
			int					m_maxsamplesizei;
			int					m_maxsamplesizeo;
			int					m_insamplenum;
			IMFSample		**	m_ppInSample;
			IMFSample		*	m_pOutSample;


			IMFTransform	*	m_pDecoder;
			IMFMediaType * m_pmto;
			IMFMediaType * m_pmti;

			UINT32			m_dfc;


			GUID					m_subtype;
			UINT32                   m_nw;
			UINT32                   m_nh;
			UINT32					m_ndur;
			UINT32					m_raitox;
			UINT32					m_raitoy;
			MFVideoInterlaceMode    m_interlace;

			eAVEncH264VProfile		m_epf;
			eAVEncCommonRateControlMode m_em;
			ULONG m_ncpu;
			bool m_iscabac;
			ULONG m_nBFrame;
			ULONG m_nGOPSize;
			ULONG m_nthread;
			ULONG m_nQuality;
			ULONG m_nQP;
			ULONG m_nMinQP;
			ULONG m_iskeyframe;
			ULONG m_nbufsizemax;
			ULONG m_nbufsizeavg;

			IMFSample *m_pDstSample;


			UINT32					m_DXVA;//use dxva
			UINT32					m_osc;//out sample count
			UINT32					m_d3da;

			ULONG					m_lowlat;//LowLatencyMode
			ULONG					m_nmaxw;//maxw
			ULONG					m_nmaxh;//maxh
			ULONG					m_tgm;
		};
	};
public:

	MyH264D(int innum){
		memset(m_Param, 0, MAXPARAM);
		m_insamplenum = innum;
		m_ppInSample = new IMFSample *[m_insamplenum];
		memset(m_ppInSample, 0, sizeof(IMFSample*)*m_insamplenum);
		m_pOutSample = 0;


		m_maxsamplesizei = 1920 * 1080 * 4 * 2;
		m_maxsamplesizeo = m_maxsamplesizei;
		m_raitox = 1;
		m_raitoy = 1;


		m_subtype = MFVideoFormat_YV12;
		m_nw = 1280;
		m_nh = 720;
		m_ndur = 33333333;
		m_raitox = 1;
		m_raitoy = 1;
		m_interlace = MFVideoInterlace_Progressive;

		m_epf = eAVEncH264VProfile_High;
		m_em = eAVEncCommonRateControlMode_Quality;
		m_ncpu = 1;
		m_iscabac = 0;
		m_nBFrame = 0;
		m_nGOPSize = 0;
		m_nthread = 1;
		m_nQuality = 45;
		m_nQP = 24;
		m_nMinQP = 0;
		m_iskeyframe = 0;
		m_nbufsizemax = 1000000;
		m_nbufsizeavg = 1000000;

		m_nmaxw = 1920;
		m_nmaxh = 1080;
		m_osc = 1;
	};
	virtual ~MyH264D(){
		Exit();
		m_insamplenum = 0;
		if (m_ppInSample != NULL){
			delete[] m_ppInSample;
			m_ppInSample = 0;
		}
	};

	virtual HRESULT __stdcall Init();
	virtual HRESULT __stdcall Exit();
	virtual HRESULT __stdcall PushYV12(void * pv, int nw, int nh, int ndx, int ndur, ITakeOut * pOut, LONGLONG llTime, LONGLONG LLDur, int ncur);
	virtual HRESULT __stdcall Encode(void * pv, int nw, int nh, int ndx, const LONGLONG & llstarts,
		int nheadlen, long & nAll, DWORD & nOldMax, unsigned char * & pOld, LONGLONG & llstartd);

	virtual HRESULT __stdcall Config(PEncConfig pConfig);


	virtual HRESULT __stdcall SetInfo(int nw, int nh, int ndur);
	virtual HRESULT __stdcall PushInData(LPBYTE pbin, DWORD dwin, LONGLONG llTime);
	virtual HRESULT __stdcall GetOutData(LPBYTE pbout, DWORD & dwout, LONGLONG & llTime);

	virtual HRESULT __stdcall Reset();

	HRESULT SetInOutFmt();

	HRESULT SetVideoFormat(UINT fmt, UINT nw, UINT nh, UINT ndx, UINT ndur, UINT raitox, UINT raitoy, MFVideoInterlaceMode    m_interlace);

	HRESULT PushData(IMFSample * pSample, int ischange, ITakeOut * pOut);

	IMFSample * FindFreeSample();



};
#define  LOG_START  0x111
class CMyLog : public Work_ThreadS
{
public:
	CMyLog();
	~CMyLog();
	void StartLog();
	void StopLog();
	ARCAPI_(BOOL) DoWork(WorkThreadEx * pWork, DWORD dwIndex);
	void doWorkLog(WorkThreadEx * pWork);
	void pushLog(char *tzText2);
	void pushLog1(LogType etype, const CHAR * FuntionName, ...);

	WorkThreadEx * m_threadLog;

	CMyBufferArray m_loglist;
	HANDLE m_hlog;

	CMyBufferList * m_logdatas;
};

extern CMyLog * g_plog;

void releaseLogData();
void PushLogData(LogType etype, const CHAR * FuntionName, ...);
void api_stopworkex(WorkThreadEx * pe, DWORD dwreult, DWORD dwwait);
void api_freeworkex(WorkThreadEx * &pe);
BOOL api_isneedwork(WorkThreadEx * pe, DWORD dwwait);

template< typename T >
interface ILinkEnumable : public IUnknown
{
	ARCAPI	Lock() PURE;
	ARCAPI	UnLock() PURE;
	//ARCAPI	TryLock() PURE;

	ARCAPI_(ULONG) GetCount() PURE;
	ARCAPI GetItem(IN ULONG ulIndex, OUT T** pItem) PURE;
	ARCAPI RemoveAllItems() PURE;
	ARCAPI AddItem(IN T* pT) PURE;
	ARCAPI RemoveItem(T* pItem) PURE;
	ARCAPI RemoveItem(DWORD dwIndex) PURE;
	ARCAPI ReArrangePos(DWORD dwReIndex, DWORD dwBeforeIndex) PURE;
	ARCAPI MySort(bool(*cmp)(T*, T*) = NULL) PURE;
	ARCAPI InsertItem(DWORD dwPos, T *pItem) PURE;

	ARCAPI_(ULONG) GetIndex(T *pItem) PURE;
};

//////////////////////////////////////////////////////////////////////////
// Some pointer check function
//////////////////////////////////////////////////////////////////////////
#ifndef CHECK_POINTER
#define CheckPointer(p,ret) { _ASSERTE((p) != NULL);if((p)==NULL) return (ret);}
#define ValidateReadPtr(p,cb) {_ASSERTE( IsBadReadPtr((PVOID)p, cb) == FALSE);}
#define ValidateWritePtr(p,cb) {_ASSERTE( IsBadWritePtr((PVOID)p, cb) == FALSE);}
#define ValidateReadWritePtr(p,cb) {ValidateReadPtr(p,cb); ValidateWritePtr(p,cb);}
#define ValidateStringPtr(p) { _ASSERTE(IsBadStringPtr((LPCTSTR)p, INFINITE) == FALSE);}
#define ValidateStringPtrA(p) { _ASSERTE(IsBadStringPtrA((LPCSTR)p, INFINITE) == FALSE);}
#define ValidateStringPtrW(p) { _ASSERTE(IsBadStringPtrW((LPCWSTR)p, INFINITE) == FALSE);}
#endif

__forceinline HRESULT GetInterface(LPUNKNOWN pUnk, void **ppv)
{
	CheckPointer(ppv, E_POINTER);
	*ppv = pUnk;
	pUnk->AddRef();
	return NOERROR;
}

#define BEGIN_QUERYINTERFACE	\
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)\
{ CheckPointer(ppvObject, E_POINTER); ValidateReadWritePtr(ppvObject, sizeof(PVOID));

#define ADD_INTERFACE(RIID, IOBJ)	\
{ if (riid == RIID){ return GetInterface((IOBJ*)(IOBJ*) this, ppvObject); } }
#define ADD_INTERFACE2(RIID, IOBJ)	\
{ if (riid == RIID){ IOBJ* p = new IOBJ(); *ppvObject = p; return NOERROR; } }

#define END_QUERYINTERFACE	\
{ *ppvObject = NULL; return E_NOINTERFACE; } }

template < typename T >
class CDDUnknown : public T
{
public:
	CDDUnknown()
		: m_cRef(0)
	{
		AddRef();
	}
	virtual ~CDDUnknown()
	{
	}

	inline static ULONG ourmax(const ULONG & a, const ULONG & b)
	{
		return a > b ? a : b;
	}
	BEGIN_QUERYINTERFACE;
	ADD_INTERFACE(IID_IUnknown, IUnknown)
		END_QUERYINTERFACE;

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		LONG lRef = InterlockedIncrement(&m_cRef);
		_ASSERTE(lRef > 0);

		return ourmax(ULONG(m_cRef), 1ul);
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		LONG lRef = InterlockedDecrement(&m_cRef);
		_ASSERTE(lRef >= 0);

		if (lRef == 0)
		{

			// COM rules say we must protect against re-entrancy.
			// If we are an aggregator and we hold our own interfaces
			// on the aggregatee, the QI for these interfaces will
			// addref ourselves. So after doing the QI we must release
			// a ref count on ourselves. Then, before releasing the
			// private interface, we must addref ourselves. When we do
			// this from the destructor here it will result in the ref
			// count going to 1 and then back to 0 causing us to
			// re-enter the destructor. Hence we add an extra refcount here
			// once we know we will delete the object.
			// for an example aggregator see filgraph\distrib.cpp.

			m_cRef++;

			delete this;
			return ULONG(0);
		}
		else {
			return ourmax(ULONG(m_cRef), 1ul);
		}
	}

protected:
#if (_MSC_VER > 1310)
	volatile LONG	m_cRef;
#else
	LONG	m_cRef;
#endif
};

template < class T >
class CDDNoAddRefReleasePtr : public T
{
private:
	STDMETHOD_(ULONG, AddRef)() = 0;
	STDMETHOD_(ULONG, Release)() = 0;
};

template < class T >
class CDDPtr
{
public:
	CDDPtr() throw()
	{
		p = NULL;
	}

	CDDPtr(T* lp) throw()
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}

	~CDDPtr() throw()
	{
		if (p)
			p->Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		_ASSERTE(p != NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		_ASSERTE(p == NULL);
		return &p;
	}
	CDDNoAddRefReleasePtr<T>* operator->() const throw()
	{
		_ASSERTE(p != NULL);
		return (CDDNoAddRefReleasePtr<T>*)p;
	}
	bool operator!() const throw()
	{
		return (p == NULL);
	}
	bool operator!=(T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(T* pT) const throw()
	{
		return p == pT;
	}
	T* operator=(T* lp) throw()
	{
		if (*this != lp)
		{
			return static_cast<T*>(DDPtrAssign((IUnknown**)&p, lp));
		}
		return *this;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	//// Compare two objects for equivalence
	//bool IsEqualObject(IUnknown* pOther) throw()
	//{
	//	if (p == NULL && pOther == NULL)
	//		return true;	// They are both NULL objects

	//	if (p == NULL || pOther == NULL)
	//		return false;	// One is NULL the other is not

	//	CComPtr<IUnknown> punk1;
	//	CComPtr<IUnknown> punk2;
	//	p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
	//	pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
	//	return punk1 == punk2;
	//}
	//// Attach to an existing interface (does not AddRef)
	//void Attach(T* p2) throw()
	//{
	//	if (p)
	//		p->Release();
	//	p = p2;
	//}
	//// Detach the interface (does not Release)
	//T* Detach() throw()
	//{
	//	T* pt = p;
	//	p = NULL;
	//	return pt;
	//}
	//HRESULT CopyTo(T** ppT) throw()
	//{
	//	ATLASSERT(ppT != NULL);
	//	if (ppT == NULL)
	//		return E_POINTER;
	//	*ppT = p;
	//	if (p)
	//		p->AddRef();
	//	return S_OK;
	//}
	//HRESULT SetSite(IUnknown* punkParent) throw()
	//{
	//	return AtlSetChildSite(p, punkParent);
	//}
	//HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw) throw()
	//{
	//	return AtlAdvise(p, pUnk, iid, pdw);
	//}
	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}
	template <class Q>
	HRESULT QueryInterface(Q** pp) const throw()
	{
		ATLASSERT(pp != NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
protected:
	inline IUnknown* DDPtrAssign(IUnknown** pp, IUnknown* lp)
	{
		if (pp == NULL)
			return NULL;

		if (lp != NULL)
			lp->AddRef();
		if (*pp)
			(*pp)->Release();
		*pp = lp;
		return lp;
	}
private:
	T* p;
};

#ifndef ___STR
#define ___STR(x) #x
#endif

#define DEFAULTTIMETCPWAIT		30000
#define DEFAULTTIMETCPDIFF		500


#ifndef DEFFUN_ZLIB_COMPRESS
#define DEFFUN_ZLIB_COMPRESS
typedef INT(__stdcall *FUN_ZLIB_COMPRESS)(LPBYTE pDst, LPDWORD pDstLen, LPBYTE	source, DWORD dwSrcLen);
typedef INT(__stdcall *FUN_ZLIB_UNCOMPRESS)(LPBYTE pDst, LPDWORD pDstLen, LPBYTE	source, DWORD dwSrcLen);
#endif


#define ASG_E_CANCELLED				HRESULT_FROM_WIN32(ERROR_CANCELLED)
#define ASG_E_TIMEOUT				HRESULT_FROM_WIN32(ERROR_TIMEOUT)
#define ASG_E_CONNECTION_ABORTED	HRESULT_FROM_WIN32(ERROR_CONNECTION_ABORTED)


#define MYMSG_SHOW	WM_USER + 0x200

#define	NOTWORKSOCKET	0x7fffffff

#define MYCMDBJ	0x12345678


#define MYCMD_BAD				0x89ab0002
#define MYCMD_CREATEDLSERVER	0x89ab0003
#define MYCMD_DLSERVEROK		0x89ab0004

#define MYCMD_GETDLLIST			0x89ab0005
#define MYCMD_GETDLLISTOK		0x89ab0006

#define MYCMD_DECONNECTDL		0x89ab0007
#define MYCMD_DECONNECTDLOK		0x89ab0008



#define MAX_REMOTEDL			6


struct ConnectDataOther
{
	DWORD dwSize;
	union
	{
		struct
		{
			DWORD	dwRealID;
			DWORD	dwUserID;
			DWORD	dwDeviceID;
		};
		BYTE		ConnectData[12];
	};
};

struct	tagDLList
{
	DWORD				dwSize;
	DWORD				dwNum;
	ConnectDataOther	DLList[MAX_REMOTEDL];
};

typedef struct tagMYCMDINFO
{
	DWORD	dwSize;
	DWORD	dwBJ;//MYCMDBJ
	DWORD	dwCMD;
	union
	{
		BYTE	bData[1012];
		struct
		{
			DWORD	peerIDTo;
			WORD	wPortTo;
			WORD	wOther;
			DWORD	peerIDDL;

			DWORD	dwUserID;
			DWORD	dwDeviceID;


			sockaddr_in	AddrGet;

		}DLSERVER;

		tagDLList	DLLIST;

		DWORD	dwpeerIDDel;
	};

}MYCMDINFO, *PMYCMDINFO;


enum	enMSCD_Type
{
	enMSCD_Min = 0,
	//	enMSCD_Local,
	enMSCD_sockaddr_in,
	enMSCD_peerName,
	enMSCD_Max = 100

};
#define MAX_PEER_LEN	64
#define NEEDRECINFONUM	200
typedef struct tagMyServerConnectData
{
	enMSCD_Type	enType;
	sockaddr_in	addrTo;
	DWORD		peerID;

	ConnectDataOther	OtherData;

}MYSERVERCONNECTDATA, *PMYSERVERCONNECTDATA;


#define ZEROMYSTRUCT(p)	{memset(&(p),0,sizeof(p));(p).dwSize = sizeof(p);}


#ifndef MYCREATEINSTANCE
#define MYCREATEINSTANCE
typedef HRESULT(*MyCreateInstance)(GUID iid, IUnknown** ppIns);
#endif















//#define USE_DSOUND
//#include "AudioPly.h"
//#include "CapV.h"
//#include "CapA.h"


//struct XpGinaBuffer
//{
//	BOOL	bGetOK;
//	_BMInfo	bi;
//	RECT rtSrc;
//};
typedef struct tag_BMInfo {
	BOOL			truecolour;
	BITMAPINFO		bmi;
	// Colormap info - comes straight after BITMAPINFO - **HACK**
	RGBQUAD			cmap[256];
}_BMInfo, *P_BMInfo;
enum NatType
{
	eInvalidNATType = 0,
	ePublic,
	eFullCone,
	eRestrictedCone,
	eRestrictedPortCone,
	eSymmetric,
	eTotalCount
};


__forceinline BOOL IsSameMSCD(PMYSERVERCONNECTDATA p0, PMYSERVERCONNECTDATA p1)
{
	if (p0->enType == p1->enType)
	{
		switch (p0->enType)
		{
		case enMSCD_peerName:
			if (p0->peerID == p1->peerID)
				return memcmp(&(p0->addrTo), &(p1->addrTo), sizeof(sockaddr_in)-8) == 0;
			break;
		case enMSCD_sockaddr_in:
			return memcmp(&(p0->addrTo), &(p1->addrTo), sizeof(sockaddr_in)-8) == 0;
		}
	}
	return false;
}






BOOL MyCreateFolder(char * src, BOOL bLastFolder);

DWORD Reg_RPeerID(HKEY hKeyMain, char * pSub, char *pName);
LONG Reg_WPeerID(HKEY hKeyMain, char * pSub, char * pName, DWORD peerID);
LONG Reg_RString(HKEY hKeyMain, char * pSub, char *pName, char * pStr, DWORD dwMaxNum);
LONG Reg_WString(HKEY hKeyMain, char * pSub, char * pName, char * pStr);

LONGLONG MyGetCurTime();
void TryOutputTime(const char * pHead, LONGLONG llStart);
void StaticOutputInfoTime(const char * pHead, DWORD pRecData_dwSize);

BOOL	API_DrawDataToWindow(HWND hWnd, LPBYTE pData, LONG w, long h, long nBit);

void	API_SetHookAlloc(BOOL bSet);

#define MAXSERVERADD 16
struct CONNECT_ID_DATA
{
	SOCKET	sk;
	HANDLE 	hEventLog;
	LONG	nIndex;

};
LONGLONG  API_xtol(char pSrc[], DWORD dwNumLen);

WORD API_GetScancode(WORD vk, BYTE & bEx);
UINT API_TopWindow(HWND hWndSet);

int		API_IsFileExist(const char * filename);
//BOOL	API_FindExistingDevice(LPCSTR cHardwareIDs);
//HRESULT API_DoInstallDevice(LPCSTR cHardwareIDs,LPCSTR pPath);



enum enShellCmd
{
	enSC_Null = 0,
	enSC_LoadDevice = '0',
	enSC_TopWindow = '1',
	enSC_Other
};



enum enWinVer
{
	enWV_Null = 0,
	enWV_NT40,
	enWV_WIN95,
	enWV_WIN98,
	enWV_WINMe,
	enWV_WIN2K,
	enWV_WINXP,
	enWV_WINXP_64,
	enWV_WIN2003,
	enWV_WIN2003R2,
	enWV_WINVISTA,
	enWV_WIN2008,
	enWV_WIN2008R2,
	enWV_WIN7,
};

__forceinline BOOL   API_IsWow64()
{
	typedef   BOOL(WINAPI   *LPFN_ISWOW64PROCESS)   (HANDLE, PBOOL);
	static LPFN_ISWOW64PROCESS fnIsWow64Process = 0;
	if (!fnIsWow64Process)
		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleA("kernel32 "), "IsWow64Process ");
	BOOL   bIsWow64 = FALSE;
	if (fnIsWow64Process)
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	return   bIsWow64;
}


__forceinline BOOL   API_IsX86()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	switch (sysinfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		return FALSE;//X64
	case PROCESSOR_ARCHITECTURE_IA64:
		return FALSE;//iA64
	case PROCESSOR_ARCHITECTURE_INTEL:
		return TRUE;//x86
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
	default:
		return FALSE;//unknown
	}
	return FALSE;
}


__forceinline enWinVer API_GetSystemVersion(WORD & wProcessorArchitecture)
{
#ifndef SM_SERVERR2
#define SM_SERVERR2             89
#endif
	enWinVer ev = enWV_Null;
	SYSTEM_INFO sysinfo;
	ZeroMemory(&sysinfo, sizeof(sysinfo));
	GetSystemInfo(&sysinfo);
	wProcessorArchitecture = sysinfo.wProcessorArchitecture;
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx((OSVERSIONINFO *)&osvi);
	switch (osvi.dwMajorVersion)
	{
	case 4:
		switch (osvi.dwMinorVersion)
		{                //判断次版本号 
		case 0:
			if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
				ev = enWV_NT40;//vname=_T("Microsoft Windows NT 4.0");                //1996年7月发布 
			else if (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				ev = enWV_WIN95;//vname=_T("Microsoft Windows 95"); 
			break;
		case 10:
			ev = enWV_WIN98;//vname=_T("Microsoft Windows 98"); 
			break;
		case 90:
			ev = enWV_WINMe;//vname=_T("Microsoft Windows Me"); 
			break;
		}
		break;
	case 5:
		switch (osvi.dwMinorVersion)
		{               //再比较dwMinorVersion的值 
		case 0:
			ev = enWV_WIN2K;//vname=_T("Microsoft Windows 2000");                    //1999年12月发布 
			break;
		case 1:
			ev = enWV_WINXP;//vname=_T("Microsoft Windows XP");                    //2001年8月发布 
			break;
		case 2:
			if (osvi.wProductType == VER_NT_WORKSTATION && wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				ev = enWV_WINXP_64;//vname=_T("Microsoft Windows XP Professional x64 Edition"); 
			else if (GetSystemMetrics(SM_SERVERR2) == 0)
			{
				ev = enWV_WIN2003;//vname=_T("Microsoft Windows Server 2003");        //2003年3月发布 
			}
			else if (GetSystemMetrics(SM_SERVERR2) != 0)
			{
				ev = enWV_WIN2003R2;//vname=_T("Microsoft Windows Server 2003 R2"); 
			}
			break;
		}
		break;
	case 6:
		switch (osvi.dwMinorVersion)
		{
		case 0:
			if (osvi.wProductType == VER_NT_WORKSTATION)/*VER_NT_WORKSTATION是桌面系统 */
			{
				ev = enWV_WINVISTA;//vname=_T("Microsoft Windows Vista"); 
			}
			else
			{
				ev = enWV_WIN2008;//vname=_T("Microsoft Windows Server 2008");          //服务器版本 
			}
			break;
		case 1:
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				ev = enWV_WIN7;//vname=_T("Microsoft Windows 7"); 
			}
			else
			{
				ev = enWV_WIN2008R2;//vname=_T("Microsoft Windows Server 2008 R2"); 
			}
			break;
		}
		break;
	}
	return ev;
}



LONG RegGinaXp(char * pGinaPath);
LONG UnregGinaXp();

#define NOTUSESERVERRUN
#ifndef NOTUSESERVERRUN
#define Global_PRE(name) "Global\\"##name
#else
#define Global_PRE(name) name
#endif

class CMyh264
{
public:
	CMyh264();
	~CMyh264();

	void OnNewData(char * prec, long len);

	void Init264Dll();
	//void h264Decode();
	BOOL  h264Decode(char*pbuff, int nsize);
	BOOL  GetDecodeV();
	BOOL  PushInDataA(char*pbuff, int nsize, LONGLONG llTime, LONGLONG * plldtime, WAVEFORMATEX * pfe, int islpcm);
	BOOL  PushInDataV(char*pbuff, int nsize, LONGLONG llTime);


	BOOL SetDecodeWHF(int nw, int nh, int frames);
	char *  ColorChange(char * pin, int nw, int nh);

	void MyCreateMyH264();
	void MyCreateMyH264D();
	void MyCreateMyAACD();
	void ExitAadio();
	void InitAadio();

	HMODULE m_hDllED_coder;
	HMODULE m_hDllED_coderD;
	HMODULE m_hDllaacd;

	HMODULE m_yuv2rgb;

	IMyH264D * m_myh264D;
	IMyAACD*m_myaacD;


	CDrawFrame * m_pdf;

	unsigned char * m_pout1;
	unsigned char * m_pout2;
	char *  m_pdata;
	int m_datasize;

	//memset(m_pdata, 0x80, 1920 * 1080 * 4);
	LONGLONG m_lltime2;
	LONGLONG m_lltime1;
	LONGLONG m_lltime;
	LONGLONG m_lldur;
	DWORD m_dwoutmax;




	DWORD m_dwout = 0;

	MyRTPINFO m_mr;
	LONGLONG m_lltime0;
	LONGLONG m_llstart;
	LONGLONG m_llcur;
	LONGLONG m_llOff;


	int m_nw;
	int m_nh;

	int m_isback;
	int m_isbackold;
	int m_isbackSize;

	IFp3d * m_P3D;
	void   MyCreateP3d(HWND hwnd);
	void	 MyOnRefreshData(void * pvData, long DataMode, long lDataLen, LONG nw, LONG nh);
	void	 MyRenderData(HWND hWindow, long x, long y, LPSIZE pSizeS, int isactive, RECT* rcSrc);
};