#include <qlayout.h>
#include <qpushbutton.h>
#include <qhbuttongroup.h>
#include <qtooltip.h>
#include <qlistview.h>
#include <qheader.h>
#include <qtoolbar.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qdir.h>

#include "main.h"
#include "proyecto.h"
#include "ideqtwindow.h"

#include "gif/proyecto.h"
#include "gif/prop.h"
#include "gif/bycategory.h"
#include "gif/byextension.h"
#include "gif/sortasc.h"
#include "gif/no.h"
/*#include "gif/panelize.h"
#include "gif/unpanelize.h"*/


Proyecto::Proyecto(QString nombre, QString fichero, QString descripcion, bool directorios, bool copiar, QWidget* parent)
	: QWidget(parent)
{
	_nombre=nombre;
	_fichero=fichero;
	_descripcion=descripcion;
	_directorios=directorios;
	_copiar=copiar;

	setCaption("Proyecto: "+nombre);
	QPixmap* icono=pillagif(proyecto_gif);
	setIcon(*icono);

	QVBoxLayout* vbox1=new QVBoxLayout(this);

	QHBoxLayout* hbox1=new QHBoxLayout(this);
	vbox1->addLayout(hbox1);
	hbox1->setMargin(3);
	hbox1->setSpacing(6);

	//QToolBar *toolbar=new QToolBar(caption(),(QMainWindow*)this,QMainWindow::Top,TRUE);

	
	QPushButton* tbProp=new QPushButton(*pillagif(prop_gif),QString::null,this);
	//tbProp->setFlat(TRUE);
	tbProp->setFocusPolicy(QWidget::NoFocus);
	tbProp->setFixedSize(24,24);
	hbox1->addWidget(tbProp);
	QToolTip::add(tbProp,"Propiedades del proyecto");

	QFrame* sep1=new QFrame(this);
	sep1->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	sep1->setLineWidth(1);
	sep1->setMidLineWidth(0);
	hbox1->addWidget(sep1);

	QButtonGroup* grupo=new QButtonGroup(3,Qt::Horizontal,this);
	grupo->setFrameStyle(QFrame::NoFrame);
	grupo->setMargin(0);
	grupo->setLineWidth(0);
	grupo->setMidLineWidth(0);
	grupo->setExclusive(TRUE);
	grupo->layout()->setMargin(0);

/*	QHBoxLayout* grupo=new QHBoxLayout(this);
	hbox1->addLayout(grupo);*/

	QPushButton* tbCategorias=new QPushButton(*pillagif(bycategory_gif),QString::null,grupo);
	tbCategorias->setFixedSize(24,24);
	tbCategorias->setToggleButton(TRUE);
	tbCategorias->setOn(TRUE);
	tbCategorias->setFocusPolicy(QWidget::NoFocus);
	QToolTip::add(tbCategorias,"Agrupar por categoría");
//	grupo->insert(tbCategorias);
//	grupo->addWidget(tbCategorias);

	QPushButton* tbExtensiones=new QPushButton(*pillagif(byextension_gif),QString::null,grupo);
	tbExtensiones->setFixedSize(24,24);
	tbExtensiones->setToggleButton(TRUE);
	tbExtensiones->setFocusPolicy(QWidget::NoFocus);
	QToolTip::add(tbExtensiones,"Agrupar por extensión");
//	grupo->insert(tbExtensiones);
//	grupo->addWidget(tbExtensiones);

	QPushButton* tbNombre=new QPushButton(*pillagif(sortasc_gif),QString::null,grupo);
	tbNombre->setFixedSize(24,24);
	tbNombre->setToggleButton(TRUE);
	tbNombre->setFocusPolicy(QWidget::NoFocus);
	QToolTip::add(tbNombre,"Mostrar desagrupados, por nombre");
//	grupo->insert(tbNombre);
//	grupo->addWidget(tbNombre);

	hbox1->addWidget(grupo);

	QFrame* sep2=new QFrame(this);
	sep2->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	sep2->setLineWidth(1);
	sep2->setMidLineWidth(0);
	hbox1->addWidget(sep2);

	QPushButton* tbQuitar=new QPushButton(*pillagif(no_gif),QString::null,this);
	//tbQuitar->setFlat(TRUE);
	tbQuitar->setFocusPolicy(QWidget::NoFocus);
	tbQuitar->setFixedSize(24,24);
	hbox1->addWidget(tbQuitar);
	QToolTip::add(tbQuitar,"Excluir del proyecto");

	QSpacerItem* spacer1=new QSpacerItem(20,20,QSizePolicy::Expanding);
	hbox1->addItem(spacer1);

/*	tbPanelize=new QPushButton(*pillagif(panelize_gif),QString::null,this);
	tbPanelize->setFocusPolicy(QWidget::NoFocus);
	tbPanelize->setFixedSize(24,24);
	tbPanelize->setToggleButton(TRUE);
	hbox1->addWidget(tbPanelize);
	QToolTip::add(tbPanelize,"Acoplar ventana");
	connect(tbPanelize,SIGNAL(toggled(bool)),this,SLOT(panelizar(bool)));
*/
	QListView* vista=new QListView(this);
	vbox1->addWidget(vista);
	vista->addColumn("Lista de archivos");
	vista->setColumnWidthMode(0,QListView::Maximum);
    //vista->header()->setClickEnabled( FALSE, vista->header()->count() - 1 );
	vista->header()->hide();
	vista->setSelectionMode(QListView::Extended);
	vista->setRootIsDecorated(TRUE);
	//QListViewItem* a=new QListViewItem(vista,"Hola");

	statusbar=new QStatusBar(this);
	vbox1->addWidget(statusbar);
	statusbar->message("0 archivo(s) - 0 bytes");

	xmldoc=new QDomDocument("eDIV_project");

	QDomComment c=xmldoc->createComment(" eDIV Project 0.1 ");
	xmldoc->appendChild(c);

	xmlproject=xmldoc->createElement("project");
	xmlproject.setAttribute("version","0.1");
	xmlproject.setAttribute("name",nombre);
	xmlproject.setAttribute("copyfiles",copiar);
	xmlproject.setAttribute("subdirs",directorios);
	xmldoc->appendChild(xmlproject);

	QDomElement xmldescriptionelement=xmldoc->createElement("description");
	xmldescription=xmldoc->createTextNode(descripcion);
	xmldescriptionelement.appendChild(xmldescription);
	xmlproject.appendChild(xmldescriptionelement);

	xmlfilelist=xmldoc->createElement("filelist");
	xmlproject.appendChild(xmlfilelist);

	//connect(this,SIGNAL(focusInEvent()),this,SLOT(updateProyectoActual()));
	//focusInEvent(NULL);
	connect(mdi,SIGNAL(windowActivated(QWidget*)),this,SLOT(updateProyectoActual(QWidget*)));

}


