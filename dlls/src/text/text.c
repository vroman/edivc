#include <SDL/SDL.h>

#include <export.h>

#include "edivfont.h"
#include "text.h"


#define MAX_FONTS	0xFF
#define MAX_WRITES	0xFFF

struct _fuente_control_s fuente_control_s[MAX_FONTS];
int existe[MAX_FONTS] ;

struct
{
	int existe ;
	int numerica ;   /* 0: texto , 1: numerica (write_int) */
	int offset_var ; /* solo para numerica = 1             */
	int centrado ;   /* solo para numerica = 1             */
	int fuente ;     /* solo para numerica = 1             */
	int x , y ;
	int cx , cy ;
	SDL_Surface *imagen ;
}textos[MAX_WRITES] ;


int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{
	CONST("all_text",0);

	FUNCTION("load_fnt",1,eDiv_LoadFnt) ;
	FUNCTION("write",5,eDiv_Write) ;
	FUNCTION("write_int",5,eDiv_WriteInt) ;
	FUNCTION("move_text",3,eDiv_MoveText) ;
	FUNCTION("delete_text",1,eDiv_DeleteText) ;
	ENTRYPOINT( frame ) ;
	ENTRYPOINT(first_load) ;
	
	return TRUE ;
}

FILE *fichero ;

int eDiv_LoadFnt(FUNCTION_PARAMS)
{
	FILE *fuente ;
	char *pool ;
	int i ;
	fpos_t pos ;
	const char *filename=getstrparm(); /* Fichero a cargar */

	for ( i = 1 ; i <= MAX_FONTS ; i++ )
	{
		if ( existe[i] == 0 )
			break ;
	}
	if ( i == MAX_FONTS ) {
		fp->Runtime_Error(113);	/* Demasiadas fuentes */
		return -1 ;
	}

	fichero = fopen("text.txt" , "w+" ) ;
	if((fuente = fopen(filename, "rb" ))==NULL) {
		fclose(fichero);
		fp->Runtime_Error(114);	/* archivo de fuente no encontrado */
		return 0;
	}
	fprintf( fichero , "Leidos %d bytes\n" , (int)fread( &fuente_control_s[i] , 1 , sizeof(fuente_control_s[i]) , fuente ) ) ;
	if (!(pool = malloc(fuente_control_s[i].size_imagen + 1 ) ) ) {
		fp->Runtime_Error(100); /* memoria insuficiente */
		return 0;
	}
    pos = fuente_control_s[i].offset_imagen ;
	fsetpos(fuente , &pos ) ;
	fprintf(fichero , "Size of image: %d bytes\n"  , fuente_control_s[i].size_imagen ) ;
	fprintf(fichero , "Leidos %d bytes\n" , (int)fread( pool , 1 , fuente_control_s[i].size_imagen , fuente ) ) ;
	fuente_control_s[i].imagen = SDL_CreateRGBSurfaceFrom( pool , fuente_control_s[i].w , fuente_control_s[i].h , fuente_control_s[i].bytespp * 8 , fuente_control_s[i].w * fuente_control_s[i].bytespp , 0 , 0 , 0, 0 ) ;

	fclose(fuente) ; 
	fclose(fichero) ;

	existe[i] = 1 ;
	return i ;
}


int eDiv_Write(FUNCTION_PARAMS)
{
	int i , j ;
	int ax ;
	SDL_Rect dstrect ;
	int fuente , x , y , centrado ;
	int w , h ;
	char *texto ;
	texto = getstrparm() ;
	centrado = getparm() ;
	y = getparm() ;
	x = getparm() ;
	fuente = getparm() ;

	if(fuente<0 || fuente>MAX_FONTS || !existe[fuente]) {
		fp->Runtime_Error(116);	/* id de fuente no válido */
		return -1;
	}

	for ( i = 1 ; i < MAX_WRITES ; i++ )
	{
		if ( textos[i].existe == 0 )
			break ;
	}
	if ( i == MAX_WRITES ) {
		fp->Runtime_Error(118);	/* demasiados textos */
		return -1 ;
	}


	textos[i].x = x ;
	textos[i].y = y ;
	textos[i].numerica = 0 ;
	textos[i].offset_var = 0 ;
	textos[i].centrado = 0 ;
	textos[i].fuente = 0 ;


	/* Calculamos el ancho del fichero */
	w = 0 ;
	for ( j = 0 ; (unsigned)j < strlen(texto) ; j++ )
		w += fuente_control_s[fuente].rect[ texto[j] ].w +1;
	h = fuente_control_s[fuente].imagen->h ;

	textos[i].imagen = SDL_CreateRGBSurface(SDL_HWSURFACE, w , h , fuente_control_s[fuente].bytespp * 8 , 0 , 0 , 0 , 0 ) ;
	SDL_SetColorKey( textos[i].imagen , SDL_SRCCOLORKEY | SDL_RLEACCEL , 0 ) ;


	/* Escribimos */
	ax = 0 ; /* Valor de x acumulativo mientras se va escribiendo */
	for ( j = 0 ; (unsigned)j < strlen(texto) ; j++ )
	{
		dstrect.x = ax ;
		dstrect.y = fuente_control_s[fuente].rect[ texto[j] ].y ;
		dstrect.w = fuente_control_s[fuente].rect[ texto[j] ].w ;
		dstrect.h = fuente_control_s[fuente].rect[ texto[j] ].h ;
		SDL_BlitSurface( fuente_control_s[fuente].imagen , &fuente_control_s[fuente].rect[ texto[j] ] , textos[i].imagen , &dstrect ) ;
		ax += fuente_control_s[fuente].rect[ texto[j] ].w +1  ;
	}

	/* Centrado */
	if ( centrado == 0 || centrado == 3 || centrado == 6 )
		textos[i].cx = 0 ;
	if ( centrado == 1 || centrado == 4 || centrado == 7 )
		textos[i].cx = (int) textos[i].imagen->w / 2 ;
	if ( centrado == 2 || centrado == 5 || centrado == 8 )
		textos[i].cx = textos[i].imagen->w -1 ;
	if ( centrado > -1 && centrado < 3 )
		textos[i].cy = 0 ;
	if ( centrado > 2 && centrado < 6 )
		textos[i].cy = (int) textos[i].imagen->h / 2 ;
	if ( centrado > 5 && centrado < 9 )
		textos[i].cy = textos[i].imagen->h -1 ;


	textos[i].existe = 1 ;

	return i ;
}

