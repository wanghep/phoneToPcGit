#pragma once
#include "mmreg.h"
#include <vector>
using namespace std;

//#include "CMyRtpInfo.h"



typedef struct  tagMYWFD_VIDEO_FORMATS_HEAD
{
	int native;
	int preferred_display_mode_supported;
}MYWFD_VIDEO_FORMATS_HEAD;

typedef struct  tagMYWFD_VIDEO_FORMATS : public tagMYWFD_VIDEO_FORMATS_HEAD
{
	int profile;
	int level;
	int CEA_Support;
	int VESA_Support;
	int HH_Support;
	int latency;
	int min_slice_size;
	int slice_enc_params;
	int frame_rate_control_support;
	int max_hres;
	int max_vres;
}MyWfd_video_fromat, *pMyWfd_video_fromat;

enum DeviceState
{
	Idle,
	Connecting,
	Connected,
	Disconnecting,
	Disconnected,
	ConnectOuttime, //连接超时
	MYDisconnected, // 主动断开连接
	ServiceNotActive,
	ReInvalidData,
	ReInvalidParameter,
	GenFailure,
};

char * Api_GetMyIniPath(char  * sPath);
BOOL API_DrawDataNew(HDC hDc, LPRECT rtOut, LPBYTE pData, LONG ws, long hs, long nBit);
void SetNewWindowRect(RECT * rectin, RECT * rectout, double nw, double nh);
//extern char * g_pnData;




