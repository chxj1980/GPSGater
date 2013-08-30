#include "stdafx.h"
#include "ClientContext.h"
#include "ClientManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientContext::CClientContext(DWORD dID,SOCKET s, GPS_Iocp* pGTMIOCP, CMemPool* pMemPool)
:m_s(s),
m_dwID(dID),
m_pGTMIOCP(pGTMIOCP),
m_dwPacketSize(0),
m_dwReceivedSize(0),
m_pRecvBuffer(NULL),
m_pRecvCBFunc(NULL),
m_pRecvCBUserData(NULL)
{
	m_pMemPool = pMemPool;
	m_pRecvBufferCell = NULL;	

	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);

	m_bOk = TRUE;
	m_i64RecvCnt = 0;
}

CClientContext::~CClientContext()
{
	////�����������������׼������ʱ���з������ݲ������ȴ�
	CSingleLock singleLock(&m_SendLock, TRUE);
	
	closesocket(m_s);
	m_pGTMIOCP = NULL;

	//if(m_pRecvBufferCell != NULL)
	//{
	//	m_pMemPool->ReleaseBuffer(m_pRecvBufferCell,1);
	//	m_pRecvBufferCell = NULL;
	//}	

	UnlockClient();



}
/*
* 
*/
int CClientContext::SyncSendData( BYTE* pBuf, DWORD dwSize )
{
	CString strLog;
	DWORD	flags = 0;			//��־
	DWORD	sendBytes =0;		//�����ֽ���
	MEM_POOL_BUFFERCELL* pSendBufferCell = NULL;
	//ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	//m_oIO.type = IOWrite;	//��������

	if (m_pMemPool != NULL)
	{
		int iRetry = 0;
		while(1)
		{
			if ((pSendBufferCell = m_pMemPool->GetEmptyBuffer(dwSize + HEADSIZE, 2)) != NULL)
			{
				break;
			}
			else
			{
				strLog.Format(_T("pSendBufferCell==NULL! Retry. iRetry=%d"), iRetry);
				WriteLog(SERVERLOGNAME,logLevelWarring,strLog);
				Sleep(40);				
			}
			if (++iRetry > 50)
			{
				strLog.Format(_T("m_pMemPool->GetEmptyBuffer(%d) timeout when send! iRetry=%d"), dwSize + HEADSIZE, iRetry);
				WriteLog(SERVERLOGNAME, logLevelWarring,strLog);
				return 2000;//2S��ûȡ����ʧ��
			}
		}
	}

	char* pDataBuffer = NULL;
	if (pSendBufferCell->pExtBuffer == NULL)
	{
		pDataBuffer = pSendBufferCell->pBuffer;
	}
	else
	{
		pDataBuffer = pSendBufferCell->pExtBuffer;
	}

	_Packet_Head head;
	head.dwTcpHead = TCP_HEAD;
	head.dwPacketSize = dwSize;

	memcpy(pDataBuffer, &head, HEADSIZE);
	memcpy(pDataBuffer + HEADSIZE, pBuf, dwSize);

	//	m_oIO.databuf.buf = m_pSendBuffer;
	//	m_oIO.databuf.len = dwSize + HEADSIZE;

	//��������
	// 	if (WSASend(m_s, 
	// 		&m_oIO.databuf,
	// 		1, 
	// 		&sendBytes, 
	// 		flags,
	// 		&m_oIO.overlapped,
	// 		NULL) == SOCKET_ERROR)
	// 	{
	// 		if(ERROR_IO_PENDING != WSAGetLastError())//�ɹ������ص�����
	// 		{
	// 			return WSAGetLastError();
	// 		}
	// 	}	
	// 
	// 	if(WaitForSingleObject(m_hSendEvent, INFINITE) != WAIT_OBJECT_0)
	// 	{
	// 		AfxMessageBox(_T("####"));
	// 	}


	FD_SET writefd;	//��д����
	timeval timeout; 
	FD_ZERO(&writefd);		//����
	FD_SET(m_s, &writefd);//��ӵ���д����

	timeout.tv_sec = 10;	//10s��connect��ʱ 
	timeout.tv_usec = 0; 
	int reVal = 0;
	reVal = select(0, NULL, &writefd, NULL, &timeout);//�ȴ��׽�����������
	if (SOCKET_ERROR == reVal)
	{				
		strLog.Format(_T("select() error = %d in GTMNetworkClient::_WriterThread()"), WSAGetLastError());
		WriteLog(SERVERLOGNAME, logLevelWarring,strLog);
		m_pMemPool->ReleaseBuffer(pSendBufferCell,2);
		return WSAGetLastError();
	}
	else if ( reVal > 0)
	{
		if (FD_ISSET(m_s, &writefd))			//�����д������
		{
			int iRet = -1;			
			iRet = send(m_s, pDataBuffer, dwSize+HEADSIZE, 0);
			if (iRet == SOCKET_ERROR)
			{
				//			m_iSendErrCode = WSAGetLastError();

				strLog.Format(_T("Send() error = %d in GTMNetworkClient::_WriterThread()"), WSAGetLastError());
				WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
				m_pMemPool->ReleaseBuffer(pSendBufferCell,2);
				return WSAGetLastError();
			}			
		}		
	}
	else	//select��ʱ
	{	
		m_pMemPool->ReleaseBuffer(pSendBufferCell,2);
		WriteLog(SERVERLOGNAME, logLevelWarring, _T("select timeout in GTMNetworkClient::_WriterThread()"));
		return WSAGetLastError();
	}	

	if(!m_pMemPool->ReleaseBuffer(pSendBufferCell,2))
	{
		strLog.Format(_T("m_pMemPool->ReleaseBuffer failed. Socket=%d"), m_s);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
		return 1111;
	}
	strLog.Format(_T("Server have sended a msg(%d).Socket=%d. SyncSendData"),dwSize,m_s);
	//WriteLog(SERVERLOGNAME, _T("SyncSendData"), strLog);
	return 0;
}

