#if !defined(AFX_CLIENTCONTEXT_H__DE6218FD_8A39_46C1_A9D2_EB6BC7082509__INCLUDED_)
#define AFX_CLIENTCONTEXT_H__DE6218FD_8A39_46C1_A9D2_EB6BC7082509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "gps_iocp.h"

//call back �ص�����״̬�붨�壨DWORD dwStatus)
#define		STATUS_OK				0XF0000001	//����
#define		STATUS_REMOTECLOSED		0XF0000010	//�Է��Ͽ���ر�
#define		STATUS_RECEIVEDATAERROR	0XF0000020	//���յ�����ʱû���ְ�ͷ���ص�����.

enum NetworkType
{
	NetworkType_TCP = 1000,
	NetworkType_WCF = 1001,
	NetworkType_UDP = 1002,
};
#define		DATALEN				1000000							//����������:1M
#define		OVERLAPPEDPLUSLEN	(sizeof(IO_OPERATION_DATA))			//��չ�ص��ṹ����

typedef void (CALLBACK *RECV_CALLBACKFUNC)(DWORD dwStatus/*״̬*/,BYTE *pClient, BYTE* pBuf, DWORD dwSize, LPVOID pUserData);
typedef struct _tag_Packet_Head
{
	DWORD dwTcpHead;                // eg: 0x00aa0155
	DWORD dwPacketSize;            //Size of Packet
}_Packet_Head;

//I/O��������
#define		IOReadHead			9							//����
#define		IOReadBody			10							//����
#define		IOWrite				11							//����
#define		IORead_GPS_Body		12

#define TCP_HEAD	0xF0a0B155
#define HEADSIZE	sizeof(_Packet_Head)
#define GPS_DATA_SIZE 1024

// I/O �������ݽṹ
typedef	struct _io_operation_data 
{
	OVERLAPPED	overlapped;					//�ص��ṹ
	//	char		recvBuf[DATALEN];			//�������ݻ�����
	WSABUF      databuf;					//WSABUF ���ݽṹ
	byte		type;						//��������
}IO_OPERATION_DATA, *PIO_OPERATION_DATA;


class CClientContext //: public INetworkConnector
{
public:
	CClientContext(SOCKET s, GPS_Iocp* pGTMIOCP, CMemPool* pMemPool);
	virtual ~CClientContext();
public:
	virtual NetworkType GetType();
	virtual	int			Connect(unsigned long ulIP, unsigned short usPort);
	virtual BOOL		SetReceiveCallBack(RECV_CALLBACKFUNC pFunc, LPVOID pUserData);
	virtual int SendData(const BYTE* pBuf, DWORD dwSize);
	virtual int			Startwork();	
	virtual	int			Stopwork();
	virtual int			GetConnectionInfo(OUT unsigned long &ulLocalIP,
										OUT unsigned short &usLocalPort,
										OUT unsigned long &ulRemoteIP,
										OUT unsigned short &usRemotePort);
	void				LockClient(void);
	void				UnlockClient(void);
public:
	int					SyncSendData(BYTE* pBuf, DWORD dwSize);//��������
	int AsynSendData(const BYTE* pBuf, DWORD dwSize);//��������
	void				OnSendCompleted(DWORD dwIOSize);		//�����������	

	BOOL				AsyncRecvData();
	void				OnRecvDataCompleted( DWORD dwIOSize );	//���հ������	

	void				SetCloseCB(DWORD dwCode);
	void				SetConnectionInfo(unsigned long ulLocalIP,
										unsigned short usLocalPort,
										unsigned long ulRemoteIP,
										unsigned short usRemotePort);


public:
	BOOL				m_bOk;
	SOCKET				m_s;				// �����̣߳��ͷ��̣߳������߳�ʹ�á���������Ҫ������ʹ��m_SendLock������
private:
	IO_OPERATION_DATA	m_iIO;			// ����չ�ص��ṹ���ݽṹ
	IO_OPERATION_DATA	m_oIO;			// ����չ�ص��ṹ���ݽṹ
	CCriticalSection	m_SendLock;		// ����������
	CCriticalSection	m_RecvLock;		// ����������
	unsigned long		m_ulLocalIP;
	unsigned short		m_usLocalPort;
	unsigned long		m_ulRemoteIP;
	unsigned short		m_usRemotePort;


	GPS_Iocp*			m_pGTMIOCP;			// ������ָ�룬 ����Ҫ����
	char*				m_pRecvBuffer;		// ��ʱ�洢ָ�룬�����ǵ��̣߳����Բ���Ҫ����
	DWORD				m_dwPacketSize;		// �����ǵ��̣߳����Բ���Ҫ����

	DWORD				m_dwReceivedSize;	// ���̲߳���������Ҫ����
	CMemPool*			m_pMemPool;			// ���̲߳���������Ҫ����
	RECV_CALLBACKFUNC	m_pRecvCBFunc;		// �ص��̣߳��ͷ��̣߳������߳�ʹ�á���������Ҫ������ʹ��m_RecvLock������
	LPVOID				m_pRecvCBUserData;	// �ص��̣߳��ͷ��̣߳������߳�ʹ�á���������Ҫ������ʹ��m_RecvLock������
	MEM_POOL_BUFFERCELL*			m_pRecvBufferCell;	// ���������̣߳��ͷ��̣߳������߳�ʹ�á���������Ҫ������ʹ��m_RecvLock������
	char				m_cRecvHeader[HEADSIZE];
	char				m_cRecvDataBuffer[GPS_DATA_SIZE];
	__int64				m_i64RecvCnt;
};

#endif // !defined(AFX_CLIENTCONTEXT_H__DE6218FD_8A39_46C1_A9D2_EB6BC7082509__INCLUDED_)
