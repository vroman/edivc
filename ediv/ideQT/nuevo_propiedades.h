#include <qdialog.h>
#include <qlistbox.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qcheckbox.h>

class Nuevo_Propiedades : public QDialog
{
	Q_OBJECT
public:
	Nuevo_Propiedades (int categoria, int tipo, QString fichero, bool enmemoria, QWidget * parent=0, const char * name=0, bool modal=FALSE, WFlags f=0);

protected slots:
	void set_tamano_mapa(int index);
	void Aceptar();
	void Cancelar();

private:
	int _categoria;
	int _tipo;
	QString _fichero;
	bool _enmemoria;

	QLineEdit* edit_nombre;
	QMultiLineEdit* edit_comentarios;
	QCheckBox* directorios;
	QCheckBox* copiar;
	
	QLineEdit* edit_x;
	QLineEdit* edit_y;

};