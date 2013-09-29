// GPSGateAdpter.h : GPSGateAdpter DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGPSGateAdpterApp
// �йش���ʵ�ֵ���Ϣ������� GPSGateAdpter.cpp
//
#include "GPS_Socket.h"
#include "DataOpter.h"
#include "GetCommand.h"
class CGPSGateAdpterApp : public CWinApp
{
public:
	CGPSGateAdpterApp();

// ��д
public:
	virtual BOOL InitInstance();
	void InitAdpter();

	DECLARE_MESSAGE_MAP()
private:
public:
	BOOL		  m_bInitAdpter;
	CGPS_Socket * m_pGPS_Socket;
	DataOpter   * m_pDataOpter;
	GetCommand	* m_pGetCommand;
};
