 #ifndef   _MGCaptureDefine_H_    
#define   _MGCaptureDefine_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vfw.h"
#include <afxtempl.h>

//0x4D5846�������������벻���ֱ��д�ļ��ı�ʾ
#define	 MXFNATIVE 'MXF'

#define  BytePerFrameAudio   6000  //ÿ��֡��Ƶ�Ĵ�С
#define  AUDIO_CHANNEL_COUNT 8 //������Ŀ

typedef BOOL CALLBACK ERROR_REPORT_CB(void*pParaCB,int nErrorCode,const WCHAR* wcsErrorString); 
typedef BOOL CALLBACK DRAW_PREVIEW_CB(int nWidth,int nHeight,BYTE* pDataBuffer);

//#define  VIDEO_WIDTH    720    //ԭʼ���
//#define  VIDEO_HEIGHT_P 576    //ԭʼ�߶� ��PAL
//#define  VIDEO_HEIGHT_N 480    //ԭʼ�߶� : NTSC
//#define  VIDEO_WIDTH_PREVIEW   320 //���ź��Ԥ�����
//#define  VIDEO_HEIGHT_PREVIEW  240 //���ź��Ԥ���߶�

   
//Picture format defination
//The BiCompression in BITMAPIBFOHEADER Should match one of the following defines
#define MG_CLRMODEL_RGB  0x00000000		// RGB24 and RGB32
#define MG_CLRMODEL_UYVY 0x59565955		// UYVY 4:2:2, U0 Y0 V0 Y1
#define MG_CLRMODEL_YUY2 0x32595559		// YUY2 4:2:2, Y0 U0 Y1 V0
#define MG_CLRMODEL_IYUV 0x56555949		// YUV 4:2:0, one Y plane match 2*2 UV planes
#define MG_CLRMODEL_UNKNOWN 0xFFFFFFFF




//��������Ƶ֡����
#define MG_BUF_MPEG_UNKNOWN			0x1000
#define MG_BUF_RAW_FRAME			0x1001
#define MG_BUF_MPEG4_ES_KEY			0x01
#define MG_BUF_MPEG4_ES_NOTKEY			0x00
#define MG_BUF_MPEG2_I_PICTURE_BODY		0x1004
#define MG_BUF_MPEG2_P_PICTURE_BODY		0x1005
#define MG_BUF_MPEG2_B_PICTURE_BODY		0x1006

typedef struct stPerAudioInfo
{
	WORD nSize;
	WORD nType;	//0:Normal audio data   1:dolby-e data
	
	stPerAudioInfo()
	{
		nSize = 0;
		nType = 0;
	}
}PERAUDIOINFO;

typedef struct stAudioChannelInfo
{
	int          iAudioNum;
	PERAUDIOINFO stAudioInfo[AUDIO_CHANNEL_COUNT];
}AudioChannelInfo, *PAudioChannelInfo;

//��ʽ
enum MG_VideoStandard
{
	MG_VS_PAL = 0,
	MG_VS_NTSC,
	MG_VS_SECOM,
	MG_VS_NOSINGNASL,
	MG_VS_UNKNOWN,
};
typedef enum _tagSTART_CAP_RET 
{
	START_CAP_SUCCESS = 0,		//�ɹ�
	PARAM_ERROR = 1,			//�߱����������
	INITIAL_ERROR = 2,			//��ʼ����δ���
	SET_PARAM_ERROR = 3,		//�������ô���
	TASK_RUN_ERROR = 4,			//�Ѿ�����������
	END_CAP_ERROR = 5,			//�����ɼ�����
	READY_CAP_ERROR = 6,		//׼���ɼ�����
	PREPARE_CAP_ERROR = 7,		//Ԥ�ɼ�����
	START_CAP_ERROR = 8,		//��ʼ�ɼ�����
	PARSE_ERROR = 9,			//������������
	QUERY_STATE_ERROR = 10,		//��ѯ״̬����
	DBE_CHANNEL_ERROR = 11,	//dbe��������2��
	MP3_BITDEPTH_ERROR = 12,			//MP3λ�Ϊ16
	DBE_CHANNEL_JUMP = 13,	//dbe������Ծ������
	DBE_CHANNEL_MORE = 14,
	DBE_BITDEPTH_ERROR = 15,
}START_CAP_RET;

