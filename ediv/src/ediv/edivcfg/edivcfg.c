#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "e_error.h"
#include "iniparser.h"

struct cfg {
	// seccion [general]
	char *lang;
	char *motdhost;
	
	// seccion [default_compiler_options]
	int max_process;
	unsigned char free_sintax;
	unsigned char extended_conditions;
	unsigned char case_sensitive;
	unsigned char ignore_errors;
	unsigned char no_strfix;
	unsigned char no_optimization;
	unsigned char no_range_check;
	unsigned char no_id_check;
	unsigned char no_null_check;
	unsigned char no_check;

	// seccion dll
	char *dllpath;
} config;


int parse_ini_file(char * ini_name)
{
	struct cfg conf;
	dictionary *ini ;

	ini=iniparser_load(ini_name);
	if (ini==NULL) {
		e_error("No se pudo leer el archivo correctamente [%s]", ini_name);
		return -1 ;
	}

	// Parsea la seccion [general]
	conf.lang=iniparser_getstr(ini,"general:lang");
	conf.motdhost=iniparser_getstr(ini,"general:motdhost");

	// Parsea la seccion [default_compiler_options]
	conf.max_process=iniparser_getint(ini,"default_compiler_options:max_process",-1);
	conf.free_sintax=iniparser_getboolean(ini,"default_compiler_options:free_sintax",-1);
	conf.extended_conditions=iniparser_getboolean(ini,"default_compiler_options:extended_conditions",-1);
	conf.case_sensitive=iniparser_getboolean(ini,"default_compiler_options:case_sensitive",-1);
	conf.ignore_errors=iniparser_getboolean(ini,"default_compiler_options:ignore_errors",-1);
	conf.no_strfix=iniparser_getboolean(ini,"default_compiler_options:no_strfix",-1);
	conf.no_optimization=iniparser_getboolean(ini,"default_compiler_options:no_optimization",-1);
	conf.no_range_check=iniparser_getboolean(ini,"default_compiler_options:no_range_check",-1);
	conf.no_id_check=iniparser_getboolean(ini,"default_compiler_options:no_id_check",-1);
	conf.no_null_check=iniparser_getboolean(ini,"default_compiler_options:no_null_check",-1);
	conf.no_check=iniparser_getboolean(ini,"default_compiler_options:no_check",-1);
	
	
	
	
	// Y ahora vamos con la seccion [dll]
#ifdef _WIN32
	conf.dllpath=iniparser_getstr(ini,"dll:windll");
#else
	conf.dllpath=iniparser_getstr(ini,"dll:nixdll");
#endif

	// Si estamos en modo debug volcamos todo a pantalla
#ifdef _DEBUG
	printf("\nidioma: %s",conf.lang);
	printf("\nhost del motd: %s",conf.motdhost);
	printf("\n---");
	printf("\nNumero maximo de procesos activos: %d",conf.max_process);
	printf("\nSintaxis libre: %d",conf.free_sintax);
	printf("\nCondiciones extendidas: %d",conf.extended_conditions);
	printf("\nCase Sensitive: %d",conf.case_sensitive);
	printf("\nIgnorar errores: %d",conf.ignore_errors);
	printf("\nNo strfix: %d",conf.no_strfix);
	printf("\nOptimizar codigo: %d",conf.no_optimization);
	printf("\nComprobar rango: %d",conf.no_range_check);
	printf("\nComprobar id: %d",conf.no_id_check);
	printf("\nComprobar nulos: %d",conf.no_null_check);
	printf("\nHacer comprobaciones: %d",conf.no_check);

	printf("\n---");
	printf("\nRuta a los archivos .dll: %s",conf.dllpath);
#endif

	iniparser_freedict(ini);
	return 0 ;
}


