#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#ifndef WIN32
#include <unistd.h>
#endif
//#include <zlib.h>
//#include <assert.h>
#include "export.h"
#include <SDL/SDL.h>
#include "graphics.h"
#include "SDL_rotozoom.h"

//#include "varindex.h"


#define FUNCTION_PARAMS2	struct _fun_params *fp

struct {
	int x,y,z,flags,angle,size,region,file,id;
	int status;
}PROCESS[1];

#define OK 1
#define ERROR2 -1

#define ESPEJO 0x01
#define ESPEJO_A	0x02
#define TRANS	0x04
#define NOCOLORKEY	0x80

#define MAX_DRAWS	1024

//GRAPH *fondo;

struct _files files[ 0xFF ] ;

SDL_Surface *Mapa[0xFFF] ;
SDL_Surface *fondo ;
struct _file file0[0xFFF] ;
int last_map[0xFF] ;
int color_trasparente ;
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

struct{
	SDL_Surface *src ;
	SDL_Rect srcrect ;
	SDL_Surface *dst ;
	SDL_Rect dstrect ;
	int z ;
	int trans ;
} blits[0xFFFF] , *orden[0xFFFF];

int last_blit ;

int gamma[3] ;


int RES_X,RES_Y;
int total_maps= 0;
int framex();
int wait();
int eDIV_PRINTF(FUNCTION_PARAMS);

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



//	printf("\nSion Entertianment 2000-2002 (c)\nGraphics.Dll 0.1 rc-1");
/*
	FUNCTION("wait",0,wait);
	FUNCTION("load_map",1,eDIV_LOAD_MAP);
	FUNCTION("setpos",5,eDIV_SETPOS);
	FUNCTION("map_xput",8,eDIV_XPUT);
	FUNCTION("fade",4,eDIV_fade);
	FUNCTION("xput",6,eDIV_XPUT);
	FUNCTION("load_fpg",1,eDIV_LOAD_FPG);
	FUNCTION("set_mode",1,eDIV_SET_MODE);
	FUNCTION("unload_fpg",1,eDIV_UNLOAD_FPG);
	FUNCTION("unload_map",1,eDIV_UNLOAD_MAP);
	FUNCTION("framex",1,framex);
	FUNCTION("printf",1,eDIV_PRINTF);
*/
	FUNCTION("load_bmp",1,eDIV_LOAD_BMP) ;
	FUNCTION("collision",1,eDIV_COLLISION) ;
	FUNCTION("set_trasparent_color",1,eDIV_SET_TRASPARENT_COLOR) ;
	FUNCTION("get_trasparent_color",0,eDIV_GET_TRASPARENT_COLOR) ;
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

	ENTRYPOINT( frame ) ;
	ENTRYPOINT( first_load ) ;



	return TRUE;
}

/* A continuación las funciones que queremos exportar. Para una mayor
 * sencillez las hemos puesto en este mismo archivo, aunque puede ser
 * aconsejable ponerlas en archivos aparte.
 */
 

//************************************  FUNCIONES GRAFICAS ******************************************//
/*
int eDIV_PRINTF(FUNCTION_PARAMS)
{
char *texto=(char *)getparm();

printf("%s",texto);
}*/

/*int wait(FUNCTION_PARAMS)
{
MessageBox(NULL,"Pulse aceptar para continuar","eDIV To0lKit <<Debug Dll>>",MB_OK);	
return 0;
}*/




/*int eDIV_fade(FUNCTION_PARAMS)
{
        
        int veloc=getparm();
		int b=getparm();
		int g=getparm();
		int r=getparm();
		//int dir=getparm();
		gr_fade_init (r, g, b, veloc, 1);

        return 0;
}*/

/*int eDIV_PUT_SCREEN(FUNCTION_PARAMS)
{
	
	GRAPH *tmp;
	int id=getparm();
	int file=getparm();
	
	tmp = bitmap_get (file,id); //Guarda el mapa en Tmp.
	tmp->x=(RES_X/2);
	tmp->y=(RES_Y/2);
	
	//	gr_blit (background_8bits,&regions[0],tmp->x,tmp->y,tmp->flags,tmp);	
	return OK;
}*/






// Funcion que coloca un mapa con caracterisitcas especiales
/*int eDIV_XPUT(FUNCTION_PARAMS)
{
	GRAPH *tmp;

	int file=getparm();
	int id=getparm();
	int x=getparm();
	int y=getparm();
	int angle=getparm();
	int scale=getparm();
	int flags=getparm();
	int region=getparm();
	
	region=1;
	
tmp = bitmap_get (file, id); //Guarda el mapa en Tmp.
printf("\nxput(%i,%i,%i,%i,%i,%i,%i,%i)",file,id,x,y,angle,scale,flags,region);
gr_rotated_blit (background_8bits, &regions[region], x, y, flags, angle, scale, tmp);

	return OK;
}*/

