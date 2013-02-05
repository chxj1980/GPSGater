#pragma once
#include "PC_TSStructDefine.h"
#include <vector>
using namespace std;

#ifdef PC_TSSTREAMBASE_EXPORTS
#define PC_TSSTREAMBASE_API __declspec(dllexport)
#else
#define PC_TSSTREAMBASE_API __declspec(dllimport)
#pragma  comment(lib,"TSStreamProcess.lib")
#endif

struct PC_THREADDATA;
struct TSDATA;
struct Transport_Packet;

class TSStreamProcessMan;
class MGBufferListEx;
class IStreamProcess;
class PC_TSSTREAMBASE_API CPC_TSStreamProcess_Interface
{
public:
	CPC_TSStreamProcess_Interface();
	virtual		~CPC_TSStreamProcess_Interface();
public:
	int			Initialize(int nTSDataSize = 2000000);
	int			SetBufferList(MGBufferListEx *pDataList);
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
	virtual int			Relocate(LPCTSTR lpBindIp,int iBindPort,LPCTSTR lpMulticastIp,int nPgmID,int nIPType);
	int			StartStreaming();
	int			SetStreamInfoNotifyCB(PC_TSStreamInfoNotifyCB pFunc, void* pUserData);
	int			SetPostESBufferCB(int nPID, PC_PostESDataCB2 pFunc, void* pUserData);	
	int			GetStreamInfo(TS_StreamInfo* pInfo);
	int			GetStreamBitrate(double& dBitrate);
	int			StopStreaming();
	int			ClearPoolData();
	int			GetStreamInfoNotifyCB(void** pFunc, void** pUserData);
	//static		int PSIInfoNotifyCB(TS_StreamInfo* pInfo, int nState, void* pParam);	
	void		ReStartStreaming();
	void		SetChannelName(LPCTSTR lpChannelName);
private:
	IStreamProcess *m_TsStreamProcess;
};