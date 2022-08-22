//---------------------------------------------------------------------
//	PropertyEditor.h.

//---------------------------------------------------------------------
#pragma once

#include <QTreeView>
#include <QStandardItemModel>
#include <QList>
#include <QHeaderView>


//---------------------------------------------------------------------
//	Const variables for item types.
//---------------------------------------------------------------------
const int NONE = -1;
const int NAME = 1;
const int STARTPT = 2;
const int ENDPT = 3;
const int INTERNALROW = 100;
const int EXTERNALROW = 101;


//---------------------------------------------------------------------
class PropertyEditor : public QTreeView
{
	Q_OBJECT

public:
	PropertyEditor(QWidget* parent = nullptr);
	~PropertyEditor();

public:
	void initlize();
	QString name() const;
	void setName(QString);
	void setStartPt(QPoint);
	void setEndPt(QPoint);

signals:
	void updating();

private slots:
	void itemHandler(QStandardItem* item);

protected:
	QList<QStandardItem*> prepareRow(const QString& first,
		const QString& second, int, bool bSequence = true);

	void drawRow(
		QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	//testing code...
private:
	QString m_pNameText;

};


//---------------------------------------------------------------------





