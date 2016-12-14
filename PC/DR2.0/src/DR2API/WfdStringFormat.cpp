#include "stdafx.h"
//#include "WorkThread.h"
//#include "MyArray.h"
#include "../../inc/wfdsink.h"

#include "WfdStringFormat.h"

#include "WfdSources.h"
//#include "AboutSocket.h"
#include "./udptcp.h"
#include "Rstdef.h"

#include "uibcstruct.h"


VideoFromatTable g_table5_10[MAX_TABLE_LEN];
VideoFromatTable g_table5_11[MAX_TABLE_LEN];
VideoFromatTable g_table5_12[MAX_TABLE_LEN];

// int getSettingTable5_10(char * sPath);
// int getSettingTable5_11(char * sPath);
// int getSettingTable5_12(char * sPath);
int gettableuse(char * sPath);

char * Api_GetMyIniPath(char  * sPath)
{
	CHAR	cModulePath[MAX_PATH] = { 0 };
	CHAR	cDrive[MAX_PATH] = { 0 };
	CHAR	cDir[MAX_PATH] = { 0 };
	CHAR	cED_coder[MAX_PATH] = { 0 };
	CHAR	cED_coderd[MAX_PATH] = { 0 };
	CHAR	cED_aacd[MAX_PATH] = { 0 };

	GetModuleFileNameA(0, cModulePath, MAX_PATH);
	_splitpath(cModulePath, cDrive, cDir, NULL, NULL);
	_makepath(cModulePath, cDrive, cDir, NULL, NULL);
	_makepath(cED_coder, cDrive, cDir, "setting.ini", NULL);

	sprintf(sPath, "%s", cED_coder);

	return sPath;

}





char * AddString(char  *  data, char * src)
{
	strcat(data, src);
	return 0;
}

CWfdStringFormat::CWfdStringFormat()
{

	m_setUdpPort = 0;
	m_IpService[0] = 0;
	m_sessionid[0] = 0;

}

CWfdStringFormat::~CWfdStringFormat()
{


}


char * CWfdStringFormat::SinkToSource1(char * sendbuf, int CSeqId)
{
	//char  sendbuf[1024] = { 0 };
	char  buf[1024] = { 0 }; // = sendbuf;
	//int i = CSeqId;

	time_t now = time(NULL);
	struct tm *now2 = gmtime(&now);
	char  buftime[128] = { 0 };
	strftime(buftime, sizeof(buftime), "%a, %b %d %Y %H:%M:%S", now2);

	AddString(buf, "RTSP/1.0 200 OK\r\n");
	AddString(buf, "CSeq: %d\r\n");
	AddString(buf, "Date: %s GMT\r\n");
	AddString(buf, "Public: org.wfa.wfd1.0, GET_PARAMETER, SET_PARAMETER\r\n");
	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, buftime);


	return buf;
}



