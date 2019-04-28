/*
*charSplit.h
*×Ö·û·Ö¸î
*/

#ifndef __CHAR_SPLIT_H__
#define	__CHAR_SPLIT_H__

#include "imageSplit.h"

using cv::Vec2i;
class CCharSplit : public CImageSplit
{
public:
	CCharSplit(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam);
	virtual ~CCharSplit();

protected:
	virtual BOOL InitSplit(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam);
	virtual BOOL SplitWork(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, vector<Vec4i>& childInfos);

private:
	enum
	{
		CHAR_SPLIT_STEP_FIND_START_LINE = 1,
		CHAR_SPLIT_STEP_WAIT_END_LINE,
	};
	BOOL SplitCharsByColSeperator(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, vector<Vec4i>& childInfos);
	BOOL ExtractCharInRow(Mat& srcImg, IMAGE_SPLIT_PARAM* pParam, Vec2i& rowStartEnd);
};


#endif // !__CHAR_SPLIT_H__



