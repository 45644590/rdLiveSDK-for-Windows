// SpinBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RDLiveSdkDemo.h"
#include "SpinBox.h"


// CSpinBox �Ի���

IMPLEMENT_DYNAMIC(CSpinBox, CDialog)

CSpinBox::CSpinBox(CWnd* pParent /*=NULL*/)
	: CDialog(CSpinBox::IDD, pParent)
{
	m_SPinNumber = 1;
	m_MInSpinNumber = 0;
	m_MaxSpinNumber = 100;
}

CSpinBox::~CSpinBox()
{
}

void CSpinBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpinBox, CDialog)
	ON_EN_UPDATE(IDC_EDIT_Text_COUNT, &CSpinBox::OnEnUpdateEditTextCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COUNT, &CSpinBox::OnDeltaposSpinCount)
END_MESSAGE_MAP()


// CSpinBox ��Ϣ�������

void CSpinBox::OnEnUpdateEditTextCount()
{
	CString CszCount;
	((CEdit*)GetDlgItem(IDC_EDIT_Text_COUNT))->GetWindowText(CszCount);
	m_iRealDataNumber = _wtoi(CszCount);
	bool bIsModify = false;
	if ( m_iRealDataNumber < m_MInSpinNumber )
	{
		m_iRealDataNumber  = m_MInSpinNumber;
		bIsModify = true;
	}
	if ( m_iRealDataNumber > m_MaxSpinNumber )
	{
		m_iRealDataNumber  = m_MaxSpinNumber;
		bIsModify = true;
	}

	if ( bIsModify )
	{
		CString Duration;
		Duration.Format(L"%d", m_iRealDataNumber);
		((CEdit*)GetDlgItem(IDC_EDIT_Text_COUNT))->SetWindowText(Duration);
	}
	
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CSpinBox::OnDeltaposSpinCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if(pNMUpDown->iDelta>0)  
	{  
		m_iRealDataNumber -= m_SPinNumber;
		if ( m_iRealDataNumber < m_MInSpinNumber )
		{
			m_iRealDataNumber  = m_MInSpinNumber;
		}
	}  
	if(pNMUpDown->iDelta<0)  
	{  
		m_iRealDataNumber += m_SPinNumber;
		if ( m_iRealDataNumber > m_MaxSpinNumber )
		{
			m_iRealDataNumber  = m_MaxSpinNumber;
		}
	}  
	CString Duration;
	Duration.Format(L"%d", m_iRealDataNumber);
	((CEdit*)GetDlgItem(IDC_EDIT_Text_COUNT))->SetWindowText(Duration);
}

void CSpinBox::Init( int MaxNumber, int MinNumber ,int spinNumber )
{
	m_MInSpinNumber = MinNumber;
	m_MaxSpinNumber = MaxNumber;
	m_SPinNumber = spinNumber;

	CString Duration;
	Duration.Format(L"%d", MinNumber);
	((CEdit*)GetDlgItem(IDC_EDIT_Text_COUNT))->SetWindowText(Duration);
}

void CSpinBox::SetEndleWindows( int isEndele )
{
	GetDlgItem(IDC_EDIT_Text_COUNT)->EnableWindow( isEndele );
	GetDlgItem(IDC_SPIN_COUNT)->EnableWindow(isEndele);
}	

void CSpinBox::SetCurrentNumber( int CurrentNumber )
{
	if ( (CurrentNumber > m_MInSpinNumber) && (CurrentNumber <= m_MaxSpinNumber) )
	{
		m_iRealDataNumber  = CurrentNumber;
	}
	else
	{
		m_iRealDataNumber = m_MInSpinNumber;
	}

	CString Duration;
	Duration.Format(L"%d", m_iRealDataNumber);
	((CEdit*)GetDlgItem(IDC_EDIT_Text_COUNT))->SetWindowText(Duration);
}
