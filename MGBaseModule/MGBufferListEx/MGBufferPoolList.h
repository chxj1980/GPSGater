#pragma once

#include "MGBufferListEx.h"
#include <list>
using namespace std;

template<class T>
class  BufferPoolListEx
{
public:
	BufferPoolListEx();
	virtual ~BufferPoolListEx();
public:
	BOOL	Initialize(DWORD nTotalSize, DWORD nUnitSize, DWORD nBLLength);
	HANDLE	GetAddItem2DataTailEvt();

	BOOL	AddItem2EmptyTail(IN T *pItem);
	BOOL	AddItem2EmptyHead(IN T *pItem);
	BOOL	AddItem2DataHead(IN T *pItem, BOOL bCheckNoneUse = FALSE);
	BOOL	AddItem2DataTail(IN T *pItem, BOOL bCheckNoneUse = FALSE);	

	T*		FetchDataHeadItem(); 
	T*		FetchEmptyHeadItem(	DWORD dwDesireSizeV0 = 0,
								DWORD dwDesireSizeV1 = 0,
								DWORD dwDesireSizeA0 = 0,
								DWORD dwDesireSizeA1 = 0); 

	void	ClearAllDataItem();

	int		GetSizeOfAllList();
	int		GetSizeOfEmptyList();  //��ȡ��������
	int		GetSizeOfDataList();   //��ȡ����������
	void	GetBufferEmptySize(OUT DWORD &dwEmptySize, OUT DWORD &dwAllSize);	//��ȡ�ڴ�ؿ��д�С���ܴ�С
private:
	list<T*>			m_BufferNullList;	//������
	list<T*>			m_BufferDataList;	//��������

	CCriticalSection	m_NullLock;   //������
	CCriticalSection	m_DataLock;   //��������

	HANDLE				m_hAddItem2DataTailEvt;//���������������֮�󣬴������¼�		

	LPBYTE				m_pStructBuffer;
	int					m_nStructAllLen;
private:
	void				DealWithFetchEmptyHeadFailed(T *pListItem);

	//�ڴ������
private:
	BOOL		InitializeMem(DWORD nTotalSize, DWORD nUnitSize);
	BOOL		GetEmptyBuffer(BYTE **pBuffer, DWORD& nUnitNum, DWORD& nStartPos,DWORD dwBufSize);
	//BOOL		ReleaseBuffer(BUFFERCELL *pBuffer);	
	BOOL		ReleaseBuffer(BYTE **pBuffer, DWORD& dwSize, DWORD& nUnitNum, DWORD& nStartPos);
	BOOL		CheckNoneUseBuffer(IN T *pItem);
	BOOL		PushBackNoneUseBuffer(DWORD& dwUnitNum, int& dwBufferSize,DWORD dwSize, DWORD dwBufPos);
private:
	LPBYTE				m_pDataBuffer;
	DWORD				*m_pUnitUsed;//0:δʹ��		1������ʹ��
	DWORD				m_dwUnitSum;
	DWORD				m_dwUnitSize;
	DWORD				m_dwTotalSize;
	DWORD				m_dwFreePos;
	DWORD				m_dwAllFreeSize; //�ڴ�ص�ǰ���õĴ�С
private:
	UINT				m_unStructBufSize;//ÿ���ṹռ�õ�mem��С	
	LPBYTE				m_pMemBuffer;
	UINT				m_nItemCount;
	CCriticalSection	m_OperationLock;	
};

template<class T>
BufferPoolListEx<T>::BufferPoolListEx()
:m_hAddItem2DataTailEvt(INVALID_HANDLE_VALUE)
,m_pStructBuffer(NULL)
{	
	m_hAddItem2DataTailEvt = CreateEvent(NULL,FALSE,FALSE,NULL);//�����Զ��¼�	

	m_pDataBuffer = NULL;
	m_pMemBuffer = NULL;
	m_pUnitUsed = NULL;
	m_dwUnitSum = 0;
	m_dwUnitSize = 1024;
	m_dwFreePos = 0;
	m_dwTotalSize = 0;
}

template<class T>
BufferPoolListEx<T>::~BufferPoolListEx()
{
	if (m_hAddItem2DataTailEvt != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hAddItem2DataTailEvt);
		m_hAddItem2DataTailEvt = INVALID_HANDLE_VALUE;
	}

	if (m_pStructBuffer != NULL)
	{
		delete []m_pStructBuffer;
		m_pStructBuffer = NULL;
	}	

	if (m_pUnitUsed != NULL)
	{
		delete []m_pUnitUsed;
		m_pUnitUsed = NULL;
	}
	if (m_pMemBuffer != NULL)
	{
		delete []m_pMemBuffer;
		m_pMemBuffer = NULL;
	}
}

