/* 
 * eDiv Compiler
 * Copyleft (C) 2000-2003 Sion Ltd.
 * Copyleft (C) 2005 eDIV Team
 * http://ediv.divsite.net
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  -Estructura de control
      contiene informacion sobre el offset y tamaño del bmp en el fichero .fnt
	  para poder leerlo, tambien contiene el offset de la estructura de informacion
  -Mapa de caracteres
      es la imagen del mapa de caracteres, se copia directamente.
*/

#ifndef __EDIV_EDIVFONT_H_
#define __EDIV_EDIVFONT_H_

#include <SDL/SDL.h>

struct _fuente_control_s{
	int offset_imagen ;
	int size_imagen ;
	int w , h , bytespp ;
	SDL_Rect rect[256] ;
	SDL_Surface *imagen ;
};

#endif