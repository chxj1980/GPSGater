// TSIPDevice.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "TSIPDevice.h"

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

// CTSIPDeviceApp

BEGIN_MESSAGE_MAP(CTSIPDeviceApp, CWinApp)
END_MESSAGE_MAP()


// CTSIPDeviceApp ����

CTSIPDeviceApp::CTSIPDeviceApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTSIPDeviceApp ����

CTSIPDeviceApp theApp;

#include "io.h"
void openCommandWindow(){
	int hCrt;
	FILE *hf;
	AllocConsole();

	hCrt = _open_osfhandle(
		(long)GetStdHandle(STD_OUTPUT_HANDLE),
		0x4000);
	hf = _fdopen(hCrt,"w");
	*stdout =*hf;
	int i = setvbuf(stdout,NULL,_IONBF,0);

}
// CTSIPDeviceApp ��ʼ��

BOOL CTSIPDeviceApp::InitInstance()
{
	CWinApp::InitInstance();
	openCommandWindow();
	return TRUE;
}