template<class T>
BOOL BufferPoolListEx<T>::Initialize(DWORD nTotalSize, DWORD nUnitSize, DWORD nBLLength)
{	
	if (m_pStructBuffer == NULL)
	{		
		m_pStructBuffer = new BYTE[sizeof(T) * nBLLength];
		ZeroMemory(m_pStructBuffer, sizeof(T) * nBLLength);
	}

	LPBYTE pSturct = m_pStructBuffer;
	for (DWORD y = 0; y < nBLLength; y++)
	{
		T *pData = (T*)pSturct;

		pSturct += sizeof(T);
		m_BufferNullList.push_back(pData);		
	}

	m_nStructAllLen = nBLLength;
	return InitializeMem(nTotalSize, nUnitSize);
}

template<class T>
BOOL BufferPoolListEx<T>::AddItem2DataHead(IN T *pItem, BOOL bCheckNoneUse)
{	
	if (bCheckNoneUse)
	{
		CheckNoneUseBuffer(pItem);
	}

	{
		CSingleLock lock(&m_DataLock, TRUE);
		m_BufferDataList.push_front(pItem);	
	}
	
	SetEvent(m_hAddItem2DataTailEvt);

	return TRUE;
}

template<class T>
BOOL BufferPoolListEx<T>::AddItem2DataTail(IN T *pItem, BOOL bCheckNoneUse)
{
	if (bCheckNoneUse)
	{
		CheckNoneUseBuffer(pItem);
	}

	{
		CSingleLock lock(&m_DataLock, TRUE);
		m_BufferDataList.push_back(pItem);
	}
	
	SetEvent(m_hAddItem2DataTailEvt);	

	return TRUE;
}

template<class T>
BOOL BufferPoolListEx<T>::AddItem2EmptyHead(IN T *pTempItem)
{
	//�ͷŻ�õ��ڴ�
	ReleaseBuffer(&pTempItem->pBufV0,pTempItem->nBufSizeUsedV0,pTempItem->stBufInfo[0].dwUnitNum, pTempItem->stBufInfo[0].dwBufferPos);
	ReleaseBuffer(&pTempItem->pBufA0,pTempItem->nBufSizeUsedA0,pTempItem->stBufInfo[1].dwUnitNum, pTempItem->stBufInfo[1].dwBufferPos);
	ReleaseBuffer(&pTempItem->pBufV1,pTempItem->nBufSizeUsedV1,pTempItem->stBufInfo[2].dwUnitNum, pTempItem->stBufInfo[2].dwBufferPos);
	ReleaseBuffer(&pTempItem->pBufA1,pTempItem->nBufSizeUsedA1,pTempItem->stBufInfo[3].dwUnitNum, pTempItem->stBufInfo[3].dwBufferPos);
	
	pTempItem->nBufSizeV0 = 0;
	pTempItem->nBufSizeV1 = 0;
	pTempItem->nBufSizeA0 = 0;
	pTempItem->nBufSizeA1 = 0;
	pTempItem->nFlag = -1;
	CSingleLock lock(&m_NullLock, TRUE);
	m_BufferNullList.push_front(pTempItem);	

	return TRUE;
}

template<class T>
BOOL BufferPoolListEx<T>::AddItem2EmptyTail(IN T *pTempItem)
{
	//�ͷŻ�õ��ڴ�
	ReleaseBuffer(&pTempItem->pBufV0,pTempItem->nBufSizeUsedV0,pTempItem->stBufInfo[0].dwUnitNum, pTempItem->stBufInfo[0].dwBufferPos);
	ReleaseBuffer(&pTempItem->pBufA0,pTempItem->nBufSizeUsedA0,pTempItem->stBufInfo[1].dwUnitNum, pTempItem->stBufInfo[1].dwBufferPos);
	ReleaseBuffer(&pTempItem->pBufV1,pTempItem->nBufSizeUsedV1,pTempItem->stBufInfo[2].dwUnitNum, pTempItem->stBufInfo[2].dwBufferPos);
	ReleaseBuffer(&pTempItem->pBufA1,pTempItem->nBufSizeUsedA1,pTempItem->stBufInfo[3].dwUnitNum, pTempItem->stBufInfo[3].dwBufferPos);
	
	pTempItem->nBufSizeV0 = 0;
	pTempItem->nBufSizeV1 = 0;
	pTempItem->nBufSizeA0 = 0;
	pTempItem->nBufSizeA1 = 0;
	pTempItem->nFlag = -1;
	CSingleLock lock(&m_NullLock, TRUE);
	m_BufferNullList.push_back(pTempItem);	

	return TRUE;
}

