#ifndef I_STREAM_PROCESS_H
#define I_STREAM_PROCESS_H


#ifdef I_STREAM_PROCESS_EXPORTS
#define I_STREAM_PROCESS_API __declspec(dllexport)
#else
#define I_STREAM_PROCESS_API __declspec(dllimport)
#endif

class IStreamProcess;

#ifdef __cplusplus
extern "C" 
{
#endif

	void I_STREAM_PROCESS_API StreamProcess_GetPlugInObject(IStreamProcess** ppIStreamProcess); //��ȡ������� ,��ʹ�������֮�󣬱�����Ӧ�ı������ReleasePluginObject�������ͷŶ���
	void I_STREAM_PROCESS_API StreamProcess_ReleasePlugInObject(IStreamProcess* pIStreamProcess);//�ͷŲ������
	int  I_STREAM_PROCESS_API StreamProcess_PluginID();//��ȡ���ID

#ifdef  __cplusplus
}
#endif

#include "PC_TSStructDefine.h"
#include "TSBaseDefine.h"
#include "MGBufferListEx.h"
class  IStreamProcess
{
public:
	virtual int			Initialize(int nTSDataSize = 2000000)=0;
	virtual int			SetBufferList(MGBufferListEx *pDataList)=0;
	/*
	 *	�ض�λIP port
	 *  ��TS��¼��ֱ�ӷ���false
	 *  ��RTMP ��¼�з���true ����¼��URL����Ϣ��

	 * @param LPCTSTR lpBindIp			[TS��¼] ����IP		  ;[RTMP]  URL
	 * @param int iBindPort				[TS��¼] �鲥���˿�   ;[RTMP]  none
	 * @param LPCTSTR lpMulticastIp     [TS��¼] �鲥��IP	  ;[RTMP]  StreamType;"http","rtmp","rtsp"
	 * @param int nPgmID				[TS��¼] ��ĿID		  ;[RTMP]
	 * @param int nIPType				[TS��¼]			  ;[RTMP]
	 * @return 0�������ģ��û����relocate  1:�����ģ������ Relocate������ģ�鲻������
	 */
	virtual int			Relocate(LPCTSTR lpBindIp,int iBindPort,LPCTSTR lpMulticastIp,int nPgmID,int nIPType)=0;

	virtual int			StartStreaming()=0;
	virtual int			SetStreamInfoNotifyCB(PC_TSStreamInfoNotifyCB pFunc, void* pUserData)=0;
	virtual int			SetPostESBufferCB(int nPID, PC_PostESDataCB2 pFunc, void* pUserData)=0;	
	virtual int			GetStreamInfo(TS_StreamInfo* pInfo)=0;
	virtual int			GetStreamBitrate(double& dBitrate)=0;
	virtual int			StopStreaming()=0;
	virtual int			ClearPoolData()=0;
	virtual int			GetStreamInfoNotifyCB(void** pFunc, void** pUserData)=0;
	virtual void		ReStartStreaming()=0;
	virtual void		SetChannelName(LPCTSTR lpChannelName)=0;
};

//�ռ��������
//�������
typedef  void (*GET_STREAMPROCESS_PLUGINOBJECT)(IStreamProcess**);
typedef  void (*RELEASE_STREAMPROCESS_PLUGINOBJECT)(IStreamProcess*);
typedef  int (*STREAMPROCESS_PLUGINID)();
#endif