//Funcion que pone un mapa en pantalla
/*int eDIV_PUT(FUNCTION_PARAMS)
{
	GRAPH *tmp;
	
	
	int y=getparm();
	int x=getparm();
	int id=getparm();
	int file=getparm();
	tmp = bitmap_get (file, id); //Guarda el mapa en Tmp.
	gr_blit (background_8bits,&regions[0],tmp->x,tmp->y,tmp->flags,tmp);	


	return OK;
}*/

//Funcion que pone un pixel en pantalla
/*int eDIV_PUT_PIXEL(FUNCTION_PARAMS)
{
	int color=getparm();
	int y=getparm();
	int x=getparm();
	gr_put_pixel (background_8bits, x,  y, color);
	return OK;
}*/


//********************************* FUNCIONES DE INICIALIZACION ***************************************//

// Funcion que carga un grafico (BMP,JPG,GIF,PCX,PNG,MAP)

/*int eDIV_LOAD_MAP(FUNCTION_PARAMS)
{
	int id;					//El ID del mapa
	int type=1;
	GRAPH * tmp;
	const char *filename=(char*)getstrparm();    // Fichero a cargar
	FILE *f=fopen("graph.log","a");
	
	//--------------------- Si el grafico es un DIV_MAP ----------------------------//

	if (strstr(filename, ".map") || strstr(filename, ".MAP"))
	{
		fprintf(f,"Se esta cargar el map:%s\n",filename);
		id=gr_load_map(filename);	
		fprintf(f,"\nDEBUG: Grafico numero %i",id);
	}
	

	
	//----------------------------- Si el grafico es un BMP -------------------------//

	if (strstr(filename, ".pcx") || strstr(filename, ".PCX"))

	{
	id=gr_load_pcx (filename);	
	}

	if (strstr(filename, ".png") || strstr(filename, ".PNG"))

	{
	id=gr_load_png (filename);	
	}

	tmp = bitmap_get (1, id); //Guarda el mapa en Tmp y lo blitea.
	tmp->x=0;
	tmp->y=0;
	tmp->flags=0;	

//	gr_blit (background_8bits,&regions[0],tmp->x,tmp->y,tmp->flags,tmp);	

	total_maps++;
	return id;
}*/


/*int eDIV_SET_MODE(FUNCTION_PARAMS)
{

	int modo=getparm();
	if (modo < 320200 || modo > 16001400)
		return ERROR2;

	RES_X=modo/1000;
	RES_Y=modo%1000;
	fondo=bitmap_get (0,0);

	gr_init(RES_X,RES_Y);

	return OK;
}*/


// Carga de FPG'S

/*int eDIV_UNLOAD_FPG(FUNCTION_PARAMS)

{

	int libid=getparm();

	grlib_destroy (libid);
	return OK;
}*/

/*int eDIV_LOAD_FPG(FUNCTION_PARAMS)
{
	const char * libname=(const)(char*)getparm();
	int id;

id=gr_load_fpg (libname);

	return id;
}*/


/*int eDIV_UNLOAD_MAP(FUNCTION_PARAMS)
{

	int id=getparm();
	grlib_unload_map (1,id);

	return 0;
}*/





