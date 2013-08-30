#include "stdafx.h"
#include "ClientManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CClientManager* CClientManager::m_pClientMgr = NULL;

CClientManager::CClientManager()
{
	m_pClientMgr = NULL; 
	m_dwClientCount = 0;
	m_i64dAllClientCount = 0;
}

CClientManager::~CClientManager()
{
	if (!m_clientMap.empty())
	{
		DeleteAllClient();
	}	
}
/* 
 * �õ��ͻ��˹������ָ��
 */
CClientManager * CClientManager::GetClientManager( void )
{
	return (m_pClientMgr == NULL)? m_pClientMgr = new CClientManager:m_pClientMgr;
}
/* 
 * ���ӿͻ���
 */
void CClientManager::AddClient( _ClientData clientdata )
{	
	CSingleLock slock(&m_csClientMapLock, TRUE);
	m_clientMap.insert(ClientMap::value_type((DWORD)clientdata.socket, clientdata));
	clientdata.pClient->OnUpdateRecvTime();
	m_dwClientCount = m_clientMap.size();
	m_i64dAllClientCount++;
	//m_dwClientCount++;
	//printf("\naddClient--%d\n",m_dwClientCount);
}

/* 
 * ɾ���ͻ���
 */
void CClientManager::DeleteClient( CClientContext *pClient, BOOL bDelete )
{
	CSingleLock slock(&m_csClientMapLock, TRUE);

	ClientMap::iterator iter;
	_ClientData ClientData;

	// walk through the events and threads and close them all
	for(iter = m_clientMap.begin(); iter != m_clientMap.end(); iter++)
	{
		ClientData = (*iter).second;
		if (ClientData.pClient == pClient)
		{
			if (bDelete)
			{
				delete pClient;
				ClientData.pClient = NULL;
			}	
			m_clientMap.erase(iter);
			break;
		}		
 	}
	m_dwClientCount = m_clientMap.size();
}

/* 
 * ɾ�����пͻ���
 */
void CClientManager::DeleteAllClient( void )
{
	CSingleLock slock(&m_csClientMapLock, TRUE);	
	//ɾ�����пͻ�������
// 	ClientMap::iterator iter;
// 	_ClientData ClientData;
// 
// 	for(iter = m_clientMap.begin(); iter != m_clientMap.end(); iter++)
// 	{
// 		ClientData = (*iter).second;		
// 		delete ClientData.pClient;
// 		ClientData.pClient = NULL;				
// 	}	

	//��ɾ�������������������Ӷ�����ʹ�����Լ��ͷ�
	m_clientMap.clear();
	m_dwClientCount = m_clientMap.size();
}

/* 
 * �ͷ��ڴ�
 */
void CClientManager::ReleaseManager( void )
{
	if (NULL != m_pClientMgr)
	{
		delete m_pClientMgr;
		m_pClientMgr = NULL;
	}	
}

/* 
 * I/O����
 */
void CClientManager::ProcessIO( byte type, CClientContext* pClient, DWORD dwIOSize )
{
	switch(type)//�ص���������
	{
	case IOReadHead:
		{
	//		pClient->OnRecvHeadCompleted(dwIOSize);
			break;
		}
	case IOReadBody://������
		{
		//	pClient->OnRecvBodyCompleted(dwIOSize);
			break;				
		}
	case IOWrite:
		{
			pClient->OnSendCompleted(dwIOSize);
			pClient->OnUpdateRecvTime();
			break;
		}
	case IORead_GPS_Body:
		{
			pClient->OnRecvDataCompleted(dwIOSize);
			pClient->OnUpdateRecvTime();
			break;
		}
	default:break;
	}		
}
DWORD CClientManager::DetectLiveTime(){

	__time64_t dNowTime = GetTickCount();

	CSingleLock slock(&m_csClientMapLock, TRUE);

	ClientMap::iterator iter;
	_ClientData ClientData;

	BOOL bDelete = TRUE;
	DWORD dwDeleteCount = 0;
	for(iter = m_clientMap.begin(); iter != m_clientMap.end(); )
	{
		ClientData = (*iter).second;
		DWORD dC =dNowTime -ClientData.pClient->GetUpdateRecvTime();
		BOOL bHadDelete=false;
		if(dC>1000*60*3)
		{
			//printf("now[%I64d]-last[%I64d]=dc[%d] max 1000*60*3.client=%x\n",dNowTime,ClientData.pClient->GetUpdateRecvTime(),dC,ClientData.pClient);

			if (bDelete)
			{
				closesocket((*iter).first);
				//ClientData.pClient->ShutDownSocket();
				/*
				delete ClientData.pClient;
				ClientData.pClient = NULL;
				iter = m_clientMap.erase(iter);
				bHadDelete=TRUE;
				/**/
			}	
			dwDeleteCount++;
		}
		if(!bHadDelete)
			++iter;
	}
	m_dwClientCount = m_clientMap.size();
	return dwDeleteCount;
}
BOOL CClientManager::FindClientAndLock(CClientContext *pClient)
{
	CSingleLock slock(&m_csClientMapLock, TRUE);

	ClientMap::iterator iter;
	_ClientData ClientData;

	//printf("FindClientAndLock--%d\n",m_clientMap.size());
	for(iter = m_clientMap.begin(); iter != m_clientMap.end(); iter++)
	{
		ClientData = (*iter).second;
		if (ClientData.pClient == pClient)
		{
			if(!pClient->m_bOk) return FALSE;
			LockClient(pClient);
			return TRUE;
		}		
	}
	return FALSE;
}

void CClientManager::LockClient(CClientContext *pClient)
{
// 	CString strLog;
// 	strLog.Format(_T("CClientManager::LockClient %x"),pClient);
// 	OutputDebugString(strLog);
	pClient->LockClient();
}

void CClientManager::UnlockClient(CClientContext *pClient)
{
// 	CString strLog;
// 	strLog.Format(_T("CClientManager::UnlockClient %x"),pClient);
// 	OutputDebugString(strLog);
	pClient->UnlockClient();
}