//��Ƶ�˿�����
typedef enum _tagMG_VidAdapt
{
	MG_VidCVBS = 0,
	MG_VidYC	,
	MG_VidYUV	,
	MG_VidSDI	,
	MG_VidESDI,
	MG_VidReserved1,
	MG_VidReserved2,
	MG_VidReserved3,
	MG_VidReserved4,
}MG_VidAdapt;

//��ƵƵ�˿�����
typedef enum _tagMG_AudAdapt
{
	MG_NoAud = 0,
	MG_AnologeAud,
	MG_DataAud	,
	MG_AudReserved2,
	MG_AudReserved3,
	MG_AudReserved4,
}MG_AudAdapt;

// д��Ƶ�ļ�����
enum MG_AVWriteType
{
	MG_WRITETYPE_UNKNOWN	= 0,
	MG_WRITETYPE_RIFF		= 1,
	MG_WRITETYPE_MATROX		= 2,
	MG_WRITETYPE_MAV		= 3,
	MG_WRITETYPE_GXF		= 4,
	MG_WRITETYPE_MXF		= 5,
	MG_WRITETYPE_PS			= 6,
	MG_WRITETYPE_WMV		= 7,
	MG_WRITETYPE_AVI2		= 8,	
	MG_WRITETYPE_MP4		= 70,
	MG_WRITETYPE_TS			= 189,
	MG_WRITETYPE_ISMV		= 191,
	MG_WRITETYPE_DONOTWRITEFILE = 0XFF
};

// д��Ƶ�ļ�����
enum MG_AudioWriteType
{
	MG_AUDIOWRITETYPE_UNKNOWN	= 0,
	MG_AUDIOWRITETYPE_WAV		= 21,
	MG_AUDIOWRITETYPE_MP2		= 22,
	MG_AUDIOWRITETYPE_MP3		= 23,
	MG_AUDIOWRITETYPE_MXF		= 24,
	MG_AUDIOWRITETYPE_AAC		= 25,
	MG_AUDIOWRITETYPE_AC3		= 26,
	MG_AUDIOWRITETYPE_DBE		= 27,
	MG_AUDIOWRITETYPE_HEAAC		= 28,
};

//��������
enum MG_EncodeType
{
	MG_ENCTYPE_UNKNOWN	= 0,
	MG_ENCTYPE_MPEG2	= 1,
	MG_ENCTYPE_MPEG4	= 2,
	MG_ENCTYPE_MJPG		= 3,
	MG_ENCTYPE_DV		= 4,
	MG_ENCTYPE_ASF		= 5,
	MG_ENCTYPE_REAL		= 7,
	MG_ENCTYPE_WMV		= 21,		
	MG_ENCTYPE_DNxHD	= 55,
	MG_ENCTYPE_AVCIntra = 56,
	MG_ENCTYPE_H264		= 57,		
};

//������������
#define MG_MPEG4_MSV2  '24pm'
#define MG_MPEG4_MSV3  'v4pm'

#define	MG_MPEG2TYPE_UNKNOWN 0
#define	MG_MPEG2_SONY_IMX 1				
#define	MG_MPEG2_MATROX_I 2				
#define	MG_MPEG2_MATROX_IBP 3			

#define  MG_DVCAM_DVSD      0  
#define  MG_DVCPRO_DV25     1 
#define  MG_DVCPRO50_DV50   2

#define	MG_WMV_9  0
#define	MG_WMV_8  1

//�ļ���ʽ
#define  MG_VFILE_MPEG2  'SEMM'   //     MPEG2 I or IBP: bih.biCompression = 'SEMM'
#define  MG_MPEG4_FILE_MSV2  '24pm'
#define  MG_MPEG4_FILE_MSV3  'v4pm'
#define  MG_FILE_DVCAM_DVSD  'dsvd'
#define  MG_FILE_DVCPRO_DV25  '52vd'
#define  MG_FILE_DVCPRO50_DV50 '05vd'
#define  MG_FILE_MJEP         'GPJM'
#define  MG_FILE_DNxHD        'NDVA'

