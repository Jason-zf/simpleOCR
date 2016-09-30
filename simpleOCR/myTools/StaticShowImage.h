#ifndef _CSTATIC_SHOWIMAGE_
#define _CSTATIC_SHOWIMAGE_

#define PIC_SHOW_FULLSTATIC 1
#define PIC_SHOW_KEEPSCALE 0

#include<opencv.hpp>
#include<windows.h>
#include <map>
using namespace std;
using namespace cv;

// CStaticShowImage

class CStaticShowImage : public CStatic
{
	DECLARE_DYNAMIC(CStaticShowImage)

public:
	CStaticShowImage();
	virtual ~CStaticShowImage();

public:

	/*
	@计算图像和控件的比例因子
	@srcImg 原始输入图像
	@wScale x方向上的比例因子
	@hScale y方向上的比例因子
	*/
	void calScale(const Mat& srcImg, float& wScale, float& hScale);

	/*
	@计算按比例在客户区的显示区域
	@srcImg 输入的原始图像
	@showRect 客户区显示区域
	*/
	void calShowArea(const Mat& srcImg, CRect& showRect);

	/*
	@显示图片
	@srcImg 原始输入图像
	@showRect 客户区显示区域
	*/
	void showPic(const Mat& srcImg,CRect* pRect,const int& showType=1);

	/*
	@显示图片
	@srcImg 输入原始图像
	@showType 是否全屏显示
	*/
	void showPic(const Mat& srcImg, const int& showType = 1);

	void fillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin=0);

private:
	Mat _srcImg;
	CRect _clientRect;
	bool _bFullScreen;
	HWND _hWndParent;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);	
	/*afx_msg void OnMouseMove(UINT nFlags, CPoint point);*/
};


#endif