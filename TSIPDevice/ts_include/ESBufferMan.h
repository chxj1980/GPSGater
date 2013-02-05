   /***********************************
 Copyright?,2001��2005, SOBEY CO,.LTD
 File name: ESBufferMan.h
 Author: zyy
 Version: 1.0
 DateTime: 
 Description: 
 Others: 
 Class and Interface List: 
 History: 
 1 Date: 
   Author: 
   Modification:�޸� ClearAllDataItem����
 2 Date: 
   Author: 
   Modification:��AddItem2EmptyTail��ʵ�ʼӵ�����ͷ������ҳ�����
 3 Date:
   Author: 
   Modification:
   

  ˵���� ���ļ�ӳ��ķ�ʽʵ�������̹���ѭ������
***********************************/


#ifndef   __ESBufferMan_H__2005_0919_
#define   __ESBufferMan_H__2005_0919_


/************************************************************************/
/* ����item
   CESBufferMan ��   CESBufferManProcess ����������е�item��Ǽ̳д���                                                                     */
/************************************************************************/
// typedef struct tagESListItem
// {
// 	int    nBufSize;     // �ڴ��С
// 	int    nBufUseSize;  // �ڴ�ʹ�ô�С 
// 	BYTE*  pBuf;         // ������ڴ�ָ�� 
// 	
// 	
// 	tagESListItem::tagESListItem()
// 	{
// 		pBuf = NULL;
// 		nBufSize = 0;
// 		nBufUseSize = 0;
// 	}
// }ESListItem, *pESListItem;

//////////////////////////////////////////////////////////////////////////

//
template <class T> 
inline T SafeGet(T &val)
{
	LONG lRet =	InterlockedExchangeAdd((LPLONG)&val, 0L);
	return (T)lRet;
}

template <class T> 
inline T SafeSet(T &val, T val1)
{
	LONG lRet = InterlockedExchange((LPLONG)&val, (LONG)val1);
	return (T)lRet;
}

//////////////////////////////////////////////////////////////////////////


#include <list>
using namespace std;
/************************************************************************/
/* ������ѭ������                                                                     */
/************************************************************************/
template<class T>
class  CESBufferMan  
{
public:
	CESBufferMan();
	virtual ~CESBufferMan();
public:
	BOOL SetListCountAndSize(unsigned int nListCount,unsigned int nBufSize=0);  //nBufSize==0 ,��ʾ�������ڴ�buffer

	BOOL AddItem2EmptyHead(T *pItem);
	BOOL AddItem2DataHead(T *pItem);
	BOOL AddItem2DataTail(T* pItem);
	BOOL AddItem2EmptyTail(T* pItem);
	T*  FetchDataHeadItem();    //if false ,return NULL
	T*  FetchEmptyHeadItem();   //if false ,return NULL
	
	void  ClearAllDataItem();   //����������������������

	HANDLE GetAddItem2DataTailEvt();
	int  GetSizeOfAllList();	//��ȡ���е��� ����
	int  GetSizeOfDataList();   //��ȡ����������
	int  GetSizeOfEmptyList();  //��ȡ��������

	typename list<T>::iterator ManListBegin();  //point to the first Item of m_ManList
	typename list<T>::iterator ManListEnd();    //point just beyond the end of m_ManList
	typename list<T>::iterator DataListBegin() {	return m_DataList.begin(); }
	typename list<T>::iterator DataListEnd() { return m_DataList.end(); }
private:
	
#pragma  warning(push)
#pragma  warning(once: 4251)
	list<T>    m_ManList;
	list<T *>  m_DataList;
	list<T *>  m_EmptyList;
#pragma  warning(pop)
		
	CRITICAL_SECTION m_CriSectionDataList;
	CRITICAL_SECTION m_CriSectionEmptyList;

	HANDLE m_hAddItem2DataTailEvt;
protected:
	void ReleaseAllList();
};



#include <vector>
/************************************************************************/
/* ���̼�ѭ������                                                                     */
/************************************************************************/