char * CWfdStringFormat::SinkToSource2(char * sendbuf, int CSeqId)
{
	//char  sendbuf[1024] = { 0 };
	//char * buf = sendbuf;
	//int i = CSeqId;

	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "OPTIONS * RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");
	AddString(buf, "Require: org.wfa.wfd1.0\r\n");
	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId);

	return buf;
}
char * CWfdStringFormat::SinkToSource3(char * sendbuf, int CSeqId, char * recvbuf)
{
	char body[3048] = { 0 };
 //#define USE_PC_CONFIG





#ifdef USE_PC_CONFIG


	if (recvbuf != NULL)
	{
		const char * split = "\r\n";
		char * p = strtok(recvbuf, split);
		OutputDebugStringA(p);
		while (p != NULL)
		{
			p = strtok(NULL, split);
			if (p == NULL)
				break;
			if (strcmp(p, "wfd_audio_codecs") == 0)// 6.1.2
			{							//wfd_audio_codecs: LPCM 00000002 00, AAC 00000001 00

				AddString(body, "wfd_audio_codecs: LPCM 00000002 00, AAC 00000001 00\r\n");
			}
			else if (strcmp(p, "wfd_video_formats") == 0)// 6.1.3
			{
				//wfd_video_formats: 00 00 01 02 0001d6b4 05155555 00000555 00 0000 0000 00 none none


				char spath[260] = { 0 };
				Api_GetMyIniPath(spath);
				int nCEA = gettableuse(spath);

				MyWfd_video_fromat wfd_video_format;
				wfd_video_format.native = 0;
				wfd_video_format.preferred_display_mode_supported = 0;
				wfd_video_format.profile = 1;
				wfd_video_format.level = 1;
				wfd_video_format.CEA_Support = nCEA;
				wfd_video_format.VESA_Support = 0;// 0x5155555;
				wfd_video_format.HH_Support = 0;// 0x555;
				wfd_video_format.latency = 0;
				wfd_video_format.min_slice_size = 0;
				wfd_video_format.slice_enc_params = 0;
				wfd_video_format.frame_rate_control_support = 0;
				wfd_video_format.max_hres = m_SystemWidth;// 1920;
				wfd_video_format.max_vres = m_SystemHeight;// 1080;

				char sFormat[4096] = { 0 };

				//从配置文件里面读取数据

				if (wfd_video_format.preferred_display_mode_supported == 0)
				{
					sprintf(sFormat, "wfd_video_formats: %02x %02x %02x %02x %08x %08x %08x %02x %04x %04x %02x none none\r\n",
						wfd_video_format.native,
						wfd_video_format.preferred_display_mode_supported,
						wfd_video_format.profile,
						wfd_video_format.level,
						wfd_video_format.CEA_Support,
						wfd_video_format.VESA_Support,
						wfd_video_format.HH_Support,
						wfd_video_format.latency,
						wfd_video_format.min_slice_size,
						wfd_video_format.slice_enc_params,
						wfd_video_format.frame_rate_control_support
						);
						
				//wfd_video_formats: 00 00 02 10 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 08 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 04 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 02 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 01 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 10 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 08 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 04 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 02 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 01 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none

				//sprintf(sFormat, "wfd_video_formats: 00 00 02 10 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 08 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 04 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 02 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 02 01 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 10 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 08 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 04 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 02 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none, 01 01 0001FFFF 0F3FFFFF 00000FFF 00 0000 0000 13 none none\r\n");

				}
				else if (wfd_video_format.preferred_display_mode_supported == 1)
				{
					sprintf(sFormat, "wfd_video_formats: %02x %02x %02x %02x %08x %08x %08x %02x %04x %04x %02x %04x %04x\r\n",
						wfd_video_format.native,
						wfd_video_format.preferred_display_mode_supported,
						wfd_video_format.profile,
						wfd_video_format.level,
						wfd_video_format.CEA_Support,
						wfd_video_format.VESA_Support,
						wfd_video_format.HH_Support,
						wfd_video_format.latency,
						wfd_video_format.min_slice_size,
						wfd_video_format.slice_enc_params,
						wfd_video_format.frame_rate_control_support,
						wfd_video_format.max_hres,
						wfd_video_format.max_vres
						);
				}

				//AddString(body, "wfd_video_formats: none\r\n");

				AddString(body, sFormat);
				//static int isfirst = 0;
				//if (isfirst == 0)
				//	AddString(body, "wfd_video_formats: 00 00 01 02 00000080 05155555 00000555 00 0000 0000 00 none none\r\n");
				//else
				//AddString(body, "wfd_video_formats: 00 00 01 02 00000020 05155555 00000555 00 0000 0000 00 none none\r\n");
				//isfirst++;
			}
			else if (strcmp(p, "wfd_3d_formats") == 0)// 6.1.4
			{
				AddString(body, "wfd_3d_formats: none\r\n");
			}
			else if (strcmp(p, "wfd_content_protection") == 0)// 6.1.5
			{
				/************************************************************************/
				/*
				wfd-content-protection  = “wfd_content_protection:” SP cp-spec CRLF
				cp-spec  = “none” / hdcp2-spec
				hdcp2-spec  = (“HDCP2.0” / “HDCP2.1”) SP “port=” IPPORT ;
				TCP port                                                                     */
				/************************************************************************/
				if (1){
					AddString(body, "wfd_content_protection: none\r\n");
				}
				else{
					//AddString(body, "wfd_content_protection: HDCP2.1 port=4990\r\n");
					AddString(body, "wfd_content_protection: HDCP2.0 port=4990\r\n");
				}
			}
			else if (strcmp(p, "wfd_display_edid") == 0)// 6.1.6
			{
				AddString(body, "wfd_display_edid: none\r\n");
			}
			else if (strcmp(p, "wfd_coupled_sink") == 0)// 6.1.7
			{
				AddString(body, "wfd_coupled_sink: none\r\n");
			}
			else if (strcmp(p, "wfd_trigger_method") == 0)// 6.1.8
			{
				AddString(body, "wfd_trigger_method: none\r\n");
			}
			else if (strcmp(p, "wfd_presentation_url") == 0)// 6.1.9
			{
				AddString(body, "wfd_presentation_url: none\r\n");
			}
			else if (strcmp(p, "wfd_client_rtp_ports") == 0)// 6.1.10
			{
				AddString(body, "wfd_client_rtp_ports: RTP/AVP/UDP;unicast %d 0 mode=play\r\n");
			}
			else if (strcmp(p, "wfd_route") == 0)// 6.1.11
			{
				AddString(body, "wfd_route: none\r\n");
			}
			else if (strcmp(p, "wfd_I2C") == 0)// 6.1.12
			{
				AddString(body, "wfd_I2C: none\r\n");
			}
			else if (strcmp(p, "wfd_av_format_change_timing") == 0) // 6.1.13
			{
				AddString(body, "wfd_av_format_change_timing: none\r\n");
			}
			else if (strcmp(p, "wfd_preferred_display_mode") == 0)// 6.1.14
			{
				AddString(body, "wfd_preferred_display_mode: none\r\n");
			}
			else if (strcmp(p, "wfd_uibc_capability") == 0)// 6.1.15
			{
				AddString(body, "wfd_uibc_capability: input_category_list=GENERIC;generic_cap_list=Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard;hidc_cap_list=none;port=none\r\n");

				//AddString(body, "wfd_uibc_capability: input_category_list=GENERIC,HIDC;generic_cap_list=Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard;hidc_cap_list=Infrared, USB, BT, Zigbee, Wi-Fi, No-SP;port=none\r\n");
				//"input_category_list = GENERIC; generic_cap_list = Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard; hidc_cap_list = none; port = none"
				//"input_category_list = GENERIC; generic_cap_list = Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard; hidc_cap_list = none; port = none"
			}
			else if (strcmp(p, "wfd_uibc_setting") == 0)// 6.1.16
			{
				AddString(body, "wfd_uibc_setting: none\r\n");
			}
			else if (strcmp(p, "wfd_standby_resume_capability") == 0)// 6.1.17
			{
				AddString(body, "wfd_standby_resume_capability: supported\r\n");
			}
			else if (strcmp(p, "wfd_standby") == 0)// 6.1.18
			{
				AddString(body, "wfd_standby: none\r\n");
			}
			else if (strcmp(p, "wfd_connector_type") == 0)// 6.1.19
			{
				AddString(body, "wfd_connector_type: none\r\n");
			}
			else if (strcmp(p, "wfd_idr_request") == 0)// 6.1.20
			{
				AddString(body, "wfd_idr_request: none\r\n");
			}
			else if (strcmp(p, "intel_friendly_name") == 0)   //2.1.1.1 intel_friendly_name
			{
				/*
				intel-friendly-name = "intel_friendly_name:" SP friendly-name CRLF
				friendly-name = 1*18(utf8byte-no-hyphen)
				utf8byte-no-hyphen = %x00-2C / %x2E-FF
				*/
				if (1)
				{
					AddString(body, "intel_friendly_name: Contoso ScreenMaster 2000\r\n");
				}
				else 
				{
					//AddString(body, "intel_friendly_name: %x2E-FF\r\n");
				}
			}
			else if (strcmp(p, "intel_sink_device_URL") == 0) //2.1.1.2 intel_sink_device_URL
			{
				/*
				intel-sink-device-URL = "intel_sink_device_URL:" SP uri CRLF
				uri = 1*256(VCHAR) / "none"
				The intel_sink_device_URL parameter specifies a URI as specified in [RFC3986]. A value of "none"
				means that no product information is available.
				*/
				if (1)
				{
					AddString(body, "intel_sink_device_URL: none\r\n");
				}
				else 
				{
					AddString(body, "intel_sink_device_URL: http://www.example.com/screenmaster/\r\n");
				}
			}
			else if (strcmp(p, "intel_sink_manufacturer_logo") == 0)//2.1.1.3 intel_sink_manufacturer_logo
			{
				/*
				intel-sink-manufacturer-logo = "intel_sink_manufacturer_logo:" SP logo CRLF
				logo = "none" / base64_logo
				base64_logo = 464*76800(BASE64CHAR)
				A value of "none" means that no image is available
				*/
				if (1)
				{
					AddString(body, "intel_sink_manufacturer_logo: none\r\n");
				}
				else
				{
					AddString(body, "intel_sink_manufacturer_logo: 464*76800(BASE64CHAR)\r\n");
				}
			}
			else if (strcmp(p, "intel_sink_manufacturer_name") == 0) //2.1.1.4 intel_sink_manufacturer_name
			{
				/*
				intel-sink-manufacturer-name = "intel_sink_manufacturer_name:" SP manufacturer-
				name CRLF
				manufacturer-name = 1*32(VCHAR) / "none"
				A value of "none" means that the manufacturer name is not available.
				*/
				if (1)
				{
					AddString(body, "intel_sink_manufacturer_name: none\r\n");
				}
				else
				{
					AddString(body, "intel_sink_manufacturer_name: Contoso Inc.\r\n");
				}
			}
			else if (strcmp(p, "intel_sink_model_name") == 0) //2.1.1.5 intel_sink_model_name
			{
				/*
				intel-sink-model-name = "intel_sink_model_name:" SP model_name CRLF
				model-name = 1*32(VCHAR) / "none"
				A value of "none" means that the model name is not available.*/
				if (1)
				{
					AddString(body, "intel_sink_model_name: none\r\n");
				}
				else
				{
					AddString(body, "intel_sink_model_name: ScreenMaster 2000\r\n");
				}
			}
			else if (strcmp(p, "intel_sink_version") == 0) //2.1.1.5 intel_sink_version
			{
				/*
				intel-sink-version = "intel_sink_version:" SP product-id SP hw-version SP sw-version CRLF
				product-id = "product_ID=" 1*16(VCHAR)
				hw-version = "hw_version=" version_tag
				sw-version = "sw_version=" version_tag
				version-tag = major "." minor "." sku "." build
				major = 1*2(DIGIT)
				minor = 1*2(DIGIT).*/

				if (1)
				{
					AddString(body, "intel_sink_version: product_ID=G4716-2000 hw_version=1.1.5.1345 sw_version=1.2.4.2451\r\n");
				}
				else  {
					AddString(body, "intel_sink_version: product_ID=G4716-2000 hw_version=1.1.5.1345 sw_version=1.2.4.2451\r\n");
				}
				
			}
			else if (strcmp(p, "microsoft_diagnostics_capability") == 0) //2.2.1.1 microsoft_diagnostics_capability
			{
				/*
				The ABNF syntax is as follows:
				microsoft-diagnostics-capability = "microsoft_diagnostics_capability:" SP
				diagnostics-capability CRLF
				diagnostics-capability = "supported" / "none"
				If the diagnostics-capability parameter has the value "supported", it means that the M8 request
				([WF-DTS1.1] section 6.4.8) that was sent by the Wi-Fi Display Sink includes the
				microsoft_teardown_reason parameter in the message body of that request. If the diagnostics-
				capability parameter has the value "none", it means that there are no changes to the M8 request*/

				if (1)
				{
					AddString(body, "microsoft_diagnostics_capability: none\r\n");
				}
				else 
				{
					AddString(body, "microsoft_diagnostics_capability: supported\r\n");
				}

			}
			else if (strcmp(p, "microsoft_teardown_reason") == 0) //2.2.1.2 microsoft_teardown_reason
			{
				if (1)
				{
					AddString(body, "microsoft_teardown_reason: none\r\n");
				}
				else{
					AddString(body, "microsoft_teardown_reason: C00D4278 No RTP data was provided for 2 minutes\r\n");
				}
			}
			else if ( strcmp(p ,"microsoft_format_change_capability") == 0) //2.3.1.1 microsoft_format_change_capability
			{
				/*
				microsoft-format-change-capability = "microsoft_format_change_capability:" SP
				format-change-caps CRLF
				format-change-caps = "supported" / "none"
				*/
				if (1)
				{
					AddString(body, "microsoft_format_change_capability: none\r\n");
				}
				else 
				{
					AddString(body, "microsoft_format_change_capability: supported\r\n");
				}
			}
			else if (strcmp(p, "microsoft_latency_management_capability") == 0) //2.4.1.1 microsoft_latency_management_capability
			{
				/*
				microsoft-latency-management-capability = "microsoft_latency_management_capability:" SP
				( latency-management-cap / latency-mode ) CRLF
				latency-management-cap = "supported" / "none"
				latency-mode = "low" / "normal" / "high"*/
				if (1)
				{
					AddString(body, "microsoft_latency_management_capability: none\r\n");
				}
				else
				{
					AddString(body, "microsoft_latency_management_capability: supported\r\n");
				}
			}
			else if (strcmp(p, "microsoft_cursor") == 0)
			{
				AddString(body, "microsoft_cursor: none\r\n");
			}
			else if (strcmp(p, "intel_fast_cursor") == 0)
			{
				AddString(body, "intel_fast_cursor: none\r\n");
			}
			else if (strcmp(p, "intel_usboip") == 0)
			{
				AddString(body, "intel_usboip: none\r\n");
			}
			else if (strcmp(p, "intel_interactivity_mode") == 0)
			{
				AddString(body, "intel_interactivity_mode: none\r\n");
			}
			else if (strcmp(p, "intel_sink_information") == 0)
			{
				AddString(body, "intel_sink_information: none\r\n");
			}
			else if (strcmp(p, "intel_managed_meetings") == 0)
			{
				AddString(body, "intel_managed_meetings: none\r\n");
			}
			else if (strcmp(p, "intel_lower_bandwidth") == 0)
			{
				AddString(body, "intel_lower_bandwidth: none\r\n");
			}
			else if (strcmp(p, "wfdx_video_formats") == 0)
			{
				AddString(body, "wfdx_video_formats: none\r\n");
			}
			else if (strcmp(p, "wfdx_rotation_capability") == 0)
			{
				AddString(body, "wfdx_rotation_capability: none\r\n");
			}
			else if (strcmp(p, "intel_source_connection_confirmation_enabled") == 0)
			{
				AddString(body, "intel_source_connection_confirmation_enabled: none\r\n");
			}
			else if (strcmp(p, "intel_source_connection_indicator_enabled") == 0)
			{
				AddString(body, "intel_source_connection_indicator_enabled: none\r\n");
			}
			
			//strcmp
			//OutputDebugStringA(p);
		}
	}
#else	

#pragma region MY1

if (recvbuf != NULL)
{
	const char * split = "\r\n";
	char * p = strtok(recvbuf, split);
	//OutputDebugStringA(p);
	while (p != NULL)
	{
		p = strtok(NULL, split);
		if (p == NULL)
			break;
		if (strcmp(p, "wfd_audio_codecs") == 0)// 6.1.2
		{
			AddString(body, "wfd_audio_codecs: LPCM 00000002 00, AAC 00000001 00, LPCM 00000003 00\r\n");
			//AddString(body, "wfd_audio_codecs: LPCM 00000003 00\r\n");
		}
		else if (strcmp(p, "wfd_video_formats") == 0)// 6.1.3
		{
			
			char sFormat[4096] = { 0 };
			sprintf(sFormat, "wfd_video_formats: 00 00 02 08 00019CFF 05557FFF 00000FFF 00 0001 0000 11 none none\r\n");
			//sprintf(sFormat, "wfd_video_formats: 00 00 02 08 000001FF 00000000 00000000 00 0001 0000 11 none none\r\n");
			AddString(body, sFormat);
			
		}
		else if (strcmp(p, "wfd_3d_video_formats") == 0)// 6.1.4
		{
			//AddString(body, "wfd_3d_video_formats: 00 00 02 08 00001247ffffe007 02 0000 00ff 11 none none, 01 08 00001247ffffe007 02 0000 00ff 11 none none\r\n");
			AddString(body, "wfd_3d_video_formats: none\r\n");

		}
		else if (strcmp(p, "wfd_content_protection") == 0)// 6.1.5
		{
			/************************************************************************/
			/*
			wfd-content-protection  = “wfd_content_protection:” SP cp-spec CRLF
			cp-spec  = “none” / hdcp2-spec
			hdcp2-spec  = (“HDCP2.0” / “HDCP2.1”) SP “port=” IPPORT ;
			TCP port                                                                     */
			/************************************************************************/
			if (1){
				AddString(body, "wfd_content_protection: none\r\n");
			}
			else{
				
				AddString(body, "wfd_content_protection: HDCP2.1 port=4990\r\n");
			}
		}
		else if (strcmp(p, "wfd_display_edid") == 0)// 6.1.6
		{
			//AddString(body, "wfd_display_edid: 0002 00ffffffffffff0036698224e1d6f40308140103812f1a78eafbb4a1554b972410484c254e0081cf95c0a9c0b3c0d1c00101010101011a3680a070381f4030203500dd0c1100001e000000ff003030303030310a0a0a0a0a0a0a000000fd00324b1e5514000a202020202020000000fc004c656e6f766f2041494f205043018c020322714f10010203040506071112131415161f230957078301000065030c001000023a801871382d40582c4500d7403200001e011d007251d01e206e285500d7403200001e011d8018711c1620582c2500d7403200009e00000000000000000000000000000000000000000000000000000000000000000000000000000063\r\n");

			AddString(body, "wfd_display_edid: 0002 00FFFFFFFFFFFF0061A40001010000000117010380683B780A298DA5555099270E474A21080001010101010101010101010101010101023A801871382D40582C450010494200001F011D007251D01E206E28550010494200001E000000FC004D692054560A20202020202020000000FD03324B1E4B0F000A202020202020018002032F714B901F140413021103060712492122053C3D3E15162023090707830100006C030C001000B02D20A0020141023A80D072382D40102C458010494200001E011D00BC52D01E20B828554010494200001E011D8018711C1620582C250010494200009E011D80D0721C1620102C258010494200009E0000000000000000D8\r\n");

		}
		else if (strcmp(p, "wfd_coupled_sink") == 0)// 6.1.7
		{
			AddString(body, "wfd_coupled_sink: 00 none\r\n");
		}
		else if (strcmp(p, "wfd_trigger_method") == 0)// 6.1.8
		{
			AddString(body, "wfd_trigger_method: none\r\n");
		}
		else if (strcmp(p, "wfd_presentation_url") == 0)// 6.1.9
		{
			AddString(body, "wfd_presentation_url: none\r\n");
		}
		else if (strcmp(p, "wfd_client_rtp_ports") == 0)// 6.1.10
		{
			AddString(body, "wfd_client_rtp_ports: RTP/AVP/UDP;unicast %d 0 mode=play\r\n");
		}
		else if (strcmp(p, "wfd_route") == 0)// 6.1.11
		{
			AddString(body, "wfd_route: none\r\n");
		}
		else if (strcmp(p, "wfd_I2C") == 0)// 6.1.12
		{
			AddString(body, "wfd_I2C: none\r\n");
		}
		else if (strcmp(p, "wfd_av_format_change_timing") == 0) // 6.1.13
		{
			AddString(body, "wfd_av_format_change_timing: none\r\n");
		}
		else if (strcmp(p, "wfd_preferred_display_mode") == 0)// 6.1.14
		{
			AddString(body, "wfd_preferred_display_mode: none\r\n");
		}
		else if (strcmp(p, "wfd_uibc_capability") == 0)// 6.1.15
		{

			AddString(body, "wfd_uibc_capability: none\r\n");
			//AddString(body, "wfd_uibc_capability: input_category_list=GENERIC;generic_cap_list=Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard;hidc_cap_list=none;port=none\r\n");
			//AddString(body, "wfd_uibc_capability: input_category_list=GENERIC,HIDC;generic_cap_list=Keyboard, Mouse, MultiTouch; hidc_cap_list =Keyboard/USB, Mouse/USB;port=none\r\n");
			//AddString(body, "wfd_uibc_capability: input_category_list=GENERIC,HIDC;generic_cap_list=Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard;hidc_cap_list=Infrared, USB, BT, Zigbee, Wi-Fi, No-SP;port=none\r\n");
			//"input_category_list = GENERIC; generic_cap_list = Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard; hidc_cap_list = none; port = none"
			//"input_category_list = GENERIC; generic_cap_list = Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard; hidc_cap_list = none; port = none"
		}
		else if (strcmp(p, "wfd_uibc_setting") == 0)// 6.1.16
		{
			AddString(body, "wfd_uibc_setting: none\r\n");
		}
		else if (strcmp(p, "wfd_standby_resume_capability") == 0)// 6.1.17
		{
			AddString(body, "wfd_standby_resume_capability: supported\r\n");
		}
		else if (strcmp(p, "wfd_standby") == 0)// 6.1.18
		{
			AddString(body, "wfd_standby: none\r\n");
		}
		else if (strcmp(p, "wfd_connector_type") == 0)// 6.1.19
		{
			AddString(body, "wfd_connector_type: 05\r\n");
		}
		else if (strcmp(p, "wfd_idr_request") == 0)// 6.1.20
		{
			AddString(body, "wfd_idr_request: none\r\n");
		}

		else if (strcmp(p, "wfd_idr_request_capability") == 0)
		{
			AddString(body, "wfd_idr_request_capability: 1\r\n");
		}
		else if (strcmp(p, "intel_friendly_name") == 0)   //2.1.1.1 intel_friendly_nameintel_sink_version
		{
			/*
			intel-friendly-name = "intel_friendly_name:" SP friendly-name CRLF
			friendly-name = 1*18(utf8byte-no-hyphen)intel_sink_version
			utf8byte-no-hyphen = %x00-2C / %x2E-FF
			*/
			if (1)
			{
				AddString(body, "intel_friendly_name: Contoso ScreenMaster 2000\r\n");
			}
			else
			{
				//AddString(body, "intel_friendly_name: %x2E-FF\r\n");
			}
		}
		else if (strcmp(p, "intel_sink_device_URL") == 0) //2.1.1.2 intel_sink_device_URL
		{
			/*
			intel-sink-device-URL = "intel_sink_device_URL:" SP uri CRLF
			uri = 1*256(VCHAR) / "none"
			The intel_sink_device_URL parameter specifies a URI as specified in [RFC3986]. A value of "none"
			means that no product information is available.
			*/
			if (1)
			{
				AddString(body, "intel_sink_device_URL: none\r\n");
			}
			else
			{
				AddString(body, "intel_sink_device_URL: http://www.example.com/screenmaster/\r\n");
			}
		}
		else if (strcmp(p, "intel_sink_manufacturer_logo") == 0)//2.1.1.3 intel_sink_manufacturer_logo
		{
			/*
			intel-sink-manufacturer-logo = "intel_sink_manufacturer_logo:" SP logo CRLF
			logo = "none" / base64_logo
			base64_logo = 464*76800(BASE64CHAR)
			A value of "none" means that no image is available
			*/
			if (1)
			{
				AddString(body, "intel_sink_manufacturer_logo: none\r\n");
			}
			else
			{
				AddString(body, "intel_sink_manufacturer_logo: 464*76800(BASE64CHAR)\r\n");
			}
		}
		else if (strcmp(p, "intel_sink_manufacturer_name") == 0) //2.1.1.4 intel_sink_manufacturer_name
		{
			/*
			intel-sink-manufacturer-name = "intel_sink_manufacturer_name:" SP manufacturer-
			name CRLF
			manufacturer-name = 1*32(VCHAR) / "none"
			A value of "none" means that the manufacturer name is not available.
			*/
			if (1)
			{
				AddString(body, "intel_sink_manufacturer_name: none\r\n");
			}
			else
			{
				AddString(body, "intel_sink_manufacturer_name: Contoso Inc.\r\n");
			}
		}
		else if (strcmp(p, "intel_sink_model_name") == 0) //2.1.1.5 intel_sink_model_name
		{
			/*
			intel-sink-model-name = "intel_sink_model_name:" SP model_name CRLF
			model-name = 1*32(VCHAR) / "none"
			A value of "none" means that the model name is not available.*/
			if (1)
			{
				AddString(body, "intel_sink_model_name: none\r\n");
			}
			else
			{
				AddString(body, "intel_sink_model_name: ScreenMaster 2000\r\n");
			}
		}
		else if (strcmp(p, "intel_sink_version") == 0) //2.1.1.5 intel_sink_version
		{
			/*
			intel-sink-version = "intel_sink_version:" SP product-id SP hw-version SP sw-version CRLF
			product-id = "product_ID=" 1*16(VCHAR)
			hw-version = "hw_version=" version_tag
			sw-version = "sw_version=" version_tag
			version-tag = major "." minor "." sku "." buildintel_sink_information
			major = 1*2(DIGIT)
			minor = 1*2(DIGIT).*/

			if (1)
			{
				AddString(body, "intel_sink_version: product_ID=905610180919 hw_version=1.0.0.0 sw_version=5.5.20.822\r\n");
			}
			else  {
				AddString(body, "intel_sink_version: product_ID=G4716-2000 hw_version=1.1.5.1345 sw_version=1.2.4.2451\r\n");
			}

		}
		else if (strcmp(p, "microsoft_diagnostics_capability") == 0) //2.2.1.1 microsoft_diagnostics_capability
		{
			/*
			The ABNF syntax is as follows:
			microsoft-diagnostics-capability = "microsoft_diagnostics_capability:" SP
			diagnostics-capability CRLF
			diagnostics-capability = "supported" / "none"
			If the diagnostics-capability parameter has the value "supported", it means that the M8 request
			([WF-DTS1.1] section 6.4.8) that was sent by the Wi-Fi Display Sink includes the
			microsoft_teardown_reason parameter in the message body of that request. If the diagnostics-
			capability parameter has the value "none", it means that there are no changes to the M8 request*/

			if (1)
			{
				AddString(body, "microsoft_diagnostics_capability: none\r\n");
			}
			else
			{
				AddString(body, "microsoft_diagnostics_capability: supported\r\n");
			}

		}
		else if (strcmp(p, "microsoft_teardown_reason") == 0) //2.2.1.2 microsoft_teardown_reason
		{
			if (1)
			{
				AddString(body, "microsoft_teardown_reason: none\r\n");
			}
			else{
				AddString(body, "microsoft_teardown_reason: C00D4278 No RTP data was provided for 2 minutes\r\n");
			}
		}
		else if (strcmp(p, "microsoft_format_change_capability") == 0) //2.3.1.1 microsoft_format_change_capability
		{
			/*
			microsoft-format-change-capability = "microsoft_format_change_capability:" SP
			format-change-caps CRLF
			format-change-caps = "supported" / "none"
			*/
			if (1)
			{
				AddString(body, "microsoft_format_change_capability: none\r\n");
			}
			else
			{
				AddString(body, "microsoft_format_change_capability: supported\r\n");
			}
		}
		else if (strcmp(p, "microsoft_latency_management_capability") == 0) //2.4.1.1 microsoft_latency_management_capability
		{
			/*
			microsoft-latency-management-capability = "microsoft_latency_management_capability:" SP
			( latency-management-cap / latency-mode ) CRLF
			latency-management-cap = "supported" / "none"
			latency-mode = "low" / "normal" / "high"*/
			if (1)
			{
				AddString(body, "microsoft_latency_management_capability: none\r\n");
			}
			else
			{
				AddString(body, "microsoft_latency_management_capability: supported\r\n");
			}
		}
		else if (strcmp(p, "microsoft_cursor") == 0)
		{
			AddString(body, "microsoft_cursor: none\r\n");
		}
		else if (strcmp(p, "intel_fast_cursor") == 0)
		{
			AddString(body, "intel_fast_cursor: port=49152\r\n");
		}
		else if (strcmp(p, "intel_usboip") == 0)
		{
			AddString(body, "intel_usboip: none\r\n");
		}
		else if (strcmp(p, "intel_interactivity_mode") == 0)
		{
			AddString(body, "intel_interactivity_mode: DEFAULT,HIGH\r\n");
		}
		else if (strcmp(p, "intel_sink_information") == 0)
		{
			AddString(body, "intel_sink_information: none\r\n");
		}
		else if (strcmp(p, "intel_managed_meetings") == 0)
		{
			AddString(body, "intel_managed_meetings: none\r\n");
		}
		else if (strcmp(p, "intel_lower_bandwidth") == 0)
		{
			AddString(body, "intel_lower_bandwidth: none\r\n");
		}
		else if (strcmp(p, "wfdx_video_formats") == 0)
		{
			AddString(body, "wfdx_video_formats: none\r\n");
		}
		else if (strcmp(p, "wfdx_rotation_capability") == 0)
		{
			AddString(body, "wfdx_rotation_capability: none\r\n");
		}
		else if (strcmp(p, "intel_source_connection_confirmation_enabled") == 0)
		{
			AddString(body, "intel_source_connection_confirmation_enabled: none\r\n");
		}
		else if (strcmp(p, "intel_source_connection_indicator_enabled") == 0)
		{
			AddString(body, "intel_source_connection_indicator_enabled: none\r\n");
		}
		else if (strcmp(p, "intel_content_protection") == 0)
		{
			
			AddString(body, "intel_content_protection: none\r\n");
		}

		//strcmp
		//OutputDebugStringA(p);
	}
}
#pragma endregion


#endif 









	char buf[3072] = { 0 };

	AddString(buf, "RTSP/1.0 200 OK\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[3072] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);

	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, bufs);

	sprintf(sendbuf, buf, CSeqId, lens);
	return buf;
}

