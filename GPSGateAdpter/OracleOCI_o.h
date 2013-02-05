#pragma once

#include "Connection.h"
//#include <iostream.h>
#include <stdlib.h>


#include "GpsData.h"
struct DataInfo 
{
	//��Щ��Ϣ ֻ��gps��������ʱ����
	DataInfo(){};
	DataInfo(double longitude,double latitude,DWORD time)
	{
		doubleLongitude = longitude;
		doubleLatitude = latitude;
		dLastTime = time;
	};
	double doubleLongitude;
	double doubleLatitude;
	int		nHeading;
	DWORD dLastTime;
	DWORD dwMaxSpeed;
	char	vehicheID[20]; 
};
//map��ֻ���浱ǰ����ͬ��������һ����
struct CommandItem{	
	int recordID;
	int nCmdType;
	char devID[20];
	char simID[20];
	char vehicle_ID[20];
	char cmdID[20];
	char cmdParam[20];
};

#include <map>
#include <afxmt.h>

typedef enum _tagJudge_ret
{
	RET_INVALID=-1,  //λ����Ϣ��Ч
	RET_NO_CAR=-3,	//û�������	���������ݿ�
	RET_SAME_LOCATION=-4,//�ж� ���ϴε������Ƿ�һ����һ���Ͳ������
	RET_NORMAL=1,	//�������������������뵽���ݿ�
	RET_OVERSPEED=2,	//�����������������ٶȳ����ˡ�
	RET_OVERAREA_OUT=3,	// ����������������Խ���ˣ�
	RET_OVERAREA_IN=4,	//���������������ǽ����˲��ý��������
	RET_GPS_ANSWER=5,	//������Ϊgps�ش����ݣ��ٶ��������Ϣ������
	RET_PIAOYI=6,	//������������ǵ�ǰλ�ã��ٶ�Ϊ0������Ư������ ���������ݿ�


}JUDGE_RET;

class COracleOCI_o
{
public:
	COracleOCI_o(void);
	~COracleOCI_o(void);
	
	int Init(char *user,char*pwd,char*serverName,BOOL bInsertAsNewVehicle,DWORD dwUpdateVehicleTime,char *serverAddr=NULL);
	int InsertData( const GPSINFO *pGpsInfo ,double doubleLongitude,double doubleLatitude,CStringA s_RecvTime ,CStringA str_CurTime,int iState);
	int InsertData(char *pInsertDataSQL);
	int CreateTable(struct tm* pCurGPSTime);
	int Updata(const GPSINFO *pGpsInfo ,double doubleLongitude,double doubleLatitude,CStringA s_RecvTime,CStringA str_CurTime,JUDGE_RET judge_ret);
	void Close();
	int getInfos_Form_VEHICLE();
	/*
 *	@return -1 λ����ϢΪ0
			-2	������������ǵ�ǰλ�ã��ٶ�Ϊ0������Ư������ ���������ݿ�
			-3	û�������	���������ݿ�
			1	�������������������뵽���ݿ�
			2	�����������������ٶȳ����ˡ�
			3   ����������������Խ���ˣ�
			4   ���������������ǽ����˲��ý��������
			5	������Ϊgps�ش����ݣ��ٶ��������Ϣ������

 */
	JUDGE_RET judge_GPSData(const GPSINFO* pGpsInfo,const INT64 *iSim,int nSpeed,double doubleLongitude,double doubleLatitude,DataInfo **pDI);
//	int WriteData( const GPSINFO *pGpsInfo );
	int WriteData( const GPSINFO *pGpsInfo );
	int writeCommand(GPSCommand * pGpsCommand);
	void printf_ErrorLog(LPCTSTR lpExtInfo);
	int Insert_As_NewVehicle(const char *pTid,const char*pSim);
	int Insert_OverSpeed_Data( const GPSINFO *pGpsInfo ,double doubleLongitude,double doubleLatitude,CStringA s_RecvTime ,CStringA str_CurTime,DataInfo *pDI);
	int Update_CommandState(char *pVehicleID);
	int Excute_DeleteSQL(char *pDeleteSQL);
	DataInfo * GetVehicleInfo(const char *pStrDevID);
	int Insert_Transgress_Data( const GPSINFO *pGpsInfo ,double doubleLongitude,double doubleLatitude,CStringA s_RecvTime ,CStringA str_CurTime,DataInfo *pDI );
	int _DoCreateTable(const char*createTableSQL);
	int Insert_ODBInfo_Data(const GPSINFO *pGpsInfo,CStringA str_CurTime);
	CConnection conn;
private:

	char m_strDate[8];
	char m_strCreateTableSQL[1024*2];
	char m_strInsertDataSQL[1024*2];
	char m_strUpdateDataSQL[1024*2];
	char m_strUpdateDataSQL_ForCmd[1024*2];
	char m_strDeleteDataSQL_ForCmd[1024*2];
private:
	std::map<INT64,DataInfo*> m_DataInfoMap;
	CArray<CommandItem*,CommandItem*> m_ArrayCommandItem;


	struct tm	m_tm_TableNameTime;
	time_t		m_time_t_TableNameTime;
	__int64		m_i64d_GetVehicleInfo;//��vehicle��ȡ��Ϣ��ʱ�� since 1970.01.01 00:00:00
	DWORD		m_dwUpdateVehicleTime;

	BOOL m_bInsert_As_NewVehicle;

	BOOL m_bInitOCI;
public:
	DWORD		m_dwLimit_MinSpeed;//��ΪƯ�Ƶ���С�ٶ���ֵ
	DWORD		m_dwLimit_MaxDistance;//Ư���жϵ� ���룬�������ֵ����ΪƯ�ơ�

};
