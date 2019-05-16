/*
* chineseSymGenTask.cpp
*
*/

#include "../include/qtGlobal.h"
#include "../include/qtServerTask.h"


CQtServerTask::CQtServerTask(QObject* pParent):QThread(pParent)
{
}


CQtServerTask::~CQtServerTask()
{

}

void CQtServerTask::run()
{
	exec();
}