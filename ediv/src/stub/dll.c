/*
 * eDiv Compiler
 * Copyleft (C) 2000-2002 Sion Entertainment
 * http://www.sion-e.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#define DEBUG_DLL
#ifdef _WIN32
	#include "dll_load.h"
	#include <io.h>
#else
	#include <dlfcn.h>
	#include <dirent.h>
	#include <sys/stat.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dll.h"
#include "extern.h"

/*int imem_temp;
int iloc_temp;*/

int carga_dll(char* nombre)
{
#ifdef _WIN32
	TYPEOF_ExportaFuncs *ExportaFuncs;
	HINSTANCE hDLL=NULL;
//	TYPEOF_extfunc *extfunc; // *** PRUEBA ***
	char resolved_path[_MAX_PATH];

	static char fichdll[256];
	strcpy(fichdll,nombre);
	strcat(fichdll,".dll");

	// Carga la DLL
	//hDLL=LoadDLL(fichdll);
	_fullpath(resolved_path,fichdll,_MAX_PATH);
	printf("intento 1: %s\n",resolved_path);
	if(resolved_path) hDLL=LoadLibraryEx(resolved_path,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);

	if(hDLL==NULL) {
		char fichdll2[256]=".\\dll\\";
		strcat(fichdll2,fichdll);
		//hDLL=LoadDLL(fichdll2);
		_fullpath(resolved_path,fichdll2,_MAX_PATH);
		printf("intento 2: %s\n",resolved_path);
		if(resolved_path) hDLL=LoadLibraryEx(resolved_path,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
		if(hDLL==NULL)
			critical_error(5,fichdll); // Librería no encontrada
	}

	// Inicializa y ejecuta la funcion ExportaFuncs de la DLL

	ExportaFuncs=NULL;

	//ExportaFuncs=(TYPEOF_ExportaFuncs *)GetDLLProcAddress(hDLL,"ExportaFuncs");
	ExportaFuncs=(TYPEOF_ExportaFuncs *)GetProcAddress(hDLL,"ExportaFuncs");

	if(ExportaFuncs==NULL) {
		//FreeDLL(hDLL);
		FreeLibrary(hDLL);
		critical_error(6,fichdll); // exportafuncs no encontrado
	}

	#ifdef DEBUG_DLL
		printf("ExportaFuncs encontrado!! :D\n");
	#endif

	ExportaFuncs(EXPORTAFUNCS_PARAMS);

/***** PRUEBA ****** PRUEBA ***** PRUEBA ***** PRUEBA *****
	if(n_externs==5) {
		extfunc=ext_funcs[4].hfuncion;
		putparm("hola");
		extfunc(params,sp);
	}
 ***** PRUEBA ****** PRUEBA ***** PRUEBA ***** PRUEBA *****/

	dlls[numdlls++].hDLL=hDLL;

#else   // linux

    TYPEOF_ExportaFuncs *ExportaFuncs;
    void *hDLL;
    char *errordll;
//	TYPEOF_extfunc *extfunc; // *** PRUEBA ***

	static char fichdll[256];

	strcpy(fichdll,nombre);
	strcat(fichdll,".so");

	// Carga la DLL
	hDLL=dlopen(fichdll,RTLD_NOW);

	if(!hDLL) {
		char fichdll2[256]="so/";
		strcat(fichdll2,fichdll);
		strcat(fichdll,".so");
		hDLL=LoadDLL(fichdll2);
		if(!hDLL)
			critical_error(5,fichdll); // Librería no encontrada
	}

	// Inicializa y ejecuta la funcion ExportaFuncs de la DLL

	ExportaFuncs=NULL;

	ExportaFuncs=(TYPEOF_ExportaFuncs *) dlsym(hDLL,"ExportaFuncs");

	if((errordll=dlerror())!=NULL) {
		dlclose(hDLL);
		critical_error(6,fichdll); // exportafuncs no encontrado
	}

	#ifdef DEBUG_DLL
		printf("ExportaFuncs encontrado!! :D\n");
	#endif

	ExportaFuncs(EXPORTAFUNCS_PARAMS);

/***** PRUEBA ****** PRUEBA ***** PRUEBA ***** PRUEBA *****
	//if(n_externs==3) {
		extfunc=ext_funcs[1].hfuncion;
		putparm(63);
        putparm(30);
        putparm(24);
        putparm(6);
		extfunc(params,sp);
	//}
 ***** PRUEBA ****** PRUEBA ***** PRUEBA ***** PRUEBA *****/

	dlls[numdlls++].hDLL=hDLL;

#endif

	return 0;
}
