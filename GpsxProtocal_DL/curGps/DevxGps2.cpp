/********************************************************************
	created:	2011/12/13
	created:	13:12:2011   14:57
	filename: 	E:\WORKSPACE\PROJECT\DEV-GPS\CODES\DLL_DEV_GPS\CURGPS\DevxGps\DevxGps.cpp
	file path:	E:\WORKSPACE\PROJECT\DEV-GPS\CODES\DLL_DEV_GPS\CURGPS\DevxGps
	file base:	DevxGps
	file ext:	cpp
	author:		areslion@qq.com
	
	purpose:	
*********************************************************************/
#include <memory.h>
#include <string.h>
#include "DevxGps.h"

HANDLE hTh_GetGPS = NULL;
HANDLE hTh_GetDB = NULL;
DWORD  IDThGPS = 0;
DWORD  IDThDB = 0;
DWORD WINAPI threadGetGPSData(LPVOID lpParameter);
DWORD WINAPI threadGetWriteDB(LPVOID lpParameter);


Protocal	Gdev;


long start()
{
	long nret = 0;

	hTh_GetGPS = CreateThread(NULL,0,threadGetGPSData,NULL,0,&IDThGPS);
// 	Sleep(30000);//Ϊ�����쳣��ʱ�رմ����ݿ�ȡ���ݿ�ģ��
// 	hTh_GetDB = CreateThread(NULL,0,threadGetWriteDB,NULL,0,&IDThGPS);	
	
	return nret;
}


long stop()
{
	long nret = 0;

	TerminateThread(hTh_GetDB,0);
	TerminateThread(hTh_GetGPS,0);
	
	return nret;
}





DWORD WINAPI threadGetGPSData(LPVOID lpParameter)
{
	char *buf = NULL;
	char strFilename[20]="Console";
	int nlen = sizeof(GPSINFO)+20;
	
	buf = (char *)malloc(nlen);	
	if(!buf) 
	{
		Gdev.m_pGps->wlog("threadGetGPSData-����buf�ռ�ʧ��",strFilename);
		MessageBox(NULL,"threadGetGPSData-����buf�ռ�ʧ��","Warning",0);
		return 0;
	}
	GPSCommand *gpsCommand = new GPSCommand();

	char *pTempBuffer = new char[1024*2];
	char *pstrCommandLine = pTempBuffer;
	char *pCommandPara = pTempBuffer + 1024/2;
	char *pGpsGateDataBuf = pTempBuffer+1024;
	
	while (TRUE)
	{
		ZeroMemory(pTempBuffer,1024*2);

		Gdev.startGPS(buf,nlen);

		//�������ȡ��������������
		ZeroMemory(gpsCommand,sizeof(GPSCommand));
		gpsCommand->strCommandLine = pstrCommandLine;
		gpsCommand->commandParameters = pCommandPara;
		Gdev.Process_Command(gpsCommand,pGpsGateDataBuf);
		Sleep(50);
	}
	
	if(buf!=NULL) free(buf);
	return 0;
}


long Protocal::stop()
{
	long nret = 0;
	return nret;
}

long Protocal::returnStar( char *buf, long nret ,char *strlog,char *strFileName)
{
	if(buf) free(buf);
	if(strlen(strlog)) m_pGps->wlog(strFileName,strlog);
	return nret;
}

long Protocal::returnStar(long nret ,char *strlog,char *strFileName)
{
	if(strlen(strlog)) m_pGps->wlog(strFileName,strlog);
	return nret;
}

Protocal::Protocal()
{
	m_pGps = NULL;
	memset(&m_gpsInfo,0x00,sizeof(GPSINFO));
	memset(m_strLogName,0x00,N_LEN);
	strcpy(m_strLogName,"aaaaa");
	strcpy(m_strConsole,"Console");
	InitializeCriticalSectionAndSpinCount(&m_mapLock, 0xFA0);
	
// 	m_gpsIPMap.clear();
}