template<class T>
class CESBufferManProcess  
{
#pragma pack(push,4)
	typedef struct tagShareMemoryHead
	{
		unsigned int nItemCount;         //����share memory�е�Item������
		unsigned int nShareMemorySize; 	 //����share memory�ڴ�ռ�ô�С
		int nWritePos;                   //��һ��д�����λ��
        int nReadPos;                    //��һ�ζ�������λ�� 
		unsigned int nIsFull;           //�������Ƿ�Ϊ��  0 ��ʾ��1��ʾ��
        unsigned int nIsEmpty;          //�������Ƿ�Ϊ��  0 ��ʾ��1��ʾ��
		LONG dwFirstFrm;               //����ڴ��д�ŵ���ʼ֡      Add
		LONG dwLastFrm;                //����ڴ��д�ŵĽ�β֡      Add
		LONG dwTaskID;                 //��¼�ڴ��д�ŵĵ�ǰ����ID  Add
		LONG dwClipID;                 //��¼�ڴ��д�ŵĵ�ǰ�ֶ�ID  Add
		tagShareMemoryHead()
		{
			dwClipID = -1;
			dwTaskID = -1;
			dwFirstFrm = -1;
			dwLastFrm = -1;
			nItemCount = 0;
			nShareMemorySize = 0;
			nWritePos = -1;
			nReadPos = -1;
			nIsFull  = 0;
			nIsEmpty = 1;
		}
	}SHAREMEMORYHEAD;
#pragma pack(pop)
public:
	CESBufferManProcess();
	virtual ~CESBufferManProcess();
public:
	//nBufSize==0 ,��ʾ�������ڴ�buffer;LPCTSTR lpName �ļ�ӳ����
	BOOL CreateShareMemory(unsigned int nBufSzieTaskInfo,unsigned int nItemCount,unsigned int nBufSizeV0,unsigned int nBufSizeV1,unsigned int nBufSizeA0,unsigned int nBufSizeA1,LPCTSTR lpSHMName,LPCTSTR lpAddDataEvtName); //server function
	BOOL OpenShareMemory(unsigned int nBufSzieTaskInfo,unsigned int nItemCount,unsigned int nBufSizeV0,unsigned int nBufSizeV1,unsigned int nBufSizeA0,unsigned int nBufSizeA1,LPCTSTR lpSHMName,LPCTSTR lpAddDataEvtName);//client function 
	void FlushForWrite(LONG FrmIndex_min ,LONG FrmIndex_Max ,LONG TaskID ,LONG ClipID);
	void Reset();   //�Թ����ڴ��е���������ʼλ�á�������0���˲�����remove������������
    BOOL CreateExtMemBuf(unsigned int nExtBufV0Size ,unsigned int nExtBufV1Size);
	T* GetEmptyItemForWrite();
	void FlushWrite();
	T* GetDataItemForRead();
	void FlushRead();
	HANDLE GetWriteDataEvt();
	BYTE * GetTaskInfoAddr();
public:
	//�����ӿ�2
	BOOL AddItem2EmptyHead(T *pItem);
	BOOL AddItem2DataHead(T *pItem);
	BOOL AddItem2DataTail(T* pItem);
	BOOL AddItem2EmptyTail(T* pItem);
	T*  FetchDataHeadItem();    //if false ,return NULL
	T*  FetchEmptyHeadItem();   //if false ,return NULL

	void  ClearAllDataItem();   //����������������������

	HANDLE GetAddItem2DataTailEvt();
private:
	int m_iWriteIndex;
	BOOL ReleaseShareMemory();
	int  FindEmptyItemForWrite(); //Ѱ������������ݵĿ�����û�п��return -1
	int  FindDataItemForRead();

	HANDLE m_hWriteDataEvt; //���������ʱ��������¼�
    HANDLE m_hMapFile;

    //share memeory ��ӳ��Ŀ�ĵ�ַ
	SHAREMEMORYHEAD * m_pShareMemoryHead; //share memeory head

/*	��ע��ADD  �����ڴ�ӳ�䵽�������̿ռ�ĵ�ַ����������ǲ�ͬ��(��Ȼ���Ƕ�ָ��
    ͬһ���ڴ�����)���������������vetor���������湲���ڴ�ӳ�䵽�����̵ĵ�ַ��Ϣ��
	�����ڱ���һ�����̸ı�ʱ�����ڱ������ڽ��и�λ                                         */
	vector<T*> m_vectorSHMItem;
	vector<BYTE*> m_vectorPBuf;
	vector<BYTE*> m_vectorPExtBuf;
	vector<HANDLE> m_vectorExtHandle;
	int  m_iItemCount;
	BYTE *m_pTaskInfo;//taskinfo �����ַ
};

//////////////////////////////////////////////////////////////////////////
// CESBufferMan


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template<class T>
CESBufferMan<T>::CESBufferMan()
{
    InitializeCriticalSection(&m_CriSectionDataList);
	InitializeCriticalSection(&m_CriSectionEmptyList);

	m_hAddItem2DataTailEvt = CreateEvent( NULL,FALSE,FALSE,NULL); 
}
template<class T>
CESBufferMan<T>::~CESBufferMan()
{
	ReleaseAllList();
	DeleteCriticalSection(&m_CriSectionDataList);
	DeleteCriticalSection(&m_CriSectionEmptyList);

	if (NULL!=m_hAddItem2DataTailEvt)
	{
		CloseHandle(m_hAddItem2DataTailEvt);
		m_hAddItem2DataTailEvt = NULL;
	}
}
/************************************************************************/
/* argument: int nListCount  ���ĳ���  ����>0
             int nBufSize   <=0,�������ڴ�buffer�ռ�;>0����nBufSize��С�Ŀռ�,������֮                                                         */
