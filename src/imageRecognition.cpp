/*
*imageRecognition.cpp
*
*/

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "../include/imageRecognition.h"

using namespace std;
using namespace cv;
CImageRecognition::CImageRecognition()
{
}


CImageRecognition::~CImageRecognition()
{
}


BOOL CImageRecognition::LoadImg(const char* pFileName)
{
	m_orgImg = imread(pFileName, IMREAD_COLOR);
	if (m_orgImg.data == NULL)
		return FALSE;

	Mat grayImg;
	grayImg.create(m_orgImg.size(), m_orgImg.type());

	cvtColor(m_orgImg, grayImg, COLOR_BGR2GRAY);
	if (!FilterImg(grayImg, grayImg))
		return FALSE;

	m_binImg.create(grayImg.size(), grayImg.type());
	if (!ThresholdImg(grayImg, m_binImg))
		return FALSE;

	m_partsForSplit.clear();
	return TRUE;
}

BOOL CImageRecognition::ExtractUsefulParts()
{
	if (!ExtractUsefulParts(m_binImg, m_partsForSplit))
		return FALSE;

	for (int i = 0; i < m_partsForSplit.size(); i++)
	{
		Mat partImg;
		CImageSplit* pPartForSplit = m_partsForSplit[i];
		pPartForSplit->GetOrgImg(partImg);
		OnExtractUsefulPartImg(partImg, i);
	}

	return TRUE;
}

BOOL CImageRecognition::Recognition()
{
	if (!m_partsForSplit.size())
		return FALSE;

	for (int i = 0; i < m_partsForSplit.size(); i++)
	{
		vector<Vec4i> splitInfo;
		
		CImageSplit* pPartForSplit = m_partsForSplit[i];

		if (!SplitUsefulPart(pPartForSplit, splitInfo))
			continue;

		Mat partImg;
		pPartForSplit->GetOrgImg(partImg);
		for (int j = 0; j < splitInfo.size(); j++)
		{
			int startRow = splitInfo[j][0];
			int endRow = splitInfo[j][1];
			int startCol = splitInfo[j][2];
			int endCol = splitInfo[j][3];

			Mat splitImg = partImg(Range(startRow, endRow), Range(startCol, endCol));
			OnSplitUsefulPartImg(splitImg, i, j);
		}

		Recogniton(pPartForSplit);
	}

	return TRUE;
}