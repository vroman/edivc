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

/*
 * Este fichero contien el codigo del Kernel del eDiv
 *   by Riseven
 *  
 */

#include <stdio.h>
#include <string.h>

#ifdef WIN32
	#include <windows.h>
#endif

#include <SDL/SDL.h>

#include "edivfont.h"
#include "kernel.h"
#include "inte.h"
#include "extern.h"
#include "main.h"



struct _k_error_s k_error_s[50] ;


int ini_kernel()
{
	//FILE *fp ; // AHORA FP ES OTRA COSA MAS IMPORTANTE XDDD
	FILE *im ;
	char *pool ;
	fpos_t pos ;


	// EDIV NOID
/*
	fp = fopen("fuente.fnt", "r" ) ;
	im = fopen("tempo.bmp", "w" ) ;
	printf( "\n%i\n" , ftell( fp ) ) ;
	printf( "\nsizeof control:%i\n" , sizeof(fuente_control_s[0]) ) ;
	//printf( "leido:%i\n" , (int)fread( &fuente_control_s[0] , 1 , sizeof(fuente_control_s[0]) , fp ) ) ;
	printf( "leido:%i\n" , (int)fread( &fuente_control_s[0] , 1 , 1393 , fp ) ) ;
	printf( "Sizeof imagen: %i" , fuente_control_s[0].size_imagen ) ;
	if (!(pool = malloc(fuente_control_s[0].size_imagen + 1 ) ) )
		printf("Error: Memoria insuficiente" ) ;
	
	pos = fuente_control_s[0].offset_imagen + 10;
	fsetpos(fp , &pos ) ;
	printf( "\n%i\n%i\n" , ftell( fp ), fuente_control_s[0].offset_imagen ) ;
	printf( "leido:%i\n" ,	fread( pool , 1 , fuente_control_s[0].size_imagen , fp ) ) ;
	
	//fwrite( &fuente_control_s , 1 , 2060 , fp ) ;
	printf( "\n%i\n%i\n" , ftell( fp ), fuente_control_s[0].size_imagen ) ;
	getchar() ;

	fwrite( pool , 1 , fuente_control_s[0].size_imagen , im ) ;
	free( pool ) ;
	fclose( im ) ;
	fclose(fp) ;
	
	// Sacamos la fuente
	fuente_control_s[0].imagen = SDL_LoadBMP("tempo.bmp") ;

	// Escribimos A
	
	textos_s[0].cadena = SDL_CreateRGBSurface(SDL_HWSURFACE, fuente_control_s[0].rect[173].w, fuente_control_s[0].rect[173].h, 24, 0xFF0000, 0x00FF00, 0x0000FF, 0x000000);
	SDL_BlitSurface( fuente_control_s[0].imagen , &fuente_control_s[0].rect[173] , textos_s[0].cadena , NULL ) ;
	textos_s[0].x = 20 ;
	textos_s[0].y = 20 ;
	textos_s[0].existe = 1 ;


	imgs[0] = SDL_LoadBMP( "BG.bmp" ) ;
	if ( imgs[0] == NULL )
		exit(0);
	imgs[1] = SDL_LoadBMP( "bola.bmp" ) ;
	if ( imgs[1] == NULL )
		exit(0);
	imgs[2] = SDL_LoadBMP( "barra.bmp" ) ;
	if ( imgs[2] == NULL )
		exit(0);
*/

/*
	//Cargamos bola.bmp PROVISIONAL mientras se hace Load_BMP()
	imgs[0] = SDL_LoadBMP( "bola.bmp" ) ;
	if ( imgs[0] == NULL )
		exit(0) ;
*/	


	strcpy( k_error_s[K_ERR_SDL_INIT].cadena , "NO SE PUDO INICIALIZAR LA SDL" );
	k_error_s[K_ERR_SDL_INIT].nivel = 1000 ;
	strcpy( k_error_s[K_ERR_SDL_SET_VIDEO_MODE].cadena , "NO SE PUDO ESTABLECER EL MODO GRAFICO" );
	k_error_s[K_ERR_SDL_SET_VIDEO_MODE].nivel = 900 ;
	strcpy( k_error_s[K_ERR_KERNEL].cadena , "ERROR DURANTE LA EJECUCION DEL KERNEL" );
	k_error_s[K_ERR_KERNEL].nivel = 400 ;
	strcpy( k_error_s[K_ERR_INTERPRETE].cadena , "ERROR DURANTE LA EJECUCION DEL INTERPRETE" );
	k_error_s[K_ERR_INTERPRETE].nivel = 1000 ;
	strcpy( k_error_s[K_ERR_INI_INTERPRETE].cadena , "ERROR DURANTE LA INICIALIZACION DEL INTERPRETE" );
	k_error_s[K_ERR_INI_INTERPRETE].nivel = 1000 ;
	
	return 0;
}


// KERNEL
// Se encarga de los procesos internos del stub
//   by Riseven

int kernel()
{
	int i ;
//	SDL_Rect dst_rect;
//	SDL_Rect src_rect ;

/*
	// Para el testeo
	for ( i = 0 ; i < num_proc_orden ; i++ )
	{
		lista_draw_proc_s[0].imagen = imgs[0] ;
		lista_draw_proc_s[0].x = mem[procs_s[proc_orden[i]].id + _X -1 ] ;
		lista_draw_proc_s[0].y = mem[procs_s[proc_orden[i]].id + _Y -1 ] ;
	}
	
	num_draw_proc = i ;

	// Se dibujan los graficos que proceden de procesos
	for ( i = 0 ; i < num_draw_proc ; i++ )
	{
		dst_rect.x = lista_draw_proc_s[i].x ;
		dst_rect.y = lista_draw_proc_s[i].y ;
		dst_rect.w = lista_draw_proc_s[i].imagen->w ;
		dst_rect.h = lista_draw_proc_s[i].imagen->h ;
		SDL_BlitSurface( lista_draw_proc_s[i].imagen , NULL , screen , &dst_rect ) ;
	}
*/
/*

	// Se escriben los textos ( ESTO SE TENDRA K MEZCLAR CON LO ANTERIOR
	for ( i = 0 ; i < 256 ; i++ )
	{
		if ( textos_s[i].existe == 1 )
		{
			dst_rect.x = textos_s[i].x ;
			dst_rect.y = textos_s[i].y ;
			dst_rect.w = textos_s[i].cadena->w ;
			dst_rect.h = textos_s[i].cadena->h ;
			SDL_BlitSurface( textos_s[i].cadena , NULL , screen , &dst_rect ) ;
		}
	}
*/
	

                    
	//SDL_Flip(screen) ;

	return 1 ;
}

void k_error( int codigo )
{
	fprintf(stdout, k_error_s[codigo].cadena ) ;

	if ( k_error_s[codigo].nivel >= K_ERROR_EXIT )
		exit(0) ;

}

