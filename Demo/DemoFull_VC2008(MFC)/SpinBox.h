#pragma once


// CSpinBox �Ի���

class CSpinBox : public CDialog
{
	DECLARE_DYNAMIC(CSpinBox)

public:
	CSpinBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpinBox();

// �Ի�������
	enum { IDD = IDD_DIALOG_Spin_Text };
	int m_MaxSpinNumber;//���ֵ
	int m_MInSpinNumber;//��Сֵ
	
	void SetCurrentNumber( int CurrentNumber );//���õ�ǰ����
	void SetEndleWindows( int isEndele );//��������
	int m_SPinNumber;//ÿ�ε��spin��ӵ�����
	int m_iRealDataNumber;//��ʵ����
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	int m_spinintBite;//
	DECLARE_MESSAGE_MAP()
public:
	void Init( int MaxNumber, int MinNumber ,int spinNumber = 0 );
	afx_msg void OnEnUpdateEditTextCount();
	afx_msg void OnDeltaposSpinCount(NMHDR *pNMHDR, LRESULT *pResult);
};
