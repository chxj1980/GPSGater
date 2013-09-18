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
#include <stdio.h>

#define	N_LEN	20

//GPS device type
enum{GPS_NULL,
	GPS_YOUHAO=0x0001,
	GPS_RUIXING = 0x0002,
	GPS_MEITRACK = 0X0003,
	GPS_JTT808   = 0x0004,
	GPS_DEV_MASK = 0x00FF,

	GPSID_SIM = 0x0100,
	GPSID_TID = 0x0200,
	GPSID_MASK = 0xFF00
};




//UDP Type
enum{MSG_NULL,
//upload data
//////////////////////////////////////////////////////////////////////////
//���Ǵ�
MSG_TXT,
MSG_HEX_V,
MSG_HEX_X,

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
typedef struct tagOBDInfo 
{
	char Horizontal_positioning_accuracy[N_LEN];	//<ˮƽ��λ����>,
	char Altitude[N_LEN];	//<���θ߶�>,
	char Mileage[N_LEN];//<�����>,
	char Runing_Time[N_LEN];//<����ʱ��>,
	char AGPS_Info[N_LEN];//<��վ��Ϣ>,
	char status_InOutTake[N_LEN];//<���������״̬>,
	char outputValue[N_LEN];//<ģ���������ֵ>,
	char RFID[N_LEN];//<RFID ��>/<ͼƬ��>/<Χ�����>,
	char Customize_Data[N_LEN];//<��������>,
	char Protocol_Version[N_LEN];//<Э��汾 V>

	char Engine_speed[N_LEN];//<������ת��>,
	char Engine_load[N_LEN];//<���������㸺��>,
	char Engine_coolant_temperature[N_LEN];//<��������ȴҺ�¶�>,
	char Hundred_kilometers[N_LEN];//<�ٹ����ͺ�>,
	char Intake_air_temperature[N_LEN];//<�����¶�>,
	char Hydraulic[N_LEN];//<��ѹ>,
	char Atmospheric_pressure[N_LEN];//<����ѹ��>,
	char Intake_pressure[N_LEN];//<�����ܾ���ѹ��>,
	char Air_flow[N_LEN];//<��������>,
	char Throttle_Position[N_LEN];//<������λ��>,
	char Single_mileage[N_LEN];//<������ʻ���>,
	char Remaining_fuel[N_LEN];//<�ٷֱ�ʣ������>,
	char ErrorCode[N_LEN];//<�����뼰����֡/����״̬>
}stOBDInfo;
typedef struct tagGPSInfo
{
	int		nDevID;
	int		nMsgID;
	DWORD   nWarnFlag;
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
	int     Noload;//00���ճ���01�����أ�10��������11������
	bool bValid;//�������Ƿ���Ч
	bool bNeedWriteDataBase;
	stOBDInfo	st_OBD_Info;
	int		nHaveOBDInfo;//obd����Ч����
	tagGPSInfo()
	{
		nHaveOBDInfo=0;
		bValid = false;
		Noload = 0;
		bNeedWriteDataBase = true;
	}
}GPSINFO;
//*XX,YYYYYYYYYY,CMD,HHMMSS,PARA1,PARA2,��#
enum CmdType{
	cmdType_ToGPS=1,
	cmdType_ToService=0xFF,
	cmdType_ToService_OverSpeed=cmdType_ToService+1
};
typedef struct{
	CmdType	commandType;
	char	strDevID[N_LEN];
	char	strSim[N_LEN];
	char	strCMDID[N_LEN*2];//��������ID���ַ�����ʾ
	char	*commandParameters;//��������Ĳ������ԡ���������
	int		nLenCmdParameters;
	char	*strCommandLine;//������ɺ�Ŀ�������
	int		nLenCommandLine;//������ɺ�Ŀ��������
	char	*pstrCommandXMl;//xml�����ַ���
	int		nLenCommandXML;
	void	*pVoid;
}GPSCommand;
//GPSINFO.nWarnFlag
enum
{
	WAR_OVERSPEED = 0x00000001,//���ٱ���,���GPSINFO.nWarnFlag & WAR_OVERSPEED !=0,���������
	WAR_OUTZONE	  = 0x00000002,//���籨��
	WAR_INZONE    = 0x00000004//��籨��
};

const int GPSINFOLEN = sizeof(GPSINFO);

long findstr(char *buf, char *targetstr,bool bLeft=true);
bool findStr(char *buf, char *targetstr,long &nlen);
bool fillStr(char *srcStr,char * dstStr, char *strTarget, long &nlen);
long getCRCVal( char *buf);
void getCRCPos( char *buf, long &xpos,long yPos );


#define FILLCR(a)	memcpy(a,"\r\n",2)
#define FillX(a,b)	if(fillStr(&buf[i],b,a,nlen)) i += nlen
#define UNFUN MessageBox(NULL,"�ݲ�֧��","Warning",MB_ICONWARNING);

#endif

