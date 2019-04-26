/*
*idCardRecognition.h
* 身份证识别类
*/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "../include/idCardRecognition.h"

using namespace std;
using namespace cv;

// threshold for id card image
#define	ID_CARD_IMAGE_THRESHOLD_TO_BIN							120		

#define	ID_CARD_NO_ROWS_MIN(_imgrows)							((_imgrows)/3)
#define	ID_CARD_NO_MIN_POINTS_PER_ROWLINE(_imgcols)				((_imgcols)/12)
#define	ID_CARD_NO_MIN_ROWLINES_FOR_CHECK_START							(30)
#define	ID_CARD_NO_MIN_ROWLINES_FOR_CHECK_END							(30)
#define	ID_CARD_NO_MIN_COLLINES_FOR_CHECK_START(charRows)				((charRows)/6)
#define	ID_CARD_NO_MIN_COLLINES_FOR_CHECK_END(charRows)					((charRows)/8)
#define ID_CARD_NO_MIN_CHARS											20
#define	ID_CARD_NO_CHARS_MAX_BETWEENS(charRows)							((charRows)/2)
#define ID_CARD_NO_MIN_CHARS_FOR_CHECK									3

#define ID_CARD_CHAR_MIN_ROWSLINES_FOR_CHECK_START(charCols)			((charCols)/6)
#define ID_CARD_CHAR_MIN_ROWSLINES_FOR_CHECK_END(charCols)				((charCols)/8)
#define ID_CARD_CHAR_GROUPS_WITHOUT_CARD_NO								4

#define ID_CARD_COMPENSATE_POINTS_FOR_COL_ROW(charCols)					((charCols)/6)

#define ID_CARD_HEAD_IMG_COLS_MAX(_imgcols)								((_imgcols)/3)
#define	ID_CARD_HEAD_IMG_MIN_COLLINES_FOR_CHECK_START						(15)
#define	ID_CARD_HEAD_IMG_MIN_COLLINES_FOR_CHECK_END							(15)
#define	ID_CARD_HEAD_IMG_MIN_ROWLINES_FOR_CHECK_START						(15)
#define	ID_CARD_HEAD_IMG_MIN_ROWLINES_FOR_CHECK_END							(15)

CIdCardRecognition::CIdCardRecognition()
{

}

CIdCardRecognition::~CIdCardRecognition()
{

}

BOOL CIdCardRecognition::FilterImg(Mat& srcImg, Mat& destImg)
{
	return TRUE;
}

BOOL CIdCardRecognition::ThresholdImg(Mat& srcImg, Mat& destImg)
{
	threshold(srcImg, destImg, ID_CARD_IMAGE_THRESHOLD_TO_BIN, 255, THRESH_BINARY);
	return TRUE;
}

BOOL CIdCardRecognition::ExtractUsefulParts(Mat& srcImg, vector<CImageSplit*>& partsForSplit)
{
	Vec4i cardNoInfo;
	Vec2i cardNoFirstCharInfo;
	if (!ExtractIdCardNo(srcImg, cardNoInfo, cardNoFirstCharInfo))
		return FALSE;

	int startRow = CalcRowStartByIdCarNo(srcImg, cardNoInfo, cardNoFirstCharInfo);
	if (startRow < 0)
		return FALSE;

	int compensates = ID_CARD_COMPENSATE_POINTS_FOR_COL_ROW(cardNoFirstCharInfo[1] - cardNoFirstCharInfo[0]);
	if (startRow > compensates)
		startRow -= compensates;

	if (cardNoInfo[2] > compensates)
		cardNoInfo[2] -= compensates;
#if 1
	Mat usefulImg = srcImg(Range(startRow, cardNoInfo[1]), Range(cardNoInfo[2], cardNoInfo[3]));
	
	namedWindow("useful img", WINDOW_NORMAL);
	imshow("useful img", usefulImg);

	Vec4i headImgInfo;
	Mat noneCardNoImg = srcImg(Range(startRow, cardNoInfo[0]), Range(cardNoInfo[2], cardNoInfo[3]));
	if (ExtractIdCardHeadImg(noneCardNoImg, headImgInfo))
	{
		namedWindow("head img", WINDOW_NORMAL);
		imshow("head img", noneCardNoImg(Range(headImgInfo[0], headImgInfo[1]), Range(headImgInfo[2], headImgInfo[3])));

		vector<Vec2i> charGroupsInfo;
		Mat groupsImg = noneCardNoImg(Range::all(), Range(0, headImgInfo[2]));

		if (ExtractIdCardCharGroups(groupsImg, charGroupsInfo, cardNoFirstCharInfo[1]- cardNoFirstCharInfo[0]))
		{
			for (int k = 0; k < charGroupsInfo.size(); k++)
			{
				char windName[32] = { 0 };
				sprintf(windName, "char%d",k);
				namedWindow(windName, WINDOW_NORMAL);

				imshow(windName, groupsImg(Range(charGroupsInfo[k][0], charGroupsInfo[k][1]), Range::all()));
			}
		}
	
	
	}
	waitKey(0);
#endif
	return TRUE;
}