// ��ý��
enum MG_ISMAType
{
	MG_ISMA_UNKNWON = 0,
	MG_ISMA_ASF,
	MG_ISMA_WMV8,
	MG_ISMA_WMV9,
	MG_ISMA_REAL8,
	MG_ISMA_REAL9,
};


//��Ƶģʽ
enum MG_AudioMode
{
	MG_MODE_UNKNOWN = 0,
	MG_MODE_MONO,
	MG_MODE_STEREO,
};

//��Ƶ����Ƶ��
enum MG_AudioFrequency
{
	MG_AF_8000  = 8000,
	MG_AF_32000 = 32000,
	MG_AF_44100 = 44100,
	MG_AF_48000 = 48000, 
};

//�忨״̬
typedef enum _tagMgCardStatus
{
	statusCardNoInit = 0,
	statusCardInited,
	statusCardCapturing,
	statusCardCapturePause,
	statusCardTransfering,
	statusCardTransferSeeking,
	statusCardTransferPause,
	statusCardUnknownState,
	statusCardNotSet,
}MgCardStatus;


typedef struct _tagManualKeyFrame
{
	DWORD dwTaskFrameNo;
	DWORD dwTimeCode;
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwBitDepth;
	LPBYTE pPicData;
	
	_tagManualKeyFrame()
	{
		dwTaskFrameNo = -1;
		dwTimeCode = -1;
		dwWidth = 0;
		dwHeight = 0;
		dwBitDepth = 0;
		pPicData = NULL;
	}
}MANUALKEYFRAME;

//�ɼ���������Ϣ
typedef struct tagCardInfo  
{
	int  nCardID;	     	//��ID
	int  nCardTypeID;       //������ID
	BOOL bInitSuccess;      //��ʼ���Ƿ�ɹ� TRUE���ɹ� FALSE �� ʧ�� 
	TCHAR  strCardDescription[MAX_PATH];  //������
	float  fCardVersion;		  //���汾��Ϣ
  
	tagCardInfo()
	{
		nCardID		= -1;
		nCardTypeID  = -1;
		bInitSuccess = FALSE;
		fCardVersion = 0;		 
	}
}CARDINFO,*PCARDINFO;
//����������
struct MGEncodePlugParam 
{
	////////////common
	int  mpeg_type;  //���� ���� ����
    ////////////video
    bool video_enable; // =false for disable video stream
						// =true for enable video stream
	int  bit_rate;     // For MPEG-1 and MPEG-2:
                        // Bit rate in bits/sec.  130000 < bit_rate <100000000
	int  video_width;  // width of destination video
	int  video_height; // height of destination video
	int  video_stand;  //video stand 
	int  nFrameRateIndex;
	int  clrModel; //the type of compression for a compressed
	int  nAverageTimePerFrame; 
	int  iGopBCount;
	int  iGopPCount;
    
	////////////audio                   
    bool  audio_enable;// =false ������  ��Ŀǰ��������Ƶ���룡����������
	                   // =true ����
	int   audio_mode;  // For MPEG-1 and MPEG-2:  0: Mono,1: Stereo
	int   audio_frequency; // For MPEG-1 and MPEG-2:
                           // =44100 (Default) , Valid values are 32000, 44100, or 48000
	int  iVideoDataType;
    MGEncodePlugParam()
	{
		mpeg_type = -1;
		video_enable = TRUE; //
		bit_rate = 0;
		video_width = 0;
		video_height = 0;
		video_stand = MG_VS_PAL;
        clrModel = MG_CLRMODEL_UYVY;
		audio_enable = FALSE; //��Ŀǰ��������Ƶ���룡����������
		audio_mode = 1;
		audio_frequency = MG_AF_48000;

		iGopBCount = 2;
		iGopPCount = 4;
		iVideoDataType = -1;
	}
    
};
//�������
struct MGEncodeParam 
{
	BOOL bPath0;  //��·0���������� 720��576 ���� 720��480
	BOOL bAudio0; //��·0 �Ƿ������Ƶ
	MG_EncodeType  nEncodeType0; //������· 0 ��������
	MGEncodePlugParam EncodePlugParam0;
	BOOL bPath1;  //��·1 
	BOOL bAudio1; //��·1 �Ƿ������Ƶ
	MG_EncodeType  nEncodeType1; //������· 1 ��������
	MGEncodePlugParam EncodePlugParam1;
};

