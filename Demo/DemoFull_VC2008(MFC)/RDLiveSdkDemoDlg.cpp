
// Demo_VC2008(MFC)Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RDLiveSdkDemo.h"
#include "RDLiveSdkDemoDlg.h"
#include "NameDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//m_sPerviewLayout = new IGlRender_SPreviewLayout;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDemoRdLiveDlg �Ի���




CDemoRdLiveDlg::CDemoRdLiveDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDemoRdLiveDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_scenario = NULL;
	m_settingDialog = NULL;
	m_MenuSpeaker = NULL;
	m_MenuMIC = NULL;
}

void CDemoRdLiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
	DDX_Control(pDX, IDC_SLIDER_Audio_Volnm, m_Speaker_Slider);
	DDX_Control(pDX, IDC_SLIDER_MIC_Volnm, m_MIC_Slider);
	DDX_Control(pDX, IDC_CHECK_Audio_select, m_Speaker_Button);
	DDX_Control(pDX, IDC_CHECK1_MIC_select, m_MIC_Button);
	DDX_Control(pDX, IDC_BUTTON_LiveCease, m_Button_LiveCease);
	DDX_Control(pDX, IDC_BUTTON_LiveBegin, m_Button_LiveBegin);
}

BEGIN_MESSAGE_MAP(CDemoRdLiveDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDemoRdLiveDlg::onTcnSelchang)
	ON_STN_CLICKED(IDC_STATIC_FPS, &CDemoRdLiveDlg::OnStnClickedStaticFps)
	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_BUTTON_Audio_equipment, &CDemoRdLiveDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_MIC_equipment, &CDemoRdLiveDlg::OnBnClickedButtonMicequipment)
	ON_BN_CLICKED(IDC_BUTTON_LiveCease, &CDemoRdLiveDlg::OnBnClickedButtonLivecease)
	ON_BN_CLICKED(IDC_BUTTON_LiveBegin, &CDemoRdLiveDlg::OnBnClickedButtonLivebegin)
	ON_BN_CLICKED(IDC_CHECK_Audio_select, &CDemoRdLiveDlg::OnBnClickedCheckAudioselect)
	ON_BN_CLICKED(IDC_CHECK1_MIC_select, &CDemoRdLiveDlg::OnBnClickedCheck1Micselect)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_Audio_Volnm, &CDemoRdLiveDlg::OnNMReleasedcaptureSliderAudioVolnm)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_MIC_Volnm, &CDemoRdLiveDlg::OnNMReleasedcaptureSliderMicVolnm)
END_MESSAGE_MAP()


// CDemoRdLiveDlg ��Ϣ�������

BOOL CDemoRdLiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	m_bIsLoading = false;
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	m_iNumberScenarios = 8;
	m_bAddSceneBut = TRUE;
	m_bScrollBarSize = false;
	m_bSceneName = TRUE;
	m_bCurLikeBScene = TRUE;
	m_fEnterScale = TRUE;
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_scenario = new scenario(this->GetDlgItem(IDC_TAB1));

	m_scenario->Create(IDD_DIALOG1,this->GetDlgItem(IDC_TAB1));

	m_settingDialog = new SettingDialog(this->GetDlgItem(IDC_TAB1));

	m_settingDialog->Create(IDD_DIALOG_SETUP,this->GetDlgItem(IDC_TAB1));


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if ( !RDLive_Init( L"17RD", L"RDLiveSdkDemo",RenderNotifyCB, EncoderNotifyCB,this  ) )
	{
		AfxMessageBox(L"��ʼ��ʧ�ܣ�");
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}

	if ( !RDLive_ResetAccredit( "d3200cc987431827|RDLiveSDK_Demo.exe", 
		"77a9eeea008524e8bdf10e18409cbdb3sULczML4CjomZFtst04v/HLUrHqWT72Mmkz7WhUEmpjXMH7/UWz5oGMwUGQPbYX+MKSpM01lSGQ/qNzCkFFyKXSwxrKIViR4iZ7ZxOuB6n80wDeCV7jHJSEN1+DqlCLm3dJWQF3CFLMOj2YJxwI/YDY9h3SjCsWFz9j/71RCHH0FWpr13vMRM6a1uRCnke2Tyly/V4S4E7BE1tR6WDcxNQTeX9w399l/EpNb8LvBNNUz6shNmM627BGBfTbPG2vj+grPaxv1rFcVRqNkT45Jrjvjp3PV8L6Py7fCUvK5PJ0Pb/olb9q/M2Yom+AZkSlE0FDcSKb0MG+QCE9f1MYacjFoU31o7cZb5ZQZ++7lMqXMDvTi9LyTYR+0lDKKwFC8EJ43/upbIuhawyXQ2w4u7Zvv9IUXqhamlTUirPmuV4lSVypdzCT+gPdEjq9krLLjRajAMutBwefKiHdrp/h65BxTErT94rH7OAU6bCmbX/o=" ) )
	{
	}
	
	CRect rc;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE);//IDC_PIC_2DΪ�ؼ�ID
	pWnd->GetClientRect(&rc);//rcΪ�ؼ��Ĵ�С��
	int x=rc.Height();
	int y=rc.Width();

	Render_SetSize( 1920, 1080 );


	m_bIsLoading = LoadProfile() ? true : false;




	m_menuMain.LoadMenu( IDR_MENU_MAIN );
	SetMenu( &m_menuMain );

	LoadMenuinformation();

	LoadScenes();

	SetAudioCaptureSetting();

	TCITEM tc1,tc2;

	tc1.mask = TCIF_TEXT;
	tc1.pszText = _T("��ǰ����");

	tc2.mask = TCIF_TEXT;
	tc2.pszText = _T("����");

	/*tc3.mask = TCIF_TEXT;
	tc3.pszText = _T("��������");*/


	m_tabctrl.InsertItem(0,&tc1 );
	m_tabctrl.InsertItem(1,&tc2 );
	//m_tabctrl.InsertItem(2,&tc3);
	CRect rec;
	m_tabctrl.GetClientRect(&rec);

	//��λѡ�ҳ��λ�ã�������Ը�������Լ�����ƫ����
	rec.bottom -= 2;
	rec.left += 1;
	rec.top += 27;
	rec.right -= 3;


	m_scenario->MoveWindow(&rec);

	m_scenario->ShowWindow(SW_SHOW);

	rec.top -= 5;

	m_settingDialog->MoveWindow(&rec);

	m_settingDialog->ShowWindow(SW_HIDE);

	CString m_Caption;
	m_Caption.LoadString( IDD_DIALOG_SETUP ); //ȡ��ť����

	m_settingDialog->MoveWindow(&rec);

	SetAdjustLayout();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDemoRdLiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDemoRdLiveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDemoRdLiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDemoRdLiveDlg::onTcnSelchang(NMHDR *pNMHDR, LRESULT *pResult)
{

	switch(m_tabctrl.GetCurSel())
	{
	case 0:m_scenario->ShowWindow(SW_SHOW);m_settingDialog->ShowWindow(SW_HIDE);break;
	case 1:m_scenario->ShowWindow(SW_HIDE);m_settingDialog->ShowWindow(SW_SHOW);break;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

BOOL CDemoRdLiveDlg::PreTranslateMessage( MSG* pMsg )
{

	return FALSE;
}

VOID WINAPI CDemoRdLiveDlg::RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam )
{
	((CDemoRdLiveDlg*)pCbParam)->PostMessage( WM_MYNotify + eNotify, hScene, iValue );
}

void CDemoRdLiveDlg::UpdatePreviewConfig( IGlRender_SPreviewLayout::EReposWnd eMode )
{
	float fScale = (float)_wtof(Getattrib(L"/Option/Base/PreviewScale/@Scale"));
	BOOL bHardware = _wtoi(Getattrib(L"/Option/Base/PreviewScale/@Hardware"));
	CRect rc;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE);//IDC_PIC_2DΪ�ؼ�ID
	pWnd->GetClientRect(&rc);//rcΪ�ؼ��Ĵ�С��
	int x=rc.Width();
	int y=rc.Height();


	//Render_SetSize( x*1.5, y*1.5 );
	//memset(&m_sPerviewLayout,0,sizeof(IGlRender_SPreviewLayout));
	m_sPerviewLayout	=  *Render_GetPreviewLayout();
	if ( eMode == IGlRender_SPreviewLayout:: eNotChangePos )
	{
		m_sPerviewLayout.fScale	= 0.0f;
	}
	else
	{
		m_sPerviewLayout.fScale	= fScale;
	}
	m_sPerviewLayout.hMainWnd	= m_hWnd;
	m_sPerviewLayout.siMinimumLimit.cx	= 100;//rc.right;
	m_sPerviewLayout.siMinimumLimit.cy	= 100;//rc.bottom;

	if ( bHardware )
	{
		if ( !m_sPerviewLayout.hMainWnd )		//���֮ǰ����ʹ�õ�OpenGL����Ԥ�������ھ���Ҫ����Ϊʹ�ô���Ԥ��
		{
		}
		m_sPerviewLayout.rtPreview.left	= 0;
		m_sPerviewLayout.rtPreview.right	= rc.right;
		m_sPerviewLayout.rtPreview.top	= 0;
		m_sPerviewLayout.rtPreview.bottom= rc.bottom;
		m_sPerviewLayout.hPreviewWnd	= pWnd->m_hWnd;
		m_sPerviewLayout.pCbFunction	= NULL;
		m_sPerviewLayout.pCbParam	= NULL;
	}
	else
	{
		if ( m_sPerviewLayout.hMainWnd )
		{
		}
		m_sPerviewLayout.rtPreview.left	= rc.left;
		m_sPerviewLayout.rtPreview.right	= rc.right;
		m_sPerviewLayout.rtPreview.top	= rc.top;
		m_sPerviewLayout.rtPreview.bottom= rc.bottom + 1;
		m_sPerviewLayout.hPreviewWnd	= 0;
		m_sPerviewLayout.pCbFunction	= onPreviewImageCB;
		m_sPerviewLayout.pCbParam		= pWnd;
	}
	m_sPerviewLayout.eReposWindow	= eMode;
	m_sPerviewLayout.bOnlyWorkArea	= TRUE;
	//���ݲ˵�������Ԥ������
	UpdateVideoSizeLabel();
	//QAction*	pAction		= m_agpBSceneCount->checkedAction();
	m_sPerviewLayout.iBSceneCount	= m_iNumberScenarios;
	m_sPerviewLayout.bAddSceneBut	= m_bAddSceneBut;
	m_sPerviewLayout.bSceneName		= m_bSceneName;
	m_sPerviewLayout.bCurLikeBScene	= m_bCurLikeBScene;
	m_sPerviewLayout.iScrollBarSize	= m_bScrollBarSize?12:0;
	//sLayout.bBSceneHolder	= FALSE;
	//sLayout.bScrollHolder	= FALSE;

	if ( Render_SetPreviewLayout( &m_sPerviewLayout ) )
	{
		if ( eMode != IGlRender_SPreviewLayout:: eNotChangePos )
		{
			m_bPreviewSizeing	= TRUE;
			MoveWindow(  m_sPerviewLayout.rtMainClient.left,m_sPerviewLayout.rtMainClient.top,
				m_sPerviewLayout.rtMainClient.right - m_sPerviewLayout.rtMainClient.left,
				m_sPerviewLayout.rtMainClient.bottom - m_sPerviewLayout.rtMainClient.top );
			m_bPreviewSizeing	= FALSE;
		}
	}

	LPBYTE	pData	= LPBYTE(&m_sPerviewLayout);
	char * cpData = new char[ sizeof( m_sPerviewLayout ) * 2 + 1 ];
	char * TmpcpData = cpData;
	for(int i = 0; i < sizeof( m_sPerviewLayout ); i++)  
	{  
		sprintf_s(TmpcpData, 3, "%02x", pData[i]);  
		TmpcpData += 2;  
	}
	*TmpcpData	= 0;
	CString str(cpData);
	delete []cpData;
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNode> spNode;
	spNode = spRootEle->selectSingleNode(OLESTR("/Option/PreviewLayout"));
	if ( spNode != NULL )
	{
		spNode->put_text(str.GetBuffer(str.GetLength())); //д��text
	}
}

BOOL CDemoRdLiveDlg::LoadProfile()
{
	//DOM
	m_sOptionDoc.CoCreateInstance(CLSID_DOMDocument);
	BOOL		bReplaceUserOpt	= FALSE;	//�Ƿ���Ҫ�滻�����û��������ļ�
	//ȡ�������ļ���·����ʹ���� RDLive_DefaultDir() �õ�ָ�����ļ��С�
	//Ĭ�������ļ���·����������ĳ�ʼ���ã����û������ļ�������ʱ������Ĭ�����õİ汾�и���ʱ�����滻���û������ļ���
	CString		szOptDefault( RDLive_DefaultDir( RDir_Application ) );
	szOptDefault +=  L"Option.xml";
	//�û������ļ���·�������û������ļ����ڣ��Ұ汾��Ĭ�������ļ�һ�£���ʹ���û������ļ���
	CString		szOptUser(RDLive_DefaultDir( RDir_Profile ) );
	szOptUser  += L"Option.xml";
	m_sOptionDoc->load((LPCTSTR)szOptUser);
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	m_sOptionDoc->get_documentElement(&spRootEle); //���ڵ�

	if ( spRootEle == NULL )
	{	
		CString source = szOptDefault ,target =RDLive_DefaultDir( RDir_Profile ) ;
		CreateDirectory(target,NULL); 
		CFileFind finder;  
		CString path;  
		path = source;    
		bool bWorking = finder.FindFile(path) ? true : false;
		while(bWorking){  
			bWorking = finder.FindNextFile() ? true : false;
			CopyFile(finder.GetFilePath(),target+"/"+finder.GetFileName(),FALSE);  
		} 
		//m_sOptionDoc->get_documentElement(&spRootEle); //���ڵ�

		m_sOptionDoc->load((LPCTSTR)szOptUser);
		//CComPtr<MSXML2::IXMLDOMElement> spRootEle;
		m_sOptionDoc->get_documentElement(&spRootEle); //���ڵ�
	}  


	
	CComPtr<MSXML2::IXMLDOMNodeList> spNodeList;
	spRootEle->get_childNodes(&spNodeList); //�ӽڵ��б�
	long nLen;
	spNodeList->get_length(&nLen); //�ӽڵ���
	if ( nLen <=0  )
	{
		return FALSE;
	}

	return TRUE;
}

