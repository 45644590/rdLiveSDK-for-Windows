#pragma once


// CNameDialog �Ի���

class CNameDialog : public CDialog
{
	DECLARE_DYNAMIC(CNameDialog)

public:
	CNameDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNameDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bIsConfirm;
	LPCWSTR m_szName;
	afx_msg void OnBnClickedOk();
};
