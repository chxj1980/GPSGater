// GPSGateAdpter.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "GPSGateAdpter.h"
#include "IGPSGateAdpter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CGPSGateAdpterApp

BEGIN_MESSAGE_MAP(CGPSGateAdpterApp, CWinApp)
END_MESSAGE_MAP()

// CGPSGateAdpterApp ����

CGPSGateAdpterApp::CGPSGateAdpterApp()
{

	CreateNMLogWriter();
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_bInitAdpter=FALSE;
	m_pGPS_Socket = NULL;
	m_pDataOpter  = NULL;
	m_pGetCommand = NULL;
}


// Ψһ��һ�� CGPSGateAdpterApp ����

CGPSGateAdpterApp theApp;


// CGPSGateAdpterApp ��ʼ��

#include <process.h>
void CGPSGateAdpterApp::InitAdpter()
{
	if(m_bInitAdpter)return;
	WriteLog(LOGNAME,logLevelError,_T("CGPSGateAdpterApp::InitInstance"));


	printf("begin new()\r\n");
	if(m_pGPS_Socket==NULL)
	{
		printf("m_pGPS_Socket = new CGPS_Socket();\r\n");
		m_pGPS_Socket = new CGPS_Socket();
	}
	if(m_pDataOpter==NULL)
	{
		printf("m_pDataOpter  = new DataOpter();\r\n");
		m_pDataOpter  = new DataOpter();
	}
	if(m_pGetCommand==NULL)
	{
		printf("m_pGetCommand = new GetCommand();\r\n");
		m_pGetCommand = new GetCommand();
	}
	printf("End new()\r\n");

	printf("begin m_pGPS_Socket->startwork()\r\n");
	int nRet = m_pGPS_Socket->StartWork();

	printf("begin m_pDataOpter->startwork()\r\n");
	int nRet1 = m_pDataOpter->StartWork();
	printf("begin m_pGetCommand->startwork()\r\n");
	int nRet2 = m_pGetCommand->StartWork();
	printf("begin startwork()\r\n");
	CString sLog;

	sLog.Format(_T("m_pGPS_Socket->StartWork()=%d--%d--%d"),nRet,nRet1,nRet2);
	WriteLog(LOGNAME,logLevelError,sLog);
	m_bInitAdpter = TRUE;
}
BOOL CGPSGateAdpterApp::InitInstance()
{
	CWinApp::InitInstance();
	m_bInitAdpter = FALSE;
	return TRUE;
}
long getGPS(char *buf,char *addr,char * cPort)
{
	theApp.InitAdpter();
	int dwPort=0;
	
	long nRet = theApp.m_pGPS_Socket->getGPS(buf,addr,dwPort);
	ltoa(dwPort,cPort,10);
	return nRet;
}

long writeGPS(const char *buf,const char *addr,const char * cPort)
{
	theApp.InitAdpter();
	return theApp.m_pGPS_Socket->writeGPS(buf,addr,atol(cPort));
}

long getGPS( GPSGATEDATA *pGpsData,char *buf)
{
	theApp.InitAdpter();
	return theApp.m_pGPS_Socket->getGPS(pGpsData,buf);
	return 0;
}
long writeGPS(const GPSGATEDATA * pGpsData,char *pDatabuf,int nDataLen)
{
	theApp.InitAdpter();
	return theApp.m_pGPS_Socket->writeGPS(pGpsData,pDatabuf,nDataLen);
}

 int writedb(const GPSINFO *pGpsInfo)
 {
	 theApp.InitAdpter();
	return theApp.m_pDataOpter->writedb(pGpsInfo);
	return 0;
}
 int WriteCommand(GPSCommand * pGpsCommand)
 {
	 theApp.InitAdpter();
	 return theApp.m_pDataOpter->writeCommand(pGpsCommand);
 }

int getCmd(GPSCommand *pGpsCmd)
{
	theApp.InitAdpter();
	return theApp.m_pGetCommand->getCmd(pGpsCmd);
}