BOOL CIdCardRecognition::ExtractIdCardNo(Mat& orgImg, Vec4i& cardNoInfo, Vec2i& firstCharInfo)
{
	Vec2i rowStartEnd;
	if (!ExtractIdCardNoRows(orgImg, rowStartEnd))
		return FALSE;

	Vec2i colStartEnd;
	Mat cardNoRowsImg;
	cardNoRowsImg = orgImg(Range(rowStartEnd[0], rowStartEnd[1]), Range::all());
	if (!ExtractIdCardNoCols(cardNoRowsImg, colStartEnd, firstCharInfo))
		return FALSE;

	cardNoInfo[0] = rowStartEnd[0];
	cardNoInfo[1] = rowStartEnd[1];

	cardNoInfo[2] = colStartEnd[0];
	cardNoInfo[3] = colStartEnd[1];
	return TRUE;
}

void CIdCardRecognition::OnExtractUsefulPartImg(Mat& partImg, int partIdx)
{

}

BOOL CIdCardRecognition::SplitUsefulPart(CImageSplit* pPartForSplit, vector<Vec4i>& splitInfo)
{
	return TRUE;
}

void CIdCardRecognition::OnSplitUsefulPartImg(Mat& splitImg, int partIdx, int splitIdx)
{

}

BOOL CIdCardRecognition::Recogniton(CImageSplit* pPartForSplit)
{
	return TRUE;
}

