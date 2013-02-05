#ifndef _SOBEY_CARD_DATA_STRUCT_H_
#define _SOBEY_CARD_DATA_STRUCT_H_

#define AUDIO_MAX_CHANNEL		16
#define VIDEO_MAX_VBI_CONUT		10

typedef enum _tagVideoPortType		//��Ƶ�˿���������ö��
{
	videoPortNone = 0,
	videoComposite,
	videoYC,
	videoComponent,
	videoSDI,
	videoType0,
	videoType1,
	videoType2,
	videoType3,
}videoPortType;

typedef enum _tagAudioPortType		//��Ƶ�˿���������ö��
{
	audioPortNone = 0,
	audioAnalog,
	audioAesEbu,
	audioSdi,
	audioType0,
	audioType1,
	audioType2,
	audioType3,
}audioPortType;

typedef enum _tagVideoSyncMode		//ͬ��ģʽö��
{
	syncRef,
	syncInput,
	SyncFree,
	syncType0,
	syncType1,
	syncType2,
	syncType3,
}videoSyncMode;

typedef enum _tagVideoDataFormat	//��Ƶ���ݸ�ʽö��
{
	videoYUV	= 0,
	videoUYVY,
	videoYUYV,
	videoFormat0,
	videoFormat1,
	videoFormat2,
	videoFormat3,
}videoDataFormat;

typedef enum _tagVideoStandard		//������Ƶ��ʽö��
{
	VideoUnknow = 0,
	SD_PAL,
	SD_Ntsc30,
	SD_Ntsc29_97,
	SD_Secam,
	HD_1920_1080_48I,
	HD_1920_1080_47_95I,
	HD_1920_1080_50I,
	HD_1920_1080_60I,
	HD_1920_1080_59_94I,

	HD_1920_1080_24P,    //=10
	HD_1920_1080_23_97P,
	HD_1920_1080_25P,
	HD_1920_1080_30P,
	HD_1920_1080_29_97P,
	HD_1920_1080_50P,
	HD_1920_1080_60P,
	HD_1920_1080_59_94P,
	HD_1080_720_24P,
	HD_1080_720_23_97P, //19

	HD_1080_720_25P,	//20
	HD_1080_720_30P,
	HD_1080_720_29_97P,
	HD_1080_720_50P,
	HD_1080_720_60P,
	HD_1080_720_59_94P,
	videoStandard0,
	videoStandard1,
	videoStandard2,
	videoStandard3,
}videoStandard;

typedef enum _tagChannelStatus		//ͨ��״̬ö��
{
	ChannelNoInit = 0,
	ChannelInited,
	ChannelCapturing,
	ChannelCapturePause,
	ChannelPrepareTransfer,
	ChannelTransfering,
	ChannelTransferSeeking,
	ChannelTransferPause,
	ChannelStatus0,
	ChannelStatus1,
	ChannelStatus2,
	ChannelStatus3,
}channelStatus;

typedef enum _tagAudioSampleBits	//��Ƶ��������Bitö��
{
	audio32Bits,
	audio24Bits,
	audio20Bits,
	audio16Bits
}audioSampleBits;

typedef enum _tagAudioSampleFrequency	//��Ƶ������ö��
{
	audio48K = 0,
	audio44K1,
	audio32K,
	audioSample0,
	audioSample1,
	audioSample2, 
	audioSample3
}audioSampleFrequency;

typedef enum _tagChannelMode			//ͨ������ģʽö��
{
	vidAudMode= 0,
	dvbMode,
	workMode0,
	workMode1,
	workMode2,
	workMode3,
}channelMode;

typedef struct _tagVersionInfomation	//�汾���ƽṹ����
{
	_tagVersionInfomation()
	{
		description = "No description";
	};
	CString str;
	BYTE	nYear;
	BYTE	nMonth;
	BYTE	nDay;
	LPSTR	description;
	void*	pReserved;
}versionInfomation,*pVersionInfomation;

