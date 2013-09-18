#include "StdAfx.h"
#include "GPS_Iocp.h"
#include "ClientContext.h"
#include "ClientManager.h"

#define  MAX_IOCP_THREAD	(35)
GPS_Iocp::GPS_Iocp(void)
{
}

GPS_Iocp::~GPS_Iocp(void)
{
}

int GPS_Iocp::InitializeSocket()
{
	CString strLog;
	WSADATA wsaData;	
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{				
		strLog.Format(_T("WSAStartup() Failed. Err=%d. GTMIOCP::InitliazeSocket()"), WSAGetLastError());
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
		return FALSE;
	}	

	WriteLog(SERVERLOGNAME, logLevelInfo, _T("InitliazeSocket() Successed. GTMIOCP::InitliazeSocket()"));
	OutputDebugString( _T("InitliazeSocket() Successed. GTMIOCP::InitliazeSocket()"));
	return TRUE;
}

int GPS_Iocp::Listen( char* ulIP, unsigned short usPort )
{
	int iErrcode = 0;
	CString strLog;

	//�����׽���
	if ((m_sListen = WSASocket(AF_INET,
		SOCK_STREAM,
		0,
		NULL, 
		0,
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{		
		strLog.Format( _T("WSASocket() Failed.Err=%d. GTMIOCP::InitliazeSocket()"), WSAGetLastError());
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
		return FALSE;
	} 

	//���׽���
	SOCKADDR_IN	servAddr;//��������ַ
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = INADDR_ANY;//htonl(ulIP);
	servAddr.sin_port = htons(usPort);	

	CString strIP;
	//strIP.Format(_T("%s"), A2W(inet_ntoa(servAddr.sin_addr)));
	//strIP.Format(_T("%s"),A2W(ulIP));

	if (bind(m_sListen, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		iErrcode = WSAGetLastError();

		strLog.Format(_T("bind() Failed.Err=%d.IP=%s.Port=%d. GTMIOCP::Listen"), 
			iErrcode, strIP, usPort);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

		ClearResources(FALSE);
		return iErrcode;
	}
	//����
	if(listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		iErrcode = WSAGetLastError();

		strLog.Format(_T("listen() Failed.Err=%d.IP=%s.Port=%d. GTMIOCP::Listen"),
			iErrcode,strIP, usPort);
		WriteLog(SERVERLOGNAME,logLevelWarring, strLog);

		ClearResources(FALSE);
		return iErrcode;
	}
	struct sockaddr_in sin;
	int len = sizeof(sin);
	if(getsockname(m_sListen, (struct sockaddr *)&sin, &len) != 0)
	{
		printf("getsockname() error:%s\n", strerror(errno));
		ClearResources(FALSE);
		return WSAGetLastError();
	}
	DWORD dwPort = ntohs(sin.sin_port);


	m_bRunning = TRUE;

	//�������ܿͻ��������¼�����
	m_hListenEvent = WSACreateEvent();
	if ( m_hListenEvent == WSA_INVALID_EVENT )
	{	
		iErrcode = WSAGetLastError();

		strLog.Format(_T("WSACreateEvent() Failed.Err=%d.IP=%s.Port=%d. GTMIOCP::Listen"),
			iErrcode, strIP, usPort);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

		ClearResources(FALSE);
		return iErrcode;
	}

	//Ϊ�����׽���ע��FD_ACCEPT�����¼�	
	if ( WSAEventSelect(m_sListen, m_hListenEvent, FD_ACCEPT) == SOCKET_ERROR )
	{
		iErrcode = WSAGetLastError();

		strLog.Format(_T("WSAEventSelect() Failed.Err=%d.IP=%s.Port=%d. GTMIOCP::Listen"),
			iErrcode, strIP, usPort);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

		ClearResources(TRUE);
		return iErrcode;
	}

	// ������ɶ˿�
	if ((m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		iErrcode = WSAGetLastError();

		strLog.Format(_T("CreateIoCompletionPort() Failed.Err=%d.IP=%s.Port=%d. GTMIOCP::Listen"),
			iErrcode, strIP, usPort);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

		ClearResources(TRUE);
		return iErrcode;
	}

	//�����ڴ��
	m_pMemPool = new CMemPool;
	m_dwMemPoolSize = 1024*10;
	if(!m_pMemPool->Initialize(m_dwMemPoolSize, 500, 200))
	{
		return 1;
	}

//	m_ulLocalIP = ulIP;
	m_usLocalPort = usPort;

	//�������ܿͻ��������߳�
	DWORD dwThreadID = 0;	
	m_hThread[0] = CreateThread(NULL, 
		0,
		_AcceptThread,
		this, 
		0,  
		&dwThreadID);
	if (NULL == m_hThread)
	{
		iErrcode = GetLastError();

		strLog.Format(_T("CreateThread(_AcceptThread) Failed.Err=%d.IP=%s.Port=%d. GTMIOCP::Listen"),
			iErrcode, strIP, usPort);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

		ClearResources(TRUE);
		return iErrcode;
	}
	m_nThreadNum = 1;

	// ��ȡCPU����
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	DWORD dwThreadSum = SystemInfo.dwNumberOfProcessors * 2 + 2;//����ֵ
    if(dwThreadSum > MAX_IOCP_THREAD)
	{
		dwThreadSum = MAX_IOCP_THREAD;
	}

	strLog.Format(_T("CreateThread Count(%d),Current Processors number(%d) GTMIOCP::Listen."),dwThreadSum,SystemInfo.dwNumberOfProcessors);
	WriteLog(SERVERLOGNAME, logLevelInfo, strLog);

	// ���������߳�
	for(DWORD i = m_nThreadNum; i < dwThreadSum; i++)
	{
		if ((m_hThread[m_nThreadNum++] = CreateThread(NULL, 
			0,
			_ServiceThread,
			this,
			0, 
			&dwThreadID)) == NULL)
		{
			iErrcode = GetLastError();

			strLog.Format(_T("CreateThread(_ServiceThread) Failed.Err=%d.IP=%s.Port=%d.ThreadNum=%d. GTMIOCP::Listen"),
				iErrcode, strIP, usPort, i);
			WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

			ClearResources(TRUE);
			return iErrcode;
		}		
	}

	m_hDoDetect = CreateEvent(0,0,0,"dodetect");
	CreateThread(NULL,
		0,
		_DoDetectThread,
		this,
		0,
		&dwThreadID);
	strLog.Format(_T("GTMCreateServer() Successed. _ServiceThread dwThreadSum=%d. IP=%s.Port=%d. GTMIOCP::Listen"),
		dwThreadSum,strIP, usPort);
	WriteLog(SERVERLOGNAME,logLevelInfo, strLog);

	return 0;

}


void GPS_Iocp::ClearResources(BOOL bCloseListenEvent)
{
	if (bCloseListenEvent)
	{
		if (m_hListenEvent != WSA_INVALID_EVENT)
		{
			WSACloseEvent(m_hListenEvent);	//�ر��¼�
			m_hListenEvent = WSA_INVALID_EVENT;
		}		
	}

	if (m_sListen != INVALID_SOCKET)
	{
		closesocket(m_sListen);
		m_sListen = INVALID_SOCKET;
	}	
}
DWORD WINAPI GPS_Iocp::_AcceptThread( void *pParam )
{	
	GPS_Iocp		*pGTMIOCP = (GPS_Iocp*)pParam;		//������ָ��
	HANDLE		hComPort = pGTMIOCP->m_hCompPort;	//��ɶ˿�
	SOCKET		sListen = pGTMIOCP->m_sListen;		//�����׽���
	SOCKET		sAccept = INVALID_SOCKET;			//�����׽���
	int			iErrorCode;
	CString		strLog;
	CString		strIP;
	TCHAR		wcIP[MAX_PATH];
	DWORD		dwAcceptID=0;

	while(pGTMIOCP->m_bRunning)
	{
		DWORD dwRet;
		dwRet = WSAWaitForMultipleEvents(1,			//�ȴ������¼�
			&pGTMIOCP->m_hListenEvent,
			FALSE,
			100,
			FALSE);		
		if(!pGTMIOCP->m_bRunning)					//������ֹͣ����
			break;

		if (dwRet == WSA_WAIT_TIMEOUT)				//�������ó�ʱ
			continue;

		WSANETWORKEVENTS events;					//�鿴�����������¼�
		int nRet = WSAEnumNetworkEvents(pGTMIOCP->m_sListen,
			pGTMIOCP->m_hListenEvent,//�¼���������
			&events);		
		if (nRet == SOCKET_ERROR)
		{		
			iErrorCode = WSAGetLastError();

			strLog.Format(_T("WSAEnumNetworkEvents() failed. Err=%d."),iErrorCode);
			WriteLog(SERVERLOGNAME,logLevelWarring, strLog);

			break;
		}

		if ( events.lNetworkEvents & FD_ACCEPT)		//����FD_ACCEPT�����¼�
		{
			if ( events.iErrorCode[FD_ACCEPT_BIT] == 0 && pGTMIOCP->m_bRunning)
			{
				//���ܿͻ�������
				SOCKADDR_IN servAddr;
				int	serAddrLen = sizeof(servAddr);	   
				if ((sAccept = WSAAccept(sListen, 
					(SOCKADDR*)&servAddr,
					&serAddrLen,
					NULL, 
					0)) == SOCKET_ERROR)
				{					
					iErrorCode = WSAGetLastError();

					strLog.Format(_T("WSAAccept() failed. Err=%d."),iErrorCode);
					WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

					break;
				}

				char *pIP = inet_ntoa(servAddr.sin_addr);				
#ifdef _UNICODE
				size_t convertedChars = 0;
				mbstowcs_s(&convertedChars, wcIP,  (strlen(pIP) + 1), pIP, _TRUNCATE);
				strIP.Format(_T("%s"), wcIP);				 
#else
				strIP.Format(_T("%s"), pIP);
#endif

				//�����ͻ��˽ڵ�
				CClientContext *pClient = new CClientContext(dwAcceptID++,sAccept,pGTMIOCP, pGTMIOCP->m_pMemPool);

				//strLog.Format(_T("WSAAccept() one client. IP=%s.Port=%d. client=%x"),strIP, ntohs(servAddr.sin_port),pClient);
				//WriteLog(SERVERLOGNAME, logLevelInfo, strLog);
				OutputDebugString(strLog);
				
				pClient->SetConnectionInfo(pGTMIOCP->m_ulLocalIP, pGTMIOCP->m_usLocalPort, ntohl(servAddr.sin_addr.s_addr), ntohs(servAddr.sin_port));
				pClient->SetReceiveCallBack(pGTMIOCP->m_pRecvCallBackFunc,pGTMIOCP->m_pRecvCBUserData);

				if (CreateIoCompletionPort((HANDLE)sAccept,	//�׽�������ɶ˿ڹ�������
					hComPort,
					(DWORD) pClient,//��ɼ�
					0) == NULL)
				{
					iErrorCode = GetLastError();

					strLog.Format(_T("CreateIoCompletionPort() failed. Err=%d.IP=%s.Port=%d."),
						iErrorCode, strIP, servAddr.sin_port);
					WriteLog(SERVERLOGNAME,logLevelWarring, strLog);

					return -1;
				}

				_ClientData clientdata;
				clientdata.socket = sAccept;
				clientdata.addrClient = servAddr;				
				clientdata.pClient = pClient;

		//		WSARecv(sAccept,
//					)
				//�������ͻ�������
				CClientManager *pClientMgr = CClientManager::GetClientManager();
				pClientMgr->AddClient(clientdata);


				//�ص����ϲ㣬���յ�һ������
				pClient->AsyncRecvData();
				//pGTMIOCP->m_pConnectCBFunc(STATUS_OK, pClient, pGTMIOCP->m_pConnectCBUserData);								
			}
		}		
	}

	//�˳��̣߳��ͷ���Դ
	WriteLog(SERVERLOGNAME, logLevelInfo, _T("Exit GTMIOCP::_AcceptThread()."));
	return 0;
}
DWORD WINAPI GPS_Iocp::_DoDetectThread(void *pParam){

	GPS_Iocp *pGTMIOCP = (GPS_Iocp*)pParam;//������ָ��
	CString	strLog;

	DWORD dwClientCount=0;
	DWORD dwStartTime = GetTickCount();
	__int64 i64d_AllDeleteCount =0;
	while(1){
		WaitForSingleObject(pGTMIOCP->m_hDoDetect,30*1000);
		dwClientCount = CClientManager::GetClientManager()->GetClientCount();
		DWORD dwDeleteCount = CClientManager::GetClientManager()->DetectLiveTime();
		i64d_AllDeleteCount+=dwDeleteCount;
			printf("dectected[%d]--del[ALL:%I64d,%d]---%d--all:%I64d\n",GetTickCount()-dwStartTime,i64d_AllDeleteCount,dwDeleteCount,dwClientCount,CClientManager::GetClientManager()->GetAllClientCount());
	}
	return 1;
}
DWORD WINAPI GPS_Iocp::_ServiceThread( void *pParam )
{
	GPS_Iocp *pGTMIOCP = (GPS_Iocp*)pParam;//������ָ��
	HANDLE	hComPort = pGTMIOCP->m_hCompPort;//��ɶ˿�
	CString	strLog;

	DWORD			dwIoSize;		//�����ֽ���
	CClientContext	*pClient;		//�ͻ���ָ��
	LPOVERLAPPED	lpOverlapped;	//�ص��ṹָ��
	bool			bExit = FALSE;	//�����߳��˳�
	DWORD			dNow = GetTickCount();
	while (!bExit)
	{
		dwIoSize = -1;
		lpOverlapped = NULL;
		pClient = NULL;
		//�ȴ�I/O�������
		BOOL bIORet = GetQueuedCompletionStatus(hComPort,
			&dwIoSize,
			(LPDWORD) &pClient,
			&lpOverlapped,
			INFINITE);

		//ʧ�ܵĲ������			
		if ((FALSE == bIORet && NULL != pClient) || dwIoSize == 0)
		{		
			//��Ҫɾ������
			//printf("_ServiceThread ioret=%d iosize=%d--gtmiocp:%x--pClient:%x\n",bIORet,dwIoSize,pGTMIOCP,pClient);

			CClientManager *pClientMgr = CClientManager::GetClientManager();

			strLog.Format(_T("XXXServiceThread recv client=%x bok=%d,iosize=%d ioret=%d count:%d\n"),pClient,pClient->m_bOk,dwIoSize,bIORet,pClientMgr->GetClientCount());
			OutputDebugString(strLog);
			if (pClient != NULL && pClientMgr->FindClientAndLock(pClient))
			{				
				int iErr = GetLastError();
				//�ͻ��˶Ͽ�,�ص��ϲ�����ϲ㸺��delete pClient				
				//strLog.Format(_T("Client break off. Delete from ClientMap. SOCKET=%d. Err=%d. dwIoSize=%d. ThreadID=%d"),
				//	pClient->m_s, iErr, dwIoSize, GetCurrentThreadId());
				//WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
				//printf(strLog);
				pClient->m_bOk = FALSE;
				pClient->SetCloseCB(STATUS_REMOTECLOSED);	
				pClientMgr->UnlockClient(pClient);
				pClientMgr->DeleteClient(pClient,TRUE);
			}
			pClient = NULL;
		}
		//�ɹ��Ĳ������
		if(bIORet && lpOverlapped && pClient) 
		{				
			//��ȡ��չ�ص��ṹָ��
			PIO_OPERATION_DATA pIO = CONTAINING_RECORD(lpOverlapped, IO_OPERATION_DATA,overlapped);

			CClientManager *pClientMgr = CClientManager::GetClientManager();
			BOOL bRtn = pClientMgr->FindClientAndLock(pClient);
			//pClientMgr->UnlockClient(pClient);

			if (bRtn)
			{
				pClientMgr->ProcessIO(pIO->type, pClient, dwIoSize);	
				pClientMgr->UnlockClient(pClient);
			}
			else
			{
				strLog.Format(_T("Client(%d) have been deleted when receiving. ThreadID=%d"),
					pClient, GetCurrentThreadId());
				WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

				pClient = NULL;
			}
		}	
		//�������˳�������������Ͷ��pClient == NULL��lpOverlapped == NULL������
		if(pClient == NULL&& lpOverlapped == NULL && !pGTMIOCP->m_bRunning)
		{			
			strLog.Format(_T("Server shutdown. Exit thread(%d). GTMIOCP::_ServiceThread"), GetCurrentThreadId());
			WriteLog(SERVERLOGNAME, logLevelWarring, strLog);

			bExit = TRUE;
		}		
	}
	return 0;
}

int GPS_Iocp::StartWork( char* ulIP, unsigned short usPort,RECV_CALLBACKFUNC pFunc,LPVOID pUserData )
{
	printf("\nstartwork--port=%d ip=%s\n",usPort,ulIP);
	m_pRecvCallBackFunc = NULL;
	m_pRecvCBUserData = NULL;
	int nRet = -1;
	nRet = InitializeSocket();
	if(nRet!=TRUE)
		return FALSE;
	nRet = Listen(ulIP,usPort);
	if(nRet ==0)
	{
		m_pRecvCallBackFunc = pFunc;
		m_pRecvCBUserData	= pUserData;
		return TRUE;
	}
	return FALSE;
}

int GPS_Iocp::SendData(BYTE *pOldClient, BYTE *pClient,const BYTE*pBuf,DWORD dwBufLen )
{
	CClientManager *pClientMgr = CClientManager::GetClientManager();
	if(pOldClient!=NULL &&pOldClient !=pClient)
	{
		//ɾ��OldClient 
		CClientContext *pOldClientContext = (CClientContext *) pOldClient;
		//pClientMgr->DeleteClient(pOldClientContext,TRUE);

	}
	//����ɾ��OldClient
	CClientContext *pClientContext = (CClientContext *) pClient;
	BOOL bRtn = pClientMgr->FindClientAndLock(pClientContext);
	int nRet = 1;
	if(bRtn)
	{
		nRet = pClientContext->SendData(pBuf,dwBufLen);
		pClientMgr->UnlockClient(pClientContext);
	}
	return (nRet==0)?1:(-nRet);
}
