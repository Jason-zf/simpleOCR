
// simpleOCRDlg.h : ͷ�ļ�
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

// CsimpleOCRDlg �Ի���
class CsimpleOCRDlg : public CDialogEx
{
// ����
public:
	CsimpleOCRDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SIMPLEOCR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Mat m_srcImg;  /*ԭʼͼƬ*/
	Mat m_tempImg;
	map<int, Rect> m_mROI;
	int key;
	int flag;
	int m_numofROI;
	/*�ж�timer�Ƿ����*/
	bool m_isTimerAlive;
	/*��ʾ���ʱ�ĺ���*/
public:
	/*
	@��ʼ������Ȥ����
	@tempImg ������ʱͼƬ
	@mROI ����Ȥ����
	*/
	void initROI(Mat& tempImg, map<int, Rect>& mROI);

	/*�ı����Ȥ����Ĵ�С*/
	void changeROISize(WPARAM& wParam, Rect &roi);

	/*�ƶ�����Ȥ����λ��*/
	void moveROI(WPARAM& wParam, Rect& roi);

	/*����ƶ�ʱ��Ϣ��Ӧ����*/
	void mouseMove();

	/*����ģ��*/
	void loadTemplate(map<int, Mat>& m);

	/*�ָ��ַ�*/
	void splitCharacter(const map<int, Mat>& srcImg, map<int, vector<Mat>>& dstImg, map<int, map<int, Rect>>& m);

	/*ͨ���߶Ȳ���ľ������������ַ�ƥ��ʶ��*/
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