struct MGWriteFilePlugParam
{	
	MG_VideoStandard vs;
	int  nFrameRateIndex;	// =1 for 23.97 f/sec  
							// =2 for 24 f/sec
							// =3 for 25 f/sec     
							// =4 for 29.97
							// =5 for 30 f/sec
    int video_Width; //���
    int video_height; //����

	int  nAverageTimePerFrame;
	    
	TCHAR strFileNameV[MAX_PATH];	
	TCHAR strFileNameA[MAX_PATH];	
	
	MG_EncodeType  nEncodeType;	    //��������
	int  subEncodeType;			    //���� ���� ����
	int  bit_rate;					// For MPEG-1 and MPEG-2:
									// Bit rate in bits/sec.  130000 < bit_rate <100000000

	MG_AudioWriteType enSrcAudioEncType;
	MG_AudioWriteType enDstAudioEncType; //��Ƶ��������
    
	DWORD  dwSamplesInA;                //���������
	DWORD  dwSamplesOutA;			    //���������
	DWORD  dwMp3RateA;                  //�����MP3������
	int	   iAudioType;					//1:��������2��˫����
	int    iBitDepth;				    //��Ƶ����λ��	
    
	DWORD		dwGOPBCount;
	DWORD		dwGOPPCount;
	MGWriteFilePlugParam()
	{
		enSrcAudioEncType = MG_AUDIOWRITETYPE_WAV;
		enDstAudioEncType = MG_AUDIOWRITETYPE_WAV;
		iAudioType = 1;
	}
};



//�ļ�����
struct MGWriteFileParam
{	
    DWORD  dwCutClipFrame; //�������Ҫ�ֶ� Ĭ��Ϊ0xfffffffe
	BOOL  bRetrospect; //
	DWORD  dwAudMaskA;//˵����Ƶ����ѡ��

	BOOL bPath0;  //��·0 �Ƿ�д�ļ� ����Ƶ��
	BOOL bAudio0; //��·0 �Ƿ������Ƶ
	BOOL bAlone0; //��·0 �Ƿ����������Ƶ �� TRUE : ����������Ƶ�ļ� �� FALSE : ����Ƶ�ļ�����һ���ļ�
    MG_AVWriteType AVWriteTypeV0; //V0д�ļ�����
	MGWriteFilePlugParam  plugParam0; //V0д�ļ�������� 

	// ldw
	MG_AudioWriteType AudioWriteTypeA0; // A0д�ļ�����
	// ldw

	BOOL bPath1;  //��·1 �Ƿ�д�ļ� ����Ƶ��
	BOOL bAudio1; //��·1 �Ƿ������Ƶ
	BOOL bAlone1; //��·1 �Ƿ����������Ƶ �� TRUE : ����������Ƶ�ļ� �� FALSE : ����Ƶ�ļ�����һ���ļ�
    MG_AVWriteType AVWriteTypeV1; //V1д�ļ�����
	MGWriteFilePlugParam  plugParam1;//V1д�ļ��������
	
	// ldw
	MG_AudioWriteType AudioWriteTypeA1; // A1д�ļ�����
	// ldw

	MGWriteFileParam()
	{
		bPath0 ;  
		bAudio0 = TRUE; 
		bAlone0 = TRUE; 
		AVWriteTypeV0 = MG_WRITETYPE_UNKNOWN;

	// ldw
		AudioWriteTypeA0 = MG_AUDIOWRITETYPE_UNKNOWN;
	// ldw

		bPath1 = FALSE;
		bAudio1 = TRUE;
		bAlone1 = TRUE; 
		AVWriteTypeV1 = MG_WRITETYPE_UNKNOWN;

	// ldw
		AudioWriteTypeA1 = MG_AUDIOWRITETYPE_UNKNOWN;
	// ldw

	}
};

