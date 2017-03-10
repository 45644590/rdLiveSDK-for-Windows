// odingOutDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RDLiveSdkDemo.h"
#include "odingOutDialog.h"
#include "RtmpDialog.h"
#include "RDLiveSdkDemoDlg.h"
// CodingOutDialog �Ի���

IMPLEMENT_DYNAMIC(CodingOutDialog, CDialog)

CodingOutDialog::CodingOutDialog(CWnd* pParent /*=NULL*/)
: CDialog(CodingOutDialog::IDD, pParent)
{

}

CodingOutDialog::~CodingOutDialog()
{
}

void CodingOutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_FILE, m_FIle_Button);
	DDX_Control(pDX, IDC_CHECK_Live_network, m_Live_Network);
	DDX_Control(pDX, IDC_BUTTON_File_B, m_FIle_B_Button);
	DDX_Control(pDX, IDC_BUTTON_FILE_V, m_FILE_V_Button);
	DDX_Control(pDX, IDC_EDIT_File, m_Edit_File);
	DDX_Control(pDX, IDC_COMBO_format, m_combo_format);
	DDX_Control(pDX, IDC_CHECK_Live_Network_Speed, m_Live_NetworkSpeed);
	DDX_Control(pDX, IDC_BUTTON_ADD_Live, m_ADD_Live_Button);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_Delete_Button);
	DDX_Control(pDX, IDC_CHECK_virtual, m_Virtual_Check);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Title);

	DDX_Control(pDX, IDC_STATIC_accredit_File, m_accredit_File);
	DDX_Control(pDX, IDC_STATIC_accredit_Rtmp, m_accredit_Rtmp);
	DDX_Control(pDX, IDC_STATIC_accredit_cloud, m_accredit_cloud);
}


BEGIN_MESSAGE_MAP(CodingOutDialog, CDialog)
	ON_BN_CLICKED(IDC_CHECK_FILE, &CodingOutDialog::OnBnClickedCheckFile)
	ON_BN_CLICKED(IDC_CHECK_Live_network, &CodingOutDialog::OnBnClickedCheckLivenetwork)
	ON_BN_CLICKED(IDC_BUTTON_File_B, &CodingOutDialog::OnBnClickedButtonFileB)
	ON_BN_CLICKED(IDC_BUTTON_FILE_V, &CodingOutDialog::OnBnClickedButtonFileV)
	ON_BN_CLICKED(IDC_BUTTON_ADD_Live, &CodingOutDialog::OnBnClickedButtonAddLive)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CodingOutDialog::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CodingOutDialog ��Ϣ�������

void CodingOutDialog::OnBnClickedCheckFile()
{
	bool bCheck = m_FIle_Button.GetCheck() ? true : false;
	if ( bCheck )
	{
		m_FIle_B_Button.EnableWindow(TRUE);
		m_FILE_V_Button.EnableWindow(TRUE);
		m_Edit_File.EnableWindow(TRUE);
		m_combo_format.EnableWindow(TRUE);
		m_spinBox->SetEndleWindows(TRUE);
	} 
	else
	{
		m_FIle_B_Button.EnableWindow(FALSE);
		m_FILE_V_Button.EnableWindow(FALSE);
		m_Edit_File.EnableWindow(FALSE);
		m_combo_format.EnableWindow(FALSE);
		m_spinBox->SetEndleWindows(FALSE);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CodingOutDialog::OnBnClickedCheckLivenetwork()
{
	bool bCheck =m_Live_Network.GetCheck() ? true : false;
	if ( bCheck )
	{
		m_Live_NetworkSpeed.EnableWindow(TRUE);
		m_ADD_Live_Button.EnableWindow(TRUE);
		m_Delete_Button.EnableWindow(TRUE);
	} 
	else
	{
		m_Live_NetworkSpeed.EnableWindow(FALSE);
		m_ADD_Live_Button.EnableWindow(FALSE);
		m_Delete_Button.EnableWindow(FALSE);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CodingOutDialog::Init()
{	
	m_combo_format.AddString(L"flv");
	m_combo_format.AddString(L"mp4");
	m_combo_format.AddString(L"avi");
	m_FIle_B_Button.EnableWindow(FALSE);
	m_FILE_V_Button.EnableWindow(FALSE);
	m_Edit_File.EnableWindow(FALSE);
	m_combo_format.EnableWindow(FALSE);
	m_Live_NetworkSpeed.EnableWindow(FALSE);
	m_ADD_Live_Button.EnableWindow(FALSE);
	m_Delete_Button.EnableWindow(FALSE);

	int AdaptiveNetwork = _wtoi(Getattrib(OLESTR("/Option/Encoder/Save/@AdaptiveNetwork")));
	int FIle = _wtoi(Getattrib(OLESTR("/Option/Encoder/Save/@File")));
	int Live= _wtoi(Getattrib(OLESTR("/Option/Encoder/Save/@Live")));

	if ( FIle )
	{
		m_FIle_Button.SetCheck(TRUE);
		m_FIle_B_Button.EnableWindow(TRUE);
		m_FILE_V_Button.EnableWindow(TRUE);
		m_Edit_File.EnableWindow(TRUE);
		m_combo_format.EnableWindow(TRUE);
	}

	if ( Live )
	{
		m_Live_Network.SetCheck(TRUE);
		m_Live_NetworkSpeed.EnableWindow(TRUE);
		m_ADD_Live_Button.EnableWindow(TRUE);
		m_Delete_Button.EnableWindow(TRUE);
	}

	CString strTitle(Getattrib(OLESTR("/Option/Encoder/Save/@Title")));
	int Split = _wtoi(Getattrib(OLESTR("/Option/Encoder/Save/File/@Split")));
	CString strDir(Getattrib(OLESTR("/Option/Encoder/Save/File/@Dir"))); 
	int Fmt = _wtoi(Getattrib(OLESTR("/Option/Encoder/Save/File/@Fmt")));

	m_spinBox = new CSpinBox;
	m_spinBox->Create(IDD_DIALOG_Spin_Text,this);
	m_spinBox->Init(8192,0,256);
	m_spinBox->ShowWindow(SW_SHOW);
	m_spinBox->SetCurrentNumber(Split);
	m_Live_NetworkSpeed.SetCheck(AdaptiveNetwork);
	m_combo_format.SetCurSel(Fmt);
	m_Edit_File.SetWindowText(strDir);
	m_Edit_Title.SetWindowText(strTitle);

	CRect rect;
	GetDlgItem(IDC_STATIC_FIle_MB)->GetClientRect(&rect);

	CRect Move_rc;
	m_spinBox->GetClientRect( Move_rc );
	int Move_height = Move_rc.Height();
	int Move_width = Move_rc.Width();
	Move_rc.left = 95;
	Move_rc.top  = 168;
	Move_rc.right = Move_rc.left + Move_width;
	Move_rc.bottom = Move_rc.top + Move_height;
	m_spinBox->MoveWindow(Move_rc);

	m_LiveDialog = new CLiveDialog(GetDlgItem(IDC_STATIC_Live_box));
	m_LiveDialog->Create(IDD_DIALOG4,GetDlgItem(IDC_STATIC_Live_box));

	CRect MOVE_rc;
	GetDlgItem(IDC_STATIC_Live_box)->GetClientRect(MOVE_rc);
	int Button_Video_Encode_Bottom = MOVE_rc.Height();
	int Button_Video_Encode_Width = MOVE_rc.Width();

	MOVE_rc.left = 5;
	MOVE_rc.top =  10;
	MOVE_rc.bottom = 140 +  MOVE_rc.top;
	MOVE_rc.right = 237 + MOVE_rc.left;

	m_LiveDialog->ShowWindow(SW_SHOW);
	m_LiveDialog->MoveWindow(&MOVE_rc);

	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
	//CComPtr<MSXML2::IXMLDOMElement> spRootEleliv = pXMLNodeNodeliv;
	CComPtr<MSXML2::IXMLDOMNodeList> sFilmList;
	long nLen;
	sFilmList = spRootEle->selectNodes("/Option/Encoder/Save/*");
	sFilmList->get_length(&nLen); //�ӽڵ���
	for ( long i  = 0;  i != nLen; i++  )
	{
		bool bIsadd = false;
		bool bIsCheck = false;
		CString szDispName;
		CComPtr<MSXML2::IXMLDOMNode> spNode;
		sFilmList->get_item(i, &spNode);
		CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long NodeLen = 0;
		spNameNodeMap->get_length(&NodeLen);
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
			if ( name == L"Fmt" )
			{
				break;
			}
			if( name == L"Checked" )
			{
				bIsCheck = _wtoi( val ) ? true : false;
			}
			else if ( name == L"DispName" )
			{
				szDispName = val;
				bIsadd = true;
			}
		}
		if ( bIsadd )
		{
			m_LiveDialog->AddLiveChooseDialog(szDispName,bIsCheck);
		}
	}
	CheckMovieSaveDir();
}

void CodingOutDialog::OnBnClickedButtonFileB()
{
	WCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = L"ѡ����Ƶ�ļ����Ŀ¼";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		m_Edit_File.SetWindowText(szPath);
	}
}

void CodingOutDialog::OnBnClickedButtonFileV()
{
	CString strAppName;
	m_Edit_File.GetWindowText(strAppName);//��ǰ����Ŀ¼
	ShellExecute(NULL,L"Open",strAppName,NULL,NULL,SW_SHOWNORMAL);  
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CodingOutDialog::OnBnClickedButtonAddLive()
{
	CRtmpDialog nameDialog;
	//nameDialog.Create(IDD_DIALOG3,this);
	//nameDialog.m_szName = SceneName;
	//nameDialog.setmodel(0);
	nameDialog.DoModal();
	if ( nameDialog.m_bIsConfirm )
	{

		for ( int i = 0; i <  m_LiveDialog->m_ArrayDialogElement->m_ArrayElement.GetSize(); i++ )
		{
			if ( m_LiveDialog->m_ArrayDialogElement->m_ArrayElement[i]->m_StrDisName == nameDialog.m_sLiveUrl.szDispName )
			{
				AfxMessageBox(L"�Ѿ�������ͬ��ֱ����Ŀ��");
				return;
			}
		}
		//����һ��ֵ��Ϊitem�ĸ���ֵ���Ա�����xml�еļ�¼һһ��Ӧ��
		UINT		uTick	= 0;
		SYSTEMTIME	sSysTime;		//ʹ�� UTC (1970-1-1) ���񾭹�����������˵��ʱ���������Ϊ����ֵ��
		FILETIME	sFileTime;		//�����ڽ����ϣ�Ӧ�ò�����1����Ӷ���������û���˲�������ô�졣
		GetSystemTime( &sSysTime );
		SystemTimeToFileTime( &sSysTime, &sFileTime );	//UTCʱ��תΪ�ļ�ʱ�䣬�ļ�ʱ������ 1601��1��1�� ������ 100ns ����
		//�ļ�ʱ����� 10000 �õ����������ٳ���1000 ������������ȥ 11644473600��1601-1-1��1970-1-1�������������õ�UTCʱ�������

		m_LiveDialog->AddLiveChooseDialog( nameDialog.m_sLiveUrl.szDispName,TRUE);

		CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeliv = Createnode(_T("Live"));
		//Type
		{
			CString str;
			str.Format(L"%d",nameDialog.m_sLiveUrl.iType);
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("Type"), CComVariant(str));//���½ڵ��������
		}
		//UrlOrId
		{
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("UrlOrId"), CComVariant(nameDialog.m_sLiveUrl.szUrlOrId));//���½ڵ��������
		}
		//PW
		{
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("PW"), CComVariant(nameDialog.m_sLiveUrl.szPw));//���½ڵ��������
		}
		//AutoLogin
		{
			CString str;
			str.Format(L"%d",nameDialog.m_sLiveUrl.bAutoLogin?TRUE:false);
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("AutoLogin"), CComVariant(str));//���½ڵ��������
		}
		//DispName
		{
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("DispName"), CComVariant(nameDialog.m_sLiveUrl.szDispName));//���½ڵ��������
		}
		//Checked
		{
			CString str;
			str.Format(L"%d",1);
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("Checked"), CComVariant(str));//���½ڵ��������
		}
		//Date
		{
			CString str;
			str.Format(L"%d",uTick);
			CComQIPtr<MSXML2::IXMLDOMElement> spEle = pXMLNodeNodeliv;
			spEle->setAttribute(OLESTR("Date"), CComVariant(str));//���½ڵ��������
		}


	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CodingOutDialog::OnBnClickedButtonDelete()
{
	int Count = -1;
	for ( int i = 0; i <  m_LiveDialog->m_ArrayDialogElement->m_ArrayElement.GetSize(); i++ )
	{
		if ( m_LiveDialog->m_ArrayDialogElement->m_ArrayElement[i]->GetSelected() )
		{
			Count = i;
			break;
		}
	}

	if ( Count >= 0 )
	{
		CComPtr<MSXML2::IXMLDOMElement> spRootEle;
		( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
		CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
		//CComPtr<MSXML2::IXMLDOMElement> spRootEleliv = pXMLNodeNodeliv;
		CComPtr<MSXML2::IXMLDOMNodeList> sFilmList;
		long nLen;
		sFilmList = spRootEle->selectNodes("/Option/Encoder/Save/*");
		sFilmList->get_length(&nLen); //�ӽڵ���
		for ( long i  = 0;  i != nLen; i++  )
		{
			bool bIsadd = false;
			CString szDispName;
			CComPtr<MSXML2::IXMLDOMNode> spNode;
			sFilmList->get_item(i, &spNode);
			CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
			spNode->get_attributes(&spNameNodeMap);
			long NodeLen = 0;
			spNameNodeMap->get_length(&NodeLen);
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
				if ( name == L"DispName" )
				{
					if ( val == m_LiveDialog->m_ArrayDialogElement->m_ArrayElement[Count]->m_StrDisName )
					{
						bIsadd = true;
					}

				}
			}
			if ( bIsadd )
			{
				//ɾ��
				CComPtr<MSXML2::IXMLDOMDocument>  pDoc; 
				MSXML2::IXMLDOMElementPtr pDocElement,root,pElement; 

				//pDoc->load("f:\\storyxmltest.xml"); 
				root = (spRootEle->selectSingleNode(OLESTR("/Option/Encoder/Save"))); 
				//pElement= spNode; 
				 root->removeChild(spNode); 
				//spNode->ParentNode->RemoveChild(spNode);
				//moldNode->Release();
			}
		}

		m_LiveDialog->m_ArrayDialogElement->DeleteDoalogElment(m_LiveDialog->m_ArrayDialogElement->m_ArrayElement[Count]);
		m_LiveDialog->adjustmentOnComponentsListLayout();
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

bool CodingOutDialog::SvaePife( BOOL bPopMsg /*= FALSE */ )
{
	DWORD	dwError	= 0;
	SEncoderSaveFile	sSave;
	ZeroMemory( &sSave, sizeof( sSave ) );
	CString				szTitle;
	GetDlgItem(IDC_EDIT_title_Live)->GetWindowText(szTitle);
	if ( bPopMsg && szTitle.IsEmpty() )
	{
		AfxMessageBox(L"���ⲻ��Ϊ�գ�\n��������⡣");
		return false;
	}
	sSave.szTitle = szTitle;
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
	pXMLNodeNode = spRootEle->selectSingleNode(OLESTR("/Option/Encoder/Save/File"));
	if ( pXMLNodeNode == NULL )
	{
		Createnode(_T("File"));
	}

	if(!Settattrib(OLESTR("/Option/Encoder/Save/@Title"),szTitle))
	{
		Creatattrib(OLESTR("/Option/Encoder/Save"),OLESTR("Title"),szTitle);
	}

	{
		CString str;
		str.Format(L"%d",m_FIle_Button.GetCheck()?1:0);
		if(!Settattrib(OLESTR("/Option/Encoder/Save/@File"),str))
		{
			Creatattrib(OLESTR("/Option/Encoder/Save"),OLESTR("File"),str);
		}
	}
	{
		CString str;
		str.Format(L"%d",m_Live_Network.GetCheck()?1:0);
		if(!Settattrib(OLESTR("/Option/Encoder/Save/@Live"),str))
		{
			Creatattrib(OLESTR("/Option/Encoder/Save"),OLESTR("Live"),str);
		}
	}
	{
		CString str;
		str.Format(L"%d",m_Virtual_Check.GetCheck()?1:0);
		if(!Settattrib(OLESTR("/Option/Encoder/Save/@AdaptiveNetwork"),str))
		{
			Creatattrib(OLESTR("/Option/Encoder/Save"),OLESTR("AdaptiveNetwork"),str);
		}
	}
	//�ļ�
	Encoder_RemoveSaveFile( -1 );	//ɾ�����еı����ļ�����
	LPWSTR				szExtNames1[]	= { L".flv", L".mp4", L".avi" };
	CString				szFilePath;
	sSave.eContainer	= (EFileContainer)m_combo_format.GetCurSel();
	if ( !CheckMovieSaveDir( bPopMsg ) )
	{
		return false;
	}

	CString FileDir;
	m_Edit_File.GetWindowText(FileDir);
	szFilePath			= FileDir;
	szFilePath		   += szTitle;
	szFilePath          += szExtNames1[m_combo_format.GetCurSel()];
	sSave.szSavePath	= szFilePath;
	sSave.iSplitSize	= INT64( m_spinBox->m_iRealDataNumber ) * 1024 * 1024;


	if(!Settattrib(OLESTR("/Option/Encoder/Save/File/@Dir"),FileDir))
	{
		Creatattrib(OLESTR("/Option/Encoder/Save/File"),OLESTR("Dir"),FileDir);
	}


	{
		CString str;
		str.Format(L"%d",m_combo_format.GetCurSel());
		if(!Settattrib(OLESTR("/Option/Encoder/Save/File/@Fmt"),str))
		{
			Creatattrib(OLESTR("/Option/Encoder/Save/File"),OLESTR("Fmt"),str);
		}
	}
	{
		CString str;
		str.Format(L"%d",m_spinBox->m_iRealDataNumber);
		if (!Settattrib(OLESTR("/Option/Encoder/Save/File/@Split"),str))
		{
			Creatattrib(OLESTR("/Option/Encoder/Save/File"),OLESTR("Split"),str);
		}
	}

	if ( m_FIle_Button.GetCheck() )
	{
		if ( !Encoder_AddSaveFile( &sSave ) )
		{
			dwError	= GetLastError();
		}
	}

	//���-ֱ��

	sSave.dwDelaySecond		= 0;
	sSave.bAdaptiveNetwork	= m_Live_NetworkSpeed.GetCheck();

	CComPtr<MSXML2::IXMLDOMElement> spRootEle1;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle1); //���ڵ�
	//CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
	//CComPtr<MSXML2::IXMLDOMElement> spRootEleliv = pXMLNodeNodeliv;
	CComPtr<MSXML2::IXMLDOMNodeList> sFilmList;
	long nLen;
	sFilmList = spRootEle1->selectNodes("/Option/Encoder/Save/*");
	sFilmList->get_length(&nLen); //�ӽڵ���
	for ( long i  = 0;  i != nLen; i++  )
	{
		bool bIsadd = false;
		bool bIsCheck = false;
		int iType = -1;
		CString szDispName;
		CString szUrlOrId;
		CComPtr<MSXML2::IXMLDOMNode> spNode;
		sFilmList->get_item(i, &spNode);
		CComPtr<MSXML2::IXMLDOMNamedNodeMap> spNameNodeMap;
		spNode->get_attributes(&spNameNodeMap);
		long NodeLen = 0;
		spNameNodeMap->get_length(&NodeLen);
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
			if ( name == L"Fmt" )
			{
				break;
			}
			if( name == L"Checked" )
			{
				bIsCheck = _wtoi( val ) ? true : false;
				bIsadd = true;
			}
			else if ( name == L"DispName" )
			{
				szDispName = val;
				bIsadd = true;
			}
			else if ( name == L"Type" ) 
			{
				if ( val.IsEmpty() )
				{
					continue;
				}

				iType = _wtoi( val );
				bIsadd = true;
			}
			else if ( name == L"UrlOrId" )
			{
				szUrlOrId = val;
				bIsadd = true;
			}
		}
		if ( bIsadd )
		{
			int Count = -1;
			for ( int i = 0; i <  m_LiveDialog->m_ArrayDialogElement->m_ArrayElement.GetSize(); i++ )
			{
				if ( m_LiveDialog->m_ArrayDialogElement->m_ArrayElement[i]->m_StrDisName == szDispName )
				{
					Count = i;
					break;
				}
			}
			if ( Count >= 0 )
			{
				if ( m_LiveDialog->m_ArrayDialogElement->m_ArrayElement[Count]->m_Check_Live_Choose.GetCheck() )
				{
					CComQIPtr<MSXML2::IXMLDOMElement> spEle = spNode;
					CComPtr<MSXML2::IXMLDOMNode> spNodeattrib;
					spNodeattrib = spEle->selectSingleNode(L"/@Checked");
					if ( spNodeattrib != NULL )
					{
						spNodeattrib->put_nodeValue(CComVariant(L"1")); //д��value
					} 
					switch( iType )
					{
					case 0:
						sSave.eContainer	= Mix_RTMP;
						sSave.szSavePath	= szUrlOrId;
						break;
					case 1:
						sSave.eContainer	= Mix_RdCloud;
						sSave.szSavePath	= szUrlOrId;
						break;
					case 2:
						sSave.eContainer	= Mix_RdCloud;
						sSave.szSavePath	= szUrlOrId;
						break;
					}
					if ( !Encoder_AddSaveFile( &sSave ) )
					{
						dwError	= GetLastError();
					}
				}
				else
				{
					CComQIPtr<MSXML2::IXMLDOMElement> spEle = spNode;
					CComPtr<MSXML2::IXMLDOMNode> spNodeattrib;
					spNodeattrib = spEle->selectSingleNode(L"/@Checked");
					if ( spNodeattrib != NULL )
					{
						spNodeattrib->put_nodeValue(CComVariant(L"0")); //д��value
						return TRUE;
					} 
				}
			}
		}
	}
	return true;

}

