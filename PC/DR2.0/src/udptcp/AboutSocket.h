#pragma once
#ifdef USEWINDOW
#include <winsock2.h>
#include <winsock.h>
#else
#endif

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



