/*! \file map_ops.c
 * \brief Funciones de operaciones con mapas
 *
 * Este fichero contiene las funciones para las operaciones con mapas
 * (modificación, obtención de información, bliteos, etc).
 *
 * \todo Falta respetar el centro del gráfico, y soporte para FLAGS 1 y 2
 */

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"

/*! \brief Copia un trozo de mapa y lo pega en otro mapa
 *
 * Esta función sólo permite operaciones en el mismo FPG.
 * \todo ¿sobrecargar para permitir operaciones entre FPG's?
 * @param f FPG donde se encuentran los dos mapas
 * @param g2 Gráfico de destino
 * @param x2 Coordenada X destino
 * @param y2 Coordenada Y destino
 * @param g1 Gráfico de origen
 * @param x1 Coordenada X origen
 * @param y1 Coordenada Y origen
 * @param w Anchura del bloque a copiar
 * @param h Altura del bloque a copiar
 * @return 1
 * @see eDIV_MAP_PUT(), eDIV_MAP_XPUT()
 */
int eDIV_MAP_BLOCK_COPY(FUNCTION_PARAMS)
{
	int f , g2 , x2 , y2 , g1 , x1 , y1 , w , h ;
	SDL_Rect srcrect , dstrect ;
	h = getparm() ;
	w = getparm() ;
	y1 = getparm() ;
	x1 = getparm() ;
	g1 = getparm() ;
	y2 = getparm() ;
	x2 = getparm() ;
	g2 = getparm() ;
	f = getparm() ;

	srcrect.x = x1 ;
	srcrect.y = y1 ;
	srcrect.w = w ;
	srcrect.h = h ;

	dstrect.x = x2 ;
	dstrect.y = y2 ;
	dstrect.w = srcrect.w ;
	dstrect.h = srcrect.h ;

	if ( !files[f].existe || !files[f].mapa[g1].existe || !files[f].mapa[g2].existe )
		return -1 ;

	SDL_BlitSurface( files[f].mapa[g1].Surface , &srcrect , files[f].mapa[g2].Surface , &dstrect ) ;
	return 1 ;

}


/*! \brief Obtiene el color del pixel, en las coordenadas (x,y), de un mapa.
 * @param f Fichero (FPG) donde se encuentra el mapa
 * @param g Código del gráfico dentro del FPG.
 * @param x Coordenada X del pixel
 * @param y Coordenada Y del pixel
 * @return El color del píxel.
 * @see eDIV_MAP_PUT_PIXEL(), eDIV_PUT_PIXEL(), eDIV_GET_PIXEL()
 */
int eDIV_MAP_GET_PIXEL(FUNCTION_PARAMS)
{
	int f , g , x , y , bpp ;
	Uint8 *p ;
	y = getparm() ;
	x = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;

	if ( x > files[f].mapa[g].Surface->w || y > files[f].mapa[g].Surface->h || x < 0 || y < 0 )
		return -2 ;

	bpp = files[f].mapa[g].Surface->format->BytesPerPixel;
    p = (Uint8 *)files[f].mapa[g].Surface->pixels + y * files[f].mapa[g].Surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
		return *p;
    case 2:
		return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			return p[0] << 16 | p[1] << 8 | p[2];
		}else
		{
            return p[0] | p[1] << 8 | p[2] << 16;
		}
    case 4:
		return *(Uint32 *)p;
	}

	return 0;
}


/*! \brief Blitea un mapa dentro de otro mapa
 *
 * Esta función sólo permite operaciones en el mismo FPG.
 * \todo ¿sobrecargar para permitir operaciones entre FPG's?
 * @param f Fichero (FPG) donde se encuentran los dos mapas.
 * @param g2 Gráfico de destino
 * @param g1 Gráfico de origen
 * @param x Coordenada X
 * @param y Coordenada Y
 * @return 1
 * @see eDIV_MAP_BLOCK_COPY(), eDIV_XPUT()
 */
int eDIV_MAP_PUT(FUNCTION_PARAMS)
{
	int f , g2 , g1 , x , y ;
	SDL_Rect dstrect ;
	y = getparm() ;
	x = getparm() ;
	g1 = getparm() ;
	g2 = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g1].existe || !files[f].mapa[g2].existe )
		return -1 ;

	dstrect.x = x ;
	dstrect.y = y ;
	dstrect.w = 0 ; /* Se ignora */
	dstrect.h = 0 ; /* Se ignora */

	SDL_BlitSurface( files[f].mapa[g1].Surface , NULL , files[f].mapa[g2].Surface , &dstrect ) ;
	return 1 ;

}


