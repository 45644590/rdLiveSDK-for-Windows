// SimpleDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//�����༭�Ļص�
VOID WINAPI RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam )
{
}
//��Ƶ����洢�ϴ��Ļص�����Ϊ���Ա����ʱ������ʱӦ�ü��� static ��ʶ����������Ϊ�ص�����ʹ�á�
VOID WINAPI EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam )
{
	if ( eNotify == eEncNotify_AcceditDone )
	{
		if ( dwValue )
		{
			printf( "Accedit Success.\nLocal Save=%f days\nRtmp Live=%f days\nCloud Live=%f days\n",
				RDLive_GetAccreditDays(eAccreditLocalSave),
				RDLive_GetAccreditDays(eAccreditRtmpLive),
				RDLive_GetAccreditDays(eAccreditCloudLive) );
		}
		else
		{
			printf( "Accedit Fail!\n" );
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//��ʼ�� SDK����˾���Ͳ�Ʒ��������Ҫ��Ϊ�����ɱ�������ļ���Ĭ��·����
	if ( !RDLive_Init( L"17RD", L"RDLiveSdkDemo",
				RenderNotifyCB, EncoderNotifyCB, 0 ) )
	{
		//ʧ�ܴ���
		printf( "Init Fail, Error=%1\n", GetLastError() );
	}
	//��֤ SDK ����Ȩ��Ϣ�����ô˽ӿ�ʱ������Ѿ���������ô��ӱ���˾��վ�첽������µ���Ȩ��Ϣ��
	//ע�⣬��Ϊ����վ���������Ȩ��Ϣ���첽�ģ������ڸ��³ɹ��󣬻�ͨ���ص�����֪ͨ��
	if ( !RDLive_ResetAccredit( "d3200cc987431827", 
		"77a9eeea008524e8bdf10e18409cbdb3sULczML4CjomZFtst04v/HLUrHqWT72Mmkz7WhUEmpjXMH7/UWz5oGMwUGQPbYX+MKSpM01lSGQ/qNzCkFFyKXSwxrKIViR4iZ7ZxOuB6n80wDeCV7jHJSEN1+DqlCLm3dJWQF3CFLMOj2YJxwI/YDY9h3SjCsWFz9j/71RCHH0FWpr13vMRM6a1uRCnke2Tyly/V4S4E7BE1tR6WDcxNQTeX9w399l/EpNb8LvBNNUz6shNmM627BGBfTbPG2vj+grPaxv1rFcVRqNkT45Jrjvjp3PV8L6Py7fCUvK5PJ0Pb/olb9q/M2Yom+AZkSlE0FDcSKb0MG+QCE9f1MYacjFoU31o7cZb5ZQZ++7lMqXMDvTi9LyTYR+0lDKKwFC8EJ43/upbIuhawyXQ2w4u7Zvv9IUXqhamlTUirPmuV4lSVypdzCT+gPdEjq9krLLjRajAMutBwefKiHdrp/h65BxTErT94rH7OAU6bCmbX/o=" ) )
	{
		//ʧ�ܴ���
		printf( "ResetAccredit Fail, Error=%1\n", GetLastError() );
	}

	//������Ҫ¼�ơ�ֱ������Ƶ�ֱ���
	if ( FALSE == Render_SetSize( 1280, 720 ) )
	{
		//ʧ�ܴ���
	}
	//������Ҫ¼�ơ�ֱ������Ƶ֡�� 
	if ( FALSE == Render_SetFps( 25.0f ) )
	{
		//ʧ�ܴ���
	}

	//����һ���������õ����������֮��Ըó����Ĳ�������ͨ���������ɡ�
	HSCENE hScene = Render_CreateScene();
	//����һ��Ԫ�����õ�Ԫ�������֮��Ը�Ԫ���Ĳ�������ͨ���������ɡ�
	HCHIP hChip = Scene_CreateChip( hScene );

	//ΪԪ������ͼ����Դ����������Ϊ¼�Ƶ�һ����Ļ��
	if ( FALSE == Chip_Open( hChip, L"Screen", L"0", FALSE, NULL ) )
	{
		//������ 
		printf( "Open Chip Fail, Error=%d\n", GetLastError() );
	} 
	//����Ԫ����ʾ���´�����Ԫ��Ĭ�������صģ�������ƣ����Ҫ����Ϊ��ʾ��
	Chip_SetVisible( hChip, TRUE );

	//�������ͷ�����������������ͷ���Ͱѵ�һ������ͷ�Ļ������������������½�
	int		iCameraCount	= Camera_GetCount();
	if ( iCameraCount )
	{
		HCHIP	hChip	= Scene_CreateChip( Render_GetCurScene() );
		if ( hChip )
		{
			//�򿪵�һ������ͷ
			if ( Chip_Open( hChip, L"Camera", Camera_GetDisplayName( 0 ) ) )
			{
			}
			//֮ǰ���õ���Ƶ�ֱ���Ϊ 1280x720����������һ�� 320x240 ������ͷ���浽���·���
			Chip_SetRect( hChip, 900, 450, 320, 240, eKeepAspectRatio );
			Chip_SetVisible( hChip, TRUE );
		}
	}

	//������Ƶ������Ϊ x264��
	Encoder_SetCurrent( VE_X264 );
	//���� x264 �������� Perset Ϊ Medium��
	Encoder_SetPreset( VE_X264, VP_x264_Medium );
	//��������Ϊ 1000Kbps��ģʽΪ��ƽ�����ʡ��� 
	Encoder_SetBitrate( VR_AverageBitrate, 1000, 0, 0 ); 
	//����¼���Ĳ���Ϊ 2 ������22050hz 16bit �����ʣ�AAC ���룬���� 96Kbps�� 
	Encoder_SetAudioParams( 2, Aud_Inp_Samp_22050, 16, Aud_Enc_AAC, 96 );
	//���ñ����ļ���ֱ����Ϣ����ؽṹ��
	SEncoderSaveFile sFile;
	ZeroMemory( &sFile, sizeof( sFile ) );
	sFile.eContainer = Mix_FLV;    //�����ļ���ʽΪ FLV
	sFile.szSavePath = L"D:\\Movie\\abc.flv";    //�����ļ�����·��
	//��ӵ���Ƶ����б�
	if ( -1 == Encoder_AddSaveFile( &sFile ) )
	{
		//ʧ�ܿ���������û�л����Ȩ������·����ʽ����ȷ
		printf( "AddSaveFile Fail, Error=%d\n", GetLastError() );
	} 
	//������һ�������Ϊ RTMP ֱ����
	//sFile.eContainer = Mix_RTMP;    //��������Ϊ RTMP ֱ��
	//sFile.szSavePath = L"rtmp://192.168.0.1/live/abc";    //����ֱ���� URL
	//if ( -1 == Encoder_AddSaveFile( &sFile ) )
	//{
	//	//ʧ�ܿ���������û�л����Ȩ������URL��ʽ����ȷ
	//	printf( "AddSaveFile Fail, Error=%d\n", GetLastError() );
	//} 
	//��ʼ¼�ƺ�ֱ����
	if ( FALSE == Encoder_Start() )
	{
		//ͨ��������û�������κ�����ŵ��·���ʧ�ܡ���Ϊ���ļ�����������Ĳ����ȵȶ����첽�ģ����ʧ�ܻ�ͨ���ص���������֪ͨ��
		printf( "Start Fail, Error=%d\n", GetLastError() );
	}

	printf( "Recording, Press Enter key Exit!\n" );
	getchar();
	//����ֹͣ����
	Encoder_End();
	//���÷���ʼ��
	RDLive_Uninit();
	return 0;
}