/************************************************************************/
template<class T>
BOOL CESBufferMan<T>::SetListCountAndSize(unsigned int nListCount,unsigned int nBufSize)
{  
	if (nListCount<=0)
	{
		return FALSE;
	}
	
	if(m_ManList.size()>0)
		ReleaseAllList();

	EnterCriticalSection(&m_CriSectionEmptyList);
	for(int n=0;n<nListCount;n++)
	{
		BYTE *pBuff = NULL;
		if (nBufSize>0)
		{
			pBuff = new BYTE[nBufSize];
			if(pBuff==NULL)
			{
				//MessageBox(NULL, "BufferManage �����ڴ�ʧ��.", NULL, MB_OK);
				LeaveCriticalSection(&m_CriSectionEmptyList);
				if(m_ManList.size()>0)
					ReleaseAllList();
				return FALSE;
			}
		}

		T item;
		item.pBuf = pBuff;
		item.nBufSize = nBufSize;
		item.nBufUseSize = 0;
		m_ManList.push_back(item);
		m_EmptyList.push_back( &(m_ManList.back()) );
	}

	LeaveCriticalSection(&m_CriSectionEmptyList);

	int nsize = m_ManList.size();
	int nsizeemptey = m_EmptyList.size();
	int nsizeData = m_DataList.size();
	return TRUE;
}
template<class T>
HANDLE CESBufferMan<T>::GetAddItem2DataTailEvt()
{
	return m_hAddItem2DataTailEvt;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
template<class T>
T*  CESBufferMan<T>::FetchEmptyHeadItem()
{
	T *pListItem = NULL;
    EnterCriticalSection(&m_CriSectionEmptyList);
	if(!m_EmptyList.empty())
	{
		pListItem = m_EmptyList.front();
		m_EmptyList.pop_front();
	}
    LeaveCriticalSection(&m_CriSectionEmptyList);
    return pListItem;
}

template<class T>
T* CESBufferMan<T>::FetchDataHeadItem()
{
	T* pListItem  = NULL;
	EnterCriticalSection(&m_CriSectionDataList);
	if(!m_DataList.empty())
	{
		pListItem = m_DataList.front();
		m_DataList.pop_front();
	}

    LeaveCriticalSection(&m_CriSectionDataList);
    return pListItem;
}
template<class T> 
BOOL CESBufferMan<T>::AddItem2DataTail(T* pItem)
{
	EnterCriticalSection(&m_CriSectionDataList);
	m_DataList.push_back(pItem);
	SetEvent(m_hAddItem2DataTailEvt); //signale event
    LeaveCriticalSection(&m_CriSectionDataList);
	return TRUE;
}
template<class T>
BOOL CESBufferMan<T>::AddItem2EmptyTail(T* pItem)
{
	EnterCriticalSection(&m_CriSectionEmptyList);
	m_EmptyList.push_back(pItem);
    LeaveCriticalSection(&m_CriSectionEmptyList);
	return TRUE;
}
template<class T>
BOOL CESBufferMan<T>::AddItem2DataHead(T *pItem)
{
	EnterCriticalSection(&m_CriSectionDataList);
	m_DataList.push_front(pItem);
    LeaveCriticalSection(&m_CriSectionDataList);
	return TRUE;
}
template<class T>
BOOL CESBufferMan<T>::AddItem2EmptyHead(T *pItem)
{
	EnterCriticalSection(&m_CriSectionEmptyList);
	m_EmptyList.push_front(pItem);
    LeaveCriticalSection(&m_CriSectionEmptyList);
	return TRUE;
}
template<class T>
void  CESBufferMan<T>::ClearAllDataItem()
{
	EnterCriticalSection(&m_CriSectionDataList);
	list<T *>  TempList(m_DataList);
	m_DataList.clear();
    LeaveCriticalSection(&m_CriSectionDataList);

    EnterCriticalSection(&m_CriSectionEmptyList);
/*	m_EmptyList.clear();
	list<ESListItem>::iterator iter;

	for(iter = m_ManList.begin(); iter != m_ManList.end(); iter++)
	{
		m_EmptyList.push_back(&(*iter));
	}*/
	m_EmptyList.merge(TempList);
    LeaveCriticalSection(&m_CriSectionEmptyList);
}



/************************************************************************/
/* description: �ͷ��������ͷ����������ڴ�                                                                */
/************************************************************************/
template<class T>
void CESBufferMan<T>::ReleaseAllList()
{
	EnterCriticalSection(&m_CriSectionEmptyList);
	EnterCriticalSection(&m_CriSectionDataList);
	m_EmptyList.clear();    
	m_DataList.clear();
	int nsize = m_ManList.size();
	int nsizeemptey = m_EmptyList.size();
	int nsizeData = m_DataList.size();
    while(!m_ManList.empty())
	{
		T* pItem = (T*)&m_ManList.front();
		if(NULL!=pItem->pBuf)
			delete pItem->pBuf;
		pItem->pBuf = NULL;
		m_ManList.pop_front();
	}	
    LeaveCriticalSection(&m_CriSectionEmptyList);
	LeaveCriticalSection(&m_CriSectionDataList);

}
template<class T>
int  CESBufferMan<T>::GetSizeOfAllList()
{
	return (int)m_ManList.size();
}

template<class T>
int  CESBufferMan<T>::GetSizeOfDataList()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriSectionDataList);
	nCount = (int)m_DataList.size();
    LeaveCriticalSection(&m_CriSectionDataList);
	return nCount;
}