int eDIV_COLLISION(FUNCTION_PARAMS)
{
	int g1 , g2 ;
	int f1 , f2 ;
	int id1, id2 ;
	int a, i ;
	SDL_Rect r1 , r2 ;
	a = getparm() ;
	// Si se le pasa un ID
	if ( a < 4000000 )
	{
		id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;
		g1 = local("graph",id1)  ;
		f1 = local("file",id1) ;
		if ( files[f1].existe == 0 || files[f1].mapa[g1].existe == 0 )
			return -1 ;
		r1.x = local("x",id1) ;
		r1.y = local("y",id1) ;
		r1.w = files[f1].mapa[g1].Surface->w ;
		r1.h = files[f1].mapa[g1].Surface->h ;
		id2 = a ;
		g2 = local("graph",id2) ;
		f2 = local("file",id2);
		if ( files[f2].existe == 0 || files[f2].mapa[g2].existe == 0 )
			return -1 ;
		r2.x = local("x",id2) ;
		r2.y = local("y",id2) ;
		r2.w = files[f2].mapa[g2].Surface->w ;
		r2.h = files[f2].mapa[g1].Surface->h ;
		//Colision barata :P
		if (!( (r2.x > r1.x && r2.x > r1.x + r1.w) || (r2.x+r2.w < r1.x && r2.x+r2.w<r1.x+r1.w )))
			if (!( (r2.y > r1.y && r2.y > r1.y + r1.h) || (r2.y+r2.h < r1.y && r2.y+r2.h<r1.y+r1.h )))
				return 1 ;
	}else
	{
		// Si se le pasa un type
		for ( i = 0 ; i < *fp->num_procs ; i++ )
		{
			id1 = fp->procs_s[ fp->proc_orden[ i ] ].id ;
			//Si el proceso se corresponde con el type
			if ( reserved("process_type",id1) == a )
			{
				g1 = local("graph",id1)  ;
				f1 = local("file",id1) ;
				if ( files[f1].existe == 0 || files[f1].mapa[g1].existe == 0 )
					return -1 ;
				r1.x = local("x",id1) ;
				r1.y = local("y",id1) ;
				r1.w = files[f1].mapa[g1].Surface->w ;
				r1.h = files[f1].mapa[g1].Surface->h ;
				id2 = a ;
				g2 = local("graph",id2) ;
				f2 = local("file",id2) ;
				if ( files[f2].existe == 0 || files[f2].mapa[g2].existe == 0 )
					return -1 ;
				r2.x = local("x",id2) ;
				r2.y = local("y",id2) ;
				r2.w = files[f2].mapa[g2].Surface->w ;
				r2.h = files[f2].mapa[g1].Surface->h ;
				//Colision barata :P
				if (!( (r2.x > r1.x && r2.x > r1.x + r1.w) || (r2.x+r2.w < r1.x && r2.x+r2.w<r1.x+r1.w )))
					if (!( (r2.y > r1.y && r2.y > r1.y + r1.h) || (r2.y+r2.h < r1.y && r2.y+r2.h<r1.y+r1.h )))
						return 1 ;
			}
		}
	}

	return 0 ;
}



int eDIV_LOAD_BMP(FUNCTION_PARAMS)
{
	int i ;
	const char *filename=getstrparm();    // Fichero a cargar
	for ( i = 1000 ; i < files[0].num ; i++ ) 
	{
		if ( files[0].mapa[i].existe == 0 )
		{
			files[0].mapa[i].Surface = SDL_LoadBMP( filename ) ;
			files[0].mapa[i].existe = 1 ;
			files[0].mapa[i].cpoint[0].x = (int)files[0].mapa[i].Surface->w / 2 ;
			files[0].mapa[i].cpoint[0].y = (int)files[0].mapa[i].Surface->h / 2 ;
			SDL_SetColorKey( files[0].mapa[i].Surface , SDL_SRCCOLORKEY | SDL_RLEACCEL , color_trasparente ) ;
			if ( i > last_map[0] )
				last_map[0] = i ;
			return i ;
		}
	}

	return -1 ;
}

int eDIV_SET_TRASPARENT_COLOR(FUNCTION_PARAMS)
{
	int b , i;
	int a = getparm() ;
	b = color_trasparente ;
	color_trasparente = a ;
	for ( i = 1 ; i <= last_map[0] ; i++ )
	{
		if ( files[0].mapa[i].existe )
			SDL_SetColorKey( files[0].mapa[i].Surface , SDL_SRCCOLORKEY | SDL_RLEACCEL  , color_trasparente ) ;
	}
	return b ;
}

int eDIV_GET_TRASPARENT_COLOR(FUNCTION_PARAMS)
{
	return color_trasparente ;
}

