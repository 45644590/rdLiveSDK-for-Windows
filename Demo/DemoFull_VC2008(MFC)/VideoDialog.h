#pragma once
#include "SpinBox.h"

// VideoDialog �Ի���

class VideoDialog : public CDialog
{
	DECLARE_DYNAMIC(VideoDialog)

public:
	VideoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~VideoDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_Video };
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//bool m_bIsFist;//ֻ���е�һ��
	CSpinBox * AddSpinButton( int MaxNumber, int MinNumber, int top, UINT uID, int spinNumber  );//���spinButton
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void SetQualityComboBox( int index );//���������ο��������б�
	DECLARE_MESSAGE_MAP()
public:
	void Init();//��ʼ��
	void SaveProfile();//����������Ϣ
	//���ʴ���
	int m_spinintBite;
	afx_msg void OnDeltaposBitrateSpinBite(NMHDR *pNMHDR, LRESULT *pResult);
	//��Ƶ����������
	int m_iEncoderIndex;
	afx_msg void OnCbnSelendokEncoderComboxEncode();
	afx_msg void OnCbnDropdownEncoderComboxEncode();
	//����ģʽ
	afx_msg void OnCbnSelendokRateModelComboxRateModel();
	//��Ƶ����
	afx_msg void OnCbnSelendokQuelityReferenceComboxQuality();

private:
	CSpinBox * m_BitRateSpinButton;//����
	CSpinBox * m_MaximumSpinButton;//�������
	CSpinBox * m_VBVSpinButton;//VBV����
	CSpinBox * m_IDRSpinButton;//IDR ���
	CSpinBox * m_I_Fps_SpinButton;//I ֡���
	CSpinBox * m_Ref_Quote_SpinButton;//Ref ����
	CSpinBox * m_B_Fps_SpinButton;//B֡���
};
