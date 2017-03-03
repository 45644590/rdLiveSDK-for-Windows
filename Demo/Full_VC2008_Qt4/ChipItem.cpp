#include "StdAfx.h"
#include "chipitem.h"

ChipItemOper*	ChipItem::m_pItemOper	= NULL;
ChipItem::ChipItem( HCHIP hChip, QListWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ZeroMemory( &m_sScrCapParams, sizeof( m_sScrCapParams ) );
	ZeroMemory( &m_sChipStatus, sizeof( m_sChipStatus ) );

	m_listWidChips		= parent;
	m_hChip				= hChip;
	if ( NULL == m_pItemOper )
	{
		m_pItemOper	= new ChipItemOper( parent );
		m_pItemOper->hide();
	}
	m_lwItem			= new QListWidgetItem;

	INT		iIndex	= Chip_GetIndex( m_hChip, NULL );
	iIndex	= m_listWidChips->count() - iIndex;
	//�����ϵ�Ԫ���б���ʾ˳����ʵ��˳�����෴�ģ�Ҳ���������ӵ���ʾ����ǰ��
	//��������ӵ�Ԫ��ʼ�ղ�����ʾ�б��˵�λ�á�
	m_siBase	= size();

	m_listWidChips->insertItem( iIndex, m_lwItem );
	m_listWidChips->setItemWidget( m_lwItem, this );
	m_lwItem->setSizeHint( m_siBase );


	if ( Sceen_GetCurChip( Render_GetCurScene() ) == m_hChip )
	{
		m_listWidChips->setCurrentRow( iIndex );
		OnChipSelected();
	}
	OnStatusChanged();
}

ChipItem::~ChipItem()
{
	m_hChip	= 0;
}

BOOL ChipItem::OnRenderNotify( QListWidget* pList, IRender_ENotify eNotify, HSCENE hScene, int iValue )
{
	ChipItem*	pChItem	= NULL;
	SHORT		shLVal	= (SHORT)( iValue & 0xFFFF );
	SHORT		shHVal	= (SHORT)( iValue >> 16 );
	//��Ϊ�б����ʾ˳���ʵ��˳���෴������Ҫ�ȼ������ǰɾ������Ŀ����ʾ�б��е�λ�á�
	INT		iViewLow	= pList->count() - shLVal - 1;
	INT		iViewHigh	= pList->count() - shHVal - 1;
	if ( shLVal < 0 || iViewLow >= pList->count() ) iViewLow = -1;
	if ( shHVal < 0 || iViewHigh >= pList->count() ) iViewHigh = -1;

	switch( eNotify )
	{
	case eNotify_ChipAdding:
		break;
	case eNotify_ChipAdded:
		new ChipItem( Scene_GetChip( hScene, shLVal ), pList );
		return TRUE;
	case eNotify_ChipDeleting:
		break;
	case eNotify_ChipDeleted:
		pChItem	= ChipItem::GetChipItem( pList, iViewLow );
		if ( pChItem && pChItem->m_hChip == m_pItemOper->GetHChip() )
		{
			pChItem->OnChipSelected( FALSE );
		}
		delete pList->takeItem( iViewLow );
		//ɾ�� item �������б��ѡ����ı䣬��������ѡ���
		iViewLow = pList->currentRow();
		pChItem	= ChipItem::GetChipItem( pList, iViewLow );
		if ( pChItem ) Chip_SetCurent( pChItem->m_hChip );
		return TRUE;
	case eNotify_ChipPosing:
		break;
	case eNotify_ChipPosed:
		pChItem	= GetChipItem( pList, iViewLow );
		if ( pChItem ) pChItem->OnPosChanged();
		break;
	case eNotify_ChipRotating:
		break;
	case eNotify_ChipRotated:
		break;
	case eNotify_ChipSwitched:
		pList->setCurrentRow( iViewLow );
		pChItem	= ChipItem::GetChipItem( pList, iViewHigh );
		if ( pChItem ) pChItem->OnChipSelected( FALSE );
		pChItem	= ChipItem::GetChipItem( pList, iViewLow );
		if ( pChItem ) pChItem->OnChipSelected( TRUE );
		return TRUE;
	case eNotify_ChipIndexChg:
		if ( iViewLow >= 0 && iViewHigh >= 0 )
		{
			ChipItem::MoveChipItem( pList, iViewLow, iViewHigh );
			return TRUE;
		}
		break;
	case eNotify_ChipStatus:
	case eNotify_ChipVisible:
	case eNotify_ChipViewLock:
		if ( iValue >= 0 )
		{
			pChItem	= ChipItem::GetChipItem( pList, iViewLow );
			if ( pChItem ) pChItem->OnStatusChanged();
		}
		break;
	case eNotify_ChipSourceSize:
		break;
	}

	return FALSE;
}

