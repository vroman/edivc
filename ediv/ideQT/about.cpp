#include "about.h"
#include "ideqtwindow.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qtextview.h>

#include "gif/about.h"
#include "gif/about_png.h"
#include "gif/sion.h"

About::About( QWidget* parent,  const char* name, WFlags fl, QWidget* padre )
    : QWidget( parent, name, fl )
{
	setCaption("Acerca de eDIV");
	setFixedSize(450,300);

	QPixmap *icono=pillagif(about_gif);
	setIcon(*icono);

	if(padre)
		move(padre->x()+padre->width()/2-width()/2,padre->y()+padre->height()/2-height()/2);

	QVBoxLayout* vbox1 = new QVBoxLayout(this);

	QPixmap* imagen=pillagif(about_png);

	QLabel* imagenlabel = new QLabel(this);
	imagenlabel->setPixmap(*imagen);
	imagenlabel->setAlignment(Qt::AlignCenter);
	vbox1->addWidget(imagenlabel);

	QHBoxLayout* hbox1 = new QHBoxLayout(this);
	vbox1->addLayout(hbox1);
	hbox1->setSpacing(10);
	hbox1->setMargin(15);

	imagen=pillagif(sion_png);
	QLabel* imagen2label = new QLabel(this);
	imagen2label->setPixmap(*imagen);
	imagen2label->setAlignment(Qt::AlignCenter);
	hbox1->addWidget(imagen2label);

	QTextView* texto = new QTextView(this);
	texto->setText("<div align=\"center\">eDIV ha llegado a tus manos gracias a Sion, Ltd.<br><br>"
		"<u>Programadores</u><br><br>"
		"Er_Makina<br>"
		"Riseven<br>"
		"|RooT|<br>"
		"Daijo<br><br>"
		"<u>Traductores</u><br><br>"
		"CicTec (italiano)<br>"
		"Tareko (portugués)<br>"
		"Er_Makina (inglés)<br>"
		"BenKo (catalán)<br>"
		"Jorge Urreta (euskera)<br>"
		"Bb_LaRiOs (francés)<br><br>"
		"<u>Betatesters</u><br><br>"
		"Matías Iacono<br>"
		"EHSN<br>"
		"chinodiv<br>"
		"[hallg]<br>"
		"loxo<br>"
		"djpipo<br>"
		"Tomás Baenas<br>"
		"Pato<br>"
		"Jags Andrew<br>"
		"BenKo<br>"
		"Ferminho<br>"
		"Bb_LaRiOs<br>"
		"CicTec<br>"
		"Satv<br>"
		"Joyal<br>"
		"SHeLe<br>"
		"fidojones<br>"
		"Paradigma Studios<br><br>"
		"<u>Webmasters</u><br><br>"
		"Er_Makina<br>"
		"[Kony]<br><br>"
		"<u>Diseño del logo</u><br><br>"
		"Donan<br><br>"
		"<u>Agradecimientos</u><br><br>"
		"Daniel Navarro<br>"
		"José Luis Cebrián<br>"
		"Beorn<br>"
		"Slàinte<br>"
		"Equipo de Hammer Technologies<br>(excepto Wario)<br></div>");
	hbox1->addWidget(texto);

}