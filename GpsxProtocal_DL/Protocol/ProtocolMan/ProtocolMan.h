// ProtocolMan.h : ProtocolMan DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProtocolManApp
// �йش���ʵ�ֵ���Ϣ������� ProtocolMan.cpp
//

class CProtocolManApp : public CWinApp
{
public:
	CProtocolManApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
