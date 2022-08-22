//---------------------------------------------------------------------
//		Headers.
//---------------------------------------------------------------------
#include "Item.h"
#include <QPen>
#include <QFontMetrics>
#include <QtGlobal>


//---------------------------------------------------------------------
Item::Item() : m_bSetLine(false)
{
	//initlize position.
	m_fromPos.setX(0);
	m_fromPos.setY(0);
	m_toPos.setX(0);
	m_toPos.setY(0);

	m_rect.setTopLeft(m_fromPos);
	m_rect.setHeight(32);
	m_rect.setWidth(40);

	// create a new font for drawing item's text.
	QFont font("Calibri", 12);
	font.setBold(true);
	m_font = font;

}


//---------------------------------------------------------------------
void
Item::update()
{
	QRect rect;

	if (m_strText != "")
	{
		rect = calculateBoundingBox(m_strText);
	}

	int iWidth = rect.width();

	m_rect.setTopLeft(QPoint(m_fromPos.x() - qMax(40, iWidth + 12) / 2,
		m_fromPos.y() - 16));
	m_rect.setHeight(32);
	m_rect.setWidth(qMax(40, iWidth + 12));

}


//---------------------------------------------------------------------
void
Item::setSettingLine(bool bSet)
{
	m_bSetLine = bSet;
}


//---------------------------------------------------------------------
QRect
Item::calculateBoundingBox(QString strText)
{
	//calculate the bounding box of text of item.
	QFontMetrics fontMetrics(m_font);
	QRect rect = fontMetrics.boundingRect(strText);

	return rect;
}


//---------------------------------------------------------------------
//		Class: Rectangle.
//---------------------------------------------------------------------
RectangleItem::RectangleItem()
{
	//none.
}


//---------------------------------------------------------------------
void
RectangleItem::render(
	QWidget* pWidget,
	bool bSelected)
{
	// create a QPainter to draw on the client area...
	QPainter painter(pWidget);
	painter.setRenderHint(QPainter::Antialiasing, true);


	painter.save();

	// draw selected color with alpha of 128...
	if (bSelected)
	{
		QBrush brush(QColor(255, 0, 0, 128));
		painter.setBrush(brush);
		painter.setPen(Qt::NoPen);
		painter.drawRoundedRect(m_rect, 3.0, 3.0);
	}

	QPen pen;
	pen.setWidth(2);
	pen.setColor(Qt::black);
	painter.setPen(pen);

	// draw item's line if the flag was checked...
	if (m_bSetLine)
	{
		painter.drawLine(m_rect.center(), m_toPos);
	}

	// restore the painter and draw normal item...
	painter.restore();

	painter.save();

	pen.setColor(Qt::black);
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(4);

	//QBrush brush(Qt::green, Qt::Dense7Pattern);
	QRadialGradient haloGradient(m_rect.center().x(), m_rect.center().y(),
		40, m_rect.center().x(), m_rect.center().y());

	haloGradient.setColorAt(0.0, QColor(81, 168, 255, 90));
	haloGradient.setColorAt(0.8, QColor(232, 246, 128, 120));
	haloGradient.setColorAt(0.9, QColor(255, 192, 162, 180));
	haloGradient.setColorAt(1.0, QColor(177, 255, 100, 200));

	painter.setPen(pen);
	painter.setBrush(haloGradient);

	//painter.drawRoundRect(m_rect, 18, 18);
	painter.drawRoundedRect(m_rect, 3.0, 3.0);

	painter.restore();

	drawText(painter);


}

//---------------------------------------------------------------------
void
RectangleItem::drawText(QPainter& painter)
{
	// draw item's text.
	painter.setFont(m_font);
	painter.drawText(m_rect, Qt::AlignCenter, m_strText);
}


//---------------------------------------------------------------------


