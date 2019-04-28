/*
*imageSplit.cpp
* Í¼Ïñ·Ö¸î
*/

#include <opencv2/imgproc.hpp>
#include "../include/imageSplit.h"

using namespace cv;
using namespace std;


CImageSplit::CImageSplit(Mat& orgImg, IMAGE_SPLIT_PARAM* pParam)
{
	m_orgImg = orgImg;
	memcpy(&m_param, pParam, sizeof(IMAGE_SPLIT_PARAM));
}


CImageSplit::~CImageSplit()
{
}

BOOL CImageSplit::InitSplit()
{
	return InitSplit(m_orgImg, &m_param);
}

BOOL CImageSplit::Split(vector<Vec4i>& childInfos)
{
	return SplitWork(m_orgImg, &m_param, childInfos);
}

void CImageSplit::SplitRecognition()
{

}