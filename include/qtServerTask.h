/*
* qtServerTask.h
* qt 
*/

#ifndef __QT_SERVER_TASK_H__
#define	__QT_SERVER_TASK_H__



#include <qthread.h>

class CQtServerTask : public QThread
{
	Q_OBJECT
public:
	CQtServerTask(QObject* pParent = NULL);
	virtual ~CQtServerTask();

protected:
	virtual void run();

};

#endif // !__QT_SERVER_TASK_H__
