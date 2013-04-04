// CTimeCount.h
// ʵ�־�ȷ��ʱ����Եĺ���

#ifndef TC_TIMECOUNT_H
#define TC_TIMECOUNT_H

#define TC_EXPORT_API
#include <winnt.h>
class  TC_EXPORT_API CTimeCount
{
public:
	CTimeCount(void);
	~CTimeCount(void);

	// ����뻹�Ǻ��룬 TRUEΪ���룬 FALSEΪ΢��
	double		GetCountTime(bool bMS);
	// ���ÿ�ʼ��ʱʱ��
	void		Reset(void);

private:
	LARGE_INTEGER m_lgFrequency;
	LARGE_INTEGER m_lgStart;
};

#endif /* TC_TIMECOUNT_H */