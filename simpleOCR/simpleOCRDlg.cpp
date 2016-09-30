
// simpleOCRDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "simpleOCR.h"
#include "simpleOCRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CsimpleOCRDlg 对话框



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
}

BEGIN_MESSAGE_MAP(CsimpleOCRDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PIC, &CsimpleOCRDlg::OnBnClickedButtonOpenPic)
END_MESSAGE_MAP()


// CsimpleOCRDlg 消息处理程序

BOOL CsimpleOCRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_listShowResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_listShowResult.InsertColumn(0, L"标号", LVCFMT_LEFT, 100, 0);
	m_listShowResult.InsertColumn(1, L"识别结果", LVCFMT_LEFT, 200, 1);
	key = 0;
	flag = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsimpleOCRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsimpleOCRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsimpleOCRDlg::OnBnClickedButtonOpenPic()
{
	// TODO:  在此添加控件通知处理程序代码
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
		initROI(m_tempImg, m_mROI);
		m_staticShowPic.showPic(m_tempImg);
	}
}

void CsimpleOCRDlg::initROI(Mat& tempImg, map<int, Rect>& mROI)
{
	int x = 20, y = 0;
	/*判断图片是否为空*/
	if (tempImg.empty())
	{
		return;
	}
	y = tempImg.rows / 2;
	for (int i = 0; i < ROI_NUM; i++)
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
			//it->first = point.x;  /*map的键值为const类型的不能进行修改*/
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
	}
	for (map<int, Rect>::iterator it = m_mROI.begin(); it != m_mROI.end(); it++)
	{
		rectangle(m_tempImg, it->second, Scalar(0, 0, 255), 1, 8);
	}
	m_staticShowPic.showPic(m_tempImg);
}

BOOL CsimpleOCRDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
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
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}