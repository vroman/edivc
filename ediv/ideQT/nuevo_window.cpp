#include "nuevo_window.h"
#include "ideqtwindow.h"

#include <qapplication.h>
#include <qpixmap.h>
#include <qiconset.h>

#include "gif/b_file.h"

#define pillagif(a) _pillagif(a,sizeof(a))
#define icono(a) QIconSet(*pillagif(a))
/*
QPixmap* _pillagif(unsigned char* datos, int tamano)
{
	QByteArray *bm=new QByteArray(tamano);
	CHECK_PTR(bm);
	bm->assign((char*)datos,tamano);
	QPixmap* ret=new QPixmap(*bm);
	CHECK_PTR(ret);
	return ret;
}*/


nuevo_window::nuevo_window( QWidget* parent, const char* name, WFlags f )
	: QWidget( parent, name, f )
{
	setCaption("Crear nuevo archivo");

	QPixmap *icono=pillagif(b_file_gif);
	setIcon(*icono);



}