template<class T>
T* BufferPoolListEx<T>::FetchEmptyHeadItem(	DWORD dwDesireSizeV0,
											DWORD dwDesireSizeV1,
											DWORD dwDesireSizeA0,
											DWORD dwDesireSizeA1)
{	
	T *pTempListItem = NULL;
	{
		CSingleLock lock(&m_NullLock, TRUE);

		if(!m_BufferNullList.empty()) 
		{
			//����һ���������͵��ڴ�ظ�ֵ			
			pTempListItem = m_BufferNullList.front();
			m_BufferNullList.pop_front();				
		}
		else
		{
			return NULL;
		}
	}

	if(!GetEmptyBuffer(&pTempListItem->pBufA0, pTempListItem->stBufInfo[1].dwUnitNum, pTempListItem->stBufInfo[1].dwBufferPos,dwDesireSizeA0))
	{
		DealWithFetchEmptyHeadFailed(pTempListItem);
		return NULL;
	}
	pTempListItem->nBufSizeA0 = dwDesireSizeA0;

	if(!GetEmptyBuffer(&pTempListItem->pBufA1, pTempListItem->stBufInfo[3].dwUnitNum, pTempListItem->stBufInfo[3].dwBufferPos,dwDesireSizeA1))
	{
		DealWithFetchEmptyHeadFailed(pTempListItem);
		return NULL;
	}
	pTempListItem->nBufSizeA1 = dwDesireSizeA1;

	if(!GetEmptyBuffer(&pTempListItem->pBufV1, pTempListItem->stBufInfo[2].dwUnitNum, pTempListItem->stBufInfo[2].dwBufferPos,dwDesireSizeV1))
	{
		DealWithFetchEmptyHeadFailed(pTempListItem);
		return NULL;
	}
	pTempListItem->nBufSizeV1 = dwDesireSizeV1;

	if(!GetEmptyBuffer(&pTempListItem->pBufV0, pTempListItem->stBufInfo[0].dwUnitNum, pTempListItem->stBufInfo[0].dwBufferPos,dwDesireSizeV0))
	{
		DealWithFetchEmptyHeadFailed(pTempListItem);
		return NULL;
	}
	pTempListItem->nBufSizeV0 = dwDesireSizeV0;

	return pTempListItem;
}

template<class T>
T* BufferPoolListEx<T>::FetchDataHeadItem()
{
	T *pTempListItem = NULL;
	{
		CSingleLock lock(&m_DataLock, TRUE);
		if (!m_BufferDataList.empty())
		{
			//����һ��������		
			pTempListItem = m_BufferDataList.front();
			m_BufferDataList.pop_front();						
		}
		else
		{
			return NULL;
		}
	}	

	return pTempListItem;
}

template<class T>
void BufferPoolListEx<T>::ClearAllDataItem()
{
	CSingleLock lockData(&m_DataLock, TRUE);
	CSingleLock lockNull(&m_NullLock, TRUE);

	T *pTempItem = NULL;
	while(!m_BufferDataList.empty()) 
	{
		pTempItem = m_BufferDataList.front();
		m_BufferDataList.pop_front();
		//�ͷŻ�õ��ڴ�
		ReleaseBuffer(&pTempItem->pBufV0,pTempItem->nBufSizeUsedV0,pTempItem->stBufInfo[0].dwUnitNum, pTempItem->stBufInfo[0].dwBufferPos);
		ReleaseBuffer(&pTempItem->pBufA0,pTempItem->nBufSizeUsedA0,pTempItem->stBufInfo[1].dwUnitNum, pTempItem->stBufInfo[1].dwBufferPos);
		ReleaseBuffer(&pTempItem->pBufV1,pTempItem->nBufSizeUsedV1,pTempItem->stBufInfo[2].dwUnitNum, pTempItem->stBufInfo[2].dwBufferPos);
		ReleaseBuffer(&pTempItem->pBufA1,pTempItem->nBufSizeUsedA1,pTempItem->stBufInfo[3].dwUnitNum, pTempItem->stBufInfo[3].dwBufferPos);

		pTempItem->nBufSizeV0 = 0;
		pTempItem->nBufSizeV1 = 0;
		pTempItem->nBufSizeA0 = 0;
		pTempItem->nBufSizeA1 = 0;
		pTempItem->nFlag = -1;

		m_BufferNullList.push_back(pTempItem);
	}
}