char * CWfdStringFormat::SinkToSource4_changeAvFormat(char * sendbuf, int CSeqId)
{
	/*
	wfd-av-format-change-timing = “wfd_av_format_change_timing:” SP PTS SP DTS CRLF
	PTS = 10*10HEXDIG; most-significant 33 bits indicating PTS value
	DTS = 10*10HEXDIG; most-significant 33 bits indicating DTS value
	*/

	//static int isfirst = 0;
	//if (isfirst == 0)
	//	AddString(body, "wfd_video_formats: 00 00 01 02 00000080 00000000 00000000 00 0000 0000 00 none none\r\n");
	//else
	//	AddString(body, "wfd_video_formats: 00 00 01 02 00000020 00000000 00000000 00 0000 0000 00 none none\r\n");
	//isfirst++;

	char * body = {
		//"wfd_av_format_change_timing: 0000000000 0000000000\r\n" // 一个新的翻译audio stream的WFD sink ？？？
		"wfd_video_formats: 00 00 01 02 00000020 00000000 00000000 00 0000 0000 00 none none\r\n"
	};
	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);

	//AddString(buf, "Session: %s\r\n");
	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, lens);

	return buf;
}


char * CWfdStringFormat::SinkToSource4(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };

	time_t now = time(NULL);
	struct tm *now2 = gmtime(&now);
	char  buftime[128] = { 0 };
	strftime(buftime, sizeof(buftime), "%a, %b %d %Y %H:%M:%S", now2);


	//time_t now = time(NULL);
	//struct tm *now2 = gmtime(&now);
	//char  buftime[128] = { 0 };
	//strftime(buftime, sizeof(buftime), "%a, %b %d %Y %H:%M:%S", now2);
	//AddString(buf, "Date: %s GMT\r\n");


	AddString(buf, "RTSP/1.0 200 OK\r\n");
	AddString(buf, "CSeq: %d\r\n");
	//AddString(buf, StringPrintf("Date: %s GMT\r\n", buftime));
	AddString(buf, "Date: %s GMT\r\n");

	AddString(buf, "\r\n");
	//sprintf(sendbuf, buf, CSeqId);
	sprintf(sendbuf, buf, CSeqId, buftime);
	return buf;
}
char * CWfdStringFormat::SinkToSource5(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };

	time_t now = time(NULL);
	struct tm *now2 = gmtime(&now);
	char  buftime[128] = { 0 };
	strftime(buftime, sizeof(buftime), "%a, %b %d %Y %H:%M:%S", now2);

	AddString(buf, "RTSP/1.0 200 OK\r\n");
	AddString(buf, "CSeq: %d\r\n");
	AddString(buf, "Date: %s GMT\r\n");
	AddString(buf, "\r\n");

	//sprintf(sendbuf, buf, CSeqId);
	sprintf(sendbuf, buf, CSeqId, buftime);
	return buf;;
}
char * CWfdStringFormat::SinkToSource6(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };
	char * ip = m_IpService;// "192.168.49.1";g_client_portUDP
	int  port = m_setUdpPort;

	AddString(buf, "SETUP rtsp://%s/wfd1.0/streamid=0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");
	AddString(buf, "Transport: RTP/AVP/UDP;unicast;client_port=%d\r\n");
	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, ip, CSeqId, port);

	return buf;
}
char * CWfdStringFormat::SinkToSource7(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };
	char * ip = m_IpService;// "192.168.49.1";

	AddString(buf, "PLAY rtsp://%s/wfd1.0/streamid=0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	AddString(buf, "Session: %s\r\n");

	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, ip, CSeqId, m_sessionid);
	return buf;
}
char * CWfdStringFormat::SinkToSource8(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };
	char * ip = m_IpService;// "192.168.49.1";

	AddString(buf, "TEARDOWN rtsp://%s/wfd1.0/streamid=0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	AddString(buf, "Session: %s\r\n");

	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, ip, CSeqId, m_sessionid);
	return buf;
}
char * CWfdStringFormat::SinkToSource9(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };
	char * ip = m_IpService;// "192.168.49.1";

	AddString(buf, "PAUSE rtsp://%s/wfd1.0/streamid=0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	AddString(buf, "Session: %s\r\n");

	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, ip, CSeqId, m_sessionid);
	return buf;
}


