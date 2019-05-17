/*
*qtObjectAgent.h
*qt Object »ùÀà
*/

#include <dp.h>
#include <QObject>
#include <qthread.h>
#include <dpAtomic.h>

class CQtObjectAgent : public QObject
{
	Q_OBJECT

public:
	CQtObjectAgent(QObject *parent, QThread* pThread);
	~CQtObjectAgent();


protected:
	virtual void OnInit(QThread* pServerObj);
	virtual void OnClose();

public:
	void Init();
	BOOL StartWork(void const* pParam);
	void StopWork();
	void SyncExit(int timeout);

signals:
	void SignalStartWork(void const* pParam);
	void SignalStopWork();
	void SignalContinueWork();
	void SignalThreadExit();
	void SignalWorkResult(bool bSuccess);
	void SignalWorkProgress(int progress);

private slots:
	void OnStartWork(void const* pParam);
	void OnStopWork();
	void OnContinueWork();
	void OnThreadExit();

protected:
	void NotifyContinueWork(int progress);

protected:
	virtual BOOL PrepareWork(void const* pParam);
	virtual void DoingWork();
	virtual void StoppingWork();
	virtual void ExitingWork();
	virtual void OnWorkResult(BOOL bSuccess);

private:
	enum SYM_WORK_STEP
	{
		SYM_WORK_STEP_NONE = 0x00,
		SYM_WORK_STEP_WORKING = 0x01,
		SYM_WORK_STEP_WAITING_STOP = 0x02,
		SYM_WORK_STEP_WAITING_EXIT = 0x03,
	};
	DP_ATOMIC	m_workStep;
	QThread*	m_pThreadObj;
};
