
#include <stdio.h>
#include <export.h>
#include "../../shared/varindex.h"
#include "file.h"





int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	CONST("seek_cur",SEEK_CUR) ;
	CONST("seek_set",SEEK_SET) ;
	CONST("seek_end",SEEK_END) ;

	FUNCTION("fopen",2,eDiv_Fopen) ;
	FUNCTION("fwrite",3,eDiv_Fwrite) ;
	FUNCTION("fread",3,eDiv_Fread) ;
	FUNCTION("ftell",1,eDiv_Ftell) ;
	FUNCTION("fseek",3,eDiv_Fseek) ;
	FUNCTION("flush",0,eDiv_Flush) ;
	FUNCTION("fclose",1,eDiv_Fclose) ;
	return TRUE ;

}

FILE *fichero[0xFF] ;
int existe[0xFF] ;

int eDiv_Fopen(FUNCTION_PARAMS)
{
	int i ;
	char *filename ;
	char *modo ;

	modo = getstrparm() ;
	filename = getstrparm() ;

	for ( i = 1 ; i < 0xFF ; i++ )
		if ( existe[i] == 0 )
			break ;
	if ( i == 0xFF )
		return 0 ;

	
	fichero[i] = fopen( filename, modo ) ;
	if ( fichero[i] == NULL )
		return 0 ;

	existe[i] = 1 ;
	return i ;
}

int eDiv_Fwrite(FUNCTION_PARAMS)
{
	int offset , longitud , handle ;
	int escrito ;
	handle = getparm() ;
	longitud = getparm() ;
	offset = getparm() ;

	if ( !existe[handle] )
		return 0 ;
	
	escrito = fwrite( &fp->mem[ offset ] , 4 , longitud , fichero[handle] ) ;

	if ( escrito == longitud )
		return 1 ;
	else
		return 0 ;
}

int eDiv_Fread(FUNCTION_PARAMS)
{
	int offset, longitud , handle ;
	int leido ;
	handle = getparm() ;
	longitud = getparm() ;
	offset = getparm() ;

	if ( !existe[handle] )
		return 0 ;

	leido = fread( &fp->mem[ offset ] , 4 , longitud , fichero[handle] ) ;

	if ( leido == longitud )
		return 1 ;
	else
		return 0 ;
}

int eDiv_Ftell(FUNCTION_PARAMS)
{
	int handle ;

	if ( !existe[handle] )
		return -1 ;

	return (int)ftell( fichero[handle] ) ;
}

int eDiv_Fseek(FUNCTION_PARAMS)
{
	int handle , posicion, modo ;
	modo = getparm() ;
	posicion = getparm() ;
	handle = getparm() ;

	if ( !existe[handle] ) 
		return -1 ;

	if ( fseek( fichero[handle] , posicion , modo ) == 0 )
		return 1 ;
	else
		return -2 ;

}

	
int eDiv_Flush(FUNCTION_PARAMS)
{
	return _flushall() ;
}

int eDiv_Fclose(FUNCTION_PARAMS)
{
	int handle, num ;

	if ( handle == 0 )
		if ( ( num = _fcloseall() ) == EOF )
			return 0 ;
		else
			return num ;
	else
		if ( !existe[handle] )
			return 0 ;
		else
			if ( ( num = fclose( fichero[handle] ) ) == EOF )
				return 0 ;
			else
				return num ;
}
			


//============== Entrypoints =====================================================





