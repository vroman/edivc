/*
 * eDiv Compiler
 * Copyright (C) 2000-2002 Sion, Ltd.
 * http://www.sionhq.com
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

#ifdef _WIN32
	#include "dll_load.h"
	#include <io.h>
#else
	#include <dlfcn.h>
	#include <dirent.h>
	#include <sys/stat.h>
#endif

#include <string.h>
#include <stdlib.h>
#include "modulos.h"
#include "extern.h"
#include "edivcfg/iniparser.h"

#include "language.h"

int imem_temp;
int iloc_temp;

/* obtiene el nombre esencial de la libreria (sin extension ni path) */
void get_rawname(char* completo, char* rawname)
{
	char* p=completo;
	int i;

	for(i=strlen(completo)-1;i>0;i--)
		if(completo[i]=='\\' || completo[i]=='/') {			
			p=&completo[i+1];
			break;
		}

	strcpy(rawname,p);
	i=strlen(rawname);

	#ifdef _WIN32
		if (rawname[i-4]=='.' && (rawname[i-3]=='d' || rawname[i-3]=='D')
							  && (rawname[i-2]=='l' || rawname[i-2]=='L')
							  && (rawname[i-1]=='l' || rawname[i-1]=='L'))
							  
							  rawname[i-4]=0;

	#else
		if(rawname[i-3]=='.' && (rawname[i-2]=='s' || rawname[i-2]=='S')
							 && (rawname[i-1]=='o' || rawname[i-1]=='O'))
			
							 rawname[i-3]=0;
	#endif
}


/* compara prioridades de dos dlls, para el qsort descendente */
int compara(const void* _a, const void* _b)
{
	struct _dlls a=*(struct _dlls*)_a;
	struct _dlls b=*(struct _dlls*)_b;
	return a.prioridad<b.prioridad?1:(a.prioridad>b.prioridad?-1:0);
}

int compara2(const void* _a, const void* _b)
{
	int a=*(int*)_a;
	int b=*(int*)_b;
	return dlls[a].prioridad<dlls[b].prioridad?1:(dlls[a].prioridad>dlls[b].prioridad?-1:0);
}


void dll_func()
{
	char dllkey[256]="dll_priority:";
	char rawname[256];
	int carga=1;
	char *dir=NULL;
	int i;

	#ifdef _WIN32
		int buscafich;
		struct _finddata_t fichero_dll;	/* busqueda de DLLs */
		char mask[256];
	#else
		DIR *directorio;
		struct dirent *fichero_dll;
	#endif

	numdlls=0;
	n_externs=0;
	
	#ifdef _WIN32
		/* Inicializa la carga de DLLs */
		CheckWindowsVersion();
		InitializeDLLLoad();
	#endif

	#ifdef _WIN32
		if (ini) 
			dir=iniparser_getstr(ini,"dll:windll");
		
		if (!dir) 
			dir="dll";
		
		strcpy(mask,dir);
		strcat(mask,"\\*.dll");

		/* Busca las DLLs que hay en el directorio */
		buscafich = _findfirst(mask, &fichero_dll);
		if (buscafich==-1)
			printf(translate(43)); /* no hay dlls */
		

		#ifdef _DEBUG
			printf("dbg: DLL encontrada: %s\n",fichero_dll.name);
		#endif

		sprintf(fichdll,"%s\\%s",dir,fichero_dll.name);

		get_rawname(fichero_dll.name,rawname);
		strcat(dllkey,rawname);

		if (ini) 
			if (iniparser_getint(ini,dllkey,0)<=P_NUNCA) 
				carga=0;

		if (carga) 
			if(!leedll()) {
				dlls[numdlls].prioridad=0;
			/* guarda el nombre de fichero en la tabla de DLLs */
				dlls[0].nombre=(char*)e_malloc(strlen(rawname)+1);
				strcpy(dlls[0].nombre,rawname);	

			/* importa las funciones de la DLL */
				if (ini) 
					dlls[numdlls].prioridad=iniparser_getint(ini,dllkey,dlls[numdlls].prioridad);

				numdlls=1;
			}

		/* hace lo mismo con el resto */
		while(1) {
			if(_findnext(buscafich,&fichero_dll)==0) {
				#ifdef _DEBUG
					printf("dbg: DLL encontrada: %s\n",fichero_dll.name);
				#endif
				carga=1;
				sprintf(fichdll,"%s\\%s",dir,fichero_dll.name);
				dlls[numdlls].prioridad=0;
				get_rawname(fichero_dll.name,rawname);
				strcpy(dllkey,"dll_priority:");
				strcat(dllkey,rawname);
				if(ini) if(iniparser_getint(ini,dllkey,0)<=P_NUNCA) carga=0;
				if(carga) if(!leedll()) {
					dlls[numdlls].nombre=(char*)e_malloc(strlen(rawname)+1);
					strcpy(dlls[numdlls].nombre,rawname);
					dlls[numdlls].usado=0;
					if(ini) dlls[numdlls].prioridad=iniparser_getint(ini,dllkey,dlls[numdlls].prioridad);
					numdlls++;
				}
			}
			else
				break;
		}

	#else	/* si estamos en Linux */

		if(ini) dir=iniparser_getstr(ini,"dll:nixdll"));
		if(!dir) dir="so";

		directorio=opendir(dir);
		if(!directorio) {
			printf(translate(44),dir); /* no hay directorio so/ */
			exit(1);
		}
		/* Nos saltamos las dos primeras entradas "./" y "../" */
		for(i=0;i<2;i++) {
			fichero_dll=readdir(directorio);
			if(fichero_dll==0)
				printf(translate(43)); /* no hay so's */
		}
		/* Busca las .so */
		while (1) {
			fichero_dll=readdir(directorio);
			if(fichero_dll==0)
				break;
			if(fichero_dll->d_type==DT_REG) {
				char dllkey[256]="dll_priority:";
				#ifdef _DEBUG
					printf("dbg: Librería encontrada: %s/%s\n",dir,fichero_dll->d_name);
				#endif
				carga=1;
				sprintf(fichdll,"%s/%s",dir,fichero_dll->d_name);
				dlls[numdlls].prioridad=0;
				get_rawname(fichero_dll->d_name,rawname);
				strcpy(dllkey,"dll_priority:");
				strcat(dllkey,rawname);
				if(ini) if(iniparser_getint(ini,dllkey,0)<=P_NUNCA) carga=0;
                if(carga) if(!leedll()) {
					dlls[numdlls].nombre=(char*)e_malloc(strlen(rawname)+1);
					strcpy(dlls[numdlls].nombre,rawname);
					dlls[numdlls].usado=0;
					if(ini) dlls[numdlls].prioridad=iniparser_getint(ini,dllkey,dlls[numdlls].prioridad);
					numdlls++;
				}
            }
		}

	#endif

	if(numdlls==0)
		printf(translate(43)); /* no hay librerias */

	nuevo_orden=e_malloc(numdlls*4);
	for(i=0;i<numdlls;i++)
		nuevo_orden[i]=i;

	qsort(nuevo_orden,numdlls,4,compara2);
	qsort(dlls,numdlls,sizeof(struct _dlls),compara);

	#ifdef _DEBUG
		for(i=0;i<numdlls;i++)
			printf("dbg: %d - %s - prioridad: %d\n",i,dlls[i].nombre,dlls[i].prioridad);
	#endif

}


