/***************************************************************************
                          modulos.c  -  description
                             -------------------
    begin                : Sat Jun 23 2001
    copyright            : (C) 2001 by Sion Entertainment
    email                : bugs@edivcentral.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef _WIN32
	#include "dll_load.h"
	#include <io.h>
	HINSTANCE hDLL[999];
	int dll_n;
#else
	#include <dlfcn.h>
	#include <dirent.h>
	#include <sys/stat.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modulos.h"
#include "extern.h"

int imem_temp;
int iloc_temp;
TYPEOF_ExportaFuncs *ExportaFuncs[100];
int funciones;


void dll_func() // ke original, no? XD
{
	#ifdef WIN32
		int buscafich;
		struct _finddata_t fichero_dll;	// busqueda de DLLs
	#else
		int i;
		DIR *directorio;
		struct dirent *fichero_dll;
	#endif

	numdlls=0;
	
	#ifdef WIN32
		// Inicializa la carga de DLLs
		CheckWindowsVersion();
		InitializeDLLLoad();
	#endif

	#ifdef WIN32
		// Busca las DLLs que hay en el directorio
		buscafich = _findfirst("dll\\*.dll", &fichero_dll);
		if (buscafich==-1) {
			printf("No se encuentra ninguna DLL");
			exit(1);
		}
		printf("DLL encontrada: %s\n",fichero_dll.name);

		// guarda el nombre de fichero en la tabla de DLLs
		printf("\nGuardando el nombre de la dll...");
		dlls[0].nombre=(char*)e_malloc(strlen(fichero_dll.name)+1);
		strcpy(dlls[0].nombre,fichero_dll.name);
		printf("\nGuardado el nombre de la dll...");

		// importa las funciones de la DLL
		strcpy(fichdll,"dll\\");
		strcat(fichdll,fichero_dll.name);
		leedll();
		
		numdlls=1;
		// hace lo mismo con el resto
		while(1) {
			if(_findnext(buscafich,&fichero_dll)==0) {
				printf("DLL encontrada: %s\n",fichero_dll.name);
                strcpy(fichdll,"dll\\");
        		strcat(fichdll,fichero_dll.name);
//				dlls[numdlls].nfuncs=0;
				if(!leedll()) {
					dlls[numdlls].nombre=(char*)e_malloc(strlen(fichero_dll.name)+1);
					strcpy(dlls[numdlls].nombre,fichero_dll.name);
					dlls[numdlls].usado=0;
					numdlls++;
				}
			}
			else
				break;
		}
     
	 
	


	#else	// si estamos en Linux

		directorio=opendir("so");
		if(!directorio) {
			printf("Directorio so/ no encontrado\n");
			exit(1);
		}
		// Nos saltamos las dos primeras entradas "./" y "../"
		for(i=0;i<2;i++) {
			fichero_dll=readdir(directorio);
			if(fichero_dll==0) {
				printf("No se encuentra ninguna librería\n");
				exit(1);
			}
		}
		// Busca las .so
		while (1) {
			fichero_dll=readdir(directorio);
			if(fichero_dll==0)
				break;
			if(fichero_dll->d_type==DT_REG) {
			    printf("Librería encontrada: so/%s\n",fichero_dll->d_name);
                strcpy(fichdll,"so/");
        		strcat(fichdll,fichero_dll->d_name);
//				dlls[numdlls].nfuncs=0;
                if(!leedll()) {
					dlls[numdlls].nombre=(char*)e_malloc(strlen(fichero_dll->d_name)+1);
					strcpy(dlls[numdlls].nombre,fichero_dll->d_name);
					dlls[numdlls].usado=0;
					numdlls++;
				}
            }
		}
		if(numdlls==0) {
			printf("No se encuentra ninguna librería\n");
			exit(1);
		}
	#endif
}


void dll_func2()
{
	int i;
	
	for(i=0;i<numdlls;i++) {
		if(dlls[i].usado) {
			#ifdef _DEBUG
				printf("Se requiere %s (id=%d)\n",dlls[i].nombre,i);
			#endif
			#ifdef WIN32
				strcpy(fichdll,"dll\\");
			#else
				strcpy(fichdll,"so/");
			#endif
			strcat(fichdll,dlls[i].nombre);
			if(leedll()) {
				printf("Error al cargar librería %s\n",dlls[i].nombre);
				exit(1);
			}
		}
	}
}


#ifdef _WIN32
int leedll(struct _finddata_t fichero_dll)
#else
int leedll()
#endif
{
#ifdef _WIN32
	
	


	// Carga la DLL
	dll_n++;
	funciones++;

	hDLL[dll_n]=LoadDLL(fichdll);

	if(hDLL[dll_n]==NULL) {
		printf("No puedo cargar %s\n",fichdll);
		return 1;
	}

	// Inicializa y ejecuta la funcion ExportaFuncs de la DLL

	ExportaFuncs[funciones]=NULL;

	ExportaFuncs[funciones]=(TYPEOF_ExportaFuncs *)GetDLLProcAddress(hDLL[dll_n],"ExportaFuncs");
	//extfunc=(TYPEOF_extfunc *)GetDLLProcAddress(hDLL,"eDIV_ediv");
	//extfunc2=(TYPEOF_extfunc2 *)GetDLLProcAddress(hDLL,"eDIV_ediv");

	if(ExportaFuncs[funciones]==NULL) {
		printf("No se encuentra ExportaFuncs en %s - DLL no valida\n",fichdll);
		FreeDLL(hDLL[dll_n]);
		return 1;
	}

//	#ifdef DEBUG_DLL
		printf("\nDll numero %i,funcion numero %i",dll_n,funciones);
//	#endif


	ExportaFuncs[funciones](EXPORTAFUNCS_PARAMS);





#else   // linux

    TYPEOF_ExportaFuncs *ExportaFuncs;
    void *hDLL;
    char *errordll;
//	TYPEOF_extfunc *extfunc; // *** PRUEBA ***

	// Carga la DLL
	hDLL=dlopen(fichdll,RTLD_NOW);

	if(!hDLL) {
		printf("No puedo cargar %s (%s)\n",fichdll,dlerror());
		return 1;
	}

	// Inicializa y ejecuta la funcion ExportaFuncs de la DLL

	ExportaFuncs=NULL;

	ExportaFuncs=(TYPEOF_ExportaFuncs *) dlsym(hDLL,"ExportaFuncs");

	if((errordll=dlerror())!=NULL) {
		printf("No se encuentra ExportaFuncs en %s - Librería no válida (%s)\n",fichdll,errordll);
		dlclose(hDLL);
		return 1;
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

	dlclose(hDLL);
#endif

	return 0;
}



int close_dll(int numero)
{
	#ifdef _WIN32
	FreeDLL(hDLL[numero]);
	#endif

	return 0;
}
