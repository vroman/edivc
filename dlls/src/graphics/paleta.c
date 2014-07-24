/*! \file paleta.c
 * \brief Funciones relacionadas con el manejo de la paleta y los colores
 */

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"

/*! \brief Cambia el color transparente (colorkey) actual
 * @param a Color que desea usarse como transparente.
 * @return El antiguo color transparente.
 * @see color_transparente
 */
int eDIV_SET_TRANSPARENT_COLOR(FUNCTION_PARAMS)
{
	int b , i;
	int a = getparm() ;
	b = color_transparente ;
	color_transparente = a ;
	for ( i = 1 ; i <= last_map[0] ; i++ )
	{
		if ( files[0].mapa[i].existe )
			SDL_SetColorKey( files[0].mapa[i].Surface , SDL_SRCCOLORKEY | SDL_RLEACCEL  , color_transparente ) ;
	}
	return b ;
}

/*! \brief Obtiene el color transparente actual 
 * @return El color transparente.
 * @see color_transparente
 */
int eDIV_GET_TRANSPARENT_COLOR(FUNCTION_PARAMS)
{
	return color_transparente ;
}

/*! \brief Crea un color a partir de las componentes RGB dadas.
 *
 * Esta función devuelve el color más próximo posible a las componentes RGB
 * dadas. En modos de color paletizado (8 bits) puede devolver un color muy
 * distinto al esperado si no se encuentra otro mejor en la paleta.
 * Esta función difiere de \link eDIV_FIND_COLOR() find_color() \endlink en
 * que recibe los parámetros en un rango de 0..255 en vez de 0..63.
 * @param r Componente roja (red)
 * @param g Componente verde (green)
 * @param b Componente azul (blue)
 * @return El color generado.
 * @see eDIV_FIND_COLOR()
 */
int eDIV_RGB(FUNCTION_PARAMS)
{
	int r,g,b ;
	b = getparm() ;
	g = getparm() ;
	r = getparm() ;

	return SDL_MapRGB(screen->format,r,g,b);
}

/*! \brief Crea un color a partir de las componentes RGB dadas.
 *
 * Esta función devuelve el color más próximo posible a las componentes RGB
 * dadas. En modos de color paletizado (8 bits) puede devolver un color muy
 * distinto al esperado si no se encuentra otro mejor en la paleta.
 * Esta función difiere de \link eDIV_RGB() RGB() \endlink en que recibe los
 * parámetros en un rango de 0..63 en vez de 0..255. Se ha incluido para
 * mantener la compatibilidad con DIV2.
 * @param r Componente roja (red)
 * @param g Componente verde (green)
 * @param b Componente azul (blue)
 * @return El color generado.
 * @see eDIV_RGB()
 */
int eDIV_FIND_COLOR(FUNCTION_PARAMS)
{
	int r,g,b ;
	b = getparm() ;
	g = getparm() ;
	r = getparm() ;

	return SDL_MapRGB(screen->format,r*4,g*4,b*4);
}

/*! \brief Obtiene las componentes RGB de un color dado.
 *
 * Esta función recibe un color y almacena en las variables indicadas sus
 * componentes RGB (siempre en un rango de 0..255). Si en lugar de un offset
 * se recibe un 0, se ignorará esa componente.
 * @param color Color del que se quiere obtener la información
 * @param offset_r Offset de la variable donde se quiere guardar el valor de la componente roja (red)
 * @param offset_g Offset de la variable donde se quiere guardar el valor de la componente verde (green)
 * @param offset_b Offset de la variable donde se quiere guardar el valor de la componente azul (blue)
 * @return 1
 * @see eDIV_SET_COLOR()
 */
int eDIV_GET_RGB(FUNCTION_PARAMS)
{
	unsigned int color,roff,goff,boff;
	unsigned char r,g,b;
	boff=getparm();
	goff=getparm();
	roff=getparm();
	color=getparm();
	SDL_GetRGB(color,screen->format,&r,&g,&b);
	if(roff) fp->mem[roff]=(int)r;
	if(goff) fp->mem[goff]=(int)g;
	if(boff) fp->mem[boff]=(int)b;
	return 1;
}

/*! \brief Hace un fundido de pantalla
 *
 * \todo ¡¡función sin acabar!!
 */
int eDIV_FADE(FUNCTION_PARAMS)
{
	int r , g , b , v ;
	v = getparm() ;
	b = getparm() ;
	g = getparm() ;
	r = getparm() ;

	return SDL_SetGamma( 1.0f , 1.0f , 1.0f ) ;
	return 1 ;
}
