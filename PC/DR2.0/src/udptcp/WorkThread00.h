#pragma once
#include "./definc.h"

class MyLK
{
public:
	MyLK();
	virtual ~MyLK();
	BOOL	lk();
	void	ulk();
#ifdef USEWINDOW
	CRITICAL_SECTION lk_mtx;
#else
	pthread_mutex_t	lk_mtx;
#endif
};
class WorkThreadEx;
class IMyWork
{
public:
	virtual BOOL __stdcall OnSetStop(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData) = 0;
	virtual BOOL __stdcall 	OnSetStart(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData) = 0;
	virtual BOOL __stdcall DoWork(WorkThreadEx * pWork, DWORD dwIndex) = 0;
};

class Work_ThreadS
	:public IMyWork
	, public MyLK
{
public:
	Work_ThreadS();
	virtual ~Work_ThreadS();
	virtual BOOL __stdcall 			OnSetStop(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData){ return TRUE; };
	virtual BOOL __stdcall 			OnSetStart(WorkThreadEx * pWork, DWORD dwIndex, DWORD dwData){ return TRUE; };
	virtual BOOL __stdcall 			DoWork(WorkThreadEx * pWork, DWORD dwIndex){ return TRUE; };

	WorkThreadEx * 					WorkInThread(DWORD	dwCmd,
		LPVOID	inPm,
		DWORD	inPmLen,
		LPVOID	bkPm,
		DWORD	bkPmLen);
private:
	void	* m_pvlists;
};



//////////////////////////////////////////////
class WorkThreadEx;
void api_stopworkex(WorkThreadEx * pe, DWORD dwreult, DWORD dwwait);
void api_freeworkex(WorkThreadEx * &pe);
BOOL api_isneedwork(WorkThreadEx * pe, DWORD dwwait);