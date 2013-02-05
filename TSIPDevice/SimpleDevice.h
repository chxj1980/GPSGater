#pragma once
#include "itsdevice.h"
#include "I_XMLParser.h"
#include <afxmt.h>
typedef CArray<SOCKET, SOCKET&> SocketList;
class SimpleDevice :
	public ITsDevice_Card
{
public:
	SimpleDevice(void);
	~SimpleDevice(void);
	/*
	 *	��ʼ��Ӳ���豸
	 * @param nCardID  �忨�ţ���ϵͳ���豸���������е�����λ�ã���0��ʼ��
	 */
	virtual BOOL	InitDevice() ;
	virtual BOOL	ReleaseDevice() ;

	/*
	 *	��ʼ��������
	 */
	virtual int		StartInput() ;
	/*
	 *	ֹͣ�������
	 */
	virtual int		StopInput() ;

	virtual int sendData(const char *pData,int nDataSize,const char *pAddr,int nPort);
	virtual int sendData(const GPSGATEDATA *pGpsGateData);
	virtual void SetMemShareBuffer(MGBufferListEx *pBufferList,DWORD dwBufferSize) ;

private:
	//udp
	SOCKADDR_IN addr1;
	SOCKADDR_IN m_SendAddr;
	SOCKET      m_Socket_UDP;
	SOCKADDR_IN recvfromAddr;

	static UINT __stdcall StartCaptureThread_UDP(void* pParam);
	void CaptureOnThread_UDP();


	HANDLE			m_hCaptureThread_UDP;

	//tcp
	SOCKET			m_Socket_TCP;
	HANDLE			m_hAcceptThread_TCP;
	HANDLE			m_hRecvThread_TCP;	

	SocketList m_aClientSocketList;
	CCriticalSection m_cCriticalSection;

	CCriticalSection m_cClearSockets_CriticalSection;


	static UINT __stdcall _RecvDataProc_Select(void* pParam);
	void RecvDataProc_Select();

	static UINT __stdcall _AcceptConProc_TCP(void* pParam);
	void AcceptConProcThread();

	static UINT __stdcall _RecvDataProc(LPVOID lParam);
	void RecvDataThread();
	void PasreTcpCmd(int socketType,SOCKET s,char *pRecvBuf,int nLen,SOCKADDR_IN* pSockAddr_in);

	//////////////////////////////////////////////////////////////////////////
	HANDLE			m_hStopCaptureThread;
	MGBufferListEx				*m_pDataList;
	BOOL m_bIsExit;
private:
	I_XMLParser * m_pIXMLParser;


	BOOL m_bNeedClearSocketList;
	int  m_MAX_Socket_Connenct_Cnt ;
};