template<class T>
int  CESBufferMan<T>::GetSizeOfEmptyList()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriSectionEmptyList);
    nCount = (int)m_EmptyList.size();
	LeaveCriticalSection(&m_CriSectionEmptyList);
	return nCount;
}
//point to the first Item of m_ManList
template<class T>
typename list<T>::iterator CESBufferMan<T>::ManListBegin()
{
	return m_ManList.begin();
}
//point just beyond the end of m_ManList

template<class T>
typename list<T>::iterator CESBufferMan<T>::ManListEnd()
{
	return m_ManList.end();
}



//////////////////////////////////////////////////////////////////////////
//CESBufferManProcess
template<class T>
CESBufferManProcess<T>::CESBufferManProcess()
{
	m_pShareMemoryHead = NULL;
	m_hMapFile = NULL;
	m_hWriteDataEvt = NULL;
}
template<class T>
CESBufferManProcess<T>::~CESBufferManProcess()
{
	ReleaseShareMemory();
}
/************************************************************************/
/* argument:
      unsigned  int nItemCount    ������
      unsigned  int nBufSize      ������Ҫ�������ٵ�buffer��С�����nBufSize==0 ,��ʾ�������ڴ�buffer
      LPCTSTR   lpSHMName         ���浽atom�б��е��ļ�ӳ����
      LPCTSTR   lpAddDataEvtName  ���浽atom�б��е��¼���
   description: ���ٵ�sharememery�д�����ݵĸ�ʽ 
               -----------------------------------------------------------------------------------------------
               || �ṹSHAREMEMORYHEAD �� nItemCount���ṹT  ��  nItemCount��buffer��ÿ��buffer��СΪnBufSize  ||
			   ------------------------------------------------------------------------------------------------                                                   */