void CDemoRdLiveDlg::AddScene()
{
	WCHAR SceneName[ 256 ] = {0};
	CNameDialog nameDialog;
	nameDialog.m_szName = SceneName;
	nameDialog.DoModal();

	if ( nameDialog.m_bIsConfirm )
	{
		HSCENE		hScene	= Render_CreateScene();
		Scene_SetName( hScene, SceneName );
	}
}

void CDemoRdLiveDlg::SetAdjustLayout()
{
	GetDlgItem(IDC_IMAGE);//Ԥ������
	GetDlgItem(IDC_STATIC_FPS);//֡�� ���ʵ���ϸ��Ϣ
	GetDlgItem(IDC_STATIC);//��ֱ����¼���йصĲ���
	GetDlgItem(IDC_TAB1);//��Ӧ���ý���

	CRect rc;
	//CWnd *pWnd = GetDlgItem(IDC_IMAGE);//IDC_PIC_2DΪ�ؼ�ID
	GetClientRect(&rc);//rcΪ�ؼ��Ĵ�С��
	//������ʾ��Ϣ
	CRect FPS_rc;
	GetDlgItem(IDC_STATIC_FPS)->GetClientRect(FPS_rc);
	FPS_rc.left = 0;
	FPS_rc.top = rc.Height() - FPS_rc.Height();
	FPS_rc.bottom = rc.Height() ;
	FPS_rc.right = rc.Width();
	GetDlgItem(IDC_STATIC_FPS)->MoveWindow(FPS_rc,TRUE);

	//�������ý���λ�����С
	int FPS_height = FPS_rc.Height();
	GetDlgItem(IDC_TAB1)->GetClientRect(FPS_rc);
	int TAB_FPS_width = FPS_rc.Width();
	int TAB_FPS_height = FPS_rc.Height();
	FPS_rc.left = rc.Width() - FPS_rc.Width();
	FPS_rc.top = 0;
	FPS_rc.bottom = rc.Height() - FPS_height ;
	FPS_rc.right = rc.Width();
	int Tab_Width = FPS_rc.Width();
	int Tab_height = FPS_rc.Height();
	GetDlgItem(IDC_TAB1)->MoveWindow(FPS_rc,TRUE);
	if ( m_scenario != NULL )
	{
		FPS_rc.left = 2;
		FPS_rc.top = 22;
		FPS_rc.bottom = Tab_height - 5;
		FPS_rc.right = Tab_Width - 5;
		m_scenario->MoveWindow(FPS_rc,TRUE);
	}
	if ( m_settingDialog != NULL )
	{
		FPS_rc.left = 2;
		FPS_rc.top = 22;
		FPS_rc.bottom = Tab_height - 5;
		FPS_rc.right = Tab_Width - 5;
		m_settingDialog->MoveWindow(FPS_rc,TRUE);
	}

	CRect Static_rc;
	GetDlgItem(IDC_STATIC)->GetClientRect(Static_rc);
	int Static_height = Static_rc.Height();
	
	//���� Ԥ�������λ�����С
	GetDlgItem(IDC_IMAGE)->GetClientRect(FPS_rc);
	
	FPS_rc.left = 0;
	FPS_rc.top = 0;
	FPS_rc.bottom = rc.Height() - 20 - Static_height - FPS_height;
	FPS_rc.right = rc.Width() - Tab_Width - 10;
	GetDlgItem(IDC_IMAGE)->MoveWindow(FPS_rc,TRUE);


	//����ֱ����¼�ư�ť��λ��
	Static_rc.left = 0;
	Static_rc.top = FPS_rc.Height() + 10;
	Static_rc.bottom = rc.Height() - 10 - FPS_height;
	Static_rc.right = FPS_rc.Width();
	GetDlgItem(IDC_STATIC)->MoveWindow(Static_rc,TRUE);
	//MIC
	CRect MIC_rc;
	GetDlgItem(IDC_BUTTON_MIC_equipment)->GetClientRect( MIC_rc );
	int MIC_height = MIC_rc.Height();
	int MIC_width = MIC_rc.Width();
	MIC_rc.right = Static_rc.right - 10;
	MIC_rc.left = MIC_rc.right - MIC_width;
	MIC_rc.top = Static_rc.top + 25;
	MIC_rc.bottom = MIC_height+MIC_rc.top ;
	GetDlgItem(IDC_BUTTON_MIC_equipment)->MoveWindow(MIC_rc,TRUE);
	CRect MIC_equipment;
	GetDlgItem(IDC_CHECK1_MIC_select)->GetClientRect( MIC_equipment );
	int MIC_equipment_height = MIC_equipment.Height();
	int MIC_equipment_width = MIC_equipment.Width();
	MIC_equipment.right = MIC_rc.left;
	MIC_equipment.left = MIC_equipment.right-MIC_equipment_width;
	MIC_equipment.top = MIC_rc.top;
	MIC_equipment.bottom = MIC_equipment_height+MIC_equipment.top ;
	GetDlgItem(IDC_CHECK1_MIC_select)->MoveWindow(MIC_equipment,TRUE);
	CRect MIC_Volnm;
	GetDlgItem(IDC_SLIDER_MIC_Volnm)->GetClientRect( MIC_Volnm );
	int MIC_Volnm_height = MIC_Volnm.Height();
	int MIC_Volnm_width = MIC_Volnm.Width();
	MIC_Volnm.right = Static_rc.right - 10;
	MIC_Volnm.left = MIC_Volnm.right - MIC_Volnm_width;
	MIC_Volnm.top = MIC_rc.bottom;
	MIC_Volnm.bottom = MIC_Volnm.top + MIC_Volnm_height;
	GetDlgItem(IDC_SLIDER_MIC_Volnm)->MoveWindow(MIC_Volnm,TRUE);

	//������
	CRect Audio_rc;
	GetDlgItem(IDC_BUTTON_Audio_equipment)->GetClientRect( Audio_rc );
	int Audio_height = Audio_rc.Height();
	int Audio_width = Audio_rc.Width();
	Audio_rc.right = MIC_Volnm.left;
	Audio_rc.left = Audio_rc.right - Audio_width;
	Audio_rc.top = Static_rc.top + 25;
	Audio_rc.bottom = Audio_height+Audio_rc.top ;
	GetDlgItem(IDC_BUTTON_Audio_equipment)->MoveWindow(Audio_rc,TRUE);
	CRect Audio_equipment;
	GetDlgItem(IDC_CHECK_Audio_select)->GetClientRect( Audio_equipment );
	int Audio_equipment_height = Audio_equipment.Height();
	int Audio_equipment_width = Audio_equipment.Width();
	Audio_equipment.right = Audio_rc.left;
	Audio_equipment.left = Audio_equipment.right-Audio_equipment_width;
	Audio_equipment.top = Static_rc.top + 25;
	Audio_equipment.bottom = Audio_equipment_height+Audio_equipment.top ;
	GetDlgItem(IDC_CHECK_Audio_select)->MoveWindow(Audio_equipment,TRUE);
	CRect Audio_Volnm;
	GetDlgItem(IDC_SLIDER_Audio_Volnm)->GetClientRect( Audio_Volnm );
	int Audio_Volnm_height = Audio_Volnm.Height();
	int Audio_Volnm_width = Audio_Volnm.Width();
	Audio_Volnm.right = MIC_Volnm.left;
	Audio_Volnm.left = Audio_Volnm.right - Audio_Volnm_width;
	Audio_Volnm.top = Audio_rc.bottom;
	Audio_Volnm.bottom = Audio_Volnm.top + Audio_Volnm_height;
	GetDlgItem(IDC_SLIDER_Audio_Volnm)->MoveWindow(Audio_Volnm,TRUE);
	//UpdatePreviewConfig(IGlRender_SPreviewLayout::eNotChangePos);

	CRect LiveCease_rc;
	m_Button_LiveCease.GetClientRect(LiveCease_rc);
	int LiveCease_Width = LiveCease_rc.Width();
	int LiveCease_Height = LiveCease_rc.Height();
	LiveCease_rc.left = Static_rc.left + ( Static_rc.Width() )/2;
	LiveCease_rc.right = LiveCease_Width + LiveCease_rc.left;
	LiveCease_rc.top = Static_rc.top + ( Static_rc.Height() )/2;
	LiveCease_rc.bottom = LiveCease_rc.top + LiveCease_Height;
	m_Button_LiveCease.MoveWindow(LiveCease_rc);
	m_Button_LiveCease.ShowWindow(SW_HIDE);

	CRect LiveBegin_rc;
	m_Button_LiveBegin.GetClientRect(LiveBegin_rc);
	int LiveBegin_Width = LiveBegin_rc.Width();
	int LiveBegin_Height = LiveBegin_rc.Height();
	LiveBegin_rc.left = Static_rc.left + ( Static_rc.Width() )/2;
	LiveBegin_rc.right = LiveBegin_Width + LiveBegin_rc.left;
	LiveBegin_rc.top = Static_rc.top + ( Static_rc.Height() )/2;
	LiveBegin_rc.bottom = LiveBegin_rc.top + LiveBegin_Height;
	m_Button_LiveBegin.MoveWindow(LiveBegin_rc);
	m_Button_LiveBegin.ShowWindow(SW_SHOW);

	m_WIndrc = rc;
	Invalidate();
}

//void CDemoRdLiveDlg::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI )
//{
//	SetAdjustLayout();
//}

void CDemoRdLiveDlg::OnSize( UINT nType, int cx, int cy )
{
	CDialog::OnSize(nType,cx,cy);
	if( !m_bIsLoading ) return;
	if(nType!=SIZE_MINIMIZED )  //�жϴ����ǲ�����С���ˣ���Ϊ������С��֮�� �����ڵĳ��Ϳ����0����ǰһ�α仯��ʱ�ͻ���ֳ���0�Ĵ������
	{
		SetAdjustLayout();
	}
	UpdatePreviewConfig(IGlRender_SPreviewLayout::eNotChangePos);

}

void CDemoRdLiveDlg::OnStnClickedStaticFps()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