template<class T>
void BufferPoolListEx<T>::DealWithFetchEmptyHeadFailed(T *pListItem)
{
	AddItem2EmptyTail(pListItem);
}

template<class T>
BOOL BufferPoolListEx<T>::InitializeMem(DWORD nTotalSize, DWORD nUnitSize)
{
	//�ȳ�ʼ��һ�����ڴ�	
	m_dwTotalSize = nTotalSize;
	m_dwAllFreeSize = m_dwTotalSize;

	m_pMemBuffer = new BYTE[m_dwTotalSize];
	if(m_pMemBuffer == NULL)
	{
		return FALSE;
	}
	ZeroMemory(m_pMemBuffer,m_dwTotalSize);

	//��һ�����ڴ�ָ��С�飬���������
	m_pDataBuffer = m_pMemBuffer;			
	m_dwUnitSize = nUnitSize;//ÿ��С��Ĵ�С
	if (nUnitSize == 0)
		return FALSE;

	m_dwUnitSum = m_dwTotalSize / m_dwUnitSize;
	m_pUnitUsed = new DWORD[m_dwUnitSum];
	memset(m_pUnitUsed, 0, sizeof(DWORD)*m_dwUnitSum);

	m_dwFreePos = 0;
	return TRUE;
}

template<class T>
BOOL BufferPoolListEx<T>::GetEmptyBuffer(BYTE **pBuffer, DWORD& nUnitNum, DWORD& nStartPos,DWORD dwBufSize)
{
	if (dwBufSize == 0)
	{
		return TRUE;
	}

	CSingleLock singlelock(&m_OperationLock,TRUE);	

	//��ʼȡ�������ݵ�ַ	
	BOOL bFind = FALSE;
	DWORD dwUnitNeed = dwBufSize / m_dwUnitSize;
	if (dwBufSize % m_dwUnitSize != 0)
		++dwUnitNeed;

	DWORD dwStart = m_dwFreePos;
	DWORD dwFree = 0;

	for (; dwStart < m_dwUnitSum; ++dwStart)
	{
		if (m_pUnitUsed[dwStart] == 0)
		{
			if (++dwFree >= dwUnitNeed)
			{
				bFind = TRUE;				
				break;
			}			
		}
		else
		{			
			dwFree = 0;
		}
	}

	if (!bFind)
	{
		dwFree = 0;
		//������ϴ�ʹ��λ��û�ҵ��㹻�ڴ棬�ʹ��ڴ��ͷ����ʼ�ҿ��е�
		for (dwStart = 0; dwStart < m_dwFreePos; ++dwStart)
		{
			if (m_pUnitUsed[dwStart] == 0)
			{
				if (++dwFree >= dwUnitNeed)
				{
					bFind = TRUE;
					break;
				}			
			}
			else
			{
				dwFree = 0;
			}
		}
	}

	if (bFind)
	{
		m_dwFreePos = dwStart+1;				//���¿���λ�ã���߲�ѯЧ��
		nStartPos	= dwStart - (dwUnitNeed-1);	//���ؿ�ʼλ�ã����ڻ���
		nUnitNum	= dwUnitNeed;				//�����ڴ浥Ԫ�������ڻ���		
		for (DWORD i=0; i<dwUnitNeed; ++i)
		{
			m_pUnitUsed[nStartPos + i] = 1;		//���ڴ��־
		}

		*pBuffer = m_pDataBuffer + nStartPos * m_dwUnitSize;	//����ָ��
		
		m_dwAllFreeSize -= dwUnitNeed * m_dwUnitSize;
	}
	else//�ұ������ڴ�أ�Ҳû�ҵ������ķ���Ҫ����ڴ棬ʧ��
	{		
		*pBuffer = NULL;
		nUnitNum = 0;
		nStartPos = 0;

		return FALSE;
	}

	return TRUE;
}

