
// simpleOCRDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "simpleOCR.h"
#include "simpleOCRDlg.h"
#include "afxdialogex.h"

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
}

BEGIN_MESSAGE_MAP(CsimpleOCRDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PIC, &CsimpleOCRDlg::OnBnClickedButtonOpenPic)
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
		initROI(m_tempImg, m_mROI);
		m_staticShowPic.showPic(m_tempImg);
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
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}