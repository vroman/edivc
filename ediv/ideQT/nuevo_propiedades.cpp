#include "nuevo_propiedades.h"
#include "ideqtwindow.h"
#include "proyecto.h"
#include "main.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qlistbox.h>
#include <qradiobutton.h>
#include <qwhatsthis.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qmessagebox.h>
#include <qvalidator.h>
#include <qdir.h>


Nuevo_Propiedades::Nuevo_Propiedades (int categoria, int tipo, QString fichero, bool enmemoria, QWidget * parent, const char * name, bool modal, WFlags f)
		: QDialog (parent, name, modal, f)
{
	_categoria=categoria;
	_tipo=tipo;
	_fichero=fichero;
	_enmemoria=enmemoria;

	QGridLayout* grid1;
	QLabel* label1;
	QLabel* label2;

	QGroupBox* group1;
	QHBoxLayout* hbox1;
	QListBox* tamanos;
	QButtonGroup* group2;
	QRadioButton* bpp_8;
	QRadioButton* bpp_16;
	QRadioButton* bpp_24;
	QRadioButton* bpp_32;

	QLabel* labelz;
	QCheckBox* checkz;

	int i;
	QString a,b;
	QDir* d;

	int num_tamanos[]= {320,200, 320,240, 320,400, 360,240, 360,360,
		376,282, 400,300, 512,384, 640,400, 640,480,
		800,600, 960,720, 1024,768, 1152,864, 1280,960,
		1280,1024, 1600,1200, 1900,1600};

	QIntValidator* valsize=new QIntValidator(0,65535,this);

	QStringList categorias;
	categorias << "o proyecto" << "o programa" << "o mapa" << "o fichero" << "a paleta" << "a fuente" << "o mundo 3D" << "o fichero musical" << "o fichero de sonido" << "o vídeo";

	setCaption("Propiedades de nuev" + categorias[categoria]);
	setFixedSize(300,200);

	QVBoxLayout* vbox1 = new QVBoxLayout(this);
	vbox1->setMargin(6);
	vbox1->setSpacing(5);

	switch(categoria) {
		case 0:
			grid1=new QGridLayout(this,2,2);
			grid1->setSpacing(5);
			vbox1->addLayout(grid1);

			label1=new QLabel("Nombre del proyecto:",this);
			grid1->addWidget(label1,0,0);

			d=new QDir(fichero);
			i=1;
			while(1) {
				if(d->cd("Proyecto"+QString::number(i))) {
					d->cdUp();
					i++;
				}
				else {
					break;
				}

			}
			edit_nombre=new QLineEdit(this);
			grid1->addWidget(edit_nombre,0,1);
			edit_nombre->setText("Proyecto"+QString::number(i));
			edit_nombre->selectAll();
			edit_nombre->setFocus();
			QWhatsThis::add(edit_nombre,"Introduce aquí el nombre que quieres dar a tu proyecto, aquí típicamente se escribe el título provisional del juego que se quiere hacer :)");

			label2=new QLabel("Comentarios:\n(Autor, copyright, etc.)",this);
			grid1->addWidget(label2,1,0,Qt::AlignTop);

			edit_comentarios=new QMultiLineEdit(this);
			edit_comentarios->setWordWrap(QMultiLineEdit::WidgetWidth);
			grid1->addWidget(edit_comentarios,1,1);
			QWhatsThis::add(edit_comentarios,"Escribe aquí cualquier cosa que creas que deberían saber aquellos que vean este proyecto: tu nombre, tu página web, tu correo electrónico, una descripción del juego...");

			directorios=new QCheckBox("Agrupar los archivos en directorios según su categoría",this);
			vbox1->addWidget(directorios);
			directorios->setChecked(TRUE);
			QWhatsThis::add(directorios,"Activa esta casilla para mantener clasificados los ficheros de tu proyecto en carpetas según sean archivos de mapas, FPG's, sonidos, ficheros de música, etc. Tu carpeta de proyecto estará más ordenada de este modo.");

			copiar=new QCheckBox("Si se añade algún archivo externo, copiarlo a la carpeta del proyecto",this);
			vbox1->addWidget(copiar);
			QWhatsThis::add(copiar,"Si activas esta casilla, al añadir al proyecto un archivo que no esté dentro de la carpeta del proyecto, se copiará en ella. De esta forma podrás localizar rápidamente cualquier archivo que use tu juego con sólo echar un vistazo a la carpeta del proyecto. Sin embargo, puedes dejar desactivada esta opción si quieres ahorrar espacio en disco.");

			break;

		case 2:
			group1=new QGroupBox(0,Qt::Vertical,"Tamaño",this);
			vbox1->addWidget(group1);
					
			hbox1=new QHBoxLayout(group1->layout());
			hbox1->setSpacing(8);

			grid1=new QGridLayout(group1);
			grid1->setSpacing(8);
			hbox1->addLayout(grid1);

			label1=new QLabel("Ancho:",group1);
			grid1->addWidget(label1,0,0);
			edit_x=new QLineEdit("320",group1);
			grid1->addWidget(edit_x,0,1);
			edit_x->setValidator(valsize);
			QWhatsThis::add(edit_x,"Escribe aquí la resolución horizontal que quieres que tenga el nuevo mapa.");

			label2=new QLabel("Alto:",group1);
			grid1->addWidget(label2,1,0);
			edit_y=new QLineEdit("200",group1);
			grid1->addWidget(edit_y,1,1);
			edit_y->setValidator(valsize);
			QWhatsThis::add(edit_y,"Escribe aquí la resolución vertical que quieres que tenga el nuevo mapa.");

			tamanos=new QListBox(group1);
			hbox1->addWidget(tamanos);

			for(i=0;i<sizeof(num_tamanos)/8;i+=1) {
				a.setNum(num_tamanos[i*2]);
				b.setNum(num_tamanos[i*2+1]);
				tamanos->insertItem(a + "x" + b);
			}
			tamanos->setCurrentItem(0);
			connect(tamanos,SIGNAL(highlighted(int)),this,SLOT(set_tamano_mapa(int)));
			QWhatsThis::add(tamanos,"Puedes elegir en esta lista uno de los tamaños predefinidos, o bien escribir el tamaño que desees directamente en las cajas de la izquierda.");

			group2=new QButtonGroup(4,Qt::Vertical,"Profundidad de color",this);
			vbox1->addWidget(group2);
			if(tipo==1 || tipo==2) {
				bpp_8=new QRadioButton("8 bits/pixel (256 colores)",group2);
				bpp_8->setChecked(TRUE);
				QWhatsThis::add(bpp_8,"Los mapas de 8 bits por pixel soportan hasta 256 colores distintos, y usan para ello una paleta de colores indexados.");
			}
			bpp_16=new QRadioButton("16 bits/pixel (65.536 colores)", group2);
			QWhatsThis::add(bpp_16,"Los mapas de 16 bits, llamados de <b>alta densidad</b>, soportan hasta 65.536 colores distintos.");
			if(tipo!=1 && tipo!=2) {
				bpp_16->setChecked(TRUE);
			}
			bpp_24=new QRadioButton("24 bits/pixel (16,7 millones de colores)",group2);
			QWhatsThis::add(bpp_24,"Los mapas de 24 bits, llamados de <b>color verdadero</b>, soportan infinidad de colores. Son los que dan una mayor calidad de imagen.");
			if(tipo==1 || tipo==3) {
				bpp_32=new QRadioButton("32 bits/pixel (24 bits + canal alpha)",group2);
				QWhatsThis::add(bpp_32,"Los mapas de 32 bits son similares a los de 24 bits, con la diferencia de que contienen un <b>canal alpha</b>, o lo que es lo mismo, un canal de transparencia, con el que se puede atribuir un nivel de transparencia a cada pixel individual.");
			}

			break;

		default:
			labelz=new QLabel("No hace falta indicar ninguna propiedad para crear este tipo de archivo. Pulsa <b>Aceptar</b> para continuar.",this);
			labelz->setAlignment(Qt::AlignCenter);
			vbox1->addWidget(labelz);
			checkz=new QCheckBox("No volver a mostrar este mensaje",this);
			vbox1->addWidget(checkz);
	}

	QHBoxLayout* hboxfinal = new QHBoxLayout(this);
	vbox1->addLayout(hboxfinal);
	hboxfinal->setAlignment(Qt::AlignRight|Qt::AlignBottom);

	QSpacerItem* spacerfinal = new QSpacerItem(20,20,QSizePolicy::Expanding);
	hboxfinal->addItem(spacerfinal);

	QPushButton* boton_aceptar = new QPushButton("Aceptar",this);
	boton_aceptar->setDefault(TRUE);
	hboxfinal->addWidget(boton_aceptar);
	QWhatsThis::add(boton_aceptar,"Pulsa este botón para crear un nuev" + categorias[categoria] + " con las características que has seleccionado.");
	connect(boton_aceptar,SIGNAL(clicked()),this,SLOT(Aceptar()));

	QPushButton* boton_cancelar = new QPushButton("Cancelar",this);
	boton_cancelar->setAccel(Qt::Key_Escape);
	hboxfinal->addWidget(boton_cancelar);
	QWhatsThis::add(boton_cancelar,"Pulsa este botón para cerrar esta ventana sin crear un nuevo archivo.");
	connect(boton_cancelar,SIGNAL(clicked()),this,SLOT(Cancelar()));

}

