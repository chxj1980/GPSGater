// MG_Tools.h : MG_Tools DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMG_ToolsApp
// �йش���ʵ�ֵ���Ϣ������� MG_Tools.cpp
//

class CMG_ToolsApp : public CWinApp
{
public:
	CMG_ToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
