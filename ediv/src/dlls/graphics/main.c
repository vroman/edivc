#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include "export.h"
//#include "errors.h"	// Falta añadir errors.h al CVS
#include <SDL/SDL.h>
#include "graphics.h"
#include "SDL_rotozoom.h"

#define ERR_CANNOTOPENFPG 105
#define ERR_INVALIDFPGHEADER 106
#define ERR_INVALIDMAPCODE 110

#define Miedzy(x,a,b)	(((x) >= (a)) && ((x) <= (b)))



#define FUNCTION_PARAMS2	struct _fun_params *fp

#define MAX_DRAWS	1024


struct _files files[ 0xFF ] ;

SDL_Surface *Mapa[0xFFF] ;
SDL_Surface *fondo ;
struct _file file0[0xFFF] ;
int last_map[0xFF] ;
int color_transparente ;
int define_region ;

typedef struct {
			unsigned char r,g,b;
		} pal_t;

typedef struct _FPGHEADER8{
        char   header[8];
		pal_t palette[256];
        //char   palette[768];
        char   colorbars[576];
}FPGHEADER8;

typedef struct _FPGHEADER{
        char   header[8];
		//pal_t palette[256];
        //char   palette[768];
        //char   colorbars[576];
}FPGHEADER;

typedef struct _FPGMAPINFO{
        int    code;
        int    lenght;
        char   description[32];
        char   filename[12];
        int    wide;
        int    height;
        int    number_of_points;
}FPGMAPINFO;


char   *graphic;  // wide*height


struct{
	int existe ;
	int region ;
	int x , y ;
	int t , c ; // requerido para move_draw()
	SDL_Surface *Surface ;
} draws[MAX_DRAWS] ;

int last_draw ;

struct _blits {
	SDL_Surface *src ;
	SDL_Rect srcrect ;
	SDL_Surface *dst ;
	SDL_Rect dstrect ;
	int z ;
	int trans ;
} blits[0xFFFF] , *orden[0xFFFF];

int last_blit ;

int gamma[3] ;

int smooth=0;	// Smooth para el ZOOM

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	CONST("m320x200",320200);
	CONST("m320x240",320240);
	CONST("m320x400",320400);
	CONST("m360x240",360240);
	CONST("m360x360",360360);
	CONST("m376x282",376282);
	CONST("m400x300",400300);
	CONST("m512x384",512384);
	CONST("m640x400",640400);
	CONST("m640x480",640480);
	CONST("m800x600",800600);
	CONST("m960x720",960720);
	CONST("m1024x768",1024768);
	CONST("m1152x864",1152864);
	CONST("m1280x960",1280960);
	CONST("m1280x1024",12801024);
	CONST("m1600x1200",16001200);
	CONST("m1900x1600",19001600);

	CONST("partial_dump",0);
	CONST("complete_dump",1);

	CONST("no_restore",-1);
	CONST("partial_restore",0);
	CONST("complete_restore",1);

	CONST("g_wide",0);
	CONST("g_height",1);
	CONST("g_x_center",2);
	CONST("g_y_center",3);

	CONST("all_drawing",-1);
	

	GLOBAL_STRUCT("video_modes",31);
		_INT("wide",0);
		_INT("height",0);
		_INT("bpp",0);		// NUEVO
		_INT("mode",0);
	END_STRUCT;
	
	GLOBAL("dump_type",1);
	GLOBAL("restore_type",1);
	GLOBAL("fading",0);
	GLOBAL("fps",0);
	GLOBAL("num_video_modes",0);
	GLOBAL("vsync",0);
	GLOBAL("draw_z",-255);
	GLOBAL("smooth",smooth); //==> Atención una nueva variable que indica si se activa o no el SMOOTH al ZOOMEAR.

	FUNCTION("load_bmp",1,eDIV_LOAD_BMP) ;
	FUNCTION("collision",1,eDIV_COLLISION) ;
	FUNCTION("set_transparent_color",1,eDIV_SET_TRANSPARENT_COLOR) ;
	FUNCTION("get_transparent_color",0,eDIV_GET_TRANSPARENT_COLOR) ;
	FUNCTION("rgb",3,eDIV_RGB) ;
	FUNCTION("advance",1,eDIV_ADVANCE) ;
	FUNCTION("xadvance",2,eDIV_XADVANCE) ;
	FUNCTION("map_block_copy",9,eDIV_MAP_BLOCK_COPY) ;
	FUNCTION("map_get_pixel",4,eDIV_MAP_GET_PIXEL) ;
	FUNCTION("map_put",5,eDIV_MAP_PUT);
	FUNCTION("map_put_pixel",5,eDIV_MAP_PUT_PIXEL);
	FUNCTION("put",4,eDIV_PUT);
	FUNCTION("put_pixel",3,eDIV_PUT_PIXEL);
	FUNCTION("put_screen",2,eDIV_PUT_SCREEN);
	FUNCTION("clear_screen",0,eDIV_CLEAR_SCREEN);
	FUNCTION("get_pixel",2,eDIV_GET_PIXEL);
	FUNCTION("new_map",5,eDIV_NEW_MAP) ;
	FUNCTION("screen_copy",7,eDIV_SCREEN_COPY) ;
	FUNCTION("out_region",2,eDIV_OUT_REGION) ;
	FUNCTION("draw",8,eDIV_DRAW) ;
	FUNCTION("move_draw",7,eDIV_MOVE_DRAW) ;
	FUNCTION("delete_draw",1,eDIV_DELETE_DRAW) ;
	FUNCTION("load_fpg",1,eDIV_LOAD_FPG) ;
	FUNCTION("get_point",5,eDIV_GET_POINT) ;
	FUNCTION("get_real_point",3,eDIV_GET_REAL_POINT) ;
	FUNCTION("graphic_info",3,eDIV_GRAPHIC_INFO) ;
	FUNCTION("fade",4,eDIV_FADE) ;
	FUNCTION("xput",6,eDIV_XPUT);
