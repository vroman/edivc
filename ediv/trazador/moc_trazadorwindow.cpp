/****************************************************************************
** TrazadorWindow meta object code from reading C++ file 'trazadorwindow.h'
**
** Created: Sat Feb 23 19:15:50 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_TrazadorWindow
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "trazadorwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *TrazadorWindow::className() const
{
    return "TrazadorWindow";
}

QMetaObject *TrazadorWindow::metaObj = 0;

void TrazadorWindow::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QMainWindow::className(), "QMainWindow") != 0 )
	badSuperclassWarning("TrazadorWindow","QMainWindow");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString TrazadorWindow::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("TrazadorWindow",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* TrazadorWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QMainWindow::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"TrazadorWindow", "QMainWindow",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
