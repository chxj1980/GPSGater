#pragma once

#include "itsdevice.h"
#include "GPS_Iocp.h"
class Socket_IOCP:
	public ITsDevice_Card
{
public:
	Socket_IOCP(void);
	~Socket_IOCP(void);
	/*
	 *	��ʼ��Ӳ���豸
	 * @param nCardID  �忨�ţ���ϵͳ���豸���������е�����λ�ã���0��ʼ��
	 */
	virtual BOOL	InitDevice(int nListenPort) ;
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
	static void CALLBACK RecvCB(DWORD dwStatus/*״̬*/,BYTE *pClient, BYTE* pBuf, DWORD dwSize, LPVOID pUserData);

private:
	GPS_Iocp * m_pGPS_Iocp;

	MGBufferListEx				*m_pDataList;
};