//	FUNCTION("setmode",4,eDIV_SETMODE);

	ENTRYPOINT( frame ) ;
	ENTRYPOINT( first_load ) ;



	return TRUE;
}

/*
 * int IntersectionRR(int rc1left,int rc1top,int rc1right,int rc1bottom,int rc2left,int rc2top,int rc2right,int rc2bottom)
 * Comprueba si hay colisión entre dos regiones rectangulares.
 *
 * Devuelve:
 * 0 - No hay colisión
 * 1 - Hay colisión
 */
int IntersectionRR(int rc1left,int rc1top,int rc1right,int rc1bottom,int rc2left,int rc2top,int rc2right,int rc2bottom)
{
	return ((Miedzy(rc1left,rc2left,rc2right) || Miedzy(rc1right,rc2left,rc2right) ||
		Miedzy(rc2left,rc1left,rc1right) || Miedzy(rc2right,rc1left,rc1right)) &&
		(Miedzy(rc1top,rc2top,rc2bottom) || Miedzy(rc1bottom,rc2top,rc2bottom) ||
		Miedzy(rc2top,rc1top,rc1bottom) || Miedzy(rc2bottom,rc1top,rc1bottom)));
}

/* A continuación las funciones que queremos exportar. Para una mayor
 * sencillez las hemos puesto en este mismo archivo, aunque puede ser
 * aconsejable ponerlas en archivos aparte.
 */

int eDIV_COLLISION(FUNCTION_PARAMS)
{
	int g1 , g2 ;
	int f1 , f2 ;
	int id1, id2 ;
	int a, i ;
	SDL_Rect r1 , r2 ;
	int _status=reservedptr("status");
	a = getparm() ;

	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;

	g1 = local("graph",id1)  ;
	f1 = local("file",id1) ;
	if ( files[f1].existe == 0 || files[f1].mapa[g1].existe == 0 )
		return 0;

	// Si se le pasa un ID
	//if ( a < 4000000 )
	if(a<fp->imem_max)
	{
		if(a==id1 || (fp->mem[id1+_status]!=2 && fp->mem[id1+_status]!=4))
			return 0;
		r1.x = local("x",id1) ;
		r1.y = local("y",id1) ;
		r1.w = files[f1].mapa[g1].Surface->w ;
		r1.h = files[f1].mapa[g1].Surface->h ;
		id2 = a ;
		g2 = local("graph",id2) ;
		f2 = local("file",id2);
		if ( files[f2].existe == 0 || files[f2].mapa[g2].existe == 0 )
			return 0;
		r2.x = local("x",id2) ;
		r2.y = local("y",id2) ;
		r2.w = files[f2].mapa[g2].Surface->w ;
		r2.h = files[f2].mapa[g1].Surface->h ;
		//Colision barata :P
		if(IntersectionRR(r1.x,r1.y,r1.x+r1.w-1,r1.x+r1.h-1,r2.x,r2.y,r2.x+r2.w-1,r2.y+r2.h-1))
			return id2;
		/*if (!( (r2.x > r1.x && r2.x > r1.x + r1.w) || (r2.x+r2.w < r1.x && r2.x+r2.w<r1.x+r1.w )))
			if (!( (r2.y > r1.y && r2.y > r1.y + r1.h) || (r2.y+r2.h < r1.y && r2.y+r2.h<r1.y+r1.h )))
				return 1 ;*/
	}
	else {
		int* type_scan=&reserved("type_scan",id1);
		int* id_scan=&reserved("id_scan",id1);
		//assert(0);
		// Si se le pasa un type

		if(*type_scan!=a) {
			*id_scan=0;
			*type_scan=a;
		}

		for ( i = *id_scan+1 ; i < *fp->num_procs ; i++ )
		{
			id2 = fp->procs_s[ fp->proc_orden[ i ] ].id;
			if(id2==id1 || (fp->mem[id1+_status]!=2 && fp->mem[id1+_status]!=4))
				continue;

			//Si el proceso se corresponde con el type
			if ( reserved("process_type",id2) == a )
			{

				r1.x = local("x",id1) ;
				r1.y = local("y",id1) ;
				r1.w = files[f1].mapa[g1].Surface->w ;
				r1.h = files[f1].mapa[g1].Surface->h ;
				//id2 = a ;
				g2 = local("graph",id2) ;
				f2 = local("file",id2) ;
				if ( files[f2].existe == 0 || files[f2].mapa[g2].existe == 0 )
					continue;
				r2.x = local("x",id2) ;
				r2.y = local("y",id2) ;
				r2.w = files[f2].mapa[g2].Surface->w ;
				r2.h = files[f2].mapa[g1].Surface->h ;
				//Colision barata :P
				if(IntersectionRR(r1.x,r1.y,r1.x+r1.w-1,r1.x+r1.h-1,r2.x,r2.y,r2.x+r2.w-1,r2.y+r2.h-1)) {
					*id_scan=i;

					return id2;
				}
			}
		}
		*type_scan=0;
	}

	return 0 ;
}