char * CWfdStringFormat::SinkToSource10(char * sendbuf, int CSeqId)
{


	//primary” / “secondary
	char * body = {
		"wfd_route: primary\r\n" // 一个新的翻译audio stream的WFD sink ？？？
		//"wfd_route: secondary\r\n"

	};
	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);

	int lens = strlen(bufs);
	//AddString(buf, "Session: %s\r\n");
	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, lens);

	return buf;
}
char * CWfdStringFormat::SinkToSource11(char * sendbuf, int CSeqId)
{

	/*
	选择显示器的类型
	0  Video Graphics Array (VGA) Connector
	1  S-Video connector
	2  Composite video connector
	3  Component video connector
	4  Digital Video Interface (DVI) connector
	5  High-Definition  Multimedia  Interface  (HDMI) connector
	6  Reserved
	7  Wi-Fi Display
	8  Japanese D connector. (A connector conforming to the EIAJ RC-5237 standard)
	9  Serial Digital Image (SDI) connector
	10  A Display Port connector (DP)
	11  Reserved
	12  A Unified Display Interface (UDI)
	13-254  Reserved
	255 11 Indicates a type of physical connector that is not listed
	from value 0 to 254
	*/

	char * body = {
		"wfd_connector_type: 00\r\n" //
	};

	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);

	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");

	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, lens);

	return buf;
}
char * CWfdStringFormat::SinkToSource12(char * sendbuf, int CSeqId)
{
	char * body = {
		"wfd_standby\r\n"
	};

	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);

	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, lens);

	return buf;
}
char * CWfdStringFormat::SinkToSource13(char * sendbuf, int CSeqId)
{//idr 帧
	char * body = {
		"wfd_idr_request\r\n"
	};

	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");
	AddString(buf, "Session: %s\r\n");
	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);


	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, m_sessionid, lens);

	return buf;
}

