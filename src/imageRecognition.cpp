/*
*imageRecognition.cpp
*
*/

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "../include/imageRecognition.h"

using namespace std;
using namespace cv;

#define IMG_THRESHOHLD_DFT		110
#define	IMG_THRESHOHLD_MAX		160
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

	m_grayImg.create(m_orgImg.size(), CV_8UC1);

	for (int i = 0; i < m_orgImg.rows; i++)
	{
		for (int j = 0; j < m_orgImg.cols; j++)
		{
			Vec3b curVal = m_orgImg.at<Vec3b>(i, j);
		//	unsigned char bVal = curVal[0];
		//	unsigned char gVal = curVal[1];
			unsigned char rVal = curVal[2];
		//	grayImg.at<unsigned char>(i, j) = 0.9*rVal + 0.05*gVal + 0.05*bVal;
			m_grayImg.at<unsigned char>(i, j) = rVal;
		}
	}


	//cvtColor(m_orgImg, grayImg, COLOR_BGR2GRAY);
	if (!FilterImg(m_grayImg, m_grayImg))
		return FALSE;

	m_binImg.create(m_grayImg.size(), m_grayImg.type());
	if (!ThresholdImg(m_grayImg, m_binImg, IMG_THRESHOHLD_DFT))
		return FALSE;

	m_partsForSplit.clear();
#if 1
	namedWindow("org Img", WINDOW_NORMAL);
	namedWindow("gray Img", WINDOW_NORMAL);
	namedWindow("bin Img", WINDOW_NORMAL);
	imshow("org Img", m_orgImg);
	imshow("gray Img", m_grayImg);
	imshow("bin Img", m_binImg);
	waitKey(0);
#endif
	return TRUE;
}

BOOL CImageRecognition::ExtractAllUsefulParts()
{
	int threshold = IMG_THRESHOHLD_DFT; 
	for (; threshold < IMG_THRESHOHLD_MAX; )
	{
		if (ExtractUsefulParts(m_binImg, m_partsForSplit))
			break;

		threshold += 3;
		ThresholdImg(m_grayImg, m_binImg, threshold);
	};

	if (threshold >= IMG_THRESHOHLD_MAX)
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

void CImageRecognition::FreeAllUsefulParts()
{
	FreeUsefulParts(m_partsForSplit);
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