/*****************************************************************/
/*                                                               */
/* eDIV_LOAD_BMP(filename);                                      */
/*                                                               */
/* Carga un BMP donde 'filename' es el fichero a cargar.         */
/*                                                               */
/* Retorna: El numero de Mapa.                                   */
/*          -1 : Si no se ha podido cargar el mapa.              */
/*                                                               */
/*****************************************************************/

int eDIV_LOAD_BMP(FUNCTION_PARAMS)
{
	int i ;
	const char *filename=getstrparm();    // Fichero a cargar
	for ( i = 1000 ; i < files[0].num ; i++ ) 
	{
		if ( files[0].mapa[i].existe == 0 )
		{
			files[0].mapa[i].Surface = SDL_LoadBMP( filename ) ;
			if(files[0].mapa[i].Surface == NULL)
			fp->Runtime_Error(143); /* "No se pudo cargar el mapa, archivo no encontrado."*/			
			files[0].mapa[i].existe = 1 ;
			files[0].mapa[i].cpoint[0].x = (int)files[0].mapa[i].Surface->w / 2 ;
			files[0].mapa[i].cpoint[0].y = (int)files[0].mapa[i].Surface->h / 2 ;
			SDL_SetColorKey( files[0].mapa[i].Surface , SDL_SRCCOLORKEY | SDL_RLEACCEL , color_transparente ) ;
			if ( i > last_map[0] )
				last_map[0] = i ;
			return i ;
		}
	}
	return -1 ;
}

/*****************************************************************/
/*                                                               */
/* eDIV_SET_TRANSPARENT_COLOR(a);                                */
/*                                                               */
/* Coloca cual es el color transparente donde 'a' es el color.   */
/*                                                               */
/* Retorna: El antiguo color transparente.                       */
/*                                                               */
/*****************************************************************/

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

/*****************************************************************/
/*                                                               */
/* eDIV_GET_TRANSPARENT_COLOR();                                 */
/*                                                               */
/* Retorna: El color transparente.                               */
/*                                                               */
/*****************************************************************/

int eDIV_GET_TRANSPARENT_COLOR(FUNCTION_PARAMS)
{
	return color_transparente ;
}

/*****************************************************************/
/*                                                               */
/* eDIV_RGB(r,g,b);                                              */
/*                                                               */
/* Crea un color a partir de los 3 colores basicos.    .         */
/*                                                               */
/* r=Rojo (Red) g=Verde (Green) b=Blue (Azul)                    */
/*                                                               */
/* Retorna: El color generado.                                   */
/*                                                               */
/*****************************************************************/

int eDIV_RGB(FUNCTION_PARAMS)
{
	int r,g,b ;
	b = getparm() ;
	g = getparm() ;
	r = getparm() ;

	return ( b + g*256 + r*65536 ) ;
}

/*****************************************************************/
/*                                                               */
/* eDIV_ADVANCE(a);                                              */
/*                                                               */
/* Avanza 'a' unidades segun el angulo del proceso.    .         */
/*                                                               */
/* Retorna: 1.                                                   */
/*                                                               */
/*****************************************************************/

int eDIV_ADVANCE(FUNCTION_PARAMS)
{
	int a , id1 , x , y , angulo ;
	a = getparm() ;
	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;
	angulo = local("angle",id1) ;
	x = (int) ((double)a * cos( (angulo/1000) * PIOVER180 )) ;
	y = (int) ((double)a * sin( (angulo/1000) * PIOVER180 )) ;
	local("x",id1) += x;
	local("y",id1) += y ;
	return 1 ;
}

/*****************************************************************/
/*                                                               */
/* eDIV_XADVANCE(angle,b)                                        */
/*                                                               */
/* Avanza 'b' unidades segun el angulo 'angle'    .    .         */
/*                                                               */
/* Retorna: 1                                                    */
/*                                                               */
/*****************************************************************/

