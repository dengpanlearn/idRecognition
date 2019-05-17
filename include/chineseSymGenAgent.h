/*
*chineseSymGenAgent.h
*/


#ifndef __CHINESE_SYM_GEN_AGENT_H__
#define __CHINESE_SYM_GEN_AGENT_H__

#include <qstring.h>
#include <qimage.h>
#include <qlist.h>
#include <dp.h>
#include <QObject>
#include "global.h"
#include "chineseSymbolGenerator.h"
#include "qtObjectAgent.h"

class CChineseSymGenAgent : public CQtObjectAgent
{
	Q_OBJECT

public:
	CChineseSymGenAgent(QObject *parent, QThread* pServerObj);
	~CChineseSymGenAgent();

protected:
	virtual void OnInit(QThread* pServerObj);
	virtual void OnClose();

protected:
	virtual BOOL PrepareWork(void const* pParam);
	virtual void DoingWork();
	virtual void StoppingWork();
	virtual void ExitingWork();
	virtual void OnWorkResult(BOOL bSuccess);

protected:
	virtual void OnCreateSymGenerator(QList<QImage*>& symImagesPerGroup, int symsPerGroup);
	virtual void OnSymGeneratorPerGroup(QList<QImage*>& symImagesPerGroup, int groupNo);


private:
	CSymbolGenerator* m_pChineseSymGen;
	char m_dirOut[ID_RECOGNITION_NAME_MAX];
	int	m_genGroups;
	int m_genSymsPerGroup;
	QList<QImage*> m_symImagesPerGroup;
	int  m_curGroupIdx;
};


#endif // !__CHINESE_SYM_GEN_AGENT_H__