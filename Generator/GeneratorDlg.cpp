
// GeneratorDlg.cpp : implementation file
//#pragma comment(lib, "Rsa.lib")
#pragma once
#include "stdafx.h"
#include "Generator.h"
#include "GeneratorDlg.h"
#include "afxdialogex.h"



//#include"CalRSA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//extern ;





// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CGeneratorDlg dialog



CGeneratorDlg::CGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeneratorDlg::IDD, pParent)
	, m_num(32)//, m_str(_T("49B1013BE3AF15F48DBBFAA52F957259"))
	//	, m_str2(_T("D0FC58424A742C21C5FE92B4D6A5E32D"))
	, m_str(_T("A5E5566D")), m_str2(_T("8A52E5D5"))
	, inpText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pRsa = NULL;
}

void CGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_num);
	DDX_Text(pDX, IDC_EDIT2, m_str);
	DDX_Text(pDX, IDC_EDIT3, m_str2);

	DDX_Text(pDX, IDC_EDIT4, inpText);
}

BEGIN_MESSAGE_MAP(CGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGeneratorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CGeneratorDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGeneratorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGeneratorDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CGeneratorDlg message handlers

BOOL CGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	pRsa = new Rsa();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGeneratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//������ʾCString
void CGeneratorDlg::InsertInfo(const CString& str)
{
	m_list.InsertString(m_list.GetCount(), str);
	m_list.SetCurSel(m_list.GetCount() - 1);
}
//������ʾstring  constֻ�� ��֤���ı䱻��ʾ��ֵ
void CGeneratorDlg::InsertInfo(const string& str)
{
//	CString temp1{ str.c_str()};
	string temp = str;	
	CString outstr{ temp.c_str() };
	m_list.InsertString(m_list.GetCount(), outstr);
	m_list.SetCurSel(m_list.GetCount() - 1);
}

void CGeneratorDlg::InsertInfo(const BigInt& big)
{
	CString outstr{};
	unsigned long data{ 0 };
	char num{ 0 };
	char temp;
	BigInt bigint{ big };
	//ȡ��������
	if (bigint.getsymbol())
	{
		outstr += "-";
	}

	static char table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	//	for (Vtor::reverse_iterator p = str.rbegin(); p != str.rend(); p++)
	UINT size = bigint.getVector().size();
	for (UINT i = 0; i <size; i++) //��Ԫ��
	{

		for (UINT j = 0; j < 8; j++)//ȡ����Ԫ��8���ַ�
		{	//�Ӹ�λ��ʼȡ		
			data = bigint.getVector()[(size - i - 1)];
			num = (data >> 4 * (7 - j)) & 0x000f;
			//���õ���Ӧ��
			temp = table[num];
			outstr += temp;
		}

	}

	m_list.InsertString(m_list.GetCount(), outstr);
	m_list.SetCurSel(m_list.GetCount() - 1);

}

//���غ���������ʾvector��ֵ
void CGeneratorDlg::InsertInfo(const Vtor& str, const bool& isnagetive )
{
	CString outstr{};
	unsigned long data{ 0 };
	char num{ 0 };
	char temp;
	//ȡ��������
	if (isnagetive)
	{
		outstr += "-";
	}

	static char table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	//	for (Vtor::reverse_iterator p = str.rbegin(); p != str.rend(); p++)
	for (UINT i = 0; i <str.size(); i++) //��Ԫ��
	{
			
		for (UINT j = 0; j < 8; j++)//ȡ����Ԫ��8���ַ�
		{	//�Ӹ�λ��ʼȡ		
			data = str[(str.size()-i-1)];
			num = (data >> 4 * (7-j))&0x000f;
			//���õ���Ӧ��
			temp = table[num];
			outstr += temp;
		}
		
	}

	m_list.InsertString(m_list.GetCount(), outstr);
	m_list.SetCurSel(m_list.GetCount() - 1);
}

void CGeneratorDlg::output(const Rsa& rsa)
{
	InsertInfo(_T("////////////////////////////"));
	InsertInfo(_T("e="));
	InsertInfo(rsa.e);
	InsertInfo(_T("N="));
	InsertInfo(rsa.N);
	InsertInfo(_T("d="));
	InsertInfo(rsa._d);
	InsertInfo(_T("p="));
	InsertInfo(rsa._p);
	InsertInfo(_T("q="));
	InsertInfo(rsa._q);
	InsertInfo(_T("////////////////////////////"));
}


void CGeneratorDlg::OnBnClickedButton1()//������Կ
{

	// TODO: Add your control notification handler code here
	CString temp;
	string instr;
	string instr2;
	
//	string s2 = "CAD9";
//	string s3 = "ABDDF";
//	long int k = -1250001988765;

	
	//���԰�stringת���CString 

	UpdateData(true);
	
	instr = CW2A(m_str.GetString());//��CStringת��Ϊstring  Unicode
	BigInt BigA{ instr };
	InsertInfo(_T("BigA="));
	InsertInfo(BigA);

	instr2 = CW2A(m_str2.GetString());	
	BigInt BigB{ instr2 };
	InsertInfo(_T("BigB="));
	InsertInfo(BigB);

	
	/*
	//��֤��������
	BigInt result;

	result = BigA + BigB;
	InsertInfo(_T("BigA+BigB="));
	InsertInfo(result);

	result = BigA - BigB;
	InsertInfo(_T("BigA-BigB="));
	InsertInfo(result);

	result = BigA*BigB;
	InsertInfo(_T("BigA*BigB="));
	InsertInfo(result);

	BigInt ca;
	result = BigA / BigB;
	InsertInfo(_T("BigA/BigB="));
	InsertInfo(result);

	result=BigA.gcd(BigA, BigB);
	InsertInfo(_T("bigA bigB�����Լ����"));
	InsertInfo(result);
	*/
	/*

	result = BigA.add(BigB);//bigA��ı�
	InsertInfo(_T("BigAadd(BigB)="));
	InsertInfo(result);

	

	result = BigA.sub(BigB);
	InsertInfo(_T("SUMsub(BigB)="));
	InsertInfo(result);
	
	

	result=BigA.multi(BigB);
	InsertInfo(_T("BigAmulti��BigB��="));
	InsertInfo(result);
	
	

	result=BigA.div(BigB, ca);
	InsertInfo(_T("��div(BigB,ca)="));
	InsertInfo(result);

	InsertInfo(_T("����ca="));
	InsertInfo(ca);
	*/
	
	/*
	s1 = createOddNum(m_num);//��������ַ���

	temp = _T("������ɵ��ַ���:");
	temp += s1.c_str();
	InsertInfo(temp);
	*/
	
	/*
	
	InsertInfo(_T("bigA bigB�����Լ����"));
	InsertInfo(BigA.gcd(BigA, BigB));

	*/
	

//	Rsa rsa; //�ڳ�ʼ��ʱ����pRsaָ�����

	

	if (m_randprime_flg==1)
	{
		pRsa->init(m_num / 2);
		//	InsertInfo(_T("���ɵĹ�Կ(e,N)="));
		//	InsertInfo(rsa.e);
		//	InsertInfo(rsa.N);
		output(*pRsa);
	}
	else
	{		
		pRsa->init(BigA, BigB);
		output(*pRsa);
	}
	/*
	InsertInfo(_T("e N�����Լ����"));
	InsertInfo(BigA.gcd(pRsa->e, pRsa->N));
	*/
	
	/*
	if (rsa.testprime(BigA))
		InsertInfo(_T("bigA��������"));
	else	
		InsertInfo(_T("bigA����������"));
	
	if (rsa.testprime(BigB))
		InsertInfo(_T("bigB��������"));
	else
		InsertInfo(_T("bigB����������"));

		*/
	
	
	

	/*
	BigInt bigA{ s3 };
	bigA.leftShift(16);
	InsertInfo(_T("���ƺ�bigA="));
	InsertInfo(bigA.getVector(),bigA.getsymbol());

	bigA.rightShift(200);
	InsertInfo(_T("���ƺ�bigA="));
	InsertInfo(bigA.getVector(), bigA.getsymbol());


	BigInt bigB{ s2 };
	InsertInfo(_T("bigB="));
	InsertInfo(bigB.getVector(), bigB.getsymbol());



	
	bigA.add(bigB);
	InsertInfo(_T("bigA+bigB="));
	InsertInfo(bigA.getVector(), bigA.getsymbol());
	*/
	/*
	unsigned long a{ 4211081216 };
	temp.Format(_T("a��%lu"), a);
	InsertInfo(temp);
	unsigned long b{ 4211081216 };
	temp.Format(_T("b��%lu"), b);
	InsertInfo(temp);

	temp.Format(_T("a+b=%lu"), a+b);
	InsertInfo(temp);
	*/

	


}


void CGeneratorDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	switch (IsDlgButtonChecked(IDC_CHECK1))
	{
	case BST_CHECKED:
		m_randprime_flg = 1;
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
		break;
	case BST_UNCHECKED:
		m_randprime_flg = 0;		
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
		break;

	default:
		break;
	}
	
}


void CGeneratorDlg::OnBnClickedButton2()//����
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	string plain = CW2A(inpText.GetString());//��CStringת��Ϊstring  Unicode
	BigInt plaintext{ plain };//����
	m_plainText = plaintext;
	
	//����ת����BigInt ���������

	InsertInfo(_T("������:"));
	InsertInfo(m_plainText);

	m_cipherText=pRsa->encryptByPu(plaintext);
	InsertInfo(_T("������:"));
	InsertInfo(m_cipherText);


}


void CGeneratorDlg::OnBnClickedButton3()//����
{
	// TODO: Add your control notification handler code here
	m_plainText = pRsa->decodeByPr(m_cipherText);
	InsertInfo(_T("���ܺ������:"));
	InsertInfo(m_plainText);
}