typedef struct _tagVideoPort			//��Ƶ�˿ڽṹ����
{
	_tagVideoPort()
	{
		eType	 = videoPortNone;
		nPortNum = 0;
		nUsed	 = 0;
		int i=0;
		for(i=0; i<6; i++)
			bSuportMode[i] = FALSE;
		for(i=0; i<30; i++)
			bSupportStandart[i] = FALSE;
		for(i=0; i<10; i++)
			bChannel[i] = FALSE;
		eCurMode	= vidAudMode;
		eFormat		= videoYUV;

		nSrc		= 0xFF;
		BindAudioPort = 0xFF;
		nGroupNumber = 0;
		description = "No description";
	}
	videoPortType	eType;				//��Ƶ�˿���������
	BOOL			bSuportMode[6];		//����channelMode����ģʽ��֧��
	BOOL			bSupportStandart[30];	//�˿ڶ�����Ƶ��ʽ��֧��
	int				nPortNum;			//�˿����

	BOOL			bChannel[10];		//��ǰ����ʹ�øö˿ڵ�ͨ��
	int				nUsed;				//��ǰ�Ƿ��ѱ�ʹ��
	channelMode		eCurMode;			//��ǰ����ģʽ
	videoDataFormat eFormat;			//��Ƶ���ݸ�ʽ

	int				nSrc;
	int				BindAudioPort;
	int				nGroupNumber;

	LPSTR			description;	//���ڴ˶˿ڵ��ַ�����
	void*			pReserved;			//����
}videoPort;

typedef struct _tagAudioPort			//��Ƶ�˿ڽṹ����
{
	_tagAudioPort()
	{
		eType			= audioPortNone;
		nPortNum		= 0;
		nMaxChannelCount= 0;
		bUsed			= FALSE;
		nSrc			= 0xFF;
		BindVideoPort	= 0xFF;
		nGroupNumber	= 0;
		description = "No description";
	}
	audioPortType	eType;				//��Ƶ�˿���������
	int				nPortNum;			//�˿����
	int				nMaxChannelCount;	//�˿�֧�ֵ������Ƶ������
	BOOL			bUsed;				//��ǰ�Ƿ��ѱ�ʹ��
	LPSTR			description;		//���ڴ˶˿ڵ��ַ�����

	int				nSrc;
	int				BindVideoPort;
	int				nGroupNumber;

	void*			pReserved;			//����
}audioPort;

typedef struct _tagSyncPort				//ͬ���˿�����
{
	_tagSyncPort()
	{
		eMode	= syncRef;
		nPortNum= 0;
		bUsed = FALSE;
		description = "No description";
	}
	videoSyncMode	eMode;				//ͬ��ģʽ
	int				nPortNum;			//�˿����
	BOOL			bUsed;				//��ǰ�Ƿ��ѱ�ʹ��
	LPSTR			description;		//���ڴ˶˿ڵ��ַ�����
	void*			pReserved;			//����
}syncPort;

typedef enum  _tagOutputSignalType
{
	signalPlay=0,			//��ͨ���Ļط��ź�
	signalInport,			//��Դ����˿ڵĻ�ͨ�ź�
	signalOtherOutChannel,	//��������ڵĻط��ź�
	signalTest,				//�����ź�
}outputSignalType;

typedef enum	_tagInterrutMode
{
	intFeild = 0,
	intFrame,
	interuptMode0,
	interuptMode1,
	interuptMode2,
	interuptMode3,
}interruptMode;

typedef struct _tagBoardCapability		//�忨������������
{
	_tagBoardCapability()
	{
		bDvbCapblity		= FALSE;
		vidCapPortNum		= 0;
		audCapPortNum		= 0;
		vidTransPortNum		= 0;
		audTransPortNum		= 0;
		syncPortNum			= 0;
		for(int i=0; i<30; i++)
			videoStandart[i] = FALSE;
		nMaxInChannelCount	= 0;
		nMaxOutChannelCount	= 0;
		bInOutSimul			= FALSE;
		description			= "No description";
	}
	BOOL		 bDvbCapblity;		 //�Ƿ�֧��DVB
	int			 nMaxInChannelCount; //���֧������ͨ����Ŀ
	int			 nMaxOutChannelCount;//���֧�����ͨ����Ŀ
	BOOL		 bInOutSimul;		 //�������ͬʱ����

	BOOL		 videoStandart[30];	//֧�ֵ���Ƶ�źŸ�ʽ
	int			 vidCapPortNum;		//��Ƶ����˿���Ŀ
	int			 audCapPortNum;		//��Ƶ����˿���Ŀ
	videoPort	 vidCapPort[100];	//��Ƶ����˿�
	audioPort	 audCapPort[100];	//��Ƶ����˿�

	int			 vidTransPortNum;	//��Ƶ����˿���Ŀ
	int			 audTransPortNum;	//��Ƶ����˿���Ŀ
	videoPort	 vidTransPort[100];	//��Ƶ����˿�
	audioPort	 audTransPort[100];	//��Ƶ����˿�

	int			 syncPortNum;		//ͬ���˿���Ŀ
	syncPort	 syncPort[100];		//ͬ������˿�

	DWORD		  Capblity0[100];	//����
	DWORD		  Capblity1[100];	//����
	LPSTR		  description;		//���������ַ�����
	void*		  pReserved;		//����
}boardCapability;