LRESULT CDemoRdLiveDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch( message )
	{
	case WM_ENTERMENULOOP:
		OutputDebugString( L"WM_ENTERMENULOOP\n");
		break;
	}

	if ( message >= WM_MYNotify ) 
	{
		IRender_ENotify eNotify = (IRender_ENotify)(message - WM_MYNotify);
		switch( eNotify )
		{
			//Ԥ�������е���ӳ�����ť��������õ���֪ͨ���ϲ������������ӳ����Ľӿڣ��Ͳ�����ӳ�����
		case eNotify_None:

			break;
			//�����һ��������
		case eNotify_SceneAdding:
			AddScene();
			return TRUE;
			break;
			//��ӵĳ���	
		case eNotify_SceneAdded:
			if ( int(lParam) )
			{
				UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
			}
			break;
			//ɾ����һ��������
		case eNotify_SceneDeleting:
			break;
			//ɾ���ĳ���
		case eNotify_SceneDeleted:
			if (  int(lParam)  )
			{
				UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
			}
			break;
			//�л��˳�����ѡ����һ��������Ϊ��ǰ������
		case eNotify_SceneSwitched:
			m_bChipListChanging = TRUE;
			if ( m_scenario == NULL  )
			{
				return FALSE;
			}
			if ( m_scenario->m_ArrayDialogElement == NULL )
			{
				m_scenario->m_ArrayDialogElement = new CArrayDialogElement;
			}
			else
			{
				int Count =  m_scenario->m_ArrayDialogElement->m_ArrayElement.GetSize();
				for ( int i = 0;  i < Count; i++ )
				{
					CDialogElement * dialogElement = m_scenario->m_ArrayDialogElement->m_ArrayElement[0];
					m_scenario->m_ArrayDialogElement->m_ArrayElement.RemoveAt(0);
					delete dialogElement;
				}
			}
			{
				if ( Render_GetSceneCount() <= 0  )
				{
					break;
				}
				HSCENE hScene = wParam;
				if ( hScene )
				{
					//m_tabctrl.DeleteItem(0);
					CString	szName	= Scene_GetName( hScene );
					if ( szName.IsEmpty() )
					{
						szName	=  L"���� " ;
						int iNumber = Render_GetSceneIndex( hScene ) + 1 ;
						char iProgressOf[20] = {0};
						_itoa_s(iNumber,iProgressOf, sizeof(iProgressOf), 10);
						szName += iProgressOf;
					}
					TCITEM tc1;
					tc1.mask = TCIF_TEXT;
					LPCWSTR WszName = szName;
					tc1.pszText = (LPWSTR)WszName;
					m_tabctrl.SetItem(0,&tc1 );
					int Count = Scene_GetChipCount( hScene );
					for ( int i = 0; i < Count; ++i )
					{
						HCHIP Chip = Scene_GetChip( hScene, i );
						CDialogElement * dialogElement = new CDialogElement(m_scenario->GetDlgItem(IDC_STATIC_SCENA_LIST));
						dialogElement->Create(IDD_ELEMENT,m_scenario->GetDlgItem(IDC_STATIC_SCENA_LIST));
						dialogElement->m_HChip = Chip;
						dialogElement->m_pant = m_scenario;
						dialogElement->Init();
						dialogElement->SetElementName( );
						if ( Sceen_GetCurChip( Render_GetCurScene() ) == Chip )
						{
							dialogElement->SetSelected(TRUE);
						}
						m_scenario->m_ArrayDialogElement->AddDialogElement(dialogElement);
						m_scenario->adjustmentOnComponentsListLayout();
					}
					Invalidate();
				}
			}
			Invalidate();
			m_bChipListChanging = FALSE;
			break;
		case eNotify_SceneNamed:
			{
				HSCENE hScene = wParam;
				if ( hScene )
				{
					//m_tabctrl.DeleteItem(0);
					CString	szName	= Scene_GetName( hScene );
					if ( szName.IsEmpty() )
					{
						szName	=  L"���� " ;
						int iNumber = Render_GetSceneIndex( hScene ) + 1 ;
						char iProgressOf[20] = {0};
						_itoa_s(iNumber,iProgressOf,sizeof(iProgressOf), 10);
						szName += iProgressOf;
					}
					TCITEM tc1;
					tc1.mask = TCIF_TEXT;
					LPCWSTR WszName = szName;
					tc1.pszText = (LPWSTR)WszName;
					m_tabctrl.SetItem(0,&tc1 );

				}
				Invalidate();
			}
			break;
			//�϶��˺�̨����Ԥ������Ĺ����������߹�������ֵ��Χ�����˸ı䡣
		case eNotify_SceneScroll:
			break;

			//��Ԥ�������ϰ���������Ҽ����ϲ����鴦�����絯���Ҽ��˵�
			//��ǰ����	 //������ڵ�Ԫ����Index��-1��ʾû���κ�Ԫ���ϡ�
		case eNotify_RBDownCScene:
			break;
			//������ڵĳ�����NULL ��ʾû���κγ����ϡ�
		case eNotify_RBDownBScene:
			break;

		case eNotify_RBUpCScene:
			break;
		case eNotify_RBUpBScene:
			break;
		default:
			HSCENE hScene = (HSCENE)wParam;
			if ( hScene == Render_GetCurScene() )
			{
				if ( m_scenario == NULL )
				{
					return FALSE;
				}
				m_scenario->OnRenderNotify( eNotify, wParam, lParam );
				return TRUE;
			}
			Invalidate();
			break;
		}

	}
	else
	{


	}
	if ( message >= WM_Encoder_ENotify )
	{
		IEncoder_ENotify eNotify = (IEncoder_ENotify)(message - WM_Encoder_ENotify);
		on_EncoderNotifyCB(eNotify,wParam,lParam);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDemoRdLiveDlg::OnCommand( WPARAM wParam, LPARAM lParam )
{
	int menuID = LOWORD(wParam);
	if ( ID_ADD_SECNE ==  menuID)
	{
		//�������
		AddScene();
	}
	else if((WM_WH_16_9 <= menuID) && ( menuID < WM_WH_FPS)  ) 
	{
		if((WM_WH_16_9 <= menuID) && ( menuID < WM_WH_4_3) )//16:9
		{
			ClearStateMenu();
			MENUITEMINFO menUinfo = {0};
			menUinfo.cbSize	= sizeof( MENUITEMINFO );
			menUinfo.fMask	= MIIM_DATA;
			VERIFY(m_MenuFilm->GetMenuItemInfo(menuID, &menUinfo));
			CSize * Size = (CSize*)menUinfo.dwItemData;
			m_MenuFilm->CheckMenuRadioItem(WM_WH_16_9,WM_WH_4_3-1,menuID,MF_BYCOMMAND );
			CMenu * pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
			pMenu->CheckMenuRadioItem(0,3,0,MF_BYCOMMAND );
			CString cs;
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Group"),CString(L"Film") );
			cs.Format(L"%d",Size->cx);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Width"),cs );
			CString szHeight;
			szHeight.Format(L"%d",Size->cy);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Height"),szHeight );
			Render_SetSize( Size->cx, Size->cy );
			//UpdatePreviewConfig( IGlRender_SPreviewLayout::eScreenCenter );
		}	
		else if (  (WM_WH_4_3 <= menuID) && ( menuID < WM_WH_CIF)  )//4:3
		{
			ClearStateMenu();
			MENUITEMINFO menUinfo = {0};
			menUinfo.cbSize	= sizeof( MENUITEMINFO );
			menUinfo.fMask	= MIIM_DATA;
			VERIFY(m_MenuTelevision->GetMenuItemInfo(menuID, &menUinfo));
			CSize * Size = (CSize*)menUinfo.dwItemData;
			m_MenuTelevision->CheckMenuRadioItem(WM_WH_4_3,WM_WH_CIF-1,menuID,MF_BYCOMMAND );
			CMenu * pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
			pMenu->CheckMenuRadioItem(0,3,1,MF_BYCOMMAND );
			Render_SetSize( Size->cx, Size->cy );
			CString cs;
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Group"),CString(L"Television") );
			cs.Format(L"%d",Size->cx);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Width"),cs );
			CString szHeight;
			szHeight.Format(L"%d",Size->cy);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Height"),szHeight );
			//UpdatePreviewConfig( IGlRender_SPreviewLayout::eScreenCenter );
		}
		else if ( (WM_WH_CIF <= menuID) && ( menuID < WM_WH_USER) )//CIF
		{
			ClearStateMenu();
			MENUITEMINFO menUinfo = {0};
			menUinfo.cbSize	= sizeof( MENUITEMINFO );
			menUinfo.fMask	= MIIM_DATA;
			VERIFY(m_MenuCIF->GetMenuItemInfo(menuID, &menUinfo));
			CSize * Size = (CSize*)menUinfo.dwItemData;
			m_MenuCIF->CheckMenuRadioItem(WM_WH_CIF,WM_WH_USER-1,menuID,MF_BYCOMMAND );
			CMenu * pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
			pMenu->CheckMenuRadioItem(0,3,2,MF_BYCOMMAND );
			Render_SetSize( Size->cx, Size->cy );
			CString cs;
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Group"),CString(L"CIF") );
			cs.Format(L"%d",Size->cx);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Width"),cs );
			CString szHeight;
			szHeight.Format(L"%d",Size->cy);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Height"),szHeight );
		}
		else if ( (WM_WH_USER <= menuID) && ( menuID < WM_WH_FPS) )//�Զ���
		{
			ClearStateMenu();
			MENUITEMINFO menUinfo = {0};
			menUinfo.cbSize	= sizeof( MENUITEMINFO );
			menUinfo.fMask	= MIIM_DATA;
			VERIFY(m_MenuUser->GetMenuItemInfo(menuID, &menUinfo));
			CSize * Size = (CSize*)menUinfo.dwItemData;
			m_MenuUser->CheckMenuRadioItem(WM_WH_USER,WM_WH_FPS-1,menuID,MF_BYCOMMAND );
			CMenu * pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(0);
			pMenu->CheckMenuRadioItem(0,3,3,MF_BYCOMMAND );
			Render_SetSize( Size->cx, Size->cy );
			CString cs;
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Group"),CString(L"User") );
			cs.Format(L"%d",Size->cx);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Width"),cs );
			CString szHeight;
			szHeight.Format(L"%d",Size->cy);
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@Height"),szHeight );
		}
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eScreenCenter );
		
	}
	else if ( ID_MENU_EXC_HW == menuID )//��߽���
	{
		CMenu* mmenu = GetMenu();
		CMenu* submenu = mmenu->GetSubMenu(0);

		UINT state = submenu->GetMenuState(ID_MENU_EXC_HW, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);

		int	iWidth	= _wtoi(Getattrib(L"/Option/Base/ResolutionRatio/@Width"));
		int	iHeight	= _wtoi(Getattrib(L"/Option/Base/ResolutionRatio/@Height"));
		if (state & MF_CHECKED)
		{
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@SwapVH"),CString(L"0") );
			Render_SetSize( iWidth, iHeight );
			submenu->CheckMenuItem(ID_MENU_EXC_HW, MF_UNCHECKED | MF_BYCOMMAND);
		}
		else
		{
			Settattrib(OLESTR("/Option/Base/ResolutionRatio/@SwapVH"),CString(L"1") );
			Render_SetSize( iHeight, iWidth );
			submenu->CheckMenuItem(ID_MENU_EXC_HW, MF_CHECKED | MF_BYCOMMAND);
		}
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eDesktopCenter );
		/*CRect rc;
		GetClientRect( rc );
		iHeight = rc.Width();
		iWidth =  rc.Height();
		rc.bottom = rc.top + iHeight;
		rc.right = rc.left + iWidth;
		MoveWindow(rc);*/
		Invalidate();
	}
	else if ( ID_HARDWARE_SPEED == menuID )//Ԥ��ʹ��Ӳ������
	{
		CMenu* mmenu = GetMenu();
		CMenu* submenu = mmenu->GetSubMenu(0);

		UINT state = submenu->GetMenuState(ID_HARDWARE_SPEED, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		if (state & MF_CHECKED)
		{
			Settattrib(OLESTR("/Option/Base/PreviewScale/@Hardware"),CString(L"0") );
			submenu->CheckMenuItem(ID_HARDWARE_SPEED, MF_UNCHECKED | MF_BYCOMMAND);
		}
		else
		{
			Settattrib(OLESTR("/Option/Base/PreviewScale/@Hardware"),CString(L"1") );
			submenu->CheckMenuItem(ID_HARDWARE_SPEED, MF_CHECKED | MF_BYCOMMAND);
			CString str = Getattrib(OLESTR("/Option/Base/PreviewScale/@Hardware"));
		}

		UpdatePreviewConfig( IGlRender_SPreviewLayout::eNotChangePos );
	}
	else if(  (WM_WH_FPS <= menuID) && ( menuID < WM_WH_WH)  )//֡��
	{
		MENUITEMINFO menUinfo = {0};
		menUinfo.cbSize	= sizeof( MENUITEMINFO );
		menUinfo.fMask	= MIIM_DATA;
		VERIFY(m_MenuFps->GetMenuItemInfo(menuID, &menUinfo));
		int FPS = menUinfo.dwItemData;
		Render_SetFps( (float)FPS );
		CString cs;
		cs.Format(L"%d",FPS);
		Settattrib(OLESTR("/Option/Base/FrameRatio/@Fps"),cs );
		//UpdateVideoSizeLabel();
		m_MenuFps->CheckMenuRadioItem(WM_WH_FPS,WM_WH_WH-1,menuID,MF_BYCOMMAND );
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eScreenCenter );
	}
	else if ( (WM_WH_WH <= menuID) && ( menuID < WM_WH_WH+9)  )//Ԥ������
	{
		MENUITEMINFO menUinfo = {0};
		menUinfo.cbSize	= sizeof( MENUITEMINFO );
		menUinfo.fMask	= MIIM_DATA;
		VERIFY(m_MenuWH->GetMenuItemInfo(menuID, &menUinfo));
		float FPS = ((float)menUinfo.dwItemData)/100.00f;
		CString cs;
		cs.Format(L"%g",FPS);
		Settattrib(OLESTR("/Option/Base/PreviewScale/@Scale"),cs );
		//m_MenuWH->CheckMenuRadioItem(WM_WH_WH,WM_WH_WH+8,menuID,MF_BYCOMMAND );
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eScreenCenter );	
		/*CString		szOptUser(RDLive_DefaultDir( RDir_Profile ) );
		szOptUser  += L"Option.xml";
		( CDemoRdLiveDlg::Instance().m_sOptionDoc )->save(CComVariant(szOptUser));*/
	}
	else if ((WM_SECNE_Layout <= menuID) && ( menuID < WM_SECNE_Layout+5)  ) //Ԥ��������
	{
		MENUITEMINFO menUinfo = {0};
		menUinfo.cbSize	= sizeof( MENUITEMINFO );
		menUinfo.fMask	= MIIM_DATA;
		VERIFY(m_MenuLayout->GetMenuItemInfo(menuID, &menUinfo));
		m_iNumberScenarios = (int)menUinfo.dwItemData;
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
		m_MenuLayout->CheckMenuRadioItem(WM_SECNE_Layout,WM_SECNE_Layout+4,menuID,MF_BYCOMMAND );
	}
	else if((WM_SECNE_Layout+5 <= menuID) && ( menuID < WM_SECNE_Layout+10)  )//Ԥ����ʾ��ѡ��
	{
		UINT state = m_MenuLayout->GetMenuState(menuID, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		BOOL bIsChecked = FALSE;
		if (state & MF_CHECKED)
		{
			bIsChecked = FALSE;
			m_MenuLayout->CheckMenuItem(menuID, MF_UNCHECKED | MF_BYCOMMAND);
		}
		else
		{
			bIsChecked = TRUE;
			m_MenuLayout->CheckMenuItem(menuID, MF_CHECKED | MF_BYCOMMAND);
		}
		switch( menuID )
		{
		case WM_SECNE_Layout+5:
			m_bAddSceneBut = bIsChecked;
			break;
		case WM_SECNE_Layout+6:
			m_bScrollBarSize = bIsChecked;
			break;
		case  WM_SECNE_Layout+7:
			m_bSceneName = bIsChecked;
			break;
		case  WM_SECNE_Layout+8:
			m_bCurLikeBScene = bIsChecked;
			break;
		case WM_SECNE_Layout+9:
			m_fEnterScale = bIsChecked ? 2.0f : 0.0f;
			break;
		}
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
	}
	else if ( (WM_SECNE_Layout+10) == menuID  )//��ԭԤ������
	{
		UINT state = m_MenuLayout->GetMenuState(menuID, MF_BYCOMMAND);
		ASSERT(state != 0xFFFFFFFF);
		BOOL bIsChecked = FALSE;
		if (state & MF_CHECKED)
		{
			bIsChecked = FALSE;
			m_MenuLayout->CheckMenuItem(menuID, MF_UNCHECKED | MF_BYCOMMAND);
		}
		else
		{
			bIsChecked = TRUE;
			m_MenuLayout->CheckMenuItem(menuID, MF_CHECKED | MF_BYCOMMAND);
		}
		if ( bIsChecked )
		{
			SetPreviewLayoutToMenu( TRUE );
		}
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
	}
	else if ( (WM_SECNE_Switch <= menuID) && ( menuID < WM_SECNE_Dletel - 1)   )//�л�����
	{
		int Number = menuID - WM_SECNE_Switch;
		HSCENE	hScene	= Render_GetScene( Number );
		Render_SetCurScene( hScene );
		Invalidate();
	}
	else if ( (WM_SECNE_Dletel <= menuID) && ( menuID < WM_SECNE_Rename - 1)  )//ɾ������
	{
		m_bChipListChanging = TRUE;
		int Number = menuID - WM_SECNE_Dletel;
		if ( Number == Render_GetSceneCount() )
		{
			int count = Render_GetSceneCount();
			for ( int i = 0; i < count; i++ )
			{
				Render_DestroyScene(Render_GetScene( 0 ));

			}
			m_bChipListChanging = FALSE;
			return CDialog::OnCommand(wParam, lParam);  
		}
		HSCENE	hScene	= Render_GetScene( Number );
		Render_DestroyScene(hScene);
		Invalidate();
		m_bChipListChanging = FALSE;
	}
	else if ( (WM_SECNE_Rename <= menuID) && ( menuID < WM_SECNE_Layout - 1)  )//����������
	{
		int Number = menuID - WM_SECNE_Rename;
		HSCENE	hScene	= Render_GetScene( Number );
		WCHAR SceneName[ 256 ] = {0};
		CNameDialog nameDialog;
		nameDialog.m_szName = SceneName;
		nameDialog.DoModal();
		if ( nameDialog.m_bIsConfirm )
		{
			Scene_SetName( hScene, SceneName );
		}
	}
	else if ( ID_empty_remove_SECNE == menuID )//��յ�ǰ����
	{
		HSCENE		hScene	= Render_GetCurScene();
		while( hScene && Scene_GetChipCount( hScene ) )
		{
			HCHIP	hChip	= Scene_GetChip( hScene, 0 );
			Chip_Destroy( hChip );
		}
	}
	else if (   ( menuID > (WM_AUDIO_Speaker - 1) ) & ( menuID < (WM_AUDIO_MIC - 1)  )  )
	{
		Audio_SetCurrent( eAudCap_Speaker, Audio_GetDevId(eAudCap_Speaker, menuID - WM_AUDIO_Speaker ) );
		m_MenuSpeaker->CheckMenuRadioItem(WM_AUDIO_Speaker,WM_AUDIO_Speaker+49,menuID,MF_BYCOMMAND );
	}
	else if (  ( menuID > (WM_AUDIO_MIC - 1) ) & ( menuID < (WM_IAMGE - 1)  ) )
	{
		Audio_SetCurrent( eAudCap_Microphone, Audio_GetDevId(eAudCap_Microphone, menuID - WM_AUDIO_MIC ) );
		m_MenuMIC->CheckMenuRadioItem(WM_AUDIO_MIC,WM_AUDIO_MIC+49,menuID,MF_BYCOMMAND );
	}
	else if( m_scenario != NULL ) 
	{
		if( (menuID > (WM_CAMERA-1) ) & ( menuID < WM_CAMERA + m_scenario->m_CameraCount) )
		{
			int iCaremaCount = menuID - WM_CAMERA; 
			HCHIP	hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Camera );
			if ( hChip )
			{
				if ( Chip_Open( hChip, Camera_GetInternalName(iCaremaCount) ) )
				{
					Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
					Chip_SetVisible( hChip, TRUE );
				}
			}
			//������Լ��Ĵ������
		}
		else if ( ( menuID > (WM_SCREEN-1) ) & ( menuID < WM_SCREEN + m_scenario->m_ScreenCount ) )
		{
			UINT	iVal = 0;
			if ( WM_SCREEN == menuID )
			{
				iVal = 0xFFFFFFFF;
			}
			IScreen_SCapParams	sCapParams	= {0};
			if ( iVal == 0xFFFFFFFF )
			{
				sCapParams.iScreen	= -1;
			}
			else if ( iVal >= 0 && iVal < 256 )
			{
				sCapParams.iScreen	= iVal;
			}
			HCHIP hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Screen );
			if ( hChip )
			{
				if ( Chip_Open( hChip, Screen_AssembleSource( &sCapParams ) ) )
				{
					Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
					//����ӵ�Ԫ������Ҫ����������ʾ��
					Chip_SetVisible( hChip, TRUE );
				}
			}
		}
		else if ( ( menuID > (WM_HWND - 1) ) & ( menuID < WM_HWND + m_scenario->m_HWndCount ) )
		{
			UINT	iVal = 0;
			if ( WM_HWND != menuID )
			{
				iVal = menuID - WM_HWND;
			}
			else
			{
				iVal = menuID - WM_HWND ;
			}
			IScreen_SCapParams	sCapParams	= {0};
			if ( int(iVal+1) > m_scenario->m_HWndList->GetSize() )
			{
				return  CDialog::OnCommand(wParam, lParam);  
			}
			sCapParams.hWindow = m_scenario->m_HWndList->GetAt(iVal); 
			HCHIP hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Screen );
			if ( hChip )
			{
				if ( Chip_Open( hChip, Screen_AssembleSource( &sCapParams ) ) )
				{
					Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
					//����ӵ�Ԫ������Ҫ����������ʾ��
					Chip_SetVisible( hChip, TRUE );
				}
			}
		}
		else if ( menuID == WM_IAMGE )
		{
			CString szFileName = m_scenario->GetImageDir();
			if ( szFileName.IsEmpty() ) return CDialog::OnCommand(wParam, lParam); ;
			HCHIP	hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Picture );
			if ( hChip )
			{
				Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
				Chip_SetVisible( hChip, TRUE );
				if ( Chip_Open( hChip, szFileName ) )
				{
				}
			}
		}
	}
	
	return CDialog::OnCommand(wParam, lParam);  
}

