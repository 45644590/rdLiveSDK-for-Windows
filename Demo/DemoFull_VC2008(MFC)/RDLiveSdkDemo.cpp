
// Demo_VC2008(MFC).cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RDLiveSdkDemo.h"
#include "RDLiveSdkDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemoRdLiveApp

BEGIN_MESSAGE_MAP(CDemoRdLiveApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDemoRdLiveApp ����

CDemoRdLiveApp::CDemoRdLiveApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDemoRdLiveApp ����

CDemoRdLiveApp theApp;


// CDemoRdLiveApp ��ʼ��

BOOL CDemoRdLiveApp::InitInstance()
{
	::CoInitialize(NULL);  
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	 AfxOleInit();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//CDemoRdLiveDlg dlg;
	m_pMainWnd = &CDemoRdLiveDlg::Instance();
	INT_PTR nResponse = CDemoRdLiveDlg::Instance().DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	
	
	
	
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CDemoRdLiveApp::ExitInstance()
{	
	CoUninitialize();
	return 0;
}
