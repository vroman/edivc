#include <assert.h>

#include <qapplication.h>
#include <qplatinumstyle.h>
#include <qmessagebox.h>
#include <export.h>
#include "trazadorwindow.h"


struct _slin {
	int inicio, final;
	int fila1, columna1;
	int fila2, columna2;
} *slin;

int linsize;


void trace(int imem, char* nombreprg, int* lin, void* dbg)
{
	return;
}


void debug(int imem, char* nombreprg, int* lin, void* dbg)
{
	static int dummy=1;
	static char* dummy2="debug";
	int i,j,k,l,c;

	QApplication app(dummy,&dummy2);

    app.setStyle( new QPlatinumStyle );

	TrazadorWindow window;
	app.setMainWidget(&window);
	
	//assert(0);

	if(!window.CargaListado(nombreprg))
		app.quit();

	linsize=lin[0]/sizeof(struct _slin);
	slin=(struct _slin*)&lin[1];

	for(i=0;i<linsize;i++) {
		if(slin[i].inicio<=imem && slin[i].final>=imem) {
			//window.listado->setSelection(slin[i].fila1,slin[i].columna1+1,slin[i].fila2,slin[i].columna2+1);
			j=k=l=c=0;
			if(slin[i].fila1>0)
				for(;j<window.listado->text().length();j++) {
					if(window.listado->text().at(j)=='\n') {
						l++;
						if(l==slin[i].fila1) break;
					}
				}
			window.listado->text().ref(j+slin[i].columna1)='#';

			break;
		}
	}
	window.show();

	app.exec();
}

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{
	void (*traceptr)(int mem, char* nombreprg, int* lin, void* dbg);
	void (*debugptr)(int mem, char* nombreprg, int* lin, void* dbg);
	traceptr=trace;
	debugptr=debug;
	PRIORITY(-512);
	EDIV_Export_Entrypoint(EDIV_trace,traceptr);
	EDIV_Export_Entrypoint(EDIV_debug,debugptr);
	return TRUE;
}
