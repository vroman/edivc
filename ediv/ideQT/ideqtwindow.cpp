#include "ideqtwindow.h"

#include <qapplication.h>
#include <qworkspace.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qaction.h>
//#include <qplatinumstyle.h>
#include "kdestyle.h"
#include <qmessagebox.h>
#include <qiconset.h>

#include "gif/ediv_icon.h"
#include "gif/b_file.h"
#include "gif/b_open.h"
#include "gif/b_disk.h"
#include "gif/prop.h"
#include "gif/print.h"
#include "gif/dooropen.h"
#include "gif/b_person.h"
#include "gif/cdaudio.h"
#include "gif/calculat.h"
#include "gif/mail.h"
#include "gif/b_twist.h"
#include "gif/vcrplay.h"
#include "gif/gears.h"
#include "gif/vcrfsfor.h"
#include "gif/tools.h"
#include "gif/library.h"
#include "gif/sound.h"
#include "gif/comppc1.h"
#include "gif/helpindx.h"
#include "gif/help.h"
#include "gif/bookopen.h"
#include "gif/emini.h"

#include "main.h"
#include "nuevoimpl.h"
#include "about.h"

QPixmap* _pillagif(unsigned char* datos, int tamano)
{
	QByteArray *bm=new QByteArray(tamano);
	CHECK_PTR(bm);
	bm->assign((char*)datos,tamano);
	QPixmap* ret=new QPixmap(*bm);
	CHECK_PTR(ret);
	return ret;
}


ideQTWindow::ideQTWindow( QWidget* parent, const char* name, WFlags f )
	: QMainWindow( parent, name, f )
{
	setCaption("eDIV");

	QPixmap *iconoe=pillagif(ediv_icon);
	setIcon(*iconoe);

//    qApp->setStyle( new QPlatinumStyle );
    qApp->setStyle( new KDEStyle );

	mdi = new QWorkspace( this, "workspace" );
	mdi->setBackgroundMode( PaletteDark );

    QPopupMenu *mnuArchivo = new QPopupMenu( this );
	CHECK_PTR(mnuArchivo);
	mnuArchivo->insertTearOffHandle();
	mnuArchivo->insertItem(icono(b_file_gif), "&Nuevo...", this, SLOT(fileNew()), CTRL+Key_N );	
	mnuArchivo->insertItem(icono(b_open_gif), "&Abrir...", this, 0, CTRL+Key_O);
	mnuArchivo->insertItem("&Cerrar",this,0);
	mnuArchivo->insertItem(icono(b_disk_gif), "&Guardar", this, 0, CTRL+Key_S );
	mnuArchivo->insertItem("Guardar c&omo...",this,0);
	mnuArchivo->insertSeparator();
	mnuArchivo->insertItem(icono(prop_gif), "&Propiedades...",this,0);
	mnuArchivo->insertItem("Con&figurar impresora...",this,0);
	mnuArchivo->insertItem(icono(print_gif), "&Imprimir",this,0);
	mnuArchivo->insertSeparator();
	mnuArchivo->insertItem(icono(dooropen_gif), "&Salir",this,SLOT(ConfirmaSalir()),CTRL+Key_Q);

	QPopupMenu *mnuHerramientas = new QPopupMenu(this);
	CHECK_PTR(mnuHerramientas);
	mnuHerramientas->insertTearOffHandle();
	mnuHerramientas->insertItem("&Fusionador de paletas",this,0);
	mnuHerramientas->insertItem("&Generador de explosiones",this,0);
	mnuHerramientas->insertItem(icono(b_person_gif), "Generador de &sprites",this,0);
	mnuHerramientas->insertItem("Generador de &texturas",this,0);
	mnuHerramientas->insertItem(icono(cdaudio_gif), "&CDiv Player",this,0);
	mnuHerramientas->insertItem(icono(calculat_gif), "C&alculadora",this,0);
	mnuHerramientas->insertSeparator();
	mnuHerramientas->insertItem("&Navegador web",this,0);
	mnuHerramientas->insertItem(icono(mail_gif), "E-&Mail",this,0);
	mnuHerramientas->insertItem(icono(b_twist_gif), "eDIV &EnchufingNET",this,0);
	mnuHerramientas->insertSeparator();
	mnuHerramientas->insertItem("¡Estoy enc&hufad@!",this,0);
	mnuHerramientas->setCheckable(TRUE);

	QPopupMenu *mnuCompilar = new QPopupMenu(this);
	CHECK_PTR(mnuCompilar);
	mnuCompilar->insertTearOffHandle();
	mnuCompilar->insertItem(icono(vcrplay_gif),"&Ejecutar",this,0,Key_F10);
	mnuCompilar->insertItem(icono(gears_gif),"&Compilar",this,0,Key_F11);
	mnuCompilar->insertItem(icono(vcrfsfor_gif),"&Trazar programa",this,0,Key_F12);
	mnuCompilar->insertItem("Crear &distribución...",this,0);

	QPopupMenu *mnuSistema = new QPopupMenu(this);
	CHECK_PTR(mnuSistema);
	mnuSistema->insertTearOffHandle();
	mnuSistema->insertItem(icono(tools_gif),"&Preferencias...",this,0);
	mnuSistema->insertItem(icono(library_gif),"&Gestor de librerías",this,0);
	mnuSistema->insertItem(icono(sound_gif),"&Ajustar volumen...",this,0);
	mnuSistema->insertItem(icono(comppc1_gif),"&Información del sistema",this,0);

	mnuVentana = new QPopupMenu(this);
	CHECK_PTR(mnuVentana);
    //mnuVentana->setCheckable( TRUE );
    connect( mnuVentana, SIGNAL( aboutToShow() ),
	     this, SLOT( MuestraMenuVentana() ) );

	QPopupMenu *mnuAyuda = new QPopupMenu(this);
	CHECK_PTR(mnuAyuda);
	mnuAyuda->insertTearOffHandle();
	mnuAyuda->insertItem(icono(helpindx_gif),"&Contenido",this,0,CTRL+Key_H);
	mnuAyuda->insertItem("&Buscar ayuda sobre...",this,0,CTRL+SHIFT+Key_H);
	mnuAyuda->insertItem(icono(help_gif),"&Ayuda sensible",this,0,Key_F1);
	mnuAyuda->insertItem(icono(bookopen_gif),"B&iblioteca eTK",this,0);
	mnuAyuda->insertSeparator();
	mnuAyuda->insertItem("&Sion Entertainment en la Red",this,0);
	mnuAyuda->insertItem("&eDivCentral.com",this,0);
	mnuAyuda->insertSeparator();
	mnuAyuda->insertItem(icono(emini_gif),"Acerca &de...",this,SLOT(muestra_about()));


	//QToolBar *menutb=new QToolBar(this);
	//CHECK_PTR(menutb);

	QMenuBar *menu=new QMenuBar(this);
	CHECK_PTR(menu);
    menu->insertItem("&Archivo", mnuArchivo);
	menu->insertItem("&Herramientas", mnuHerramientas);
	menu->insertItem("&Compilar",mnuCompilar);
	menu->insertItem("&Sistema",mnuSistema);
	menu->insertItem("&Ventana",mnuVentana);
	menu->insertItem("A&yuda",mnuAyuda);

	QToolBar *maintb=new QToolBar("principal",this,QMainWindow::Top,TRUE);
	CHECK_PTR(maintb);
	
	QToolButton *tbNuevo=new QToolButton (*pillagif(b_file_gif), "Nuevo", QString::null, this, SLOT(fileNew()), maintb);
	
	QToolButton *tbAbrir=new QToolButton (*pillagif(b_open_gif), "Abrir", QString::null, this, 0, maintb);
	QToolButton *tbGuardar=new QToolButton (*pillagif(b_disk_gif), "Guardar", QString::null, this, 0, maintb);
	maintb->addSeparator();
	QToolButton *tbProp=new QToolButton (*pillagif(prop_gif), "Propiedades", QString::null, this, 0, maintb);
	QToolButton *tbImprimir=new QToolButton (*pillagif(print_gif), "Imprimir", QString::null, this, 0, maintb);

	QToolBar *compilartb=new QToolBar(this);
	CHECK_PTR(compilartb);

	QToolButton *tbEjecutar=new QToolButton (*pillagif(vcrplay_gif), "Ejecutar", QString::null, this, 0, compilartb);
	QToolButton *tbTrazar=new QToolButton (*pillagif(vcrfsfor_gif), "Trazar programa", QString::null, this, 0, compilartb);
	QToolButton *tbCompilar=new QToolButton (*pillagif(gears_gif), "Compilar", QString::null, this, 0, compilartb);

	QToolBar *ayudatb=new QToolBar(this);
	CHECK_PTR(ayudatb);

	QToolButton *tbContenido=new QToolButton (*pillagif(helpindx_gif), "Temas de ayuda", QString::null, this, 0, ayudatb);
	QToolButton *tbAyuda=new QToolButton (*pillagif(help_gif), "Ayuda sensible", QString::null, this, 0, ayudatb);

	setCentralWidget( mdi );

}


