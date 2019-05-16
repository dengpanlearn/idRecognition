/*
*recognitionWindow.cpp
*/

#include <qtextcodec.h>
#include <qapplication.h>
#include <qfile.h>
#include <qmenu.h>
#include <qmenubar.h>
#include "../include/recognitionWindow.h"
#include "../include/symbolGenWidget.h"
#include "../include/resourceDef.h"
#include "../include/global.h"

CRecognitionWindow::CRecognitionWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_pChineseSymbolWidget = NULL;
	InitUi();
	RetranslateInit();
}

CRecognitionWindow::~CRecognitionWindow()
{
	if (m_pChineseSymbolWidget != NULL)
	{
		delete m_pChineseSymbolWidget;
		m_pChineseSymbolWidget = NULL;
	}
}

void CRecognitionWindow::InitUi()
{
	QTextCodec* pCodec = QTextCodec::codecForLocale();
	m_pActChineseSymbolGen = new QAction(pCodec->toUnicode(RECOGNITION_WINDOW_ACT_CHINESE_SYMBOL_GEN));
	m_pActSymbolTrain = new QAction(pCodec->toUnicode(RECOGNITION_WINDOW_ACT_SYMBOL_TRAIN));
	m_pActIdRecognition = new QAction(pCodec->toUnicode(RECOGNITION_WINDOW_ACT_ID_RECOGNITION));

	QMenuBar* pMenuBar = menuBar();
	QMenu* pMenu = new QMenu(pCodec->toUnicode(RECOGNITION_WINDOW_MENU_FUNC));
	pMenu->addAction(m_pActChineseSymbolGen);
	pMenu->addAction(m_pActSymbolTrain);
	pMenu->addAction(m_pActIdRecognition);
	pMenuBar->addMenu(pMenu);
}

void CRecognitionWindow::RetranslateInit()
{
	QFile qssFile(":/idRecognition/idRecognition.qss");
	if (qssFile.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qssFile.readAll());
		qssFile.close();
	}

	connect(m_pActChineseSymbolGen, SIGNAL(triggered(bool)), this, SLOT(OnSelectChineseGen(bool)));
}

void CRecognitionWindow::closeEvent(QCloseEvent *closeEvent)
{
	if (m_pChineseSymbolWidget != NULL)
	{
		m_pChineseSymbolWidget->close();
	}
	GlobalExit();
}

void CRecognitionWindow::OnSelectChineseGen(bool check)
{

	if (m_pChineseSymbolWidget == NULL)
	{
		m_pChineseSymbolWidget = new CSymbolGenWidget();
	}


	if (this->centralWidget() != NULL)
		this->takeCentralWidget();

	this->setCentralWidget(m_pChineseSymbolWidget);
}
