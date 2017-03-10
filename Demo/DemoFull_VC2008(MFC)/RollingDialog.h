#pragma once
#include "VideoDialog.h"
#include "odingOutDialog.h"
#include "AudioCodingDialog.h"
// CRollingDialog �Ի���

class CRollingDialog : public CDialog
{
	DECLARE_DYNAMIC(CRollingDialog)

public:
	CRollingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRollingDialog();
	int iDisplayingSerialNumber;//��ʾ���
	VideoDialog * m_VideoDialog;//��Ƶ��������
	int m_VideoDialog_width;//��Ƶ�����
	int m_VideoDialog_height;//��Ƶ�����

	CodingOutDialog * m_codingOutDialog;//�������
	int m_codingOutDialog_width;//���������
	int m_codingOutDialog_height;//���������

	AudioCodingDialog * m_AudioCOdingDialog;//��Ƶ����
	int m_AudioCOdingDialog_width;//��Ƶ�����
	int m_AudioCOdingDialog_height;//��Ƶ�����
	void AdjustSize();
// �Ի�������
	enum { IDD = IDD_DIALOG4 };
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_iEdtPos;
	void MoveWindows( int ipos );
	void Init();
	BOOL saveEncod( BOOL bPopMsg  = FALSE );//�������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//����Ķ������߰�ť
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);//���ֵĶ���
	afx_msg void OnNMThemeChangedScrollBar(NMHDR *pNMHDR, LRESULT *pResult);
};
