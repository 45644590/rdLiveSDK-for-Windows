#ifndef RANGESLIDER_H
#define RANGESLIDER_H

#include <QWidget>

class CRangeSlider : public QWidget
{
	Q_OBJECT

public:
	CRangeSlider(QWidget *parent);
	~CRangeSlider();

	VOID SetRange( double fMin, double fMax );
	VOID GetRange( double* pMin, double* pMax );

	VOID SetSlider( double fSliderMin, double fSliderMax );
	double GetSliderMin() { return m_fSliderMin; }
	double GetSliderMax() { return m_fSliderMax; };
	VOID SetProgress( double fPos, BOOL bEnable );
	double GetProgress() { return m_fPos; }

	VOID SetColors( const QColor& coFrame, const QColor& coBack,
		const QColor& coSlider, const QColor& coLine, 
		const QColor& coProgress, const QColor& coText ); 

	enum	EHit
	{
		eHit_None,
		eHit_Left,
		eHit_Right,
		eHit_LeftLine,
		eHit_RightLine,
		eHit_Slider,
	};

	enum	ETextDraw
	{
		//��ʾֵ������ģʽ��ֻ��ʹ������һ����
		eTex_None		= 0,	//ʲô������ʾ
		eTex_Percent	= 1,	//��ʾΪ�ٷ���
		eTex_Number		= 2,	//��ʾΪ����
		//��ʾֵ��λ�� MASK��ʹ�ö��ֵ��ϡ�
		eTex_Left		= 0x10,	//��ʾ��ߵ�ֵ
		eTex_Right		= 0x20,
		eTex_Middle		= 0x40,
		//
		eTex_RightRange	= 0x100,	//�Ҳ��ֵʹ����ʾΪ�Ҳ�ճ��ķ�Χ������ֱ����ʾ�ұߵ�ֵ��
		eTex_MidSlider	= 0x200,	//�м��ֵ���滬���ƶ���
	};

	VOID SetText( DWORD eDrawMask, int iPrecision, int iSpace );
	VOID SetHitMask( DWORD dwHitMask )	{ m_dwHitMask = dwHitMask; }
signals:
	void sliderRange( double fLeft, double fRight );
	void progress( double fProgress );
private:
	virtual void	mouseMoveEvent ( QMouseEvent * event );
	virtual void	mousePressEvent ( QMouseEvent * event );
	virtual void	mouseReleaseEvent ( QMouseEvent * event );
	virtual void	paintEvent ( QPaintEvent * event );
	virtual void	resizeEvent ( QResizeEvent * event );
	void CalcPos( int iWidth );
	void CalcHit( int x, int y );

	double	m_fMax;
	double	m_fMin;
	double	m_fSliderMin;
	double	m_fSliderMax;
	double	m_fPos;
	BOOL	m_bEnablePos;
	QColor	m_coFrame;
	QColor	m_coBack;
	QColor	m_coSlider;
	QColor	m_coLine;
	QColor	m_coProgress;

	int		m_iLeft;
	int		m_iRight;
	int		m_iPos;
	DWORD	m_dwHitMask;
	EHit	m_eHit;
	int		m_iOffset;

	DWORD	m_dwTexDraw;
	int		m_iPrecision;
	int		m_iTexSpace;
	QColor	m_coText;
};

#endif // RANGESLIDER_H
