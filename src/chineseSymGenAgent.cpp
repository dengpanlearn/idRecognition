#include "../include/qtGlobal.h"
#include "../include/qtGlobal.h"
#include "chineseSymGenAgent.h"

#define CHINESE_SYMBOL_IMAGE_WIDTH	55
#define CHINESE_SYMBOL_IMAGE_HEIGHT	55

CChineseSymGenAgent::CChineseSymGenAgent(QObject *parent, QThread* pServerObj)
	: CQtObjectAgent(parent, pServerObj)
{
	m_pChineseSymGen = NULL;
	m_genGroups = 0;
	m_genSymsPerGroup = 0;
	m_curGroupIdx = 0;
}

CChineseSymGenAgent::~CChineseSymGenAgent()
{
	OnClose();
}
 
void CChineseSymGenAgent::OnInit(QThread* pServerObj)
{
	CQtObjectAgent::OnInit(pServerObj);
	connect(pServerObj, SIGNAL(finished()), this, SLOT(deleteLater()));
	pServerObj->setStackSize(CHINESE_SYM_GEN_TASK_ST);
}

void CChineseSymGenAgent::OnClose()
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

BOOL CChineseSymGenAgent::PrepareWork(void const* pParam)
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
	param.symbolSizeMin = 55;
	param.symbolSizeMax = 55;
	param.sizeUnit = 2;
	param.symbolStart = 0xB0A0;
	param.symbolEnd = 0xF7FE;


	m_pChineseSymGen = new CChineseSymbolGenerator(&param);
	if (m_pChineseSymGen == NULL)
		return FALSE;

	QString* pOutDir = (QString*)pParam;
	memset(m_dirOut, 0, sizeof(m_dirOut));
	QString2Char(*pOutDir, m_dirOut);


	m_pChineseSymGen->InitSymbolGenerator(&m_genGroups, &m_genSymsPerGroup);
	OnCreateSymGenerator(m_symImagesPerGroup, m_genSymsPerGroup);
	return TRUE;
}

void CChineseSymGenAgent::DoingWork()
{
	if (m_pChineseSymGen->NextSymbolGroup(m_symImagesPerGroup))
	{
		OnSymGeneratorPerGroup(m_symImagesPerGroup, m_curGroupIdx++);
		NotifyContinueWork((UINT)(m_curGroupIdx * 100) / m_genGroups);
	}
	else
	{
		CQtObjectAgent::DoingWork();
	}
}

void CChineseSymGenAgent::StoppingWork()
{
	CQtObjectAgent::StoppingWork();
}

void CChineseSymGenAgent::ExitingWork()
{
	CQtObjectAgent::ExitingWork();
}

void CChineseSymGenAgent::OnWorkResult(BOOL bSuccess)
{
	CQtObjectAgent::OnWorkResult(bSuccess);
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