void CClientContext::OnRecvDataCompleted( DWORD dwIOSize )
{		
	CString strLog;
	ASSERT(m_cRecvDataBuffer != NULL);

	m_dwReceivedSize += dwIOSize;
	m_i64RecvCnt++;
	strLog.Format(_T("GTM:[1]m_dwPacketSize=%d,m_dwReceivedSize=%d,dwIOSize=%d,"),m_dwPacketSize,m_dwReceivedSize,dwIOSize);
	//OutputDebugString(strLog);
	//printf("recv[%x]_%I64d,iosize=%d---total=%d\r\n",this,m_i64RecvCnt,dwIOSize,m_dwReceivedSize);

	char * pData = m_iIO.databuf.buf;
	pData[dwIOSize] = '\0';
	//strLog.Format()
// 	if (m_dwReceivedSize < m_dwPacketSize)
// 	{				
// 		AsyncRecvBody(m_dwPacketSize - m_dwReceivedSize);
// 	}
// 	else if (m_dwReceivedSize == m_dwPacketSize)
	{			
		strLog.Format(_T("GTM:[2]m_dwPacketSize=%d,m_dwReceivedSize=%d,dwIOSize=%d"),m_dwPacketSize,m_dwReceivedSize,dwIOSize);
		//OutputDebugString(strLog);

		strLog.Format(_T("Server have received a msg(%d).Socket=%d. OnRecvBodyCompleted"),m_dwPacketSize,m_s);
		//WriteLog(SERVERLOGNAME, _T("OnRecvBodyCompleted"), strLog);

		if (m_pRecvCBFunc != NULL)
		{
			m_pRecvCBFunc(STATUS_OK,LPBYTE(this), LPBYTE(pData), dwIOSize, m_pRecvCBUserData);
		}
		else
		{
			WriteLog(SERVERLOGNAME, logLevelWarring, _T("m_pRecvCBFunc == NULL when OnRecvBodyCompleted!"));
		}

		//����Ҳ��m_CallBackLock��������ֹ��������ͬʱ�ͷ��ڴ��
// 		if(!m_pMemPool->ReleaseBuffer(m_pRecvBufferCell,1))
// 		{
// 			strLog.Format(_T("m_pMemPool->ReleaseBuffer failed. Socket=%d"), m_s);
// 			WriteLog(SERVERLOGNAME,logLevelWarring, strLog);
// 		}
// 		m_pRecvBufferCell = NULL;

		AsyncRecvData();
	}
// 	else
// 	{
// 		strLog.Format(_T("GTM:[3]m_dwPacketSize=%d,m_dwReceivedSize=%d,dwIOSize=%d"),m_dwPacketSize,m_dwReceivedSize,dwIOSize);
// 		//OutputDebugString(strLog);
// 	}
}

