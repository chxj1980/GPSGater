// XMLParser.h : XMLParser DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXMLParserApp
// �йش���ʵ�ֵ���Ϣ������� XMLParser.cpp
//

class CXMLParserApp : public CWinApp
{
public:
	CXMLParserApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
