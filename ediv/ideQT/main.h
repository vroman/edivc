#ifndef __MAIN_H__
#define __MAIN_H__

#include <qstring.h>
#include <qworkspace.h>
#include "ideqtwindow.h"

#define PROJECTS_FOLDER_NAME "Mis proyectos de eDIV"

#ifdef main_cpp
#	define EXTERN
#else
#	define EXTERN extern
#endif

EXTERN class _main {

public:
	QString personal_folder;
	QString projects_folder;

	typedef struct {
		QString nombre;
		QString directorio;
	} t_proyecto;

	t_proyecto proyecto_actual;
} Main;

EXTERN ideQTWindow* MainWindow;
EXTERN QWorkspace* mdi;

#endif //__MAIN_H__