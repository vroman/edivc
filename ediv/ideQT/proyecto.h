#include <qwidget.h>
//#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qdom.h>

class Proyecto : public QWidget
{
	Q_OBJECT

public:
	Proyecto(QString nombre, QString fichero, QString descripcion, bool directorios, bool copiar, QWidget* parent);

public slots:
	void save();

protected slots:
//	void panelizar(bool panelizado);
	void updateProyectoActual(QWidget* w);

protected:
	QString _nombre;
	QString _fichero;
	QString _descripcion;
	bool _directorios;
	bool _copiar;

private:
/*	QPushButton* tbPanelize;
	QToolBar* toolbar;*/
	QStatusBar* statusbar;

	QDomDocument* xmldoc;
	QDomElement xmlproject;
	QDomText xmldescription;
	QDomElement xmlfilelist;
};