void CDemoRdLiveDlg::LoadMenuinformation()
{
	if ( GetIsLoading() )
	{	
		//ͼ��
		CString CszName;
		CMenu * pMenu = AfxGetMainWnd()->GetMenu();
		pMenu->GetSubMenu(0)->RemoveMenu(0,0);

		//�ֱ���
		m_MenuFilm = CreaoadMenu(OLESTR("/Option/Base/ResolutionRatio/Film/*"),OLESTR("/Option/Base/ResolutionRatio/Film/@Str"),&CszName,WM_WH_16_9,0);
		pMenu->GetSubMenu(0)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuFilm->m_hMenu,CszName);

		m_MenuTelevision = CreaoadMenu(OLESTR("/Option/Base/ResolutionRatio/Television/*"),OLESTR("/Option/Base/ResolutionRatio/Television/@Str"),&CszName,WM_WH_4_3,0);
		pMenu->GetSubMenu(0)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuTelevision->m_hMenu,CszName);

		m_MenuCIF = CreaoadMenu(OLESTR("/Option/Base/ResolutionRatio/CIF/*"),OLESTR("/Option/Base/ResolutionRatio/CIF/@Str"),&CszName,WM_WH_CIF,0);
		pMenu->GetSubMenu(0)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuCIF->m_hMenu,CszName);

		m_MenuUser = CreaoadMenu(OLESTR("/Option/Base/ResolutionRatio/User/*"),OLESTR("/Option/Base/ResolutionRatio/User/@Str"),&CszName,WM_WH_USER,0);
		pMenu->GetSubMenu(0)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuUser->m_hMenu,CszName);
		pMenu->GetSubMenu(0)->CheckMenuRadioItem(0,3,0,MF_BYCOMMAND );

		CMenu * Menu;
		int width  = _wtoi(Getattrib(OLESTR("/Option/Base/ResolutionRatio/@Width")));
		int Max;
		int Min;
		LPWSTR szNode;
		if ( L"Film" == Getattrib(OLESTR("/Option/Base/ResolutionRatio/@Group")) )
		{
			Menu = m_MenuFilm;
			Max = WM_WH_4_3 - 1;
			Min = WM_WH_16_9;
			szNode = L"/Option/Base/ResolutionRatio/Film/*";
		} 
		else if ( L"Television" == Getattrib(OLESTR("/Option/Base/ResolutionRatio/@Group")) )
		{
			Menu = m_MenuTelevision;
			Max = WM_WH_CIF - 1;
			Min = WM_WH_4_3;
			szNode = L"/Option/Base/ResolutionRatio/Television/*";
		} 
		else if ( L"CIF" == Getattrib(OLESTR("/Option/Base/ResolutionRatio/@Group")) )
		{
			Menu = m_MenuTelevision;
			Max = WM_WH_USER - 1;
			Min = WM_WH_CIF;
			szNode = L"/Option/Base/ResolutionRatio/CIF/*";
		} 
		else 
		{
			Menu = m_MenuUser;
			Max = WM_WH_FPS-1;
			Min = WM_WH_USER;
			szNode = L"/Option/Base/ResolutionRatio/User/*";
		}
		Render_SetSize( width, _wtoi(Getattrib(OLESTR("/Option/Base/ResolutionRatio/@Height"))) );
		SetCurrentOptions(Menu,width,szNode,Max,Min);

		// ������͸� ID_MENU_EXC_HW
		pMenu->GetSubMenu(0)->AppendMenu(MF_STRING,ID_MENU_EXC_HW,L"������͸�");
		pMenu->GetSubMenu(0)->AppendMenu(MF_SEPARATOR);
		//֡��
		m_MenuFps = CreaoadMenu(OLESTR("/Option/Base/FrameRatio/*"),NULL,NULL,WM_WH_FPS,1);
		pMenu->GetSubMenu(0)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuFps->m_hMenu,L"֡��");
		pMenu->GetSubMenu(0)->AppendMenu(MF_SEPARATOR);
		int FPS = _wtoi(Getattrib(OLESTR("/Option/Base/FrameRatio/@Fps")));

		int Fps_Number = 0;
		switch( FPS )
		{
		case 60:
			Render_SetFps( (float)FPS );
			Fps_Number = 1 ;
			break;
		case 30:
			Render_SetFps( (float)FPS );
			Fps_Number = 2;
			break;
		case 25:
			Render_SetFps( (float)FPS );
			Fps_Number = 3;
			break;
		case 20:
			Render_SetFps( (float)FPS );
			Fps_Number = 4;
			break;
		case 10:
			Render_SetFps( (float)FPS );
			Fps_Number = 5;
			break;
		}
		if ( Fps_Number > 0 )
			m_MenuFps->CheckMenuRadioItem(WM_WH_FPS,WM_WH_WH-1,WM_WH_FPS+( Fps_Number -1 ),MF_BYCOMMAND );

		//���ű���
		m_MenuWH = CreaoadMenu(OLESTR("/Option/Base/PreviewScale/*"),NULL,NULL,WM_WH_WH,2);
		pMenu->GetSubMenu(0)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuWH->m_hMenu,L"Ԥ������");
		//Ԥ��ʹ��Ӳ������ ID_HARDWARE_SPEED
		pMenu->GetSubMenu(0)->AppendMenu(MF_STRING,ID_HARDWARE_SPEED,L"Ԥ��ʹ��Ӳ������");
		if( _wtoi(Getattrib(OLESTR("/Option/Base/PreviewScale/@Hardware"))) )
		{
			pMenu->GetSubMenu(0)->CheckMenuItem(ID_HARDWARE_SPEED, MF_CHECKED | MF_BYCOMMAND);
		}
		
		//����
		pMenu->GetSubMenu(1)->RemoveMenu(0,0);
		//���
		pMenu->GetSubMenu(1)->AppendMenu(MF_STRING,ID_ADD_SECNE,L"��ӳ���....");

		//�л�
		m_MenuSwitch =  new CMenu;
		m_MenuSwitch->CreatePopupMenu();
		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuSwitch->m_hMenu,L"�л���");

		//ɾ������
		m_MenuDletel =  new CMenu;
		m_MenuDletel->CreatePopupMenu();
		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuDletel->m_hMenu,L"ɾ������");

		//����������
		m_MenuRename =  new CMenu;
		m_MenuRename->CreatePopupMenu();
		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuRename->m_hMenu,L"����������");

		//Ԥ������
		m_MenuLayout = new CMenu;
		m_MenuLayout->CreatePopupMenu();
		AppendMenuData(m_MenuLayout,0,WM_SECNE_Layout,L"�رպ�̨����Ԥ��");
		AppendMenuData(m_MenuLayout,2,WM_SECNE_Layout+1,L"��ʾ2����̨����");
		AppendMenuData(m_MenuLayout,4,WM_SECNE_Layout+2,L"��ʾ4����̨����");
		AppendMenuData(m_MenuLayout,6,WM_SECNE_Layout+3,L"��ʾ6����̨����");
		AppendMenuData(m_MenuLayout,8,WM_SECNE_Layout+4,L"��ʾ8����̨����");
		m_MenuLayout->AppendMenu(MF_SEPARATOR);	
		m_MenuLayout->AppendMenu(MF_STRING,WM_SECNE_Layout+5,L"��ʾ��ӳ�����ť");
		m_MenuLayout->AppendMenu(MF_STRING,WM_SECNE_Layout+6,L"��ʾ����������");
		m_MenuLayout->AppendMenu(MF_STRING,WM_SECNE_Layout+7,L"��ʾ��������");
		m_MenuLayout->AppendMenu(MF_STRING,WM_SECNE_Layout+8,L"��̨��ʾ��ǰ����");
		m_MenuLayout->AppendMenu(MF_STRING,WM_SECNE_Layout+9,L"������볡���Ŵ�");
		m_MenuLayout->AppendMenu(MF_SEPARATOR);	
		m_MenuLocation = new CMenu;
		m_MenuLocation->CreatePopupMenu();
		m_MenuLocation->AppendMenu(MF_STRING,WM_SECNE_location,L"��");
		m_MenuLocation->AppendMenu(MF_STRING,WM_SECNE_location+1,L"��");
		m_MenuLocation->AppendMenu(MF_STRING,WM_SECNE_location+2,L"��");
		m_MenuLocation->AppendMenu(MF_STRING,WM_SECNE_location+3,L"��");

		m_MenuLayout->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuLocation->m_hMenu,L"��ʾ��λ��");

		m_MenuLayout->AppendMenu(MF_SEPARATOR);	
		m_MenuLayout->AppendMenu(MF_STRING,WM_SECNE_Layout+10,L"��ԭԤ������");

		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuLayout->m_hMenu,L"Ԥ������");

		//��յ�ǰ����
		pMenu->GetSubMenu(1)->AppendMenu(MF_STRING,ID_empty_remove_SECNE,L"��յ�ǰ����");


		AfxGetMainWnd()->GetMenu()->GetSubMenu(2)->RemoveMenu(0,0);
		SetPreviewLayoutToMenu();
	}
}