//�ɼ����Ʋ���,�ӿڲ���
struct MGCaptureCtrlParam
{ 
	int nTaskID;//����ID

	MG_VideoStandard enumVideoST;//��Ƶ��ʽ
	int  nFrameRateIndex;//֡������ // =1 for 23.97 f/sec  
					                // =2 for 24 f/sec
									// =3 for 25 f/sec     
									// =4 for 29.97
									// =5 for 30 f/sec
	int	nWidth;//��Ƶ���
	int	nHeight;//��Ƶ�߶�
	int	nWidthLow;//�����ʿ��
	int	nHeightLow;//�����ʸ߶� 

	int nInOutCount;//������������Ķ��������֧��100��
	DWORD  dwInFrame[100];//��ʼ֡ ���Ϊ0 ��������ʼ. ����֡����Ҫ�ɼ��ģ�
	DWORD  dwOutFrame[100];//������֡�� ������ɼ���֡�������ֵ �����Զ��رգ�  �������Ҫ�в��Զ����� ��Ϊ0xfffffffe
	DWORD  dwTotalFrame[100];
	DWORD  dwCutClipFrame; //�������Ҫ�ֶ� Ĭ��Ϊ0xfffffffe		
	BOOL  bRetrospect; //�Ƿ��˷�ɼ� 
	BOOL  bDetectKeyFrame;//�Ƿ�����ת��֡ʶ��

	int iGopBCount;
	int iGopPCount;

	BOOL bPath0;  //��·0 �Ƿ�д�ļ� ����Ƶ��
	BOOL bAudio0; //��·0 �Ƿ������Ƶ
	BOOL bAlone0; //��·0 �Ƿ����������Ƶ �� TRUE : ����������Ƶ�ļ� �� FALSE : ����Ƶ�ļ�����һ���ļ�
	TCHAR strFileNameV0[MAX_PATH];
	TCHAR strFileNameA0[MAX_PATH];
	MG_EncodeType  nEncodeType0; //������· 0 ��������
	int  subEncodeType0;  //���� ���� ����
	int  bit_rate0;     // For MPEG-1 and MPEG-2:
	// Bit rate in bits/sec.  130000 < bit_rate <100000000
	MG_AVWriteType AVWriteTypeV0; //V0д�ļ�����
	DWORD  dwAudMaskA0;//˵����Ƶ����ѡ��
	DWORD  dwSamplesOutA0;//���������
	DWORD dwMp3RateA0; //�����MP3������
	int   iAudBitDepth0;//��Ƶ����λ��
	MG_AudioWriteType AudioWriteTypeA0; //A0д�ļ����� ,���û���ҵ��ö��󣬾�ֱ��ʹ��wav	
	
	BOOL bPath1;  //��·1 �Ƿ�д�ļ� ����Ƶ��
	BOOL bAudio1; //��·1 �Ƿ������Ƶ
	BOOL bAlone1; //��·1 �Ƿ����������Ƶ �� TRUE : ����������Ƶ�ļ� �� FALSE : ����Ƶ�ļ�����һ���ļ�
	TCHAR strFileNameV1[MAX_PATH];
	TCHAR strFileNameA1[MAX_PATH];
	MG_EncodeType  nEncodeType1; //������· 1 ��������
	int  subEncodeType1;  //���� ���� ����
	int  bit_rate1;     // For MPEG-1 and MPEG-2:
	// Bit rate in bits/sec.  130000 < bit_rate <100000000
	MG_AVWriteType AVWriteTypeV1; //V1д�ļ�����
	DWORD  dwAudMaskA1;//˵����Ƶ����ѡ��
	DWORD  dwSamplesOutA1;//���������
	DWORD dwMp3RateA1; //�����MP3������
	int   iAudBitDepth1;//��Ƶ����λ��	
	MG_AudioWriteType AudioWriteTypeA1; //A1д�ļ����� ,���û���ҵ��ö��󣬾�ֱ��ʹ��wav