int eDiv_WriteInt(FUNCTION_PARAMS)
{
	int i , j ;
	int w , h , ax ;
	SDL_Rect dstrect ;
	int fuente , x , y , centrado , offset_var ;
	char *texto ;
	char texto2[30] ;
	offset_var = getparm() ;
	centrado = getparm() ;
	y = getparm() ;
	x = getparm() ;
	fuente = getparm() ;

	if(fuente<0 || fuente>MAX_FONTS || !existe[fuente]) {
		fp->Runtime_Error(116);	/* id de fuente no válido */
		return -1;
	}

	for ( i = 1 ; i < MAX_WRITES ; i++ )
	{
		if ( textos[i].existe == 0 )
			break ;
	}
	if ( i == MAX_WRITES ) {
		fp->Runtime_Error(118);	/* demasiados textos */
		return -1 ;
	}

	textos[i].x = x ;
	textos[i].y = y ;
	textos[i].numerica = 1 ;
	textos[i].offset_var = offset_var ;
	textos[i].centrado = centrado ;
	textos[i].fuente = fuente ;


	fichero = fopen("wrin.txt" , "w+") ;
	fprintf( fichero , "valor: %i\n" , global("fps") ) ;
	fclose(fichero) ;

	itoa( fp->mem[ textos[i].offset_var ] , texto2 , 10 ) ;
	texto = texto2 ;

	
	/* Calculamos el ancho del fichero */
	w = 0 ;
	for ( j = 0 ; (unsigned)j < strlen(texto) ; j++ )
		w += fuente_control_s[fuente].rect[ texto[j] ].w +1;
	h = fuente_control_s[fuente].imagen->h ;

	textos[i].imagen = SDL_CreateRGBSurface(SDL_HWSURFACE, w , h , fuente_control_s[fuente].bytespp * 8 , 0 , 0 , 0 , 0 ) ;
	SDL_SetColorKey( textos[i].imagen , SDL_SRCCOLORKEY | SDL_RLEACCEL , 0 ) ;

	/* Escribimos */
	ax = 0 ; /* Valor de x acumulativo mientras se va escribiendo */
	for ( j = 0 ; (unsigned)j < strlen(texto) ; j++ )
	{
		dstrect.x = ax ;
		dstrect.y = fuente_control_s[fuente].rect[ texto[j] ].y ;
		dstrect.w = fuente_control_s[fuente].rect[ texto[j] ].w ;
		dstrect.h = fuente_control_s[fuente].rect[ texto[j] ].h ;
		SDL_BlitSurface( fuente_control_s[fuente].imagen , &fuente_control_s[fuente].rect[ texto[j] ] , textos[i].imagen , &dstrect ) ;
		ax += fuente_control_s[fuente].rect[ texto[j] ].w +1  ;
	}

	/* Centrado */
	if ( centrado == 0 || centrado == 3 || centrado == 6 )
		textos[i].cx = 0 ;
	if ( centrado == 1 || centrado == 4 || centrado == 7 )
		textos[i].cx = (int) textos[i].imagen->w / 2 ;
	if ( centrado == 2 || centrado == 5 || centrado == 8 )
		textos[i].cx = textos[i].imagen->w -1 ;
	if ( centrado > -1 && centrado < 3 )
		textos[i].cy = 0 ;
	if ( centrado > 2 && centrado < 6 )
		textos[i].cy = (int) textos[i].imagen->h / 2 ;
	if ( centrado > 5 && centrado < 9 )
		textos[i].cy = textos[i].imagen->h -1 ;



	textos[i].existe = 1 ;

	return i ;
}

int eDiv_MoveText(FUNCTION_PARAMS)
{
	int i , x , y ;
	y = getparm() ;
	x = getparm() ;
	i = getparm() ;

	textos[i].x = x ;
	textos[i].y = y ;

	return 1 ;
}

