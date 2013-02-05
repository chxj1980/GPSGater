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
			break;
		}
	case IORead_GPS_Body:
		{
			pClient->OnRecvDataCompleted(dwIOSize);
			break;
		}
	default:break;
	}		
}

BOOL CClientManager::FindClientAndLock(CClientContext *pClient)
{
	CSingleLock slock(&m_csClientMapLock, TRUE);

	ClientMap::iterator iter;
	_ClientData ClientData;
	
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