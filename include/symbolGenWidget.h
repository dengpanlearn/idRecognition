/*
*symbolGenWidget.h
*/


#ifndef __SYMBOL_GEN_WIDGET_H__
#define __SYMBOL_GEN_WIDGET_H__

#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qlineedit.h>
#include <QWidget>

class CSymbolGenWidget : public QWidget
{
	Q_OBJECT

public:
	CSymbolGenWidget(QWidget *parent =NULL);
	~CSymbolGenWidget();

private:
	void InitUi();
	void RetranslateUi();

private:
	QLineEdit*		m_pLineEditOutDir;
	QPushButton*	m_pBtnView;
	QProgressBar*	m_pProgress;
	QPushButton*	m_pBtnGen;
};
#endif // !__SYMBOL_GEN_WIDGET_H__