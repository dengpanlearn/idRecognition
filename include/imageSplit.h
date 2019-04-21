/*
*imageSplit.h
* 图像分割基类，所有图像分割都派生于该类
*/

#ifndef __IMAGE_SPLIT_H__
#define __IMAGE_SPLIT_H__
#include<dp.h>
#include <opencv2/core.hpp>

using std::vector;
using cv::Mat;
using cv::Vec4i;

class CImageSplit
{
public:
	CImageSplit(Mat& orgImg);
	virtual ~CImageSplit();

public:
	BOOL InitSplit();
	BOOL Split(vector<Vec4i>& childInfo);

protected:
	virtual BOOL InitSplit(Mat& orgImg)=0;
	virtual BOOL Split(Mat& orgImg, vector<Vec4i>& childInfo) = 0;

private:
	Mat	m_orgImg;
};

#endif // !__IMAGE_SPLIT_H__

