/*
* charSplit.cpp
*/

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "../include/charSplit.h"

using namespace std;
using namespace cv;

#define CHAR_SPLIT_ROW_LINES_FOR_CHK_START(_rows)		((_rows)/6)
#define CHAR_SPLIT_ROW_LINES_FOR_CHK_END(_rows)			((_rows)/8)
#define CHAR_SPLIT_COL_LINES_FOR_CHK_START(_rows)		((_rows)/6)
#define CHAR_SPLIT_COL_LINES_FOR_CHK_END(_rows)			((_rows)/8)

CCharSplit::CCharSplit(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam):CImageSplit(srcImg, pParam)
{
}


CCharSplit::~CCharSplit()
{
}

BOOL CCharSplit::InitSplit(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam)
{
	if (!pParam->minSplitUnits)
		return FALSE;

	if (pParam->rowLinesForChkStart <= 0)
		pParam->rowLinesForChkStart = CHAR_SPLIT_ROW_LINES_FOR_CHK_START(srcImg.rows);

	if (pParam->rowLinesForChkEnd <= 0)
		pParam->rowLinesForChkEnd = CHAR_SPLIT_ROW_LINES_FOR_CHK_END(srcImg.rows);

	if (pParam->colLinesForChkStart <= 0)
		pParam->colLinesForChkStart = CHAR_SPLIT_COL_LINES_FOR_CHK_START(srcImg.rows);


	if (pParam->colLinesForChkEnd <= 0)
		pParam->colLinesForChkEnd = CHAR_SPLIT_COL_LINES_FOR_CHK_END(srcImg.rows);
	
	if ((pParam->rowLinesForChkStart == 0) || (pParam->rowLinesForChkEnd == 0) || (pParam->colLinesForChkStart == 0)
		|| (pParam->colLinesForChkEnd == 0))
		return FALSE;

	if ((pParam->rowLinesForChkStart >= srcImg.rows) || (pParam->rowLinesForChkEnd >= srcImg.rows))
		return FALSE;

	if ((pParam->colLinesForChkStart >= srcImg.cols) || (pParam->colLinesForChkEnd >= srcImg.cols))
		return FALSE;

	return TRUE;
}

BOOL CCharSplit::SplitWork(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, vector<Vec4i>& childInfos)
{
	if (!SplitCharsByColSeperator(srcImg, pParam, childInfos))
		return FALSE;

	vector<Vec4i>::iterator itor = childInfos.begin();
	while (itor != childInfos.end())
	{
		
		Mat childImg = srcImg(Range::all(), Range(itor->val[2], itor->val[3]));
		Vec2i rowStartEnd;
		if (!ExtractCharInRow(childImg, pParam, rowStartEnd))
		{
			itor = childInfos.erase(itor);
			continue;
		}

		itor->val[0] = rowStartEnd[0];
		itor->val[1] = rowStartEnd[1];
		itor++;
	}

	return TRUE;
}

BOOL CCharSplit::SplitCharsByColSeperator(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, vector<Vec4i>& childInfos)
{
	Vec4i startEndInfo;
	int keepLines = 0;
	int step = CHAR_SPLIT_STEP_FIND_START_LINE;

	for (int i = 0; i < srcImg.cols; i++)
	{
		int j = 0;
		for (; j < srcImg.rows; j++)
		{
			if (!srcImg.at<unsigned char>(j, i))
				break;

			if ((j > 0) && (!srcImg.at<unsigned char>(j - 1, i)))
				break;

		}

		if (step == CHAR_SPLIT_STEP_FIND_START_LINE)
		{
			if (j == srcImg.rows)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= pParam->colLinesForChkStart)
			{
				startEndInfo[2] = (i - keepLines + 1);
				keepLines = 0;
				step = CHAR_SPLIT_STEP_WAIT_END_LINE;
			}
		}
		else
		{
			if (i == (srcImg.cols - 1))
			{
				keepLines++;
				goto _SAVE_START_END;
			}

			if (j < srcImg.rows)
			{
				keepLines = 0;
				continue;
			}


			if (++keepLines < pParam->colLinesForChkEnd)
				continue;

		_SAVE_START_END:
			startEndInfo[3] = i - keepLines + 1 + 1;
			childInfos.push_back(startEndInfo);
			keepLines = 0;
			step = CHAR_SPLIT_STEP_FIND_START_LINE;
		}
	}

	return ((int)childInfos.size() >= pParam->minSplitUnits);
}

BOOL CCharSplit::ExtractCharInRow(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, Vec2i& rowStartEnd)
{
	int keepLines = 0;
	int step = CHAR_SPLIT_STEP_FIND_START_LINE;

	for (int i = 0; i < srcImg.rows; i++)
	{
		int j = 0;

		for (; j < srcImg.cols; j++)
		{
			if (!srcImg.at<unsigned char>(i, j))
				break;

			if ((i > 0) && (!srcImg.at<unsigned char>(i - 1,j)))
				break;

			if ((i < (srcImg.rows - 1)) && (j < (srcImg.cols - 1)) && (!srcImg.at<unsigned char>(i + 1, j + 1)))
				break;
		}

		if (step == CHAR_SPLIT_STEP_FIND_START_LINE)
		{
			if (j == srcImg.cols)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= pParam->rowLinesForChkStart)
			{
				rowStartEnd[0] = (i - keepLines + 1);
				keepLines = 0;
				step = CHAR_SPLIT_STEP_WAIT_END_LINE;
			}
		}
		else
		{
			if (i == (srcImg.rows - 1))
			{
				keepLines++;
				goto _SAVE_START_END;
			}

			if (j < srcImg.cols)
			{
				keepLines = 0;
				continue;
			}


			if (++keepLines < pParam->rowLinesForChkEnd)
				continue;

		_SAVE_START_END:
			rowStartEnd[1] = i - keepLines + 1 + 1;
			return TRUE;
		}
	}


	return FALSE;
}