BOOL CDemoRdLiveDlg::GetIsLoading()
{
	return  m_bIsLoading;
}

CMenu * CDemoRdLiveDlg::CreaoadMenu( LPWSTR szList,LPWSTR szattrib ,CString * szName, UINT UResolutionRatio, int SerialNumber)
{
	CMenu * pMenu =  new CMenu;
	pMenu->CreatePopupMenu();

	long nLen;
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	m_sOptionDoc->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNodeList> sFilmList;
	sFilmList = spRootEle->selectNodes(szList);
	sFilmList->get_length(&nLen); //�ӽڵ���

	if ( szattrib != NULL )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode1;
		spNode1 = spRootEle->selectSingleNode(szattrib);
		CComVariant VarVal;
		spNode1->get_nodeValue(&VarVal); //�ڵ�ֵ
		(*szName) = CString( COLE2CT(VarVal.bstrVal) );
	}

	for ( long i  = 0;  i != nLen; i++ )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode;

		sFilmList->get_item(i, &spNode);
		CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long NodeLen = 0;
		spNameNodeMap->get_length(&NodeLen);
		if ( NodeLen > 0 )
		{
			ULONG_PTR Cout = 0;
			if ( SerialNumber == 0 )
			{
				CSize * size = new CSize;
				Cout = (ULONG_PTR)size;
			}
			for ( long j = 0; j != NodeLen; j++ )
			{
				CComPtr<MSXML2::IXMLDOMNode> spNodeAttrib; //ע������Ҳ��һ��IXMLDOMNode
				spNameNodeMap->get_item(j, &spNodeAttrib);
				CComBSTR bsNodeName;
				spNodeAttrib->get_nodeName(&bsNodeName); //�ڵ���
				CString name = COLE2CT(bsNodeName);
				CComVariant varVal;
				spNodeAttrib->get_nodeValue(&varVal); //�ڵ�ֵ
				CString val = COLE2CT(varVal.bstrVal);
				if ( SerialNumber == 0 )
				{
					CSize * Size =((CSize*)Cout) ;
					if ( L"Width" == name )
						Size->cx = _wtoi(val);
					else if( L"Height" == name  )
						Size->cy = _wtoi(val);
				}
				else if( SerialNumber == 1 )
				{
					if ( L"val" == name )
					{
						UINT Number = _wtoi(val);
						Cout = (ULONG_PTR)Number;
					}
				}
				else if( SerialNumber == 2 )
				{

					if ( L"val" == name )
					{
						float fNumber = (float)_tstof(val);
						Cout = (ULONG_PTR)(fNumber*100);
					}
				}

				if ( L"Str" == name )
				{
					MENUITEMINFO info;
					info.cbSize =sizeof(MENUITEMINFO);
					info.fMask =MFT_STRING | MIIM_DATA | MIIM_ID | MIIM_TYPE;
					info.fType =MFT_STRING;
					info.fState =0;
					info.wID =UResolutionRatio+i;
					info.hSubMenu =NULL;
					info.hbmpChecked =NULL;
					info.hbmpUnchecked =NULL;
					info.dwItemData = (ULONG_PTR)Cout;
					info.dwTypeData =(LPWSTR)(LPCWSTR)val;
					info.cch =8;
					pMenu->InsertMenuItem(UResolutionRatio+i,&info,true);

				}
			}
		}
		else
		{
			pMenu->AppendMenu(MF_SEPARATOR);	
		}
	}

	return pMenu;
}

void CDemoRdLiveDlg::ClearStateMenu()
{
	m_MenuTelevision->CheckMenuRadioItem(WM_WH_4_3,WM_WH_CIF-1,WM_WH_4_3,MF_BYCOMMAND );
	m_MenuTelevision->CheckMenuItem(WM_WH_4_3,MF_CHECKED);
	m_MenuTelevision->CheckMenuItem(WM_WH_4_3,MF_UNCHECKED);

	m_MenuFilm->CheckMenuRadioItem(WM_WH_16_9,WM_WH_4_3-1,WM_WH_16_9,MF_BYCOMMAND );
	m_MenuFilm->CheckMenuItem(WM_WH_16_9,MF_CHECKED);
	m_MenuFilm->CheckMenuItem(WM_WH_16_9,MF_UNCHECKED);

	m_MenuCIF->CheckMenuRadioItem(WM_WH_CIF,WM_WH_USER-1,WM_WH_CIF,MF_BYCOMMAND );
	m_MenuCIF->CheckMenuItem(WM_WH_CIF,MF_CHECKED);
	m_MenuCIF->CheckMenuItem(WM_WH_CIF,MF_UNCHECKED);

	m_MenuUser->CheckMenuRadioItem(WM_WH_USER,WM_WH_FPS-1,WM_WH_CIF,MF_BYCOMMAND );
	m_MenuUser->CheckMenuItem(WM_WH_USER,MF_CHECKED);
	m_MenuUser->CheckMenuItem(WM_WH_USER,MF_UNCHECKED);
}

CDemoRdLiveDlg& CDemoRdLiveDlg::Instance()
{
	static CDemoRdLiveDlg	mgr;
	return mgr;
}

void CDemoRdLiveDlg::SetCurrentOptions( CMenu * pMenu, int width ,LPWSTR szList,int MAX, int MIN )
{
	long nLen;
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	m_sOptionDoc->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNodeList> sFilmList;
	sFilmList = spRootEle->selectNodes(szList);
	sFilmList->get_length(&nLen); //�ӽڵ���

	for ( long i  = 0;  i != nLen; i++ )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode;

		sFilmList->get_item(i, &spNode);
		CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long NodeLen = 0;
		spNameNodeMap->get_length(&NodeLen);
		if ( NodeLen > 0 )
		{
			for ( long j = 0; j != NodeLen; j++ )
			{
				CComPtr<MSXML2::IXMLDOMNode> spNodeAttrib; //ע������Ҳ��һ��IXMLDOMNode
				spNameNodeMap->get_item(j, &spNodeAttrib);
				CComBSTR bsNodeName;
				spNodeAttrib->get_nodeName(&bsNodeName); //�ڵ���
				CString name = COLE2CT(bsNodeName);
				CComVariant varVal;
				spNodeAttrib->get_nodeValue(&varVal); //�ڵ�ֵ
				CString val = COLE2CT(varVal.bstrVal);
				if ( L"Width" == name )
				{
					if ( width == _wtoi(val) )
					{
						pMenu->CheckMenuRadioItem(MIN,MAX,i+MIN,MF_BYCOMMAND );
						return;
					}
					break;
				}
				else
				{
					continue;
				}
			}
		}
	}
}

void CDemoRdLiveDlg::AppendMenuData( CMenu * pMenu, int NumberData, int Count, LPWSTR Name )
{
	MENUITEMINFO info;
	info.cbSize =sizeof(MENUITEMINFO);
	info.fMask =MFT_STRING | MIIM_DATA | MIIM_ID | MIIM_TYPE;
	info.fType =MFT_STRING;
	info.fState =0;
	info.wID =Count;
	info.hSubMenu =NULL;
	info.hbmpChecked =NULL;
	info.hbmpUnchecked =NULL;
	info.dwItemData = (ULONG_PTR)NumberData;
	info.dwTypeData = Name;
	info.cch =8;
	pMenu->InsertMenuItem(Count,&info,true);
}

void CDemoRdLiveDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	if ( pPopupMenu == AfxGetMainWnd()->GetMenu()->GetSubMenu(1) )
	{
		if ( Render_GetSceneCount() <= 0 )
		{
			CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
			return;
		}
		HSCENE		hCurScene	= Render_GetCurScene();
		int iorder = 0;
		CMenu * pMenu = AfxGetMainWnd()->GetMenu();
		pMenu->RemoveMenu((UINT) m_MenuSwitch->m_hMenu, MF_BYCOMMAND);
		pMenu->RemoveMenu((UINT) m_MenuDletel->m_hMenu, MF_BYCOMMAND);
		pMenu->RemoveMenu((UINT) m_MenuRename->m_hMenu, MF_BYCOMMAND);
		delete m_MenuSwitch;
		delete m_MenuDletel;
		delete m_MenuRename;

		m_MenuSwitch =  new CMenu;
		m_MenuSwitch->CreatePopupMenu();

		m_MenuDletel =  new CMenu;
		m_MenuDletel->CreatePopupMenu();

		m_MenuRename =  new CMenu;
		m_MenuRename->CreatePopupMenu();

		for ( int i = 0; i < Render_GetSceneCount(); ++i )
		{
			HSCENE	hScene	= Render_GetScene( i );
			CString	szName	= Scene_GetName( hScene );
			if ( szName.IsEmpty() )
			{
				szName	=  L"���� " ;
				int iNumber = Render_GetSceneIndex( hScene ) + 1 ;
				char iProgressOf[20] = {0};
				_itoa_s(iNumber,iProgressOf,sizeof(iProgressOf),10);
				szName += iProgressOf;
			}
			m_MenuSwitch->AppendMenu(MF_STRING,WM_SECNE_Switch+i,szName);
			m_MenuDletel->AppendMenu(MF_STRING,WM_SECNE_Dletel+i,szName);
			m_MenuRename->AppendMenu(MF_STRING,WM_SECNE_Rename+i,szName);
			if ( hCurScene == hScene ) iorder = i;
		}
		int sdf;
		sdf = WM_SECNE_Switch;
		sdf = WM_SECNE_Dletel;
		sdf = WM_SECNE_Rename;
		m_MenuDletel->AppendMenu(MF_STRING,WM_SECNE_Dletel+Render_GetSceneCount(),L"ɾ�����пճ���" );

		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuSwitch->m_hMenu,L"�л���");
		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuDletel->m_hMenu,L"ɾ������");
		pMenu->GetSubMenu(1)->AppendMenu(MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT) m_MenuRename->m_hMenu,L"����������");
		m_MenuSwitch->CheckMenuRadioItem(WM_SECNE_Switch,WM_SECNE_Switch+Render_GetSceneCount() - 1,WM_SECNE_Switch +iorder ,MF_BYCOMMAND );

		CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	}

	if ( pPopupMenu == AfxGetMainWnd()->GetMenu()->GetSubMenu(2) )
	{
		if ( m_scenario != NULL )
		{
			if ( !Render_GetCurScene() )
			{
				CWnd::MessageBox(L"����ӳ�������������ݣ�",L"��ʾ",MB_OK|MB_ICONEXCLAMATION );
				CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
				return;
			}
			CRect Move_rc;
			Move_rc.left = 120;
			Move_rc.bottom = 40;
			m_scenario->AddChipMenu(Move_rc,AfxGetMainWnd()->GetMenu()->GetSubMenu(2),true);
		}
	}
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	// TODO: �ڴ˴������Ϣ����������
}