/*
* ����������ϴ���
*/
void CClientContext::OnSendCompleted( DWORD dwIOSize )
{

}

NetworkType CClientContext::GetType()
{
	return NetworkType_TCP;
}

int CClientContext::Connect(unsigned long ulIP, unsigned short usPort)
{
	return 0;
}

BOOL CClientContext::SetReceiveCallBack(RECV_CALLBACKFUNC pFunc, LPVOID pUserData)
{
	m_pRecvCBFunc = pFunc;
	m_pRecvCBUserData = pUserData;
	return TRUE;
}

int CClientContext::Startwork()
{
	//AsyncRecvHead();	//���հ�ͷ
	AsyncRecvData();
	return 0;
}
void CClientContext::ShutDownSocket()
{
	if(m_s!=NULL){
		//shutdown(m_s);
		closesocket(m_s);
	}
}
int CClientContext::Stopwork()
{
	if (m_s != NULL)
	{
		CancelIo((HANDLE)m_s);	
	}


	//��ʹ����ɾ���˶���ʱ��֪ͨ��������ֻɾ����Ӧ�ڵ�
	CClientManager *pClientMgr = CClientManager::GetClientManager();
	pClientMgr->DeleteClient(this, FALSE);	


	// ɾ��������Դ
	CSingleLock singleLock(&m_SendLock, TRUE);
	CSingleLock singlelock(&m_RecvLock, TRUE);


	if (m_s != NULL)
	{
		closesocket(m_s);
		m_s = NULL;
	}
	m_pGTMIOCP = NULL;

	if(m_pRecvBufferCell != NULL)
	{
		m_pMemPool->ReleaseBuffer(m_pRecvBufferCell,1);
		m_pRecvBufferCell = NULL;
	}	
	return 0;
}

int CClientContext::SendData( const BYTE* pBuf, DWORD dwSize )
{	
	CSingleLock singleLock(&m_SendLock, TRUE);
	//return SyncSendData(pBuf, dwSize);
	return AsynSendData(pBuf, dwSize);
}

BOOL CClientContext::AsyncRecvData()
{	
	DWORD	flags = 0;		//��־
	DWORD	recvBytes = 0;	//�����ֽ���

	//	ZeroMemory(m_pRecvBuffer, m_dwRecvBufferLen);
	ZeroMemory(&m_iIO, OVERLAPPEDPLUSLEN);
	m_iIO.type = IORead_GPS_Body;//��������
	m_dwPacketSize = 0;
	m_dwReceivedSize = 0;

	m_iIO.databuf.buf = m_cRecvDataBuffer;	//���հ�ͷ
	m_iIO.databuf.len = GPS_DATA_SIZE;		//��ͷ����

	//��ȡ����
	if (WSARecv(m_s, 
		&m_iIO.databuf,
		1, 
		&recvBytes, 
		&flags,
		&m_iIO.overlapped,
		NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())
		{
			return FALSE;
		}
	}	
	return TRUE;	
}void CClientContext::SetCloseCB(DWORD dwCode)
{	
	if (dwCode == STATUS_REMOTECLOSED)
	{
		if (m_pRecvCBFunc != NULL)
		{
			m_pRecvCBFunc(STATUS_REMOTECLOSED, (LPBYTE)this,NULL, 0, m_pRecvCBUserData);		
		}
		else
		{
			WriteLog(SERVERLOGNAME, logLevelWarring, _T("m_pRecvCBFunc == NULL when SetCloseCB"));
		}
	}
}