/*void Proyecto::panelizar(bool panelizado)
{
	if(panelizado) {
		tbPanelize->setIconSet(*pillagif(unpanelize_gif));
		//toolbar=new QToolBar("a",MainWindow,QMainWindow::Left);
		reparent(MainWindow,0,QPoint(0,0),TRUE);
	}
	else {
		tbPanelize->setIconSet(*pillagif(panelize_gif));
		reparent(mdi,0,QPoint(0,0),TRUE);
		//delete toolbar;
	}

}*/


void Proyecto::save()
{
	QFile f(_fichero);
	while(1) {
		if(f.open(IO_WriteOnly)) {
			QTextStream (&f) << "<?xml version=\"1.0\" encoding=\"iso8859-1\"?>" << xmldoc->toString();
			f.close();
			break;
		}
		else {
			QMessageBox msg("Error","Error al escribir \""+_fichero+"\".\nComprueba que tienes permiso de escritura sobre el archivo y que hay suficiente espacio libre en el disco.",QMessageBox::Warning,QMessageBox::Abort,QMessageBox::Retry,QMessageBox::Ignore);
			msg.setButtonText(QMessageBox::Abort,"Abortar");
			msg.setButtonText(QMessageBox::Retry,"Reintentar");
			msg.setButtonText(QMessageBox::Ignore,"Ignorar");
			int ret=msg.exec();
			if(ret==QMessageBox::Abort) {
				delete this;
				break;
			}
			else if(ret==QMessageBox::Ignore) {
				break;
			}
		}
	}
}


void Proyecto::updateProyectoActual(QWidget* w)
{
	if(w==this) {
		QFileInfo f(_fichero);
		Main.proyecto_actual.nombre=_nombre;
		Main.proyecto_actual.directorio=f.dirPath();

#ifdef _DEBUG
		qDebug("proyecto_actual.nombre=\"%s\"\n",Main.proyecto_actual.nombre.latin1());
		qDebug("proyecto_actual.directorio=\"%s\"\n",Main.proyecto_actual.directorio.latin1());
#endif
	}
}