void CDemoRdLiveDlg::SetPreviewLayoutToMenu( BOOL bRestore /*= FALSE */ )
{
	if ( bRestore )
	{
		Render_SetPreviewLayout( NULL );
		m_sPerviewLayout	= *Render_GetPreviewLayout();
	}
	else
	{
		CComPtr<MSXML2::IXMLDOMElement> spRootEle;
		( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
		CComPtr<MSXML2::IXMLDOMNode> spNode1;
		spNode1 = spRootEle->selectSingleNode(OLESTR("/Option/PreviewLayout"));
		m_sPerviewLayout	= *Render_GetPreviewLayout();

		CString hexLayout = spNode1->text;
		//OutputDebugString( hexLayout );
		hexLayout.MakeUpper();
		if ( hexLayout.GetLength() == sizeof( m_sPerviewLayout ) * 2 )
		{
			LPBYTE	pData	= LPBYTE(&m_sPerviewLayout);
			LPCWSTR	pStr	= hexLayout;
			for ( int i = 0; i < sizeof( m_sPerviewLayout ); ++i )
			{
				if ( pStr[0] >= '0' && pStr[0] <='9')
					pData[i]	= ( pStr[0] - '0' ) << 4;
				else if ( pStr[0] >= 'A' && pStr[0] <='F')
					pData[i]	= ( pStr[0] - 'A' + 10 ) << 4;

				if ( pStr[1] >= '0' && pStr[1] <='9')
					pData[i]	|= ( pStr[1] - '0' );
				else if ( pStr[1] >= 'A' && pStr[1] <='F')
					pData[i]	|= ( pStr[1] - 'A' + 10 );
				pStr	+= 2;
			}
			printf("");

		}
	}

	if ( 0 == m_sPerviewLayout.iBSceneCount )
	{
		m_iNumberScenarios = m_sPerviewLayout.iBSceneCount;
	}
	else if ( 2 == m_sPerviewLayout.iBSceneCount )
	{
		m_iNumberScenarios = m_sPerviewLayout.iBSceneCount;

	}
	else if ( 4 == m_sPerviewLayout.iBSceneCount )
	{
		m_iNumberScenarios = m_sPerviewLayout.iBSceneCount;
	}
	else if ( 6 == m_sPerviewLayout.iBSceneCount )
	{
		m_iNumberScenarios = m_sPerviewLayout.iBSceneCount;
	}
	else if ( 8 == m_sPerviewLayout.iBSceneCount )
	{
		m_iNumberScenarios = m_sPerviewLayout.iBSceneCount;
	}
	m_MenuLayout->CheckMenuRadioItem(WM_SECNE_Layout,WM_SECNE_Layout+4,WM_SECNE_Layout+(m_sPerviewLayout.iBSceneCount/2),MF_BYCOMMAND );

	if ( m_sPerviewLayout.fEnterScale > 1.0 )
	{
		m_MenuLayout->CheckMenuItem(WM_SECNE_Layout+9, MF_CHECKED | MF_BYCOMMAND);
		m_fEnterScale = m_sPerviewLayout.fEnterScale;
	}
	if ( m_sPerviewLayout.bAddSceneBut )
	{
		m_MenuLayout->CheckMenuItem(WM_SECNE_Layout+5, MF_CHECKED | MF_BYCOMMAND);
		m_bAddSceneBut =  m_sPerviewLayout.bAddSceneBut ;
	}
	if ( m_sPerviewLayout.bSceneName )
	{
		m_MenuLayout->CheckMenuItem(WM_SECNE_Layout+7, MF_CHECKED | MF_BYCOMMAND);
		m_bSceneName = m_sPerviewLayout.bSceneName;
	}
	if ( m_sPerviewLayout.iScrollBarSize )
	{
		m_MenuLayout->CheckMenuItem(WM_SECNE_Layout+6, MF_CHECKED | MF_BYCOMMAND);
		m_bScrollBarSize = m_sPerviewLayout.iScrollBarSize? 12 : 0;
	}
	if ( m_sPerviewLayout.bCurLikeBScene )
	{
		m_MenuLayout->CheckMenuItem(WM_SECNE_Layout+8, MF_UNCHECKED | MF_BYCOMMAND);
		m_bCurLikeBScene = m_sPerviewLayout.bCurLikeBScene;
	}
	UpdatePreviewConfig(IGlRender_SPreviewLayout::eDesktopCenter);
}

BOOL CDemoRdLiveDlg::SaveProfile()
{
	SYSTEMTIME st; 

	CString strDate,strTime; 
	GetLocalTime(&st);
	strDate.Format(L"%d%d%d",st.wYear,st.wMonth,st.wDay); 
	if( !Settattrib(OLESTR("/Option/@Update"),strDate) )
	{
		if(!Creatattrib(OLESTR("/Option"),OLESTR("Update"),strDate))
			return FALSE;
	}
	//��Ƶ�豸
	//SaveAudioCaptureSetting();
	//eleUser.setAttribute( "Update", QDate::currentDate().toString( "yyyyMMdd" ) );
	//�ڱ��������ļ�֮ǰ���ѵ�ǰ�ı������ôӽ��汣�浽XML����
	//ui.widEncoderSetting->UpdateEncoderConfig();

	//���������ļ�
	CString		szOptUser(RDLive_DefaultDir( RDir_Profile ) );
	szOptUser  += L"Option.xml";
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->save(CComVariant(szOptUser));
	return TRUE;
}

void CDemoRdLiveDlg::OnClose()
{
	
	//���ñ���
	SaveScenes();
	m_settingDialog->SaveEncoderSetting();
	SaveAudioCaptureSetting();
	SaveProfile();
	RDLive_Uninit();
	DestroyWindow();
}	

BOOL CDemoRdLiveDlg::LoadScenes()
{
	m_sScenes.CoCreateInstance(CLSID_DOMDocument);
	//ȡ�������ļ���·����ʹ���� RDLive_DefaultDir() �õ�ָ�����ļ��С�
	CString		szOptScenes( RDLive_DefaultDir( RDir_Profile ) );
	szOptScenes +=  L"Scenes.xml";
	if( !m_sScenes->load((LPCTSTR)szOptScenes))
		return FALSE;

	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	m_sScenes->get_documentElement(&spRootEle);
	if ( spRootEle == NULL )
	{
		return FALSE;		//XML ���ݴ���
	}

	CString szSelScene = Getattrib(OLESTR("/Scenes/@Current"),FALSE);
	int				iSelScene	= _wtoi(szSelScene);
	HSCENE	HSScene = 0; 
	long nLen;
	CComPtr<MSXML2::IXMLDOMNodeList> sFilmList;
	spRootEle->get_childNodes(&sFilmList);
	sFilmList->get_length(&nLen); //�ӽڵ���	

	for( int i = 0; i < nLen; i++ )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode;

		sFilmList->get_item(i, &spNode);
		CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long NodeLen = 0;
		spNameNodeMap->get_length(&NodeLen);
		if ( NodeLen > 0 )
		{
			int				iSelChip = -1;
			CString         ScensStr;
			for ( long j = 0; j != NodeLen; j++ )
			{
				CComPtr<MSXML2::IXMLDOMNode> spNodeAttrib; //ע������Ҳ��һ��IXMLDOMNode
				spNameNodeMap->get_item(j, &spNodeAttrib);
				CComBSTR bsNodeName;
				spNodeAttrib->get_nodeName(&bsNodeName); //�ڵ���
				CString name = COLE2CT(bsNodeName);
				CComVariant varVal;
				spNodeAttrib->get_nodeValue(&varVal); //�ڵ�ֵ
				CString val = COLE2CT(varVal.bstrVal);
				if ( name == L"Current" )
				{
					iSelChip = _wtoi(val);
				} 
				else if(  name == L"Name"  )
				{
					ScensStr = val;
				}
			}

			HSCENE	hScene	= Render_CreateScene();
			Scene_SetName( hScene, ScensStr );

			if ( iSelScene == Render_GetSceneCount() - 1 )
			{
				HSScene = hScene;
			}
			long nSceneLen;
			CComPtr<MSXML2::IXMLDOMNodeList> sSceneList;
			spNode->get_childNodes( &sSceneList );
			sSceneList->get_length(&nSceneLen); //�ӽڵ���
			for (long i = 0; i != nSceneLen; ++i)
			{
				CComPtr<MSXML2::IXMLDOMNode> spChildNode;
				sSceneList->get_item(i, &spChildNode);

				CString szLockPos, szLockAR,szSoure,szLockSize,szCannotReuse,
					szVisible,szPaused,szLockAngle;
				IPinInput_EClass	eClassType	= ePinInput_Unknow;

				CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
				spChildNode->get_attributes(&spNameNodeMap);
				long SceneLen = 0;
				spNameNodeMap->get_length(&SceneLen);
				if ( SceneLen > 0 )
				{
					for ( long x = 0; x != SceneLen; x++ )
					{
						CComPtr<MSXML2::IXMLDOMNode> spNodeAttrib; //ע������Ҳ��һ��IXMLDOMNode
						spNameNodeMap->get_item(x, &spNodeAttrib);
						CComBSTR bsNodeName;
						spNodeAttrib->get_nodeName(&bsNodeName); //�ڵ���
						CString name = COLE2CT(bsNodeName);
						CComVariant varVal;
						spNodeAttrib->get_nodeValue(&varVal); //�ڵ�ֵ
						CString val = COLE2CT(varVal.bstrVal);
						if ( bsNodeName == L"LockPos" )
						{
							szLockPos = val;
						}
						else if (bsNodeName == L"Type" )
						{
							eClassType = (IPinInput_EClass)_wtoi( val );
						}
						else if (bsNodeName == L"LockAR" )
						{
							szLockAR = val;
						}
						else if (bsNodeName == L"Source" )
						{
							szSoure = val;
						}
						else if (bsNodeName == L"LockSize" )
						{
							szLockSize = val;
						}
						else if (bsNodeName == L"CannotReuse" )
						{
							szCannotReuse = val;
						}
						else if (bsNodeName == L"Visible" )
						{
							szVisible = val;
						}
						else if (bsNodeName == L"Paused" )
						{
							szPaused = val;
						}
						else if (bsNodeName == L"LockAngle" )
						{
							szLockAngle = val;
						}
					}
				}
				HCHIP			hChip		= Scene_CreateChip( hScene, eClassType );
				if ( Chip_Open( hChip, szSoure, _wtoi(szCannotReuse) ? TRUE : FALSE ) )
				{
				}
				else if ( eClassType == ePinInput_Camera )
				{
					Chip_Open( hChip, Camera_GetInternalName( 0 ),
						_wtoi(szCannotReuse)? TRUE : FALSE );
				}

				Chip_SetViewLock( hChip, eLock_AspectRatio, _wtoi(szLockAR) ? TRUE : FALSE );
				Chip_SetViewLock( hChip, eLock_Position, _wtoi(szLockPos) ? TRUE : FALSE );
				Chip_SetViewLock( hChip, eLock_Size, _wtoi(szLockSize) ? TRUE : FALSE ); 
				Chip_SetViewLock( hChip, eLock_Angle, _wtoi(szLockAngle) ? TRUE : FALSE ); 

				Chip_SetVisible( hChip, _wtoi(szVisible) ? TRUE : FALSE );
				//Chip_SetVolume( hChip, eleChip.attribute( "Volume" ).toFloat(), eleChip.attribute( "Mutex" ).toInt() ? TRUE : FALSE );
				if ( iSelChip == Scene_GetChipCount( hScene ) - 1 )
				{
					Chip_SetCurent( hChip );
				}
			

				long iNlength;
				CComPtr<MSXML2::IXMLDOMNodeList> spNodeListRect;
				spChildNode->get_childNodes(&spNodeListRect);
				spNodeListRect->get_length(&iNlength);
				CString szW,szX,szY,szH,szL,szT,szR,szB;
				BOOL isData = FALSE;
				BOOL isWH = FALSE;
				for( long u = 0; u < iNlength; u++ )
				{
					//CString szW,szX,szY,szH;
					CComPtr<MSXML2::IXMLDOMNode> spChildNoderect;
					spNodeListRect->get_item(u, &spChildNoderect);
					CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMapRect;
					spChildNoderect->get_attributes(&spNameNodeMapRect);
					long RectLen = 0;
					spNameNodeMapRect->get_length(&RectLen);

					if (  RectLen > 0 )
					{
						for( int Rect = 0; Rect <  RectLen; Rect++ )
						{
							CComPtr<MSXML2::IXMLDOMNode> spNodeAttribRect; //ע������Ҳ��һ��IXMLDOMNode
							spNameNodeMapRect->get_item(Rect, &spNodeAttribRect);
							CComBSTR bsNodeNameRect;
							spNodeAttribRect->get_nodeName(&bsNodeNameRect); //�ڵ���
							CString nameRect = COLE2CT(bsNodeNameRect);
							CComVariant varValRect;
							spNodeAttribRect->get_nodeValue(&varValRect); //�ڵ�ֵ
							CString valRect = COLE2CT(varValRect.bstrVal);

							if ( nameRect == L"W" )
							{
								szW = valRect;
								isData =TRUE;
							} 
							else if( nameRect == L"X" )
							{
								szX = valRect;
								isData =TRUE;
							}
							else if( nameRect == L"Y" )
							{
								szY = valRect;
								isData =TRUE;
							}
							else if( nameRect == L"H" )
							{
								szH = valRect;
								isData =TRUE;
							
							}
							else if ( nameRect == L"L" )
							{
								szL = valRect;
								 isWH = TRUE;
							} 
							else if( nameRect == L"T" )
							{
								szT = valRect;
								 isWH = TRUE;
							}
							else if( nameRect == L"R" )
							{
								szR = valRect;
								 isWH = TRUE;
							}
							else if( nameRect == L"B" )
							{
								szB = valRect;
								 isWH = TRUE;
							}
						}
					}
					
				}
				if( isData )
					Chip_SetRectPercent( hChip,(float)_wtof(szX) ,(float)_wtof(szY),(float)_wtof(szW) ,(float)_wtof(szH), eIgnoreAspectRatio );
				if( isWH )
					Chip_SetClipPercent( hChip, (float)_wtof(szL), (float)_wtof(szT),(float)_wtof(szR), (float)_wtof(szB) );
				//CComPtr<MSXML2::IXMLDOMNode> sRectNode;
				//CComPtr<MSXML2::IXMLDOMNode> spChild1Node;
				//sRectNode = spChildNode->selectSingleNode(OLESTR("/Chip/Rect"));
				//if( sRectNode )
				//{
				//	
				//	CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMapRect;
				//	sRectNode->get_attributes(&spNameNodeMapRect);
				//	long RectLen = 0;
				//	spNameNodeMapRect->get_length(&RectLen);
				//
				//	
				//}
				//spChild1Node = spChildNode->selectSingleNode(OLESTR("/Chip/Chip"));
				//if( spChild1Node )
				//{
				//	CString 
				//	CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMapChild1;
				//	spChild1Node->get_attributes(&spNameNodeMapChild1);
				//	long RectLen = 0;
				//	spNameNodeMapChild1->get_length(&RectLen);
				//	if (  RectLen > 0 )
				//	{
				//		for( int Rect = 0; Rect <  RectLen; Rect++ )
				//		{
				//			CComPtr<MSXML2::IXMLDOMNode> spNodeAttribChild1; //ע������Ҳ��һ��IXMLDOMNode
				//			spNameNodeMapChild1->get_item(Rect, &spNodeAttribChild1);
				//			CComBSTR bsNodeNameRect;
				//			spNodeAttribChild1->get_nodeName(&bsNodeNameRect); //�ڵ���
				//			CString nameRect = COLE2CT(bsNodeNameRect);
				//			CComVariant varValRect;
				//			spNodeAttribChild1->get_nodeValue(&varValRect); //�ڵ�ֵ
				//			CString valRect = COLE2CT(varValRect.bstrVal);

				//			if ( nameRect == L"L" )
				//			{
				//				szL = valRect;
				//			} 
				//			else if( nameRect == L"T" )
				//			{
				//				szT = valRect;
				//			}
				//			else if( nameRect == L"R" )
				//			{
				//				szR = valRect;
				//			}
				//			else if( nameRect == L"B" )
				//			{
				//				szB = valRect;
				//			}
				//		}
				//	}
				//	
				//}
			}

		}
		if ( iSelScene == Render_GetSceneCount() - 1 )
		{
			Render_SetCurScene( HSScene );
		}
	}

	

	return TRUE;
}