BOOL CIdCardRecognition::ExtractIdCardNoRows(Mat& orgImg, Vec2i& rowStartEnd)
{
	int i = orgImg.rows - 1;
	int keepLines = 0;
	int step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;
	for (; i > ID_CARD_NO_ROWS_MIN(orgImg.rows); i--)
	{
		int onPoints = 0;
		for (int j = 0; j < orgImg.cols; j++)
		{
			if (!orgImg.at<unsigned char>(i, j))
				goto _ON_POINTS;

			if ((i > 0) && (!orgImg.at<unsigned char>(i - 1, j)))
				goto _ON_POINTS;

			if ((i > 0) && (j > 0) && (!orgImg.at<unsigned char>(i - 1, j - 1)))
				goto _ON_POINTS;

			continue;

		_ON_POINTS:
			onPoints++;
		}

		if (step == EXTRACT_ID_CARD_STEP_FIND_START_PIX)
		{
			if (onPoints < ID_CARD_NO_MIN_POINTS_PER_ROWLINE(orgImg.cols))
			{
				keepLines = 0;
				continue;
			}

			keepLines++;
			if (keepLines >= ID_CARD_NO_MIN_ROWLINES_FOR_CHECK_START)
			{
				rowStartEnd[1] = i - 1 + keepLines + 1;
				step = EXTRACT_ID_CARD_STEP_WAIT_END_PIX;
				keepLines = 0;
			}
		}
		else
		{
			if (onPoints >= ID_CARD_NO_MIN_POINTS_PER_ROWLINE(orgImg.cols))
			{
				keepLines = 0;
				continue;
			}

			keepLines++;
			if (keepLines >= ID_CARD_NO_MIN_ROWLINES_FOR_CHECK_END)
			{
				rowStartEnd[0] = i - 1 + keepLines;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CIdCardRecognition::ExtractIdCardNoCols(Mat& orgImg, Vec2i& colStartEnd, Vec2i& firstChar)
{
	int keepLines = 0;
	int step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;
	vector<Vec2i> charStartEndList;

	Vec2i charStartEndPos;
	for (int i = 0; i < orgImg.cols; i++)
	{
		int j = 0;
		for (; j < orgImg.rows; j++)
		{
			if (!orgImg.at<unsigned char>(j, i))
				break;

			if ((j > 0) && (!orgImg.at<unsigned char>(j - 1, i)))
				break;

			if ((i < (orgImg.cols - 1)) && (j > 0) && (!orgImg.at<unsigned char>(j - 1, i + 1)))
				break;
		}

		if (step == EXTRACT_ID_CARD_STEP_FIND_START_PIX)
		{
			if (j == orgImg.rows)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_NO_MIN_COLLINES_FOR_CHECK_START(orgImg.rows))
			{
				charStartEndPos[0] = (i - keepLines + 1);
				keepLines = 0;
				step = EXTRACT_ID_CARD_STEP_WAIT_END_PIX;
			}
		}
		else
		{
			if (i == (orgImg.cols - 1))
			{
				keepLines++;
				goto _SAVE_START_END;
			}

			if (j < orgImg.rows)
			{
				keepLines = 0;
				continue;
			}


			if (++keepLines < ID_CARD_NO_MIN_COLLINES_FOR_CHECK_END(orgImg.rows))
				continue;

		_SAVE_START_END:
			charStartEndPos[1] = i - keepLines + 1+1;
			charStartEndList.push_back(charStartEndPos);
			keepLines = 0;
			step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;
		}
	}

	if (charStartEndList.size() < ID_CARD_NO_MIN_CHARS)
		return FALSE;

	int startIdex = 0;
	int checkIdNoCharsTimes = 0;
	for (int i = 1; i < charStartEndList.size(); i++)
	{
		int betweens = charStartEndList[i][0] - charStartEndList[i - 1][1];
		if (betweens > ID_CARD_NO_CHARS_MAX_BETWEENS(orgImg.rows))
		{
			startIdex = i;
			checkIdNoCharsTimes = 0;
		}

		if (++checkIdNoCharsTimes >= ID_CARD_NO_MIN_CHARS_FOR_CHECK)
		{
			colStartEnd[0] = charStartEndList[startIdex][0];
			firstChar[0] = colStartEnd[0];
			firstChar[1] = charStartEndList[startIdex][1];
			break;
		}
	}

	startIdex = (int)charStartEndList.size() - 1;
	checkIdNoCharsTimes = 0;
	for (int i = startIdex - 1; i >= 0; i--)
	{
		int betweens = charStartEndList[i + 1][0] - charStartEndList[i][1];
		if (betweens > ID_CARD_NO_CHARS_MAX_BETWEENS(orgImg.rows))
		{
			startIdex = i;
			checkIdNoCharsTimes = 0;
		}

		if (++checkIdNoCharsTimes >= ID_CARD_NO_MIN_CHARS_FOR_CHECK)
		{
			colStartEnd[1] = charStartEndList[startIdex][1];
			break;
		}
	}

	return (colStartEnd[1] > colStartEnd[0]);
}

int CIdCardRecognition::CalcRowStartByIdCarNo(Mat& orgImg, Vec4i& cardNoInfo, Vec2i& firstCharInfo)
{
	int keepLines = 0;
	int step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;

	vector<Vec2i> charsStartEnd;
	Vec2i startEndPos;
	Mat partImg = orgImg(Range(0, cardNoInfo[0]), Range(firstCharInfo[0], firstCharInfo[1]));

	for (int i = partImg.rows - 1; i >= 0; i--)
	{
		int j = 0;

		for (; j < partImg.cols; j++)
		{
			if (!partImg.at<unsigned char>(i, j))
				break;

			if ((i > 0) && (!partImg.at<unsigned char>(i - 1, j)))
				break;

			if ((i < (partImg.rows - 1)) && (j < (partImg.cols - 1)) && (!partImg.at<unsigned char>(i + 1, j + 1)))
				break;
		}

		if (step == EXTRACT_ID_CARD_STEP_FIND_START_PIX)
		{
			if (j == partImg.cols)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_CHAR_MIN_ROWSLINES_FOR_CHECK_START(partImg.cols))
			{
				startEndPos[1] = (i-1+keepLines+1);
				keepLines = 0;
				step = EXTRACT_ID_CARD_STEP_WAIT_END_PIX;
			}
		}
		else
		{
			if (i == 0)
			{
				keepLines++;
				goto _SAVE_START_END;
			}

			if (j < partImg.cols)
			{
				keepLines = 0;
				continue;
			}


			if (++keepLines < ID_CARD_CHAR_MIN_ROWSLINES_FOR_CHECK_END(partImg.cols))
				continue;

		_SAVE_START_END:
			startEndPos[0] = i + keepLines - 1;
			charsStartEnd.push_back(startEndPos);
			keepLines = 0;
			step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;
		}
	}

	if (charsStartEnd.size() < ID_CARD_CHAR_GROUPS_WITHOUT_CARD_NO)
		return -1;


	return charsStartEnd[ID_CARD_CHAR_GROUPS_WITHOUT_CARD_NO - 1][0];
}

BOOL CIdCardRecognition::ExtractIdCardHeadImg(Mat& noneCardNoImg, Vec4i& headImgInfo)
{
	Vec2i colStartEnd;
	if (!ExtractIdCardHeadImgCols(noneCardNoImg, colStartEnd))
		return FALSE;

	Vec2i rowStartEnd;
	if (!ExtractIdCardHeadImgRows(noneCardNoImg(Range::all(), Range(colStartEnd[0], colStartEnd[1])), rowStartEnd))
		return FALSE;

	headImgInfo[0] = rowStartEnd[0];
	headImgInfo[1] = rowStartEnd[1];
	headImgInfo[2] = colStartEnd[0];
	headImgInfo[3] = colStartEnd[1];
	return TRUE;
}

BOOL CIdCardRecognition::ExtractIdCardHeadImgCols(Mat& noneCardNoImg, Vec2i& colStartEnd)
{
	int keepLines = 0;
	int step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;

	for (int i = noneCardNoImg.cols - 1; i > ID_CARD_HEAD_IMG_COLS_MAX(noneCardNoImg.cols); i--)
	{
		int j = 0;
		for (; j < noneCardNoImg.rows; j++)
		{

			if (!noneCardNoImg.at<unsigned char>(j, i))
				break;

			if ((i > 0) && (!noneCardNoImg.at<unsigned char>(j, i-1)))
				break;

			if ((i < (noneCardNoImg.cols - 1)) && (j < (noneCardNoImg.rows - 1)) && (!noneCardNoImg.at<unsigned char>(j + 1, i + 1)))
				break;
		}

		if (step == EXTRACT_ID_CARD_STEP_FIND_START_PIX)
		{
			if (j == noneCardNoImg.rows)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_HEAD_IMG_MIN_COLLINES_FOR_CHECK_START)
			{
				colStartEnd[1] = (i -1 + keepLines + 1);
				keepLines = 0;
				step = EXTRACT_ID_CARD_STEP_WAIT_END_PIX;
			}
		}
		else
		{
			if (j < noneCardNoImg.rows)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_HEAD_IMG_MIN_COLLINES_FOR_CHECK_END)
			{
				colStartEnd[0] = (i - 1 + keepLines);
				keepLines = 0;
				return TRUE;
			}
		}
	}


	return FALSE;
}