int eDiv_DeleteText(FUNCTION_PARAMS)
{
	int i;
	i = getparm();

	if (i==0) {
		for (i=1;i<MAX_WRITES;i++) {
			if(textos[i].existe) {
				/* Por que esta comentado esto: */
				/* SDL_FreeSurface(textos[i].imagen) ; */
				textos[i].existe=0;
			}
		}
		return 1;
	}
	else if (i<1 || i>MAX_WRITES) {
		fp->Runtime_Error(119);	/* id de texto no válido */
		return -1 ;
	}

	textos[i].existe = 0;

	return 1;
}


/*
 * Entrypoints
 */
void frame(FUNCTION_PARAMS)
{
	SDL_Rect srcrect, dstrect;
	char *texto;
	char texto2[30];
	int i, j;
	int w, h, ax;

	for (i=0; i<0xFFF ;i++)
		if ( textos[i].existe )
		{
			if ( textos[i].numerica )
			{
				SDL_FreeSurface( textos[i].imagen ) ;

				itoa( fp->mem[ textos[i].offset_var ] , texto2 , 10 ) ;
				texto = texto2 ;
				
				/* Calculamos el ancho del fichero */
				w = 0 ;
				for ( j = 0 ; (unsigned)j < strlen(texto) ; j++ )
					w += fuente_control_s[textos[i].fuente].rect[ texto[j] ].w +1;
				h = fuente_control_s[textos[i].fuente].imagen->h ;

				textos[i].imagen = SDL_CreateRGBSurface(SDL_HWSURFACE, w , h , fuente_control_s[textos[i].fuente].bytespp * 8 , 0 , 0 , 0 , 0 ) ;
				SDL_SetColorKey( textos[i].imagen , SDL_SRCCOLORKEY | SDL_RLEACCEL , 0 ) ;

				/* Escribimos */
				ax = 0 ; /* Valor de x acumulativo mientras se va escribiendo */
				for ( j = 0 ; (unsigned)j < strlen(texto) ; j++ )
				{
					dstrect.x = ax ;
					dstrect.y = fuente_control_s[textos[i].fuente].rect[ texto[j] ].y ;
					dstrect.w = fuente_control_s[textos[i].fuente].rect[ texto[j] ].w ;
					dstrect.h = fuente_control_s[textos[i].fuente].rect[ texto[j] ].h ;
					SDL_BlitSurface( fuente_control_s[textos[i].fuente].imagen , &fuente_control_s[textos[i].fuente].rect[ texto[j] ] , textos[i].imagen , &dstrect ) ;
					ax += fuente_control_s[textos[i].fuente].rect[ texto[j] ].w +1  ;
				}

				/* Centrado */
				if ( textos[i].centrado == 0 || textos[i].centrado == 3 || textos[i].centrado == 6 )
					textos[i].cx = 0 ;
				if ( textos[i].centrado == 1 || textos[i].centrado == 4 || textos[i].centrado == 7 )
					textos[i].cx = (int) textos[i].imagen->w / 2 ;
				if ( textos[i].centrado == 2 || textos[i].centrado == 5 || textos[i].centrado == 8 )
					textos[i].cx = textos[i].imagen->w -1 ;
				if ( textos[i].centrado > -1 && textos[i].centrado < 3 )
					textos[i].cy = 0 ;
				if ( textos[i].centrado > 2 && textos[i].centrado < 6 )
					textos[i].cy = (int) textos[i].imagen->h / 2 ;
				if ( textos[i].centrado > 5 && textos[i].centrado < 9 )
					textos[i].cy = textos[i].imagen->h -1 ;
			}


			if(textos[i].imagen->w==0)
				continue;

			srcrect.x = 0 ;
			srcrect.y = 0 ;
			srcrect.w = textos[i].imagen->w ;
			srcrect.h = textos[i].imagen->h ;

			dstrect.x = textos[i].x;
			dstrect.y = textos[i].y;
			dstrect.w = srcrect.w ;
			dstrect.h = srcrect.h ;

			fp->Dibuja( textos[i].imagen,dstrect.x,dstrect.y,textos[i].cx,textos[i].cy,0,0,0,255,100,0);
		}

	/*
	 * de momento solo hay esta demostracion de que fp->Dibuja funciona xDD
	 * falta hacer los scrolls :P
	 */
	/*
	if ( fp->existe.dibuja )	
	{
		if ( fp->files[0].existe )
		{
			if ( fp->files[0].mapa[1000].existe )
			{
				srcrect.x = 0 ;
				srcrect.y = 0 ;
				srcrect.w = fp->files[0].mapa[1000].Surface->w ;
				srcrect.h = fp->files[0].mapa[1000].Surface->h ;
				fp->Dibuja( fp->files[0].mapa[1000].Surface , srcrect , srcrect , 0 , 128 ) ;
			}
		}
	}
	*/			
}



void first_load(FUNCTION_PARAMS)
{
	int i ;
	for ( i = 0 ; i < 256 ; i++ )
		existe[i] = 0 ;

	for ( i = 0 ; i < 1024 ; i++ )
		textos[i].existe = 0 ;
}