BOOL CDemoRdLiveDlg::SaveScenes()
{
	CString		szOptScenes( RDLive_DefaultDir( RDir_Profile ) );
	szOptScenes +=  L"Scenes.xml";

	CoInitialize(NULL);
	// �����ĵ�
	CComPtr<MSXML2::IXMLDOMDocument> pXMLDoc = NULL;
	//����DOMDocument����
	HRESULT hr = pXMLDoc.CoCreateInstance(__uuidof(MSXML2::DOMDocument));
	if (!SUCCEEDED(hr))
		return FALSE;
	CComPtr<MSXML2::IXMLDOMProcessingInstruction> pXMLProc = NULL;
	pXMLProc = pXMLDoc->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
	_variant_t vNullVal;
	vNullVal.vt = VT_NULL;
	pXMLDoc->insertBefore(pXMLProc, vNullVal);
	_variant_t varNodeType((short)MSXML2::NODE_ELEMENT);

	//������Ŀ¼
	CComPtr<MSXML2::IXMLDOMNode> pXMLNodeRoot= NULL; 
	pXMLNodeRoot = pXMLDoc->createNode(varNodeType, _T("Scenes"), _T(""));
	CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttr = NULL;
	 pXMLDoc->appendChild(pXMLNodeRoot);

	for( int iScene = 0; iScene < Render_GetSceneCount(); ++iScene )
	{
		HSCENE		hScene		= Render_GetScene( iScene );
		//����������½��
		CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
		pXMLNodeNode = pXMLNodeRoot->appendChild(pXMLDoc->createElement(_T("Scene")));

		for ( int iChip = 0; iChip < Scene_GetChipCount( hScene ); ++iChip )
		{
			HCHIP		hChip	= Scene_GetChip( hScene, iChip );
			//����������½��
			CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeChip= NULL; 
			pXMLNodeNodeChip = pXMLNodeNode->appendChild(pXMLDoc->createElement(_T("Chip")));
			IPinInput_SStatusInfo	sStatus;
			Chip_GetStatusInfo( hChip, &sStatus );

			//�����½������ Type
			{
				CString str;
				str.Format(L"%d", Chip_GetClassType( hChip ) );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipType = NULL;
				pXMLAttrChipType = pXMLDoc->createAttribute(_T("Type"));
				pXMLAttrChipType->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipType);
			}
			//�����½������ Source
			CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipSource = NULL;
			pXMLAttrChipSource = pXMLDoc->createAttribute(_T("Source"));
			pXMLAttrChipSource->nodeTypedValue = Chip_GetSourceName( hChip );
			pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipSource);

			//�����½������ CannotReuse
			{
				CString str;
				str.Format(L"%d",sStatus.bCannotReuse ? 1 : 0);
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipCannotReuse = NULL;
				pXMLAttrChipCannotReuse = pXMLDoc->createAttribute(_T("CannotReuse"));
				pXMLAttrChipCannotReuse->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipCannotReuse);
			}

			//�����½������ Paused
			{
				CString str;
				str.Format(L"%d",sStatus.eStatus == ePin_Paused ? 1 : 0 );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipPaused = NULL;
				pXMLAttrChipPaused = pXMLDoc->createAttribute(_T("Paused"));
				pXMLAttrChipPaused->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipPaused);
			}

			//�����½������ Visible
			{
				CString str;
				str.Format(L"%d", Chip_IsVisible( hChip )  );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipVisible = NULL;
				pXMLAttrChipVisible = pXMLDoc->createAttribute(_T("Visible"));
				pXMLAttrChipVisible->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipVisible);
			}

			//�����½������ LockAR
			{
				CString str;
				str.Format(L"%d",Chip_GetViewLock( hChip, eLock_AspectRatio )  );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipLockAR = NULL;
				pXMLAttrChipLockAR = pXMLDoc->createAttribute(_T("LockAR"));
				pXMLAttrChipLockAR->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipLockAR);
			}

			//�����½������ LockPos
			{
				CString str;
				str.Format(L"%d",Chip_GetViewLock( hChip, eLock_Position )  );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipLockPos = NULL;
				pXMLAttrChipLockPos = pXMLDoc->createAttribute(_T("LockPos"));
				pXMLAttrChipLockPos->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipLockPos);
			}

			//�����½������ LockSize
			{
				CString str;
				str.Format(L"%d",Chip_GetViewLock( hChip, eLock_Size )  );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipLockSize = NULL;
				pXMLAttrChipLockSize = pXMLDoc->createAttribute(_T("LockSize"));
				pXMLAttrChipLockSize->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipLockSize);
			}

			//�����½������ LockAngle
			{
				CString str;
				str.Format(L"%d",Chip_GetViewLock( hChip, eLock_Angle )  );
				CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChipLockAngle = NULL;
				pXMLAttrChipLockAngle = pXMLDoc->createAttribute(_T("LockAngle"));
				pXMLAttrChipLockAngle->nodeTypedValue = str.GetBuffer(str.GetLength());
				pXMLNodeNodeChip->attributes->setNamedItem(pXMLAttrChipLockAngle);
			}

			//�����½�� Rect
			FLOAT	fX, fY, fW, fH;
			Chip_GetRectPercent( hChip, &fX, &fY, &fW, &fH, FALSE );
			CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeRect= NULL; 
			pXMLNodeNodeRect = pXMLNodeNodeChip->appendChild(pXMLDoc->createElement(_T("Rect")));
			{
				{
					//�����½������ X
					CString str;
					str.Format(L"%f",fX);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrRectX = NULL;
					pXMLAttrRectX = pXMLDoc->createAttribute(_T("X"));
					pXMLAttrRectX->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeRect->attributes->setNamedItem(pXMLAttrRectX);
				}

				{
					//�����½������ Y
					CString str;
					str.Format(L"%f",fX);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrRectY = NULL;
					pXMLAttrRectY = pXMLDoc->createAttribute(_T("Y"));
					pXMLAttrRectY->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeRect->attributes->setNamedItem(pXMLAttrRectY);
				}

				{
					//�����½������ W
					CString str;
					str.Format(L"%f",fW);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrRectW = NULL;
					pXMLAttrRectW = pXMLDoc->createAttribute(_T("W"));
					pXMLAttrRectW->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeRect->attributes->setNamedItem(pXMLAttrRectW);
				}

				{
					//�����½������ H
					CString str;
					str.Format(L"%f",fW);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrRectH = NULL;
					pXMLAttrRectH = pXMLDoc->createAttribute(_T("H"));
					pXMLAttrRectH->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeRect->attributes->setNamedItem(pXMLAttrRectH);
				}
			}
			

			//�����½�� Clip
			FLOAT	fL, fT, fR, fB;
			Chip_GetClipPercent( hChip, &fL, &fT, &fR, &fB );
			CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeChip2= NULL; 
			pXMLNodeNodeChip2 = pXMLNodeNodeChip->appendChild(pXMLDoc->createElement(_T("Chip")));
			{
				{
					//�����½������ L
					CString str;
					str.Format(L"%f",fL);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChip2L = NULL;
					pXMLAttrChip2L = pXMLDoc->createAttribute(_T("L"));
					pXMLAttrChip2L->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeChip2->attributes->setNamedItem(pXMLAttrChip2L);
				}

				{
					//�����½������ T
					CString str;
					str.Format(L"%f",fT);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChip2T = NULL;
					pXMLAttrChip2T = pXMLDoc->createAttribute(_T("T"));
					pXMLAttrChip2T->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeChip2->attributes->setNamedItem(pXMLAttrChip2T);
				}

				{
					//�����½������ R
					CString str;
					str.Format(L"%f",fR);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChip2R = NULL;
					pXMLAttrChip2R = pXMLDoc->createAttribute(_T("R"));
					pXMLAttrChip2R->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeChip2->attributes->setNamedItem(pXMLAttrChip2R);
				}
				{
					//�����½������ B
					CString str;
					str.Format(L"%f",fB);
					CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrChip2B = NULL;
					pXMLAttrChip2B = pXMLDoc->createAttribute(_T("B"));
					pXMLAttrChip2B->nodeTypedValue = str.GetBuffer(str.GetLength());
					pXMLNodeNodeChip2->attributes->setNamedItem(pXMLAttrChip2B);
				}
			}
		}
		
		//�����½������ 
		CComPtr<MSXML2::IXMLDOMAttribute> pXMLAttrScene = NULL;
		pXMLAttrScene = pXMLDoc->createAttribute(_T("Name"));
		pXMLAttrScene->nodeTypedValue = Scene_GetName( hScene );
		pXMLNodeNode->attributes->setNamedItem(pXMLAttrScene);
		
		CString str;
		str.Format(L"%d",Chip_GetIndex( Sceen_GetCurChip( hScene ), NULL ));
		pXMLAttrScene = pXMLDoc->createAttribute(_T("Current"));
		LPWSTR Wstr = str.GetBuffer(str.GetLength());
		pXMLAttrScene->nodeTypedValue = str.GetBuffer(str.GetLength());
		pXMLNodeNode->attributes->setNamedItem(pXMLAttrScene);
	}
	//������Ŀ¼����
	CString str;
	str.Format(L"%d",Render_GetSceneIndex( Render_GetCurScene()));
	pXMLAttr = pXMLDoc->createAttribute(_T("Current"));
	pXMLAttr->nodeTypedValue = str.GetBuffer(str.GetLength());
	pXMLNodeRoot->attributes->setNamedItem(pXMLAttr);


   pXMLDoc->save(szOptScenes.GetBuffer(szOptScenes.GetLength()));
	return TRUE;
}

CString CDemoRdLiveDlg::GetattribValue( LPWSTR szattrib ,CComPtr<MSXML2::IXMLDOMNode>  pChildNode )
{
	CString szName;
	if ( szattrib != NULL )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode1;
		spNode1 = pChildNode->selectSingleNode(szattrib);
		if ( spNode1 == NULL )
		{
			return szName;
		}
		CComVariant VarVal;
		spNode1->get_nodeValue(&VarVal); //�ڵ�ֵ
		szName = CString( COLE2CT(VarVal.bstrVal) );
	}
	return szName;
}

void CDemoRdLiveDlg::UpdateVideoSizeLabel()
{	
	CString Ttmp = Getattrib( OLESTR("/Option/Base/ResolutionRatio/@SwapVH") );

	if ( !Ttmp.IsEmpty() )
	{
		bool	bSwapVH	= _wtoi(Getattrib( OLESTR("/Option/Base/ResolutionRatio/@SwapVH") )) ? true : false;
		int		iWidth	= _wtoi(Getattrib( OLESTR("/Option/Base/ResolutionRatio/@Width") ));
		int		iHeight	= _wtoi(Getattrib( OLESTR("/Option/Base/ResolutionRatio/@Height") ));
		float   fps		= (float)_wtof(Getattrib(OLESTR("/Option/Base/FrameRatio/@Fps")));
		float	fScale  = (float)_wtof(Getattrib(L"/Option/Base/PreviewScale/@Scale"));
		if ( bSwapVH )
		{
			int TmpNumber = iWidth;
			iWidth = iHeight;
			iHeight = TmpNumber;
		}
		CString str;
		str.Format(L" ��Ƶ��%dx%d %ff/s  Ԥ����%d%%",iWidth,iHeight,fps,(int)(fScale*100+0.5f));
		GetDlgItem(IDC_STATIC_FPS)->SetWindowText(str);
	}
}	

void CDemoRdLiveDlg::SetAudioCaptureSetting()
{
	m_Speaker_Slider.SetRange(0,100);
	m_Speaker_Slider.SetLineSize(1);
	m_Speaker_Slider.SetPos(100);
	//m_Speaker_Slider.SetPos();

	m_MIC_Slider.SetRange(0,100);
	m_MIC_Slider.SetLineSize(1);
	m_MIC_Slider.SetPos(100);
	//m_MIC_Slider.SetPos();

	CComPtr<MSXML2::IXMLDOMElement> spRootEle1;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle1); //���ڵ�

	int			iCount		= Audio_GetDevCount( eAudCap_Speaker );
	if ( iCount > 0 )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode;
		spNode = spRootEle1->selectSingleNode(OLESTR("/Option/AuidoCapture/Speaker"));
		if ( spNode != NULL )
		{
			Audio_SetCurrent( eAudCap_Speaker, Getattrib(OLESTR("/Option/AuidoCapture/Speaker/@DevId")) );
			Audio_Enable( eAudCap_Speaker, _wtoi(Getattrib(OLESTR("/Option/AuidoCapture/Speaker/@Mutex"))  )? FALSE : TRUE);
			Audio_SetVolume( eAudCap_Speaker, (float)_wtof(Getattrib(OLESTR("/Option/AuidoCapture/Speaker/@Volume")) ));
			m_Speaker_Slider.SetPos( int( Audio_GetVolume(eAudCap_Speaker)*100.0f ));
		}
		else
		{
			Audio_Enable( eAudCap_Speaker, TRUE );
			Audio_SetVolume( eAudCap_Speaker, 1.0f );
		}
		m_Speaker_Slider.SetTipSide( int( Audio_GetVolume( eAudCap_Speaker )*100.0f) );
		m_Speaker_Button.SetCheck( !Audio_IsEnabled( eAudCap_Speaker ) );
	}

	iCount		= Audio_GetDevCount( eAudCap_Microphone );
	if ( iCount > 0 )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode1;
		spNode1 = spRootEle1->selectSingleNode(OLESTR("/Option/AuidoCapture/Microphone"));
		if ( spNode1 != NULL )
		{
			Audio_SetCurrent( eAudCap_Microphone, Getattrib(OLESTR("/Option/AuidoCapture/Microphone/@DevId")) );
			Audio_Enable( eAudCap_Microphone, _wtoi(Getattrib(OLESTR("/Option/AuidoCapture/Microphone/@Mutex"))) ? FALSE : TRUE );
			Audio_SetVolume( eAudCap_Microphone, (float)_wtof(Getattrib(OLESTR("/Option/AuidoCapture/Microphone/@Volume")) ));
			m_MIC_Slider.SetPos(int(Audio_GetVolume(eAudCap_Microphone)*100.0f));
		}
		else
		{
			Audio_Enable( eAudCap_Microphone, TRUE );
			Audio_SetVolume( eAudCap_Microphone, 1.0f );

		}
		m_MIC_Slider.SetPos(int(Audio_GetVolume( eAudCap_Microphone )*100.0f));
		m_MIC_Button.SetCheck( !Audio_IsEnabled( eAudCap_Microphone ) );
	}
	

}

