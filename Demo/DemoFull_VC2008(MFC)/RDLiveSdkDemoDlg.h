
// Demo_VC2008(MFC)Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "scenario.h"
#include "SettingDialog.h"
#include "afxwin.h"

// CDemoRdLiveDlg �Ի���
class CDemoRdLiveDlg : public CDialog
{
// ����
public:
	CDemoRdLiveDlg(CWnd* pParent = NULL);	// ��׼���캯��
	//������Ϊ��ʵ���࣬ͨ�� Instance() ȡ��ʵ��
	static CDemoRdLiveDlg& Instance();

	enum { IDD = IDD_DEMO_VC2008MFC_DIALOG };
	//�ѽ����ϵ����ø��µ������ļ���SDK������
	BOOL SaveProfile();
	//���س��������ļ�
	BOOL LoadScenes();
	//���泡�������ļ�
	BOOL SaveScenes();

	SettingDialog * m_settingDialog;//�������ý���
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	virtual BOOL PreTranslateMessage(MSG* pMsg);//win�Զ�����Ϣ����
	BOOL	m_bChipListChanging;	//��ǣ���ǰ��Ԫ���б���޸��ǳ����Լ����õģ��������û���������˲���Ҫ���¼���Ӧ�н����κδ���
	BOOL GetIsLoading();//�Ƿ���سɹ�

	CMenu * CreaMenuAudioCapture( EAudioCaptureDev audioCapTureDev , INT menuId );//�����豸
	void SetAudioCaptureSetting();//������Ƶ
	void SaveAudioCaptureSetting();//������Ƶ
	void on_EncoderNotifyCB( IEncoder_ENotify eNotify, ULONG dwValue, ULONG ptrValue );
// ʵ��
protected:
	CString GetattribValue( LPWSTR szattrib ,CComPtr<MSXML2::IXMLDOMNode>  pChildNode);
	void AppendMenuData( CMenu * pMenu, int NumberData, int Count, LPWSTR Name);
	bool m_bIsLoading;//�Ƿ���سɹ�
	HICON m_hIcon;
	int m_iNumberScenarios;//��������
	BOOL m_bAddSceneBut;//��ӳ�����ť
	BOOL m_bScrollBarSize;//������
	BOOL m_bSceneName;//��ʾ��������
	BOOL m_bCurLikeBScene;//��̨��ʾ��ǰ����
	FLOAT m_fEnterScale;//������볡���Ŵ�
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void UpdateVideoSizeLabel();//��Ƶ�ֱ��� ֡�� ����ʾ
private:
	
	void ClearStateMenu();//����˵�״̬
	void SetCurrentOptions( CMenu * pMenu, int width ,LPWSTR szList,int MAX, int MIN  );//���ò˵�
	//  SerialNumber 0Ϊ�ֱ���  1Ϊ ֡��  2Ϊ����
	CMenu * CreaoadMenu( LPWSTR szList,LPWSTR szattrib ,CString * szName, UINT UResolutionRatio , int SerialNumber);//������Ӧ�Ĳ˵�
	void LoadMenuinformation();
	//��Ƶ�豸
	//������
	CMenu * m_MenuSpeaker;
	//MIC
	CMenu * m_MenuMIC;
	//�ֱ���
	CMenu * m_MenuFilm;//16:9
	CMenu * m_MenuTelevision;//4:3
	CMenu * m_MenuCIF;//CIF
	CMenu * m_MenuUser;//�Զ���
	CMenu * m_MenuFps;//֡��
	CMenu * m_MenuWH;//����
	CMenu * m_MenuSwitch;//�л�
	CMenu * m_MenuDletel;//ɾ������
	CMenu * m_MenuRename;//����������
	CMenu * m_MenuLayout;//Ԥ������
	CMenu * m_MenuLocation;//��ʾ��λ��
	//���沼�ֵ���
	void SetAdjustLayout();
	//���������ļ�
	BOOL LoadProfile();
	
	//�������ļ����������õ������SDK
	BOOL SettingToUiAndSdk();

	BOOL m_bPreviewSizeing;
	IGlRender_SPreviewLayout m_sPerviewLayout;	//Ԥ������
	CMenu		m_menuMain;
	scenario *   m_scenario;//������������

	void DrawPreviewImage( INT iWidth, INT iHeight, LPBYTE pImageBuffer );//������OpenGLͼ�����
	CRect m_WIndrc;
public:
	
	static VOID WINAPI onPreviewImageCB( INT iWidth, INT iHeight, LPBYTE pImageBuffer, LPVOID pCbParam );//ͼ�����ݻص�
	//��������
	void SetPreviewLayoutToMenu( BOOL bRestore = FALSE );
	CComPtr<MSXML2::IXMLDOMDocument> m_sScenes;//������Ŀ¼
	CComPtr<MSXML2::IXMLDOMDocument> m_sOptionDoc;//option��Ŀ¼
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);//�˵���������
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg  void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	void AddScene();
	void UpdatePreviewConfig( IGlRender_SPreviewLayout::EReposWnd eMode );
	static VOID WINAPI RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam );//�����ص�
	static VOID WINAPI EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam );//����ص�

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tabctrl;
	afx_msg void onTcnSelchang(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedStaticFps();
	afx_msg void OnClose( );
protected:
	CComPtr<MSXML2::IXMLDOMNode> Createnode( LPWSTR nodename );
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonMicequipment();
	CSliderCtrl m_Speaker_Slider;
	CSliderCtrl m_MIC_Slider;
	CButton m_Speaker_Button;
	CButton m_MIC_Button;
	CButton m_Button_LiveCease;
	CButton m_Button_LiveBegin;
	afx_msg void OnBnClickedButtonLivecease();//ֹͣ
	afx_msg void OnBnClickedButtonLivebegin();//��ʼ
	afx_msg void OnBnClickedCheckAudioselect();//��Ƶ
	afx_msg void OnBnClickedCheck1Micselect();
	afx_msg void OnNMReleasedcaptureSliderAudioVolnm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderMicVolnm(NMHDR *pNMHDR, LRESULT *pResult);
};
