//---------------------------------------------------------------------
//	Item.h.

//---------------------------------------------------------------------
#ifndef _ITEM_H
#define _ITEM_H

#pragma once


//---------------------------------------------------------------------
#include <QtCore/QString>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>



//---------------------------------------------------------------------
class Item
{
public:
	explicit Item();
	inline virtual void setText(QString);
	inline virtual QString Text() const;
	inline virtual void setOriginalPos(const QPoint pos);
	inline virtual void setEndPos(const QPoint pos);
	inline virtual QPoint OriginalPos() const;
	virtual QPoint EndPos() const;
	inline QRect rect() const;
	virtual void setSettingLine(bool bSet = true);
	bool settingLine() const;
	virtual QRect calculateBoundingBox(QString);
	virtual void update();
	virtual void render(QWidget*, bool bSelected = false) = 0;

protected:
	QString m_strText;
	QPoint m_fromPos;
	QPoint m_toPos;
	QRect m_rect;
	QFont m_font;
	bool m_bSetLine;
};


//---------------------------------------------------------------------
//		class for Rectangle Item...
//---------------------------------------------------------------------
class RectangleItem : public Item
{
public:
	RectangleItem();
	void drawText(QPainter& painter);
	virtual void render(QWidget*, bool bSelected = false);

};


//---------------------------------------------------------------------


//---------------------------------------------------------------------
//		including inline files...
//---------------------------------------------------------------------
#include "Item.inl"

//---------------------------------------------------------------------
#endif

//---------------------------------------------------------------------


