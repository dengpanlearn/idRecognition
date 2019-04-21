/*
*spliteRecognition.cpp
* 分割图像识别基类
*/

#include <opencv2/imgproc.hpp>
#include "../include/spliteRecognition.h"
using namespace cv;
using namespace std;


CSpliteRecognition::CSpliteRecognition(Mat& spliteImage)
{
	m_spliteImg = spliteImage;
}

CSpliteRecognition::~CSpliteRecognition()
{

}

BOOL CSpliteRecognition::InitRecognition()
{
	return InitRecognition(m_spliteImg);
}

BOOL CSpliteRecognition::Recognition(char* pOutBuf, int len)
{
	return Recognition(m_spliteImg, pOutBuf, len);
}