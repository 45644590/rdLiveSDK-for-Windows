// NameDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RDLiveSdkDemo.h"
#include "NameDialog.h"


// CNameDialog �Ի���

IMPLEMENT_DYNAMIC(CNameDialog, CDialog)

CNameDialog::CNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNameDialog::IDD, pParent)
{
	m_szName = NULL;
	m_bIsConfirm = FALSE;
}

CNameDialog::~CNameDialog()
{
}

void CNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNameDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CNameDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CNameDialog ��Ϣ�������

void CNameDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( m_szName != NULL )
	{
		m_bIsConfirm = TRUE;
		CString str;
		GetDlgItem(IDC_Name)->GetWindowText(str);
		m_szName = str;
	}
	OnOK();
}