/*! \brief Cambia el color de un píxel determinado de un mapa
 * @param fichero Fichero (FPG) donde se encuentra el mapa
 * @param grafico Código del mapa dentro del FPG
 * @param x Coordenada X del píxel
 * @param y Coordenada y del píxel
 * @param color Color
 * @return 1
 * @see eDIV_MAP_GET_PIXEL(), eDIV_PUT_PIXEL(), eDIV_GET_PIXEL()
 */
int eDIV_MAP_PUT_PIXEL(FUNCTION_PARAMS)
{
	int f , g , x , y , c , bpp ;
	Uint8 *p ;


	c = getparm() ;
	y = getparm() ;
	x = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[0].mapa[g].existe )
		return -1 ;

	if ( x > files[f].mapa[g].Surface->w || y > files[f].mapa[g].Surface->h || x < 0 || y < 0 )
		return -2 ;

	if ( SDL_MUSTLOCK( files[f].mapa[g].Surface ) ) {
		if ( SDL_LockSurface( files[f].mapa[g].Surface ) < 0 ) {
            return -3;
        }
    }


	bpp = files[f].mapa[g].Surface->format->BytesPerPixel ;
	p = (Uint8 *)files[f].mapa[g].Surface->pixels + y * files[f].mapa[g].Surface->pitch + x * bpp ;

    switch(bpp) {

	case 1:
        *p = c;
        break;

    case 2:
        *(Uint16 *)p = c;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (c >> 16) & 0xff;
            p[1] = (c >> 8) & 0xff;
            p[2] = c & 0xff;
        } else {
            p[0] = c & 0xff;
            p[1] = (c >> 8) & 0xff;
            p[2] = (c >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = c;
        break;
    }

    if ( SDL_MUSTLOCK( files[f].mapa[g].Surface ) ) {
        SDL_UnlockSurface( files[f].mapa[g].Surface );
    }


	return 1 ;
}


/*! \brief Blitea un mapa sobre el buffer del fondo
 * @param fichero Fichero (FPG) donde se encuentra el gráfico
 * @param grafico Código del mapa dentro del FPG
 * @param x Coordenada X destino
 * @param y Coordenada Y destino
 * @return 1
 * @see eDIV_MAP_PUT(), eDIV_XPUT()
 */
int eDIV_PUT(FUNCTION_PARAMS)
{
	int f , g , x , y ;
	SDL_Rect dstrect ;

	y = getparm() ;
	x = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;
    
	dstrect.x = x - files[f].mapa[g].cpoint[0].x ;
	dstrect.y = y - files[f].mapa[g].cpoint[0].y ;
	dstrect.w = 0 ; /* Se ignora */
	dstrect.h = 0 ; /* Se ignora */

	SDL_BlitSurface( files[f].mapa[g].Surface , NULL , fondo , &dstrect ) ;

	return 1 ;

}


/*! \brief Como eDIV_PUT() pero permitiendo indicar ángulo, tamaño y flags.
 * \todo Falta soporte para flags. Revisar y añadir fp->Runtime_Error()
 * @param fichero Fichero (FPG) donde se encuentra el gráfico
 * @param grafico Código del mapa dentro del FPG
 * @param x Coordenada X destino
 * @param y Coordenada Y destino
 * @param angle Ángulo (en milésimas de grado)
 * @param zoom Tamaño (en porcentaje)
 * @return 1 si se ha colocado correctamente, -1 si el mapa no existe.
 * @see eDIV_PUT(), eDIV_MAP_XPUT(), eDIV_MAP_PUT()
 */
int eDIV_XPUT(FUNCTION_PARAMS)
{
	int f , g , x , y ;
	double zoom,angle;
	SDL_Surface *map;
	SDL_Rect dstrect ;

	zoom  = getparm();
	angle = getparm();
	y = getparm() ;
	x = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;
    
	dstrect.x = x - files[f].mapa[g].cpoint[0].x ;
	dstrect.y = y - files[f].mapa[g].cpoint[0].y ;
	dstrect.w = 0 ; /* Se ignora */
	dstrect.h = 0 ; /* Se ignora */

	map=xput(files[f].mapa[g].Surface,zoom,angle);

	SDL_BlitSurface(map , NULL , fondo , &dstrect ) ;
	SDL_FreeSurface (map);

	return 1 ;

}


/*! \brief Blitea un mapa sobre el buffer del fondo en la ESQUINA SUPERIOR IZQUIERDA de la pantalla
 * @param fichero Fichero (FPG) donde se encuentra el gráfico
 * @param grafico Código del mapa dentro del FPG
 * @return 1
 * @see eDIV_PUT(), eDIV_LOAD_SCREEN()
 */
