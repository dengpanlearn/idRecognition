
#include <qpainter.h>
#include <qfont.h>
#include <qtextcodec.h>
#include <qpen.h>
#include "chineseSymbolGenerator.h"

#define CHINESE_SYMBOL_SIZE_UNIT_DFT		2
#define CHINESE_SYMBOL_ROTATE_UNIT_DFT		2
#define	CHINESE_SYMBOL_ROTATE_MIN			-5
#define	CHINESE_SYMBOL_ROTATE_MAX			5
#define	CHINESE_SYMBOL_SIZE_MIN				12
#define	CHINESE_SYMBOL_SIZE_MAX				16
#define	CHINESE_SYMBOL_GBK_START			0xB0A1
#define	CHINESE_SYMBOL_GBK_END				0xF7FE

#define	CHINESE_SYMBOL_GBK_FIRST_MIN			0xB0
#define	CHINESE_SYMBOL_GBK_FIRST_MAX			0xF7
#define	CHINESE_SYMBOL_GBK_SECOND_MIN			0xA1
#define	CHINESE_SYMBOL_GBK_SECOND_MAX			0xFE

CChineseSymbolGenerator::CChineseSymbolGenerator(SymbolGeneratorParam* pParam):CSymbolGenerator(pParam)
{
}


CChineseSymbolGenerator::~CChineseSymbolGenerator()
{
}

UINT CChineseSymbolGenerator::InitGenerator(SymbolGeneratorParam* pParam, int* pSymbolGroups, int* pSymbolsPerGroup)
{
	if (pParam->rotateMin >= pParam->rotateMax)
	{
		pParam->rotateMin = CHINESE_SYMBOL_ROTATE_MIN;
		pParam->rotateMax = CHINESE_SYMBOL_ROTATE_MAX;
	}

	if (pParam->symbolStart >= pParam->symbolEnd)
	{
		pParam->symbolStart = CHINESE_SYMBOL_GBK_START;
		pParam->symbolEnd = CHINESE_SYMBOL_GBK_END;
	}

	if (pParam->symbolSizeMin >= pParam->symbolSizeMax)
	{
		pParam->symbolSizeMin = CHINESE_SYMBOL_SIZE_MIN;
		pParam->symbolSizeMax = CHINESE_SYMBOL_SIZE_MAX;
	}

	if (pParam->sizeUnit <= 0)
		pParam->sizeUnit = CHINESE_SYMBOL_SIZE_UNIT_DFT;


	if (pParam->rotateUnit <= 0)
		pParam->rotateUnit = CHINESE_SYMBOL_ROTATE_UNIT_DFT;

	UINT firstHalfStart = (pParam->symbolStart & 0xff00) >> 8;
	UINT firstHalfEnd = (pParam->symbolEnd & 0xff00) >> 8;
	UINT secondHalfStart = pParam->symbolStart & 0xff ;
	UINT secondHalfEnd = pParam->symbolEnd & 0xff;

	if (firstHalfEnd < firstHalfStart)
	{
		firstHalfStart = (pParam->symbolStart & 0xff00) >> 8;
		firstHalfEnd = (pParam->symbolEnd & 0xff00) >> 8;
		secondHalfStart = pParam->symbolStart & 0xff;
		secondHalfEnd = pParam->symbolEnd & 0xff;
	}

	if (firstHalfStart < CHINESE_SYMBOL_GBK_FIRST_MIN)
	{
		firstHalfStart = CHINESE_SYMBOL_GBK_FIRST_MIN;
	}

	if (firstHalfEnd > CHINESE_SYMBOL_GBK_FIRST_MAX)
	{
		firstHalfEnd = CHINESE_SYMBOL_GBK_FIRST_MAX;
	}

	if (secondHalfStart < CHINESE_SYMBOL_GBK_SECOND_MIN)
	{
		secondHalfStart = CHINESE_SYMBOL_GBK_SECOND_MIN;
	}

	if (secondHalfStart > CHINESE_SYMBOL_GBK_SECOND_MAX)
	{
		secondHalfStart = CHINESE_SYMBOL_GBK_SECOND_MAX;
	}

	if (secondHalfEnd > CHINESE_SYMBOL_GBK_SECOND_MAX)
	{
		secondHalfEnd = CHINESE_SYMBOL_GBK_SECOND_MAX;
	}


	if (secondHalfEnd < CHINESE_SYMBOL_GBK_SECOND_MIN)
	{
		secondHalfEnd = CHINESE_SYMBOL_GBK_SECOND_MIN;
	}

	pParam->symbolStart = secondHalfStart | (firstHalfStart << 8);
	pParam->symbolEnd = secondHalfEnd | (firstHalfEnd << 8);

	int domains = (firstHalfEnd - firstHalfStart + 1);
	int symbolsPerDomain = CHINESE_SYMBOL_GBK_SECOND_MAX - CHINESE_SYMBOL_GBK_SECOND_MIN + 1;

	int firstDomainSymbols = (CHINESE_SYMBOL_GBK_SECOND_MAX - secondHalfStart +1);


	int lastDomainSymbols = secondHalfEnd - CHINESE_SYMBOL_GBK_SECOND_MIN +1;
	
	if (domains == 1)
		pSymbolGroups[0] = lastDomainSymbols;
	else
		pSymbolGroups[0] = firstDomainSymbols + lastDomainSymbols + (domains - 2)*symbolsPerDomain;

	int rotateTimes = (pParam->rotateMax - pParam->rotateMin) / pParam->rotateUnit+1;
	int sizeTimes = (pParam->symbolSizeMax - pParam->symbolSizeMin) / pParam->sizeUnit+1;
	pSymbolsPerGroup[0] = rotateTimes*sizeTimes* (pParam->hasBold ? 2 : 1);

	return (secondHalfStart | (firstHalfStart << 8));
}

