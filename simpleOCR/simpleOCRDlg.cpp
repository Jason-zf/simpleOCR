
// simpleOCRDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "simpleOCR.h"
#include "simpleOCRDlg.h"
#include "afxdialogex.h"

#include <cstdlib>
#include "myTools/CvtString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsimpleOCRDlg �Ի���



CsimpleOCRDlg::CsimpleOCRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsimpleOCRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsimpleOCRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listShowResult);
	DDX_Control(pDX, IDC_STATIC_SHOW_PIC, m_staticShowPic);
	DDX_Control(pDX, IDC_EDIT_ROI_NUM, m_editROINum);
}

BEGIN_MESSAGE_MAP(CsimpleOCRDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PIC, &CsimpleOCRDlg::OnBnClickedButtonOpenPic)
	ON_BN_CLICKED(IDC_BUTTON_SPLIT_CHARACTER, &CsimpleOCRDlg::OnBnClickedButtonSplitCharacter)
	ON_BN_CLICKED(IDC_BUTTON_OCR, &CsimpleOCRDlg::OnBnClickedButtonOcr)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, &CsimpleOCRDlg::OnBnClickedButtonClearAll)
//	ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CsimpleOCRDlg ��Ϣ�������

BOOL CsimpleOCRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_listShowResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_listShowResult.InsertColumn(0, L"���", LVCFMT_LEFT, 100, 0);
	m_listShowResult.InsertColumn(1, L"ʶ����", LVCFMT_LEFT, 200, 1);
	key = 0;
	flag = false;
	m_numofROI = 3;	
	m_isTimerAlive = false;
	m_nRows = 0;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CsimpleOCRDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsimpleOCRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsimpleOCRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsimpleOCRDlg::OnBnClickedButtonOpenPic()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CvtChar myCvtChar;
	CString pathName;
	CFileDialog dlg(TRUE, L".\\testPic\\1.jpg", NULL, OFN_HIDEREADONLY | OFN_READONLY, L"Pictures Files(*.jpg,*.bmp,*.png)|*.jpg;*.bmp;*.png|All Files(*.*)|*.*||", NULL);
	if (dlg.DoModal())
	{
		pathName = dlg.GetPathName();
	}
	string str = myCvtChar.WideByte2MultiByte(pathName);
	m_srcImg = imread(str);
	if (!m_srcImg.empty())
	{
		m_srcImg.copyTo(m_tempImg);		
		SetTimer(1, 40, NULL);
		m_isTimerAlive = true;
	}
}

void CsimpleOCRDlg::initROI(Mat& tempImg, map<int, Rect>& mROI)
{
	int x = 20, y = 0;
	/*�ж�ͼƬ�Ƿ�Ϊ��*/
	if (tempImg.empty())
	{
		return;
	}
	y = tempImg.rows / 2;
	for (int i = 0; i < m_numofROI; i++)
	{
		x += ROI_WIDTH * 2;
		Rect tempRect = Rect(x, y, ROI_WIDTH, ROI_HEIGHT);
		mROI.insert(pair<int, Rect>(i, tempRect));
		rectangle(tempImg, tempRect, Scalar(0, 0, 255), 1, 8);
	}
}

void CsimpleOCRDlg::changeROISize(WPARAM& wParam, Rect &roi)
{
	if (GetKeyState(VK_CONTROL) >= 0)
	{
		switch (wParam)
		{
		case VK_UP:
		{
			roi.height -= 1;
			break;
		}
		case VK_DOWN:
		{
			roi.height += 1;
			break;
		}
		case VK_LEFT:
		{
			roi.width -= 1;
			break;
		}
		case VK_RIGHT:
		{
			roi.width += 1;
			break;
		}
		default:
			break;
		}
	}
	else
	{
		return;
	}
}

void CsimpleOCRDlg::moveROI(WPARAM& wParam, Rect& roi)
{
	int state = GetKeyState(VK_CONTROL);
	if (state < 0)
	{
		switch (wParam)
		{
		case VK_UP:
		{
			roi.y -= 1;
			break;
		}
		case VK_DOWN:
		{
			roi.y += 1;
			break;
		}
		case VK_LEFT:
		{
			roi.x -= 1;
			break;
		}
		case VK_RIGHT:
		{
			roi.x += 1;
			break;
		}
		default:
			break;
		}
	}
}