HCHIP ChipItem::GetChipHandle( QListWidget* pList, int iViewIndex )
{
	QListWidgetItem*	pItem	= pList->item( iViewIndex );
	if ( pItem )
	{
		ChipItem*	pChip	= (ChipItem*)pList->itemWidget( pItem );
		return pChip ? pChip->m_hChip : NULL;
	}
	return NULL;
}

ChipItem* ChipItem::GetChipItem( QListWidget* pList, int iViewIndex )
{
	QListWidgetItem*	pItem	= pList->item( iViewIndex );
	if ( pItem )
	{
		ChipItem*	pChip	= (ChipItem*)pList->itemWidget( pItem );
		return pChip;
	}
	return NULL;
}

BOOL ChipItem::MoveChipItem( QListWidget* pList, int iViewFrom, int iViewTo )
{
	QListWidgetItem*	pItem	= pList->item( iViewFrom );
	if ( pItem )
	{
		ChipItem*	pChip	= (ChipItem*)pList->itemWidget( pItem );
		if ( pChip )
		{
			HCHIP	hChip	= pChip->m_hChip;;
			delete pList->takeItem( iViewFrom );
			new ChipItem( hChip, pList );
			return TRUE;
		}
	}
	return FALSE;
}

VOID ChipItem::OnStatusChanged()
{
	if ( !Chip_GetStatusInfo( m_hChip, &m_sChipStatus ) )
	{
		return;
	}
	if ( m_sChipStatus.eStatus == ePin_Opening || m_szClass.isEmpty() )
	{
		DisplaySourceName();
	}
	SetLabelIcon();
	BOOL	bSeted	= FALSE;
	BOOL	bCurr	= ( this == (ChipItem*)m_listWidChips->itemWidget( m_listWidChips->currentItem() ) );

	if ( m_sChipStatus.eStatus >= ePin_Opened )
	{
		IPinInput_SCharacteristics	sCharact	= {0};
		if ( Chip_GetCharacteristics( m_hChip, &sCharact ) )
		{
			if ( sCharact.bExistImage || sCharact.bExistMovie )
			{
				ui.chkHideChip->setChecked( !Chip_IsVisible( m_hChip ) );
				ui.chkLockAspect->setChecked( Chip_GetViewLock( m_hChip, eLock_AspectRatio ) );
				ui.chkLockAspect->setChecked( Chip_GetViewLock( m_hChip, eLock_AspectRatio ) );
				ui.chkLockPos->setChecked( Chip_GetViewLock( m_hChip, eLock_Position ) );
				ui.chkLockSize->setChecked( Chip_GetViewLock( m_hChip, eLock_Size ) );
				ui.chkLockAngle->setChecked( Chip_GetViewLock( m_hChip, eLock_Angle ) );
				ui.widImgOper->setEnabled( true );
				if ( bCurr && m_pItemOper->isHidden() )
				{
					m_lwItem->setSizeHint( QSize( m_siBase.width(), m_siBase.height() + m_pItemOper->height() ) );
					m_pItemOper->OnChipSelected( m_hChip );
					m_pItemOper->show();
				}
				bSeted	= TRUE;
			}
		}
	}
	if ( !bSeted )
	{
		ui.chkHideChip->setChecked( true );
		ui.chkLockAspect->setChecked( false );
		ui.chkLockPos->setChecked( false );
		ui.chkLockSize->setChecked( false );
		ui.chkLockAngle->setChecked( false );
		ui.widImgOper->setEnabled( false );
		if ( bCurr )
		{
			m_pItemOper->hide();
			m_lwItem->setSizeHint( m_siBase );
		}
	}

	//switch( sStatus.eStatus )
	//{
	//case ePin_None:		//û������Դ
	//case ePin_Error:		//Դ����
	//	break;
	//case ePin_Closed:		//�Ѿ��ر�
	//case ePin_Opened:		//Դ�����
	//case ePin_Stoped:		//Դ�Ѿ�ֹͣ
	//case ePin_Paused:		//�Ѿ���ͣ
	//case ePin_Played:		//���ڲ���
	//	break;
	//case ePin_Closing:	//���ڹر�
	//	break;
	//case ePin_Opening:	//���ڴ�
	//	break;
	//case ePin_Stoping:	//����ֹͣ
	//	break;
	//case ePin_Pausing:	//������ͣ
	//	break;
	//case ePin_Loading:	//��������
	//	break;
	//}
}

