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


#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>

struct _fuente_control_s {
	int offset_imagen ;
	int size_imagen ;
	int w , h , bytespp ;
	SDL_Rect rect[256] ;
	SDL_Surface *imagen ;
};

int main(int argc, char* argv[])
{
	FILE *fp, *out;
	char tras[3], none[3];
	char *pool, *px;
	struct _fuente_control_s fuente_control_s;
	SDL_Surface *imagen ;
	int i, x, y;
	fpos_t pos;
	
	out = fopen("stdout.txt", "w" );

	imagen = SDL_LoadBMP("fuente.bmp");
	SDL_LockSurface(imagen);
        
	// Se calculan los rects
	px = imagen->pixels ;


	// Color de borde ( El primero k se encuentra
	none[0] = *(px + 0);
	none[1] = *(px + 1);
	none[2] = *(px + 2);

	// Color transparente ( Fila 1, Columna1, corresponde al caracter 0 )
	tras[0] = *(px + imagen->pitch + 3 + 0);
	tras[1] = *(px + imagen->pitch + 3 + 1);
	tras[2] = *(px + imagen->pitch + 3 + 2);


	i = 1;
	x = 3;
	y = 0;
	for ( x = 3 ; x < imagen->w ; x++ )
	{
		for ( y = 0 ; y < imagen->h ; y++ )
		{
			if (*(px + imagen->pitch*y + x*3 + 0) != none[0] ||
				*(px + imagen->pitch*y + x*3 + 1) != none[1] ||
				*(px + imagen->pitch*y + x*3 + 2) != none[2] )
			{
				fuente_control_s.rect[i].x = x ;
				fuente_control_s.rect[i].y = y ;
				for ( ; y < imagen->h ; y++ )
				{
					if (*(px + imagen->pitch*y + x*3 + 0) == none[0] &&
						*(px + imagen->pitch*y + x*3 + 1) == none[1] &&
						*(px + imagen->pitch*y + x*3 + 2) == none[2] )
						break ;
				}
				y--;
				fuente_control_s.rect[i].h = y - fuente_control_s.rect[i].y + 1;

				for ( ; x < imagen->w ; x++ )
				{
					if (*(px + imagen->pitch*y + x*3 + 0) == none[0] &&
						*(px + imagen->pitch*y + x*3 + 1) == none[1] &&
						*(px + imagen->pitch*y + x*3 + 2) == none[2] )
						break ;
				}
				x--;
				fuente_control_s.rect[i].w = x - fuente_control_s.rect[i].x + 1;
				x++;
				
				fprintf(out, "%i:  %i,%i-%i,%i\n" , 
					i ,
					fuente_control_s.rect[i].x ,
					fuente_control_s.rect[i].y ,
					fuente_control_s.rect[i].w ,
					fuente_control_s.rect[i].h );
					
				i++;
				break ;
			}
		}
		if ( i > 255)
			break ;
	}

	fp = fopen("fuente.fnt" , "w" );

	fuente_control_s.offset_imagen = sizeof( fuente_control_s );
	fuente_control_s.size_imagen = imagen->w * imagen->h * imagen->format->BytesPerPixel;
	fuente_control_s.w = imagen->w ;
	fuente_control_s.h = imagen->h ;
	fuente_control_s.bytespp = imagen->format->BytesPerPixel;

	printf("Sizeof control: %i\nOffset imagen: %i\nSizeof imagen: %i" ,
		fuente_control_s.offset_imagen, fuente_control_s.offset_imagen ,
		fuente_control_s.size_imagen );
	getchar();

	if(!(pool = malloc(fuente_control_s.size_imagen + 1) ) )
		printf("ERROR: Memoria insuficiente\n");

	for ( x = 0 ; x < imagen->w ; x++ )
		for ( y = 0 ; y < imagen->h ; y++ )
			for ( i = 0 ; i < imagen->format->BytesPerPixel ; i++ )
				pool[ ( x + y * imagen->w ) * imagen->format->BytesPerPixel + i ] = *(px + ( y * imagen->pitch ) + ( x * imagen->format->BytesPerPixel ) + i );
	
	SDL_UnlockSurface( imagen );


	printf("\nescribe: %i" , fwrite( &fuente_control_s , 1 , sizeof(fuente_control_s) , fp ) );

	pos = fuente_control_s.offset_imagen;
	fsetpos(fp , &pos );

	printf( "\n%i\n" , ftell( fp ) );
	getchar();
	

	fwrite( pool , 1 , fuente_control_s.size_imagen , fp );

	free(pool);

	exit (EXIT_SUCCESS);
}


