/*
*recognitionWindow.cpp
*/

#include <qtextcodec.h>
#include <qapplication.h>
#include <qfile.h>
#include <qmenu.h>
#include <qmenubar.h>
#include "../include/recognitionWindow.h"
#include "../include/resourceDef.h"

CRecognitionWindow::CRecognitionWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_pChineseSymbolWidget = NULL;
	InitUi();
	RetranslateInit();
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
}
