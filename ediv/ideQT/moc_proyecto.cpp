/****************************************************************************
** Proyecto meta object code from reading C++ file 'proyecto.h'
**
** Created: Wed Oct 9 23:31:05 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Proyecto
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "proyecto.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Proyecto::className() const
{
    return "Proyecto";
}

QMetaObject *Proyecto::metaObj = 0;

void Proyecto::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("Proyecto","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString Proyecto::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Proyecto",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* Proyecto::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(Proyecto::*m1_t0)();
    typedef void(Proyecto::*m1_t1)(QWidget*);
    m1_t0 v1_0 = Q_AMPERSAND Proyecto::save;
    m1_t1 v1_1 = Q_AMPERSAND Proyecto::updateProyectoActual;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "save()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "updateProyectoActual(QWidget*)";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"Proyecto", "QWidget",
	slot_tbl, 2,
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