int eDIV_XADVANCE(FUNCTION_PARAMS)
{
	int a , b , id1 , x , y , angulo ;
	b = getparm() ;
	a = getparm() ;
	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;
	angulo = a ;
	x = (int) ((double)b * cos( (angulo/1000) * PIOVER180 )) ;
	y = (int) ((double)b * sin( (angulo/1000) * PIOVER180 )) ;
	local("x",id1) += x ;
	local("y",id1) += y ;
	return 1 ;
}

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

/*****************************************************************/
/*                                                               */
/* eDIV_MAP_GET_PIXEL(y,x,g,f);                                  */
/*                                                               */
/* Coje el color de un pixel, de un mapa. Coordenadas, x e y     */
/*                                                               */
/* del mapa 'g' del fichero 'f'.                                 */
/*                                                               */
/* Retorna: El color del pixel.                                  */
/*                                                               */
/*****************************************************************/

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
	dstrect.w = 0 ; // Se ignora
	dstrect.h = 0 ; // Se ignora

	SDL_BlitSurface( files[f].mapa[g1].Surface , NULL , files[f].mapa[g2].Surface , &dstrect ) ;
	return 1 ;

}

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
	dstrect.w = 0 ; // Se ignora
	dstrect.h = 0 ; // Se ignora

	SDL_BlitSurface( files[f].mapa[g].Surface , NULL , fondo , &dstrect ) ;

	return 1 ;

}

/*****************************************************************/
/*                                                               */
/* eDIV_XPUT(f,g,x,y,angle,zoom);                                */
/*                                                               */
/* Coloca un MAPA 'g' del fichero 'f' en x e y con un angulo     */
/* angle y un tamaño zoom/100;                                   */
/*                                                               */
/* Retorna: 1: Si se ha colocado correctamente                   */
/*         -1: Si el mapa no existe.                             */
/*****************************************************************/

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
	dstrect.w = 0 ; // Se ignora
	dstrect.h = 0 ; // Se ignora

	map=xput(files[f].mapa[g].Surface,zoom,angle);

	SDL_BlitSurface(map , NULL , fondo , &dstrect ) ;
	SDL_FreeSurface (map);

	return 1 ;

}




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



int eDIV_CLEAR_SCREEN(FUNCTION_PARAMS)
{
	SDL_FillRect( fondo , NULL , 0 ) ;

	return 1;
}


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

int eDIV_OUT_REGION(FUNCTION_PARAMS2)
{
	int id , r ;
	int f, g , x , y ;
	r = getparm() ;
	id = getparm() ;

	f = local("file",id) ;
	g = local("graph",id) ;
	
	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;

	x = local("x",id) ;
	y = local("y",id) ;

	if ( x < fp->regions[r].x + fp->regions[r].w && x + files[f].mapa[g].Surface->w > fp->regions[r].x &&
		y < fp->regions[r].y + fp->regions[r].h && y + files[f].mapa[g].Surface->h > fp->regions[r].y )
		return 0 ;
	else
		return 1 ;
}

int eDIV_DRAW(FUNCTION_PARAMS2)
{
	int i ;
	int t , c , o , r , x0, y0 , x1 , y1  ;
	SDL_Rect dstrect ;
	y1 = getparm() ;
	x1 = getparm() ;
	y0 = getparm() ;
	x0 = getparm() ;
	r = getparm() ;
	o = getparm() ;
	c = getparm() ;
	t = getparm() ;

	for ( i = 0 ; i < 1024 ; i++ )
	{
		if ( draws[i].existe == 0 )
		{
			draws[i].Surface = SDL_CreateRGBSurface( SDL_HWSURFACE , x1 - x0 , y1 - y0 , screen->format->BitsPerPixel , 0xFF0000 , 0x00FF00 , 0x0000FF , 0x000000 ) ;
			switch ( t )
			{
			case 3:
				dstrect.x = 0 ;
				dstrect.y = 0 ;
				dstrect.w = x1 - x0 ;
				dstrect.h = y1 - y0 ;
				SDL_FillRect( draws[i].Surface , &dstrect , c ) ;
				break ;
			}
			draws[i].region = r ;
			draws[i].x = x0 ;
			draws[i].y = y0 ;
			draws[i].t = t ;
			draws[i].c = c ;
			if ( o < 15 && o > -1 )
				SDL_SetAlpha( draws[i].Surface , SDL_SRCALPHA | SDL_RLEACCEL , 17 * (o) ) ;
			draws[i].existe = 1 ;
			if ( i > last_draw )
				last_draw = i ;
			return i ;
		}
	}
	return -1 ;
}

