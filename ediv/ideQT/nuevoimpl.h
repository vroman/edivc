#ifndef NUEVO_H
#define NUEVO_H

#include <qwidget.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qdialog.h>

class Nuevo : public QDialog
{ 
    Q_OBJECT

public:
    Nuevo( QWidget* parent = 0, const char* name = 0, bool modal=TRUE, WFlags fl = 0, QWidget* padre = 0 );

protected slots:
	void ListaClick(int index);
	void Aceptar();
	void Cancelar();
	void Examinar();
	void combo_tipo_selecciona(int index);

private:
	QLabel* descripcion;
	QGroupBox* box_opciones;
	QLabel* label1;
	QComboBox* combo_tipo;
	QLabel* label2;
	QLineEdit* edit_ruta;
	QPushButton* boton_examinar;
	QPushButton* boton_aceptar;

	int _categoria;
	int _tipo;

	void set_descripcion(int categoria, int tipo);

};

#endif // NUEVO_H
