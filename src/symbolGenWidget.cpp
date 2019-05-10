/*
*symbolGenWidget.cpp
*
*/
#include <qtextcodec.h>
#include <qapplication.h>
#include <qlabel.h>
#include <qfile.h>
#include <qlayout.h>
#include "../include/resourceDef.h"
#include "../include/symbolGenWidget.h"

CSymbolGenWidget::CSymbolGenWidget(QWidget *parent)
	: QWidget(parent)
{
	InitUi();
	RetranslateUi();
}

CSymbolGenWidget::~CSymbolGenWidget()
{
}

void CSymbolGenWidget::InitUi()
{
	setObjectName("Symbol_Gen_Widget");
	QTextCodec* pCodec = QTextCodec::codecForLocale();
	QLabel* pLabOutDir = new QLabel(pCodec->toUnicode(SYMBOL_GEN_WIDGET_LAB_DIR_OUT));
	m_pLineEditOutDir = new QLineEdit();
	m_pBtnView = new QPushButton(pCodec->toUnicode(SYMBOL_GEN_WIDGET_BTN_DIR_OUT));

	QHBoxLayout* pLytDir = new QHBoxLayout();
	pLytDir->addWidget(pLabOutDir);
	pLytDir->addWidget(m_pLineEditOutDir);
	pLytDir->addWidget(m_pBtnView);

	
	m_pProgress = new QProgressBar();
	m_pBtnGen = new QPushButton(pCodec->toUnicode(SYMBOL_GEN_WIDGET_BTN_GENERATOR));
	QHBoxLayout* pLytGen = new QHBoxLayout();
	
	pLytGen->addWidget(m_pProgress);
	pLytGen->addWidget(m_pBtnGen);
	QVBoxLayout* pLytMain = new QVBoxLayout();
	
	pLytMain->addLayout(pLytDir);
	
	pLytMain->addLayout(pLytGen);

	pLytMain->setAlignment(pLytDir, Qt::AlignTop);
	pLytMain->setAlignment(pLytGen, Qt::AlignTop);
	setLayout(pLytMain);
}

void CSymbolGenWidget::RetranslateUi()
{
	QFile qssFile(":/idRecognition/idRecognition.qss");
	if (qssFile.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qssFile.readAll());
		qssFile.close();
	}
}
