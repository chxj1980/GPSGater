// IAudioBufDecoder.h: interface for the IAudioBufDecoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IAUDIOBUFDECODER_H__9E579847_0571_4C94_A608_D67ED30AD67B__INCLUDED_)
#define AFX_IAUDIOBUFDECODER_H__9E579847_0571_4C94_A608_D67ED30AD67B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SBT_AudioStructDefine.h"

#ifdef _XH_AUDIO_DEC_EXPORTS
#define XH_AUDIO_DEC_API __declspec(dllexport)
#else
#define XH_AUDIO_DEC_API __declspec(dllimport)
#endif

#define PURE = 0

class IAudioBufDecoder  
{
public:
	IAudioBufDecoder();
	virtual ~IAudioBufDecoder();

	//��ʼ��
	//����ֵ�� > 0    ----   success
	//         other  ----   fail
	virtual  int Initialize() PURE;

	//�õ�֡����Ϣ
	//[IN]pFrameBuf,֡buf����ʼ��ַ; dwFrameLenָ�����ݴ�С;pstFrameInfo����֡��Ϣ
	virtual  int GetFrameInfo(const BYTE* pFrameBuf, DWORD dwFrameLen, void* pstFrameInfo) PURE;
	
	
	//����һ֡
	//[IN]pFrameBuf , ֡buf����ʼ��ַ
	//[IN]dwFrameLen, ֡�Ĵ�С
	//[OUT]ppBufOut, ����󣬴��PCM���ݵ���ʼ��ַ
	//[OUT]dwBufOut, ����һ֡��PCM���ݵĴ�С
	//[OUT]pWavOut, �����PCM���ݵĸ�ʽ
	//����ֵ��  >1 -- success,      others -- fail 
	virtual  int DecodeFrame(const BYTE* pFrameBuf, DWORD dwFrameLen, BYTE** ppBufOut, DWORD& dwBufOut,
		                      WAVEFORMATEX** pWavOut = NULL) PURE;

	//����ʼ��
	virtual  int UnInitialize() PURE;
};

extern "C" int XH_AUDIO_DEC_API CreateAudioBufDecoder(IAudioBufDecoder ** ppDec,int nDecoderType);
extern "C" void XH_AUDIO_DEC_API DestroyAudioBufDecoder(IAudioBufDecoder * pDec);

#endif // !defined(AFX_IAUDIOBUFDECODER_H__9E579847_0571_4C94_A608_D67ED30AD67B__INCLUDED_)