void dll_func2()
{
	int i=numdlls;
	char* dir=NULL;

	n_externs=0;
	
	for(i=0;i<numdlls;i++)
		nuevo_orden[i]=i;

	for(numdlls=0;numdlls<i;numdlls++) {
		if((dlls[numdlls].prioridad>P_NUNCA) && (dlls[numdlls].usado || dlls[numdlls].prioridad>=P_SIEMPRE)) {
			#ifdef _DEBUG
				printf("dbg: Se requiere %s (id=%d)\n",dlls[numdlls].nombre,numdlls);
			#endif
			#ifdef _WIN32
				if(ini) dir=iniparser_getstr(ini,"dll:windll");
				if(!dir) dir="dll";
			#else
				if(ini) dir=iniparser_getstr(ini,"dll:nixdll");
				if(!dir) dir="so";
			#endif
			sprintf(fichdll,"%s\\%s.dll",dir,dlls[numdlls].nombre);
			if(leedll()) {
				printf(translate(45),dlls[numdlls].nombre); /* error al cargar libreria */
				exit(1);
			}
		}
	}
}


int leedll()
{
#ifdef _WIN32
	TYPEOF_ExportaFuncs *ExportaFuncs;
	HINSTANCE hDLL;

	/* Carga la DLL */
	hDLL=LoadDLL(fichdll);

	if(hDLL==NULL) {
		printf(translate(46),fichdll); /* no puedo cargar libreria */
		return 1;
	}

	/* Inicializa y ejecuta la funcion ExportaFuncs de la DLL */
	ExportaFuncs=NULL;
	ExportaFuncs=(TYPEOF_ExportaFuncs *)GetDLLProcAddress(hDLL,"ExportaFuncs");

	if(ExportaFuncs==NULL) {
		printf(translate(47),fichdll); /* exportafuncs no encontrado */
		FreeDLL(hDLL);
		return 1;
	}

	#ifdef DEBUG_DLL
		printf("dbg: ExportaFuncs encontrado!! :D\n");
	#endif

	ExportaFuncs(EXPORTAFUNCS_PARAMS);

	FreeDLL(hDLL);

#else   /* Linux */

    TYPEOF_ExportaFuncs *ExportaFuncs;
    void *hDLL;
    char *errordll;

	/* Carga la DLL */
	hDLL=dlopen(fichdll,RTLD_NOW);

	if(!hDLL) {
		printf(translate(46),fichdll,dlerror()); /* no puedo cargar libreria */
		return 1;
	}

	/* Inicializa y ejecuta la funcion ExportaFuncs de la DLL */

	ExportaFuncs=NULL;

	ExportaFuncs=(TYPEOF_ExportaFuncs *) dlsym(hDLL,"ExportaFuncs");

	if((errordll=dlerror())!=NULL) {
		printf(translate(47),fichdll,errordll); /* exportafuncs no encontrado */
		dlclose(hDLL);
		return 1;
	}

	#ifdef DEBUG_DLL
		printf("dbg: ExportaFuncs encontrado!! :D\n");
	#endif

	ExportaFuncs(EXPORTAFUNCS_PARAMS);
	dlclose(hDLL);
#endif

	return 0;
}
