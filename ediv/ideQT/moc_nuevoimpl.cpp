/****************************************************************************
** Nuevo meta object code from reading C++ file 'nuevoimpl.h'
**
** Created: Wed Jul 10 01:54:25 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Nuevo
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "nuevoimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Nuevo::className() const
{
    return "Nuevo";
}

QMetaObject *Nuevo::metaObj = 0;

void Nuevo::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("Nuevo","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString Nuevo::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Nuevo",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* Nuevo::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(Nuevo::*m1_t0)(int);
    typedef void(Nuevo::*m1_t1)();
    typedef void(Nuevo::*m1_t2)();
    typedef void(Nuevo::*m1_t3)();
    typedef void(Nuevo::*m1_t4)(int);
    m1_t0 v1_0 = Q_AMPERSAND Nuevo::ListaClick;
    m1_t1 v1_1 = Q_AMPERSAND Nuevo::Aceptar;
    m1_t2 v1_2 = Q_AMPERSAND Nuevo::Cancelar;
    m1_t3 v1_3 = Q_AMPERSAND Nuevo::Examinar;
    m1_t4 v1_4 = Q_AMPERSAND Nuevo::combo_tipo_selecciona;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(5);
    slot_tbl[0].name = "ListaClick(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "Aceptar()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    slot_tbl[2].name = "Cancelar()";
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl_access[2] = QMetaData::Protected;
    slot_tbl[3].name = "Examinar()";
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl_access[3] = QMetaData::Protected;
    slot_tbl[4].name = "combo_tipo_selecciona(int)";
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl_access[4] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"Nuevo", "QDialog",
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
