#pragma once
#include "MGCaptureDefine.h"
#include "GpsGateDatadef.h"
#ifdef _MG_BUFFER_LISTEX_EXPORT_DEFINE_
#define MG_BUFFER_LISTEX_EXPORT __declspec(dllexport)
#else
#define MG_BUFFER_LISTEX_EXPORT __declspec(dllimport)
#pragma comment(lib,"MGBufferListExU.lib")
#endif
//��ǰ��tagListItemInfoEx����¼ʹ�õģ����������ģ��Ҫ�Լ����ƣ������´�ListItemInfoBase�������������͸�BufferListEx
typedef struct tagListItemInfoEx : public ListItemInfoBase
{
	int		nFlag;			//1������ʼ��Я���������  0��һ��������  2:������� 
	DWORD	dwFieldIndex;
	SOCKETINFO	curSocketInfo;

	int  nBufSizeV0;
	int  nBufSizeV1;
	int  nBufSizeA0;
	int  nBufSizeA1;


	tagListItemInfoEx()
	{
		dwFieldIndex =0;
		pBufV0 = NULL;
		pBufA0 = NULL;
		pBufV1 = NULL;
		pBufA1 = NULL;

		nBufSizeUsedV0 = 0;
		nBufSizeUsedV1 = 0;
		nBufSizeUsedA0 = 0;
		nBufSizeUsedA1 = 0;
		memset(&curSocketInfo,0,sizeof(curSocketInfo));
	}
}LISTITEMINFOEX,*pLISTITEMINFOEX;

template<class T> class BufferPoolListEx;
class MG_BUFFER_LISTEX_EXPORT MGBufferListEx
{
public:
	MGBufferListEx();
	virtual ~MGBufferListEx();
public:
	BOOL	Initialize(DWORD nTotalSize, DWORD nUnitSize, DWORD nBLLength);
	HANDLE	GetAddItem2DataTailEvt();

	BOOL	AddItem2EmptyTail(IN LISTITEMINFOEX *pItem);
	BOOL	AddItem2EmptyHead(IN LISTITEMINFOEX *pItem);
	BOOL	AddItem2DataHead(IN LISTITEMINFOEX *pItem, BOOL bCheckNoneUse = FALSE);
	BOOL	AddItem2DataTail(IN LISTITEMINFOEX *pItem, BOOL bCheckNoneUse = FALSE);	

	LISTITEMINFOEX*		FetchDataHeadItem(); 
	LISTITEMINFOEX*		FetchEmptyHeadItem(	DWORD dwDesireSizeV0,											
											DWORD dwDesireSizeA0,
											DWORD dwDesireSizeV1,
											DWORD dwDesireSizeA1); 

	void	ClearAllDataItem();	

	int		GetSizeOfAllList();	//��ȡ���е��� ����
	int		GetSizeOfEmptyList();  //��ȡ��������
	int		GetSizeOfDataList();   //��ȡ����������
	void	GetBufferEmptySize(OUT DWORD &dwEmptySize, OUT DWORD &dwAllSize);	//��ȡ�ڴ�ؿ��д�С���ܴ�С

	static void InitListItemInfo(LISTITEMINFOEX* pItemDes,LISTITEMINFOEX* pItemSrc);
private:
	BufferPoolListEx<LISTITEMINFOEX> *m_pBufferPoolListEx;
};