char * CWfdStringFormat::SinkToSource14(char * sendbuf, int CSeqId)
{
	char * body = {
		"wfd_uibc_capability: input_category_list=GENERIC;generic_cap_list=Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard;hidc_cap_list=none;port=none\r\n"
	};

	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);


	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, lens);

	return buf;
}


char * CWfdStringFormat::SinkToSource15(char * sendbuf, int CSeqId)
{
	char * body = {
		"wfd_uibc_capability: input_category_list=GENERIC;generic_cap_list=Keyboard, Mouse, SingleTouch, MultiTouch, KBD_Windows, KBD_Android, KBD_Standard;hidc_cap_list=none;port=none\r\n"
	};

	char  buf[1024] = { 0 }; // = sendbuf;

	AddString(buf, "SET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	char bufs[1024] = { 0 };
	sprintf(bufs, body, m_setUdpPort);
	//char * bodytemp = StringPrintf(body, "1234");
	int lens = strlen(bufs);


	AddString(buf, "Content-Length: %d\r\n");

	AddString(buf, "Content-Type: text/parameters\r\n");
	AddString(buf, "\r\n");

	AddString(buf, body);
	//AddString(buf, "\r\n");

	sprintf(sendbuf, buf, CSeqId, lens);

	return buf;
}

char * CWfdStringFormat::SinkToSource16(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };

	AddString(buf, "RTSP/1.0 200 OK\r\n");
	AddString(buf, "CSeq: %d\r\n");
	//AddString(buf, "Session: %s\r\n");
	AddString(buf, "\r\n");
	//sprintf(sendbuf, buf, CSeqId, m_sessionid);
	sprintf(sendbuf, buf, CSeqId);
	return buf;
}




