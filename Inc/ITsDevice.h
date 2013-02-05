#pragma once

#ifdef I_TSDEVICEMANAGER_EXPORTS
#define I_TSDEVICEMANAGER_API __declspec(dllexport)
#else
#define I_TSDEVICEMANAGER_API __declspec(dllimport)
#endif
#include "GpsGateDatadef.h"
class MGBufferListEx;
//����ΪTSDevice�����ߵĻ��ඨ��
class I_TSDEVICEMANAGER_API ITSDeviceManager
{
public:
	BOOL static CreateInstance(ITSDeviceManager **ppTSDeviceManager);
	/*
	 *	��ʼ��
	 */
	virtual BOOL InitDeviceMan()=0;

	virtual void SetBufferList(MGBufferListEx *pBufferList) = 0;
	/*
	 *	����ʼ��
	 */
	virtual BOOL UnInitDeviceMan()=0;
	/*
	 *	��ʼ����
	 */
	virtual BOOL StartWork()=0;
	/*
	 * ֹͣ����
	*/
	virtual BOOL StopWork()=0;

	virtual int GetBindPort()=0;
	virtual CString GetChannelName()=0;
	/*
	 *	����ĳ��ͨ����IP
	 */
	virtual int Relocate(LPCTSTR lpBindIp,int iBindPort,LPCTSTR lpMulticastIp,int nIPType)=0;//nIPType=0:UDP;nIPType=1:RTP
};

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//����ΪTSDevice�Ļ��ඨ��
struct TsOnIpChannelInfo
{
	CString			m_sBindIp;
	int				m_nBindPort;
	CString			m_sMulticastIp;

	CString			m_sProtol;
	BOOL			m_bSSRC;
	int				m_nSSRC;

	TsOnIpChannelInfo()
	{
		m_sBindIp = _T("127.0.0.1");
		m_nBindPort = 0;
		m_sMulticastIp = _T("0.0.0.0");

		m_sProtol = _T("RTP");
		m_bSSRC = FALSE;
		m_nSSRC = 0;
	}
};

enum DeviceTYPE
{
	ASI_DEVICE	= 1,
	IP_DEVICE	= 2
};

struct tag_DeviceInfo
{
	DeviceTYPE DeviceType;//�豸����
	int nAnalyzeType;//����ͨ��������
	int nDeviceIdx;//�豸id
	int nDeviceTypeID;// ͬһ�����豸��ID����ϵͳ���豸���������е�����λ�ã���0��ʼ
	int nChannelCount;//���豸���е�ͨ������
};

#ifdef _ITSDEVICE_EXPORT_
#define TSDEVICE_EXPORT __declspec(dllexport)
#else
#define TSDEVICE_EXPORT __declspec(dllimport)
#endif

class ITsDevice_Card;

#ifdef __cplusplus
extern "C" 
{
#endif

	void TSDEVICE_EXPORT TSDevice_GetPlugInObject(ITsDevice_Card** ppITsDevice_Card); //��ȡ������� ,��ʹ�������֮�󣬱�����Ӧ�ı������ReleasePluginObject�������ͷŶ���
	void TSDEVICE_EXPORT TSDevice_ReleasePlugInObject(ITsDevice_Card* pITsDevice_Card);//�ͷŲ������
	DeviceTYPE  TSDEVICE_EXPORT TSDevice_PluginID();//��ȡ���ID

#ifdef  __cplusplus
}
#endif

static const int TS_BUFFER_SIZE = 1024*1024*2;

class TSDEVICE_EXPORT ITsDevice_Card
{
public:
	static int CreateInstance(ITsDevice_Card **pCard);
	/*
	 *	��ʼ��Ӳ���豸
	 * @param nCardID  �忨�ţ���ϵͳ���豸���������е�����λ�ã���0��ʼ��
	 */
	virtual BOOL	InitDevice(int nListenPort) = 0;
	virtual BOOL	ReleaseDevice() = 0;

	/*
	 *	��ʼ��������
	 */
	virtual int		StartInput() = 0;
	/*
	 *	ֹͣ�������
	 */
	virtual int		StopInput() = 0;

	virtual int sendData(const char *pData,int nDataSize,const char *pAddr,int nPort)=0;
	virtual int sendData(const GPSGATEDATA *pGpsGateData)=0;
	virtual void SetMemShareBuffer(MGBufferListEx *pBufferList,DWORD dwBufferSize) = 0;

};

//�ռ��������
//�������
typedef  void (*GETPLUGINOBJECT)(ITsDevice_Card**);
typedef  void (*RELEASEPLUGINOBJECT)(ITsDevice_Card*);
typedef  DeviceTYPE (*PLUGINID)();
typedef struct tagPLUGIN_CELL_TSDEVICE
{
	HINSTANCE	hPlugInstance;
	DeviceTYPE  nPlugInType;
	GETPLUGINOBJECT GetPlugInObject;
	RELEASEPLUGINOBJECT ReleasePlugInObject;
}PLUGIN_CELL_TSDEVICE, *PPLUGIN_CELL_TSDEVICE;