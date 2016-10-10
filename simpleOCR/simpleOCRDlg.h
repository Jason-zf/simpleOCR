
// simpleOCRDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "myTools/StaticShowImage.h"
#include "myTools/CvtChar.h"
#include <string>
#include <map>
#include <vector>
#include <opencv.hpp>
using namespace std;
using namespace cv;

const int ROI_NUM = 3;
const int ROI_WIDTH = 40;
const int ROI_HEIGHT = 40;
const int THRESH_MIN = 70; 

// CsimpleOCRDlg 对话框
class CsimpleOCRDlg : public CDialogEx
{
// 构造
public:
	CsimpleOCRDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIMPLEOCR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Mat m_srcImg;  /*原始图片*/
	Mat m_tempImg;
	map<int, Rect> m_mROI;
	int key;
	int flag;
	int m_numofROI;
	/*判断timer是否存在*/
	bool m_isTimerAlive;
	/*显示结果时的函数*/
public:
	/*
	@初始化感兴趣区域
	@tempImg 输入临时图片
	@mROI 感兴趣区域
	*/
	void initROI(Mat& tempImg, map<int, Rect>& mROI);

	/*改变感兴趣区域的大小*/
	void changeROISize(WPARAM& wParam, Rect &roi);

	/*移动感兴趣区域位置*/
	void moveROI(WPARAM& wParam, Rect& roi);

	/*鼠标移动时消息响应函数*/
	void mouseMove();

	/*加载模板*/
	void loadTemplate(map<int, Mat>& m);

	/*分割字符*/
	void splitCharacter(const map<int, Mat>& srcImg, map<int, vector<Mat>>& dstImg, map<int, map<int, Rect>>& m);

	/*通过尺度不变的矩特性来进行字符匹配识别*/
	void shapeMatch(const map<int, vector<Mat>>& imgROI, const map<int, Mat>& mTemplate, map<int, double>& result);
public:
	CListCtrl m_listShowResult;
	CStaticShowImage m_staticShowPic;
	afx_msg void OnBnClickedButtonOpenPic();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnBnClickedButtonSplitCharacter();
	afx_msg void OnBnClickedButtonOcr();
	afx_msg void OnBnClickedButtonClearAll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editROINum;
};
