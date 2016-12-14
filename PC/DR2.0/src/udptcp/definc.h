#pragma once

#include <SDKDDKVer.h>

#include "../../include/common/CommonMacro.h"
#pragma   warning(disable:4996)
#if _WIN32_WINNT < 0x0500
#define _WIN32_WINNT 0x0500 
#endif

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

#include <stdio.h>
#include <tchar.h>

#include <Windows.h>

#ifndef ARCAPI
#define ARCAPI virtual HRESULT WINAPI
#endif

#ifndef ARCAPI_
#define ARCAPI_(x) virtual x WINAPI
#endif

#ifndef ARCAPIN_
#define ARCAPIN_(x) x WINAPI
#endif


