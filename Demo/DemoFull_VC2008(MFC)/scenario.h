#pragma once

#include "ArrayDialogElement.h"
// scenario �Ի���

class scenario : public CDialog
{
	DECLARE_DYNAMIC(scenario)

public:
	scenario(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~scenario();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	BOOL OnRenderNotify( IRender_ENotify eNotify, WPARAM wParam, LPARAM lParam );//Ԫ���б仯
	CArrayDialogElement * m_ArrayDialogElement;//Ԫ������

	int m_iEdtPos;
	void MoveWindows( int ipos );
	int m_Height;//��ʵ�߶�Ԫ���б�
	CMenu * m_AddButtonMenu;//��ӳ����˵�
	void AddChipMenu(CRect rc  ,CMenu * pMenu  , bool bIsmen);

	CMenu * m_MenuScreen;//��Ļ/���� �˵�
	int m_ScreenCount;//��Ļ ����
	int m_HWndCount;//���� ����
	CMenu * m_MenuCamera;//����ͷ�˵�
	int m_CameraCount;//����ͷ ����

	int m_iScreenCount;//���ڵ�����
	int m_iCameraCount;//����ͷ������

	CArray <HWND> * m_HWndList;//���ھ��
	CString GetImageDir();//ѡ��ͼƬ·��
protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
	
public:
	void adjustmentOnComponentsListLayout();//Ԫ���б��ֵ���
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int HWndCountW();

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����Ķ������߰�ť
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);//���ֵĶ���
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonMoveDown();
};
