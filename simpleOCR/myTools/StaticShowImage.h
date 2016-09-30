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
	@����ͼ��Ϳؼ��ı�������
	@srcImg ԭʼ����ͼ��
	@wScale x�����ϵı�������
	@hScale y�����ϵı�������
	*/
	void calScale(const Mat& srcImg, float& wScale, float& hScale);

	/*
	@���㰴�����ڿͻ�������ʾ����
	@srcImg �����ԭʼͼ��
	@showRect �ͻ�����ʾ����
	*/
	void calShowArea(const Mat& srcImg, CRect& showRect);

	/*
	@��ʾͼƬ
	@srcImg ԭʼ����ͼ��
	@showRect �ͻ�����ʾ����
	*/
	void showPic(const Mat& srcImg,CRect* pRect,const int& showType=1);

	/*
	@��ʾͼƬ
	@srcImg ����ԭʼͼ��
	@showType �Ƿ�ȫ����ʾ
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