template<class T>
BOOL BufferPoolListEx<T>::ReleaseBuffer(BYTE **pBuffer, DWORD& dwSize, DWORD& nUnitNum, DWORD& nStartPos)
{
	if (nUnitNum == 0)
	{
		return TRUE;
	}
	CSingleLock singlelock(&m_OperationLock,TRUE);
	
	if (*pBuffer != NULL)
	{
		if (nStartPos < 0 || nStartPos >= m_dwUnitSum)
		{
			return FALSE;
		}
		if ((nStartPos + nUnitNum > m_dwUnitSum) || (nStartPos + nUnitNum < 0))
		{
			return FALSE;
		}
		for (DWORD i=0; i<nUnitNum; ++i)
		{
			m_pUnitUsed[nStartPos + i] = 0;//��λ��־
		}

		m_dwAllFreeSize += nUnitNum * m_dwUnitSize;

		*pBuffer = NULL;		
		nStartPos = 0;
		nUnitNum = 0;
		dwSize = 0;		
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

template<class T>
HANDLE BufferPoolListEx<T>::GetAddItem2DataTailEvt()
{
	return m_hAddItem2DataTailEvt;
}

template<class T>
int BufferPoolListEx<T>::GetSizeOfEmptyList()
{
	CSingleLock lock(&m_NullLock, TRUE);

	return m_BufferNullList.size();
}

template<class T>
int BufferPoolListEx<T>::GetSizeOfDataList()
{
	CSingleLock lock(&m_DataLock, TRUE);

	return m_BufferDataList.size();
}

template<class T>
void BufferPoolListEx<T>::GetBufferEmptySize(OUT DWORD &dwEmptySize, OUT DWORD &dwAllSize)
{
	CSingleLock lock(&m_OperationLock, TRUE);

	dwEmptySize = m_dwAllFreeSize;
	dwAllSize	= m_dwTotalSize;
}

template<class T>
int BufferPoolListEx<T>::GetSizeOfAllList()
{
	return m_nStructAllLen;
}

template<class T>
BOOL BufferPoolListEx<T>::CheckNoneUseBuffer(IN T *pItem)
{
	if (pItem->stBufInfo[0].dwUnitNum != 0)//V0
	{
		PushBackNoneUseBuffer(pItem->stBufInfo[0].dwUnitNum,pItem->nBufSizeV0, pItem->nBufSizeUsedV0,pItem->stBufInfo[0].dwBufferPos);
	}

	if (pItem->stBufInfo[1].dwUnitNum != 0)//A0
	{
		PushBackNoneUseBuffer(pItem->stBufInfo[1].dwUnitNum,pItem->nBufSizeA0, pItem->nBufSizeUsedA0,pItem->stBufInfo[1].dwBufferPos);
	}

	if (pItem->stBufInfo[2].dwUnitNum != 0)//V1
	{
		PushBackNoneUseBuffer(pItem->stBufInfo[2].dwUnitNum,pItem->nBufSizeV1, pItem->nBufSizeUsedV1,pItem->stBufInfo[2].dwBufferPos);
	}

	if (pItem->stBufInfo[3].dwUnitNum != 0)//A1
	{
		PushBackNoneUseBuffer(pItem->stBufInfo[3].dwUnitNum,pItem->nBufSizeA1, pItem->nBufSizeUsedA1,pItem->stBufInfo[3].dwBufferPos);
	}

	return TRUE;
}

template<class T>
BOOL BufferPoolListEx<T>::PushBackNoneUseBuffer(DWORD& dwUnitNum,int& dwBufferSize, DWORD dwSize, DWORD dwBufPos)
{
	DWORD dwUsedUnitLen = dwSize / m_dwUnitSize;
	if (dwSize % m_dwUnitSize != 0)
	{
		++dwUsedUnitLen;
	}

	//���ʵ��û���õ���ʱ�������ô�󣬾��ͷŶ����
	if (dwUsedUnitLen < dwUnitNum)
	{
		CSingleLock singlelock(&m_OperationLock,TRUE);
		
		for (DWORD i=0; i<(dwUnitNum - dwUsedUnitLen); ++i)
		{
			m_pUnitUsed[dwBufPos+dwUsedUnitLen + i] = 0;//��λ��־
		}
		
		m_dwAllFreeSize += (dwUnitNum - dwUsedUnitLen)*m_dwUnitSize;
		//���µ�ǰ�ڵ����Ϣ
		dwUnitNum = dwUsedUnitLen;
		dwBufferSize = dwUsedUnitLen*m_dwUnitSize;
	}

	return TRUE;
}