typedef struct _tagVBIInfo
{
	_tagVBIInfo()
	{
		nType = 0;
		nDID  = 0;
		nSDID = 0;
	}
	DWORD	nType;
	DWORD	nDID;
	DWORD	nSDID;
}vbiInfo;

typedef struct _tagVBIDataStruct
{
	_tagVBIDataStruct()
	{
		bActive = FALSE;
		pData	= NULL;
		nLen	= 0;
	}
	vbiInfo		info;
	LPBYTE		pData;
	DWORD		nLen;
	BOOL		bActive;
}vbiData;

typedef struct _tagInportInformation			//����ͨ����Ϣ����
{
	_tagInportInformation()
	{
		description = "No description";
	};
	videoStandard			eStd;				//������Ƶ��ʽ
	audioSampleBits			audBits;			//��Ƶ�����������
	audioSampleFrequency	eSample;			//��Ƶ������
	LPSTR					description;		//������Ϣ�ַ�����
	void*					pReserved;			//����
}inportInformation;

typedef struct _tagInputChannelParam			//����ͨ������
{
	_tagInputChannelParam()
	{
		eStd		= SD_PAL;
		nVbiNum		= 0;
		bEnableD10	= FALSE;
		description = "No description";
	}
	channelMode	  eMode;						//ͨ������ģʽ
	videoPort	  eVideoPort;					//������Ƶͨ��
	audioPort	  eAudioPort;					//������Ƶͨ��
	videoStandard eStd;							//(δ��)
	videoDataFormat eFormat;					//��Ƶ���ݸ�ʽ
	audioSampleBits audBits;					//��Ƶ�������
	BOOL		  bEnableD10;					//D10ʹ�ܣ�����SD�źţ���Ч��
	DWORD		  nCacheLen;					//�忨Chache���
	int			  nVbiNum;
	vbiInfo		  vbiParam[10];
	LPSTR		  description;					//����ͨ�������ַ�����
	void*		  pReserved;					//����
}inputChannelParam;

typedef struct _tagInterruptInfo					//���ж���Ϣ
{
	_tagInterruptInfo()
	{
		description = "No description";
	};
	LONGLONG	 nCounter;						//���жϼ���ֵ
	DWORD		 nCacheLen;						//��ǰ�忨����Cache���
	LPSTR		 description;					//���ж��ַ�����
	DWORD		 nInfo0;
	DWORD		 nInfo1;
	DWORD		 nInfo2;
	DWORD		 nInfo3;
	void*		 pReserved;						//����
}interruptInfo;

typedef struct _tagOutputChannelParam			//���ͨ������
{
	_tagOutputChannelParam()
	{
		eMode			= vidAudMode;
		nVbiNum			= 0;
		nDvbRate		= 0.0;
		bEnableD10		= FALSE;
		description		= "No description";
		nCacheLen		= 5;
	}
	channelMode				eMode;				//����ģʽ
	syncPort				eSyncPort;			//ͬ���˿�
	double					nDvbRate;
	videoStandard			eStd[100];			//���˿�����źŸ�ʽ
	videoStandard			eVidStd;			//����ź���Ƶ��ʽ
	BOOL					bEnableD10;			//D10ʹ�ܣ�����SD�źţ���Ч��
	videoDataFormat			eFormat;			//�����Ƶ���ݸ�ʽ
	audioSampleBits			audBits;			//��Ƶ�������
	audioSampleFrequency	eAudSample;			//��Ƶ������
	DWORD					nCacheLen;			//SDKѭ��Buffer���
	int						nVbiNum;
	vbiInfo					vbiParam[10];
	LPSTR					description;		//���ͨ�������ַ�����
	void*					pReserved;			//��չʹ���������SobeyCardParam.h�ļ��е�����
}outputChannelParam;
	
typedef struct _tagWarningInformation			//������Ϣ����
{
	_tagWarningInformation()
	{
		bFanWarning		= FALSE;
		bSyncWarning	= FALSE;
		bInportWarning	= FALSE;
		bOutportWarning = FALSE;
		for(int i=0; i<10; i++)
			bOtherWarning[i] = FALSE;
	}
	BOOL	bFanWarning;					//���ȱ���
	BOOL	bSyncWarning;					//ͬ������
	BOOL	bInportWarning;					//�����źű���
	BOOL	bOutportWarning;				//�������
	BOOL	bOtherWarning[10];				//���ñ����ź�
	DWORD	nWarningData[15];				//����
	void*	pReserved;						//����
}warningInformation;