void Nuevo_Propiedades::set_tamano_mapa(int index)
{
	int num_tamanos[]= {320,200, 320,240, 320,400, 360,240, 360,360,
	376,282, 400,300, 512,384, 640,400, 640,480,
	800,600, 960,720, 1024,768, 1152,864, 1280,960,
	1280,1024, 1600,1200, 1900,1600};

	edit_x->setText(QString::number(num_tamanos[index*2]));
	edit_y->setText(QString::number(num_tamanos[index*2+1]));

}

void Nuevo_Propiedades::Aceptar()
{
	Proyecto* proj;
	QDir* d;

	switch(_categoria) {
		case 0:
			if(edit_nombre->text().length()==0) {
				QMessageBox mb("Error","Tienes que escribir un nombre para el proyecto.",QMessageBox::Warning,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
				mb.exec();
				edit_nombre->setFocus();
				return;
			}

			if(edit_nombre->text().left(8)=="Proyecto" && edit_nombre->text().at(8).isDigit()) {
				QMessageBox mb("Aviso","El nombre que has introducido ("+edit_nombre->text()+") es poco descriptivo.\n¿Seguro que deseas mantener este nombre?",QMessageBox::NoIcon,QMessageBox::Yes,QMessageBox::No|QMessageBox::Default,QMessageBox::NoButton);
				mb.setButtonText(QMessageBox::Yes,"Sí");
				//QCheckBox* mbcheck=new QCheckBox("No mostrar este mensaje de nuevo",&mb);
				if(mb.exec()==QMessageBox::No) {
					edit_nombre->setFocus();
					return;
				}
			}

			d=new QDir(_fichero);
			if(!d->mkdir(edit_nombre->text())) {
				if(d->cd(edit_nombre->text())) {
					QMessageBox mb("Error","Ya existe una carpeta de proyecto con ese nombre. Introduce un nombre distinto.",QMessageBox::Warning,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
					mb.exec();
				} else {
					QMessageBox mb("Error","Error al crear la carpeta del proyecto.\nEs posible que hayas introducido un nombre con caracteres incorrectos.",QMessageBox::Warning,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
					mb.exec();
				}
				edit_nombre->setFocus();
				return;
			}
			d->cd(edit_nombre->text());
			//_fichero.prepend(d->canonicalPath()+"/");
			//_fichero.append("/"+edit_nombre->text()+".epj");
			_fichero=d->canonicalPath()+"/"+edit_nombre->text()+".epj";
			proj=new Proyecto(edit_nombre->text(),_fichero,edit_comentarios->text(),directorios->isChecked(),copiar->isChecked(),mdi);					
			proj->show();
			if(!_enmemoria) proj->save();


	}

	delete this;
}

void Nuevo_Propiedades::Cancelar()
{
	delete this;
}