int eDIV_MOVE_DRAW(FUNCTION_PARAMS2)
{
	SDL_Rect dstrect ;
	int id , c , o , x0 , y0 , x1 , y1 ;
	y1 = getparm() ;
	x1 = getparm() ;
	y0 = getparm() ;
	x0 = getparm() ;
	o = getparm() ;
	c = getparm() ;
	id = getparm() ;
	
	if ( !draws[id].existe )
		return -1 ;

	if ( x1 - x0 != draws[id].Surface->w || y1 - y0 != draws[id].Surface->h || c != draws[id].c )
	{
		if ( x1 - x0 != draws[id].Surface->w || y1 - y0 != draws[id].Surface->h )
		{
			SDL_FreeSurface( draws[id].Surface ) ;
			draws[id].Surface = SDL_CreateRGBSurface( SDL_HWSURFACE , x1 - x0 , y1 - y0 , screen->format->BitsPerPixel , 0xFF0000 , 0x00FF00 , 0x0000FF , 0x000000 ) ;
		}
		switch ( draws[id].t )
		{
		case 3:
			dstrect.x = 0 ;
			dstrect.y = 0 ;
			dstrect.w = x1 - x0 ;
			dstrect.h = y1 - y0 ;
			SDL_FillRect( draws[id].Surface , &dstrect , c ) ;
			break ;
		}
	}

	draws[id].x = x0 ;
	draws[id].y = y0 ;
	draws[id].c = c ;
	if ( o < 15 && o > -1 )
		SDL_SetAlpha( draws[id].Surface , SDL_SRCALPHA | SDL_RLEACCEL , 17 * (o) ) ;
	else
		if ( o == 15 )
			SDL_SetAlpha( draws[id].Surface , 0 , 255 ) ;

	return 1 ;
}

int eDIV_DELETE_DRAW(FUNCTION_PARAMS2)
{
	int n ;
	n = getparm() ;

	if(n==-1) {
		for(n=0;n<MAX_DRAWS;n++) {
			if(draws[n].existe) {
				SDL_FreeSurface( draws[n].Surface );
				draws[n].existe = 0;
			}
		}
		last_draw=0;
		return 1;
	}

	if ( !draws[n].existe )
		return -1 ;

	SDL_FreeSurface( draws[n].Surface ) ;
	draws[n].existe = 0 ;

	for ( ; last_draw > 0 ; last_draw-- )
	{
		if ( draws[last_draw].existe )
			break ;
	}
	return 1 ;


	
}


int eDIV_LOAD_FPG(FUNCTION_PARAMS2)
{
	char * archivo ;
	FILE *f;
	FPGHEADER8 cabecera8;
	FPGHEADER cabecera;
	FPGMAPINFO infomapa;
	int tamano;
	int cont=0,num,i;
	int bpp;

	SDL_Color p[256];

	archivo = getstrparm() ;

	f=fopen(archivo,"rb");
	if(f==NULL) {
		fp->Runtime_Error(ERR_CANNOTOPENFPG);
	}
	
	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);

	fread(&cabecera,1,sizeof(FPGHEADER),f);
	
	// TODO: optimizar esto ligeramente (comprobar primero los bytes comunes y luego
	//       leer "pg","16","24","32")
	if(strcmp(cabecera.header,"fpg\x1A\x0D\x0A")) {
		if(strcmp(cabecera.header,"f16\x1A\x0D\x0A")) {
			if(strcmp(cabecera.header,"f24\x1A\x0D\x0A")) {
				if(strcmp(cabecera.header,"f32\x1A\x0D\x0A")) {
					fp->Runtime_Error(ERR_INVALIDFPGHEADER);
				}
				else {
					bpp=32;
				}
			}
			else {
				bpp=24;
			}
		}
		else {
			bpp=16;
		}
	}
	else {
		bpp=8;
		fseek(f,0,SEEK_SET);
		fread(&cabecera8,1,sizeof(FPGHEADER8),f);

		for(i=0;i<256;i++) {
			p[i].r=cabecera8.palette[i].r*4;
			p[i].g=cabecera8.palette[i].g*4;
			p[i].b=cabecera8.palette[i].b*4;
		}
		SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL,p,0,256);
	}



	while(ftell(f)<tamano) {
		fread(&infomapa,1,sizeof(FPGMAPINFO),f);
		num=infomapa.code;
		if(num>999 || num<0) {
			fp->Runtime_Error(ERR_INVALIDMAPCODE);
		}
		if ( files[0].mapa[num].existe == 1 )
			return -1 ;
		files[0].mapa[num].existe = 1 ;
				
		if(infomapa.number_of_points==0) {
			files[0].mapa[num].cpoint[0].x = (int) infomapa.wide/2 ;
			files[0].mapa[num].cpoint[0].y = (int) infomapa.height/2 ;
		} else {
			fread(files[0].mapa[num].cpoint,2,2*infomapa.number_of_points,f) ;
		}

		graphic = (char*)malloc(infomapa.wide*infomapa.height*bpp/8);
		fread(graphic,1,infomapa.wide*infomapa.height*bpp/8,f);

		files[0].mapa[num].Surface = SDL_CreateRGBSurfaceFrom(graphic,infomapa.wide,infomapa.height,bpp,infomapa.wide*bpp/8,0,0,0,0) ;

		cont++;
		if(bpp==8) {
			SDL_SetPalette(files[0].mapa[num].Surface,SDL_LOGPAL|SDL_PHYSPAL,p,0,256);
		}
		SDL_SetColorKey(files[0].mapa[num].Surface,SDL_SRCCOLORKEY|SDL_RLEACCEL,0);
	}

	fclose(f);
	return 0;
}

