
#include <stdio.h>
#include <export.h>
#include <errno.h>
//#include "../../shared/varindex.h"
#include "file.h"





int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	CONST("seek_cur",SEEK_CUR) ;
	CONST("seek_set",SEEK_SET) ;
	CONST("seek_end",SEEK_END) ;

	GLOBAL("unit_size",4);

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

/* POR HACER:
 * Los handles deben ser IMPARES
 */

int eDiv_Fopen(FUNCTION_PARAMS)
{
	int i ;
	char *filename ;
	char modo[128];

	strcpy(modo,getstrparm());
	filename = getstrparm() ;

	for (i=0;i<strlen(modo);i++)
		if (modo[i]!='r' && modo[i]!='w' && modo[i]!='a' && modo[i]!='+') break;
	if (i<strlen(modo)) {
		fp->Runtime_Error(166); /* modo de acceso al archivo no válido */
		return 0;
	}
	strcat(modo,"b");

	for ( i = 1 ; i < 0xFF ; i++ )
		if ( existe[i] == 0 )
			break ;
	if ( i == 0xFF ) {
		fp->Runtime_Error(169); /* demasiados archivos abiertos */
		return 0 ;
	}
	
	fichero[i] = fopen( filename, modo ) ;
	if ( fichero[i] == NULL ) {
		if(errno==EMFILE)
			fp->Runtime_Error(169); /* demasiados archivos abiertos */
		return 0 ;
	}

	existe[i] = 1 ;
	return i;
}

int eDiv_Fwrite(FUNCTION_PARAMS)
{
	int offset , longitud , handle ;
	int escrito ;
	handle = getparm() ;
	longitud = getparm() ;
	offset = getparm() ;

	if ( !existe[handle] ) {
		fp->Runtime_Error(170); /* handle no válido */
		return 0;
	}
	
	escrito = fwrite( &fp->mem[ offset ] , global("unit_size") , longitud , fichero[handle] ) ;

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

	leido = fread( &fp->mem[ offset ] , global("unit_size") , longitud , fichero[handle] ) ;
	
	if ( leido == longitud )
		return 1 ;
	else
		return 0 ;
}

int eDiv_Ftell(FUNCTION_PARAMS)
{
	int handle=getparm();

	if ( !existe[handle] ) {
		fp->Runtime_Error(170); /* handle no válido */
		return -1 ;				/* En caso de error, DIV2 devuelve handle, pero no me parece muy correcto... */
	}

	return (int)ftell( fichero[handle] ) ;
}

int eDiv_Fseek(FUNCTION_PARAMS)
{
	int handle , posicion, modo ;
	modo = getparm() ;
	posicion = getparm()*global("unit_size") ;
	handle = getparm() ;

	if ( !existe[handle] ) {
		fp->Runtime_Error(170); /* handle no válido */
		return -1 ;				/* ocurre lo mismo que con ftell... */
	}

	if ( fseek( fichero[handle] , posicion , modo ) == 0 )
		return 0 ;
	else
		return -1 ;

}

	
int eDiv_Flush(FUNCTION_PARAMS)
{
	return flushall() ;	/* flushall()-numfiles */
}

int eDiv_Fclose(FUNCTION_PARAMS)
{
	int handle=getparm(), num ;

	if ( handle == 0 )
		if ( ( num = fcloseall() ) == EOF )
			return 0 ;
		else
			return num ;
	else
		if ( !existe[handle] ) {
			fp->Runtime_Error(170);
			return 0 ;
		}
		else
			if ( ( num = fclose( fichero[handle] ) ) == EOF )
				return 0 ;
			else
				return num ;
}
			


//============== Entrypoints =====================================================





