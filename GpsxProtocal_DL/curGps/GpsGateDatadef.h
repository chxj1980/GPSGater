#ifndef GPS_GATE_DATA_DEF_H
#define GPS_GATE_DATA_DEF_H


typedef struct
{
	char socketType;//0: �˽ṹ��Ϊ��Ч���ݣ�1��tcp���� 2��udp����

	SOCKADDR_IN udpSockAddr;

	SOCKET tcpSocketHandle;

	BYTE   reserve[128];//Ԥ��128���Ժ���չʹ�á�

}SOCKETINFO;

typedef  struct
{

	char *pDatabuf;
	int   nDataLen;

	SOCKETINFO curSocketInfo;

	SOCKETINFO oldSocketInfo;

}GPSGATEDATA;




#endif

