/*
*spliteRecognition.h
* 分割图像识别基类
*/

#ifndef __SPLITE_RECOGNITION_H__
#define __SPLITE_RECOGNITION_H__
#include <dp.h>
#include <opencv2/core.hpp>
using cv::Mat;
class CSpliteRecognition
{
public:
	CSpliteRecognition(Mat& spliteImage);
	virtual ~CSpliteRecognition();

public:
	BOOL InitRecognition();
	BOOL Recognition(char* pOutBuf, int len);

protected:
	virtual BOOL InitRecognition(Mat& orgImg)=0;
	virtual BOOL Recognition(Mat& orgImg, char* pOutBuf, int len) = 0;

private:
	Mat		m_spliteImg;
};
#endif // !__SPLITE_RECOGNITION_H__