/************************************************************************/
template<class T>
BOOL CESBufferManProcess<T>::CreateShareMemory(unsigned int nBufSzieTaskInfo,unsigned int nItemCount,unsigned int nBufSizeV0,unsigned int nBufSizeV1,unsigned int nBufSizeA0,unsigned int nBufSizeA1,LPCTSTR lpSHMName,LPCTSTR lpAddDataEvtName)
{
	unsigned int nSHMSize = sizeof(SHAREMEMORYHEAD)+nBufSzieTaskInfo+nItemCount*sizeof(T)+nItemCount*(nBufSizeV0+nBufSizeV1+nBufSizeA0+nBufSizeA1);
	m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		                               PAGE_READWRITE, 0,
		                               nSHMSize, lpSHMName);
	if (m_hMapFile==NULL) 
	{ 
		return false;
	}
	m_iItemCount = nItemCount;
	m_hWriteDataEvt = CreateEvent(NULL, FALSE, FALSE, lpAddDataEvtName);
	if (NULL==m_hWriteDataEvt)
	{
		return false;
	}
    //ӳ�䵽����
	m_pShareMemoryHead = (SHAREMEMORYHEAD*)MapViewOfFile(m_hMapFile, 
								FILE_MAP_WRITE | FILE_MAP_READ,
								0, 0,
								nSHMSize);
	if(m_pShareMemoryHead==NULL)
	{
		return false;
	}
	m_pTaskInfo = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD);
	BYTE* pItemBegin = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD)+nBufSzieTaskInfo;
	BYTE* pBufBegin  = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD)+nBufSzieTaskInfo+nItemCount*sizeof(T);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//���ӳ��������ڴ������Ƿ�����
		//���head ����
		if ((m_pShareMemoryHead->nItemCount!=nItemCount)||(m_pShareMemoryHead->nShareMemorySize!=nSHMSize))
		{
			return false;
		}
		//��share memory �еĸ���item����ӵ�������
		DWORD dwBufSize = nBufSizeV0+nBufSizeV1+nBufSizeA0+nBufSizeA1;
		for(int n=0;n<nItemCount;n++)
		{
			T* pItem =  (T*)(pItemBegin+n*sizeof(T));
			m_vectorSHMItem.push_back(pItem);
			if (dwBufSize>0)
			{
				m_vectorPBuf.push_back(pBufBegin+n*dwBufSize);
			}
			else
			{
				m_vectorPBuf.push_back(NULL);
			}
			m_vectorPExtBuf.push_back(NULL);
			m_vectorExtHandle.push_back(NULL);
		}	
		return true;
	}
	//��ʼ��share memory head
	m_pShareMemoryHead->nItemCount = nItemCount;
	m_pShareMemoryHead->nShareMemorySize = nSHMSize;
	m_pShareMemoryHead->nWritePos = -1;
    m_pShareMemoryHead->nReadPos  = -1;
    m_pShareMemoryHead->nIsFull   = 0;
    m_pShareMemoryHead->nIsEmpty  = 1;
	m_pShareMemoryHead->dwClipID = -1;
	m_pShareMemoryHead->dwFirstFrm = -1;
	m_pShareMemoryHead->dwLastFrm = -1;
	m_pShareMemoryHead->dwTaskID = -1;
    //��share memory �еĸ���item����ӵ�������
	//BYTE* pItemBegin = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD);
	//BYTE* pBufBegin  = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD)+nItemCount*sizeof(T);
	DWORD dwBufSize = nBufSizeV0+nBufSizeV1+nBufSizeA0+nBufSizeA1;
	for(int n=0;n<nItemCount;n++)
	{
		T* pItem =  (T*)(pItemBegin+n*sizeof(T));
		pItem->nBufSizeV0 =  nBufSizeV0;
		pItem->nBufSizeUsedV0 = 0;
		pItem->pBufV0 = NULL;
		pItem->nBufSizeV1 =  nBufSizeV1;
		pItem->nBufSizeUsedV1 = 0;
		pItem->pBufV1 = NULL;
		pItem->nBufSizeA0 =  nBufSizeA0;
		pItem->nBufSizeUsedA0 = 0;
		pItem->pBufA0 = NULL;
		pItem->nBufSizeA1 =  nBufSizeA1;
		pItem->nBufSizeUsedA1 = 0;
		pItem->pBufA1 = NULL;
		pItem->nExtBufV0Size = 0;
		pItem->nExtbufV0UseSize = 0;
		pItem->pExtBufV0 = NULL;
		pItem->nExtBufV1Size = 0;
		pItem->nExtBufV1UseSize = 0;
		pItem->pExtBufV1 = NULL;
		m_vectorSHMItem.push_back(pItem);
		if (dwBufSize>0)
		{
			m_vectorPBuf.push_back(pBufBegin+n*dwBufSize);
		}
		else
		{
			m_vectorPBuf.push_back(NULL);
		}
		m_vectorPExtBuf.push_back(NULL);
		m_vectorExtHandle.push_back(NULL);
	}
	return true;
}
/************************************************************************/
/* argument:
      unsigned  int nItemCount    ������
      unsigned  int nBufSize      ������Ҫ�������ٵ�buffer��С�����nBufSize==0 ,��ʾ�������ڴ�buffer
      LPCTSTR   lpSHMName         ���浽atom�б��е��ļ�ӳ����
      LPCTSTR   lpAddDataEvtName  ���浽atom�б��е��¼���
      !! ���еĲ��� ���������CreateShareMemory�еĲ�����ͬ
     
   description: ���ٵ�sharememery�д�����ݵĸ�ʽ 
               -----------------------------------------------------------------------------------------------
               || �ṹSHAREMEMORYHEAD �� nItemCount���ṹT  ��  nItemCount��buffer��ÿ��buffer��СΪnBufSize  ||
			   ------------------------------------------------------------------------------------------------                                                   */
/************************************************************************/
template<class T>
BOOL CESBufferManProcess<T>::OpenShareMemory(unsigned int nBufSzieTaskInfo,unsigned int nItemCount,unsigned int nBufSizeV0,unsigned int nBufSizeV1,unsigned int nBufSizeA0,unsigned int nBufSizeA1,LPCTSTR lpSHMName,LPCTSTR lpAddDataEvtName)
{
	m_hMapFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
									 FALSE,
									lpSHMName);
	if (m_hMapFile == NULL) 
	{ 
		return false;
	}
	m_iItemCount = nItemCount;
    m_hWriteDataEvt = OpenEvent(EVENT_MODIFY_STATE|SYNCHRONIZE, FALSE, lpAddDataEvtName);
    if (NULL==m_hWriteDataEvt)
	{
		return false;
    }
	//ӳ����ļ���С
    unsigned int nSHMSize = sizeof(SHAREMEMORYHEAD)+nBufSzieTaskInfo+nItemCount*sizeof(T)+nItemCount*(nBufSizeV0+nBufSizeV1+nBufSizeA0+nBufSizeA1);
	//ӳ�䵽����
	m_pShareMemoryHead = (SHAREMEMORYHEAD*)MapViewOfFile(m_hMapFile, 
								FILE_MAP_WRITE | FILE_MAP_READ,
								0, 0,
								nSHMSize);
	if(m_pShareMemoryHead==NULL)
	{
		return false;
	}

	//���ӳ��������ڴ������Ƿ�����
	//���head ����
	if ((m_pShareMemoryHead->nItemCount!=nItemCount)||(m_pShareMemoryHead->nShareMemorySize!=nSHMSize))
	{
		return false;
	}
	m_pTaskInfo = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD);
	//��share memory �еĸ���item����ӵ�������
	BYTE* pItemBegin = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD)+nBufSzieTaskInfo;
	BYTE* pBufBegin  = (BYTE*)m_pShareMemoryHead+sizeof(SHAREMEMORYHEAD)+nBufSzieTaskInfo+nItemCount*sizeof(T);
	DWORD dwBufSize = nBufSizeV0+nBufSizeV1+nBufSizeA0+nBufSizeA1;
	for(int n=0;n<nItemCount;n++)
	{
		T* pItem =  (T*)(pItemBegin+n*sizeof(T));
		m_vectorSHMItem.push_back(pItem);
		if (dwBufSize>0)
		{
			m_vectorPBuf.push_back(pBufBegin+n*dwBufSize);
		}
		else
		{
			m_vectorPBuf.push_back(NULL);
		}
		m_vectorPExtBuf.push_back(NULL);
		m_vectorExtHandle.push_back(NULL);
	}	
	return true;
}

