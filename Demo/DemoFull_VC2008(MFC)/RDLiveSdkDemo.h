
// Demo_VC2008(MFC).h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDemoRdLiveApp:
// �йش����ʵ�֣������ Demo_VC2008(MFC).cpp
//

class CDemoRdLiveApp : public CWinAppEx
{
public:
	CDemoRdLiveApp();

// ��д
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��
	
	DECLARE_MESSAGE_MAP()
};

extern CDemoRdLiveApp theApp;