VOID ChipItem::OnChipSelected( BOOL bSelect )
{
	if ( bSelect )
	{
		ui.verticalLayout->addWidget( m_pItemOper );
		if ( Chip_GetStatus( m_hChip ) >= ePin_Opened )
		{
			m_lwItem->setSizeHint( QSize( m_siBase.width(), m_siBase.height() + m_pItemOper->height() ) );
			m_pItemOper->OnChipSelected( m_hChip );
			m_pItemOper->show();
		}
		else
		{
			m_pItemOper->hide();
			m_lwItem->setSizeHint( m_siBase );
		}
	}
	else
	{
		m_pItemOper->hide();
		m_pItemOper->setParent( m_listWidChips );
		ui.verticalLayout->removeWidget( m_pItemOper );
		m_lwItem->setSizeHint( m_siBase );
	}
}

VOID ChipItem::OnPosChanged()
{
}

VOID ChipItem::DisplaySourceName()
{
	m_szClass	= QString::fromUtf16( Chip_GetClassName( m_hChip ) );
	m_szSource	= QString::fromUtf16( Chip_GetSourceName( m_hChip ) );
	QString	szName;
	QFontMetrics	fm( ui.labSourceName->font() );
	if ( m_szClass.isEmpty() )
	{
		szName	= "Unknow";
	}
	else if ( m_szClass == "Picture" )
	{
		szName	= m_szSource;
		szName	= fm.elidedText( szName, Qt::ElideLeft, ui.labSourceName->width() );
	}
	else if ( m_szClass == "Camera" )
	{
		int	iCamera	= Camera_GetIndex( m_szSource.utf16() );
		if ( iCamera >= 0 ) 
			szName	= QString::fromUtf16( Camera_GetFriendlyName( iCamera ) );
		szName	= fm.elidedText( szName, Qt::ElideRight, ui.labSourceName->width() );
	}
	else if ( m_szClass == "Screen" )
	{
		if ( Screen_AnalysisSource( m_szSource.utf16(), &m_sScrCapParams ) )
		{
			if ( m_sScrCapParams.szWindow[0] )
			{
				szName	= QString::fromUtf16( L"���ڣ�" );
				szName	+= QString::fromUtf16( m_sScrCapParams.szWindow ); 
				szName	= fm.elidedText( szName, Qt::ElideRight, ui.labSourceName->width() );
			}
			else if ( m_sScrCapParams.hWindow )
			{
				szName	= QString::fromUtf16( L"���ڣ�" );
				WCHAR	szWindow[MAX_PATH]	= {0};
				if ( 0 == InternalGetWindowText( m_sScrCapParams.hWindow, szWindow, ARRAYSIZE( szWindow ) ) || 0 == szWindow[0] )
				{
					wsprintf( szWindow, L"0x08X", m_sScrCapParams.hWindow );
					szName	+= QString::fromUtf16( szWindow );
				}
				szName	+= QString::fromUtf16( szWindow );
				szName	= fm.elidedText( szName, Qt::ElideRight, ui.labSourceName->width() );
			}
			else if ( m_sScrCapParams.iScreen < 0 )
			{
				if ( m_sScrCapParams.bUseInitRect )
				{
					szName	= QString::fromUtf16( L"���� [%1x%2]" )
						.arg( m_sScrCapParams.rtInit.right ).arg( m_sScrCapParams.rtInit.bottom );
				}
				else
				{
					MONITORINFOEXW	monit;
					QRect			rtScreen;
					for ( int i = 0; i < Screen_GetCount(); ++i )
					{
						Screen_GetInfo( i, &monit );
						rtScreen	|= QRect( monit.rcMonitor.left, monit.rcMonitor.top, 
							monit.rcMonitor.right - monit.rcMonitor.left, monit.rcMonitor.bottom - monit.rcMonitor.top );
					}
					szName	= QString::fromUtf16( L"�������� [%1x%2]" )
						.arg( rtScreen.width() ).arg( rtScreen.height() );
				}
			}
			else
			{
				if ( m_sScrCapParams.bUseInitRect )
				{
					szName	= QString::fromUtf16( L"��Ļ %1 (%3,%4)-(%5,%6)" ).arg( m_sScrCapParams.iScreen )
						.arg( m_sScrCapParams.rtInit.left ).arg( m_sScrCapParams.rtInit.top )
						.arg( m_sScrCapParams.rtInit.right ).arg( m_sScrCapParams.rtInit.bottom );
				}
				else
				{
					MONITORINFOEXW	monit;
					QRect			rtScreen;
					Screen_GetInfo( m_sScrCapParams.iScreen, &monit );
					rtScreen	= QRect( monit.rcMonitor.left, monit.rcMonitor.top, 
						monit.rcMonitor.right - monit.rcMonitor.left, monit.rcMonitor.bottom - monit.rcMonitor.top );

					szName	= QString::fromUtf16( L"��Ļ %1 (%3,%4)-(%5,%6)" ).arg( m_sScrCapParams.iScreen )
						.arg( rtScreen.x() ).arg( rtScreen.y() ).arg( rtScreen.right() + 1 ).arg( rtScreen.bottom() + 1 );
				}
			}
		}
	}
	ui.labSourceName->setText( szName );
}

VOID ChipItem::SetLabelIcon()
{
	static QImage	imgNone( ":/Resources/ItemToolbar/inpNone32.png" );
	static QImage	imgPicture( ":/Resources/ItemToolbar/inpPicture32.png" );
	static QImage	imgCamera( ":/Resources/ItemToolbar/inpCamera32.png" );
	static QImage	imgSceeen( ":/Resources/ItemToolbar/inpDesktop32.png" );
	static QImage	imgWindow( ":/Resources/ItemToolbar/inpWindow32.png" );
	static QImage	imgMovie( ":/Resources/ItemToolbar/inpMovie32.png" );

	static QImage	imgStaError( ":/Resources/ItemToolbar/inpStatusError.png" );
	static QImage	imgStaStoped( ":/Resources/ItemToolbar/inpStatusStoped.png" );
	static QImage	imgStaPaused( ":/Resources/ItemToolbar/inpStatusPaused.png" );
	static QImage	imgStaPlayed( ":/Resources/ItemToolbar/inpStatusPlayed.png" );
	static QImage	imgStaClosing( ":/Resources/ItemToolbar/inpStatusClosing.png" );
	static QImage	imgStaOpening( ":/Resources/ItemToolbar/inpStatusOpening.png" );
	static QImage	imgStaStoping( ":/Resources/ItemToolbar/inpStatusStoping.png" );
	static QImage	imgStaPausing( ":/Resources/ItemToolbar/inpStatusPausing.png" );
	static QImage	imgStaLoading( ":/Resources/ItemToolbar/inpStatusLoading.png" );

	QImage	imgIcon;
	if ( m_szClass.isEmpty() )
	{
		imgIcon	= imgNone;
	}
	else if ( m_szClass == "Picture" )
	{
		imgIcon	= imgPicture;
	}
	else if ( m_szClass == "Camera" )
	{
		imgIcon	= imgCamera;
	}
	else if ( m_szClass == "Screen" )
	{
		if ( m_sScrCapParams.szWindow[0] || m_sScrCapParams.hWindow )
		{
			imgIcon	= imgWindow;
		}
		else
		{
			imgIcon	= imgSceeen;
		}
	}
	QPainter	pnt( &imgIcon );

	switch( m_sChipStatus.eStatus )
	{
	case ePin_None:		//û������Դ
		break;
	case ePin_Error:		//Դ����
		pnt.drawImage( 16, 16, imgStaError );
		break;
	case ePin_Closed:		//�Ѿ��ر�
	case ePin_Closing:	//���ڹر�
		for ( int y = 0; y < imgIcon.height(); ++y )
		{
			LPBYTE	pPix	= imgIcon.scanLine( y );
			for ( int x = 0; x < imgIcon.width(); ++x )
			{
				BYTE	c = ( pPix[0] + pPix[1] + pPix[2] ) / 3;
				pPix[0]	= c;
				pPix[1]	= c;
				pPix[2]	= c;
				pPix	+= 4;
			}
		}
		if ( m_sChipStatus.eStatus == ePin_Closing ) pnt.drawImage( 16, 16, imgStaClosing );
		break;
	case ePin_Opened:		//Դ�����
		break;
	case ePin_Stoped:		//Դ�Ѿ�ֹͣ
		pnt.drawImage( 16, 16, imgStaStoped );
		break;
	case ePin_Paused:		//�Ѿ���ͣ
		pnt.drawImage( 16, 16, imgStaPaused );
		break;
	case ePin_Played:		//���ڲ���
		pnt.drawImage( 16, 16, imgStaPlayed );
		break;
	case ePin_Opening:	//���ڴ�
		pnt.drawImage( 16, 16, imgStaOpening );
		break;
	case ePin_Stoping:	//����ֹͣ
		pnt.drawImage( 16, 16, imgStaStoping );
		break;
	case ePin_Pausing:	//������ͣ
		pnt.drawImage( 16, 16, imgStaPausing );
		break;
	case ePin_Loading:	//��������
		pnt.drawImage( 16, 16, imgStaLoading );
		break;
	}
	ui.labChipIcon->setPixmap( QPixmap::fromImage( imgIcon ) );
}

void ChipItem::on_chkHideChip_clicked( bool checked )
{
	Chip_SetVisible( m_hChip, checked ? FALSE : TRUE );
}

void ChipItem::on_chkLockAspect_clicked( bool checked )
{
	Chip_SetViewLock( m_hChip, eLock_AspectRatio, checked ? TRUE : FALSE );
}
void ChipItem::on_chkLockPos_clicked( bool checked )
{
	Chip_SetViewLock( m_hChip, eLock_Position, checked ? TRUE : FALSE );
}
void ChipItem::on_chkLockSize_clicked( bool checked )
{
	Chip_SetViewLock( m_hChip, eLock_Size, checked ? TRUE : FALSE );
}
void ChipItem::on_chkLockAngle_clicked( bool checked )
{
	Chip_SetViewLock( m_hChip, eLock_Angle, checked ? TRUE : FALSE );
}

void ChipItem::on_butMiddle_clicked( bool checked )
{
	FLOAT fWidth, fHeight;
	Chip_GetRectPercent( m_hChip, NULL, NULL, &fWidth, &fHeight, FALSE );
	IChip_EBorderFitMode	eFitMode	= Chip_GetViewLock( m_hChip, eLock_AspectRatio ) ? eKeepAspectRatio : eIgnoreAspectRatio;
	Chip_SetRectPercent( m_hChip, ( 1.0f - fWidth ) * 0.5f, ( 1.0f - fHeight ) * 0.5f, fWidth, fHeight, eFitMode );
}

void ChipItem::on_butScaleFull_clicked( bool checked )
{
	IChip_EBorderFitMode	eFitMode	= eIgnoreAspectRatio;
	FLOAT fX, fY, fWidth, fHeight;
	Chip_GetRectPercent( m_hChip, &fX, &fY, &fWidth, &fHeight, FALSE );
	if ( Chip_GetViewLock( m_hChip, eLock_AspectRatio ) )
	{
		if ( ( fX == 0.0f && fWidth == 1.0f && fY >= 0.0f && fHeight <= 1.0f ) ||
			( fX >= 0.0f && fWidth <= 1.0f && fY == 0.0f && fHeight == 1.0f ) )
		{
			eFitMode	= eKeepAspectRatioByExpanding;
		}
		else
		{
			eFitMode	= eKeepAspectRatio;
		}
	}
	Chip_SetRectPercent( m_hChip, 0.0f, 0.0f, 1.0f, 1.0f, eFitMode );
}

