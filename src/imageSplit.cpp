/*
*imageSplit.cpp
* ͼ��ָ�
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
	return InitSplit(m_orgImg);
}

BOOL CImageSplit::Split(vector<Vec4i>& childInfo)
{
	return Split(m_orgImg, childInfo);
}

void CImageSplit::SplitRecognition()
{

}