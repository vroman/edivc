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

#ifndef __GRLIB_H
#define __GRLIB_H

/* -------------------------------------------------------------------- */
/* Librería gráfica                                                     */
/* -------------------------------------------------------------------- */

/* Tipos y datos globales */

typedef struct _cpoint
{
	short int x ;
	short int y ;
}
CPOINT ;

typedef struct _animation
{
	int	frames ;
	int	length ;
	int	pos ;
	int	speed ;
	int	remaining ;
	int	last_frame ;
	Sint16  * order ;
}
ANIMATION ;

typedef struct _bitmap
{
	int	width ;
	int	pitch ;
	int	height ;
	int	code ;
	int	depth ;
	char	name[32] ;
	void	* data ;
	CPOINT	* cpoints ;
	int	flags ;
	Uint32	offset ;

        ANIMATION * animation ;
        Sint16    * blend_table ;

	int	modified ;
	int	info_flags ;

	//Hasta q no esten los entrypoints
	int x;
	int y;
	int z;
}
GRAPH ;

#define F_NCPOINTS	0x0FFF
#define F_ANIMATION	0x1000

#define GI_NOCOLORKEY 1

typedef struct _font
{
	GRAPH  * chars[256] ;
	int	voffset[256] ;
	int	width  [256] ;
}
FONT ;

typedef struct _grlib
{
	GRAPH	* * maps ;
	int	map_count ;
	int	map_reserved ;
}
GRLIB ;

typedef struct _clipregion
{
	int x ;
	int y ;
	int x2 ;	/* Inclusive */
	int y2 ;
}
REGION ;

extern int scr_initialized ;
extern REGION regions[32] ;
extern GRAPH * background ;
extern GRAPH * scrbitmap ;
extern GRLIB * syslib ;
extern int keytable[] ;	
extern int last_frame_ms ;
extern int joy_x[], joy_y[], joy_b[] ;

#define region_get(n) ( ((n)>=0 && (n)<=31) ? &regions[n] : &regions[0] )

/* Inicialización y control de tiempo */
/* ---------------------------------- */

extern void gr_init           (int w, int h) ;
extern void gr_set_fps        (int fps, int max_jump) ;
extern void gr_wait_frame     () ;
extern void gr_advance_timers () ;
extern void gr_draw_frame     () ;

/* Paleta de colores */
/* ----------------- */

extern SDL_Color vpalette[256] ;
extern SDL_Color  palette[256] ;

extern Uint16 colorequiv[256]   ;	/* Equivalencia paleta -> pantalla   */
extern Uint16 colorghost[65536] ;	/* Deja un color a 50% de intensidad */

extern GRAPH * background_8bits ;
extern int     background_8bits_used ;
extern int     background_is_black ;

extern Uint8 trans_table[256][256] ;	/* Tabla de transparencias 8 bits      	 */
extern int   trans_table_updated ;	/* 1 = La tabla es utilizable            */

extern void  gr_make_trans_table() ;

extern int  palette_loaded ;		/* ¿Se ha cargado ya la paleta inicial ? */
extern int  palette_changed ;		/* Poner a 1 cuando se cambien colores   */
extern int  fade_on ;			/* ¿Hay un fade activo?                  */
extern int  fade_step ;			/* Si lo hay, posición (0=off)           */

extern int  gr_read_pal           (file * file) ;
extern void gr_refresh_palette    () ;
extern void gr_fade_init          (int r, int g, int b, int speed, int dir) ;
extern void gr_fade_step          () ;
extern void gr_roll_palette       (int color0, int num, int inc) ;
extern int  gr_find_nearest_color (int r, int g, int b) ;
extern void gr_set_rgb            (int c, int r, int g, int b) ;
extern int  gr_rgb                (int r, int g, int b) ;
extern void gr_get_rgb            (int color, int *r, int *g, int *b) ;
extern void gr_set_colors         (int color, int num, Uint8 * pal) ;

/* Gestión de bitmaps y librerías de gráficos */
/* ------------------------------------------ */

extern int      gr_load_map       (const char * filename) ;
extern int      gr_load_png       (const char * filename) ;
extern int      gr_load_pcx       (const char * filename) ;
extern int      gr_load_fpg       (const char * filename) ;
extern int      gr_load_pal       (const char * filename) ;

extern int      grlib_new        () ;
extern GRLIB *  grlib_get        (int libid) ;
extern void     grlib_destroy    (int libid) ;
extern int      grlib_add_map    (GRLIB * lib, GRAPH * map) ;
extern int      grlib_unload_map (GRLIB * lib, int mapcode) ;

extern GRAPH * bitmap_new        (int code, int w, int h, int depth) ;
extern GRAPH * bitmap_clone      (GRAPH * this) ;
extern GRAPH * bitmap_new_syslib (int w, int h, int depth) ;
extern GRAPH * bitmap_get        (int libid, int mapcode) ;
extern void    bitmap_destroy    (GRAPH * map) ;
extern void    bitmap_add_cpoint (GRAPH *map, int x, int y) ;
extern void    bitmap_analize    (GRAPH * bitmap) ;
extern void    bitmap_animate    (GRAPH * bitmap) ;
extern void    bitmap_animate_to (GRAPH * bitmap, int pos, int speed) ;


/* Bajo nivel */
/* ---------- */

/* Flags para gr_blit */
#define B_HMIRROR	0x01
#define B_VMIRROR	0x02
#define B_TRANSLUCENT	0x04
#define B_NOCOLORKEY	0x80

/* Las funciones gráficas admiten dest=0 para referirse a la pantalla.
 * Para poder usar esta funcionalidad, debe estar bloqueada antes */

extern int  gr_lock_screen   () ;
extern void gr_unlock_screen () ;

/* Primitivas gráficas */

extern void gr_clear     (GRAPH * dest) ;
extern void gr_clear_as  (GRAPH * dest, int color) ;
extern void gr_put_pixel (GRAPH * dest, int x, int y, int color) ;
extern int  gr_get_pixel (GRAPH * dest, int x, int y) ;

extern void gr_setcolor  (int c) ;
extern void gr_vline     (GRAPH * dest, REGION * clip, int x, int y, int h) ;
#endif
