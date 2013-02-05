#pragma once
#include <afxtempl.h>

/*
#ifdef _UNICODE
	#ifdef _DEBUG
		#pragma comment(lib, "ReadConfigValueDLLUD.lib")
	#else
		#pragma comment(lib, "ReadConfigValueDLLU.lib")
	#endif
#else
	#ifdef _DEBUG
		#pragma comment(lib, "ReadConfigValueDLLD.lib")
	#else
		#pragma comment(lib, "ReadConfigValueDLL.lib")
	#endif
#endif
*/
#pragma comment(lib, "ReadConfigValueDLLU.lib")

//////////////////////////////////////////////////////////////////////////
//���ò�����ȡ������

/*
����˵��:���������ļ�
����˵��:
strConfigPathname:�����ļ�����
����ֵ:
0 	ʧ��
1 	�ɹ�
*/
AFX_EXT_API BOOL	LoadConfigParam(LPCWSTR strConfigPathname);

AFX_EXT_API BOOL	LoadConfigParamA(char* strConfigPathname);


/*
����˵��:��ȡֵΪ�ַ�����������
����˵��:
lpszSectionName	: Section������
lpszItemName	: Item ������
lpszDefault		: �ַ�Ĭ��ֵ
bAutoCreate		: �Ƿ��Զ�����������Ч.	

����ֵ:	�����ַ�������ֵ	

*/
AFX_EXT_API LPCWSTR ReadConfigParamValue(const LPCWSTR lpszSectionName,const LPCWSTR lpszItemName,const LPCWSTR lpszDefault,const LPCWSTR lpszFileName);


AFX_EXT_API char* ReadConfigParamValueA(const char* lpszSectionName,const char* lpszItemName,const char* lpszDefault,const char* lpszFileName);


/*
����˵��:��ȡֵΪ���ֵ�������
����˵��:
lpszSectionName	: Section������
lpszItemName	: Item ������
lpszDefault		: ����Ĭ��ֵ
bAutoCreate		: �Ƿ��Զ�����������Ч.	
����ֵ:	������������ֵ		
*/
AFX_EXT_API int		ReadConfigParamValue(const LPCWSTR lpszSectionName,const LPCWSTR lpszItemName,const int nDefault,const LPCWSTR lpszFileName);

AFX_EXT_API int		ReadConfigParamValueA(const char* lpszSectionName,const char* lpszItemName,const int nDefault,const char* lpszFileName);


/*
����˵��:дֵΪ�ַ�����������
����˵��:
lpszSectionName	: Section������
lpszItemName	: Item ������
lpszDefault		: Ĭ��ֵ
bWrite2File		: �Ƿ�д���ļ���һ�㲻��	
����ֵ:	0 	ʧ��
1 	�ɹ�
*/
AFX_EXT_API BOOL	WriteConfigParamValue(const LPCWSTR lpszSectionName,const LPCWSTR lpszItemName,const LPCWSTR lpszItemValue,const LPCWSTR lpszFileName);


AFX_EXT_API BOOL	WriteConfigParamValueA(const char* lpszSectionName,const char* lpszItemName,const char* lpszItemValue, const char* lpszFileName);


/*
����˵��:дֵΪ���ֵ�������
����˵��:
lpszSectionName	: Section������
lpszItemName	: Item ������
lpszDefault		: Ĭ��ֵ
bAutoCreate		: �Ƿ�д���ļ���һ�㲻��		
����ֵ:	0 	ʧ��
1 	�ɹ�
*/
AFX_EXT_API BOOL	WriteConfigParamValue(const LPCWSTR lpszSectionName,const LPCWSTR lpszItemName,const int nItemValue,const LPCWSTR lpszFileName);

AFX_EXT_API BOOL	WriteConfigParamValueA(const char* lpszSectionName,const char* lpszItemName,const int nItemValue,const char* lpszFileName);


/*
����˵��:����������ļ�
����˵��:
����ֵ:	0 	ʧ��
1 	�ɹ�
*/
AFX_EXT_API BOOL	SaveConfigParam();
