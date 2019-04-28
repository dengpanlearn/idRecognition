/*
*imageSplit.h
* 图像分割基类，所有图像分割都派生于该类
*/

#ifndef __IMAGE_SPLIT_H__
#define __IMAGE_SPLIT_H__
#include<dp.h>
#include <opencv2/core.hpp>
#include "spliteRecognition.h"

using std::vector;
using cv::Mat;
using cv::Vec4i;

struct IMAGE_SPLIT_PARAM
{
	int rowLinesForChkStart;
	int rowLinesForChkEnd;
	int colLinesForChkStart;
	int colLinesForChkEnd;

	int  minSplitUnits;
};

class CImageSplit
{
public:
	CImageSplit(Mat& orgImg, IMAGE_SPLIT_PARAM* pParam);
	virtual ~CImageSplit();

public:
	BOOL InitSplit();
	BOOL Split(vector<Vec4i>& childInfos);

	void SplitRecognition();
	inline void GetOrgImg(Mat& orgImg);

protected:
	virtual BOOL InitSplit(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam)=0;
	virtual BOOL SplitWork(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, vector<Vec4i>& childInfos) = 0;

private:
	Mat							m_orgImg;
	IMAGE_SPLIT_PARAM			m_param;
	vector<CSpliteRecognition*> m_splitsForRecog;
};

inline void CImageSplit::GetOrgImg(Mat& orgImg)
{
	orgImg = m_orgImg;
}
#endif // !__IMAGE_SPLIT_H__

