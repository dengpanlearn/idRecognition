/*
*imageSplit.cpp
* Í¼Ïñ·Ö¸î
*/

#include <opencv2/imgproc.hpp>
#include "../include/imageSplit.h"

using namespace cv;
using namespace std;


CImageSplit::CImageSplit(Mat& orgImg)
{
	m_orgImg = orgImg;
}


CImageSplit::~CImageSplit()
{
}

BOOL CImageSplit::InitSplit()
{
	InitSplit(m_orgImg);
}

BOOL CImageSplit::Split(vector<Vec4i>& childInfo)
{
	Split(m_orgImg, childInfo);
}