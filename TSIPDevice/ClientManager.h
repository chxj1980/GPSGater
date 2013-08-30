#if !defined(AFX_CLIENTMANAGER_H__688E1208_4C99_4411_938C_D8B547910E87__INCLUDED_)
#define AFX_CLIENTMANAGER_H__688E1208_4C99_4411_938C_D8B547910E87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "ClientContext.h"
using namespace std;

typedef struct tagThreadData
{
	SOCKET			socket;
	SOCKADDR_IN		addrClient;	
	CClientContext* pClient;
} _ClientData;

typedef map<DWORD, _ClientData, less<DWORD>, allocator<_ClientData> > ClientMap;


//����ģʽ
class CClientManager  
{
public:
	virtual ~CClientManager();
public:
	static CClientManager *GetClientManager(void);	//�õ�����ͻ��˶���ָ��
	static void		ReleaseManager(void);				//�ͷŹ���ͻ��˶���ռ����Դ
	void			ProcessIO(byte type, CClientContext* pClient, DWORD dwIOSize);	//I/O����
	void			AddClient(_ClientData clientdata);		//���ӿͻ���
	void			DeleteClient(CClientContext *pClient, BOOL bDelete);	//ɾ���ͻ���
	void			DeleteAllClient(void);					//ɾ�����пͻ���
	BOOL			FindClientAndLock(CClientContext *pClient);
	void			LockClient(CClientContext *pClient);
	void			UnlockClient(CClientContext *pClient);
	DWORD			GetClientCount(){return m_dwClientCount;}
	__int64			GetAllClientCount(){return m_i64dAllClientCount;}
	DWORD			DetectLiveTime();
private:
	ClientMap		m_clientMap;							//����ͻ���ӳ�䣬��Ӧһ��Ψһ��CommID
	CClientManager();
	CClientManager(const CClientManager& other);
	CClientManager& operator = (CClientManager &other);
	static CClientManager *m_pClientMgr;
	CCriticalSection m_csClientMapLock;							//�����ͻ����������
	DWORD			m_dwClientCount;
	__int64			m_i64dAllClientCount;
};

#endif // !defined(AFX_CLIENTMANAGER_H__688E1208_4C99_4411_938C_D8B547910E87__INCLUDED_)
