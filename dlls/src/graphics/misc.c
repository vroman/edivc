/*! \file misc.c
 * \brief Funciones varias
 */

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"
#include "default_palette.h"

/*! \brief Cambia el modo de la pantalla
 * \todo ¿más flags?
 *
 * Esta función permite distintas sintaxis:
 *	- Sintaxis de DIV2:
 *		@param modo Modo de pantalla indicado por una constante tipo mWWWxHHH ó por un identificador de video_modes[]
 *
 *	- Sintaxis de eDIV:
 *		@param anchura Ancho de la nueva resolución
 *		@param altura Alto de la nueva resolución
 *		@param [bpp] Profundidad de color, puede ser 8, 16, 24 ó 32 (por defecto es 8)
 *		@param [flags] Flags de la ventana del juego, puede ser una combinación de:
 *						  _fullscreen (1), ... ya veremos si añadimos más xD
 * .
 *
 * @return 0
 */
int eDIV_SET_MODE(FUNCTION_PARAMS)
{
	int modo;

	switch(fp->num_params) {
		case 4:
			fp->graphics->flags=getparm();
		case 3:
			fp->graphics->bpp=getparm();
			fp->graphics->alto=getparm();
			fp->graphics->ancho=getparm();
			break;
		case 1:
			modo=getparm();
			if(modo>1280960) {
				fp->graphics->ancho=modo/10000;
				fp->graphics->alto=modo%10000;
			}
			else {
				fp->graphics->ancho=modo/1000;
				fp->graphics->alto=modo%1000;
			}
			fp->graphics->bpp=8;
			fp->graphics->flags=0;
	}
	
	/* Esto avisa a las DLLs */
	fp->graphics->resflags|=GR_CHANGED;

	SDL_FreeSurface(fondo);
	SDL_FreeSurface(screen);

	screen=SDL_SetVideoMode(fp->graphics->ancho,fp->graphics->alto,fp->graphics->bpp,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_HWACCEL|((fp->graphics->flags&GR_FULLSCREEN)?SDL_FULLSCREEN:0)|((fp->graphics->bpp==8)?SDL_HWPALETTE:0));
	fp->graphics->buffer=screen->pixels;

	if(fp->graphics->bpp==8)
		SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL,(SDL_Color*)default_palette,0,256);

	if ( screen == NULL ) {
		fp->Critical_Error(7); /* No se pudo inicializar SDL */
		return 0;
	}

	fondo=SDL_CreateRGBSurface(SDL_HWSURFACE,fp->graphics->ancho,fp->graphics->alto,fp->graphics->bpp,0,0,0,0);
	fp->graphics->background=fondo->pixels;

	if(fp->graphics->bpp==8)
		PaletteCopy(fondo,screen);

	regions[0].w=fp->graphics->ancho;
	regions[0].h=fp->graphics->alto;

	return 0;
}
