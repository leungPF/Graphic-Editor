//---------------------------------------------------------------------
//	mainwindow.cpp.
//---------------------------------------------------------------------
#include "mainwindow.h"
#include "renderwidget.h"
#include <QPainter>
#include <QActionGroup>
#include <QMenuBar>
#include <QtoolBar>
#include <QFileDialog>
#include <QDockWidget>
#include "propertyeditor.h"
#include "ItemManager.h"
#include "ResourceManager.h"
#include "AboutDialog.h"


//---------------------------------------------------------------------
MainWindow::MainWindow(void) : QMainWindow(nullptr)
{
	// make a bitmap for icon drawing.
	QPixmap pm(40, 40);
	QPainter pmp(&pm);
	pmp.fillRect(0, 0, 20, 20, Qt::lightGray);
	pmp.fillRect(20, 20, 20, 20, Qt::lightGray);
	pmp.fillRect(0, 20, 20, 20, Qt::darkGray);
	pmp.fillRect(20, 0, 20, 20, Qt::darkGray);
	pmp.end();

	setWindowIcon(pm);
}

//---------------------------------------------------------------------
void
MainWindow::doInitialization()
{
	//do some initialization, such as create center window etc.
	m_pRenderWidget = new RenderWidget(this);
	setCentralWidget(m_pRenderWidget);

	createActions();
	createMenus();
	createToolBars();
	createDockWindows();

	setWindowTitle(tr("GraphicEditor"));
	connect(this, SIGNAL(loadPicture(QString)), m_pRenderWidget, SLOT(loadPixmap(QString)));

}


//---------------------------------------------------------------------
void
MainWindow::typeGroupChanged(QAction* action)
{
	if (action == m_pItemSltAct)
	{
		RenderWidget::ms_eType = ST_ItemSelection;
	}
	else if (action == m_pItemCrtAct)
	{
		RenderWidget::ms_eType = ST_Create;
	}
	else if (action == m_pItemDrawLnAct)
	{
		RenderWidget::ms_eType = ST_ItemDrawLine;
	}
	else if (action == m_pItemTranAct)
	{
		RenderWidget::ms_eType = ST_ItemTranslation;
	}

}


//---------------------------------------------------------------------
void
MainWindow::openGraph()
{
	// open a OpenFileName box to get a file name.
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open Image"),
		".",
		tr("Image Files (*.png *.jpg *.bmp)"));

	QDir dirfrom;
	QFileInfo fileInfo(fileName);
	fileName = dirfrom.relativeFilePath(fileInfo.absoluteFilePath());

	// emit a loadPicture signal to listeners.
	emit loadPicture(fileName);

}


//---------------------------------------------------------------------
void
MainWindow::openFile()
{
	// open a OpenFileName box to get a file name.
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Open Resource"),
		".",
		tr("Image Files (*.gepro)"));

	if (fileName != "")
	{
		ResourceManager::readDataFromFile(fileName);
	}

}


//---------------------------------------------------------------------
void
MainWindow::newFile()
{
	ItemManager::getSingleton()->deleteAllItems();

	ItemManager::getSingleton()->synchornizePropeties(true);
	m_pRenderWidget->detachGraph();
	m_pRenderWidget->update();
}


//---------------------------------------------------------------------
void
MainWindow::onAbout()
{
	// show about dialog.
	if (!_dialog)
		_dialog = new AboutDialog(this);
	_dialog->show();
}


