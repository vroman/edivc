/*! \file graphics.h
 * \brief Cabecera principal de la librería
 *
 * Este fichero, aparte de otras cosas, contiene los prototipos de todos los .c
 * (excepto de SDL_rotozoom.c).
 * \todo hacer video_modes[] y num_video_modes
 */

#ifndef __EDIV_GRAPHICS_H_
#define __EDIV_GRAPHICS_H_

#include <SDL/SDL.h>


/* Constantes */

#define PIOVER180	0.017453292519943295769236907684886	/*!< Constante para pasar de grados a radianes y viceversa */

#define ERR_FILENOTFOUND		105
#define ERR_INVALIDFPGHEADER	106
#define ERR_INVALIDMAPCODE		110

#define MAX_DRAWS	1024				/*!< Número máximo de objetos que soporta draw() (en el DIV2 eran 256) */
#define MAX_REGIONS 32					/*!< Número máximo de regiones de pantalla (en el DIV2 eran 32) */

/* Macros */

/*! Se usa para la colisión por bounding-box */
#define Miedzy(x,a,b)	(((x) >= (a)) && ((x) <= (b)))

/*! Asigna a una superficie la paleta de otra superficie */
#define PaletteCopy(dst,src) SDL_SetPalette(dst,SDL_LOGPAL|SDL_PHYSPAL,src->format->palette->colors,0,256);


/* Globales */

/*! Color en la paleta de un MAP, FPG, etc */
typedef struct {
	unsigned char r,g,b;
} pal_t;

/*! Cabecera de los FPG de 8 bits */
typedef struct _FPGHEADER8{
    char   header[8];
	pal_t palette[256];
    /* char   palette[768]; */
    char   colorbars[576];
}FPGHEADER8;

/*! Cabecera de los FPG no paletizados */
typedef struct _FPGHEADER{
    char   header[8];
	/* pal_t palette[256];    */
    /* char   palette[768];   */
    /* char   colorbars[576]; */
}FPGHEADER;

/*! Cabecera de un mapa dentro de un FPG */
typedef struct _FPGMAPINFO{
    int    code;
    int    lenght;
    char   description[32];
    char   filename[12];
    int    wide;
    int    height;
    int    number_of_points;
}FPGMAPINFO;

//char   *graphic;  /* wide*height */

/*! Tabla de objetos creados con draw()
 * @see eDIV_DRAW(), frame()
 */
struct{
	int existe ;
	int region ;
	int x , y ;
	int t , c ;	/* requerido para move_draw() */
	SDL_Surface *Surface ;
} draws[MAX_DRAWS] ;

int last_draw ;

/*! Tabla de blits
 * @see Dibuja(), frame(), ordena_por_z()
 */
struct _blits {
	SDL_Surface *src ;
	SDL_Rect srcrect ;
	SDL_Surface *dst ;
	SDL_Rect dstrect ;
	int z ;
	int trans ;
	int region;
} blits[0xFFFF] , *orden[0xFFFF];

struct _regions regions[MAX_REGIONS];	/*!< Tabla de regiones */

SDL_Surface *screen;			/*!< Superficie del backbuffer */
SDL_Surface *fondo;				/*!< Superficie del fondo */
SDL_Color activepal[256];		/*!< Paleta activa (fades, etc) */

BOOL adaptar_paleta;			/*!< Para el force_pal() */

struct _files files[ 0xFF ] ;	/*!< Tabla de FPG's */

SDL_Surface *Mapa[0xFFF] ;

struct _file file0[0xFFF] ;
int last_map[0xFF] ;
int color_transparente ;		/*!< Color transparente actual (por defecto, 0) */
int define_region ;
BOOL primer_frame;				/*!< Indica a frame() si es la primera vez que se ejecuta */

int last_blit ;

int gamma[3] ;

int smooth;						/*!< Indica si se desea suavizar los gráficos @see xput() */

/*
** Prototipos
*/

/* Carga/descarga */
int eDIV_LOAD_BMP(FUNCTION_PARAMS);
int eDIV_LOAD_FPG(FUNCTION_PARAMS) ;

/* Colisiones */
int eDIV_COLLISION(FUNCTION_PARAMS);

/* Paleta */
int eDIV_SET_TRANSPARENT_COLOR(FUNCTION_PARAMS);
int eDIV_GET_TRANSPARENT_COLOR(FUNCTION_PARAMS);
int eDIV_RGB(FUNCTION_PARAMS);
int eDIV_FIND_COLOR(FUNCTION_PARAMS);
int eDIV_GET_RGB(FUNCTION_PARAMS);
int eDIV_FADE(FUNCTION_PARAMS) ;

/* Advance y xadvance*/
int eDIV_ADVANCE(FUNCTION_PARAMS);
int eDIV_XADVANCE(FUNCTION_PARAMS) ;

/* Operaciones con mapas */
int eDIV_MAP_BLOCK_COPY(FUNCTION_PARAMS) ;
int eDIV_MAP_GET_PIXEL(FUNCTION_PARAMS) ;
int eDIV_MAP_PUT(FUNCTION_PARAMS) ;
int eDIV_MAP_PUT_PIXEL(FUNCTION_PARAMS);
int eDIV_PUT(FUNCTION_PARAMS);
int eDIV_PUT_PIXEL(FUNCTION_PARAMS);
int eDIV_PUT_SCREEN(FUNCTION_PARAMS);
int eDIV_CLEAR_SCREEN(FUNCTION_PARAMS);
int eDIV_GET_PIXEL(FUNCTION_PARAMS);
int eDIV_NEW_MAP(FUNCTION_PARAMS) ;
int eDIV_SCREEN_COPY(FUNCTION_PARAMS) ;
int eDIV_GET_POINT(FUNCTION_PARAMS) ;
int eDIV_GET_REAL_POINT(FUNCTION_PARAMS) ;
int eDIV_GRAPHIC_INFO(FUNCTION_PARAMS) ;
int eDIV_XPUT(FUNCTION_PARAMS);

/* Regiones */
int eDIV_DEFINE_REGION(FUNCTION_PARAMS) ;
int eDIV_OUT_REGION(FUNCTION_PARAMS) ;

/* Draws */
int eDIV_DRAW(FUNCTION_PARAMS) ;
int eDIV_MOVE_DRAW(FUNCTION_PARAMS) ;
int eDIV_DELETE_DRAW(FUNCTION_PARAMS) ;

/* Otras */
int eDIV_SET_MODE(FUNCTION_PARAMS);

/* Funciones internas */
//int Dibuja(SDL_Surface *src , SDL_Rect srcrect , SDL_Rect dstrect , int z , int trans,int size,int angle);
int Dibuja(SDL_Surface *src,int x,int y,int cx,int cy,int region,int z,int flags,int trans,int size,int angle);
SDL_Surface *xput(SDL_Surface *src,double size,double angle);

#endif