int eDIV_GET_POINT(FUNCTION_PARAMS2)
{
	int f , g , n , dx , dy ;
	dy = getparm() ;
	dx = getparm() ;
	n = getparm() ;
	g = getparm() ;
	f = getparm() ;

	if ( !files[f].existe || !files[f].mapa[g].existe )
		return -1 ;

	fp->mem[ dx ] = files[f].mapa[g].cpoint[n].x ;
	fp->mem[ dy ] = files[f].mapa[g].cpoint[n].y ;

	return 0 ;
}

int eDIV_GET_REAL_POINT(FUNCTION_PARAMS2)
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

	fp->mem[ dx ] = files[f].mapa[g].cpoint[n].x - files[f].mapa[g].cpoint[0].x + x ;
	fp->mem[ dy ] = files[f].mapa[g].cpoint[n].y - files[f].mapa[g].cpoint[0].y + y ;

	return 0 ;
}

int eDIV_GRAPHIC_INFO(FUNCTION_PARAMS2)
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

	
int eDIV_FADE(FUNCTION_PARAMS2)
{
	int r , g , b , v ;
	v = getparm() ;
	b = getparm() ;
	g = getparm() ;
	r = getparm() ;

	return SDL_SetGamma( 1.0f , 1.0f , 1.0f ) ;
	return 1 ;
}
	
/*int eDIV_SETMODE(FUNCTION_PARAMS)
{
	int x,y,bpp,full;
	full = getparm();
	bpp  = getparm();
	y = getparm();
	x = getparm();
//SDL_SWSURFACE SDL_HWSURFACE

	if(full)
	screen = SDL_SetVideoMode(x, y, bpp, SDL_SWSURFACE  |SDL_FULLSCREEN );
	else
	screen = SDL_SetVideoMode(x, y, bpp, SDL_SWSURFACE   );
return;
}*/

//*********************************** Entry Points **********************************************//

FILE * fichero ;
FILE * memo ;

/*
 * Función para usar con qsort() para ordenar los blits por su Z
 */

int ordena_por_z(const void* a, const void* b)
{
	struct _blits* aa=(struct _blits*)a;
	struct _blits* bb=(struct _blits*)b;
	return (aa->z>bb->z)?-1:((aa->z<bb->z)?1:0);
}

