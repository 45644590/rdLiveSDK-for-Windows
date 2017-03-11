#pragma once
#include "resource.h"
#include "afxwin.h"

// CDialogElement �Ի���

class CDialogElement : public CDialog
{
	DECLARE_DYNAMIC(CDialogElement)

public:
	CDialogElement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogElement();

	ULONG_PTR m_gdiplusToken;
	
// �Ի�������
	enum { IDD = IDD_ELEMENT };
	void Init();
	HCHIP m_HChip;
	IScreen_SCapParams		m_sScrCapParams;
	IPinInput_SStatusInfo	m_sChipStatus;
	IPinInput_EClass		m_eClassType;
	CString			m_szSource;
	BOOL  m_BisSelected;//�Ƿ�ѡ��
	int m_cy;//������
	CBrush	m_Bursh;
	CDialog * m_pant;//������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//BOOL LoadImageFromResource(CImage *pImage, UINT nResID,LPCTSTR lpTyp);
	DECLARE_MESSAGE_MAP()
public:
	void DrawImage(  UINT ImageID );
	void SetElementName();//����Ԫ����
	BOOL GetSelected();
	void SetSelected( BOOL bIsSelected );
	virtual BOOL PreTranslateMessage(MSG* pMsg);//win�Զ�����Ϣ����
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC *pDC,CWnd *pWnd,UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	BOOL m_BIsShow;//true Ϊ�ԣ� false Ϊ����
	BOOL m_BIsLockAspect;//true Ϊ�������ı���
	BOOL m_BIsPosition;//true Ϊ������λ��
	BOOL m_BIsSize;//true Ϊ������ק
	BOOL m_BIsAngle;//true Ϊ����ת
public:
	afx_msg void OnBnClickedCheckShowHide();//Ԫ������
	afx_msg void OnBnClickedCheck5();//��������
	afx_msg void OnBnClickedCheck6();//����ͼ��λ��
	afx_msg void OnBnClickedCheck7();//���������ק
	afx_msg void OnBnClickedCheck8();//������ת
	afx_msg void OnBnClickedButton1();//ͼ��������
	afx_msg void OnBnClickedButton2();//ͼ��ƽ��
	CStatic m_Image;
	CButton m_SH_Button;
	CButton m_WH_Btton;
	CButton m_POS_Button;
	CButton m_siz_Button;
	CButton m_rotating_Button;
};
