/* Fenix - Compilador/intérprete de videojuegos
 * Copyright (C) 1999 José Luis Cebrián Pagüe
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

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <png.h>

#include "fxi.h"

extern SDL_Surface * screen ;

/* --------------------------------- */
/* Rutina de lectura de gráficos PNG */
/* --------------------------------- */

static file * png ;

static void user_read_data (png_structp png_ptr, 
		png_bytep data, png_size_t length)
{
	file_read (png, data, length) ;
}

GRAPH * gr_read_png (const char * filename)
{
	GRAPH * bitmap ;
	unsigned int n, x ;
	Uint16 * ptr ;
	Uint32 * orig ;
	Uint32 row[2048] ;
	Uint32 Rshift, Gshift, Bshift ;
	Uint32 Rmask, Gmask, Bmask ;

	png_bytep	rowpointers[2048] ;

	png_structp	png_ptr ;
	png_infop	info_ptr, end_info ;
	png_uint_32    	width, height, rowbytes;
	int		depth, color ;

	/* Abre el fichero y se asegura de que screen está inicializada */

	png = file_open (filename, "rb") ;
//	if (!png) gr_error ("No existe %s\n", filename) ;

	/* Prepara las estructuras internas */

	png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0) ;
	info_ptr = png_create_info_struct (png_ptr) ;
	end_info = png_create_info_struct (png_ptr) ;
//	if (!info_ptr || !end_info) gr_error ("Error al cargar PNG") ;

	/* Rutina de error */

	if (setjmp (png_ptr->jmpbuf))
	{
		png_destroy_read_struct (&png_ptr, &info_ptr, &end_info) ;
		file_close (png) ;
		return 0 ;
	}

	/* Recupera información sobre el PNG */

	png_set_read_fn (png_ptr, 0, user_read_data) ;
	png_read_info (png_ptr, info_ptr) ;
	png_get_IHDR (png_ptr, info_ptr, &width, &height, &depth, &color, 0, 0 , 0) ;

//	if (color == PNG_COLOR_TYPE_GRAY || color == PNG_COLOR_TYPE_GRAY_ALPHA)
//		gr_error ("No se soportan PNG en escala de grises") ;
//	if (height > 2048 || width > 2048) 
//		gr_error ("PNG demasiado grande") ;

	/* Configura los distintos modos disponibles */

	if (depth < 8) 
		png_set_expand(png_ptr) ;
	if (depth == 16) 
		png_set_strip_16(png_ptr) ;
	if (color == PNG_COLOR_TYPE_RGB) 
		png_set_filler (png_ptr, 0xFF, PNG_FILLER_AFTER) ;
	png_set_bgr(png_ptr) ;

	/* Recupera el fichero, convirtiendo a 16 bits si es preciso */

	rowbytes = png_get_rowbytes (png_ptr, info_ptr) ;
	bitmap = bitmap_new (0, width, height, color == PNG_COLOR_TYPE_PALETTE ? 8 : 16) ;
//	if (!bitmap) gr_error ("Error al cargar PNG") ;
	if (color == PNG_COLOR_TYPE_PALETTE)
	{
		for (n = 0 ; n < height ; n++)
			rowpointers[n] = ((Uint8*)bitmap->data) + n*bitmap->width ;
		png_read_image (png_ptr, rowpointers) ;
	}
	else
	{
		ptr = (Uint16*) bitmap->data ;
		Bshift = 16-screen->format->Rshift ;
		Gshift =  8-screen->format->Gshift ;
		Rshift =  0-screen->format->Bshift ;

		Bmask  = 0xFF0000 ;
		Gmask  = 0x00FF00 ;
		Rmask  = 0x0000FF ;

		for (n = 0 ; n < screen->format->Rloss ; n++) Rshift++, Rmask <<= 1 ;
		for (n = 0 ; n < screen->format->Gloss ; n++) Gshift++, Gmask <<= 1 ;
		for (n = 0 ; n < screen->format->Bloss ; n++) Bshift++, Bmask <<= 1 ;

		Bmask &= 0xFF0000 ;
		Gmask &= 0x00FF00 ;
		Rmask &= 0x0000FF ;

		for (n = 0 ; n < height ; n++)
		{
			rowpointers[0] = (void *)row ;
			png_read_rows (png_ptr, rowpointers, 0, 1) ;

			orig = row ;
			for (x = 0 ; x < width ; x++)
			{
				if ((*orig) & 0x80000000) 
				{
				  *ptr = ((*orig & Rmask) >> Rshift)|
					 ((*orig & Gmask) >> Gshift)|
					 ((*orig & Bmask) >> Bshift)  ;
				  if (!*ptr) (*ptr)++ ;
				}
				else *ptr = 0 ;
				ptr++, orig++ ;
			}
		}
	}

	/* Fin */

	png_read_end (png_ptr, 0) ;
	file_close (png) ;
	bitmap->modified = 1 ;
	return bitmap ;
}

