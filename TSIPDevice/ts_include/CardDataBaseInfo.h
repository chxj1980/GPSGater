#pragma once


#pragma pack(push,1)
struct CardDataBaseInfo 
{
	char cFlag;
	LONGLONG llCounter;
	DWORD dwAllocVideoDataSize;
	DWORD dwVideoDataSize;
	int nVideoWidth;
	int nVideoHeight;
	int nVideoStandard;
	int nAllocAudioDataSize;//ָ����MAX_AUDIO_CHANNEL�е�һ��
	int nAudioDataSize;
	DWORD dwSampleCount;

	CardDataBaseInfo()
	{
		Reset();
	}

	void Reset()
	{
		ZeroMemory(this,sizeof(CardDataBaseInfo));
	}
};
#pragma pack(pop)

#pragma  pack(push,1)
struct TS_PACK_DATA
{
	 	int    nBufSize;     // �ڴ��С
	 	int    nBufUseSize;  // �ڴ�ʹ�ô�С 
	 	BYTE*  pBuf;         // ������ڴ�ָ�� 
};
#pragma pack(pop)