#include <stdio.h>
#include <stdlib.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qmultilineedit.h>
#include <qmessagebox.h>
#include <qfont.h>

#include "trazadorwindow.h"


TrazadorWindow::TrazadorWindow( QWidget* parent, const char* name, WFlags f )
	: QMainWindow( parent, name, f )
{
	buffer_listado=0;

	setCaption("eDIV - Trazador de programas");

	QBoxLayout *vlayout1 = new QVBoxLayout(this,2);
		QGridLayout *glayout1 = new QGridLayout(vlayout1,0,0,10);
			//vlayout1->addLayout(glayout1);
			QLabel *labNumProcesos = new QLabel(this);
			glayout1->addWidget(labNumProcesos,0,0);
			labNumProcesos->setText("0/0 Procesos");
			QLabel *labIdProceso = new QLabel(this);
			glayout1->addWidget(labIdProceso,0,1);
			labIdProceso->setText("Id Proceso: 0 Normal");

			QListView *listProcesos = new QListView(this);
			listProcesos->addColumn("Proceso");
			listProcesos->addColumn("Id");
			listProcesos->addColumn("Estado");
			listProcesos->addColumn("Frame");
			glayout1->addWidget(listProcesos,1,0);
			
			QBoxLayout *vlayout2 = new QVBoxLayout(this,3);
				glayout1->addLayout(vlayout2,1,1);
				QLabel *labNombreProceso = new QLabel(this);
				vlayout2->addWidget(labNombreProceso,0);
				labNombreProceso->setText("NombreProceso");
				labNombreProceso->setFrameShape(QFrame::Panel);
				labNombreProceso->setFrameShadow(QFrame::Sunken);

				QBoxLayout *hlayout1 = new QHBoxLayout(this,3);
					vlayout2->addLayout(hlayout1,1);
					QLabel *labSprite = new QLabel(this);
					hlayout1->addWidget(labSprite,0);
					labSprite->setBackgroundColor(Qt::black);
					labSprite->setMinimumSize(128,128);
					labSprite->setMaximumSize(128,128);
					QLayoutItem *spacer1 = new QSpacerItem(50,128,QSizePolicy::Fixed,QSizePolicy::Fixed);
					hlayout1->addItem(spacer1);

					QBoxLayout *vlayout3 = new QVBoxLayout(this,2);
						hlayout1->addLayout(vlayout3,2);
						QPushButton *btnVerDatos = new QPushButton("Ver datos",this);
						vlayout3->addWidget(btnVerDatos,0);
						btnVerDatos->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
						QLabel *labInfoProceso = new QLabel(this);
						vlayout3->addWidget(labInfoProceso,1);
						labInfoProceso->setText("x=0\ny=0\npantalla\nnormal");
						labInfoProceso->setAlignment(Qt::AlignCenter);

				QBoxLayout *hlayout2 = new QHBoxLayout(this,3);
					vlayout2->addLayout(hlayout2,2);
					//QLayoutItem *spacer2 = new QSpacerItem(2,2,QSizePolicy::Maximum,QSizePolicy::Preferred);
					//hlayout2->addItem(spacer2);
					QPushButton *btnPerfiles = new QPushButton("Perfiles",this);
					hlayout2->addWidget(btnPerfiles,0);
					btnPerfiles->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
					QPushButton *btnEjecProceso = new QPushButton("Ejecutar Proceso",this);
					hlayout2->addWidget(btnEjecProceso,1);
					btnEjecProceso->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
					QPushButton *btnSigFrame = new QPushButton("Siguiente Frame",this);
					hlayout2->addWidget(btnSigFrame,2);
					btnSigFrame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
		
		QBoxLayout *hlayout3 = new QHBoxLayout(this,2);
			vlayout1->addLayout(hlayout3,1);
			
			QBoxLayout *vlayout4 = new QVBoxLayout(this,8);
				hlayout3->addLayout(vlayout4,0);
				QLabel *labIdProceso2 = new QLabel(this);
				vlayout4->addWidget(labIdProceso2,0);
				labIdProceso2->setText("0");
				labIdProceso2->setFrameShape(QFrame::Panel);
				labIdProceso2->setFrameShadow(QFrame::Sunken);
				QLayoutItem *spacer3 = new QSpacerItem(2,10,QSizePolicy::Fixed,QSizePolicy::Fixed);
				vlayout4->addItem(spacer3);
				QPushButton *btnIrA = new QPushButton("Ir a...",this);
				vlayout4->addWidget(btnIrA);
				QPushButton *btnRuptura = new QPushButton("Ruptura",this);
				vlayout4->addWidget(btnRuptura);
				QPushButton *btnAqui = new QPushButton("¡Aquí!",this);
				vlayout4->addWidget(btnAqui);
				QPushButton *btnTrazar = new QPushButton("Trazar",this);
				vlayout4->addWidget(btnTrazar);
				QPushButton *btnPaso = new QPushButton("Paso",this);
				vlayout4->addWidget(btnPaso);
				QLayoutItem *spacer4 = new QSpacerItem(2,10,QSizePolicy::Fixed,QSizePolicy::Expanding);
				vlayout4->addItem(spacer4);

			listado = new QTextView(this);
			hlayout3->addWidget(listado,1);
			listado->setFont(QFont("courier",10));
			//listado->setReadOnly(TRUE);

}

TrazadorWindow::~TrazadorWindow()
{
	if(buffer_listado) free(buffer_listado);
}

int TrazadorWindow::CargaListado(char* archivo)
{
	FILE* f;
	int tam;

	if((f=fopen(archivo,"r"))==NULL) {
		QString mensaje="No se encuentra ";
		mensaje += archivo;
		QMessageBox *mb = new QMessageBox("Error",mensaje,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
		mb->exec();
		return 0;
	}

	fseek(f,0,SEEK_END);
	tam=ftell(f);
	fseek(f,0,SEEK_SET);

	if((buffer_listado=(char*)malloc(tam))==NULL) {
		fclose(f);
		QMessageBox *mb = new QMessageBox("Error","Memoria insuficiente",QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
		mb->exec();
		return 0;
	}

	fread(buffer_listado,1,tam,f);
	fclose(f);

	listado->setText(buffer_listado);
	
	return tam;
}
