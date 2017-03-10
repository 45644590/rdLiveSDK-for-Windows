#pragma once
#include "RollingDialog.h"

// SettingDialog �Ի���

class SettingDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingDialog)

public:
	SettingDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SettingDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETUP };
	int m_selectCount;//ѡ����ʾ������
	CRollingDialog * m_RollingDialog;//������
	//VideoDialog * m_videoDialog;//��Ƶ����
	BOOL SaveEncoderSetting(BOOL bPopMsg = FALSE );//��Ƶ����Ƶ������ı�������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonVideoEncode();
	afx_msg void OnBnClickedButtonAdioEncode();
	afx_msg void OnBnClickedButtonEncodeOut();
};
