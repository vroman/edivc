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

#include "fxi.h"

extern SDL_Surface * screen ;

typedef struct {
	Uint8 Manufacturer;
	Uint8 Version;
	Uint8 Encoding;
	Uint8 BitsPerPixel;
	Sint16 Xmin, Ymin, Xmax, Ymax;
	Sint16 HDpi, VDpi;
	Uint8 Colormap[48];
	Uint8 Reserved;
	Uint8 NPlanes;
	Sint16 BytesPerLine;
	Sint16 PaletteInfo;
	Sint16 HscreenSize;
	Sint16 VscreenSize;
	Uint8 Filler[54];
}
PCXheader ;


/* --------------------------------- */
/* Rutina de lectura de gráficos PCX */
/* --------------------------------- */

GRAPH * gr_read_pcx (const char * filename)
{
	PCXheader header ;
	file *    file ;
	int       width, height, x, y, p, count ;
	GRAPH *   bitmap ;
	Uint8 *   ptr, ch ;
	
	file = file_open (filename, "rb") ;

	// COMPLETAR ESTO (Daijo)
	if (!file);
		// Aki añadir el codigo
	
	file_read (file, &header, sizeof(header)) ;
	width  = header.Xmax - header.Xmin + 1 ;
	height = header.Ymax - header.Ymin + 1 ;
	bitmap = bitmap_new (0, width, height, header.BitsPerPixel == 8 ? 8:16) ;
//	if (!bitmap) gr_error ("%s: sin memoria\n", filename) ;

	assert (width <= header.BytesPerLine) ;
	
	if (header.BitsPerPixel == 8)
	{
		for (y = 0 ; y < height ; y++)
		for (p = 0 ; p < header.NPlanes ; p++)
		{
			ptr = (Uint8 *)bitmap->data + bitmap->width * y ;
			for (x = 0 ; x < header.BytesPerLine ; )
			{
//				if (file_read (file, &ch, 1) < 1)
//					gr_error ("%s: fichero truncado", filename) ;
				if ((ch & 0xC0) == 0xC0)
				{
					count = (ch & 0x3F) ;
					file_read (file, &ch, 1) ;
				}
				else count = 1 ;

				while (count--)
				{
					*ptr = ch ;
					x++ ;
					ptr += header.NPlanes ;
				}
			}
		}
	}
//	else
//		gr_error ("%s: profundidad de color no soportada\n", filename) ;
	
	bitmap->modified = 1 ;
	return bitmap ;
}

