#pragma  once



class CWfdStringFormat
{
public:
	CWfdStringFormat();
	~CWfdStringFormat();

	char * SinkToSource1(char *sendbuf, int CSeqId);
	char * SinkToSource2(char *sendbuf, int CSeqId);
	char * SinkToSource3(char *sendbuf, int CSeqId, char * recvbuf = NULL);
	char * SinkToSource4(char *sendbuf, int CSeqId);
	char * SinkToSource4_changeAvFormat(char *sendbuf, int CSeqId);

	char * SinkToSource5(char *sendbuf, int CSeqId);
	char * SinkToSource6(char *sendbuf, int CSeqId);
	char * SinkToSource7(char *sendbuf, int CSeqId);
	char * SinkToSource8(char *sendbuf, int CSeqId);
	char * SinkToSource9(char *sendbuf, int CSeqId);

	char * SinkToSource10(char *sendbuf, int CSeqId);
	char * SinkToSource11(char *sendbuf, int CSeqId);
	char * SinkToSource12(char *sendbuf, int CSeqId);
	char * SinkToSource13(char *sendbuf, int CSeqId);
	char * SinkToSource14(char *sendbuf, int CSeqId);
	char * SinkToSource15(char *sendbuf, int CSeqId);
	char * SinkToSource16(char *sendbuf, int CSeqId);
	char * SinkToSource17(char *sendbuf, int CSeqId);

	int m_setUdpPort;
	char m_IpService[260];
	char m_sessionid[260];
};




