#pragma once 
#include "windows.h"
//#include "LK_base.h"
//#include "MyArray.h"


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
	unsigned char table_id : 8;


	unsigned char section_length1 : 4;
	unsigned char reserved_1 : 2;
	unsigned char zero : 1;
	unsigned char section_syntax_indicator : 1;

	unsigned char section_length2 : 8;

	unsigned char program_number1 : 8;
	unsigned char program_number2 : 8;

	unsigned char current_next_indicator : 1;
	unsigned char version_number : 5;
	unsigned char reserved_2 : 2;

	unsigned char section_number : 8;

	unsigned char last_section_number : 8;

	unsigned char PCR_PID1 : 5;
	unsigned char reserved_3 : 3;


	unsigned char PCR_PID2 : 8;


	unsigned char program_info_length1 : 4;
	unsigned char reserved_4 : 4;


	unsigned char program_info_length2 : 8;
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

struct MyBufInfo
{
public:
	DWORD	m_dwLen;
	unsigned long long m_lltime;
	unsigned char * m_pbdata;
	int m_nw;
	int m_nh;
	int m_nisBlack;
	int m_nbkSize;
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
	CMyBufferArray *m_rtpslist,
	CMyBufferArray *m_alist,
	CMyBufferArray *m_vlist,
	MyRTPINFO *m_mr,
	CMyBufferList * m_vddatas,
	CMyBufferList * m_addatas,
	HANDLE m_hrtps,
	HANDLE m_hv,
	HANDLE m_ha,
	unsigned long long & m_startAVT,
	char &m_isNeedDump,
	int & nohp,
	int & sneedidr
	);
