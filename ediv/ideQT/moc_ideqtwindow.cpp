/****************************************************************************
** ideQTWindow meta object code from reading C++ file 'ideqtwindow.h'
**
** Created: Fri Jul 12 23:18:31 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_ideQTWindow
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "ideqtwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *ideQTWindow::className() const
{
    return "ideQTWindow";
}

QMetaObject *ideQTWindow::metaObj = 0;

void ideQTWindow::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QMainWindow::className(), "QMainWindow") != 0 )
	badSuperclassWarning("ideQTWindow","QMainWindow");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString ideQTWindow::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("ideQTWindow",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* ideQTWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QMainWindow::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(ideQTWindow::*m1_t0)();
    typedef void(ideQTWindow::*m1_t1)();
    typedef void(ideQTWindow::*m1_t2)();
    typedef void(ideQTWindow::*m1_t3)();
    typedef void(ideQTWindow::*m1_t4)(int);
    m1_t0 v1_0 = Q_AMPERSAND ideQTWindow::fileNew;
    m1_t1 v1_1 = Q_AMPERSAND ideQTWindow::ConfirmaSalir;
    m1_t2 v1_2 = Q_AMPERSAND ideQTWindow::muestra_about;
    m1_t3 v1_3 = Q_AMPERSAND ideQTWindow::MuestraMenuVentana;
    m1_t4 v1_4 = Q_AMPERSAND ideQTWindow::MenuVentanaActivado;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(5);
    slot_tbl[0].name = "fileNew()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "ConfirmaSalir()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "muestra_about()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Protected;
    slot_tbl[3].name = "MuestraMenuVentana()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Private;
    slot_tbl[4].name = "MenuVentanaActivado(int)";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"ideQTWindow", "QMainWindow",
	slot_tbl, 5,
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