CComPtr<MSXML2::IXMLDOMNode> CodingOutDialog::Createnode( LPWSTR nodename )
{	
	//����Live
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNode= NULL; 
	pXMLNodeNode = spRootEle->selectSingleNode(OLESTR("/Option/Encoder/Save"));
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
		pXMLNodeNodeSave = spRootEle->selectSingleNode(OLESTR("/Option/Encoder"));
		if ( pXMLNodeNodeSave != NULL )
		{
			CComPtr<MSXML2::IXMLDOMNode> pXMLNodeNodeChip= NULL; 
			pXMLNodeNodeChip = pXMLNodeNodeSave->appendChild(( CDemoRdLiveDlg::Instance().m_sOptionDoc )->createElement(L"Save"));
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

bool CodingOutDialog::CheckMovieSaveDir( BOOL bPopMsg /*= FALSE */ )
{
	CString FileDir;
	bool	bUseDefault	= false;
	m_Edit_File.GetWindowText(FileDir);
	if ( FileDir.IsEmpty() )
	{
		bUseDefault	= true;
	} 
	else
	{
		if ( !PathIsDirectory(FileDir) )
		{
			INT_PTR nRes;   
			// ��ʾ��Ϣ�Ի���   
			nRes = MessageBox(_T("ָ������Ƶ�ļ��в����ڣ����Ҵ���ʧ�ܣ��Ƿ�ʹ��Ĭ�ϵ���Ƶ�ļ��У�"), _T("��Ƶ�ļ���"), MB_OKCANCEL | MB_ICONQUESTION);   
			// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
			if (IDCANCEL == nRes)   
				return false;
			else
				bUseDefault	= true;
		}
		else
		{
			bUseDefault	= true;
		}
	}
	if ( bUseDefault )
	{
		m_Edit_File.SetWindowText( RDLive_DefaultDir( RDir_Videos )  );
	}
	return true;
}


void CodingOutDialog::OnAcceditDone( BOOL bSuccess, LPWSTR szErrStr  )
{
	if ( bSuccess )
	{
		FLOAT	fLocalDays	= RDLive_GetAccreditDays( eAccreditLocalSave );
		FLOAT	fRtmpDays	= RDLive_GetAccreditDays( eAccreditRtmpLive );
		FLOAT	fCloudDays	= RDLive_GetAccreditDays( eAccreditCloudLive );

		CString	szTiltestr;
		szTiltestr.Format(L"��Ȩʣ�� %f ��" ,fLocalDays);
		m_accredit_File.SetWindowText(szTiltestr);
		CString str;
		str.Format( L"RTMP ��Ȩʣ�� RTMP %f ��",fRtmpDays);
		m_accredit_Rtmp.SetWindowText( str);
		CString cloudStr;
		cloudStr.Format(L"Cloud ��Ȩʣ�� %f ��",fCloudDays);
		m_accredit_cloud.SetWindowText( cloudStr);
		m_FIle_Button.EnableWindow( true );
		m_Live_Network.EnableWindow( true );
	}
	else
	{
		CString STR( L"��Ȩʧ�ܣ�");
		STR +=szErrStr;
		m_accredit_File.SetWindowText(STR);
		CString STRD( L"��Ȩʧ�ܣ�");
		STRD +=szErrStr;
		m_accredit_Rtmp.SetWindowText(STRD);
		m_FIle_Button.EnableWindow( false );
		m_Live_Network.EnableWindow( false );
		m_FIle_B_Button.EnableWindow(FALSE);
		m_FILE_V_Button.EnableWindow(FALSE);
		m_Edit_File.EnableWindow(FALSE);
		m_spinBox->SetEndleWindows(FALSE);
		m_combo_format.EnableWindow(FALSE);
		m_Live_NetworkSpeed.EnableWindow(FALSE);
		m_ADD_Live_Button.EnableWindow(FALSE);
		m_Delete_Button.EnableWindow(FALSE);
	}
}