	//Add by wx 2009-11-23,�ɼ�������ӽڵ㣬Ϊ��֧���в�1440*1080�ɼ�������Ӧʶ��
	DWORD	dwFirPicNum;	//ˢ����֡����ͼ��֡��
	DWORD	dwVideo_width;
	DWORD	dwVideo_height;
	WORD	 wCHROMA_FORMAT;	//_chroma_420 = 0,_chroma_422 = 1,_chroma_444 = 2,
   //Add over
	//��������������
	int  iHighVideoDataType;
	//��������������
	int  iLowVdeoDataType;

	//Add by zkw 2011-11-02
	
	int							AudioEncodeTypeA0;//A1�����ʽ
	int							AudioEncodeTypeA1;//A1�����ʽ
	
	

	TCHAR strStream_URL[255]; //��ý��url
	TCHAR strStream_Type[255];//��ý������

	DWORD dwRev[8];
	//End add
	MGCaptureCtrlParam()
	{
		iGopBCount = 2;
		iGopPCount = 4;

		bPath0 = TRUE; 
		bAudio0 = TRUE;
		bAlone0 = TRUE;
		strFileNameV0[0] = 0;
		strFileNameA0[0] = 0;
		nEncodeType0 = MG_ENCTYPE_UNKNOWN;
		subEncodeType0 = -1;  //���� ���� ����
		bit_rate0 = -1;     // For MPEG-1 and MPEG-2:
		AVWriteTypeV0 = MG_WRITETYPE_UNKNOWN;
		dwSamplesOutA0 = 0;
		iAudBitDepth0 = 16;
		
		bPath1 = TRUE;
		bAudio1 = TRUE;
		bAlone1 = TRUE;
		strFileNameV1[0] = 0;
		strFileNameA1[0] = 0;
		nEncodeType1 = MG_ENCTYPE_UNKNOWN; //������· 1 ��������
		subEncodeType1 = -1;  //���� ���� ����
		bit_rate1 = -1;     // For MPEG-1 and MPEG-2:
		AVWriteTypeV1 = MG_WRITETYPE_UNKNOWN;
		dwSamplesOutA1 = 0;
		//zkw modify 2011-11-02
		iAudBitDepth1 = 16;//16;
		//End modify
		nInOutCount = 0;
		dwTotalFrame[0] = 0;
		dwFirPicNum = 0;
		dwVideo_width = 1920;
		dwVideo_height= 1080;
		wCHROMA_FORMAT = 1;
        iHighVideoDataType = -1;
		iLowVdeoDataType   = 0;

		
	}
};

//Add by wx for 4.5 Import
struct MGWriteFileParam_Plus
{	
    DWORD	dwCutClipFrame; //�������Ҫ�ֶ� Ĭ��Ϊ0xfffffffe
	BOOL	bRetrospect; 
	DWORD	dwAudMaskA;//˵����Ƶ����ѡ��
	int		nUseImportMode;		//����Importģʽ,0:������ģʽ��1����SDKд�ļ�������ģʽ��2�������ֱ��дMXF����ģʽ

	TCHAR strAudioName[8][MAX_PATH];		//for import use
	BOOL bPath0;  //��·0 �Ƿ�д�ļ� ����Ƶ��
	BOOL bAudio0; //��·0 �Ƿ������Ƶ
	BOOL bAlone0; //��·0 �Ƿ����������Ƶ �� TRUE : ����������Ƶ�ļ� �� FALSE : ����Ƶ�ļ�����һ���ļ�
    MG_AVWriteType AVWriteTypeV0; //V0д�ļ�����
	MGWriteFilePlugParam  plugParam0; //V0д�ļ�������� 
		
	MG_AudioWriteType AudioWriteTypeA0; // A0д�ļ�����	
	
	BOOL bPath1;  //��·1 �Ƿ�д�ļ� ����Ƶ��
	BOOL bAudio1; //��·1 �Ƿ������Ƶ
	BOOL bAlone1; //��·1 �Ƿ����������Ƶ �� TRUE : ����������Ƶ�ļ� �� FALSE : ����Ƶ�ļ�����һ���ļ�
    MG_AVWriteType AVWriteTypeV1; //V1д�ļ�����
	MGWriteFilePlugParam  plugParam1;//V1д�ļ��������
		
