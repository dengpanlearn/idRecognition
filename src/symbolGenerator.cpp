/*
*symbolGenerator.cpp
*/

#include <qpainter.h>
#include "symbolGenerator.h"



CSymbolGenerator::CSymbolGenerator(SymbolGeneratorParam* pParam)
{
	m_curSymbolVal = 0;
	memcpy(&m_param, pParam, sizeof(SymbolGeneratorParam));
}


CSymbolGenerator::~CSymbolGenerator()
{

}

void CSymbolGenerator::InitSymbolGenerator(int* pSymbolGroups, int* pSymbolsPerGroup)
{
	m_curSymbolVal = InitGenerator(&m_param, pSymbolGroups, pSymbolsPerGroup);
}

BOOL CSymbolGenerator::NextSymbolGroup(QList<QImage* >& symbolImgs)
{
	if (m_curSymbolVal > m_param.symbolEnd)
		return FALSE;

	GeneratorNextGroup(&m_param, m_curSymbolVal, symbolImgs);

	m_curSymbolVal = GetNextSymbolValue(m_curSymbolVal);
	
	return TRUE;
}