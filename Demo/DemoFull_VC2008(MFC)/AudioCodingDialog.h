#pragma once
#include "SpinBox.h"

// AudioCodingDialog �Ի���

class AudioCodingDialog : public CDialog
{
	DECLARE_DYNAMIC(AudioCodingDialog)

public:
	AudioCodingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AudioCodingDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };
	void Init();//��ʼ��
	void SaveAuido();//��Ƶ���뱣��
	CSpinBox * m_RateBitSpinButton;//��Ƶ����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdownCombo1();
	afx_msg void OnBnClickedRadiochannelstate();
	afx_msg void OnBnClickedRadiochannelsingle();
};
