
// GeneratorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include"BigInt.h"
#include"Rsa.h"



// CGeneratorDlg dialog
class CGeneratorDlg : public CDialogEx
{
// Construction
public:
	CGeneratorDlg(CWnd* pParent = NULL);	// standard constructor
	

	void InsertInfo(const CString& str);

	void InsertInfo(const string& str);	
	
	void InsertInfo(const Vtor& str, const bool& isnagetive=false);
	void InsertInfo(const BigInt& num);

	void output(const Rsa& rsa);
// Dialog Data
	enum { IDD = IDD_GENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	bool m_randprime_flg = 0;
public:
	CListBox m_list;
	UINT m_num;
	afx_msg void OnBnClickedButton1();
	CString m_str;
	CString m_str2;
	
	afx_msg void OnBnClickedCheck1();
	
	CString inpText;//文本输入

	BigInt m_plainText;//明文
	BigInt m_cipherText;//密文
	Rsa* pRsa;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