int eDIV_PUT_SCREEN(FUNCTION_PARAMS)
{
	int f , g ;

	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;
    
	SDL_BlitSurface( files[f].mapa[g].Surface , NULL , fondo , NULL ) ;

	return 1 ;

}

/*! \brief Cambia el color del pixel, en las coordenadas (x,y), del fondo.
 * @param x Coordenada X del pixel
 * @param y Coordenada Y del pixel
 * @param color Color
 * @return 1
 * @see eDIV_MAP_PUT_PIXEL(), eDIV_GET_PIXEL()
 */
int eDIV_PUT_PIXEL(FUNCTION_PARAMS)
{
	int x , y , c , bpp ;
	Uint8 *p ;


	c = getparm() ;
	y = getparm() ;
	x = getparm() ;

	if ( x > fondo->w || y > fondo->h || x < 0 || y < 0 )
		return -1 ;

    if ( SDL_MUSTLOCK(fondo) ) {
        if ( SDL_LockSurface(fondo) < 0 ) {
            return -2;
        }
    }


	bpp = fondo->format->BytesPerPixel ;
	p = (Uint8 *)fondo->pixels + y * fondo->pitch + x * bpp ;

    switch(bpp) {

	case 1:
        *p = c;
        break;

    case 2:
        *(Uint16 *)p = c;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (c >> 16) & 0xff;
            p[1] = (c >> 8) & 0xff;
            p[2] = c & 0xff;
        } else {
            p[0] = c & 0xff;
            p[1] = (c >> 8) & 0xff;
            p[2] = (c >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = c;
        break;
    }

    if ( SDL_MUSTLOCK(fondo) ) {
        SDL_UnlockSurface(fondo);
    }


	return 1 ;
}


/*! \brief Rellena el buffer del fondo con el color 0.
 * @return 1
 */
int eDIV_CLEAR_SCREEN(FUNCTION_PARAMS)
{
	SDL_FillRect( fondo , NULL , 0 ) ;

	return 1;
}

/*! \brief Obtiene el color del pixel, en las coordenadas (x,y), del buffer del fondo.
 * @param x Coordenada X del pixel
 * @param y Coordenada Y del pixel
 * @return El color del píxel.
 * @see eDIV_PUT_PIXEL(), eDIV_MAP_GET_PIXEL()
 */
int eDIV_GET_PIXEL(FUNCTION_PARAMS)
{
	int x , y , bpp;
	Uint8 *p ;
	y = getparm() ;
	x = getparm() ;

	if ( x > fondo->w || y > fondo->h || x < 0 || y < 0 )
		return -1 ;

    bpp = fondo->format->BytesPerPixel;
    p = (Uint8 *)fondo->pixels + y * fondo->pitch + x * bpp;

    switch(bpp) {
    case 1:
		return *p;
    case 2:
		return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			return p[0] << 16 | p[1] << 8 | p[2];
		}else
		{
            return p[0] | p[1] << 8 | p[2] << 16;
		}
    case 4:
		return *(Uint32 *)p;
	}

	return 0;
}


/*! \brief Crea un nuevo mapa con las características indicadas.
 *
 * El nuevo mapa se creará en el fichero 0, como si se hubiera cargado con
 * \link eDIV_LOAD_MAP() load_map() \endlink.
 * @param w Anchura del mapa
 * @param h Altura del mapa
 * @param cx Coordenada X del centro del mapa
 * @param cy Coordenada Y del centro del mapa
 * @param c Color con el que se rellenará el nuevo mapa
 * @return El código del nuevo gráfico, o -1 si ocurrió un error
 * @see eDIV_LOAD_MAP(), eDIV_UNLOAD_MAP()
 */
int eDIV_NEW_MAP(FUNCTION_PARAMS)
{
	int i ;
	int w , h , cx , cy , c ;
	c = getparm() ;
	cy = getparm() ;
	cx = getparm() ;
	h = getparm() ;
	w = getparm() ;

	for ( i = 1000 ; i < files[0].num ; i++ )
	{
		if ( !files[0].mapa[i].existe )
		{
			files[0].mapa[i].Surface = SDL_CreateRGBSurface( SDL_HWSURFACE , w , h , screen->format->BitsPerPixel , 0xFF0000 , 0x00FF00 , 0x0000FF , 0x000000 ) ;
			files[0].mapa[i].existe = 1 ;
			files[0].mapa[i].cpoint[0].x = cx ;
			files[0].mapa[i].cpoint[0].y = cy ;
			SDL_FillRect( files[0].mapa[i].Surface , NULL , c ) ;
			SDL_SetColorKey( files[0].mapa[i].Surface , SDL_SRCCOLORKEY | SDL_RLEACCEL , color_transparente ) ;
			if ( i > last_map[0] )
				last_map[0] = i ;
			return i ;
		}
	}
	return -1 ;
}


