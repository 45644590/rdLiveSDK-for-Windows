
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Demo_VC2008(MFC).pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include "RDLiveSdkDemoDlg.h"


CString Getattrib( LPWSTR szattrib , bool bIsInterface)
{
	CString szName;
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	if( bIsInterface )
		( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	else
		( CDemoRdLiveDlg::Instance().m_sScenes )->get_documentElement(&spRootEle);

	if ( szattrib != NULL )
	{
		CComPtr<MSXML2::IXMLDOMNode> spNode1;
		spNode1 = spRootEle->selectSingleNode(szattrib);
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

BOOL Settattrib( LPWSTR szattrib, CString szValue , bool bIsInterface )
{
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	if( bIsInterface )
		( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	else
		( CDemoRdLiveDlg::Instance().m_sScenes )->get_documentElement(&spRootEle);
		
	CComPtr<MSXML2::IXMLDOMNode> spNode;
	spNode = spRootEle->selectSingleNode(szattrib);
	if ( spNode != NULL )
	{
		spNode->put_nodeValue(CComVariant(szValue)); //д��value
		return TRUE;
	} 
	return FALSE;
}

BOOL Creatattrib( LPWSTR szNode, LPWSTR szattrib,CString szValue, bool bIsInterface )
{	
	CComPtr<MSXML2::IXMLDOMElement> spRootEle;
	if( bIsInterface )
		( CDemoRdLiveDlg::Instance().m_sOptionDoc )->get_documentElement(&spRootEle); //���ڵ�
	else
		( CDemoRdLiveDlg::Instance().m_sScenes )->get_documentElement(&spRootEle);

	CComPtr<MSXML2::IXMLDOMNode> spNode;
	spNode = spRootEle->selectSingleNode(szNode);
	if ( spNode != NULL )
	{
		CComQIPtr<MSXML2::IXMLDOMElement> spEle = spNode;
		spEle->setAttribute(szattrib, CComVariant(szValue));//���½ڵ��������
		return true;
	}
	return false;
}