bool Protocal::setCurGpsDev( char *buf,GPSINFO &gpsInfo )
{
	char	strTmp[100] = "";
	int		nlen = 0;

	if(buf[0]=='*'||buf[0]=='$'||buf[0]=='X')
	{
		gpsInfo.nDevID += GPS_RUIXING;
		gpsInfo.nDevID += GPSID_TID;
		m_pGps = &m_gpsRuiXing;

		if(buf[0]=='*') nlen = 77;
		else nlen = 32;
		memcpy(strTmp,buf,nlen);
		memcpy(buf,strTmp,nlen);
	}
	else if(0)
	{
		gpsInfo.nDevID += GPS_YOUHAO;
		gpsInfo.nDevID += GPSID_SIM;
		m_pGps = &m_gpsYouHao;
	}
	else return false;
	m_nDataLen = nlen;
	return true;
}



long Protocal::getGpsInfo( char *buf,GPSINFO &gpsInfo )
{
	long nret = 0;
	BOOL bToGps = FALSE;

	if(!strlen(buf)) bToGps = TRUE;
	nret = m_pGps->getGpsInfo(buf,gpsInfo);
	m_pGps->wlog(buf,m_nDataLen,gpsInfo,bToGps);


	return nret;
}



long Protocal::SynchronGPSData(char *pInData,int nLenInData,GPSINFO &gpsInfo)
{
	long	nret = 0;

	if(!setCurGpsDev(pInData,gpsInfo))	return 1;//txt do not fitable any protocal
	nret = getGpsInfo(pInData,gpsInfo);
	
	return nret;
}



//GPSGATEDATA gpsData
long Protocal::readGPS(GPSGATEDATA &gpsData)
{
	long nret = 0;
	
	nret = getGPS(&gpsData);
	return nret;
}

long Protocal::updateHaxiMap(char *strSIM,GPSGATEDATA gpsData)
{
	long nret = 0;
	char strTmp[512]="";
	
	pair<RUIX_MAP::iterator, bool> inserted;
	
	
	EnterCriticalSection(&m_mapLock);
	m_gpsRuiXMap.erase(strSIM);
	inserted = m_gpsRuiXMap.insert(RUIX_MAP::value_type(strSIM,gpsData));
	if(inserted.second==true) sprintf(strTmp,"updateHaxiMap-��ϣ���гɹ�����SIM��Ϣ-%s",strSIM);
	else sprintf(strTmp,"updateHaxiMap-��ϣ���в���SIM��Ϣʧ��-%s",strSIM);
	m_pGps->wlog("Console",strTmp);
	
	LeaveCriticalSection(&m_mapLock);
	return nret;
}

//BOOL getIPPort(char *strSIM,GPSGATEDATA gpsData);
BOOL Protocal:: getIPPort(char *strSIM,GPSGATEDATA &gpsData)
{
	BOOL bret = FALSE;
	char strTmp[100]="";
	char strFilname[20]="DevxGps.cpp";
	
	RUIX_MAP::iterator findItem;
	
	
	EnterCriticalSection(&m_mapLock);
	//m_gpsRuiXMap.

 	findItem = m_gpsRuiXMap.find(strSIM);
	if(findItem==m_gpsRuiXMap.end())
	{
		sprintf(strTmp,"getIPPort-��IPӳ�����û���ҵ�SIM�ĵ�ַ��Ϣ-SIM:%s",strSIM);
		m_pGps->wlog(m_strConsole,strTmp);
		bret= FALSE;
	}
	else
	{
		gpsData =  findItem->second;
		sprintf(strTmp,"getIPPort-��IPӳ������ҵ���Ϣ-%s",strSIM);
		m_pGps->wlog(m_strConsole,strTmp);
		bret = TRUE;
	}
	LeaveCriticalSection(&m_mapLock);	
	
	return bret;	
}



BOOL Protocal::writeDataBase( GPSINFO gpsInfo )
{
	return writedb(&gpsInfo);
}

//long Protocal::startProcess_GPSData()
//{
//	const int nDataBufMaxSize=1024;
//	char *pDataBuf = new char[nDataBufMaxSize];
//
//	long nret = 0;
//	char strTmp[512]="";
//	char strFilename[20]="";
//	GPSINFO gpsInfo;
//	GPSGATEDATA  gpsData;
//
//	strcpy(strFilename,"Console");
//	memset(&gpsInfo,0x00,sizeof(GPSINFO));
//	memset(&gpsData,0x00,sizeof(GPSGATEDATA));
//	gpsData.pDatabuf = pDataBuf;
//	BOOL bExit=FALSE;
//
//	while(!bExit)
//	{
//		long nLenGpsData = readGPS(gpsData);
//		if(nLenGpsData<1)
//		{
//			returnStar(0,"",strFilename);
//			Sleep(50);
//			continue;
//		}
//
//		sprintf(strTmp,"[GPS]��[Console]-nDataLen:%d",gpsData.nDataLen);
//		m_pGps->wlog(strFilename,gpsData.pDatabuf,gpsData.nDataLen);
//
//		long nRet = 0;
//		nRet = SynchronGPSData(pDataBuf,gpsData.nDataLen,gpsInfo);
//		if(nRet<1)
//		{
//			returnStar(2,"startGPS-����GPS���Ķ��еı��Ĳ����ϱ��ĵĹ淶",strFilename);
//			continue;
//		}
//		nRet = updateHaxiMap(gpsInfo.COMMADDR,gpsData);
//
//		nRet = writeDataBase(gpsInfo);
//		if(nRet <0)
//		{
//			m_pGps->wlog(gpsInfo.COMMADDR,"[Protocal]��[DB]-Fail to write DB");
//			continue;
//		}
//
//		nRet = m_pGps->getResMsg(pDataBuf,gpsInfo);
//		if(nRet >0)
//		{
//			nRet = writeGPSx(gpsData);
//			if(nRet<1)
//			{
//				sprintf(strTmp,"[Console]��[GPS]%s-ʧ��",gpsData.pDatabuf);			
//			}
//			else 
//				sprintf(strTmp,"[Console]��[GPS]-%s",gpsData.pDatabuf);
//			m_pGps->wlog(gpsInfo.COMMADDR,strTmp);
//		}
//		sprintf(strTmp,"starGps-��ǰ���Ĳ���Ҫ��Ӧ--SIM:%s",gpsInfo.COMMADDR);
//		
//
//	}
//	return 0;
//
//}
long Protocal::startGPS(char *buf,int nlen)
{
	long nret = 0;
	char strTmp[1024]="";
	char strFilename[20]="";
	GPSINFO gpsInfo;
	GPSGATEDATA  gpsData;
	
	strcpy(strFilename,"Console");
	memset(buf,0x00,nlen);
	memset(&gpsInfo,0x00,sizeof(GPSINFO));
	memset(&gpsData,0x00,sizeof(GPSGATEDATA));
	
	gpsData.pDatabuf = buf;	
	if(!readGPS(gpsData)) return returnStar(0,"",strFilename);
// 	strcpy(buf,"*HQ,6120108162,V1,044541,A,2624.7708,N,10317.3470,E,0.00,354,270312,FFFFFBFF#");
// 	sprintf(strTmp,"[GPS]��[Console]-nDataLen:%d",gpsData.nDataLen);
// 	m_pGps->wlog(strFilename,gpsData.pDatabuf,gpsData.nDataLen);

	if(SynchronGPSData(buf,gpsData.nDataLen,gpsInfo)) return returnStar(2,"startGPS-����GPS���Ķ��еı��Ĳ����ϱ��ĵĹ淶",strFilename);
	
	//���ݱ�����������������Ĳ���
	if(gpsInfo.nMsgID == MSG_LOGIN)
	{
		/*if(nret)*/ updateHaxiMap(gpsInfo.COMMADDR,gpsData);
	}
	else if(gpsInfo.nMsgID == MSG_TERMINALFEEDBACK)
	{
		sprintf(strTmp,"v4-->%s->%s",gpsInfo.CMDID,gpsInfo.CMDARGUS);
		m_pGps->wlog(gpsInfo.COMMADDR,strTmp);	
	}
	
	{
		//normal����

		int nRet = writeDataBase(gpsInfo);
		if(nRet<1) 
		{
			sprintf(strTmp,"[Protocal]��[DB]-Fail to write DB.%d",nRet);
			m_pGps->wlog(gpsInfo.COMMADDR,strTmp);	
		}
		//else m_pGps->wlog(gpsInfo.COMMADDR,"[Protocal]��[DB]-success to write DB");

		if(m_pGps->getResMsg(buf,gpsInfo))
		{
			nRet =writeGPSx(gpsData); 
			if(nRet<1)
			{
				sprintf(strTmp,"ret [Console]��[GPS]%s-ʧ��.%d",gpsData.pDatabuf,nRet);			
			}
			else sprintf(strTmp,"ret [Console]��[GPS]-%s",gpsData.pDatabuf);			
			m_pGps->wlog(gpsInfo.COMMADDR,strTmp);
		}
		else sprintf(strTmp,"starGps-��ǰ���Ĳ���Ҫ��Ӧ--SIM:%s",gpsInfo.COMMADDR);

	}
	return nret;	
}

