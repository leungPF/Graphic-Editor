//---------------------------------------------------------------------
//	RenderWidget.cpp
//---------------------------------------------------------------------
#include "renderwidget.h"
#include <QtGui>
#include <QRubberBand>
#include "ResourceManager.h"
#include "ItemManager.h"
#include "ItemFactory.h"


//---------------------------------------------------------------------
//	global variables...
//---------------------------------------------------------------------
SelectionType RenderWidget::ms_eType = ST_Create;


//---------------------------------------------------------------------
//	Const variables.
//---------------------------------------------------------------------
const int SCREENWIDTH{ 800 };
const int SCREENHEIGHT{ 600 };


//---------------------------------------------------------------------
RenderWidget::RenderWidget(QWidget* parent)
	: QWidget(parent), m_bRSelectionLocket(false),
	m_bSelectionLocked(false), m_pBackgroundPicture(nullptr),
	m_uiBkWidth(500), m_uiBkHeight(400), m_pRubberBand(nullptr)
{
	m_pBkPicRect = new QRect(70, 70, m_uiBkWidth, m_uiBkHeight);
	m_pStartPoint = new QPoint();
	m_pRBStartPt = new QPoint();
	m_pRBLineEnd = new QPoint();
	m_pItemStartPt = new QPoint();

	m_pBackgroundPicture = new QPixmap();

	// initialize center widget's size. 
	fbWidth = SCREENWIDTH; fbHeight = SCREENHEIGHT;

	// register listener...
	ItemManager::getSingleton()->registerListener(IListener::MT_Update, this);
	ResourceManager::registerListener(IListener::MT_SetPicture, this);
	ResourceManager::registerListener(IListener::MT_GetPicture, this);

	// draw background...
	QPixmap pm(40, 40);
	QPainter pmp(&pm);
	pmp.fillRect(0, 0, 20, 20, QColor(245, 245, 245));
	pmp.fillRect(20, 20, 20, 20, QColor(245, 245, 245));
	pmp.fillRect(0, 20, 20, 20, Qt::lightGray);
	pmp.fillRect(20, 0, 20, 20, Qt::lightGray);
	pmp.end();

	QPalette pal = palette();
	pal.setBrush(backgroundRole(), QBrush(pm));
	setAutoFillBackground(true);
	setPalette(pal);

}


//---------------------------------------------------------------------
RenderWidget::~RenderWidget()
{
	if (m_pRubberBand)
	{
		delete m_pRubberBand;
		m_pRubberBand = nullptr;
	}

}


//---------------------------------------------------------------------
void
RenderWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Delete)
	{
		Item* pItem = ItemManager::getSingleton()->currentItem();
		ItemManager::getSingleton()->deleteItem(pItem);

		update();
	}
}


//---------------------------------------------------------------------
void
RenderWidget::setBkWidth(unsigned int uiWidth)
{
	m_uiBkWidth = uiWidth;
	m_pBkPicRect->setWidth(m_uiBkWidth);

	update();
}


//---------------------------------------------------------------------
void
RenderWidget::setBkHeight(unsigned int uiHeight)
{
	m_uiBkHeight = uiHeight;
	m_pBkPicRect->setHeight(m_uiBkHeight);

	update();
}


//---------------------------------------------------------------------
void
RenderWidget::drawLine()
{
	// draw a temp line.
	QPainter painter(this);

	painter.drawLine(*m_pStartPoint, *m_pRBLineEnd);
}


//---------------------------------------------------------------------
QSize
RenderWidget::sizeHint()
const
{
	QSize size = QSize(fbWidth, fbHeight);
	return size;

}


//---------------------------------------------------------------------
void
RenderWidget::detachGraph()
{
	delete m_pBackgroundPicture;
	m_pBackgroundPicture = nullptr;

}


//---------------------------------------------------------------------
void
RenderWidget::handleMessage(IListener::MsgType eMessage)
{
	// message handler...
	switch (eMessage)
	{
	case IListener::MT_Update:
		// do updating...
		update();
		break;

	case IListener::MT_SetPicture:
		// set picture...
		loadPixmap(m_strPicName = ResourceManager::PicturePath());
		m_pBkPicRect->moveTopLeft(ResourceManager::graphPt());

		break;

	case IListener::MT_GetPicture:
		// get picture...
		ResourceManager::setPicturePath(m_strPicName);
		ResourceManager::setGraphRect(*m_pBkPicRect);
		break;

	}

}


