//---------------------------------------------------------------------
//	PropertyEditor.cpp.

//---------------------------------------------------------------------
#include "propertyeditor.h"
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QApplication>
#include "ItemManager.h"


//---------------------------------------------------------------------
PropertyEditor::PropertyEditor(QWidget* parent)
	: QTreeView(parent)
{
	//
}


//---------------------------------------------------------------------
PropertyEditor::~PropertyEditor()
{
	//
}


//---------------------------------------------------------------------
void
PropertyEditor::initlize()
{
	// create a QStandardItemModel to QTreeView.
	QStandardItemModel* standardModel = new QStandardItemModel(0, 2, this);

	// setup headers.
	standardModel->setHeaderData(0, Qt::Horizontal, tr("Property"));
	standardModel->setHeaderData(1, Qt::Horizontal, tr("Value"));

	QList<QStandardItem*> internalRow = prepareRow("Internal", "",
		INTERNALROW, false);
	QList<QStandardItem*> externalRow = prepareRow("External", "",
		EXTERNALROW, false);

	QStandardItem* item = standardModel->invisibleRootItem();

	// adding rows to the invisible root item produces a root element
	item->appendRow(internalRow);
	item->appendRow(externalRow);

	QList<QStandardItem*> nameRow = prepareRow("Name", "***", NAME);
	QList<QStandardItem*> startPtRow = prepareRow("StartPoint", "***", STARTPT);
	QList<QStandardItem*> endPtRow = prepareRow("EndPoint", "***", ENDPT);

	// adding a row to an item starts a subtree
	internalRow.first()->appendRow(nameRow);

	externalRow.first()->appendRow(startPtRow);
	externalRow.first()->appendRow(endPtRow);

	// set model to view...
	setModel(standardModel);
	expandAll();
	setColumnWidth(0, 120);
	setAlternatingRowColors(true);
	setAnimated(true);

	connect(
		standardModel,
		SIGNAL(itemChanged(QStandardItem*)),
		this,
		SLOT(itemHandler(QStandardItem*)));

}


//---------------------------------------------------------------------
QString
PropertyEditor::name()
const
{
	return m_pNameText;
}


//---------------------------------------------------------------------
void
PropertyEditor::setName(QString strName)
{
	QModelIndex indexInternal = dynamic_cast<QStandardItemModel*>(
		model())->index(0, 0, QModelIndex());

	QModelIndex indexNameValue = dynamic_cast<QStandardItemModel*>(
		model())->index(0, 1, indexInternal);

	QStandardItem* itemNameValue = dynamic_cast<QStandardItemModel*>(
		model())->itemFromIndex(indexNameValue);

	itemNameValue->setText(strName);

}


//---------------------------------------------------------------------
void
PropertyEditor::setStartPt(QPoint startPt)
{
	QString strStartPt = QString(tr("(%1, %2)")).arg(startPt.x()).arg(startPt.y());

	QModelIndex indexExternal = dynamic_cast<QStandardItemModel*>(
		model())->index(1, 0, QModelIndex());

	QModelIndex indexStartPtValue = dynamic_cast<QStandardItemModel*>(
		model())->index(0, 1, indexExternal);

	QStandardItem* itemStartPtValue = dynamic_cast<QStandardItemModel*>(
		model())->itemFromIndex(indexStartPtValue);

	itemStartPtValue->setText(strStartPt);

}


//---------------------------------------------------------------------
void
PropertyEditor::setEndPt(QPoint endPt)
{
	QString strEndtPt = QString(tr("(%1, %2)")).arg(endPt.x()).arg(endPt.y());

	QModelIndex indexExternal = dynamic_cast<QStandardItemModel*>(
		model())->index(1, 0, QModelIndex());

	QModelIndex indexEndPtValue = dynamic_cast<QStandardItemModel*>(
		model())->index(1, 1, indexExternal);

	QStandardItem* itemEndPtValue = dynamic_cast<QStandardItemModel*>(
		model())->itemFromIndex(indexEndPtValue);

	itemEndPtValue->setText(strEndtPt);
}


//---------------------------------------------------------------------
void
PropertyEditor::itemHandler(QStandardItem* item)
{
	// synchronize the data of items... 
	int iType = item->data().toInt();

	switch (iType)
	{
	case NAME:

		m_pNameText = item->text();

		break;

	case STARTPT:
		break;

	case ENDPT:
		break;

	}

	ItemManager::getSingleton()->synchornizePropeties(false);

}


//---------------------------------------------------------------------
QList<QStandardItem*>
PropertyEditor::prepareRow(
	const QString& first,
	const QString& second,
	int nType,
	bool bSequence /*= true*/)
{
	// the value of bSequence is true is first item or 
	// false is the second item...

	QList<QStandardItem*> rowItems;

	QStandardItem* pTempFirstItem = new QStandardItem(first);
	pTempFirstItem->setEditable(false);

	QStandardItem* pTempSecondItem = new QStandardItem(second);


	if (bSequence)
	{
		pTempSecondItem->setData(nType);
	}
	else
	{
		pTempFirstItem->setData(nType);
	}


	rowItems << pTempFirstItem;
	rowItems << pTempSecondItem;

	return rowItems;
}


//---------------------------------------------------------------------
void
PropertyEditor::drawRow(
	QPainter* painter,
	const QStyleOptionViewItem& option,
	const QModelIndex& index)
	const
{
	QStyleOptionViewItem opt = option;

	QStandardItem* item = dynamic_cast<QStandardItemModel*>(
		model())->itemFromIndex(index);

	QColor c = QColor(234, 191, 255);

	// color the different groups...
	if (int i = item->data().toInt() == EXTERNALROW)
	{
		c = QColor(199, 255, 255);
	}
	else if (item->parent())
	{
		if (item->parent()->data() == EXTERNALROW)
		{
			c = QColor(199, 255, 255);
		}
	}

	// draw the background of row...
	if (c.isValid()) {
		painter->fillRect(option.rect, c);
		opt.palette.setColor(QPalette::AlternateBase, c.lighter(112));
	}

	QTreeView::drawRow(painter, opt, index);

	// draw the frame line...
	QColor color = static_cast<QRgb>(QApplication::style()->styleHint(
		QStyle::SH_Table_GridLineColor, &opt));

	painter->save();

	painter->setPen(QPen(color));
	painter->drawLine(opt.rect.x(), opt.rect.bottom(), opt.rect.right(),
		opt.rect.bottom());

	painter->restore();

}


//---------------------------------------------------------------------





