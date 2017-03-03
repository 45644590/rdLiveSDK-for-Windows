#include "stdafx.h"
#include "rdlivesdkdemo.h"
#include "chipitem.h"

RDLiveSdkDemo::RDLiveSdkDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	m_bPreviewSizeing	= FALSE;
	m_bChipListChanging	= FALSE;
	ZeroMemory( &m_sPerviewLayout, sizeof(m_sPerviewLayout) );
	m_timReszie	= new QTimer( this );
	m_timReszie->setObjectName( "timReszie" );
	m_timReszie->setSingleShot( true );

	ui.setupUi(this);
	m_agpSwictScene		= new QActionGroup( this );
	m_agpBSceneCount	= new QActionGroup( this );
	ui.actBSceneCount0->setData( int(0) );
	m_agpBSceneCount->addAction( ui.actBSceneCount0 );
	ui.actBSceneCount2->setData( int(2) );
	m_agpBSceneCount->addAction( ui.actBSceneCount2 );
	ui.actBSceneCount4->setData( int(4) );
	m_agpBSceneCount->addAction( ui.actBSceneCount4 );
	ui.actBSceneCount6->setData( int(6) );
	m_agpBSceneCount->addAction( ui.actBSceneCount6 );
	ui.actBSceneCount8->setData( int(8) );
	m_agpBSceneCount->addAction( ui.actBSceneCount8 );
	m_agpBSceneCount->setExclusive( true );
	m_agpBScenePos		= new QActionGroup( this );
	ui.actBSceneUp->setData( int(0) );
	m_agpBScenePos->addAction( ui.actBSceneUp );
	ui.actBSceneDown->setData( int(1) );
	m_agpBScenePos->addAction( ui.actBSceneDown );
	ui.actBSceneLeft->setData( int(2) );
	m_agpBScenePos->addAction( ui.actBSceneLeft );
	ui.actBSceneRight->setData( int(3) );
	m_agpBScenePos->addAction( ui.actBSceneRight );
	m_agpBScenePos->setExclusive( true );

	ui.widVolumeSpk->SetRange( 0.0, 1.0 );
	ui.widVolumeSpk->SetText( 0, 0, 0 );
	ui.widVolumeSpk->SetHitMask( ( 1 << CRangeSlider::eHit_Right ) | ( 1 << CRangeSlider::eHit_RightLine ) );
	ui.widVolumeMic->SetRange( 0.0, 1.0 );
	ui.widVolumeMic->SetText( 0, 0, 0 );
	ui.widVolumeMic->SetHitMask( ( 1 << CRangeSlider::eHit_Right ) | ( 1 << CRangeSlider::eHit_RightLine ) );

	ui.widEncoderSetting->SetDocOption( &m_docOption );


	m_pLabVideoSize	= new QLabel( this );
	m_pLabFScale	= new QLabel( this );
	m_pLabTimeFps	= new QLabel( this );
	statusBar()->addWidget( m_pLabVideoSize );
	statusBar()->addWidget( m_pLabFScale );
	statusBar()->addWidget( m_pLabTimeFps );

	qRegisterMetaType<int>("IRender_ENotify");
	qRegisterMetaType<void*>("HSCENE");
	connect( this, SIGNAL(signalRenderNotifyCB( IRender_ENotify, HSCENE, int )), this, SLOT(on_RenderNotifyCB( IRender_ENotify, HSCENE, int )) );
	qRegisterMetaType<int>("IEncoder_ENotify");
	connect( this, SIGNAL(signalEncoderNotifyCB( IEncoder_ENotify, ulong, ulong )), this, SLOT(on_EncoderNotifyCB( IEncoder_ENotify, ulong, ulong )) );

	


	if ( !RDLive_Init( L"17RD", L"RDLiveSdkDemo" ) )
	{
		QMessageBox::about( this, "RDLiveSDK", "Init Fail!" );
		close();
	}
	if ( !RDLive_ResetAccredit( "d3200cc987431827", 
		"77a9eeea008524e8bdf10e18409cbdb3sULczML4CjomZFtst04v/HLUrHqWT72Mmkz7WhUEmpjXMH7/UWz5oGMwUGQPbYX+MKSpM01lSGQ/qNzCkFFyKXSwxrKIViR4iZ7ZxOuB6n80wDeCV7jHJSEN1+DqlCLm3dJWQF3CFLMOj2YJxwI/YDY9h3SjCsWFz9j/71RCHH0FWpr13vMRM6a1uRCnke2Tyly/V4S4E7BE1tR6WDcxNQTeX9w399l/EpNb8LvBNNUz6shNmM627BGBfTbPG2vj+grPaxv1rFcVRqNkT45Jrjvjp3PV8L6Py7fCUvK5PJ0Pb/olb9q/M2Yom+AZkSlE0FDcSKb0MG+QCE9f1MYacjFoU31o7cZb5ZQZ++7lMqXMDvTi9LyTYR+0lDKKwFC8EJ43/upbIuhawyXQ2w4u7Zvv9IUXqhamlTUirPmuV4lSVypdzCT+gPdEjq9krLLjRajAMutBwefKiHdrp/h65BxTErT94rH7OAU6bCmbX/o=" ) )
	{
	}
	Render_SetNotifyCallBack( RenderNotifyCB, EncoderNotifyCB, this );
	LoadProfile();


}

