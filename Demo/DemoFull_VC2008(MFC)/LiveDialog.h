#pragma once
#include "afxwin.h"
#include "LiveChooseDialog.h"
#include "ArrayLiveChooseDialog.h"
// CLiveDialog �Ի���

class CLiveDialog : public CDialog
{
	DECLARE_DYNAMIC(CLiveDialog)

public:
	CLiveDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLiveDialog();

	ArrayLiveChooseDialog * m_ArrayDialogElement;
// �Ի�������
	int m_Height;
	enum { IDD = IDD_DIALOG4 };
	int m_iEdtPos;
	void AddLiveChooseDialog( CString StrDisName , bool Checke = FALSE );
	void adjustmentOnComponentsListLayout( );
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void MoveWindows( int ipos );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����Ķ������߰�ť
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);//���ֵĶ���
	CScrollBar m_ScrollBar_Bar;
};
