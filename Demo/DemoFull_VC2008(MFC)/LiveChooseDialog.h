#pragma once
#include "resource.h"
#include "afxwin.h"


// CLiveChooseDialog �Ի���

class CLiveChooseDialog : public CDialog
{
	DECLARE_DYNAMIC(CLiveChooseDialog)

public:
	CLiveChooseDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLiveChooseDialog();
	CBrush	m_Bursh;
	int m_cy;//������
	BOOL  m_BisSelected;//�Ƿ�ѡ��
	// �Ի�������
	enum { IDD = IDD_DIALOG_Live };
	CString m_StrDisName;
	void Init( CString StrDisName , bool Checke = FALSE );
	CDialog * m_pant;//������
	void SetSelected( BOOL bIsSelected );
	BOOL  GetSelected();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);//win�Զ�����Ϣ����
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC *pDC,CWnd *pWnd,UINT nCtlColor);
	CStatic m_Stat_Live_Text;
	CButton m_Check_Live_Choose;
};
