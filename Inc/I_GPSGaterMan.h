#pragma once

#ifdef I_GPS_GATER_MAN_EXPORTS
#define I_GPSGATERMAN_API __declspec(dllexport)
#else
#define I_GPSGATERMAN_API __declspec(dllimport)

#endif

//����ΪTSDevice�����ߵĻ��ඨ��
class I_GPSGATERMAN_API I_GPSGaterMan
{
public:
	BOOL static CreateInstance(I_GPSGaterMan **ppGPSGaterMan);
	/*
	 *	��ʼ��
	 */
	virtual BOOL InitGPSGaterMan()=0;
	/*
	 *	����ʼ��
	 */
	virtual BOOL UnInitGPSGaterMan()=0;
	/*
	 *	��ʼ����
	 */
	virtual BOOL StartWork()=0;
	/*
	 * ֹͣ����
	*/
	virtual BOOL StopWork()=0;
};