template<class T>
BOOL CESBufferManProcess<T>::ReleaseShareMemory()
{
	if (m_pShareMemoryHead!=NULL)
	{
		if (!UnmapViewOfFile(m_pShareMemoryHead)) 
		{ 
			return false;
		}
        m_pShareMemoryHead = NULL;
	}
	if (NULL!=m_hMapFile)
	{
		CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
	}
	if (NULL!=m_hWriteDataEvt)
	{
		CloseHandle(m_hWriteDataEvt);
		m_hWriteDataEvt = NULL;
	}
	for (int n = 0 ;n < m_iItemCount ; n++) //�ͷ���չ�ڴ�   ADD
	{
		if (m_vectorPExtBuf[n] != NULL)
		{
			if (!UnmapViewOfFile(m_vectorPExtBuf[n]))
			{
				CString strLog;
				strLog.Format(_T("UnmapViewOfFile pExtBuf[%d] Failed") ,n);
				OutputDebugString(strLog);
				return FALSE;
			}
			m_vectorPExtBuf[n] = NULL;
		}

		if (m_vectorExtHandle[n] != NULL)
		{
			CloseHandle(m_vectorExtHandle[n]);
			m_vectorExtHandle[n] = NULL;
		}
	}
	return true;
}

template<class T>
void CESBufferManProcess<T>::Reset()
{
	SafeSet(m_pShareMemoryHead->nWritePos,(int)-1);
	SafeSet(m_pShareMemoryHead->nReadPos,(int)-1);
	SafeSet(m_pShareMemoryHead->nIsFull,(unsigned int)0);
	SafeSet(m_pShareMemoryHead->nIsEmpty,(unsigned int)1);
	SafeSet(m_pShareMemoryHead->dwClipID ,(LONG)-1);
	SafeSet(m_pShareMemoryHead->dwTaskID ,(LONG)-1);
	SafeSet(m_pShareMemoryHead->dwFirstFrm ,(LONG)-1);
	SafeSet(m_pShareMemoryHead->dwLastFrm ,(LONG)-1);

}

/************************************************************************/
/* FindEmptyItemForWrite() ����������ݵĽ��̵��ã����Ա�֤��m_pShareMemoryHead->nWritePos 
   �ǲ���ı�ġ�����m_pShareMemoryHead->nReadPos��ʱ�ڸı䣬
   �����ǰ�ȫ��                                                                 */
/************************************************************************/
template<class T>
int  CESBufferManProcess<T>::FindEmptyItemForWrite()
{
	unsigned int nIsFull = SafeGet(m_pShareMemoryHead->nIsFull);
    if (nIsFull==1)
	{
		return -1;
    }
	int nWritePos   = SafeGet(m_pShareMemoryHead->nWritePos);  
	unsigned int nIndex = (nWritePos+1)%m_pShareMemoryHead->nItemCount;
	return nIndex;
}
/************************************************************************/
/*FindDataItemForRead() ����������ݵĽ��̵��ã����Ա�֤��m_pShareMemoryHead->nDataBegin 
   �ǲ���ı�ġ�����m_pShareMemoryHead->nDataEnd��ʱ�ڸı䣬
   �����ǰ�ȫ��                                                                */
/************************************************************************/
template<class T>
int  CESBufferManProcess<T>::FindDataItemForRead()
{
    unsigned int nIsEmpty  = SafeGet(m_pShareMemoryHead->nIsEmpty);
	if (nIsEmpty==1)
	{
		return -1;
	}
    int nReadPos = SafeGet(m_pShareMemoryHead->nReadPos);	
	unsigned int nIndex = (nReadPos+1)%m_pShareMemoryHead->nItemCount;
	return nIndex;		

}