	MG_AudioWriteType AudioWriteTypeA1; // A1д�ļ�����	

	//�����ֶ�
	BYTE Reserve [128];
	DWORD dwReserve1;
	DWORD dwReserve2;
	
	MGWriteFileParam_Plus()
	{
		nUseImportMode = 0;
		bPath0 = TRUE;  
		bAudio0 = TRUE; 
		bAlone0 = TRUE; 
		AVWriteTypeV0 = MG_WRITETYPE_UNKNOWN;
		AudioWriteTypeA0 = MG_AUDIOWRITETYPE_UNKNOWN;				
		bPath1 = FALSE;
		bAudio1 = FALSE;
		bAlone1 = FALSE; 
		AVWriteTypeV1 = MG_WRITETYPE_UNKNOWN;				
		AudioWriteTypeA1 = MG_AUDIOWRITETYPE_UNKNOWN;				
	}
};

typedef struct _ClipFrameLog
{	
	DWORD			dwFrameBufferSize;	//��֡���ݵ��ܴ�С	
	DWORD			dwFrm_type;				//��֡��Ƶ����֡����
	DWORD			dwVSize;				//��֡��������Ƶ���ݴ�С
	BOOL				bIsEnd;					//��������һ֡����TRUE
	_ClipFrameLog()
	{
		dwFrameBufferSize = 0;
		dwFrm_type = MG_BUF_MPEG_UNKNOWN;
		dwVSize = 0;
		bIsEnd = FALSE;
	}
}ClipFrameLog;
//Add over

//���ļ��У�ÿһ��ITEM��Ĳ���
struct  MGFileFrameItem  
{
	char   FrameHead[16];
	int nKeyFrame;  //<0:���ǹؼ�֡    >=0���ǹؼ�֡���
    int nManualFrame; //<0:�����˹����֡    >=0 ���˹����֡���
	int nManualFrameType;//�˹����֡����

	BYTE*  pBufV0;     //����ָ��---V0
	int  nBufSizeV0;  //V0��buffer����
    BYTE*  pBufV1;     //����ָ��---V1
	int  nBufSizeV1;  //V1��Buffer����
	BYTE*  pBufA0;     //����ָ��---A0
	int  nBufSizeA0;  //
	BYTE*  pBufA1;     //����ָ��---A1
	int  nBufSizeA1;

	int  nFrameTypeV0;    //V0֡����
	int  nFrameTypeV1;    //V1֡����
	
	int nFileEndFlag ;    //�ļ�������־
 
	MGFileFrameItem()
	{
		ZeroMemory(FrameHead,16);
		strcpy_s(FrameHead,_countof(FrameHead),"SobeyFrm");
		pBufV0 = NULL;     //����ָ��---V0
		nBufSizeV0 = 0;  //V0��buffer����
		pBufV1 = NULL;     //����ָ��---V1
		nBufSizeV1 = 0;  //V1��Buffer����
		pBufA0 = NULL;     //����ָ��---A0
		nBufSizeA0 = 0;  //
		pBufA1 = NULL;     //����ָ��---A1
		nBufSizeA1 = 0;

		nKeyFrame = -1;
		nManualFrame = -1;

		nFileEndFlag = 0;
	}
};

struct ASIStreamParam
{
	UINT	uProgrameNum;
	UINT	uPMTPid;
	UINT	uVideoPID;
	UINT	uAudioPID;
	int		nWidth;
	int		nHeight;
	int	    nChromaFormat;
	int		nProgressiveSequence;
	int		nVideoStandard;
	TCHAR	chServiceName[256];
	ASIStreamParam::ASIStreamParam()
	{
		uProgrameNum = 0;
		uVideoPID = 0;
		uAudioPID = 0;
		nWidth = 0;
		nHeight = 0;
	    nChromaFormat = 0;
		nProgressiveSequence = 0;
		nVideoStandard = 0;
		uPMTPid = 0;
		memset(chServiceName,0,sizeof(TCHAR)*256);
	}
};

typedef CArray<ASIStreamParam,ASIStreamParam> CStreamParamArray;