//---------------------------------------------------------------------
void
RenderWidget::paintEvent(QPaintEvent* event)
{
	// below, render all the items including items and background picture...
	QPainter painter(this);

	if (m_pBackgroundPicture != NULL)
	{
		if (!(m_pBackgroundPicture->isNull()))
		{
			painter.drawPixmap(*m_pBkPicRect, *m_pBackgroundPicture);
		}
	}


	ItemManager::getSingleton()->RenderItems(dynamic_cast<QWidget*>(this));

	if (m_bSelectionLocked && ms_eType == ST_ItemDrawLine)
	{
		drawLine();
	}

}


//---------------------------------------------------------------------
void
RenderWidget::mousePressEvent(QMouseEvent* event)
{
	setFocus();
	// record the start point of the cursor.
	*m_pStartPoint = event->pos();

	if (event->button() == Qt::LeftButton)
	{
		if (ms_eType == ST_ItemDrawLine)
		{
			// check the point if it inside the item's rectangle.
			if (ItemManager::getSingleton()->SelectedItem(event->pos()))
			{
				m_bSelectionLocked = true;
			}

			ItemManager::getSingleton()->synchornizePropeties(true);

		}
		else if (ms_eType == ST_ItemTranslation)
		{
			if (ItemManager::getSingleton()->SelectedItem(event->pos()))
			{
				*m_pItemStartPt = ItemManager::getSingleton()->currentItem()->OriginalPos();

				m_bSelectionLocked = true;
			}

			ItemManager::getSingleton()->synchornizePropeties(true);

		}
		else if (ms_eType == ST_ItemSelection)
		{
			ItemManager::getSingleton()->SelectedItem(event->pos());


			ItemManager::getSingleton()->synchornizePropeties(true);

		}
		else if (ms_eType == ST_Create)
		{
			// create a new item.
			Item* pItem = dynamic_cast<Item*>(ItemFactory::GetSingleton()->
				createItem(IT_RECTANGLE));
			pItem->setOriginalPos(event->pos());
			pItem->update();

			// add a new item that we just created to ItemManager.
			ItemManager::getSingleton()->addItem(pItem);

			ItemManager::getSingleton()->synchornizePropeties(true);


		}


	}
	else if (event->button() == Qt::RightButton)
	{
		// temp rubber band rectangle...
		if (!m_pRubberBand)
			m_pRubberBand = new QRubberBand(QRubberBand::Rectangle, this);

		m_pRubberBand->setGeometry(*m_pBkPicRect);
		m_pRubberBand->show();

		*m_pRBStartPt = m_pRubberBand->pos();

		m_bSelectionLocked = true;
	}

}


//---------------------------------------------------------------------
void
RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
	*m_pRBLineEnd = event->pos();

	QPoint deltaPoint = event->pos() - *m_pStartPoint;

	if (m_bSelectionLocked)
	{
		if (m_pRubberBand)
		{
			m_pRubberBand->move(*m_pRBStartPt + deltaPoint);
		}
		else if (ms_eType == ST_ItemTranslation)
		{
			Item* pCurrItem = ItemManager::getSingleton()->currentItem();
			pCurrItem->setOriginalPos(*m_pItemStartPt + deltaPoint);
			pCurrItem->update();
		}

		ItemManager::getSingleton()->synchornizePropeties(true);
	}

	update();
}


//---------------------------------------------------------------------
void
RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_bSelectionLocked)
	{

		if (m_pRubberBand)
		{
			m_pRubberBand->hide();
			m_pBkPicRect->translate(m_pRubberBand->pos() - m_pBkPicRect->topLeft());

			delete m_pRubberBand;
			m_pRubberBand = nullptr;
		}
		else if (ms_eType == ST_ItemDrawLine)
		{
			ItemManager::getSingleton()->currentItem()->setEndPos(event->pos());
			ItemManager::getSingleton()->currentItem()->setSettingLine(true);

			ItemManager::getSingleton()->synchornizePropeties(true);
		}

		m_bSelectionLocked = false;
	}

	update();
}


//---------------------------------------------------------------------
void
RenderWidget::updating()
{
	update();
}


//---------------------------------------------------------------------
void
RenderWidget::loadPixmap(QString strPixmap)
{
	bool bRet = false;

	//load picture from file...
	if (!m_pBackgroundPicture)
	{
		m_pBackgroundPicture = new QPixmap();
	}

	bRet = m_pBackgroundPicture->load(strPixmap);

	if (bRet)
	{
		*m_pBkPicRect = m_pBackgroundPicture->rect();
		m_strPicName = strPixmap;
	}


	update();

}

//---------------------------------------------------------------------