int eDIV_RGB(FUNCTION_PARAMS)
{
	int r,g,b ;
	b = getparm() ;
	g = getparm() ;
	r = getparm() ;

	return ( b + g*256 + r*65536 ) ;
}

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
			SDL_SetColorKey( files[0].mapa[i].Surface , SDL_SRCCOLORKEY | SDL_RLEACCEL , color_trasparente ) ;
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
	//SDL_Surface *surface_temporal ;

	//Uint8 r,g,b;
	SDL_Color p[256];

	// TODO: quitar printf's y exit's y poner fp->Runtime_Error()

	archivo = getstrparm() ;
	printf("LOAD_FPG %s\n",archivo);

	f=fopen(archivo,"rb");
	if(f==NULL) {
		printf("Error al abrir archivo %s\n",archivo);
		exit(1);
	}
	
	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);
	
	printf("Leyendo cabecera\n");

	fread(&cabecera,1,sizeof(FPGHEADER),f);
	
	// TODO: optimizar esto ligeramente (comprobar primero los bytes comunes y luego
	//       leer "pg","16","24","32")
	if(strcmp(cabecera.header,"fpg\x1A\x0D\x0A")) {
		if(strcmp(cabecera.header,"f16\x1A\x0D\x0A")) {
			if(strcmp(cabecera.header,"f24\x1A\x0D\x0A")) {
				if(strcmp(cabecera.header,"f32\x1A\x0D\x0A")) {
					printf("El archivo no es un FPG\n");
					exit(1);
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
			printf("paleta[%d].r = %d\n",i,cabecera8.palette[i].r);
			printf("paleta[%d].g = %d\n",i,cabecera8.palette[i].g);
			printf("paleta[%d].b = %d\n",i,cabecera8.palette[i].b);
		}

		for(i=0;i<256;i++) {
			p[i].r=cabecera8.palette[i].r*4;
			p[i].g=cabecera8.palette[i].g*4;
			p[i].b=cabecera8.palette[i].b*4;
		}
			printf("Aplicando paleta global..\n");
		SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL,p,0,256);
	}



	while(ftell(f)<tamano) {
		fread(&infomapa,1,sizeof(FPGMAPINFO),f);
		num=infomapa.code;
		if(num>999 || num<0) {
			printf("Código incorrecto de mapa: %d\n",num);
			exit(1);
		}
		if ( files[0].mapa[num].existe == 1 )
			return -1 ;
		files[0].mapa[num].existe = 1 ;
				
		printf("Leyendo mapa de código %d\n",num);
		if(infomapa.number_of_points==0) {
			files[0].mapa[num].cpoint[0].x = (int) infomapa.wide/2 ;
			files[0].mapa[num].cpoint[0].y = (int) infomapa.height/2 ;
		} else {
			//fpgmaps[num].mapa.points=(short*)malloc(2*infomapa.number_of_points);
			printf("Puntos de control: %d\n",infomapa.number_of_points);
			//fread(fpgmaps[num].mapa.points,2,2*infomapa.number_of_points,f);
			fread(files[0].mapa[num].cpoint,2,2*infomapa.number_of_points,f) ;
		}
		printf("Ancho: %d\n",infomapa.wide);
		printf("Alto: %d\n",infomapa.height);
		//fpgmaps[num].mapa.graphic=(char*)malloc(infomapa.wide*infomapa.height*bpp/8);
		//fread(fpgmaps[num].mapa.graphic,1,infomapa.wide*infomapa.height*bpp/8,f);
		//fpgmaps[num].surface=SDL_CreateRGBSurfaceFrom(fpgmaps[num].mapa.graphic,infomapa.wide,infomapa.height,bpp,infomapa.wide*bpp/8,0,0,0,0);
		graphic = (char*)malloc(infomapa.wide*infomapa.height*bpp/8);
		fread(graphic,1,infomapa.wide*infomapa.height*bpp/8,f);
		//files[0].mapa[num].Surface = SDL_CreateRGBSurfaceFrom(graphic,infomapa.wide,infomapa.height,bpp,infomapa.wide*bpp/8,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask) ;
		printf("r: %i , g: %i , b: %i , a: %i \n" , screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask ) ;
		//if ( bpp == screen->format->BitsPerPixel )
		files[0].mapa[num].Surface = SDL_CreateRGBSurfaceFrom(graphic,infomapa.wide,infomapa.height,bpp,infomapa.wide*bpp/8,0,0,0,0) ;
		//else
		//{
		//	files[0].mapa[num].Surface = SDL_CreateRGBSurfaceFrom(graphic,infomapa.wide,infomapa.height,screen->format->BitsPerPixel,infomapa.wide*screen->format->BytesPerPixel,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask) ;

			//surface_temporal = SDL_CreateRGBSurfaceFrom(graphic,infomapa.wide,infomapa.height,screen->format->BitsPerPixel,infomapa.wide*screen->format->BytesPerPixel,0,0,0,0) ;
			//files[0].mapa[num].Surface = SDL_DisplayFormat(surface_temporal) ;
			//SDL_FreeSurface( surface_temporal ) ;
//		}


//		fpgmaps[num].surface=SDL_LoadBMP("/root/Console/background.bmp");
		cont++;
		if(bpp==8) {
			printf("Aplicando paleta a la surface..\n");
				//set_map_color(num,i,cabecera.palette[i].r,cabecera.palette[i].g,cabecera.palette[i].b);
			SDL_SetPalette(files[0].mapa[num].Surface,SDL_LOGPAL|SDL_PHYSPAL,p,0,256);
		}
		SDL_SetColorKey(files[0].mapa[num].Surface,SDL_SRCCOLORKEY|SDL_RLEACCEL,0);
	}
	//set_pal(&cabecera.palette[0]);
/*   	r=fopen("colores","w");
   	for(i=0;i<256;i++) {
   		fprintf(r,"Color %d: R %d, G %d, B %d\n",i,(char)cabecera.palette[i*3],(char)cabecera.palette[i*3+1],(char)cabecera.palette[i*3+2]);
   	}
   	fclose(r);*/
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
	

//*********************************** Entry Points **********************************************//

FILE * fichero ;
FILE * memo ;

void frame(FUNCTION_PARAMS)
{
	static int una_vez = 1 ;
	int i , temp , id , f , g , r , z , trans,angle,size;
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

	// Draws
	z = global("draw_z");
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
					Dibuja( draws[i].Surface , srcrect , dstrect , z , draws[i].t) ; 
				}
			}
		}
	}

	for ( i = 0 ; i < *fp->num_procs ; i++ )
	{
		id = fp->procs_s[ fp->proc_orden[i] ].id ;
		f = local("file",id);
		g = local("graph",id);
		r = local("region",id);
		z = local("z",id);
		size = local("size",id);
		angle = local("angle",id);
		dstrect.x = local("x",id);
		dstrect.y = local("y",id);
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
				Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans ) ; 
			else if ( define_region == 0 )
			{
				if ( dstrect.x >= fp->regions[r].x && dstrect.x + files[f].mapa[g].Surface->w <= fp->regions[r].x + fp->regions[r].w &&
					dstrect.y >= fp->regions[r].y && dstrect.y + files[f].mapa[g].Surface->h <= fp->regions[r].y + fp->regions[r].h )
				{
					Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans) ; 
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

						Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans ) ; 
					}
				}
			}
		}
	}
	//if ( last_map >= 2 )
	//	Mapa[2]->flags |= SDL_SRCALPHA ;

	// Volcamos la pila de bliteos
	for ( i = 0 ; i <= last_blit ; i++ )
	{
		SDL_SetAlpha( orden[i]->src, SDL_SRCALPHA , orden[i]->trans ) ;
		SDL_BlitSurface( orden[i]->src , &orden[i]->srcrect , screen , &orden[i]->dstrect ) ;
	
	}
	last_blit = -1 ;

	SDL_Flip(screen) ;

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
	color_trasparente = 0 ;
	if (SDL_Init(SDL_INIT_VIDEO)) ;
		//k_error(K_ERR_SDL_INIT);

		if ( screen == NULL ) ;
		//k_error(K_ERR_SDL_SET_VIDEO_MODE);


	screen = SDL_SetVideoMode(320, 200, 24, SDL_HWSURFACE  );
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
	SDL_ShowCursor(SDL_DISABLE);

	//prueba = SDL_LoadBMP("prueba.bmp" );

	//fclose(fichero ) ;

}




