
#include <export.h>
#include "dinmem.h"


/*
 * POR HACER: eDIV debe controlar todas las peticiones de memoria dinámica
 * es decir, si hacemos un free() donde no debemos, debe producirse un 
 * fp->Runtime_Error
 */
int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	FUNCTION("malloc",1,eDiv_Malloc) ;
	FUNCTION("free",1,eDiv_Free) ;
	return TRUE ;
}

int eDiv_Malloc(FUNCTION_PARAMS)
{
	int longitud ;
	int *puntero ;
	longitud = getparm() ;

	puntero = calloc(longitud,4) ;
	if ( puntero == NULL ) {
		fp->Runtime_Error(100); /* no hay memoria suficiente */
		return 0 ;
	}
	else
		return ( puntero - fp->mem ) ;
}

int eDiv_Free(FUNCTION_PARAMS)
{
	int offset ;
	int *puntero ;
	offset = getparm() ;
	puntero = &fp->mem[offset] ;
	free( puntero ) ;

	return 1 ;
}