void frame(FUNCTION_PARAMS)
{
	static int una_vez = 1 ;
	int i ,  id , f , g , r , z , trans,angle,size,resolution;
	SDL_Rect dstrect , srcrect ;

	fichero = fopen( "draw.txt" , "w" ) ;

	if ( define_region == 1)
	{

		if ( fp->existe.regions == 1 )
		{
			define_region = 0 ;
			fp->regions[0].x = 0 ;
			fp->regions[0].y = 0 ;
			fp->regions[0].w = 320 ;
			fp->regions[0].h = 200 ;
		}
	}

	//SDL_FillRect( screen , NULL , 0 ) ;
	SDL_BlitSurface( fondo , NULL , screen , NULL ) ;

	/*
	 * Draws
	 */

	z = global("draw_z");
	smooth = global("smooth");
	for ( i = 0 ; i <= last_draw ; i++ )
	{
		if ( draws[i].existe )
		{
			if ( draws[i].x + draws[i].Surface->w >= fp->regions[0].x && draws[i].x < fp->regions[0].x + fp->regions[0].w &&
				draws[i].y + draws[i].Surface->h >= fp->regions[0].y && draws[i].y < fp->regions[0].y + fp->regions[0].h )
			{
				if ( draws[i].x >= fp->regions[0].x && draws[i].x + draws[i].Surface->w < fp->regions[0].x + fp->regions[0].w &&
					draws[i].y >= fp->regions[0].y && draws[i].y + draws[i].Surface->h < fp->regions[0].y + fp->regions[0].h )
				{
					srcrect.x = 0 ;
					srcrect.y = 0 ;
					srcrect.w = draws[i].Surface->w ;
					srcrect.h = draws[i].Surface->h ;

					dstrect.x = draws[i].x ;
					dstrect.y = draws[i].y ;
					dstrect.w = 0 ; // Se ignora
					dstrect.h = 0 ; // Se ignora
					//Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , 0 ) ; 
					Dibuja( draws[i].Surface , srcrect , dstrect , z , draws[i].t,100,0) ; 
				}
			}
		}
	}

	/*
	 * Procesos
	 */

	for ( i = 0 ; i < *fp->num_procs ; i++ )
	{
		int _status=reservedptr("status");
		id = fp->procs_s[ fp->proc_orden[i] ].id ;

		if(fp->mem[id+_status]!=2 && fp->mem[id+_status]!=4)
			continue;

		f = local("file",id);
		g = local("graph",id);
		r = local("region",id);
		z = local("z",id);
		size = local("size",id);
		angle = local("angle",id);
		dstrect.x = local("x",id);
		dstrect.y = local("y",id);
		resolution = local("resolution",id);
		if(resolution!=0)
		{
		dstrect.x = dstrect.x / resolution;
		dstrect.y = dstrect.y / resolution;
		}
		dstrect.w = 0 ;
		dstrect.h = 0 ;
		if ( files[f].mapa[g].existe )
		{
			srcrect.x = 0 ;
			srcrect.y = 0 ;
			srcrect.w = files[f].mapa[g].Surface->w ;
			srcrect.h = files[f].mapa[g].Surface->h ;
			dstrect.x -= files[f].mapa[g].cpoint[0].x ;
			dstrect.y -= files[f].mapa[g].cpoint[0].y ;
			if ( local("flags",id) & 4 )
				//SDL_SetAlpha(files[f].mapa[g].Surface, SDL_SRCALPHA , fp->mem[ id + fp->varindex[_loc_transparency] ] ) ;
				if ( local("flags",id) & 8 ) {
					trans = 255 - local("transparency",id);
					if(trans<0) trans=0; else if(trans>255) trans=255;
				}
				else
					trans = 128 ;
            else
				//SDL_SetAlpha(files[f].mapa[g].Surface, NULL , SDL_ALPHA_OPAQUE ) ;
				trans = SDL_ALPHA_OPAQUE ;
			if ( r == 0 )
					Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans , size , angle) ; 
			else if ( define_region == 0 )
			{
				if ( dstrect.x >= fp->regions[r].x && dstrect.x + files[f].mapa[g].Surface->w <= fp->regions[r].x + fp->regions[r].w &&
					dstrect.y >= fp->regions[r].y && dstrect.y + files[f].mapa[g].Surface->h <= fp->regions[r].y + fp->regions[r].h )
				{

					Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans , size , angle) ; 
				}else
				{
					if ( dstrect.x < fp->regions[r].x + fp->regions[r].w && dstrect.x + files[f].mapa[g].Surface->w > fp->regions[r].x &&
						dstrect.y < fp->regions[r].y + fp->regions[r].h && dstrect.y + files[f].mapa[g].Surface->h > fp->regions[r].y )
					{
//						SDL_BlitSurface( files[f].mapa[g].Surface , NULL , screen , &dstrect ) ;
						srcrect.x = 0 ;
						srcrect.y = 0 ;
						srcrect.w = files[f].mapa[g].Surface->w ;
						srcrect.h = files[f].mapa[g].Surface->h ;

						if ( dstrect.x < fp->regions[r].x )
						{
							srcrect.x = fp->regions[r].x - dstrect.x ;
							srcrect.w -= fp->regions[r].x - dstrect.x ;
							dstrect.x = fp->regions[r].x ;
						}
						if ( dstrect.y < fp->regions[r].y )
						{
							srcrect.y = fp->regions[r].y - dstrect.y ;
							srcrect.h -= fp->regions[r].y - dstrect.y ;
							dstrect.y = fp->regions[r].y ;
						}
						if ( dstrect.x + srcrect.w > fp->regions[r].x + fp->regions[r].w )
						{
							srcrect.w -= dstrect.x + srcrect.w - fp->regions[r].x - fp->regions[r].w ;
						}
						if ( dstrect.y + srcrect.h > fp->regions[r].y + fp->regions[r].h )
						{
							srcrect.h -= dstrect.y + srcrect.h - fp->regions[r].y - fp->regions[r].h ;
						}

	
						Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans , size , angle) ; 
					}
				}
			}
		}
	}
	//if ( last_map >= 2 )
	//	Mapa[2]->flags |= SDL_SRCALPHA ;

	/*
	 * Volcamos la pila de bliteos
	 */

	qsort(blits,last_blit+1,sizeof(struct _blits),ordena_por_z);

	for ( i = 0 ; i <= last_blit ; i++ )
	{
		//SDL_SetAlpha( blits[i].src, SDL_SRCALPHA , /*blits[i].trans*/128 ) ;
		SDL_BlitSurface( blits[i].src , &blits[i].srcrect , screen , &blits[i].dstrect ) ;
	    SDL_FreeSurface (blits[i].src);
	}
	last_blit = -1 ;

	SDL_Flip(screen) ;
	fp->screen=screen;
	fclose(fichero);

}

