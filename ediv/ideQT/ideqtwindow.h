#ifndef __IDEQTWINDOW_H__
#define __IDEQTWINDOW_H__

#include <qmainwindow.h>
#include <qpopupmenu.h>
#include <qworkspace.h>


class QWorkspace;

class ideQTWindow : public QMainWindow
{
	Q_OBJECT
public:
	ideQTWindow( QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );

protected slots:
	void fileNew();
	void ConfirmaSalir();
	void muestra_about();

private slots:
	void MuestraMenuVentana();
	void MenuVentanaActivado( int id );

private:
	QPopupMenu* mnuVentana;
};

#define pillagif(a) _pillagif(a,sizeof(a))
#define icono(a) QIconSet(*pillagif(a))


QPixmap* _pillagif(unsigned char* datos, int tamano);


#endif //__IDEQTWINDOW_H__