
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#include "..\Include\RDLiveSDK.h"
#pragma comment ( lib, "..\\Lib\\RDLiveSDK.lib" )

#import "msxml3.dll" 

using namespace MSXML2;


#define WM_MYNotify	(WM_USER+10)//��̨��ʾ

#define WM_CAMERA	(WM_USER+200)
	
#define WM_SCREEN	(WM_USER+250)
#define WM_HWND		(WM_USER + 270 )
#define WM_WH_16_9  (WM_USER + 500) //16:9�ֱ���
#define WM_WH_4_3  (WM_USER + 550) //16:9�ֱ���
#define WM_WH_CIF  (WM_USER + 600)//CIF�ֱ���
#define WM_WH_USER  (WM_USER + 650)//�Զ���ֱ���

#define  WM_WH_FPS (WM_USER + 700)//֡��
#define  WM_WH_WH (WM_USER + 710)//���ű���

#define WM_SECNE_Switch (WM_USER + 750)

#define WM_SECNE_Dletel (WM_USER + 850)

#define WM_SECNE_Rename  (WM_USER + 951)

#define WM_SECNE_Layout  (WM_USER + 1051)

#define WM_SECNE_location (WM_USER + 1151)

#define WM_AUDIO_Speaker ( WM_USER + 1251 )//������
#define WM_AUDIO_MIC ( WM_USER + 1301 )//MIC

#define WM_IAMGE ( WM_USER + 1351 ) //ͼƬ���

#define WM_Encoder_ENotify ( WM_USER + 1352 )

//#define  
//��ť��״̬  
enum  
{  
	CTRL_NOFOCUS = 0x01,            //��ͨ  
	CTRL_FOCUS,                     //mousemove  
	CTRL_SELECTED,                  //buttondown  
	CTRL_DISABLE,                   //��Ч  
};  

//ͼƬ��ʽ  
enum  
{  
	BTN_IMG_1 = 1,                  //  
	BTN_IMG_3 = 3,                  //3��ͼ��1��ͼƬ����3Сͼ����ͬ��  
	BTN_IMG_4 = 4,                  //4��ͼ  
};  

//��ť����  
enum  
{  
	BTN_TYPE_NORMAL = 0x10,         //��ͨBTN  
	BTN_TYPE_MENU,                  //�˵����͵�BTN  
	BTN_TYPE_STATIC,                //��̬���͵�BTN  
};  

#include <gdiplus.h>                //

using namespace Gdiplus;            //ʹ��GDI+�������ռ�, �����õĻ�ÿ��ʹ��Gdiplusʱ�����������ռ����
#pragma comment(lib, "gdiplus.lib") //�ڹ���������������


CString Getattrib( LPWSTR szattrib , bool bIsInterface = TRUE  );//��ȡ����

BOOL Settattrib( LPWSTR szattrib, CString szValue, bool bIsInterface = TRUE   );//��������

BOOL Creatattrib( LPWSTR szNode, LPWSTR szattrib,CString szValue, bool bIsInterface  = TRUE  );//��������

