#ifndef RDLIVESDKDEMO_H
#define RDLIVESDKDEMO_H

#include <QtGui/QMainWindow>
#include "ui_rdlivesdkdemo.h"

class RDLiveSdkDemo : public QMainWindow
{
	Q_OBJECT

public:
	RDLiveSdkDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RDLiveSdkDemo();

private:
	Ui::RDLiveSdkDemoClass ui;

	QDomDocument	m_docOption;
	QLabel*			m_pLabVideoSize;
	QLabel*			m_pLabFScale;
	QLabel*			m_pLabTimeFps;
	BOOL			m_bPreviewSizeing;		//��ǣ���ǰ�Ĵ��������ǳ����Լ����õģ��������û���������˲���Ҫ���¼���Ӧ�н����κδ���
	QTimer*			m_timReszie;
	BOOL			m_bChipListChanging;	//��ǣ���ǰ��Ԫ���б���޸��ǳ����Լ����õģ��������û���������˲���Ҫ���¼���Ӧ�н����κδ���

	//���������ļ�
	BOOL LoadProfile();
	//�ѽ����ϵ����ø��µ������ļ���SDK������
	BOOL SaveProfile();
	//�������ļ����������õ������SDK
	BOOL SettingToUiAndSdk();

	//���볡�������ļ�
	BOOL LoadScenes();
	BOOL SaveScenes();

	QDomElement FindXmlElement( const QString& szPath, BOOL bCreateIfNull = FALSE );

	VOID UpdateVideoSizeLabel();
	static VOID WINAPI RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam );
	static VOID WINAPI EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam );
	VOID SetChipListButStatus();

	void	resizeEvent( QResizeEvent * event );
	void	closeEvent( QCloseEvent * event );

	IGlRender_SPreviewLayout m_sPerviewLayout;	//Ԥ������
	QActionGroup*	m_agpSwictScene;	//�˵���ѡ�������л��˵���
	QActionGroup*	m_agpBSceneCount;	//�˵���ѡ����̨������ʾ����
	QActionGroup*	m_agpBScenePos;		//�˵���ѡ����̨������ʾ�ڵ�ǰ�ĳ��������������ĸ�λ��
	//�������ļ��е����ø��µ������ SDK
	void SetImageSettingToMenu();
	void SetPreviewLayoutToMenu( BOOL bRestore = FALSE );
	void SetAudioCaptureSetting();
	//�Ѷ�Ԥ���Ľ������ã����µ� SDK �Լ� XML ����Ȼ����ݷ���ֵ���贰��
	void UpdatePreviewConfig( IGlRender_SPreviewLayout::EReposWnd eMode );
	//
signals:
	void	signalRenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, int iValue );
	void	signalEncoderNotifyCB( IEncoder_ENotify eNotify, ulong dwValue, ulong ptrValue );
public slots:
	void on_timReszie_timeout();
	void on_RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, int iValue );
	void on_EncoderNotifyCB( IEncoder_ENotify eNotify, ulong dwValue, ulong ptrValue );
	//��ʼ
	void on_chkStart_toggled ( bool checked );
	//Ԫ������
	void on_butAddChip_clicked();
	void on_butCloseChip_clicked();
	void on_butUpChip_clicked();
	void on_butDownChip_clicked();
	void on_listWidChips_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );

	//¼������
	void on_butAudioPopSpk_clicked();
	void on_butAudioPopMic_clicked();
	void on_chkSpeaker_clicked( bool checked );
	void on_chkMicrophone_clicked( bool checked );
	void on_widVolumeSpk_sliderRange( double fLeft, double fRight );
	void on_widVolumeMic_sliderRange( double fLeft, double fRight );
	
	//ͼ��˵�
	void on_agpImageSize_triggered ( QAction * action );
	void on_menuFpses_triggered ( QAction * action );
	void on_menuPreviewScale_triggered ( QAction * action );
	void on_actSwapVH_triggered( bool checked );
	void on_actPreviewOpenGL_triggered( bool checked );

	//�������ֲ˵�
	void on_menuScene_aboutToShow();
	void on_menuSwitchScene_triggered ( QAction * action );
	void on_menuDeleteScene_triggered ( QAction * action );
	void on_menuRenameScene_triggered ( QAction * action );
	void on_actAddScene_triggered( bool checked );
	void on_actClearCurScene_triggered( bool checked );
	void on_menuPreviewLayout_triggered( QAction * action );
	
	//�������ݲ˵�
	void on_menuAddScreen_aboutToShow();
	void on_menuAddCamera_aboutToShow();
	void on_menuAddScreen_triggered ( QAction * action );
	void on_menuAddCamera_triggered ( QAction * action );
	void on_actiAddPicture_triggered( bool checked );
};

#endif // RDLIVESDKDEMO_H
