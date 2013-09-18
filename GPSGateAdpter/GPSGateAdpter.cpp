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
	m_pGPS_Socket = new CGPS_Socket();
	m_pDataOpter  = new DataOpter();
	m_pGetCommand = new GetCommand();
}


// Ψһ��һ�� CGPSGateAdpterApp ����

CGPSGateAdpterApp theApp;


// CGPSGateAdpterApp ��ʼ��

#include <process.h>
BOOL CGPSGateAdpterApp::InitInstance()
{
	WriteLog(LOGNAME,logLevelError,_T("CGPSGateAdpterApp::InitInstance"));
	CWinApp::InitInstance();
	int nRet = m_pGPS_Socket->StartWork();
	int nRet1 = m_pDataOpter->StartWork();
	int nRet2 = m_pGetCommand->StartWork();
	CString sLog;

	sLog.Format(_T("m_pGPS_Socket->StartWork()=%d--%d--%d"),nRet,nRet1,nRet2);
	WriteLog(LOGNAME,logLevelError,sLog);
	return TRUE;
}

long getGPS(char *buf,char *addr,char * cPort)
{
	int dwPort=0;
	
	long nRet = theApp.m_pGPS_Socket->getGPS(buf,addr,dwPort);
	ltoa(dwPort,cPort,10);
	return nRet;
}

long writeGPS(const char *buf,const char *addr,const char * cPort)
{
	return theApp.m_pGPS_Socket->writeGPS(buf,addr,atol(cPort));
}

long getGPS( GPSGATEDATA *pGpsData,char *buf)
{
	return theApp.m_pGPS_Socket->getGPS(pGpsData,buf);
	return 0;
}
long writeGPS(const GPSGATEDATA * pGpsData,char *pDatabuf,int nDataLen)
{
	return theApp.m_pGPS_Socket->writeGPS(pGpsData,pDatabuf,nDataLen);
}

 int writedb(const GPSINFO *pGpsInfo)
{
	return theApp.m_pDataOpter->writedb(pGpsInfo);
	return 0;
}
 int WriteCommand(GPSCommand * pGpsCommand)
 {
	 return theApp.m_pDataOpter->writeCommand(pGpsCommand);
 }

int getCmd(GPSCommand *pGpsCmd)
{
	return theApp.m_pGetCommand->getCmd(pGpsCmd);
}