void CsimpleOCRDlg::mouseMove()
{
	POINT point;
	::GetCursorPos(&point);
	float wScale = 0, hScale = 0;
	RECT rect;
	GetDlgItem(IDC_STATIC_SHOW_PIC)->GetWindowRect(&rect);
	point.x = point.x - rect.left;
	point.y = point.y - rect.top;
	m_staticShowPic.calScale(m_srcImg, wScale, hScale);
	point.x = point.x*wScale;
	point.y = point.y*hScale;
	if (m_srcImg.empty())
	{
		return;
	}
	m_srcImg.copyTo(m_tempImg);
	map<int, Rect>::iterator it = m_mROI.begin();
	for (; it != m_mROI.end(); it++)
	{
		if (it->second.x <= point.x&&point.x <= it->second.x + ROI_WIDTH&&point.y >= it->second.y&&point.y <= it->second.y + ROI_HEIGHT)
		{
			//it->first = point.x;  /*map�ļ�ֵΪconst���͵Ĳ��ܽ����޸�*/
			flag = true;
			key = it->first;
			break;
		}
		else
		{
			continue;
		}
	}
	if (flag == true)
	{
		map<int, Rect>::iterator it = m_mROI.find(key);
		it->second.x = point.x;
		it->second.y = point.y;
		if (m_isTimerAlive == true)
		{
			KillTimer(1);
			initROI(m_tempImg, m_mROI);
			m_staticShowPic.showPic(m_tempImg);
			m_isTimerAlive = false;
		}
	}
	for (map<int, Rect>::iterator it = m_mROI.begin(); it != m_mROI.end(); it++)
	{
		rectangle(m_tempImg, it->second, Scalar(0, 0, 255), 1, 8);
	}
	m_staticShowPic.showPic(m_tempImg);

}

BOOL CsimpleOCRDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	{		
		map<int, Rect>::iterator it = m_mROI.find(key);
		if (it != m_mROI.end())
		{
			changeROISize(pMsg->wParam, it->second);
			moveROI(pMsg->wParam, it->second);

		}
		m_srcImg.copyTo(m_tempImg);
		for (map<int, Rect>::iterator it = m_mROI.begin(); it != m_mROI.end(); it++)
		{
			rectangle(m_tempImg, it->second, Scalar(0, 0, 255), 1, 8);
		}	
		m_staticShowPic.showPic(m_tempImg);		
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (pMsg->wParam == MK_LBUTTON)
		{				
			mouseMove();
		}
		else
		{
			flag = false;
		}	
		break;
	}
	case WM_LBUTTONDOWN:
	{		
	}
	case WM_LBUTTONUP:
	{			
		break;
	}
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CsimpleOCRDlg::splitCharacter(const map<int, Mat>& srcImg, map<int, vector<Mat>>& dstImg, map<int, map<int, Rect>>& m)
{
	if (srcImg.empty())
	{
		return;
	}
	for (int i = 0; i < srcImg.size(); i++)
	{
		Mat color = srcImg.at(i);
		Mat gray;
		vector<Mat> imgROI;
		map<int, Rect> mRect;
		cvtColor(color, gray, COLOR_BGR2GRAY);
		threshold(gray, gray, THRESH_MIN, 255, THRESH_BINARY_INV);
		/*vector<vector<Point>> *contours = new vector<vector<Point>>[6];*/
		vector<vector<Point>> contours;
		if (!gray.empty())
		{
			findContours(gray, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		}
		for (int j = 0; j < contours.size(); j++)
		{
			Rect rectROI = boundingRect(contours.at(j));
			rectROI.x -= 2;
			rectROI.y -= 2;
			rectROI.width += 4;
			rectROI.height += 4;
			mRect.insert(pair<int, Rect>(j, rectROI));
		}
		/*ͨ��findContours�����ҳ�����������һ���ǰ�����ĸ��ԭ˳�����Ӧ����������*/
		for (size_t i = 0; i < mRect.size(); i++)
		{
			for (size_t j = i; j < mRect.size(); j++)
			{
				if (mRect[i].x < mRect[j].x)
				{
					Rect temp = mRect[i];
					mRect[i] = mRect[j];
					mRect[j] = temp;
				}
			}
		}
		for (size_t i = 0; i < mRect.size(); i++)
		{
			Mat colorROI = color(mRect[i]);
			/*�洢��ĸʱ˳���ǰ��մӵ�λ����λ˳��洢*/
			imgROI.push_back(colorROI);
		}
		m.insert(pair<int, map<int, Rect>>(i, mRect));
		dstImg.insert(pair<int, vector<Mat>>(i, imgROI));
	}
}

void CsimpleOCRDlg::OnBnClickedButtonSplitCharacter()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	map<int, Mat> srcSeqImg;
	map<int, vector<Mat>> dstSeqImg;
	map<int, map<int, Rect>> m;
	string filePath;
	string path;
	for (int i = 0; i < m_mROI.size(); i++)
	{
		Mat temp = m_srcImg(m_mROI.at(i));
		if (!temp.empty())
		{
			srcSeqImg.insert(pair<int, Mat>(i, temp));
		}
	}
	splitCharacter(srcSeqImg, dstSeqImg, m);
	/*�洢ģ���ַ*/
	filePath = filePath = ".\\template\\temp\\";
	for (map<int, vector<Mat>>::iterator it = dstSeqImg.begin(); it != dstSeqImg.end(); it++)
	{
		char buffer[10];
		_itoa_s(it->first, buffer, 10);
		string fileName = buffer;
		path = filePath + "T_" + buffer + "_";
		for (int i = 0; i < it->second.size(); i++)
		{
			char temp[10];
			_itoa_s(i, temp, 10);
			path = filePath + "T_" + buffer + "_" + temp + ".jpg";
			Mat color = it->second.at(i);
			Mat gray;
			cvtColor(color, gray, COLOR_BGR2GRAY);
			imwrite(path, gray);

		}
	}


}