char * CWfdStringFormat::SinkToSource17(char * sendbuf, int CSeqId)
{
	char  buf[1024] = { 0 };
	char * ip = m_IpService;// "192.168.49.1";

	AddString(buf, "GET_PARAMETER rtsp://%s/wfd1.0/streamid=0 RTSP/1.0\r\n");
	AddString(buf, "CSeq: %d\r\n");

	AddString(buf, "Session: %s\r\n");

	AddString(buf, "\r\n");

	sprintf(sendbuf, buf, ip, CSeqId, m_sessionid);
	return buf;
}




void SetTheVideoFromat5_10(VideoFromatTable * vft)
{
	DWORD indexand = 1;
	//0  0  640x480 p60
	{

		vft->index = 0;
		vft->nw = 640;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//1  1  720x480 p60
	{
		vft++;
		vft->index = 1;
		vft->nw = 720;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//2  2  720x480 i60
	{
		vft++;
		vft->index = 2;
		vft->nw = 720;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 1;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//3  3  720x576 p50
	{
		vft++;
		vft->index = 3;
		vft->nw = 720;
		vft->nh = 576;
		vft->frames = 50;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//4  4  720x576 i50
	{
		vft++;
		vft->index = 4;
		vft->nw = 720;
		vft->nh = 576;
		vft->frames = 50;
		vft->type = 1;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//5  5  1280x720 p30
	{
		vft++;
		vft->index = 5;
		vft->nw = 1280;
		vft->nh = 720;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//6  6  1280x720 p60
	{
		vft++;
		vft->index = 6;
		vft->nw = 1280;
		vft->nh = 720;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//1920x1080 p30
	{
		vft++;
		vft->index = 7;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//8  8  1920x1080 p60
	{
		vft++;
		vft->index = 8;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//9  9  1920x1080 i60
	{
		vft++;
		vft->index = 9;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 60;
		vft->type = 1;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//10  10  1280x720 p25
	{
		vft++;
		vft->index = 10;
		vft->nw = 1280;
		vft->nh = 720;
		vft->frames = 25;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//11  11  1280x720 p50
	{
		vft++;
		vft->index = 11;
		vft->nw = 1280;
		vft->nh = 720;
		vft->frames = 50;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//12  12  1920x1080 p25
	{
		vft++;
		vft->index = 12;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 25;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//13  13  1920x1080 p50
	{
		vft++;
		vft->index = 13;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 50;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//14  14  1920x1080 i50
	{
		vft++;
		vft->index = 14;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 50;
		vft->type = 1;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//15  15  1280x720 p24
	{
		vft++;
		vft->index = 15;
		vft->nw = 1280;
		vft->nh = 720;
		vft->frames = 24;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//16  16  1920x1080 p24
	{
		vft++;
		vft->index = 16;
		vft->nw = 1920;
		vft->nh = 1080;
		vft->frames = 24;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
}


void SetTheVideoFromat5_11(VideoFromatTable * vft)
{
	DWORD indexand = 1;
	//0  0  800x600 p30
	{
		vft->index = 0;
		vft->nw = 800;
		vft->nh = 600;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//1  1  800x600 p60
	{
		vft++;
		vft->index = 1;
		vft->nw = 800;
		vft->nh = 600;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//2  2  1024x768 p30
	{
		vft++;
		vft->index = 2;
		vft->nw = 1024;
		vft->nh = 768;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//3  3  1024x768 p60
	{
		vft++;
		vft->index = 3;
		vft->nw = 1024;
		vft->nh = 768;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//4  4  1152x864 p30
	{
		vft++;
		vft->index = 4;
		vft->nw = 1152;
		vft->nh = 864;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//5  5  1152x864 p60
	{
		vft++;
		vft->index = 5;
		vft->nw = 1152;
		vft->nh = 864;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//6  6  1280x768 p30
	{
		vft++;
		vft->index = 6;
		vft->nw = 1280;
		vft->nh = 768;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//7  7  1280x768 p60
	{
		vft++;
		vft->index = 7;
		vft->nw = 1280;
		vft->nh = 768;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//8  8  1280x800 p30
	{
		vft++;
		vft->index = 8;
		vft->nw = 1280;
		vft->nh = 800;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//9  9  1280x800 p60
	{
		vft++;
		vft->index = 9;
		vft->nw = 1280;
		vft->nh = 800;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//10  10  1360x768 p30
	{
		vft++;
		vft->index = 10;
		vft->nw = 1360;
		vft->nh = 768;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//11  11  1360x768 p60
	{
		vft++;
		vft->index = 11;
		vft->nw = 1360;
		vft->nh = 768;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//12  12  1366x768 p30
	{
		vft++;
		vft->index = 12;
		vft->nw = 1366;
		vft->nh = 768;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//13  13  1366x768 p60
	{
		vft++;
		vft->index = 13;
		vft->nw = 1366;
		vft->nh = 768;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//14  14  1280x1024 p30
	{
		vft++;
		vft->index = 14;
		vft->nw = 1280;
		vft->nh = 1024;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//15  15  1280x1024 p60
	{
		vft++;
		vft->index = 15;
		vft->nw = 1280;
		vft->nh = 1024;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//16  16  1400x1050 p30
	{
		vft++;
		vft->index = 16;
		vft->nw = 1400;
		vft->nh = 1050;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//17  17  1400x1050 p60
	{
		vft++;
		vft->index = 17;
		vft->nw = 1400;
		vft->nh = 1050;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//18  18  1440x900 p30
	{
		vft++;
		vft->index = 18;
		vft->nw = 1440;
		vft->nh = 900;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//19  19  1440x900 p60
	{
		vft++;
		vft->index = 19;
		vft->nw = 1440;
		vft->nh = 900;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//20  20  1600x900 p30
	{
		vft++;
		vft->index = 20;
		vft->nw = 1600;
		vft->nh = 900;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//21  21  1600x900 p60
	{
		vft++;
		vft->index = 21;
		vft->nw = 1600;
		vft->nh = 900;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//22  22  1600x1200 p30
	{
		vft++;
		vft->index = 22;
		vft->nw = 1600;
		vft->nh = 1200;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//23  23  1600x1200 p60
	{
		vft++;
		vft->index = 23;
		vft->nw = 1600;
		vft->nh = 1200;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//24  24  1680x1024 p30
	{
		vft++;
		vft->index = 24;
		vft->nw = 1680;
		vft->nh = 1024;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//25  25  1680x1024 p60
	{
		vft++;
		vft->index = 25;
		vft->nw = 1680;
		vft->nh = 1024;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//26  26  1680x1050 p30
	{
		vft++;
		vft->index = 26;
		vft->nw = 1680;
		vft->nh = 1050;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//27  27  1680x1050 p60
	{
		vft++;
		vft->index = 27;
		vft->nw = 1680;
		vft->nh = 1050;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//28  28  1920x1200 p30
	{
		vft++;
		vft->index = 28;
		vft->nw = 1920;
		vft->nh = 1200;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//29  29  1920x1200 p60
	{
		vft++;
		vft->index = 29;
		vft->nw = 1920;
		vft->nh = 1200;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//31:30 - Reserved
	{
		vft++;
		vft->index = 30;
		vft->nw = 0;
		vft->nh = 0;
		vft->frames = 0;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	{
		vft++;
		vft->index = 31;
		vft->nw = 0;
		vft->nh = 0;
		vft->frames = 0;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
}

void SetTheVideoFromat5_12(VideoFromatTable * vft)
{
	DWORD indexand = 1;
	//0  0  800x480 p30
	{
		vft->index = 0;
		vft->nw = 800;
		vft->nh = 480;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//1  1  800x480 p60
	{
		vft++;
		vft->index = 1;
		vft->nw = 800;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}

	//2  2  854x480 p30
	{
		vft++;
		vft->index = 2;
		vft->nw = 854;
		vft->nh = 480;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}

	//3  3  854x480 p60
	{
		vft++;
		vft->index = 3;
		vft->nw = 854;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//4  4  864x480 p30
	{
		vft++;
		vft->index = 4;
		vft->nw = 864;
		vft->nh = 480;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//5  5  864x480 p60
	{
		vft++;
		vft->index = 5;
		vft->nw = 864;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//6 6 640x360 p30
	{
		vft++;
		vft->index = 6;
		vft->nw = 640;
		vft->nh = 360;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//7 7 640x360 p60
	{
		vft++;
		vft->index = 7;
		vft->nw = 640;
		vft->nh = 360;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//8  8  960x540 p30
	{
		vft++;
		vft->index = 8;
		vft->nw = 960;
		vft->nh = 540;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//9  9  960x540 p60
	{
		vft++;
		vft->index = 9;
		vft->nw = 960;
		vft->nh = 540;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//10  10  848x480 p30
	{
		vft++;
		vft->index = 10;
		vft->nw = 848;
		vft->nh = 480;
		vft->frames = 30;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//11  11  848x480 p60
	{
		vft++;
		vft->index = 11;
		vft->nw = 848;
		vft->nh = 480;
		vft->frames = 60;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
	//31:12 - Reserved
	{
		vft++;
		vft->index = 12;
		vft->nw = 0;
		vft->nh = 0;
		vft->frames = 0;
		vft->type = 0;
		vft->indexand = indexand;
		indexand = indexand << 1;
	}
}

void GetVideoFromtTable(VideoFromatTable *  pvft, int tableid, int index)
{
	if (tableid == 0)
	{
		for (int i = 0; i < MAX_TABLE_LEN; i++)
		{
			VideoFromatTable *  pvftget = &g_table5_10[i];
			if (index == pvftget->index)
			{
				memcpy(pvft, pvftget, sizeof(VideoFromatTable));
				break;
			}
		}
	}
	else if (tableid == 1)
	{
		for (int i = 0; i < MAX_TABLE_LEN; i++)
		{
			VideoFromatTable *  pvftget = &g_table5_11[i];
			if (index == pvftget->index)
			{
				memcpy(pvft, pvftget, sizeof(VideoFromatTable));
				break;
			}
		}
	}
	else if (tableid == 2)
	{
		for (int i = 0; i < MAX_TABLE_LEN; i++)
		{
			VideoFromatTable *  pvftget = &g_table5_12[i];
			if (index == pvftget->index)
			{
				memcpy(pvft, pvftget, sizeof(VideoFromatTable));
				break;
			}
		}
	}

}


int gettableuse(char * sPath)
{

	int nget = 0;
	int table5_10 = GetPrivateProfileIntA("main", "usetabt5_10", 0, sPath);
	int table5_11 = GetPrivateProfileIntA("main", "usetabt5_11", 0, sPath);
	int table5_12 = GetPrivateProfileIntA("main", "usetabt5_12", 0, sPath);
	if (table5_10 == 1)
	{
		nget = getSettingTable5_10(sPath , g_table5_10);
		return nget;
	}
	else if (table5_11 == 1)
	{
		nget = getSettingTable5_11(sPath, g_table5_11);
		return nget;
	}
	else if (table5_12 == 1)
	{
		nget = getSettingTable5_12(sPath, g_table5_12);
		return nget;
	}
	return nget;
}

int getRightRatio(int sw, int sh, int uw, int uh)
{
	if (sw >= uw  && sh >= uh)
	{
		return 1;
	}
	return 0;
}



int getSettingTable5_10(char * sPath, VideoFromatTable * tableget)
{
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);

	int vd = 0;
	int isSer = 0;

	for (int i = 0; i < MAX_TABLE_LEN; i++)
	{
		VideoFromatTable *  pvftget = &tableget[i];

		int uw = pvftget->nw;
		int uh = pvftget->nh;
		if (uw != 0 && uh != 0)
		{
			isSer = getRightRatio(sw, sh, uw, uh);
			vd = vd | (isSer << i);
		}
	}

	int  viedow = GetPrivateProfileIntA("main", "videomode", 1920, sPath);
	if (viedow == 1920)
	{
		return  (1 << 7) | (1 << 5);
	}
	else if (viedow == 1280)
	{
		return  (1 << 5);
	}
	return vd;
}
int getSettingTable5_11(char * sPath, VideoFromatTable * tableget)
{

	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);

	int vd = 0;
	int isSer = 0;

	for (int i = 0; i < MAX_TABLE_LEN; i++)
	{
		VideoFromatTable *  pvftget = &tableget[i];
		int uw = pvftget->nw;
		int uh = pvftget->nh;
		if (uw != 0 && uh != 0)
		{
			isSer = getRightRatio(sw, sh, uw, uh);
			vd = vd | (isSer << i);
		}
	}
	return vd;
}
int getSettingTable5_12(char * sPath, VideoFromatTable * tableget)
{
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);

	int vd = 0;
	int isSer = 0;

	for (int i = 0; i < MAX_TABLE_LEN; i++)
	{
		VideoFromatTable *  pvftget = &tableget[i];
		int uw = pvftget->nw;
		int uh = pvftget->nh;
		if (uw != 0 && uh != 0)
		{
			isSer = getRightRatio(sw, sh, uw, uh);
			vd = vd | (isSer << i);
		}
	}
	return vd;
}




int wrtiekey(void * pvdata, int key0, int isdown)
{
	UIBC_STRUCT * pstr = (UIBC_STRUCT *)pvdata;
	UIBC_G * pug = (UIBC_G *)(&(pstr->ug));
	pstr->head.version = 0;//
	pstr->head.T = 0;				//时间戳字段不存在
	pstr->head.reserved = 0;
	pstr->head.gategory = gategory_CG;
	pstr->head.lenghth = sizeof(pstr->head) + sizeof(pug->head) + sizeof(pug->key);
	//pstr->timestamop = timestamop;
	if (isdown == 1)
		pug->head.ID = uibcg_KD;
	else
		pug->head.ID = uibcg_KU;
	pug->head.length = sizeof(pug->key);//
	pug->key.key1 = key0;//
	pug->key.key2 = NULL;
	return pstr->head.lenghth;
}
int writemouse_touch(void * pvdata, int n, int x, int y, int isdown)
{
	UIBC_STRUCT * pstr = (UIBC_STRUCT *)pvdata;
	UIBC_G * pug = (UIBC_G *)(&(pstr->ug));
	pstr->head.version = 0;//
	pstr->head.T = 0;				//时间戳字段不存在
	pstr->head.reserved = 0;
	pstr->head.gategory = gategory_CG;
	pstr->head.lenghth = sizeof(pstr->head) + sizeof(pug->head) + sizeof(pug->ms);
	//pstr->timestamop = timestamop;
	if (isdown == 1)
		pug->head.ID = uibcg_MD;
	else
		pug->head.ID = uibcg_MU;
	pug->head.length = sizeof(pug->ms);
	pug->ms.N = n;
	pug->ms.mi->t = 1;
	pug->ms.mi->x = x;
	pug->ms.mi->y = y;

	return pstr->head.lenghth;
}

int writemouse_move(void * pvdata, int n, int x, int y)
{
	UIBC_STRUCT * pstr = (UIBC_STRUCT *)pvdata;
	UIBC_G * pug = (UIBC_G *)(&(pstr->ug));
	pstr->head.version = 0;//
	pstr->head.T = 0;				//时间戳字段不存在
	pstr->head.reserved = 0;
	pstr->head.gategory = gategory_CG;
	pstr->head.lenghth = sizeof(pstr->head) + sizeof(pug->head) + sizeof(pug->ms);
	//pstr->timestamop = timestamop;
	pug->head.ID = uibcg_MU;
	pug->head.length = sizeof(pug->ms);
	pug->ms.N = n;
	pug->ms.mi->t = 1;
	pug->ms.mi->x = x;
	pug->ms.mi->y = y;

	return pstr->head.lenghth;
}