void first_load(FUNCTION_PARAMS2)
{
	Uint32 rmask , gmask , bmask , amask ;
	int i ;

	fp->Dibuja = Dibuja ;
	fp->files = files ;
	fp->existe.dibuja = 1 ;

	

	for ( i = 0 ; i < 0xFF ; i++ )
		last_map[i] = 0 ;
	color_transparente = 0 ;
	if (SDL_Init(SDL_INIT_VIDEO)) ;
		//k_error(K_ERR_SDL_INIT);

		if ( screen == NULL ) ;
		//k_error(K_ERR_SDL_SET_VIDEO_MODE);


	screen = SDL_SetVideoMode(320, 200, 24, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL);
		rmask = 0x00ff0000;
		gmask = 0x0000ff00;
		bmask = 0x000000ff;
		amask = 0x00000000;
	fondo = SDL_CreateRGBSurface( SDL_HWSURFACE , 320 , 200 , 24 , rmask , gmask , bmask , amask ) ;
	//screen = SDL_SetVideoMode(320, 200, 24, NULL );
	
	for ( i = 0 ; i < 0xFF ; i++ )
	{
		files[i].existe = 0 ;
	}
	files[0].existe = 1 ;
	files[0].num = 0xFFF ;
	files[0].mapa = file0 ;

	for ( i = 0 ; i < 0xFFF ; i++ )
	{
		files[0].mapa[i].existe = 0 ;
	}

	last_blit = -1 ;

	gamma[0] = 128 ;
	gamma[1] = 128 ;
	gamma[2] = 128 ;

	define_region = 1 ;

	SDL_WM_SetCaption(fp->nombre_program, NULL);
	SDL_ShowCursor(0);

	//prueba = SDL_LoadBMP("prueba.bmp" );



}




//---------------------------- Funciones Internas de la DLL ---------------------------------//

// ------- Dibuja -------//
// Se encarga de meter un registro en la pila de bliteos, segun su z
// Esta funcion no se encarga de discernir si esta dentro o fuera de la region o pantalla,
// eso se debe calcular antes y pasarle la informacion resultante a Dibuja

int Dibuja(SDL_Surface *src , SDL_Rect srcrect , SDL_Rect dstrect , int z , int trans,int size,int angle)
{
	float zoom;
	double angulo;
	register int i , j ;
	
	last_blit++ ;
		
	//if(size==1244324)size=0; //Peta si el ZOOM es igual a 1244324 xDDDDD (Que valor maximo puede tener size?? 1000???
	if(size==0)size=100; //==== > Para la campatiblidad entre DLL's.
		
	zoom=size*0.01;

	angulo=angle*10;
	
	//blits[last_blit].src =  //rotozoomSurface (src, angle, zoom,smooth);//zoomSurface (src, zoom,zoom,SMOOTHING_OFF);;
	//blits[last_blit].src = SDL_BlitSurface(rotozoomSurface (src, angle, 2,1), NULL , fondo , &dstrect );//src ;
	
	//blits[last_blit].src = src;
	blits[last_blit].src =xput(src, zoom,angulo);

	/* PEKEÑO HACK PARA ARREGLAR TRANSPARENCY
	 * Debería limpiarse y revisarse un poco :P
	 */
	if(blits[last_blit].src->flags & SDL_SRCALPHA) {
		for(i=0;i<blits[last_blit].src->h*blits[last_blit].src->w*blits[last_blit].src->format->BytesPerPixel;i+=blits[last_blit].src->format->BytesPerPixel) {
			if(*((int*)&((unsigned char*)blits[last_blit].src->pixels)[i])!=color_transparente)
				((unsigned char*)blits[last_blit].src->pixels)[i+3]=trans;
		}
	}
	else {
		SDL_SetAlpha(blits[last_blit].src,SDL_SRCALPHA,trans);
	}

	blits[last_blit].srcrect.x = srcrect.x ;
	blits[last_blit].srcrect.y = srcrect.y ;
	blits[last_blit].srcrect.w = blits[last_blit].src->w;//srcrect.w ;
	blits[last_blit].srcrect.h = blits[last_blit].src->h;//srcrect.h ;
	blits[last_blit].dstrect.x = dstrect.x ;
	blits[last_blit].dstrect.y = dstrect.y ;
	blits[last_blit].dstrect.w = dstrect.w ;
	blits[last_blit].dstrect.h = dstrect.h ;
	blits[last_blit].z = z ;
	blits[last_blit].trans = trans ;



	// Buscamos su posicion
/*	for ( i = 0 ; i < last_blit ; )
	{
		if ( orden[i]->z < z ) 
		{
			for ( j = last_blit ; j > i ; j-- )
			{
				orden[ j ] = orden[ j-1 ] ;
			}
			break ;
		}
		i++ ;
	}
	orden[i] = &blits[last_blit] ;*/



	return 1 ;
}



SDL_Surface *xput(SDL_Surface *src,double size,double angle)
{

	int s;
	SDL_Surface *dst;
	SDL_Surface *tmp;

    s=smooth;
	if(size==1 && angle ==0)s=0;    
	tmp= zoomSurface (src, size, size,s);
	dst=rotozoomSurface (tmp, angle, 1,s);
	SDL_FreeSurface (tmp);
	
	return dst;
}