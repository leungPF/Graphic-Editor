//---------------------------------------------------------------------
//	mainwindow.h.
//---------------------------------------------------------------------
#pragma once

#include <QMainWindow>


//---------------------------------------------------------------------
//	incomplete classes.
//---------------------------------------------------------------------
class RenderWidget;
class AboutDialog;


//---------------------------------------------------------------------
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	void doInitialization();

private slots:
	void save();
	void typeGroupChanged(QAction* action);
	void openGraph();
	void openFile();
	void newFile();
	void onAbout();

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createDockWindows();


signals:
	void loadPicture(QString);
	void updating();

private:
	RenderWidget* m_pRenderWidget{ nullptr };

	QActionGroup* m_pTypeActionGroup;

	QAction* m_pNewAct;
	QAction* m_pOpenAct;
	QAction* m_pSaveAct;
	QAction* m_pExitAct;
	QAction* m_pAboutAct;
	QAction* m_pItemSltAct;
	QAction* m_pItemCrtAct;
	QAction* m_pItemTranAct;
	QAction* m_pItemDrawLnAct;
	QAction* m_pAddPicture;

	QMenu* m_fileMenu;
	QMenu* m_editMenu;
	QMenu* viewMenu;
	QMenu* helpMenu;
	QToolBar* m_pToolBar;
	AboutDialog* _dialog{ nullptr };

	//---------------------------------------------------------------------

};


//---------------------------------------------------------------------