UINT CChineseSymbolGenerator::GetNextSymbolValue(UINT curSymbolVaule)
{
	UINT lowHalfGBK = curSymbolVaule & 0xff;
	UINT highHalfGBK = (curSymbolVaule & 0xff00) >> 8;
	lowHalfGBK++;

	 if (lowHalfGBK == 0xff)
	{
		lowHalfGBK = 0xA1;
		highHalfGBK++;
	}

	return ((highHalfGBK << 8) | lowHalfGBK);
}

void CChineseSymbolGenerator::GeneratorNextGroup(SymbolGeneratorParam* pParam, UINT symbolValue, QList<QImage* >& symbolImgs)
{
	int imgIdx = 0;
	symbolValue = ((symbolValue & 0xff) << 8) | ((symbolValue & 0xff00) >> 8);
	for (int fontSize = pParam->symbolSizeMin; fontSize <= pParam->symbolSizeMax; fontSize += pParam->sizeUnit)
	{
		for (int rotate = pParam->rotateMin; rotate <= pParam->rotateMax; rotate += pParam->rotateUnit)
		{
			int hasBold = 0;
			if (pParam->hasBold)
				hasBold++;

			do
			{
				if (imgIdx >= symbolImgs.size())
					break;

				QImage* pImage = symbolImgs[imgIdx++];
				pImage->fill(Qt::white);
				QTextCodec* pTextCodec = QTextCodec::codecForLocale();
				QPainter painter(pImage);
				QPen pen = painter.pen();
			//	QRect rect = pImage->rect();
			//	painter.translate(rect.width()/2, rect.height()/2);
				painter.rotate(rotate);
				pen.setColor(Qt::black);
				QFont font = painter.font();

				font.setFamily("simsun");
				font.setPointSize(fontSize);
				font.setBold(hasBold);

				painter.setPen(pen);
				painter.setFont(font);

				char charBuf[18] = { 0 };
				
				memcpy(charBuf, &symbolValue, 2);
				QString symbString = pTextCodec->toUnicode(charBuf);
				painter.drawText(pImage->rect(), Qt::AlignCenter, symbString);
				
			} while (hasBold--);
		}
	}
}
