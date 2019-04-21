/*
*imageRecogniton.h 
* 图像识别基类，所有图像识别都派生于该类
*/

#ifndef __IMAGE_RECOGNITON_H__
#define __IMAGE_RECOGNITON_H__
#include <dp.h>
#include <opencv2/core.hpp>

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
	BOOL ExtractUsefulParts();
	BOOL Recognition();

protected:
	virtual BOOL FilterImg(Mat& srcImg, Mat& destImg)=0;
	virtual BOOL ThresholdImg(Mat& srcImg, Mat& destImg)=0;

	virtual BOOL ExtractUsefulParts(Mat& srcImg, vector<Vec4i>& extractInfo)=0;
	virtual BOOL SplitUsefulPart(Mat& partImg, vector<Vec4i>& splitInfo)=0;
	virtual BOOL Recogniton(Mat& splitImg, char* pOutBuf, int len)=0;

protected:
	Mat		m_orgImg;
	Mat		m_binImg;	
	vector<Vec4i> m_extractParts;
};


#endif // !__IMAGE_RECOGNITON_H__