typedef enum VideoDataFormat
{
	VID_RGB = 0,
	VID_YUV,
	VID_YUYV,
	VID_UYVY
};
typedef struct Uploadinfo
{
  TCHAR strTaskName[100]; 	//������������
  int nTaskID;	      	//��������ID	
  int nTrimIn;	      	//�������	
  int nTrimOut;	      	//�������	
  int state;
  MGCaptureCtrlParam captureParam; 	//�ɼ�����
}UploadinfoMG;

enum eTaskStatus
{
	StreamIn_TaskBegin,
    StreamIn_StopTask, 
    StreamIn_TaskEnd,
	StreamIn_TaskBlackCutClip,

	Encode_ThreadBegin,
	Encode_TaskBegin,
    Encode_TaskEnd,
	Encode_TaskBlackCutClip,
	Encode_ThreadEnd,

	Write_ThreadBegin,
	Write_TaskBegin,
    Write_TaskEnd,
	Write_TaskBlackCutClip,
	Write_ThreadEnd,

	Audit_Begin,
};
struct Format_Black
{
	DWORD dwFormat;		//SDI�ź���ʽ
	BOOL bIsBlack;		//�Ƿ��Ǻڳ�
};
//MGStreamIn�ص�����
class IMGStreamInCallBack
{
public:
	virtual void StreamIn_ERROR_CB(int nTaskID,int nErrorCode,int nErrorGrade,CString strErrorString)=0;
    virtual void StreamIn_TaskStatus_CB(int nTaskID,eTaskStatus eStatus,CString strGUID = _T(""))=0;
    virtual void StreamIn_Preview_CB(int nWidth,int nHeight,BYTE* pDataBuffer)=0;
};

//X1StreamIn�ص�����
class IStreamInCallBack
{
public:
	virtual void Encode_ERROR_CB(int nTaskID,int nErrorCode,int nErrorGrade,CString strErrorString)=0;
    virtual void Encode_TaskStatus_CB(int nTaskID,eTaskStatus eStatus)=0;
};

//Encode�ص���
class IMGEncodeCallBack
{
public:
    virtual void Encode_ERROR_CB(int nTaskID,int nErrorCode,int nErrorGrade,CString strErrorString)=0;
    virtual void Encode_TaskStatus_CB(int nTaskID,eTaskStatus eStatus)=0;
};
//д�ļ��ص�
class IMGWriteCallBack
{
public:
    virtual void Write_ERROR_CB(int nTaskID,int nErrorCode,int nErrorGrade,CString strErrorString)=0;
    virtual void Write_TaskStatus_CB(int nTaskID,eTaskStatus eStatus,int iTaskClipSum=0)=0;	
};

class IMGCaptureCtrlCallBack
{
public:
    virtual void CaptureCtrl_ERROR_CB(int nTaskID,int nErrorCode,int nErrorGrade,CString strErrorString)=0;
    virtual void CaptureCtrl_TaskStatus_CB(int nTaskID,eTaskStatus eStatus,int iTaskClipSum=0,CString strGUID = _T(""))=0;
	virtual void CaptureCtrl_Preview_CB(int nWidth,int nHeight,BYTE* pDataBuffer)=0;
};

typedef struct _stBUFFERCELL
{	
	DWORD	dwBufferPos;
	DWORD	dwUnitNum;

	_stBUFFERCELL()
	{				
		dwBufferPos = 0;
		dwUnitNum	= 0;		
	}
}BUFFERCELL;

struct ListItemInfoBase
{
	LPBYTE	pBufV0;
	LPBYTE	pBufA0;
	LPBYTE	pBufV1;
	LPBYTE	pBufA1;

	DWORD  nBufSizeUsedV0; //V0 bufferռ��Byte��
	DWORD  nBufSizeUsedA0; //A0 bufferռ��Byte��
	DWORD  nBufSizeUsedV1; //V1 bufferռ��Byte��	
	DWORD  nBufSizeUsedA1; //A1 bufferռ��Byte��

	BUFFERCELL stBufInfo[4];//V0/A0/V1/A1	
};
#endif