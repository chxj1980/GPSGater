// TSIPDevice.h : TSIPDevice DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTSIPDeviceApp
// �йش���ʵ�ֵ���Ϣ������� TSIPDevice.cpp
//

class CTSIPDeviceApp : public CWinApp
{
public:
	CTSIPDeviceApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
