//---------------------------------------------------------------------
//	RenderWidget.h//ÖÐÑë´°¿Ú
//---------------------------------------------------------------------
#pragma once


//---------------------------------------------------------------------
#include <QWidget>
#include "Item.h"
#include "IListener.h"


//---------------------------------------------------------------------
//	incomplete types.
//---------------------------------------------------------------------
class QRubberBand;


//---------------------------------------------------------------------
enum SelectionType
{
	ST_ItemSelection,
	ST_ItemDrawLine,
	ST_ItemTranslation,
	ST_Create,
	ST_BackGroundPix,
	ST_Num

};


//---------------------------------------------------------------------
class RenderWidget : public QWidget, public IListener
{
	Q_OBJECT

public:
	RenderWidget(QWidget* parent);
	~RenderWidget();

	virtual void keyReleaseEvent(QKeyEvent* event) override;
	void setBkWidth(unsigned int uiWidth);
	void setBkHeight(unsigned int uiHeight);
	void drawLine();
	QSize sizeHint() const;
	void detachGraph();

	//---------------------------------------------------------------------
	//	implementation of IListener.
	//---------------------------------------------------------------------
	virtual void handleMessage(IListener::MsgType) override;

	//---------------------------------------------------------------------

public:
	static SelectionType ms_eType;

signals:
	void timeout();

protected:
	void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

public slots:
	void loadPixmap(QString);
	void updating();


private:
	QRubberBand* m_pRubberBand;
	bool m_bSelectionLocked;
	bool m_bRSelectionLocket;
	QPoint* m_pStartPoint;
	QPoint* m_pItemStartPt;
	QPoint* m_pRBStartPt;
	QPoint* m_pRBLineStart;
	QPoint* m_pRBLineEnd;
	QPixmap* m_pBackgroundPicture;
	QRect* m_pBkPicRect;
	QString m_strPicName;
	unsigned int m_uiBkWidth;
	unsigned int m_uiBkHeight;
	int fbWidth, fbHeight;

};


//---------------------------------------------------------------------