template<class T>
T* CESBufferManProcess<T>::GetEmptyItemForWrite()
{
	int nIndex = FindEmptyItemForWrite();
	if (nIndex<0)
	{
		return NULL;
	}
	m_iWriteIndex = nIndex;
	DWORD OffSet = 0;
	T* pEmptyItem = m_vectorSHMItem[nIndex];
    pEmptyItem->pBufV0 = m_vectorPBuf[nIndex];
	OffSet += pEmptyItem->nBufSizeV0;
	pEmptyItem->pBufV1 = m_vectorPBuf[nIndex]+OffSet;
	OffSet += pEmptyItem->nBufSizeV1;
	pEmptyItem->pBufA0 = m_vectorPBuf[nIndex]+OffSet;
	OffSet += pEmptyItem->nBufSizeA0;
    pEmptyItem->pBufA1 = m_vectorPBuf[nIndex]+OffSet;
	if (m_vectorPExtBuf[nIndex] != NULL)    //�ͷ���չBUF   ADD
	{
       UnmapViewOfFile(m_vectorPExtBuf[nIndex]);
	   m_vectorPExtBuf[nIndex] = NULL;
	   pEmptyItem->pExtBufV0 = NULL;
	   pEmptyItem->pExtBufV1 = NULL;
	   pEmptyItem->nExtBufV0Size = 0;
	   pEmptyItem->nExtbufV0UseSize = 0;
	   pEmptyItem->nExtBufV1Size = 0;
	   pEmptyItem->nExtBufV1UseSize = 0;
	}
	if (m_vectorExtHandle[nIndex] != NULL)
	{
		CloseHandle(m_vectorExtHandle[nIndex]);
		m_vectorExtHandle[nIndex] = NULL;
	}
	return pEmptyItem;
}

template<class T>
void CESBufferManProcess<T>::FlushWrite()
{
	int nIndex = FindEmptyItemForWrite();
	if (nIndex<0)
	{
		return;
	}
	int nReadPos = SafeGet(m_pShareMemoryHead->nReadPos);
	if((nIndex==nReadPos)||(nIndex-nReadPos==m_pShareMemoryHead->nItemCount))
	{
		SafeSet(m_pShareMemoryHead->nIsFull,(unsigned int)1);
	}
	SafeSet(m_pShareMemoryHead->nWritePos,nIndex);
	SafeSet(m_pShareMemoryHead->nIsEmpty,(unsigned int)0);
	SetEvent(m_hWriteDataEvt);
}


template<class T>
T* CESBufferManProcess<T>::GetDataItemForRead()
{
	int nIndex = FindDataItemForRead();
	if (nIndex<0)
	{
		return NULL;
	}
	DWORD OffSet = 0;
	T* pDataItem = m_vectorSHMItem[nIndex];
    pDataItem->pBufV0 = m_vectorPBuf[nIndex];
	OffSet += pDataItem->nBufSizeV0;
	pDataItem->pBufV1 = m_vectorPBuf[nIndex]+OffSet;
	OffSet += pDataItem->nBufSizeV1;
	pDataItem->pBufA0 = m_vectorPBuf[nIndex]+OffSet;
	OffSet += pDataItem->nBufSizeA0;
    pDataItem->pBufA1 = m_vectorPBuf[nIndex]+OffSet;
	if (pDataItem->nExtBufV0Size>0 || pDataItem->nExtBufV1Size>0)//�������ڶ�λ��չBUF��ַ   ADD
	{
		CString csName;
		csName.Format(_T("SOBEY_MSV_EXTBUF_NAME_%d") ,nIndex);
		m_vectorExtHandle[nIndex] = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE ,
			                             FALSE ,csName);
		if (m_vectorExtHandle[nIndex] == NULL)
		{
			CString strLog;
			strLog.Format(_T("Error: OpenFileMapping ExtBuf Failed. function: CESBufferManProcess<T>::GetDataItemForRead()"));
			OutputDebugString(strLog);
			return NULL;
		}
		m_vectorPExtBuf[nIndex] = (BYTE*)MapViewOfFile(m_vectorExtHandle[nIndex], 
			                       FILE_MAP_WRITE | FILE_MAP_READ,0, 0,
								   pDataItem->nExtBufV0Size+pDataItem->nExtBufV1Size);
		if (pDataItem->nExtBufV0Size > 0)
		{
		    pDataItem->pExtBufV0 = m_vectorPExtBuf[nIndex];
		}
		else
		{
			pDataItem->pExtBufV0 =  NULL;
		}
		if (pDataItem->nExtBufV1Size > 0)
		{
			pDataItem->pExtBufV1 = m_vectorPExtBuf[nIndex]+pDataItem->nExtBufV0Size;
		}
		else
		{
			pDataItem->pExtBufV1 =  NULL;
		}
	}
    return pDataItem;
}

