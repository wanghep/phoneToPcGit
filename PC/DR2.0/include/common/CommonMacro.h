#ifndef _09_11_24_COMMON_MACRO_H
#define _09_11_24_COMMON_MACRO_H


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
	if(!ptzFormat)
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
	if(!ptzFormat)
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
	if(p){delete (p);(p)=NULL;}
#endif

#ifndef SafeDeletes
#define SafeDeletes(p)\
if(p){delete [] (p);(p)=NULL;}
#endif

#ifndef SafeCloseHandle
#define SafeCloseHandle(h)\
	if(h){CloseHandle(h); (h) = NULL;}
#endif

#ifndef SafeCloseFile
#define SafeCloseFile(pf)\
	if(pf){fclose(pf); (pf) = NULL;}
#endif

#ifndef WaitCloseThread
#define WaitCloseThread(hhh, ttt)\
	if(hhh && ttt != 0){WaitForSingleObject(hhh, ttt);}\
	SafeCloseHandle(hhh);
#endif

#ifndef SafeFreeLibrary
#define SafeFreeLibrary(h)\
if(h){FreeLibrary(h); (h) = NULL;}
#endif

#ifndef SafeReleaseP
#define SafeReleaseP(p)\
{if(p){(p)->Release(); (p) = NULL;}}
#endif


#ifndef SafeAddRef
#define SafeAddRef(p)\
{if(p){p->AddRef(); }}
#endif

#define CheckNull(p)\
	if(!p){return E_ParameterNull;}

#ifndef CheckFailP
#define CheckFailP(hr, p)\
if (FAILED(hr) || !(p)){return hr;}
#endif


#ifndef MAILSETBSTR
#define MAILSETBSTR(pIn, p)\
	if(pIn){(p) = SysAllocString(pIn);}
#endif

#ifndef SAFEFRREBSTR
#define SAFEFRREBSTR(p)\
	if(p){SysFreeString(p);(p) = NULL;}
#endif

#ifndef CheckPointer
#define CheckPointer(p,ret) { if((p)==NULL) return (ret);}
#endif

#define MyClearPath(szPath)\
{\
	int nCl1 = (int)_tcslen(szPath) - 1;\
	while (nCl1 >= 0)\
	{\
		if(szPath[nCl1] == '\\')\
			nCl1 --;\
		else\
			break;\
	}\
	szPath[nCl1 + 1] = '\0';\
}

#define MyParentPath(szFullPath)\
{\
	int nnnlen = (int)_tcslen(szFullPath) - 1;\
	while (nnnlen >= 0)\
	{\
		if(szFullPath[nnnlen] == '\\')\
		{\
		szFullPath[nnnlen] = '\0';\
		break;\
		}\
		nnnlen --;\
	}\
}

#define _MyLLPrintf(szStr, llData)\
if(szStr)\
{\
	DWORD high = llData >> 32;\
	DWORD low = llData & 0xFFFFFFFF;\
	_stprintf(szStr, _T("%08X%08X"), high, low);\
}


#define _AddXGanString(szXString)\
	if(szXString && (szXString[_tcslen(szXString) - 1] != '\\'))\
	{\
	_tcscat(szXString, _T("\\"));\
	}

#define SafeMemcpy(pcpdst, pcpsrc, cpsize)\
if(pcpdst && pcpsrc)\
{\
	memcpy(pcpdst, pcpsrc, cpsize);\
}

#define  EXIT_ON_ERROR(hr) \
	if(FAILED(hr)){goto Exit;}



//////////////////////////////////////////////////////////////////////////
//for ExportFriend
#define SAFEFREEBSTR(bstr) \
	if(bstr){SysFreeString(bstr);}

#define  SAFE_DEL_HTTPFILE(p) \
	if(p) { (p)->Close(); delete (p); (p) = 0;}

#define  EXIT_ON_NULL(p,hr) \
	if((p) == NULL) { (hr) = E_FAIL; goto Exit; }

#define  SAFE_CLOSE_HTTPCONN(p) \
	if(p) { (p)->Close();}

#define  EXIT_ON_NULL2(p) \
	if((p) == NULL) {goto Exit;}

#endif/*_09_11_24_COMMON_MACRO_H*/
