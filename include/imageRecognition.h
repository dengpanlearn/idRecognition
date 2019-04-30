/*
*imageRecogniton.h 
* 图像识别基类，所有图像识别都派生于该类
*/

#ifndef __IMAGE_RECOGNITON_H__
#define __IMAGE_RECOGNITON_H__
#include <dp.h>
#include <opencv2/core.hpp>
#include "imageSplit.h"

using cv::Mat;
using cv::Vec4i;
using std::vector;


class CImageRecognition
{
public:
	CImageRecognition();
	virtual ~CImageRecognition();

public:
	BOOL LoadImg(const char* pFileName);
	BOOL ExtractAllUsefulParts();
	void FreeAllUsefulParts();
	BOOL Recognition();

protected:
	virtual BOOL FilterImg(Mat& srcImg, Mat& destImg)=0;
	virtual BOOL ThresholdImg(Mat& srcImg, Mat& destImg, int thresholdVal)=0;

	virtual BOOL ExtractUsefulParts(Mat& srcImg, vector<CImageSplit*>& partsForSplit)=0;
	virtual void FreeUsefulParts(vector<CImageSplit*>& partsForSplit) = 0;
	virtual void OnExtractUsefulPartImg(Mat& partImg, int partIdx) = 0;

	virtual BOOL SplitUsefulPart(CImageSplit* pPartForSplit, vector<Vec4i>& splitInfo)=0;
	virtual void OnSplitUsefulPartImg(Mat& splitImg, int partIdx, int splitIdx) = 0;

	virtual BOOL Recogniton(CImageSplit* pPartForSplit)=0;

protected:
	Mat		m_orgImg;
	Mat		m_grayImg;
	Mat		m_binImg;	
	vector<CImageSplit*> m_partsForSplit;
};


#endif // !__IMAGE_RECOGNITON_H__


