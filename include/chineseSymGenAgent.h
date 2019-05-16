/*
*chineseSymGenAgent.h
*/


#ifndef __CHINESE_SYM_GEN_AGENT_H__
#define __CHINESE_SYM_GEN_AGENT_H__

#include <qstring.h>
#include <qimage.h>
#include <qlist.h>
#include <qthread.h>
#include <dp.h>
#include <dpAtomic.h>
#include <QObject>
#include "global.h"
#include "chineseSymbolGenerator.h"


class CChineseSymGenAgent : public QObject
{
	Q_OBJECT

public:
	CChineseSymGenAgent(QObject *parent, QThread* pServerTask);
	~CChineseSymGenAgent();

private:
	void OnInit();

public:
	BOOL StartGen(QString& outDir);
	void StopGen();
	void SyncExit(int timeout);

signals:
	void SignalStartGen(QString const& outDir);
	void SignalStopGen();
	void SignalContinueGen();
	void SignalThreadExit();
	void SignalGenResult(bool bSuccess);
	void SignalGenProgress(int progress);

private slots:
	void OnStartGen(QString const& outDir);
	void OnStopGen();
	void OnContinueGen();
	void OnThreadExit();

protected:
	BOOL PrepareStartGen();
	BOOL DoContinueGen();
	void OnGenResult(BOOL bSuccess);

protected:
	virtual void OnCreateSymGenerator(QList<QImage*>& symImagesPerGroup, int symsPerGroup);
	virtual void OnSymGeneratorPerGroup(QList<QImage*>& symImagesPerGroup, int groupNo);


	enum SYM_GEN_STEP
	{
		SYM_GEN_STEP_NONE = 0x00,
		SYM_GEN_STEP_WORKING = 0x01,
		SYM_GEN_STEP_WAITING_STOP = 0x02,
		SYM_GEN_STEP_WAITING_EXIT = 0x03,
	};

private:
	CSymbolGenerator* m_pChineseSymGen;
	char m_dirOut[ID_RECOGNITION_NAME_MAX];
	int	m_genGroups;
	int m_genSymsPerGroup;
	QList<QImage*> m_symImagesPerGroup;
	int  m_curGroupIdx;
	DP_ATOMIC	m_genStep;

	QThread* m_pThreadObj;
};


#endif // !__CHINESE_SYM_GEN_AGENT_H__