typedef struct _tagVideoDataStruct			//��Ƶ��������
{
	_tagVideoDataStruct()
	{
		nVbiNum		= 0;
		nPicWide	= 0;
		nPicHigh	= 0;
		pUyvy		= NULL;
		pY			= NULL;
		pU			= NULL;
		pV			= NULL;
		pReserved	= NULL;
	}
	LPBYTE	pUyvy;							//UYVY���ݸ�ʽָ�루��videoDataFormatΪvideoUYVYʹ�ã�
	LPBYTE	pY;								//YUV���ݸ�ʽYָ�루��videoDataFormatΪvideoYUVʹ�ã�
	LPBYTE	pU;								//YUV���ݸ�ʽUָ�루��videoDataFormatΪvideoYUVʹ�ã�
	LPBYTE	pV;								//YUV���ݸ�ʽVָ�루��videoDataFormatΪvideoYUVʹ�ã�
	videoStandard std;						//�����ݵ���ʽ��Ϣ
	DWORD	nPicWide;						//ͼ����
	DWORD	nPicHigh;						//ͼ��߶�
	int		nVbiNum;
	vbiData	VbiData[10];
	void*	pReserved;						//����
}videoDataStruct,*pVideoDataStruct;

typedef struct _tagAudioDataStruct			//��Ƶ������������
{
	_tagAudioDataStruct()
	{
		pReserved		= NULL;
		pData			= NULL;
		nSampleCount	= 0;
		nAudioUvValue	= 0;
	}
	LPBYTE	pData;							//��Ƶ����ָ��
	DWORD	nSampleCount;					//���������������
	DWORD	nAudioUvValue;					//UV��ֵ
	void*	pReserved;						//����
}audioDataStruct,*pAudioDataStruct;

typedef struct _tagDataStruct				//ͨ�����ݽṹ
{
	_tagDataStruct()
	{
		bActive		= FALSE;
		pDvbSrc		= NULL;
		pReserved	= NULL;
		bInfo		= FALSE;
	}
	LPBYTE				pDvbSrc;			//DVB����ָ�루��ͨ������ģʽΪdvbģʽ����Ч��
	DWORD				nDvbBytes;			//DVB���ݳ��ȣ���ͨ������ģʽΪdvbģʽ����Ч��

	videoDataStruct		tVidSrc;			//��Ƶ������������ͨ������ģʽΪAVģʽ����Ч��
	audioDataStruct		tAudSrc[AUDIO_MAX_CHANNEL];//��Ƶ������������ͨ������ģʽΪAVģʽ����Ч��

	LONGLONG			nCounter;			//���ݼ���ֵ
	BOOL				bActive;			//��Ч���
	BOOL				bInfo;
	DWORD				nInfoValue;
	void*				pReserved;			//����
}dataStruct;

typedef struct _tagInterruptHandle			//�ж�����
{
	_tagInterruptHandle()
	{
		hErrReportInterrupt = NULL;
		hBufferInterrupt	= NULL;
		hInterrupt			= NULL;
		for(int i=0; i<10; i++)
			hReserved[i]	= NULL;
	}
	HANDLE				hErrReportInterrupt;	//���汨���ж�
	HANDLE				hBufferInterrupt;		//�ɼ�Bufer��/����Bufer�� �ж�
	HANDLE				hInterrupt;				//�ɼ�/���� ���ж�
	HANDLE				hReserved[10];			//����
}interruptHandle;

typedef struct _tagBoardInfo				//�忨����
{
	_tagBoardInfo()
	{
		description = "No description";
		nBoardNum	= 0;
		nType		= 0;
		pMemPtr		= NULL;
		nMemSize	= 0;
		pReserved	= NULL;
	}
	UINT			nBoardNum;				//�忨���
	DWORD			nType;					//�忨���ͺţ���SobeyCardBoardType.h��
	LPSTR			description;			//�忨�ַ�����
	boardCapability	capability;				//�忨������������

	DWORD	 nMemSize;						//�忨�����ڴ�ߴ�
	LPBYTE	 pMemPtr;						//�忨�ڴ�ָ��
	DWORD	 nMemAddr;						//�忨�ڴ������ַ
	void*	 pReserved;						//����
}boardInfo;

#endif