int CClientContext::GetConnectionInfo(OUT unsigned long &ulLocalIP, OUT unsigned short &usLocalPort, OUT unsigned long &ulRemoteIP, OUT unsigned short &usRemotePort)
{
	ulLocalIP = m_ulLocalIP;
	usLocalPort = m_usLocalPort;
	ulRemoteIP = m_ulRemoteIP;
	usRemotePort = m_usRemotePort;
	return 0;
}

void CClientContext::SetConnectionInfo(unsigned long ulLocalIP, unsigned short usLocalPort, unsigned long ulRemoteIP, unsigned short usRemotePort)
{
	m_ulLocalIP = ulLocalIP;
	m_usLocalPort = usLocalPort;
	m_ulRemoteIP = ulRemoteIP;
	m_usRemotePort = usRemotePort;
}

void CClientContext::LockClient()
{
	m_RecvLock.Lock();
}

void CClientContext::UnlockClient()
{
	m_RecvLock.Unlock();
}

int CClientContext::AsynSendData( const BYTE* pBuf, DWORD dwSize )
{
	CString strLog;
	DWORD	flags = 0;			//��־
	DWORD	sendBytes =0;		//�����ֽ���
	MEM_POOL_BUFFERCELL* pSendBufferCell = NULL;
	ZeroMemory(&m_oIO, OVERLAPPEDPLUSLEN);
	m_oIO.type = IOWrite;	//��������

	if (m_pMemPool != NULL)
	{
		int iRetry = 0;
		while(1)
		{
			if ((pSendBufferCell = m_pMemPool->GetEmptyBuffer(dwSize + HEADSIZE, 2)) != NULL)
			{
				break;
			}
			else
			{
				strLog.Format(_T("pSendBufferCell==NULL! Retry. iRetry=%d"), iRetry);
				WriteLog(SERVERLOGNAME,logLevelWarring,strLog);
				Sleep(20);				
			}
			if (++iRetry > 50)
			{
				strLog.Format(_T("m_pMemPool->GetEmptyBuffer(%d) timeout when send! iRetry=%d"), dwSize + HEADSIZE, iRetry);
				WriteLog(SERVERLOGNAME,logLevelWarring,strLog);
				return 2000;//1S��ûȡ����ʧ��
			}
		}
	}

	char* pDataBuffer = NULL;
	if (pSendBufferCell->pExtBuffer == NULL)
	{
		pDataBuffer = pSendBufferCell->pBuffer;
	}
	else
	{
		pDataBuffer = pSendBufferCell->pExtBuffer;
	}

	_Packet_Head head;
	head.dwTcpHead = TCP_HEAD;
	head.dwPacketSize = dwSize;

	//memcpy(pDataBuffer, &head, HEADSIZE);
	//memcpy(pDataBuffer + HEADSIZE, pBuf, dwSize);
	memcpy(pDataBuffer,pBuf,dwSize);
	m_oIO.databuf.buf = pDataBuffer;
//	m_oIO.databuf.len = dwSize + HEADSIZE;
	m_oIO.databuf.len = dwSize ;

	//��������
	if (WSASend(m_s, 
		&m_oIO.databuf,
		1, 
		&sendBytes, 
		flags,
		&m_oIO.overlapped,
		NULL) == SOCKET_ERROR)
	{
		if(ERROR_IO_PENDING != WSAGetLastError())//�ɹ������ص�����
		{
			if(!m_pMemPool->ReleaseBuffer(pSendBufferCell,2))
			{
				strLog.Format(_T("m_pMemPool->ReleaseBuffer failed. Socket=%d"), m_s);
				WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
				return 1111;
			}
			return WSAGetLastError();
		}
	}	

	if(!m_pMemPool->ReleaseBuffer(pSendBufferCell,2))
	{
		strLog.Format(_T("m_pMemPool->ReleaseBuffer failed. Socket=%d"), m_s);
		WriteLog(SERVERLOGNAME, logLevelWarring, strLog);
		return 1111;
	}
	return 0;
}