#ifndef I_GATE_INTERFACE_DLL_H
#define I_GATE_INTERFACE_DLL_H


#ifdef GATE_INTERFACE_EXPORTS
#define GATE_INTERFACE_API __declspec(dllexport)
#else
#define GATE_INTERFACE_API __declspec(dllimport)
#pragma comment(lib,"GPSGateAdpter.lib")
#endif
#include "GpsData.h"
#include "GpsGateDatadef.h"
extern "C" {
	/*
	 * ��ȡgps����
	 * @param buf  ��������
	 * @param addr udp��ַ
	 * @param cPort udp�˿�
	 * @return long buf�ĳ���  0 Ϊû�б���
	**/
//	GATE_INTERFACE_API long getGPS( char *buf,char *addr,char * cPort);
	GATE_INTERFACE_API long getGPS( GPSGATEDATA *pGpsData,char *buf);

	/*
	 * ��gpsд��������
	 * @param buf  ��������
	 * @param addr udp��ַ
	 * @param cport udp�˿�
	 * @return long   >0 ���ͳɹ�  <1 ����ʧ��
	**/
//	GATE_INTERFACE_API long writeGPS(const char *buf,const char *addr,const char * cPort);
	GATE_INTERFACE_API long writeGPS(const GPSGATEDATA * pGpsData,char *pDatabuf,int nDataLen);

	/*
	 * ��������ϵ���Ϣ д�����ݿ�
	 * @param pGpsInfo
	 * @return  >0 д��ɹ�  <1 д��ʧ��
	**/
	GATE_INTERFACE_API int writedb(const GPSINFO *pGpsInfo);

	/*
	 * ��ȡӦ�ò� ��gps�·�������
	 * @param pGpsInfo
	 * @return >0 ��ȡ�ɹ�  <1 ��ȡʧ��
	**/
	GATE_INTERFACE_API int getCmd(GPSCommand *pGpsCommand);

	/*
	 * web ��gpsGater�·�������ָ��
	 * @param pGpsInfo
	 * @return >0 ��ȡ�ɹ�  <1 ��ȡʧ��
	**/
	GATE_INTERFACE_API int WriteCommand(GPSCommand *pGpsCommand);



};
#endif

//
/*
 long GATE_INTERFACE_API getGPS(const char *buf,const char *addr,int nPort);

/**/