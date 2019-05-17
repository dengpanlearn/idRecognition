/*
*symbolGenWidget.cpp
*
*/
#include <qtextcodec.h>
#include <qapplication.h>
#include <qlabel.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qlayout.h>
#include "../include/resourceDef.h"
#include "../include/symbolGenWidget.h"

CSymbolGenWidget::CSymbolGenWidget(QWidget *parent)
	: QWidget(parent)
{
	m_pGenAgent = NULL;
	m_pServerTask = NULL;
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
	m_pProgress->setVisible(false);
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

	m_pServerTask = new CQtServerTask();
}

void CSymbolGenWidget::RetranslateUi()
{
	QFile qssFile(":/idRecognition/idRecognition.qss");
	if (qssFile.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(qssFile.readAll());
		qssFile.close();
	}

	m_pGenAgent = new CChineseSymGenAgent(NULL, m_pServerTask);
	m_pGenAgent->moveToThread(m_pServerTask);
	m_pGenAgent->Init();
	m_pServerTask->start();

	connect(m_pBtnView, SIGNAL(clicked(bool)), this, SLOT(OnViewOutDir(bool)));
	connect(m_pBtnGen, SIGNAL(clicked(bool)), this, SLOT(OnStartGen(bool)));
	connect(m_pGenAgent, SIGNAL(SignalWorkProgress(int)), this, SLOT(OnUpdateProgress(int)));
	connect(m_pGenAgent, SIGNAL(SignalWorkResult(bool)), this, SLOT(OnUpdateResult(bool)));
}

void CSymbolGenWidget::closeEvent(QCloseEvent *closeEvent)
{
	if (m_pGenAgent != NULL)
	{
		m_pGenAgent->SyncExit(5000);
	}
}

void CSymbolGenWidget::OnViewOutDir(bool check)
{
	QString defaultDir = m_pLineEditOutDir->text();
	if (defaultDir.isEmpty())
	{
		defaultDir = QDir::currentPath();
	}

	QTextCodec* pCodec = QTextCodec::codecForLocale();
	QString curDir = QFileDialog::getExistingDirectory(this, pCodec->toUnicode(SYMBOL_GEN_WIDGET_DIR_OUT_FILEDIALGO), defaultDir);
	if (!curDir.isEmpty())
	{
		m_pLineEditOutDir->setText(curDir);
	}
}

void CSymbolGenWidget::OnStartGen(bool check)
{
	QString outDir = m_pLineEditOutDir->text();
	if (outDir.isEmpty())
	{
		QTextCodec* pTextCodec = QTextCodec::codecForLocale();
		QMessageBox msgBox;

		msgBox.setWindowTitle(pTextCodec->toUnicode("提示"));
		msgBox.setText(pTextCodec->toUnicode("请选择正确路径"));

		msgBox.setIcon(QMessageBox::Information);
		msgBox.addButton(QMessageBox::Ok);
		msgBox.setStyleSheet(
			"QPushButton {"
			"border: 1px solid #555;"
			"padding: 4px;"
			"min-width: 65px;"
			"min-height: 40px;"
			"border - radius:5px;"
			"font-size: 20px;"
			"text-align:center;"
			"}"
			"QPushButton:hover {"
			" background-color: #999;"
			"}"
			"QPushButton:pressed {"
			"background-color: #333;"
			"border-color: #555;"
			"color: beige;"
			"}"

			"QLabel{ min-width: 100px;min-height:50px;font:15px;}"
		);

		msgBox.exec();

		return;
	}


	if ((m_pGenAgent == NULL) || !m_pGenAgent->StartWork(&outDir))
	{
		QTextCodec* pTextCodec = QTextCodec::codecForLocale();
		QMessageBox msgBox;

		msgBox.setWindowTitle(pTextCodec->toUnicode("提示"));
		msgBox.setText(pTextCodec->toUnicode("中文字符生存失败"));

		msgBox.setIcon(QMessageBox::Information);
		msgBox.addButton(QMessageBox::Ok);
		msgBox.setStyleSheet(
			"QPushButton {"
			"border: 1px solid #555;"
			"padding: 4px;"
			"min-width: 65px;"
			"min-height: 40px;"
			"border - radius:5px;"
			"font-size: 20px;"
			"text-align:center;"
			"}"
			"QPushButton:hover {"
			" background-color: #999;"
			"}"
			"QPushButton:pressed {"
			"background-color: #333;"
			"border-color: #555;"
			"color: beige;"
			"}"

			"QLabel{ min-width: 100px;min-height:50px;font:15px;}"
		);

		msgBox.exec();

		return;
	}

	
	m_pProgress->setVisible(true);
	m_pProgress->setValue(0);
	m_pProgress->setMaximum(100);
	m_pBtnGen->setEnabled(false);
	m_pBtnView->setEnabled(false);
}

void CSymbolGenWidget::OnUpdateProgress(int progress)
{
	int lastVaule = m_pProgress->value();
	if (lastVaule != progress)
		m_pProgress->setValue(progress);
}

void CSymbolGenWidget::OnUpdateResult(bool bSuccess)
{
	QTextCodec* pTextCodec = QTextCodec::codecForLocale();
	QMessageBox msgBox;

	msgBox.setWindowTitle(pTextCodec->toUnicode("提示"));
	if (bSuccess)
		msgBox.setText(pTextCodec->toUnicode("中文字符生成完成"));
	else
		msgBox.setText(pTextCodec->toUnicode("中文字符生成失败"));


	msgBox.setIcon(QMessageBox::Information);
	msgBox.addButton(QMessageBox::Ok);
	msgBox.setStyleSheet(
		"QPushButton {"
		"border: 1px solid #555;"
		"padding: 4px;"
		"min-width: 65px;"
		"min-height: 40px;"
		"border - radius:5px;"
		"font-size: 20px;"
		"text-align:center;"
		"}"
		"QPushButton:hover {"
		" background-color: #999;"
		"}"
		"QPushButton:pressed {"
		"background-color: #333;"
		"border-color: #555;"
		"color: beige;"
		"}"

		"QLabel{ min-width: 100px;min-height:50px;font:15px;}"
	);

	msgBox.exec();
	m_pBtnView->setEnabled(true);
	m_pBtnGen->setEnabled(true);
	m_pProgress->setVisible(false);
}