
#include <export.h>
#include "../../shared/varindex.h"
#include "dinmem.h"





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
	if ( puntero == NULL )
		return 0 ;
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



//============== Entrypoints =====================================================