//---------------------------------------------------------------------
void
MainWindow::createActions()
{
	//create some actions...
	m_pNewAct = new QAction(QIcon(":/img/new.png"), tr("&New"), this);
	m_pNewAct->setShortcuts(QKeySequence::New);
	m_pNewAct->setStatusTip(tr("Create a new scene"));

	m_pOpenAct = new QAction(QIcon(":/img/open.png"), tr("&Open..."), this);
	m_pOpenAct->setShortcuts(QKeySequence::Open);
	m_pOpenAct->setStatusTip(tr("Open a new scene"));

	m_pExitAct = new QAction(QIcon(":/img/exit.png"), tr("&Quit"), this);
	m_pExitAct->setShortcuts(QKeySequence::Quit);
	m_pExitAct->setStatusTip(tr("Quit"));

	m_pAboutAct = new QAction(QIcon(":/img/about.jpg"), tr("&About..."), this);
	m_pAboutAct->setStatusTip(tr("About"));

	m_pAddPicture = new QAction(QIcon(":/img/AddPicture.png"), tr("&AddPicture..."), this);
	m_pAddPicture->setStatusTip(tr("Add a new Picture"));

	m_pSaveAct = new QAction(QIcon(":/img/save.png"), tr("&Save..."), this);
	m_pSaveAct->setShortcuts(QKeySequence::Save);
	m_pSaveAct->setStatusTip(tr("Save the current form letter"));

	m_pItemSltAct = new QAction(QIcon(":/img/Selection.png"), tr("SelectItem"), this);
	m_pItemSltAct->setStatusTip(tr("Select items..."));

	m_pItemTranAct = new QAction(QIcon(":/img/Translation.png"), tr("Translate"), this);
	m_pItemTranAct->setStatusTip(tr("Translate items..."));

	m_pItemCrtAct = new QAction(QIcon(":/img/NewItem.png"), tr("CreateItem"), this);
	m_pItemCrtAct->setStatusTip(tr("Create a Item..."));

	m_pItemDrawLnAct = new QAction(QIcon(":/img/DrawLine.png"), tr("DrawLine"), this);
	m_pItemDrawLnAct->setStatusTip(tr("Draw a line between item and picture..."));

	m_pTypeActionGroup = new QActionGroup(this);
	m_pTypeActionGroup->addAction(m_pItemSltAct);
	m_pTypeActionGroup->addAction(m_pItemCrtAct);
	m_pTypeActionGroup->addAction(m_pItemTranAct);
	m_pTypeActionGroup->addAction(m_pItemDrawLnAct);
	m_pItemCrtAct->setCheckable(true);
	m_pItemSltAct->setCheckable(true);
	m_pItemTranAct->setCheckable(true);
	m_pItemDrawLnAct->setCheckable(true);
	m_pItemCrtAct->setChecked(true);

	// action group action signal and slot...
	connect(m_pTypeActionGroup, SIGNAL(triggered(QAction*)), this,
		SLOT(typeGroupChanged(QAction*)));

	// actions' signals connecting...
	connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(save()));
	connect(m_pAddPicture, SIGNAL(triggered()), this, SLOT(openGraph()));
	connect(m_pOpenAct, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(m_pNewAct, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(m_pExitAct, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_pAboutAct, SIGNAL(triggered()), this, SLOT(onAbout()));

}


//---------------------------------------------------------------------
void
MainWindow::createMenus()
{
	// make a menu bar.
	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_fileMenu->addAction(m_pNewAct);
	m_fileMenu->addAction(m_pOpenAct);
	m_fileMenu->addAction(m_pSaveAct);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_pExitAct);

	m_editMenu = menuBar()->addMenu(tr("&Edit"));
	m_editMenu->addAction(m_pAddPicture);

	m_editMenu = menuBar()->addMenu(tr("&About"));
	m_editMenu->addAction(m_pAboutAct);
}


//---------------------------------------------------------------------
void
MainWindow::createToolBars()
{
	// create a toolbar and add some actions...
	m_pToolBar = new QToolBar(tr("ToolBar"));

	addToolBar(Qt::LeftToolBarArea, m_pToolBar);

	m_pToolBar->addAction(m_pNewAct);
	m_pToolBar->addAction(m_pSaveAct);
	m_pToolBar->addAction(m_pOpenAct);
	m_pToolBar->addSeparator();
	m_pToolBar->addSeparator();
	m_pToolBar->addAction(m_pAddPicture);
	m_pToolBar->addSeparator();
	m_pToolBar->addSeparator();
	m_pToolBar->addAction(m_pItemSltAct);
	m_pToolBar->addAction(m_pItemDrawLnAct);
	m_pToolBar->addAction(m_pItemTranAct);
	m_pToolBar->addAction(m_pItemCrtAct);
	m_pToolBar->addSeparator();
	m_pToolBar->addSeparator();

}


//---------------------------------------------------------------------
void
MainWindow::createStatusBar()
{
}


//---------------------------------------------------------------------
void
MainWindow::createDockWindows()
{
	// create dockable window...
	QDockWidget* dock = new QDockWidget(tr("Property Editor"), this);
	PropertyEditor* pProEditorWidget = new PropertyEditor(dock);
	pProEditorWidget->initlize();

	dock->setWidget(pProEditorWidget);

	addDockWidget(Qt::RightDockWidgetArea, dock);

	ItemManager::getSingleton()->setPropertyEditor(pProEditorWidget);

}


//---------------------------------------------------------------------
void
MainWindow::save()
{
	// save the current scene.
	QString fileName = QFileDialog::getSaveFileName(
		this,
		tr("Choose a file name"),
		".",
		tr("XML (*.gepro)"));

	if (fileName.isEmpty())
		return;

	ResourceManager::writeDataToFile(fileName);

}


//---------------------------------------------------------------------
MainWindow::~MainWindow()
{
	setCentralWidget(nullptr);

	delete m_pRenderWidget;
	m_pRenderWidget = nullptr;

	if (_dialog)
	{
		delete _dialog;
		_dialog = nullptr;
	}


}


//---------------------------------------------------------------------





 