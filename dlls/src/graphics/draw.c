/*! \file draw.c
 * \brief Funciones de dibujo de figuras 2D (draw's)
 */

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"

/*! \brief Crea un nuevo draw
 * \todo Permitir más tipos de figuras, y hacer que se dibujen en tiempo real, para que sean más rápidos y no consuman memoria
 * @param t Tipo de figura (1=línea, 2=rectángulo, 3=rectángulo relleno, 4=elipse, 5=elipse rellena)
 * @param c Color
 * @param o Opacidad (0..15)
 * @param r Región
 * @param x0 Coordenada X inicial
 * @param y0 Coordenada Y inicial
 * @param x1 Coordenada X final
 * @param y1 Coordenada Y final
 * @return El identificador del draw, o -1 si hubo un error
 * @see eDIV_MOVE_DRAW(), eDIV_DELETE_DRAW(), #draws, frame()
 */
int eDIV_DRAW(FUNCTION_PARAMS)
{
	int i ;
	int t , c , o , r , x0, y0 , x1 , y1  ;
	SDL_Rect dstrect ;
	y1 = getparm() ;
	x1 = getparm() ;
	y0 = getparm() ;
	x0 = getparm() ;
	r = getparm() ;
	o = getparm() ;
	c = getparm() ;
	t = getparm() ;

	for ( i = 0 ; i < 1024 ; i++ )
	{
		if ( draws[i].existe == 0 )
		{
			draws[i].Surface = SDL_CreateRGBSurface( SDL_HWSURFACE , x1 - x0 , y1 - y0 , screen->format->BitsPerPixel , 0xFF0000 , 0x00FF00 , 0x0000FF , 0x000000 ) ;
			switch ( t )
			{
			case 3:
				dstrect.x = 0 ;
				dstrect.y = 0 ;
				dstrect.w = x1 - x0 ;
				dstrect.h = y1 - y0 ;
				SDL_FillRect( draws[i].Surface , &dstrect , c ) ;
				break ;
			}
			draws[i].region = r ;
			draws[i].x = x0 ;
			draws[i].y = y0 ;
			draws[i].t = t ;
			draws[i].c = c ;
			if ( o < 15 && o > -1 )
				SDL_SetAlpha( draws[i].Surface , SDL_SRCALPHA | SDL_RLEACCEL , 17 * (o) ) ;
			draws[i].existe = 1 ;
			if ( i > last_draw )
				last_draw = i ;
			return i ;
		}
	}
	return -1 ;
}


/*! \brief Cambia los atributos de un draw
 * @param id Identificador del draw a modificar
 * @param c Color
 * @param o Opacidad (0..15)
 * @param x0 Coordenada X inicial
 * @param y0 Coordenada X inicial
 * @param x1 Coordenada Y final
 * @param y1 Coordenada X final
 * @return 1 si todo fue bien, -1 si hubo un error
 * @see eDIV_DRAW(), eDIV_DELETE_DRAW(), #draws
 */
int eDIV_MOVE_DRAW(FUNCTION_PARAMS)
{
	SDL_Rect dstrect ;
	int id , c , o , x0 , y0 , x1 , y1 ;
	y1 = getparm() ;
	x1 = getparm() ;
	y0 = getparm() ;
	x0 = getparm() ;
	o = getparm() ;
	c = getparm() ;
	id = getparm() ;
	
	if ( !draws[id].existe )
		return -1 ;

	if ( x1 - x0 != draws[id].Surface->w || y1 - y0 != draws[id].Surface->h || c != draws[id].c )
	{
		if ( x1 - x0 != draws[id].Surface->w || y1 - y0 != draws[id].Surface->h )
		{
			SDL_FreeSurface( draws[id].Surface ) ;
			draws[id].Surface = SDL_CreateRGBSurface( SDL_HWSURFACE , x1 - x0 , y1 - y0 , screen->format->BitsPerPixel , 0xFF0000 , 0x00FF00 , 0x0000FF , 0x000000 ) ;
		}
		switch ( draws[id].t )
		{
		case 3:
			dstrect.x = 0 ;
			dstrect.y = 0 ;
			dstrect.w = x1 - x0 ;
			dstrect.h = y1 - y0 ;
			SDL_FillRect( draws[id].Surface , &dstrect , c ) ;
			break ;
		}
	}

	draws[id].x = x0 ;
	draws[id].y = y0 ;
	draws[id].c = c ;
	if ( o < 15 && o > -1 )
		SDL_SetAlpha( draws[id].Surface , SDL_SRCALPHA | SDL_RLEACCEL , 17 * (o) ) ;
	else
		if ( o == 15 )
			SDL_SetAlpha( draws[id].Surface , 0 , 255 ) ;

	return 1 ;
}

/*! \brief Elimina un draw
 * @param n Identificador del draw o \a all_drawing (-1).
 * @return 1 si todo fue bien, -1 si hubo un error
 * @see eDIV_DRAW(), eDIV_MOVE_DRAW(), #draws
 */
int eDIV_DELETE_DRAW(FUNCTION_PARAMS)
{
	int n ;
	n = getparm() ;

	if(n==-1) {
		for(n=0;n<MAX_DRAWS;n++) {
			if(draws[n].existe) {
				SDL_FreeSurface( draws[n].Surface );
				draws[n].existe = 0;
			}
		}
		last_draw=0;
		return 1;
	}

	if ( !draws[n].existe )
		return -1 ;

	SDL_FreeSurface( draws[n].Surface ) ;
	draws[n].existe = 0 ;

	for ( ; last_draw > 0 ; last_draw-- )
	{
		if ( draws[last_draw].existe )
			break ;
	}
	return 1 ;


	
}