/*! \brief Copia una región de la pantalla en un mapa
 * \todo La región copiada debe ESTIRARSE para adaptarse a la región destino
 * @param r Región de la pantalla
 * @param f Fichero del mapa destino
 * @param g Gráfico destino
 * @param x Coordenada X destino
 * @param y Coordenada Y destino
 * @param w Ancho de la región destino dentro del mapa
 * @param h Alto de la región destino dentro del mapa
 * @return 1
 * @see eDIV_DEFINE_REGION(), eDIV_MAP_BLOCK_COPY()
 */
int eDIV_SCREEN_COPY(FUNCTION_PARAMS)
{
	SDL_Rect srcrect , dstrect ;
	int r, f, g , x, y , w , h ;
	h = getparm() ;
	w = getparm() ;
	y = getparm() ;
	x = getparm() ;
	g = getparm() ;
	f = getparm() ;
	r = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;
    
	srcrect.x = 0 ;
	srcrect.y = 0 ;
	srcrect.w = w ;
	srcrect.h = h ;

	dstrect.x = x ;
	dstrect.y = y ;
	dstrect.w = 0 ;
	dstrect.h = 0 ;

	SDL_BlitSurface( screen, &srcrect , files[f].mapa[g].Surface , &dstrect ) ;
	return 1 ;
}

/*! \brief Obtiene las coordenadas de un punto de control de un mapa
 * @param f Fichero en el que se encuentra el mapa
 * @param g Código del mapa dentro del FPG
 * @param n Número del punto de control
 * @param offset_x Offset de la variable donde guardar la coordenada X
 * @param offset_y Offset de la variable donde guardar la coordenada Y
 * @return 0 si todo va bien, -1 si el mapa no existe
 * @see eDIV_GET_REAL_POINT()
 */
int eDIV_GET_POINT(FUNCTION_PARAMS)
{
	int f , g , n , dx , dy ;
	dy = getparm() ;
	dx = getparm() ;
	n = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;

	if(dx) fp->mem[ dx ] = files[f].mapa[g].cpoint[n].x ;
	if(dy) fp->mem[ dy ] = files[f].mapa[g].cpoint[n].y ;

	return 0 ;
}

/*! \brief Obtiene las coordenadas en pantalla de un punto de control del proceso actual
 * \todo Tener en cuenta angle, size, etc
 * @param n Número del punto de control
 * @param offset_x Offset de la variable donde guardar la coordenada X
 * @param offset_y Offset de la variable donde guardar la coordenada Y
 * @return 0 si todo va bien, -1 si el mapa no existe
 * @see eDIV_GET_POINT()
 */
int eDIV_GET_REAL_POINT(FUNCTION_PARAMS)
{
	int f , g , n , dx , dy , x , y , id;
	dy = getparm() ;
	dx = getparm() ;
	n = getparm() ;
	id = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id;
	f = local("file",id) ;
	g = local("graph",id) ;
	x = local("x",id) ;
	y = local("y",id) ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;

	if(dx) fp->mem[ dx ] = files[f].mapa[g].cpoint[n].x - files[f].mapa[g].cpoint[0].x + x ;
	if(dy) fp->mem[ dy ] = files[f].mapa[g].cpoint[n].y - files[f].mapa[g].cpoint[0].y + y ;

	return 0 ;
}

/*! \brief Obtiene información sobre un mapa
 * @param f Fichero donde se encuentra el mapa
 * @param g Código del gráfico dentro del FPG
 * @param i Indica qué información quiere extraerse del mapa, puede ser uno de:
 *			\a g_wide (0), \a g_height (1), \a g_x_center (2) ó \a g_y_center (3).
 * @return La información solicitada, o -1 si hubo un error.
 * @see eDIV_GET_POINT()
 */
int eDIV_GRAPHIC_INFO(FUNCTION_PARAMS)
{
	int f , g , i ;
	i = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;

	switch (i)
	{
		case 0:
			return files[f].mapa[g].Surface->w ;
		case 1:
			return files[f].mapa[g].Surface->h ;
		case 2:
			return files[f].mapa[g].cpoint[0].x ;
		case 3:
			return files[f].mapa[g].cpoint[0].y ;
	}
	return -1;
}