template<class T>
void CESBufferManProcess<T>::FlushRead()
{
	int nIndex = FindDataItemForRead();
	if (nIndex<0)
	{
		return;
	}

	if (m_vectorPExtBuf[nIndex] != NULL)    //�ͷ���չBUF   ADD
	{
		UnmapViewOfFile(m_vectorPExtBuf[nIndex]);
		m_vectorPExtBuf[nIndex] = NULL;
	}
	if (m_vectorExtHandle[nIndex] != NULL)
	{
		CloseHandle(m_vectorExtHandle[nIndex]);
		m_vectorExtHandle[nIndex] = NULL;
	}

	int nWritePos = SafeGet(m_pShareMemoryHead->nWritePos);
    if (nWritePos == nIndex)
	{
		SafeSet(m_pShareMemoryHead->nIsEmpty,(unsigned int)1);
    }

	SafeSet(m_pShareMemoryHead->nReadPos,nIndex);
	SafeSet(m_pShareMemoryHead->nIsFull,(unsigned int)0);
}

template<class T>
void CESBufferManProcess<T>::FlushForWrite(LONG FrmIndex_min ,LONG FrmIndex_Max ,LONG TaskID ,LONG ClipID)
{
    SafeSet(m_pShareMemoryHead->dwClipID ,(LONG)ClipID);
	SafeSet(m_pShareMemoryHead->dwTaskID ,(LONG)TaskID);
	SafeSet(m_pShareMemoryHead->dwFirstFrm ,(LONG)FrmIndex_min);
	SafeSet(m_pShareMemoryHead->dwLastFrm ,(LONG)FrmIndex_Max);
}

//Ϊ�˱������ʹ�������ռ䣬V0,V1����չ�ڴ������һ����   ADD
template<class T>
BOOL CESBufferManProcess<T>::CreateExtMemBuf(unsigned int nExtBufV0Size ,unsigned int nExtBufV1Size)
{
	if (nExtBufV0Size+nExtBufV1Size <= 0)
	{
		return FALSE;
	}
    CString csName;
	csName.Format(_T("SOBEY_MSV_EXTBUF_NAME_%d") ,m_iWriteIndex);
	m_vectorExtHandle[m_iWriteIndex] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		                               PAGE_READWRITE, 0,
									   nExtBufV0Size+nExtBufV1Size, csName);
		                               
	if (m_vectorExtHandle[m_iWriteIndex] == NULL)
	{
		CString strLog;
		strLog.Format(_T("Error: CreateFileMapping ExtBuf Failed. function: CESBufferManProcess<T>::CreateExtMemBuf()"));
		OutputDebugString(strLog);
		return FALSE;
	}
	m_vectorPExtBuf[m_iWriteIndex] = (BYTE*)MapViewOfFile(m_vectorExtHandle[m_iWriteIndex], 
		                              FILE_MAP_WRITE | FILE_MAP_READ,0, 0,
							          nExtBufV0Size+nExtBufV1Size);
	T* pEmptyItem = m_vectorSHMItem[m_iWriteIndex];
	pEmptyItem->nExtBufV0Size = nExtBufV0Size;
	pEmptyItem->nExtBufV1Size = nExtBufV1Size;
	if (nExtBufV0Size > 0)  //V0ʹ����չ�ռ�
	{
       pEmptyItem->pExtBufV0 = m_vectorPExtBuf[m_iWriteIndex];
	}
	else
	{
       pEmptyItem->pExtBufV0 = NULL;
	}

	if (nExtBufV1Size > 0)  //V1ʹ����չ�ռ�
	{
		pEmptyItem->pExtBufV1 = m_vectorPExtBuf[m_iWriteIndex]+nExtBufV0Size;
	}
	else
	{
		pEmptyItem->pExtBufV1 = NULL;
	}
	return TRUE;
}


template<class T>
HANDLE CESBufferManProcess<T>::GetWriteDataEvt()
{
	return m_hWriteDataEvt;
}


template<class T>
BOOL CESBufferManProcess<T>::AddItem2EmptyHead(T *pItem)
{

}
template<class T>
BOOL CESBufferManProcess<T>::AddItem2DataHead(T *pItem)
{

}
template<class T>
BOOL CESBufferManProcess<T>::AddItem2DataTail(T* pItem)
{

}
template<class T>
BOOL CESBufferManProcess<T>::AddItem2EmptyTail(T* pItem)
{

}
template<class T>
T*  CESBufferManProcess<T>::FetchDataHeadItem()    //if false ,return NULL
{

}
template<class T>
T*  CESBufferManProcess<T>::FetchEmptyHeadItem()   //if false ,return NULL
{

}
template<class T>
void  CESBufferManProcess<T>::ClearAllDataItem()  //����������������������
{

}
template<class T>
HANDLE CESBufferManProcess<T>::GetAddItem2DataTailEvt()
{

}
template<class T>
BYTE * CESBufferManProcess<T>::GetTaskInfoAddr()
{
	return  m_pTaskInfo;
}
#endif
