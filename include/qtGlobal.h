/*qtGlobal.h
*/

#ifndef __QT_GLOBAL_H__
#define	__QT_GLOBAL_H__

#include <dp.h>
#include <qstring.h>

#ifdef UNICODE
#define	QString2Char(strq, strc)				WcharToChar((LPCTSTR )(strq).utf16(),strc)

#else
#define	QString2Char(strq, strc)				{strc = strq.toUtf8().constData()}

#endif // UNICODE

#endif // !__QT_GLOBAL_H__

