/****************************************************************************
** KDEAniMenu meta object code from reading C++ file 'kdestyle.h'
**
** Created: Wed Oct 9 23:31:04 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_KDEAniMenu
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 8
#elif Q_MOC_OUTPUT_REVISION != 8
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "kdestyle.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *KDEAniMenu::className() const
{
    return "KDEAniMenu";
}

QMetaObject *KDEAniMenu::metaObj = 0;

void KDEAniMenu::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QObject::className(), "QObject") != 0 )
	badSuperclassWarning("KDEAniMenu","QObject");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString KDEAniMenu::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("KDEAniMenu",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* KDEAniMenu::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QObject::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(KDEAniMenu::*m1_t0)();
    typedef void(KDEAniMenu::*m1_t1)();
    m1_t0 v1_0 = Q_AMPERSAND KDEAniMenu::slotDestroyFake;
    m1_t1 v1_1 = Q_AMPERSAND KDEAniMenu::slotFinished;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "slotDestroyFake()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl_access[0] = QMetaData::Protected;
    slot_tbl[1].name = "slotFinished()";
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl_access[1] = QMetaData::Protected;
    metaObj = QMetaObject::new_metaobject(
	"KDEAniMenu", "QObject",
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


const char *KDEStyle::className() const
{
    return "KDEStyle";
}

QMetaObject *KDEStyle::metaObj = 0;

void KDEStyle::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QPlatinumStyle::className(), "QPlatinumStyle") != 0 )
	badSuperclassWarning("KDEStyle","QPlatinumStyle");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION
QString KDEStyle::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("KDEStyle",s);
}

#endif // QT_NO_TRANSLATION
QMetaObject* KDEStyle::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QPlatinumStyle::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"KDEStyle", "QPlatinumStyle",
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