long Protocal::start()
{
	return 0;
}

long Protocal::Process_Command(GPSCommand *gpsCommand,char *pGpsDataBuf)
{
	///��GPSGateAdpter��ȡ����
	//�����xml��ʽ���ŵ�GPS�豸�н����������buffer
	//��buffer ͨ��socket ���͸� ��Ӧ��gps
	
	int nRet = getCmd(gpsCommand);
	if(nRet < 1)
		return nRet;
	char strTmp[100];
	do{
		if(gpsCommand->commandType == cmdType_ToGPS)
		{
			GPSGATEDATA gpsData;

			//ͨ��deviceID ��ȡ �豸��ǰ�� socket��Ϣ
			int iCnt = 0;
			BOOL bGetIPPort  = false;
			while(!bGetIPPort)
			{
				bGetIPPort = getIPPort(gpsCommand->strDevID,gpsData);
				if(iCnt++ > 5)
					break;
			}
			iCnt=0;
			while(!bGetIPPort)
			{
				bGetIPPort = getIPPort(gpsCommand->strSim,gpsData);
				if(iCnt++ > 5)
					break;
			}
			if(!bGetIPPort)
			{
				//��ʱ��û�л�ȡ����Ӧsim ��ip��port��ֱ�Ӷ���
				//����������������ȥ���´���ʹ��
				nRet = -2;
				break;
			}
			gpsData.pDatabuf = pGpsDataBuf;
			memcpy(gpsData.pDatabuf,gpsCommand->strCommandLine,gpsCommand->nLenCommandLine);
			gpsData.nDataLen = gpsCommand->nLenCommandLine;

			nRet = writeGPSx(gpsData);	
			if(nRet<1)
			{
				sprintf(strTmp,"[Console]��[GPS]%s-ʧ��",gpsData.pDatabuf);			
			}
			else 
				sprintf(strTmp,"[Console]��[GPS]-%s",gpsData.pDatabuf);

		}
		//�������Ƿ���GPS���������
		nRet= WriteCommand(gpsCommand);

	}while(0);

 	char strLog[100];
 	sprintf(strLog,"cmd:%d,ret:%d--",gpsCommand->commandType,nRet);
 	m_pGps->wlog(m_strConsole,strLog);

	return nRet;
}
//GPSGATEDATA gpsData
long Protocal::writeGPSx(GPSGATEDATA gpsData)
{
	long nret = 0;
	
	//	nret = writeGPS(buf,addr,port);
	nret = writeGPS(&gpsData);
	
	return nret;
}