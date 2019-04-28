/*idCardRecogniton.h
* 身份证识别类实现
*/

#ifndef __ID_CARD_RECOGNITION_H__
#define	__ID_CARD_RECOGNITION_H__
#include "imageRecognition.h"

using std::vector;
using cv::Vec2i;
class CIdCardRecognition : public CImageRecognition
{
public:
	CIdCardRecognition();
	virtual ~CIdCardRecognition();

protected:
	virtual BOOL FilterImg(Mat& srcImg, Mat& destImg);
	virtual BOOL ThresholdImg(Mat& srcImg, Mat& destImg);
	virtual BOOL ExtractUsefulParts(Mat& srcImg, vector<CImageSplit*>& partsForSplit);
	virtual void FreeUsefulParts(vector<CImageSplit*>& partsForSplit);
	virtual void OnExtractUsefulPartImg(Mat& partImg, int partIdx);

	virtual BOOL SplitUsefulPart(CImageSplit* pPartForSplit, vector<Vec4i>& splitInfo);
	virtual void OnSplitUsefulPartImg(Mat& splitImg, int partIdx, int splitIdx);

	virtual BOOL Recogniton(CImageSplit* pPartForSplit);

private:
	enum
	{
		EXTRACT_ID_CARD_STEP_FIND_START_PIX = 1,
		EXTRACT_ID_CARD_STEP_WAIT_END_PIX,
	};

	BOOL ExtractIdCardNo(Mat& orgImg, Vec4i& cardNoInfo, Vec2i& firstCharInfo);
	BOOL ExtractIdCardNoRows(Mat& orgImg, Vec2i& rowStartEnd);
	BOOL ExtractIdCardNoCols(Mat& orgImg, Vec2i& colStartEnd, Vec2i& firstChar);

	int CalcRowStartByIdCarNo(Mat& orgImg, Vec4i& cardNoInfo, Vec2i& firstCharInfo);

	BOOL ExtractIdCardHeadImg(Mat& noneCardNoImg, Vec4i& headImgInfo);
	BOOL ExtractIdCardHeadImgCols(Mat& noneCardNoImg, Vec2i& colStartEnd);
	BOOL ExtractIdCardHeadImgRows(Mat& srcImg, Vec2i& rowStartEnd);

	BOOL ExtractIdCardCharGroups(Mat& srcImg, vector<Vec2i>& groupInfo, int charCols);
};

#endif // !__ID_CARD_RECOGNITION_H__