void CDemoRdLiveDlg::SaveAudioCaptureSetting()
{
	Audio_Enable( eAudCap_Speaker, m_Speaker_Button.GetCheck()? TRUE : FALSE);
	Audio_SetVolume( eAudCap_Speaker, ((float)m_Speaker_Slider.GetPos())/100.00f );

	Audio_Enable( eAudCap_Microphone, m_MIC_Button.GetCheck() ? TRUE : FALSE );
	Audio_SetVolume( eAudCap_Microphone, ((float)m_MIC_Slider.GetPos())/100.00f);

	CComPtr<MSXML2::IXMLDOMElement> spRootEle1;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle1); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNode> spNode;
	spNode = spRootEle1->selectSingleNode(OLESTR("/Option/AuidoCapture/Speaker"));
	if( spNode == NULL )
		Createnode(L"Speaker");

	CComPtr<MSXML2::IXMLDOMNode> spNode1;
	spNode1 = spRootEle1->selectSingleNode(OLESTR("/Option/AuidoCapture/Speaker"));
	if( spNode1 == NULL )
		Createnode(L"Microphone");

	//Speaker Volume
	{
		CString str;
		str.Format(L"%f",Audio_GetVolume( eAudCap_Speaker));
		if(!Settattrib(OLESTR("/Option/AuidoCapture/Speaker/@Volume"),str))
		{
			Creatattrib(OLESTR("/Option/AuidoCapture/Speaker"),OLESTR("Volume"),str);
		}
	}
	//Speaker DevId
	if(!Settattrib(OLESTR("/Option/AuidoCapture/Speaker/@DevId"),Audio_GetCurrent(eAudCap_Speaker)) )
	{
		Creatattrib(OLESTR("/Option/AuidoCapture/Speaker"),OLESTR("DevId"),Audio_GetCurrent(eAudCap_Speaker));
	}
	//Speaker	Mutex
	{
		CString str;
		str.Format( L"%d",Audio_IsEnabled(eAudCap_Speaker)?1:0);
		if(!Settattrib(OLESTR("/Option/AuidoCapture/Speaker/@Mutex"),str))
		{
			Creatattrib(OLESTR("/Option/AuidoCapture/Speaker"),OLESTR("Mutex"),str);
		}
	}
	
	//Microphone Volume
	{
		CString str;
		str.Format(L"%f",Audio_GetVolume( eAudCap_Microphone));
		if(!Settattrib(OLESTR("/Option/AuidoCapture/Microphone/@Volume"),str))
		{
			Creatattrib(OLESTR("/Option/AuidoCapture/Microphone"),OLESTR("Volume"),str);
		}
	}
	//Microphone DevId
	if(!Settattrib(OLESTR("/Option/AuidoCapture/Microphone/@DevId"),Audio_GetCurrent(eAudCap_Microphone)) )
	{
		Creatattrib(OLESTR("/Option/AuidoCapture/Microphone"),OLESTR("DevId"),Audio_GetCurrent(eAudCap_Microphone));
	}
	//Microphone	Mutex
	{
		CString str;
		str.Format(L"%d",Audio_IsEnabled(eAudCap_Microphone)?1:0);
		if(!Settattrib(OLESTR("/Option/AuidoCapture/Microphone/@Mutex"),str))
		{
			Creatattrib(OLESTR("/Option/AuidoCapture/Microphone"),OLESTR("Mutex"),str);
		}
	}


}

void CDemoRdLiveDlg::OnBnClickedButton1()
{
	if ( m_MenuSpeaker != NULL )
	{
		delete m_MenuSpeaker;
		m_MenuSpeaker = NULL;
	}

	m_MenuSpeaker = CreaMenuAudioCapture( eAudCap_Speaker, WM_AUDIO_Speaker );
	RECT pt ;
	this->GetDlgItem(IDC_BUTTON_Audio_equipment)->GetWindowRect(&pt);
	m_MenuSpeaker->TrackPopupMenu(TPM_LEFTALIGN,pt.left,pt.bottom,this);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

CMenu * CDemoRdLiveDlg::CreaMenuAudioCapture( EAudioCaptureDev audioCapTureDev  , INT menuId  )
{
	CMenu * pMenu = new CMenu;
	pMenu->CreatePopupMenu();
	INT			iCount	= Audio_GetDevCount( audioCapTureDev );
	LPCWSTR		pCurDev	= Audio_GetCurrent( audioCapTureDev );
	
	CString Str = Getattrib(OLESTR("/Option/AuidoCapture/Speaker/@DevId"));

	for ( int i = 0; i < iCount; i++ )
	{
		pMenu->AppendMenu(MF_STRING,menuId+i,Audio_GetDevName( audioCapTureDev, i ));
		if ( pCurDev && wcscmp( pCurDev, Audio_GetDevId( audioCapTureDev, i ) ) == 0    )
		{
			pMenu->CheckMenuRadioItem(menuId,menuId+49,menuId+i,MF_BYCOMMAND );
		}
	}
	return pMenu;
}

void CDemoRdLiveDlg::OnBnClickedButtonMicequipment()
{
	if ( m_MenuMIC != NULL )
	{
		delete m_MenuMIC;
		m_MenuMIC = NULL;
	}

	m_MenuMIC = CreaMenuAudioCapture( eAudCap_Microphone, WM_AUDIO_MIC );
	RECT pt ;
	this->GetDlgItem(IDC_BUTTON_MIC_equipment)->GetWindowRect(&pt);
	m_MenuMIC->TrackPopupMenu(TPM_LEFTALIGN,pt.left,pt.bottom,this);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDemoRdLiveDlg::on_EncoderNotifyCB( IEncoder_ENotify eNotify, ULONG dwValue, ULONG ptrValue )
{
	switch( eNotify )
	{
	case eEncNotifySuccess:
		break;
	case eEncNotifyStarted:
		m_Button_LiveCease.EnableWindow(true);
		break;
	case eEncNotifyStoped:
		m_Button_LiveCease.ShowWindow(SW_HIDE);
		m_Button_LiveBegin.ShowWindow(SW_SHOW);
		m_Button_LiveCease.EnableWindow(true);
		//ui.chkStart->setChecked( false );
		//ui.chkStart->setEnabled( true );
		if ( dwValue )
		{
			CString str;
			str.Format(L"ֱ��/¼�Ƴ�������ֹͣ��\n������룺%d",dwValue);
			AfxMessageBox(str);
		}
		break;
	case eEncNotifyEncodeError:
		break;
	case eEncNotifyEncodeFps:
		break;
	case eEncNotifyWriteFileFail:
		break;
	case eEncNotifyDisconnected:
		break;
	case eEncNotifyReConnectStart:
		break;
	case eEncNotifyReConnectDone:
		break;
	case eEncNotifyReConnectFail:
		break;
	case eEncNotifyUploadBitrate:
		break;
	case eEncNotifyDiscardPacks:
		break;
	case eEncNotifyDiscardFrame:
		break;
	case eEncNotifyResetBitrate:
		break;
	case eEncNotifyResetPreset:
		break;
	case eEncNotify_AcceditDone:
		m_settingDialog->m_RollingDialog->m_codingOutDialog->OnAcceditDone( dwValue ? TRUE : FALSE, (LPWSTR)ptrValue  );
		break;
	case eEncNotify_GetCloudFail:
		{
			CString str;
			str.Format(L"ȡ����ֱ����ַʧ�ܡ�\n��%d",LPWSTR(ptrValue));
				AfxMessageBox(str);
		}
		break;
	}
}

VOID WINAPI CDemoRdLiveDlg::EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam )
{
	((CDemoRdLiveDlg*)pCbParam)->PostMessage( WM_Encoder_ENotify + eNotify,dwValue, ptrValue );
}


CComPtr<MSXML2::IXMLDOMNode> CDemoRdLiveDlg::Createnode( LPWSTR nodename )
{	
	//����Live
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
	pXMLNodeNode = spRootEle->selectSingleNode(OLESTR("/Option/AuidoCapture"));
	CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeliv= NULL; 
	if ( pXMLNodeNode != NULL )
	{
		CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeChip= NULL; 
		pXMLNodeNodeChip = pXMLNodeNode->appendChild(( CDemoRdLiveDlg::Instance().m_sOptionDoc )->createElement(nodename));
		if ( pXMLNodeNodeChip == NULL )
		{
			AfxMessageBox(L"XML�ڵ㴴��ʧ�ܣ�");
			return pXMLNodeNodeliv;
		}
		else
		{
			pXMLNodeNodeliv = pXMLNodeNodeChip;
		}
	}
	else
	{
		CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeSave= NULL; 
		pXMLNodeNodeSave = spRootEle->selectSingleNode(OLESTR("/Option"));
		if ( pXMLNodeNodeSave != NULL )
		{
			CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeChip= NULL; 
			pXMLNodeNodeChip = pXMLNodeNodeSave->appendChild(( CDemoRdLiveDlg::Instance().m_sOptionDoc )->createElement(L"AuidoCapture"));
			if ( pXMLNodeNodeChip != NULL )
			{
				CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeChipLive= NULL; 
				pXMLNodeNodeChipLive = pXMLNodeNodeChip->appendChild(( CDemoRdLiveDlg::Instance().m_sOptionDoc )->createElement(nodename));
				if ( pXMLNodeNodeChipLive == NULL )
				{
					AfxMessageBox(L"XML�ڵ㴴��ʧ�ܣ�");
					return pXMLNodeNodeliv;
				}
				else
				{
					pXMLNodeNodeliv = pXMLNodeNodeChipLive;
				}
			}
		}
	}
	return pXMLNodeNodeliv;
}

void CDemoRdLiveDlg::OnBnClickedButtonLivecease()
{
	m_Button_LiveCease.ShowWindow(SW_HIDE);
	m_Button_LiveBegin.ShowWindow(SW_SHOW);
	m_Button_LiveBegin.EnableWindow(TRUE);
	Encoder_End();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDemoRdLiveDlg::OnBnClickedButtonLivebegin()
{
	SaveAudioCaptureSetting();
	m_Button_LiveCease.ShowWindow(SW_SHOW);
	m_Button_LiveBegin.ShowWindow(SW_HIDE);
	m_Button_LiveCease.EnableWindow(false);
	bool	bSuccess	= false;
	if ( m_settingDialog->SaveEncoderSetting(true) )
	{
		if ( Encoder_Start() )
		{
			bSuccess	= true;
		}
	}
	if (!bSuccess)
	{

		m_Button_LiveCease.ShowWindow(SW_HIDE);
		m_Button_LiveBegin.ShowWindow(SW_SHOW);
		m_Button_LiveBegin.EnableWindow(true);
		/*ui.chkStart->setChecked( false );
		ui.chkStart->setEnabled( true );*/
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

VOID WINAPI CDemoRdLiveDlg::onPreviewImageCB( INT iWidth, INT iHeight, LPBYTE pImageBuffer, LPVOID pCbParam )
{
	CDemoRdLiveDlg::Instance().DrawPreviewImage(iWidth,iHeight,pImageBuffer);
}

void CDemoRdLiveDlg::DrawPreviewImage( INT iWidth, INT iHeight, LPBYTE pImageBuffer )
{
	CBitmap memBmp;
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	CDC *pDC=this->GetDC(); 
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE);//IDC_PIC_2DΪ�ؼ�ID
	pWnd->GetWindowRect(&rect);//rcΪ�ؼ��Ĵ�С��
	this->ScreenToClient(rect);

	memBmp.CreateCompatibleBitmap(pDC,iWidth,iHeight );
	memBmp.SetBitmapBits(iWidth*iHeight*4,pImageBuffer);
	CBitmap *pOldBmp=memDC.SelectObject(&memBmp);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	memBmp.DeleteObject();
	memDC.DeleteDC();

}

void CDemoRdLiveDlg::OnBnClickedCheckAudioselect()
{
	SaveAudioCaptureSetting();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDemoRdLiveDlg::OnBnClickedCheck1Micselect()
{
	SaveAudioCaptureSetting();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDemoRdLiveDlg::OnNMReleasedcaptureSliderAudioVolnm(NMHDR *pNMHDR, LRESULT *pResult)
{
	SaveAudioCaptureSetting();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDemoRdLiveDlg::OnNMReleasedcaptureSliderMicVolnm(NMHDR *pNMHDR, LRESULT *pResult)
{
	SaveAudioCaptureSetting();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
