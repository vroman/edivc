/****************************************************************************
** Nuevo_Propiedades meta object code from reading C++ file 'nuevo_propiedades.h'
**
** Created: Sun Jul 14 23:22:00 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Nuevo_Propiedades
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "nuevo_propiedades.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Nuevo_Propiedades::className() const
{
    return "Nuevo_Propiedades";
}

QMetaObject *Nuevo_Propiedades::metaObj = 0;

void Nuevo_Propiedades::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("Nuevo_Propiedades","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString Nuevo_Propiedades::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Nuevo_Propiedades",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* Nuevo_Propiedades::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(Nuevo_Propiedades::*m1_t0)(int);
    typedef void(Nuevo_Propiedades::*m1_t1)();
    typedef void(Nuevo_Propiedades::*m1_t2)();
    m1_t0 v1_0 = Q_AMPERSAND Nuevo_Propiedades::set_tamano_mapa;
    m1_t1 v1_1 = Q_AMPERSAND Nuevo_Propiedades::Aceptar;
    m1_t2 v1_2 = Q_AMPERSAND Nuevo_Propiedades::Cancelar;
    QMetaData *slot_tbl = QMetaObject::new_metadata(3);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(3);
    slot_tbl[0].name = "set_tamano_mapa(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "Aceptar()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "Cancelar()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"Nuevo_Propiedades", "QDialog",
	slot_tbl, 3,
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
