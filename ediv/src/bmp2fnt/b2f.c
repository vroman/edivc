/* 
 * eDiv Compiler
 * Copyleft (C) 2000-2002 Sion Entertainment
 * http://www.sion-e.com
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


#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <SDL/SDL.h>
#include "edivfont.h"


int main(int argc, char* argv[])
{
	FILE *fp ;
	int im ;
	FILE *img ;
	FILE *out ;
	char* pool;
	struct _fuente_control_s fuente_control_s ;
	char tras[3] ;
	char none[3] ;
	SDL_Surface *imagen ;
	int i , x , y ;
	char *px ;
	fpos_t pos ;
	
	out = fopen("stdout.txt", "w" ) ;

/*
	fuente_control_s.rect['A'].x = 220 ;
	fuente_control_s.rect['A'].y = 3 ;
	fuente_control_s.rect['A'].w = 7 ;
	fuente_control_s.rect['A'].h = 7 ;
*/

	imagen = SDL_LoadBMP("fuente.bmp") ;
	SDL_LockSurface( imagen );
        
	// Se calculan los rects
	px = imagen->pixels ;


	// Color de borde ( El primero k se encuentra
	none[0] = *(px + 0) ;
	none[1] = *(px + 1) ;
	none[2] = *(px + 2) ;

	// Colo trasparente ( Fila 1, Columna1, corresponde al caracter 0 )
	tras[0] = *(px + imagen->pitch + 3 + 0) ;
	tras[1] = *(px + imagen->pitch + 3 + 1) ;
	tras[2] = *(px + imagen->pitch + 3 + 2) ;


	i = 1 ;
	x = 3 ;
	y = 0 ;
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
				fuente_control_s.rect[i].w = x - fuente_control_s.rect[i].x + 1 ;
				x++;
				
				fprintf(out, "%i:  %i,%i-%i,%i\n" , 
					i ,
					fuente_control_s.rect[i].x ,
					fuente_control_s.rect[i].y ,
					fuente_control_s.rect[i].w ,
					fuente_control_s.rect[i].h ) ;
					
				i++;
				break ;
			}
		}
		if ( i > 255)
			break ;
	}

	//SDL_UnlockSurface( imagen );
	//SDL_FreeSurface( imagen ) ;

	//im = open( "fuente.bmp" , O_RDONLY | O_BINARY ) ; 
	fp = fopen("fuente.fnt" , "w" ) ;

	fuente_control_s.offset_imagen = sizeof( fuente_control_s ) ;
	fuente_control_s.size_imagen = imagen->w * imagen->h * imagen->format->BytesPerPixel ;
	fuente_control_s.w = imagen->w ;
	fuente_control_s.h = imagen->h ;
	fuente_control_s.bytespp = imagen->format->BytesPerPixel ;

	printf("Sizeof control: %i\nOffset imagen: %i\nSizeof imagen: %i" ,
		fuente_control_s.offset_imagen, fuente_control_s.offset_imagen ,
		fuente_control_s.size_imagen ) ;
	getchar();

	//close(im) ;
	//img = fopen("fuente.bmp" , "r" ) ;

	if(!(pool = malloc(fuente_control_s.size_imagen + 1) ) )
		printf("ERROR: Memoria insuficiente\n");

	// generamos la imagen a guardar
	for ( x = 0 ; x < imagen->w ; x++ )
		for ( y = 0 ; y < imagen->h ; y++ )
			for ( i = 0 ; i < imagen->format->BytesPerPixel ; i++ )
				pool[ ( x + y * imagen->w ) * imagen->format->BytesPerPixel + i ] = *(px + ( y * imagen->pitch ) + ( x * imagen->format->BytesPerPixel ) + i ) ;
	
	SDL_UnlockSurface( imagen );


	//fread( pool , 1 , fuente_control_s.size_imagen , img ) ;
	printf("\nescribe: %i" , fwrite( &fuente_control_s , 1 , sizeof(fuente_control_s) , fp ) );
	//fwrite( &fuente_control_s , 1 , fuente_control_s.offset_imagen , fp ) ;

	pos = fuente_control_s.offset_imagen ;
	fsetpos(fp , &pos ) ;

	//fwrite( &fuente_control_s , 1 , 2060 , fp ) ;
		printf( "\n%i\n" , ftell( fp ) ) ;
	getchar() ;
	

	fwrite( pool , 1 , fuente_control_s.size_imagen , fp ) ;

	free(pool) ;

	return 0 ;
}


