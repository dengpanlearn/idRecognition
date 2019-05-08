/*
*symbolGenerator.h
* 符号生成接口基类
*
*/


#ifndef __SYMBOL_GENERATOR_H__
#define __SYMBOL_GENERATOR_H__
#include <dp.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qimage.h>

struct SymbolGeneratorParam
{
	UINT	symbolStart;
	UINT	symbolEnd;
	int		rotateMin;
	int		rotateMax;
	int		rotateUnit;
	int		symbolSizeMin;
	int		symbolSizeMax;
	int		sizeUnit;
	BOOL	hasBold;
};

class CSymbolGenerator
{
public:
	CSymbolGenerator(SymbolGeneratorParam* pParam);
	virtual ~CSymbolGenerator();

public:
	void InitSymbolGenerator(int* pSymbolGroups, int* pSymbolsPerGroup);
	BOOL NextSymbolGroup(QList<QImage* >&symbolImgs);

protected:
	virtual UINT InitGenerator(SymbolGeneratorParam* pParam, int* pSymbolGroups, int* pSymbolsPerGroup)=0;
	virtual UINT GetNextSymbolValue(UINT curSymbolVaule)=0;
	virtual void GeneratorNextGroup(SymbolGeneratorParam* pParam, UINT symbolValue, QList<QImage* >& symbolImgs) = 0;

private:
	SymbolGeneratorParam m_param;
	UINT m_curSymbolVal;

};


#endif // !__SYMBOL_GENERATOR_H__