void ideQTWindow::fileNew()
{
	// Use your own documentclass

	/*QMessageBox("a","a",QMessageBox::NoIcon,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
	QWidget *doc = new QWidget( mdi, "$MSDEVPROJECTNAME document" );
	doc->setCaption( tr("Document%1").arg( mdi->windowList().count()+1 ) );
	doc->show();*/
	//QWidget *dialognew = new Nuevo(0,0,TRUE);
	//dialognew->show();
	QWidget *dialognew = new Nuevo(0,0,TRUE,0,this);
	dialognew->show();
}


void ideQTWindow::ConfirmaSalir()
{
    QMessageBox mb( "Salir de eDIV",
                    "¿Seguro que deseas salir?",
					QMessageBox::NoIcon,
                    QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No | QMessageBox::Escape,
					QMessageBox::NoButton);
	mb.setButtonText(QMessageBox::Yes,"Sí");
    if ( mb.exec() == QMessageBox::Yes ) qApp->quit();
}


void ideQTWindow::MuestraMenuVentana()
{
    mnuVentana->clear();
    int cascadeId = mnuVentana->insertItem("&Cascada", mdi, SLOT(cascade() ) );
    int tileId = mnuVentana->insertItem("&Mosaico", mdi, SLOT(tile() ) );
    if ( mdi->windowList().isEmpty() ) {
	mnuVentana->setItemEnabled( cascadeId, FALSE );
	mnuVentana->setItemEnabled( tileId, FALSE );
    }
    mnuVentana->insertSeparator();
    QWidgetList windows = mdi->windowList();
    for ( int i = 0; i < int(windows.count()); ++i ) {
	int id = mnuVentana->insertItem(windows.at(i)->caption(),
					 this, SLOT( MenuVentanaActivado( int ) ) );
	mnuVentana->setItemParameter( id, i );
	mnuVentana->setItemChecked( id, mdi->activeWindow() == windows.at(i) );
    }
}


void ideQTWindow::MenuVentanaActivado( int id )
{
    QWidget* w = mdi->windowList().at( id );
    if ( w ) {
	w->showNormal();
	w->setFocus();
    }
}

void ideQTWindow::muestra_about()
{
	QWidget *dialogabout = new About(0,0,WType_Modal,this);
	dialogabout->show();
}