RDLiveSdkDemo::~RDLiveSdkDemo()
{

}

VOID WINAPI RDLiveSdkDemo::RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam )
{
	RDLiveSdkDemo*	pThis	= (RDLiveSdkDemo*)pCbParam;
	emit pThis->signalRenderNotifyCB( eNotify, hScene, iValue );
}
VOID WINAPI RDLiveSdkDemo::EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam )
{
	RDLiveSdkDemo*	pThis	= (RDLiveSdkDemo*)pCbParam;
	emit pThis->signalEncoderNotifyCB( eNotify, dwValue, ptrValue );
}

BOOL RDLiveSdkDemo::LoadProfile()
{
	BOOL		bReplaceUserOpt	= FALSE;	//�Ƿ���Ҫ�滻�����û��������ļ�
	//ȡ�������ļ���·����ʹ���� RDLive_DefaultDir() �õ�ָ�����ļ��С�
	//Ĭ�������ļ���·����������ĳ�ʼ���ã����û������ļ�������ʱ������Ĭ�����õİ汾�и���ʱ�����滻���û������ļ���
	QString		szOptDefault	= QString::fromUtf16( (PWORD)RDLive_DefaultDir( RDir_Application ) ) + "Option.xml";
	//�û������ļ���·�������û������ļ����ڣ��Ұ汾��Ĭ�������ļ�һ�£���ʹ���û������ļ���
	QString		szOptUser		= QString::fromUtf16( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Option.xml";
	//�� Windows ��ʽ��·��ת��Ϊ Qt ��·���ַ�����ʵ����ֻ�ǰ� \ �滻���� /��
	szOptDefault	= QDir::fromNativeSeparators( szOptDefault );
	szOptUser	= QDir::fromNativeSeparators( szOptUser );

	QFile		fileUser( szOptUser );
	if ( !fileUser.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		bReplaceUserOpt	= TRUE;		//�ļ���ʧ��
	}
	else
	{
		if ( !m_docOption.setContent( &fileUser ) )
		{
			bReplaceUserOpt	= TRUE;		//���� XML ��ʽʧ��
		}
		else
		{
			QDomElement		eleUser	= m_docOption.documentElement();
			if ( eleUser.tagName() != "Option" )
			{
				bReplaceUserOpt	= TRUE;		//XML ���ݴ���
			}
			else
			{
				INT				iEdition	= eleUser.attribute( "Edition" ).toInt();
				QFile			fileDefault( szOptDefault );
				if ( fileDefault.open( QIODevice::ReadOnly | QIODevice::Text ) )
				{
					QDomDocument	docDefault;
					if ( docDefault.setContent( &fileDefault ) )
					{
						QDomElement		eleDefault	= docDefault.documentElement();
						if ( eleDefault.tagName() == "Option" && 
							eleUser.attribute( "Edition" ).toInt() != eleDefault.attribute( "Edition" ).toInt() )
						{
							bReplaceUserOpt	= TRUE;		//Ĭ�������ļ��İ汾����
						}
					}
					fileDefault.close();
				}
			}
		}
		fileUser.close();
	}

	if ( bReplaceUserOpt )
	{
		QFile::remove( szOptUser );
		if ( !QFile::copy( szOptDefault, szOptUser ) )
		{
			return FALSE;		//�ļ�����ʧ��
		}
		if ( !fileUser.open( QIODevice::ReadOnly | QIODevice::Text ) )
		{
			return FALSE;		//�ļ���ʧ��
		}
		if ( !m_docOption.setContent( &fileUser ) )
		{
			return FALSE;		//���� XML ��ʽʧ��
		}
		QDomElement		eleUser	= m_docOption.documentElement();
		if ( eleUser.tagName() != "Option" )
		{
			return FALSE;		//XML ���ݴ���
		}
	}
	return SettingToUiAndSdk();
}

BOOL RDLiveSdkDemo::SaveProfile()
{
	QDomElement		eleUser	= m_docOption.documentElement();
	if ( eleUser.tagName() != "Option" )
	{
		return FALSE;		//XML ���ݴ���
	}
	eleUser.setAttribute( "Update", QDate::currentDate().toString( "yyyyMMdd" ) );
	//�ڱ��������ļ�֮ǰ���ѵ�ǰ�ı������ôӽ��汣�浽XML����
	ui.widEncoderSetting->UpdateEncoderConfig();
	
	//���������ļ�
	QString		szOptUser		= QString::fromUtf16( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Option.xml";
	szOptUser	= QDir::fromNativeSeparators( szOptUser );
	QFile		fileUser( szOptUser );
	if ( !fileUser.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		return FALSE;		//�ļ���ʧ��
	}
	QTextStream	steXml( &fileUser );
	steXml.setCodec( "UTF-8" );
	m_docOption.save( steXml, 4, QDomNode::EncodingFromTextStream );
	fileUser.close();
	return TRUE;
}

BOOL RDLiveSdkDemo::LoadScenes()
{
	//ȡ�������ļ���·����ʹ���� RDLive_DefaultDir() �õ�ָ�����ļ��С�
	QString		szOptScenes		= QString::fromUtf16( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Scenes.xml";
	szOptScenes	= QDir::fromNativeSeparators( szOptScenes );

	QDomDocument	docScenes;
	QFile			fileScenes( szOptScenes );
	if ( !fileScenes.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		return FALSE;
	}
	if ( !docScenes.setContent( &fileScenes ) )
	{
		return FALSE;		//���� XML ��ʽʧ��
	}
	fileScenes.close();

	QDomElement		eleSceneAll	= docScenes.documentElement();
	if ( eleSceneAll.tagName() != "Scenes" )
	{
		return FALSE;		//XML ���ݴ���
	}

	QDomNode		nodeScene	= eleSceneAll.firstChild();
	int				iSelScene	= eleSceneAll.attribute( "Current", "-1" ).toInt();
	while( !nodeScene.isNull() )
	{
		if ( !nodeScene.isElement() )
		{
			nodeScene	= nodeScene.nextSibling();
			continue;
		}
		QDomElement		eleScene	= nodeScene.toElement();
		if ( eleScene.tagName() != "Scene" )
		{
			nodeScene	= nodeScene.nextSibling();
			continue;
		}
		HSCENE	hScene	= Render_CreateScene();
		Scene_SetName( hScene, eleScene.attribute( "Name" ).utf16() );
		if ( iSelScene == Render_GetSceneCount() - 1 )
		{
			Render_SetCurScene( hScene );
		}
		QDomNode	nodeChip	= eleScene.firstChild();
		while( !nodeChip.isNull() )
		{
			if ( !nodeChip.isElement() )
			{
				nodeChip	= nodeChip.nextSibling();
				continue;
			}
			QDomElement		eleChip		= nodeChip.toElement();
			if ( eleChip.tagName() != "Chip" )
			{
				nodeChip	= nodeChip.nextSibling();
				continue;
			}
			int				iSelChip	= eleScene.attribute( "Current", "-1" ).toInt();
			HCHIP			hChip		= Scene_CreateChip( hScene );
			if ( Chip_Open( hChip, eleChip.attribute( "Type" ).utf16(), 
				eleChip.attribute( "Source" ).utf16(),
				eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE ) )
			{
			}
			else if ( eleChip.attribute( "Type" ) == "Camera" )
			{
				Chip_Open( hChip, L"Camera", 
				Camera_GetDisplayName( 0 ),
				eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE );
			}
			QDomElement		eleRect		= eleChip.firstChildElement( "Rect" );
			if ( !eleRect.isNull() )
			{
				Chip_SetRectPercent( hChip, eleRect.attribute( "X" ).toFloat(), eleRect.attribute( "Y" ).toFloat(),
					eleRect.attribute( "W" ).toFloat(), eleRect.attribute( "H" ).toFloat(), eIgnoreAspectRatio );
			}
			QDomElement		eleClip		= eleChip.firstChildElement( "Clip" );
			if ( !eleClip.isNull() )
			{
				Chip_SetClipPercent( hChip, eleClip.attribute( "L" ).toFloat(), eleClip.attribute( "T" ).toFloat(),
					eleClip.attribute( "R" ).toFloat(), eleClip.attribute( "B" ).toFloat() );
			}

			Chip_SetViewLock( hChip, eLock_AspectRatio, eleChip.attribute( "LockAR" ).toInt() ? TRUE : FALSE );
			Chip_SetViewLock( hChip, eLock_Position, eleChip.attribute( "LockPos" ).toInt() ? TRUE : FALSE );
			Chip_SetViewLock( hChip, eLock_Size, eleChip.attribute( "LockSize" ).toInt() ? TRUE : FALSE ); 
			Chip_SetViewLock( hChip, eLock_Angle, eleChip.attribute( "LockAngle" ).toInt() ? TRUE : FALSE ); 

			Chip_SetVisible( hChip, eleChip.attribute( "Visible" ).toInt() ? TRUE : FALSE );
			Chip_SetVolume( hChip, eleChip.attribute( "Volume" ).toFloat(), eleChip.attribute( "Mutex" ).toInt() ? TRUE : FALSE );
			if ( iSelChip == Scene_GetChipCount( hScene ) - 1 )
			{
				Chip_SetCurent( hChip );
			}
			nodeChip	= nodeChip.nextSibling();
		}
		nodeScene	= nodeScene.nextSibling();
	}

	return TRUE;
}

BOOL RDLiveSdkDemo::SaveScenes()
{
	QString		szOptScenes		= QString::fromUtf16( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Scenes.xml";
	szOptScenes	= QDir::fromNativeSeparators( szOptScenes );

	QDomDocument	docScenes;

	QDomProcessingInstruction	domProc	= docScenes.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	docScenes.appendChild( domProc );
	QDomElement		eleSceneAll	= docScenes.createElement( "Scenes" );
	docScenes.appendChild( eleSceneAll );

	for ( int iScene = 0; iScene < Render_GetSceneCount(); ++iScene )
	{
		HSCENE		hScene		= Render_GetScene( iScene );
		QDomElement	eleScene	= docScenes.createElement( "Scene" );
		eleScene.setAttribute( "Name", QString::fromUtf16( Scene_GetName( hScene ) ) );
		for ( int iChip = 0; iChip < Scene_GetChipCount( hScene ); ++iChip )
		{
			HCHIP		hChip	= Scene_GetChip( hScene, iChip );
			QDomElement	eleChip	= docScenes.createElement( "Chip" );
			IPinInput_SStatusInfo	sStatus;
			Chip_GetStatusInfo( hChip, &sStatus );

			eleChip.setAttribute( "Type", QString::fromUtf16( Chip_GetClassName( hChip ) ) );
			eleChip.setAttribute( "Source", QString::fromUtf16( Chip_GetSourceName( hChip ) ) );
			eleChip.setAttribute( "CannotReuse", sStatus.bCannotReuse ? 1 : 0 );
			eleChip.setAttribute( "Paused", sStatus.eStatus == ePin_Paused ? 1 : 0 );
			eleChip.setAttribute( "Visible", Chip_IsVisible( hChip ) );
			eleChip.setAttribute( "LockAR", Chip_GetViewLock( hChip, eLock_AspectRatio )  );
			eleChip.setAttribute( "LockPos", Chip_GetViewLock( hChip, eLock_Position )  );
			eleChip.setAttribute( "LockSize", Chip_GetViewLock( hChip, eLock_Size )  );
			eleChip.setAttribute( "LockAngle", Chip_GetViewLock( hChip, eLock_Angle )  );

			QDomElement	eleRect	= docScenes.createElement( "Rect" );
			FLOAT	fX, fY, fW, fH;
			BOOL	bKeepAspectRatio;
			Chip_GetRectPercent( hChip, &fX, &fY, &fW, &fH, FALSE );
			eleRect.setAttribute( "X", fX );
			eleRect.setAttribute( "Y", fY );
			eleRect.setAttribute( "W", fW );
			eleRect.setAttribute( "H", fH );
			eleChip.appendChild( eleRect );

			QDomElement	eleClip	= docScenes.createElement( "Clip" );
			FLOAT	fL, fT, fR, fB;
			Chip_GetClipPercent( hChip, &fL, &fT, &fR, &fB );
			eleClip.setAttribute( "L", fL );
			eleClip.setAttribute( "T", fT );
			eleClip.setAttribute( "R", fR );
			eleClip.setAttribute( "B", fB );
			eleChip.appendChild( eleClip );

			eleScene.appendChild( eleChip );
		}
		int	iCurrent	= Chip_GetIndex( Sceen_GetCurChip( hScene ), NULL );
		eleScene.setAttribute( "Current", iCurrent );
		eleSceneAll.appendChild( eleScene );
	}
	eleSceneAll.setAttribute( "Current", Render_GetSceneIndex( Render_GetCurScene() ) );

	QFile			fileScenes( szOptScenes );
	if ( !fileScenes.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		return FALSE;		//�ļ���ʧ��
	}
	QTextStream	steXml( &fileScenes );
	steXml.setCodec( "UTF-8" );
	docScenes.save( steXml, 4, QDomNode::EncodingFromTextStream );
	fileScenes.close();
	return TRUE;
}

BOOL RDLiveSdkDemo::SettingToUiAndSdk()
{
	SetImageSettingToMenu();
	SetPreviewLayoutToMenu();
	ui.widEncoderSetting->SetEncoderSetting();
	SetAudioCaptureSetting();
	return TRUE;
}

void RDLiveSdkDemo::SetAudioCaptureSetting()
{
	int			iCount		= Audio_GetDevCount( eAudCap_Speaker );
	if ( iCount > 0 )
	{
		QDomElement	eleSpeaker	= FindXmlElement( "AuidoCapture/Speaker" );
		if ( !eleSpeaker.isNull() )
		{
			Audio_SetCurrent( eAudCap_Speaker, eleSpeaker.attribute( "DevId" ).utf16() );
			Audio_Enable( eAudCap_Speaker, eleSpeaker.attribute( "Mutex" ).toInt() ? FALSE : TRUE );
			Audio_SetVolume( eAudCap_Speaker, eleSpeaker.attribute( "Volume" ).toFloat() );
		}
		else
		{
			Audio_Enable( eAudCap_Speaker, TRUE );
			Audio_SetVolume( eAudCap_Speaker, 1.0f );
		}
		ui.widVolumeSpk->SetSlider( 0.0, Audio_GetVolume( eAudCap_Speaker ) );
		ui.chkSpeaker->setChecked( !Audio_IsEnabled( eAudCap_Speaker ) );
	}
	else
	{
		ui.chkSpeaker->setChecked( true );
	}
	ui.widVolumeOperSpk->setEnabled( iCount > 0 );

	iCount		= Audio_GetDevCount( eAudCap_Microphone );
	if ( iCount > 0 )
	{
		QDomElement	eleMicro	= FindXmlElement( "AuidoCapture/Microphone" );
		if ( !eleMicro.isNull() )
		{
			Audio_SetCurrent( eAudCap_Microphone, eleMicro.attribute( "DevId" ).utf16() );
			Audio_Enable( eAudCap_Microphone, eleMicro.attribute( "Mutex" ).toInt() ? FALSE : TRUE );
			Audio_SetVolume( eAudCap_Microphone, eleMicro.attribute( "Volume" ).toFloat() );
		}
		else
		{
			Audio_Enable( eAudCap_Microphone, TRUE );
			Audio_SetVolume( eAudCap_Microphone, 1.0f );
		}
		ui.widVolumeMic->SetSlider( 0.0, Audio_GetVolume( eAudCap_Microphone ) );
		ui.chkMicrophone->setChecked( !Audio_IsEnabled( eAudCap_Microphone ) );
	}
	else
	{
		ui.chkMicrophone->setChecked( true );
	}
	ui.widVolumeOperMic->setEnabled( iCount > 0 );
}

VOID RDLiveSdkDemo::UpdateVideoSizeLabel()
{
	QDomElement		eleRR	= FindXmlElement( "Base/ResolutionRatio" );
	QDomElement		eleFR	= FindXmlElement( "Base/FrameRatio" );
	if ( !eleRR.isNull() && !eleFR.isNull() )
	{
		bool	bSwapVH	= eleRR.attribute( "SwapVH" ).toInt() ? true : false;
		int		iWidth	= eleRR.attribute( "Width" ).toInt();
		int		iHeight	= eleRR.attribute( "Height" ).toInt();
		float	fps		= eleFR.attribute( "Fps" ).toFloat();
		if ( bSwapVH )
			m_pLabVideoSize->setText( QString::fromUtf16(L" ��Ƶ��%1x%2 %3f/s ").arg(iHeight).arg(iWidth).arg(fps) );
		else
			m_pLabVideoSize->setText( QString::fromUtf16(L" ��Ƶ��%1x%2 %3f/s ").arg(iWidth).arg(iHeight).arg(fps) );
	}
}

void RDLiveSdkDemo::on_timReszie_timeout()
{
	static bool bFirstResize	= true;
	if ( bFirstResize )
	{
		bFirstResize	= false;
		LoadScenes();
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eScreenCenter );
	}
	else
	{
		UpdatePreviewConfig( IGlRender_SPreviewLayout::eNotChangePos );
	}
}

void RDLiveSdkDemo::on_chkStart_toggled ( bool checked )
{
	if ( checked )	//��ʼ
	{
		ui.chkStart->setEnabled( false );
		if ( !ui.widEncoderSetting->UpdateEncoderConfig( true ) ||
			!Encoder_Start() )
		{
			ui.chkStart->setChecked( false );
			ui.chkStart->setEnabled( true );
		}
	}
	else
	{
		ui.chkStart->setEnabled( false );
		Encoder_End();
	}
}

void RDLiveSdkDemo::on_butAddChip_clicked()
{
	QPoint	pos	= ui.butAddChip->mapToGlobal( QPoint( 0, ui.butAddChip->height() ) );
	ui.menuChipInput->popup( pos );
}

void RDLiveSdkDemo::on_butCloseChip_clicked()
{
	int		iCur	= ui.listWidChips->currentRow();
	if ( iCur >= 0 )
	{
		HCHIP	hChip	= ChipItem::GetChipHandle( ui.listWidChips, iCur );
		if ( hChip ) Chip_Destroy( hChip );
	}
}

void RDLiveSdkDemo::on_butUpChip_clicked()
{
	HSCENE	hScene	= Render_GetCurScene();
	HCHIP	hChip	= Sceen_GetCurChip( hScene );
	if ( hChip )
	{
		Chip_SetIndex( hChip, Chip_GetIndex( hChip, NULL ) + 1 );
	}
}

void RDLiveSdkDemo::on_butDownChip_clicked()
{
	HSCENE	hScene	= Render_GetCurScene();
	HCHIP	hChip	= Sceen_GetCurChip( hScene );
	if ( hChip )
	{
		if ( hChip ) Chip_SetIndex( hChip, Chip_GetIndex( hChip, NULL ) - 1 );
	}
}

void RDLiveSdkDemo::on_listWidChips_currentItemChanged( QListWidgetItem * current, QListWidgetItem * previous )
{
	if ( !m_bChipListChanging && current )
	{
		ChipItem*	pChip	= (ChipItem*)ui.listWidChips->itemWidget( current );
		if ( pChip ) Chip_SetCurent( pChip->GetHChip() );
	}
}


void RDLiveSdkDemo::on_butAudioPopSpk_clicked()
{
	QActionGroup	agpMenu( this );
	QMenu		menuDev;
	INT			iCount	= Audio_GetDevCount( eAudCap_Speaker );
	LPCWSTR		pCurDev	= Audio_GetCurrent( eAudCap_Speaker );
	agpMenu.setExclusive( true );
	for ( int i = 0; i < iCount; ++i )
	{
		QAction*	pAct	= menuDev.addAction( QString::fromUtf16( Audio_GetDevName( eAudCap_Speaker, i ) ) );
		pAct->setData( QString::fromUtf16( Audio_GetDevId( eAudCap_Speaker, i ) ) );
		pAct->setCheckable( true );
		if ( pCurDev && wcscmp( pCurDev, Audio_GetDevId( eAudCap_Speaker, i ) ) == 0 )
		{
			pAct->setChecked( true );
		}
		agpMenu.addAction( pAct );
	}
	QAction*	pSel	= menuDev.exec( ui.butAudioPopSpk->mapToGlobal( QPoint( 0, ui.butAudioPopSpk->height() ) ) );
	if ( pSel )
	{
		if ( Audio_SetCurrent( eAudCap_Speaker, pSel->data().toString().utf16() ) )
		{
			QDomElement	eleSpeaker	= FindXmlElement( "AuidoCapture/Speaker", TRUE );
			eleSpeaker.setAttribute( "DevId", pSel->data().toString() );
		}
	}
}

void RDLiveSdkDemo::on_butAudioPopMic_clicked()
{
	QActionGroup	agpMenu( this );
	QMenu		menuDev;
	INT			iCount	= Audio_GetDevCount( eAudCap_Microphone );
	LPCWSTR		pCurDev	= Audio_GetCurrent( eAudCap_Microphone );
	agpMenu.setExclusive( true );
	for ( int i = 0; i < iCount; ++i )
	{
		QAction*	pAct	= menuDev.addAction( QString::fromUtf16( Audio_GetDevName( eAudCap_Microphone, i ) ) );
		pAct->setData( QString::fromUtf16( Audio_GetDevId( eAudCap_Microphone, i ) ) );
		pAct->setCheckable( true );
		if ( pCurDev && wcscmp( pCurDev, Audio_GetDevId( eAudCap_Microphone, i ) ) == 0 )
		{
			pAct->setChecked( true );
		}
		agpMenu.addAction( pAct );
	}
	QAction*	pSel	= menuDev.exec( ui.butAudioPopMic->mapToGlobal( QPoint( 0, ui.butAudioPopMic->height() ) ) );
	if ( pSel )
	{
		if ( Audio_SetCurrent( eAudCap_Microphone, pSel->data().toString().utf16() ) )
		{
			QDomElement	eleMicro	= FindXmlElement( "AuidoCapture/Microphone", TRUE );
			eleMicro.setAttribute( "DevId", pSel->data().toString() );
		}
	}
}

void RDLiveSdkDemo::on_chkSpeaker_clicked( bool checked )
{
	if ( Audio_Enable( eAudCap_Speaker, !checked ) )
	{
		QDomElement	eleSpeaker	= FindXmlElement( "AuidoCapture/Speaker", TRUE );
		eleSpeaker.setAttribute( "Mutex", checked ? 1 : 0 );
	}
}

void RDLiveSdkDemo::on_chkMicrophone_clicked( bool checked )
{
	if ( Audio_Enable( eAudCap_Microphone, !checked ) )
	{
		QDomElement	eleMicro	= FindXmlElement( "AuidoCapture/Microphone", TRUE );
		eleMicro.setAttribute( "Mutex", checked ? 1 : 0 );
	}
}

void RDLiveSdkDemo::on_widVolumeSpk_sliderRange( double fLeft, double fRight )
{
	if ( Audio_SetVolume( eAudCap_Speaker, fRight ) )
	{
		QDomElement	eleSpeaker	= FindXmlElement( "AuidoCapture/Speaker", TRUE );
		eleSpeaker.setAttribute( "Volume", fRight );
	}
}

void RDLiveSdkDemo::on_widVolumeMic_sliderRange( double fLeft, double fRight )
{
	if ( Audio_SetVolume( eAudCap_Microphone, fRight ) )
	{
		QDomElement	eleMicro	= FindXmlElement( "AuidoCapture/Microphone", TRUE );
		eleMicro.setAttribute( "Volume", fRight );
	}
}

void RDLiveSdkDemo::resizeEvent( QResizeEvent * event )
{
	if ( !m_bPreviewSizeing )
		m_timReszie->start( 100 );
}

void RDLiveSdkDemo::closeEvent( QCloseEvent * event )
{
	SaveProfile();
	SaveScenes();
	RDLive_Uninit();
}

QDomElement RDLiveSdkDemo::FindXmlElement( const QString& szPath, BOOL bCreateIfNull )
{
	QStringList		lstNode		= szPath.split( '/' );
	
	QDomElement		eleFind		= m_docOption.documentElement();  //���ظ�Ԫ��
	for ( int i = 0; i < lstNode.count(); ++i )
	{
		QDomElement	eleParent	= eleFind;
		eleFind		= eleParent.firstChildElement( lstNode[i] );
		if ( eleFind.isNull() )
		{
			if ( bCreateIfNull )
			{
				eleFind	= m_docOption.createElement( lstNode[i]);
				eleParent.appendChild( eleFind );
			}
			else
			{
				break;
			}
		}
	}
	return eleFind;
}


void RDLiveSdkDemo::on_RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, int iValue )
{
	switch( eNotify )
	{
	case eNotify_None:
		break;
	case eNotify_SceneAdding:
		on_actAddScene_triggered( false );
		break;
	case eNotify_SceneAdded:
		if ( iValue )
		{
			UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
		}
		break;
	case eNotify_SceneDeleting:
		break;
	case eNotify_SceneDeleted:
		if ( iValue )
		{
			UpdatePreviewConfig( IGlRender_SPreviewLayout::eDoNotExceedDesktop );
		}
		break;
	case eNotify_SceneSwitched:		//�л��˵�ǰѡ�еĳ���
		m_bChipListChanging	= TRUE;
		//��鵱ǰ�Ƿ���ѡ����еĻ�����Ҫ��ѡ�����и��ӵĿ��ƴ����Ƴ���ע�����Ƴ��������Ƴ���
		//������Ƴ�����ô��ɾ�������ʱ�򣬸��ӵĿ��ƴ���Ҳ�ᱻɾ����
		if ( ui.listWidChips->currentItem() )
		{
			ChipItem*	pChip	= (ChipItem*)ui.listWidChips->itemWidget( ui.listWidChips->currentItem() );
			if ( pChip ) pChip->OnChipSelected( FALSE );
		}
		//��ɾ��Ԫ���б��е�����Ԫ��
		while( ui.listWidChips->count() )
		{
			delete ui.listWidChips->takeItem( 0 );
		}
		if ( hScene )
		{
			QString	szName	= QString::fromUtf16( Scene_GetName( hScene ) );
			if ( szName.isEmpty() )
			{
				szName	= QString::fromUtf16( L"���� %1" ).arg( Render_GetSceneIndex( hScene ) + 1 );
			}
			ui.tabSector->setTabText( 0, szName );
			ui.menuChipInput->setEnabled( true );
			ui.butAddChip->setEnabled( true );
			//�������ӵ�Ԫ���б�
			for ( int i = 0; i < Scene_GetChipCount( hScene ); ++i )
			{
				new ChipItem( Scene_GetChip( hScene, i ), ui.listWidChips );
			}
		}
		else
		{
			ui.tabSector->setTabText( 0, QString::fromUtf16( L"��") );
			ui.menuChipInput->setEnabled( false );
			ui.butAddChip->setEnabled( false );
		}
		SetChipListButStatus();
		m_bChipListChanging	= FALSE;
		break;
	case eNotify_SceneNamed:
		if ( hScene == Render_GetCurScene() )
		{
			QString	szName	= QString::fromUtf16( Scene_GetName( hScene ) );
			if ( szName.isEmpty() )
			{
				szName	= QString::fromUtf16( L"���� %1" ).arg( Render_GetSceneIndex( hScene ) + 1 );
			}
			ui.tabSector->setTabText( 0, szName );
		}
		break;
	case eNotify_SceneScroll:
		break;
	case eNotify_RBDownCScene:
		break;
	case eNotify_RBDownBScene:
		break;
	case eNotify_RBUpCScene:
		break;
	case eNotify_RBUpBScene:
		break;
	}
	if ( hScene == Render_GetCurScene() )
	{
		m_bChipListChanging	= TRUE;
		if ( ChipItem::OnRenderNotify( ui.listWidChips, eNotify, hScene, iValue ) )
		{
			SetChipListButStatus();
		}
		m_bChipListChanging	= FALSE;
	}
}

void RDLiveSdkDemo::on_EncoderNotifyCB( IEncoder_ENotify eNotify, ulong dwValue, ulong ptrValue )
{
	switch( eNotify )
	{
	case eEncNotifySuccess:
		break;
	case eEncNotifyStarted:
		ui.chkStart->setEnabled( true );
		break;
	case eEncNotifyStoped:
		ui.chkStart->setChecked( false );
		ui.chkStart->setEnabled( true );
		if ( dwValue )
		{
			QMessageBox::critical( this, QString::fromUtf16( L"ֱ��/¼��" ), 
				QString::fromUtf16( L"ֱ��/¼�Ƴ�������ֹͣ��\n������룺%1" ).arg( dwValue ) );
		}
		break;
	case eEncNotifyEncodeError:
		break;
	case eEncNotifyEncodeFps:
		if ( ptrValue )
		{
			PSEncStatusEncodeFps	pFps	= (PSEncStatusEncodeFps)ptrValue;
			QString	szTime	= QString::fromUtf16( L"ʱ����%1:%2:%3  ����֡�ʣ�%4" )
				.arg( int( pFps->iRecordTime / 1000 / 60 / 60 ), 2, 10, QChar('0') )
				.arg( int( pFps->iRecordTime / 1000 / 60 ) % 60, 2, 10, QChar('0') )
				.arg( int( pFps->iRecordTime / 1000 ) % 60, 2, 10, QChar('0') )
				.arg( pFps->fCurEncodeFps, 0, 'f', 2 );
			szTime	+= QString( "(%1)" ).arg( pFps->iInpFrameNum );
			m_pLabTimeFps->setText( szTime );
		}
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
		ui.widEncoderSetting->OnAcceditDone( dwValue ? TRUE : FALSE, QString::fromUtf16( (PWORD)ptrValue ) );
		break;
	case eEncNotify_GetCloudFail:
		QMessageBox::critical( this, QString::fromUtf16( L"ֱ��/¼��" ), 
			QString::fromUtf16( L"ȡ����ֱ����ַʧ�ܡ�\n��%1" ).arg( QString::fromUtf16( PWORD(ptrValue) ) ) );
		break;
	}
}

VOID RDLiveSdkDemo::SetChipListButStatus()
{
	INT		iIndex	= ui.listWidChips->currentRow();
	ui.butCloseChip->setEnabled( iIndex >= 0 );
	ui.butUpChip->setEnabled( iIndex > 0 );
	ui.butDownChip->setEnabled( iIndex >= 0 && iIndex < ui.listWidChips->count() - 1);
}





