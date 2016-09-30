// StaticShowImage.cpp : 实现文件
//

#include "stdafx.h"
#include "StaticShowImage.h"


// CStaticShowImage

IMPLEMENT_DYNAMIC(CStaticShowImage, CStatic)

CStaticShowImage::CStaticShowImage()
{
	_clientRect = CRect(0, 0, 0, 0);
	_bFullScreen = false;	
}

CStaticShowImage::~CStaticShowImage()
{
}


BEGIN_MESSAGE_MAP(CStaticShowImage, CStatic)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CStaticShowImage 消息处理程序

void CStaticShowImage::calScale(const Mat& srcImg, float& wScale, float& hScale)
{
	GetWindowRect(&_clientRect);
	ScreenToClient(&_clientRect);
	wScale = (float)srcImg.cols / _clientRect.Width();
	hScale = (float)srcImg.rows / _clientRect.Height();
}

void CStaticShowImage::calShowArea(const Mat& srcImg, CRect& showRect)
{
	int w, h, x = -1, y = -1;
	float wScale = 0.0, hScale = 0.0;
	calScale(srcImg, wScale, hScale);
	/*获取客户区中心的位置*/
	CPoint center;
	center = _clientRect.CenterPoint();

	if (wScale < 1 && hScale < 1)  /*图像比较小时显示到中心处*/
	{
		w = srcImg.cols;
		h = srcImg.rows;
		x = center.x - w / 2;
		y = center.y - h / 2;
	}
	else                          /*图形比较大时比例因子去较大值*/
	{
		/*float scale = (wScale>hScale) ? wScale, x = 0 : hScale, y = 0;*/
		float scale = 0.0;
		if (wScale > hScale)
		{
			scale = wScale;
			x = 0;
		}
		else
		{
			scale = hScale;
			y = 0;
		}

		w = (float)srcImg.cols / scale;
		h = (float)srcImg.rows / scale;

		if (x == 0)
		{
			y = center.y - (int)(h / 2);
		}
		else
		{
			x = center.x - (int)(w / 2);
		}
	}
	showRect = CRect(x, y, x + w, y + h);
}

void CStaticShowImage::showPic(const Mat& srcImg, CRect* pRect, const int& showType)
{
	CDC *pDC = GetDC();
	srcImg.copyTo(_srcImg);
	HDC hDC = pDC->GetSafeHdc();
	if (pRect == NULL)
	{
		pRect = new CRect();
		if (showType == PIC_SHOW_FULLSTATIC)
		{
			GetWindowRect(pRect);
			ScreenToClient(pRect);
		}
		else
		{
			calShowArea(srcImg, *pRect);
		}
	}
	/*检查数据是否为空*/
	if (!srcImg.empty() && pDC&&pRect)
	{

		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = srcImg.cols, bmp_h = srcImg.rows;
		fillBitmapInfo(bmi, bmp_w, bmp_h, IPL_DEPTH_8U*srcImg.channels(), 0);

		/*单通道*/
		if (IPL_DEPTH_8U*srcImg.channels() == 8)
		{
			RGBQUAD* palette = bmi->bmiColors;
			int i;
			for (i = 0; i < 256; i++)
			{
				palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
				palette[i].rgbReserved = 0;
			}
		}
		if (pRect->Width() < bmp_w)
		{
			SetStretchBltMode(
				pDC->m_hDC,           // handle to device context
				HALFTONE);
		}
		else
		{
			SetStretchBltMode(
				pDC->m_hDC,           // handle to device context
				COLORONCOLOR);
		}
		fillBitmapInfo(bmi, bmp_w, bmp_h, IPL_DEPTH_8U*srcImg.channels(), 0);
		::StretchDIBits(
			hDC,
			pRect->left, pRect->top, pRect->Width(), pRect->Height(),
			0, 0, bmp_w, bmp_h,
			srcImg.data, bmi, DIB_RGB_COLORS, SRCCOPY);
	}
	SetTimer(2, 40, NULL);

}

void CStaticShowImage::showPic(const Mat& srcImg, const int& showType)
{
	srcImg.copyTo(_srcImg);	
	SetTimer(1, 40, NULL);
}

void  CStaticShowImage::fillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	try
	{
		assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	}
	catch (Exception e)
	{
		throw e;
	}


	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void CStaticShowImage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{
		CDC *pDC = GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect* pRect = new CRect();
		GetWindowRect(pRect);
		ScreenToClient(pRect);

		/*检查数据是否为空*/
		if (!_srcImg.empty() && pDC&&pRect)
		{
			uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
			BITMAPINFO* bmi = (BITMAPINFO*)buffer;
			int bmp_w = _srcImg.cols, bmp_h = _srcImg.rows;
			fillBitmapInfo(bmi, bmp_w, bmp_h, IPL_DEPTH_8U*_srcImg.channels(), 0);

			/*单通道*/
			if (IPL_DEPTH_8U*_srcImg.channels() == 8)
			{
				RGBQUAD* palette = bmi->bmiColors;
				int i;
				for (i = 0; i < 256; i++)
				{
					palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
					palette[i].rgbReserved = 0;
				}
			}
			if (pRect->Width() < bmp_w)
			{
				SetStretchBltMode(
					pDC->m_hDC,           // handle to device context
					HALFTONE);
			}
			else
			{
				SetStretchBltMode(
					pDC->m_hDC,           // handle to device context
					COLORONCOLOR);
			}
			fillBitmapInfo(bmi, bmp_w, bmp_h, IPL_DEPTH_8U*_srcImg.channels(), 0);
			::StretchDIBits(
				hDC,
				pRect->left, pRect->top, pRect->Width(), pRect->Height(),
				0, 0, bmp_w, bmp_h,
				_srcImg.data, bmi, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	}
	case 2:
	{
		showPic(_srcImg);
		break;
	}
	default:
		break;

	}
	CStatic::OnTimer(nIDEvent);
}

void CStaticShowImage::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	KillTimer(1);
	/*KillTimer(2);*/
}

/*全屏显示（鼠标左键双击）*/
void CStaticShowImage::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
	/*全屏显示*/
	LONG style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
	if (_bFullScreen == false) /*全屏显示*/
	{
		_hWndParent = ::GetParent(m_hWnd);
		::ShowWindow(_hWndParent, SW_HIDE);
		::SetParent(m_hWnd, NULL);
		style &= ~(WS_DLGFRAME | WS_THICKFRAME);
		SetWindowLong(this->m_hWnd, GWL_STYLE, style);
		this->ShowWindow(SW_SHOWMAXIMIZED);
		//CRect rect;  
		//this->GetWindowRect(&rect);  
		//::SetWindowPos((this->m_hWnd,HWND_NOTOPMOST,rect.left-1, rect.top-1, rect.right-rect.left + 3, rect.bottom-rect.top + 3, SWP_FRAMECHANGED);  
		int   nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int   nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, nScreenWidth, nScreenHeight, SWP_FRAMECHANGED);
		_bFullScreen = true;
	}
	else
	{
		style |= WS_DLGFRAME | WS_THICKFRAME;
		SetWindowLong(this->m_hWnd, GWL_STYLE, style);
		::SetParent(m_hWnd, _hWndParent);
		::ShowWindow(_hWndParent, SW_SHOW);
		_bFullScreen = false;
	}
	CStatic::OnLButtonDblClk(nFlags, point);
}