void CsimpleOCRDlg::loadTemplate(map<int, Mat>& m)
{
	string path;
	for (int i = 0; i < 11; i++)
	{
		char buff[10];
		_itoa_s(i, buff, 10);
		string temp = buff;
		path = ".\\template\\T_" + (string)buff + ".jpg";
		Mat image = imread(path);
		m.insert(pair<int, Mat>(i, image));
	}
}

void CsimpleOCRDlg::shapeMatch(const map<int, vector<Mat>>& imgROI, const map<int, Mat>& mTemplate, map<int, double>& result)
{
	for (int i = 0; i < imgROI.size(); i++)
	{
		vector<Mat> vecImg = imgROI.at(i);
		double num = 0;
		int scale = 10;
		int flag = false;
		for (int j = vecImg.size() - 1; j >= 0; j--)
		{
			Mat color = vecImg.at(j);
			Mat gray;
			map<double, int> cormes1, cormes2, cormes3;
			cvtColor(color, gray, COLOR_BGR2GRAY);
			threshold(gray, gray, THRESH_MIN, 255, THRESH_BINARY_INV);
			vector<vector<Point>>contours1, contours2;
			vector<Vec4i>hierarchy1, hierarchy2;
			findContours(gray, contours1, hierarchy1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
			for (int k = 0; k < mTemplate.size(); k++)
			{
				Mat templColor = mTemplate.at(k);
				Mat templGray;
				cvtColor(templColor, templGray, COLOR_BGR2GRAY);
				threshold(templGray, templGray, THRESH_MIN, 255, THRESH_BINARY_INV);
				findContours(templGray, contours2, hierarchy2, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
				if (contours2.size() == contours1.size())
				{
					int buff =contours2[0].size()-contours1[0].size();	
					buff = abs(buff);
					if (buff<=5)
					{
						double buff1 = matchShapes(contours1[0], contours2[0], CV_CONTOURS_MATCH_I1, 0.0);
						cormes1.insert(pair<double, int>(buff1, k));
					}
					
				}

			}
			map<double, int>::iterator it = cormes1.begin();
			if (flag == true)
			{
				num = num + (double)it->second / scale;
				scale *= 10;
			}
			else
			{
				if (it->second == 10)
				{
					flag = true;
				}
				else
				{
					num = num * 10 + it->second;
				}
			}
		}
		result.insert(pair<int, double>(i, num));
	}
}

/*�ַ�ת��*/
wstring num2string(double num)
{
	wostringstream ostr;
	ostr << num;
	return ostr.str();
}

void CsimpleOCRDlg::OnBnClickedButtonOcr()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	map<int, Mat> imgROI;
	map<int, vector<Mat>> imgROI2Segment;
	map<int, map<int, Rect>> map2Segment;
	map<int, vector<Mat>> segmentDescriptor;
	map<int, Mat> templateDescriptor;
	map<int, Mat> mTemplate;
	/*��������result.txt�ļ���*/
	ofstream out(".\\result\\result.txt");
	if (!out)
	{
		return;
	}
	/*����ģ��*/
	loadTemplate(mTemplate);
	if (m_srcImg.empty() || m_mROI.empty())
	{
		return;
	}
	for (map<int, Rect>::iterator it = m_mROI.begin(); it != m_mROI.end(); it++)
	{
		Mat temp;
		temp = m_srcImg(it->second);
		imgROI.insert(pair<int, Mat>(it->first, temp));
	}
	splitCharacter(imgROI, imgROI2Segment, map2Segment);
	map<int, double>result1, result2;
	shapeMatch(imgROI2Segment, mTemplate, result2);
	out << "���" << setw(10) << "ʶ����" << endl;
	for (size_t i = 0; i < result2.size(); i++)
	{		
		CvtString cvtString;
		wstring str;
		str = cvtString.to_cstring<int>(i+m_nRows);
		m_listShowResult.InsertItem(i+m_nRows, str.c_str());		
		str = cvtString.to_cstring<double>(result2.at(i));
		m_listShowResult.SetItemText(i+m_nRows, 1, str.c_str());
		out << i << setw(10 + 2 - sizeof(i) / sizeof(int)) << result2.at(i) << endl;
	}
	m_nRows += result2.size();
	out.close();
}

void CsimpleOCRDlg::OnBnClickedButtonClearAll()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Mat image = Mat(Size(320, 240), CV_8U, Scalar(255, 255, 255));
	image.copyTo(m_srcImg);
	image.copyTo(m_tempImg);
	m_staticShowPic.showPic(m_srcImg);
	m_mROI.clear();
}

void CsimpleOCRDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
	{
		CString str;
		m_editROINum.GetWindowTextW(str);
		CvtString cvtString;
		int buff = cvtString.cstring_to<int>(str.GetString());
		if (buff>0)
		{
			m_numofROI = buff;
		}
		m_srcImg.copyTo(m_tempImg);
		initROI(m_tempImg, m_mROI);
		m_staticShowPic.showPic(m_tempImg);
		break;
	}
	default:
		break;
	}
		CDialogEx::OnTimer(nIDEvent);
}
