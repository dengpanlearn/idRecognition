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

	m_extractParts.clear();
	return TRUE;
}