// XMLParser.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "XMLParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CXMLParserApp

BEGIN_MESSAGE_MAP(CXMLParserApp, CWinApp)
END_MESSAGE_MAP()


// CXMLParserApp ����

CXMLParserApp::CXMLParserApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CXMLParserApp ����

CXMLParserApp theApp;


// CXMLParserApp ��ʼ��

BOOL CXMLParserApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#include "I_XMLParser.h"
#include "XMLParser3.h"
#include "tinyXMLParser.h"
BOOL I_XMLParser::CreateInstance(I_XMLParser **ppIXMLParser)
{
	if(ppIXMLParser==NULL)
		return FALSE;
	//*ppIXMLParser = (I_XMLParser *)new CXMLParser3();
	*ppIXMLParser = (I_XMLParser *)new CTinyXMLParser();
	if(NULL == *ppIXMLParser)return FALSE;
	return TRUE;
}
