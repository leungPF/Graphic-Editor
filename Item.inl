
inline
void
Item::setText(QString strText)
{
	m_strText = strText;
}


//---------------------------------------------------------------------
inline
QPoint
Item::OriginalPos()
const
{
	return m_fromPos;
}


//---------------------------------------------------------------------
inline
QPoint
Item::EndPos()
const
{
	return m_toPos;
}


//---------------------------------------------------------------------
inline
QString
Item::Text()
const
{
	return m_strText;
}


//---------------------------------------------------------------------
inline
void
Item::setOriginalPos(const QPoint pos)
{
	m_fromPos = pos;
}


//---------------------------------------------------------------------
inline
void
Item::setEndPos(const QPoint pos)
{
	m_toPos = pos;
}


//---------------------------------------------------------------------
inline
QRect
Item::rect()
const
{
	return m_rect;
}


//---------------------------------------------------------------------
inline
bool
Item::settingLine()
const
{
	return m_bSetLine;
}


//---------------------------------------------------------------------






#pragma once
