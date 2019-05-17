/*
* qtObjectAgent.cpp
*/
#include "../include/qtObjectAgent.h"

CQtObjectAgent::CQtObjectAgent(QObject *parent, QThread* pThreadObj)
	: QObject(parent)
{
	m_workStep = 0;
	m_pThreadObj = pThreadObj;
}

CQtObjectAgent::~CQtObjectAgent()
{

}

void CQtObjectAgent::OnInit(QThread* pServerObj)
{
	connect(this, SIGNAL(SignalStartWork(void const*)), this, SLOT(OnStartWork(void const*)));
	connect(this, SIGNAL(SignalStopWork()), this, SLOT(OnStopWork()));
	connect(this, SIGNAL(SignalContinueWork()), this, SLOT(OnContinueWork()));
	connect(this, SIGNAL(SignalThreadExit()), this, SLOT(OnThreadExit()));
	connect(pServerObj, SIGNAL(finished()), pServerObj, SLOT(deleteLater()));
}

void CQtObjectAgent::OnClose()
{

}

void CQtObjectAgent::Init()
{

	OnInit(m_pThreadObj);
}

BOOL CQtObjectAgent::StartWork(void const* pParam)
{
	if (!DP_ATOMIC_CAS(m_workStep, SYM_WORK_STEP_NONE, SYM_WORK_STEP_WORKING))
		return FALSE;

	emit SignalStartWork(pParam);
	return TRUE;
}

void CQtObjectAgent::StopWork()
{
	if (!DP_ATOMIC_CAS(m_workStep, SYM_WORK_STEP_WORKING, SYM_WORK_STEP_WAITING_STOP))
		return;

	emit SignalStopWork();
}

void CQtObjectAgent::SyncExit(int timeout)
{
	if (!m_pThreadObj->isRunning())
		return;

	DP_ATOMIC_SET(m_workStep, SYM_WORK_STEP_WAITING_EXIT);
	emit SignalThreadExit();

	if (!m_pThreadObj->wait(timeout))
	{
		m_pThreadObj->quit();
	}
}

void CQtObjectAgent::OnStartWork(void const* pParam)
{
	if (!PrepareWork(pParam))
	{
		DP_ATOMIC_SET(m_workStep, SYM_WORK_STEP_NONE);
		OnWorkResult(FALSE);
		return;
	}

	emit SignalContinueWork();
}

void CQtObjectAgent::OnStopWork()
{
	
}

void CQtObjectAgent::OnContinueWork()
{
	UINT curStep = DP_ATOMIC_GET(m_workStep);
	
	if (curStep == SYM_WORK_STEP_WORKING)
	{
		DoingWork();
	}
	else if (curStep == SYM_WORK_STEP_WAITING_STOP)
	{
		StoppingWork();
	}
}

void CQtObjectAgent::OnThreadExit()
{
	ExitingWork();
}

void CQtObjectAgent::NotifyContinueWork(int progress)
{
	emit SignalWorkProgress(progress);
	emit SignalContinueWork();
}

BOOL CQtObjectAgent::PrepareWork(void const* pParam)
{
	return TRUE;
}

void CQtObjectAgent::DoingWork()
{
	DP_ATOMIC_SET(m_workStep, SYM_WORK_STEP_NONE);
	OnWorkResult(TRUE);
}

void CQtObjectAgent::StoppingWork()
{
	DP_ATOMIC_SET(m_workStep, SYM_WORK_STEP_NONE);
	OnWorkResult(TRUE);
}

void CQtObjectAgent::ExitingWork()
{
	m_pThreadObj->quit();
}

void CQtObjectAgent::OnWorkResult(BOOL bSuccess)
{
	emit SignalWorkResult(bSuccess);
}