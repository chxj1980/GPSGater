#pragma once
#include "CMemPool.h"
typedef void (CALLBACK *RECV_CALLBACKFUNC)(DWORD dwStatus/*״̬*/,BYTE *pClient, BYTE* pBuf, DWORD dwSize, LPVOID pUserData);

class GPS_Iocp
{
public:
	GPS_Iocp(void);
	~GPS_Iocp(void);
	int InitializeSocket();
	int				Listen(char* ulIP, unsigned short usPort);
	int				StartWork(char* ulIP, unsigned short usPort,RECV_CALLBACKFUNC pFunc,LPVOID pUserData);
	int				SendData(BYTE *pOldClient,BYTE *pClient,const BYTE*pBuf,DWORD dwBufLen);
private:
	void					ClearResources(BOOL bCloseListenEvent);
	static	DWORD WINAPI	_AcceptThread(void *pParam);//���ܿͻ��������߳�
	static	DWORD WINAPI	_ServiceThread(void *pParam);//�����߳�
	static 	DWORD WINAPI	_DoDetectThread(void *pParam);
	HANDLE					m_hDoDetect;
private:
	RECV_CALLBACKFUNC		m_pRecvCallBackFunc;
	LPVOID				m_pRecvCBUserData;				// �ص��̣߳��ͷ��̣߳������߳�ʹ�á���������Ҫ������ʹ��m_RecvLock������

	SOCKET					m_sListen;					//�����׽���
	DWORD					m_dwMemPoolSize;			//�ڴ�ش�С
	BOOL					m_bRunning;
	HANDLE					m_hListenEvent;				//�����¼����
	HANDLE					m_hCompPort;				//��ɶ˿ھ��	
	HANDLE					m_hThread[35];				//���߳�=�����߳�+�����̣߳��ȹ������������һ��16�˵Ļ����ϣ����߳���=16*2+2+1���������
	int						m_nThreadNum;				//ʵ���߳�����
	CMemPool*				m_pMemPool;					//�ڴ��
	unsigned long			m_ulLocalIP;
	unsigned short			m_usLocalPort;
};
