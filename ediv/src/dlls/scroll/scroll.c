

#include <SDL/SDL.h>
#include <export.h>
//#include "../../shared/varindex.h"
#include "scroll.h"


struct
{
	int existe ;
	int file, graph ;
	int x0 , y0 , x1 , y1 ;
	int z ;
	int camera ;
	int ratio ;
	int speed ;
	int region1 , region2 ;
}scrolls[10] ;


int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	GLOBAL_STRUCT("scroll",9);
		_INT("x0",0);
		_INT("y0",0);
		_INT("x1",0);
		_INT("y1",0);
		_INT("z",0);
		_INT("camera",0);
		_INT("ratio",0);
		_INT("speed",0);
		_INT("region1",0);
		_INT("region2",0);
	END_STRUCT;

	FUNCTION("start_scroll",6,eDiv_StartScroll) ;

	ENTRYPOINT( frame ) ;
	
	return TRUE ;
}

FILE * fichero ;

int eDiv_StartScroll(FUNCTION_PARAMS)
{
	int num , f , g1 , g2 , r , b ;
	b = getparm() ;
	r = getparm() ;
	g2 = getparm() ;
	g1 = getparm() ;
	f = getparm() ;
	num = getparm() ;

	if ( scrolls[num].existe )
		return -1 ;

	scrolls[num].existe = 1 ;
/*
	fichero = fopen("scroll.txt" , "w+" ) ;
	fprintf(fichero , "glo_scroll: %d" , varindex[_glo_scroll] ) ;
	fclose(fichero) ;*/
	/*
	fp->mem[ varindex[_glo_scroll] + 0 ] = 0 ;
	fp->mem[ varindex[_glo_scroll] + 1 ] = 0 ;
	fp->mem[ varindex[_glo_scroll] + 2 ] = 0 ;
	fp->mem[ varindex[_glo_scroll] + 3 ] = 0 ;
	fp->mem[ varindex[_glo_scroll] + 4 ] = 512 ;
	fp->mem[ varindex[_glo_scroll] + 0 ] = 0 ;
	fp->mem[ varindex[_glo_scroll] + 0 ] = b ;
	fp->mem[ varindex[_glo_scroll] + 0 ] = 0 ;
	fp->mem[ varindex[_glo_scroll] + 0 ] = r ;
	fp->mem[ varindex[_glo_scroll] + 0 ] = r ;
*/
	return 1 ;
}



//============== Entrypoints =====================================================

void frame(FUNCTION_PARAMS)
{
	SDL_Rect srcrect, dstrect ;

	// de momento solo hay esta demostracion de que fp->Dibuja funciona xDD
	// falta hacer los scrolls :P
/*
	if ( fp->existe.dibuja )	
	{
		if ( fp->files[0].existe )
		{
			if ( fp->files[0].mapa[1000].existe )
			{
				srcrect.x = 0 ;
				srcrect.y = 0 ;
				srcrect.w = fp->files[0].mapa[1000].Surface->w ;
				srcrect.h = fp->files[0].mapa[1000].Surface->h ;
				fp->Dibuja( fp->files[0].mapa[1000].Surface , srcrect , srcrect , 0 , 128 ) ;
			}
		}
	}
			
*/			

}




