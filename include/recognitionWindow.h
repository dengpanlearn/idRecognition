/*
*recognitionWindow.h
*/

#ifndef __RECOGNITION_WINDOW_H__
#define __RECOGNITION_WINDOW_H__
 


#include <QtWidgets/QMainWindow>
#include <qaction.h>
class CRecognitionWindow : public QMainWindow
{
	Q_OBJECT

public:
	CRecognitionWindow(QWidget *parent = Q_NULLPTR);

private:
	void InitUi();
	void RetranslateInit();

private slots:
	void OnSelectChineseGen(bool);

private:
	QAction*		m_pActChineseSymbolGen;
	QAction*		m_pActSymbolTrain;
	QAction*		m_pActIdRecognition;
	QWidget*		m_pChineseSymbolWidget;
};
#endif // !__RECOGNITION_WINDOW_H__