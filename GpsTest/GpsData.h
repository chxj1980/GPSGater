/********************************************************************
	created:	2012/01/19
	created:	19:1:2012   15:29
	filename: 	F:\workspace\project\Project-DuanYB\dev-gps\codes\Dll_Dev_GPS\curGps\DevxGps\GpsClass.h
	file path:	F:\workspace\project\Project-DuanYB\dev-gps\codes\Dll_Dev_GPS\curGps\DevxGps
	file base:	GpsClass
	file ext:	h
	author:		areslion@qq.com
	
	purpose:	
*********************************************************************/
#ifndef GPSCLASS_H
#define GPSCLASS_H

#include <wtypes.h>

#define	N_LEN	20

//GPS device type
enum{GPS_NULL,
	GPS_YOUHAO	
};




typedef struct  
{
	// 	long	nID;
	char	YH[N_LEN];
	char	CM[N_LEN];
	char	SEQ[N_LEN];
	char	COMMADDR[N_LEN];
	char	CMDID[N_LEN];
	char	CMDARGUS[N_LEN];
	char	VERFYCODE[N_LEN];
	char	CRLF[N_LEN];
}YHCM;

typedef struct  
{
	// 	long	nID;
	int		nID;
	char	YH[N_LEN];
	char	AN[N_LEN];
	char	Commaddr[N_LEN];
	char	Time[N_LEN];
	char	UTC[N_LEN];
	char	Latitude[N_LEN];
	char	Longitude[N_LEN];
	char	Altitude[N_LEN];
	char	Heading[N_LEN];
	char	Speed[N_LEN];
	char	TransactionFlag[N_LEN];
	char	OEMStatus[N_LEN];
	char	StatusString[N_LEN];
	char	VERFYCODE[N_LEN];
	char	CRLF[N_LEN];
}YHAN;

typedef struct  
{
	// 	long	nID;
	char	YH[N_LEN];
	char	AN[N_LEN];
	char	Commaddr[N_LEN];
	char	UTC[N_LEN];
	char	Latitude[N_LEN];
	char	Longitude[N_LEN];
	char	Altitude[N_LEN];
	char	Heading[N_LEN];
	char	Speed[N_LEN];
	char	TransactionFlag[N_LEN];
	char	OEMStatus[N_LEN];
	char	StatusString[N_LEN];
	char	Time[N_LEN];
	char	VERFYCODE[N_LEN];
	char	CRLF[N_LEN];
}YHAP;


typedef struct  
{
	// 	long	nID;
	char	YH[N_LEN];
	char	AK[N_LEN];
	char	Commaddr[N_LEN];
	char	VERFYCODE[N_LEN];
	char	CRLF[N_LEN];
}YHAK;


//UDP Type
enum{MSG_NULL,
//upload data
/////////////////////////////////////////////////////     GPS������
MSG_RES_GPS,


/////////////////////////////////////////////////////
MSG_TOCONSOLE,//GSP��Console����Ϣ��ʼ��
MSG_LOGIN,
MSG_LOGOFF,
MSG_MAITAIN,
MSG_LOCATION,
MSG_TERMINALFEEDBACK,
MSG_AUTONAVIGATION,


/////////////////////////////////////////////////////     ���ġ�GPS
MSG_RES_CONSOLE,//GPS�豸��Ӧ��������
MSG_RES_LOGIN,//��½�ɹ�����Ӧ����,���ն˱�����½�ɹ�
MSG_RES_KEEPMAITAIN,//���յ�ά�����ӱ��ĵ�ʱ��,Ӧ�����豸���͸���Ӧ����,���򳬹�3�����ʱ��֮��,GPS�豸���½���GPRS������
MSG_RES_LOGOFF,//���߱���,�յ�GPS�豸�����߱���֮��,�����ø���Ӧ��Ϣ������Ӧ


/////////////////////////////////////////////////////
MSG_TOGPS,//Console��GPS�豸��������Ϣ
MSG_CALLSTOP,//����ֹͣ
MSG_CALLONETIME,//��������
MSG_CALLNTIMES,//���κ���
MSG_CALLTIMER,//��ʱ����
MSG_SETGPS,//����GPS����
MSG_SETKEEPINTERVAL,//������·ά�����
MSG_UNDEF//δ���壬����
};

typedef struct 
{
	int		nDevID;
	int		nMsgID;
	char	PreFix[N_LEN];//The UDP head
	char	SEQ[N_LEN];//Serial number
	char	COMMADDR[N_LEN];//SIM NO.
	char	CMDID[N_LEN];//command ID
	char	CMDARGUS[N_LEN];//commad parameters
	char	Time[N_LEN];// ���Ĳ���ʱ��
	char	UTC[N_LEN];//
	char	Latitude[N_LEN];//γ�ȣ���λΪ���� 16����ASCII�ַ���
	char	Longitude[N_LEN];//���ȣ���λΪ���� 16����ASCII�ַ���
	char	Altitude[N_LEN];//���θ߶ȣ���λΪ���� 16����ASCII�ַ���
	char	Heading[N_LEN];//���򣬵�λ�ȣ�ֵ��Χ0-360 16����ASCII�ַ���
	char	Speed[N_LEN];//�ٶ�	16����ASCII�ַ���	A6B	Ϊ��λm/h
	char	TransactionFlag[N_LEN];//����״̬��	��","�ָ���һ����16����ASCII�ַ���
	char	OEMStatus[N_LEN];//��չ״̬��
	char	StatusString[N_LEN];// �ն�״̬����	��","�ָ����ն�������Ϣ
	char	VERFYCODE[N_LEN];// Verify codes
	char	CRLF[N_LEN];//CR+LF
	char    comPressFlag[N_LEN];//���д���
	char    callTimes[N_LEN];//���д���
	char    IntervalTime[N_LEN];//����ʱ����
	char    IPV4[N_LEN];//�������IP��ַ
	char	PortNum[N_LEN];// ������Ķ˿ں�
	char	APN[N_LEN];//�ƶ������APN�ַ���
}GPSINFO;


long findstr(char *buf, char *targetstr,bool bLeft=true);
bool findStr(char *buf, char *targetstr,long &nx,long &ny);
bool fillStr(char *srcStr,char * dstStr, char *strTarget, long &nx, long &ny);
long getCRCVal( char *buf);
void getCRCPos( char *buf, long &xpos,long yPos );


#define FILLCR(a)	memcpy(a,"\r\n",2)
#define FillX(a,b)	if(fillStr(&buf[i],b,a,nx,ny)) i = ny + 1


#endif

