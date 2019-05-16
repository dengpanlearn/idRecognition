/*
*symbolGenWidget.h
*/


#ifndef __SYMBOL_GEN_WIDGET_H__
#define __SYMBOL_GEN_WIDGET_H__

#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qlineedit.h>
#include <QWidget>
#include "chineseSymGenAgent.h"
#include "qtServerTask.h"

class CSymbolGenWidget : public QWidget
{
	Q_OBJECT

public:
	CSymbolGenWidget(QWidget *parent =NULL);
	~CSymbolGenWidget();

private:
	void InitUi();
	void RetranslateUi();

protected:
	virtual void closeEvent(QCloseEvent *closeEvent);

private slots:
	void OnViewOutDir(bool);
	void OnStartGen(bool);
	void OnUpdateProgress(int progress);
	void OnUpdateResult(bool bSuccess);

private:
	QLineEdit*		m_pLineEditOutDir;
	QPushButton*	m_pBtnView;
	QProgressBar*	m_pProgress;
	QPushButton*	m_pBtnGen;
	CChineseSymGenAgent* m_pGenAgent;
	QThread*			m_pServerTask;
};
#endif // !__SYMBOL_GEN_WIDGET_H__