//---------------------------- Funciones Internas de la DLL ---------------------------------//

// ------- Dibuja -------//
// Se encarga de meter un registro en la pila de bliteos, segun su z
// Esta funcion no se encarga de discernir si esta dentro o fuera de la region o pantalla,
// eso se debe calcular antes y pasarle la informacion resultante a Dibuja

int Dibuja(SDL_Surface *src , SDL_Rect srcrect , SDL_Rect dstrect , int z , int trans )
{
	register int i , j ;

	last_blit++ ;
	//blits[last_blit].src = SDL_BlitSurface(rotozoomSurface (src, angle, 2,1), NULL , fondo , &dstrect );//src ;
	blits[last_blit].src = src;
	blits[last_blit].srcrect.x = srcrect.x ;
	blits[last_blit].srcrect.y = srcrect.y ;
	blits[last_blit].srcrect.w = srcrect.w ;
	blits[last_blit].srcrect.h = srcrect.h ;
	blits[last_blit].dstrect.x = dstrect.x ;
	blits[last_blit].dstrect.y = dstrect.y ;
	blits[last_blit].dstrect.w = dstrect.w ;
	blits[last_blit].dstrect.h = dstrect.h ;
	blits[last_blit].z = z ;
	blits[last_blit].trans = trans ;
	


	// Buscamos su posicion
	for ( i = 0 ; i < last_blit ; )
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
	orden[i] = &blits[last_blit] ;



	return 1 ;
}



SDL_Surface *xput(SDL_Surface *src,double size,double angle)
{
SDL_Surface *dst;
SDL_Surface *tmp;

    tmp= zoomSurface (src, size/100, size/100,1);
	dst=rotozoomSurface (tmp, angle, 1,1);
	//SDL_FreeSurface (tmp);
	
	return dst;
}