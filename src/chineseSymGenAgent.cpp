#include "../include/qtGlobal.h"
#include "chineseSymGenAgent.h"

#define CHINESE_SYMBOL_IMAGE_WIDTH	55
#define CHINESE_SYMBOL_IMAGE_HEIGHT	55

CChineseSymGenAgent::CChineseSymGenAgent(QObject *parent, QThread* pServerTask)
	: QObject(parent)
{
	m_pChineseSymGen = NULL;
	m_genGroups = 0;
	m_genSymsPerGroup = 0;
	m_curGroupIdx = 0;
	m_genStep = 0;
	m_pThreadObj = pServerTask;
	OnInit();
}

CChineseSymGenAgent::~CChineseSymGenAgent()
{
	if (m_pChineseSymGen != NULL)
	{
		delete m_pChineseSymGen;
		m_pChineseSymGen = NULL;
	}

	QList<QImage*>::iterator itor = m_symImagesPerGroup.begin();

	while (itor != m_symImagesPerGroup.end())
	{
		QImage* pImage = itor[0];
		itor = m_symImagesPerGroup.erase(itor);
		delete pImage;
	}
}
 
void CChineseSymGenAgent::OnInit()
{
	connect(this, SIGNAL(SignalStartGen(QString const&)), this, SLOT(OnStartGen(QString const&)));
	connect(this, SIGNAL(SignalStopGen()), this, SLOT(OnStopGen()));
	connect(this, SIGNAL(SignalContinueGen()), this, SLOT(OnContinueGen()));
	connect(this, SIGNAL(SignalThreadExit()), this, SLOT(OnThreadExit()));
	connect(m_pThreadObj, SIGNAL(finished()), this, SLOT(deleteLater()));
}

BOOL CChineseSymGenAgent::StartGen(QString& outDir)
{
	if (!DP_ATOMIC_CAS(m_genStep, SYM_GEN_STEP_NONE, SYM_GEN_STEP_WORKING))
		return FALSE;

	emit SignalStartGen(outDir);
	return TRUE;
} 

void CChineseSymGenAgent::StopGen()
{
	if (!DP_ATOMIC_CAS(m_genStep, SYM_GEN_STEP_WORKING, SYM_GEN_STEP_WAITING_STOP))
		return;

	emit SignalStopGen();
}

void CChineseSymGenAgent::SyncExit(int timeout)
{
	DP_ATOMIC_SET(m_genStep, SYM_GEN_STEP_WAITING_EXIT);
	emit SignalThreadExit();

	if (!m_pThreadObj->wait(timeout))
	{
		m_pThreadObj->quit();
	}
}



void CChineseSymGenAgent::OnStartGen(QString const& outDir)
{
	if (!PrepareStartGen())
	{
		OnGenResult(FALSE);
		return;
	}

	memset(m_dirOut, 0, sizeof(m_dirOut));
	QString2Char(outDir, m_dirOut);


	emit SignalContinueGen();
}


void CChineseSymGenAgent::OnStopGen()
{
	// ¿Õº¯Êý
}

void CChineseSymGenAgent::OnContinueGen()
{
	UINT curStep = DP_ATOMIC_GET(m_genStep);
	if (curStep == SYM_GEN_STEP_WORKING)
	{
		DoContinueGen();
	}
	else if (curStep == SYM_GEN_STEP_WAITING_STOP)
	{
		OnGenResult(TRUE);
	}
}

void CChineseSymGenAgent::OnThreadExit()
{
	m_pThreadObj->quit();
}

BOOL CChineseSymGenAgent::PrepareStartGen()
{
	if (m_pChineseSymGen != NULL)
	{
		delete m_pChineseSymGen;
		m_pChineseSymGen = NULL;
	}

	SymbolGeneratorParam param;
	param.hasBold = 1;
	param.rotateMax = 2;
	param.rotateMin = -2;
	param.rotateUnit = 2;
	param.symbolSizeMin = 26;
	param.symbolSizeMax = 32;
	param.sizeUnit = 2;
	param.symbolStart = 0xB0A0;
	param.symbolEnd = 0xF7FE;


	m_pChineseSymGen = new CChineseSymbolGenerator(&param);
	if (m_pChineseSymGen == NULL)
		return FALSE;


	m_pChineseSymGen->InitSymbolGenerator(&m_genGroups, &m_genSymsPerGroup);
	OnCreateSymGenerator(m_symImagesPerGroup, m_genSymsPerGroup);
	return TRUE;
}

BOOL CChineseSymGenAgent::DoContinueGen()
{
	if (m_pChineseSymGen->NextSymbolGroup(m_symImagesPerGroup))
	{
		OnSymGeneratorPerGroup(m_symImagesPerGroup, m_curGroupIdx++);

			emit SignalGenProgress((UINT)(m_curGroupIdx*100)/m_genGroups);
			emit SignalContinueGen();
		return TRUE;
	}


	OnGenResult(TRUE);
	return TRUE;
}

void CChineseSymGenAgent::OnGenResult(BOOL bSuccess)
{
	emit SignalGenResult(bSuccess);
	DP_ATOMIC_SET(m_genStep, SYM_GEN_STEP_NONE);
}


void CChineseSymGenAgent::OnCreateSymGenerator(QList<QImage*>& symImagesPerGroup, int symsPerGroup)
{
	int curSize = symImagesPerGroup.size();
	if (curSize < symsPerGroup)
	{
		int addSyms = symsPerGroup - curSize;

		for (int i = 0; i < addSyms; i++)
		{
			QImage* pImage = new QImage(CHINESE_SYMBOL_IMAGE_WIDTH, CHINESE_SYMBOL_IMAGE_HEIGHT, QImage::Format_RGB888);
			if (pImage == NULL)
				break;

			symImagesPerGroup.push_back(pImage);
		}
	}
	else if (curSize > symsPerGroup)
	{
		int deleteSyms = curSize - symsPerGroup;

		QList<QImage*>::iterator itor = symImagesPerGroup.begin();

		while (itor != symImagesPerGroup.end() && deleteSyms > 0)
		{
			QImage* pImage = itor[0];
			itor = symImagesPerGroup.erase(itor);
			delete pImage;
		}
	}

	m_curGroupIdx = 0;
}

void CChineseSymGenAgent::OnSymGeneratorPerGroup(QList<QImage*>& symImagesPerGroup, int groupNo)
{
	for (int j = 0; j < symImagesPerGroup.size(); j++)
	{
		QImage* pImage = symImagesPerGroup[j];

		char fileName[ID_RECOGNITION_NAME_MAX] = { 0 };
		sprintf_s(fileName, ID_RECOGNITION_NAME_MAX - 1, "%s\\%d_%d.jpg", m_dirOut, groupNo, j);

		pImage->save(fileName, "jpg");
	}
}