BOOL CIdCardRecognition::ExtractIdCardHeadImgRows(Mat& srcImg, Vec2i& rowStartEnd)
{
	int keepLines = 0;
	int step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;

	for (int i = 0; i < srcImg.rows; i++)
	{
		int j = 0;
		for (; j < srcImg.cols; j++)
		{
			if (!srcImg.at<unsigned char>(i, j))
				break;

			if ((i > 0) && (!srcImg.at<unsigned char>(i-1, j)))
				break;

			if ((i < (srcImg.rows - 1)) && (j < (srcImg.cols - 1)) && (!srcImg.at<unsigned char>(i + 1, j + 1)))
				break;
		}

		if (step == EXTRACT_ID_CARD_STEP_FIND_START_PIX)
		{
			if (j == srcImg.cols)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_HEAD_IMG_MIN_ROWLINES_FOR_CHECK_START)
			{
				rowStartEnd[0] = (i - keepLines + 1);
				keepLines = 0;
				step = EXTRACT_ID_CARD_STEP_WAIT_END_PIX;
			}
		}
		else
		{
			if (j < srcImg.cols)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_HEAD_IMG_MIN_ROWLINES_FOR_CHECK_END)
			{
				rowStartEnd[1] = (i - keepLines+1+1);
				keepLines = 0;
				return TRUE;
			}
		}

	}

	return FALSE;
}

BOOL CIdCardRecognition::ExtractIdCardCharGroups(Mat& srcImg, vector<Vec2i>& groupInfo, int charCols)
{
	int keepLines = 0;
	int step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;


	Vec2i startEndPos;

	for (int i = 0; i < srcImg.rows; i++)
	{
		int j = 0;

		for (; j < srcImg.cols; j++)
		{
			if (!srcImg.at<unsigned char>(i, j))
				break;

			if ((i > 0) && (!srcImg.at<unsigned char>(i - 1, j)))
				break;

			if ((i < (srcImg.rows - 1)) && (j < (srcImg.cols - 1)) && (!srcImg.at<unsigned char>(i + 1, j + 1)))
				break;
		}

		if (step == EXTRACT_ID_CARD_STEP_FIND_START_PIX)
		{
			if (j == srcImg.cols)
			{
				keepLines = 0;
				continue;
			}

			keepLines++;

			if (keepLines >= ID_CARD_CHAR_MIN_ROWSLINES_FOR_CHECK_START(charCols))
			{
				startEndPos[0] = (i - keepLines + 1);
				keepLines = 0;
				step = EXTRACT_ID_CARD_STEP_WAIT_END_PIX;
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


			if (++keepLines < ID_CARD_CHAR_MIN_ROWSLINES_FOR_CHECK_END(charCols))
				continue;

		_SAVE_START_END:
			startEndPos[1] = i - keepLines + 1 + 1;
			groupInfo.push_back(startEndPos);
			keepLines = 0;
			step = EXTRACT_ID_CARD_STEP_FIND_START_PIX;
		}
	}

	return (groupInfo.size() >= ID_CARD_CHAR_GROUPS_WITHOUT_CARD_NO);
}
