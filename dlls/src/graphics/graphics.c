/*! \file graphics.c
 * \brief DLL principal del motor gráfico 2D de eDIV
 *
 * En esta DLL se encuentran las principales funciones relacionadas con el
 * manejo de gráficos. Tiene prioridad P_SIEMPRE ya que se supone que la
 * mayoría de los programas hechos con eDIV requerirán tratamiento de gráficos.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifndef WIN32
 #include <unistd.h>
#endif

#include <SDL/SDL.h>

#include <assert.h>

#include "export.h"
#include "graphics.h"
#include "SDL_rotozoom.h"
#include "default_palette.h"


/*! \brief Función de exportación de símbolos de la DLL
 * @return TRUE si la DLL da su permiso para ser cargada, en caso contrario, FALSE
 */
int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{
	/* Modos predefinidos para set_mode (sólo 8 bpp, compatibilidad DIV2) */
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

	/* flags para set_mode */
	CONST("_fullscreen",GR_FULLSCREEN);

	/* valores para dump_type */
	CONST("partial_dump",0);
	CONST("complete_dump",1);

	/* valores para restore_type */
	CONST("no_restore",-1);
	CONST("partial_restore",0);
	CONST("complete_restore",1);

	/* valores para graphic_info() */
	CONST("g_wide",0);
	CONST("g_height",1);
	CONST("g_x_center",2);
	CONST("g_y_center",3);

	/* constante para delete_draw() */
	CONST("all_drawing",-1);
	

	/* estructura para get_video_modes() */
	GLOBAL_STRUCT("video_modes",31);
		_INT("wide",0);
		_INT("height",0);
		_INT("bpp",0);		/* NUEVO */
		_INT("mode",0);
	END_STRUCT;
	
	/* variables globales */
	GLOBAL("dump_type",1);
	GLOBAL("restore_type",1);
	GLOBAL("fading",0);
	GLOBAL("fps",0);
	GLOBAL("num_video_modes",0);
	GLOBAL("vsync",0);
	GLOBAL("draw_z",-255);
	GLOBAL("smooth",smooth); /* > Atención una nueva variable que indica si se activa o no el SMOOTH al ZOOMEAR. */

	/* variables locales */
	LOCAL("x",0);
	LOCAL("y",0);
	LOCAL("z",0);
	LOCAL("graph",0);
	LOCAL("size",100);
	LOCAL("angle",0);
	LOCAL("region",0);
	LOCAL("file",0);
	LOCAL("xgraph",0);
	LOCAL("resolution",0);
	LOCAL("flags",0);
	LOCAL("transparency",128);

	/* FUNCIONES */

	/* Carga/descarga */
	FUNCTION("load_bmp",1,eDIV_LOAD_BMP) ;
	FUNCTION("load_fpg",1,eDIV_LOAD_FPG) ;

	/* Colisiones */
	FUNCTION("collision",1,eDIV_COLLISION) ;

	/* Paleta */
	FUNCTION("set_transparent_color",1,eDIV_SET_TRANSPARENT_COLOR) ;
	FUNCTION("get_transparent_color",0,eDIV_GET_TRANSPARENT_COLOR) ;
	FUNCTION("rgb",3,eDIV_RGB) ;
	FUNCTION("find_color",3,eDIV_FIND_COLOR);
	FUNCTION("fade",4,eDIV_FADE) ;
	FUNCTION("get_rgb",4,eDIV_GET_RGB);

	/* Advance y xadvance */
	FUNCTION("advance",1,eDIV_ADVANCE) ;
	FUNCTION("xadvance",2,eDIV_XADVANCE) ;

	/* Operaciones con mapas */
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
	FUNCTION("get_point",5,eDIV_GET_POINT) ;
	FUNCTION("get_real_point",3,eDIV_GET_REAL_POINT) ;
	FUNCTION("graphic_info",3,eDIV_GRAPHIC_INFO) ;
	FUNCTION("xput",6,eDIV_XPUT);

	/* Regiones */
	FUNCTION("define_region",5,eDIV_DEFINE_REGION) ;
	FUNCTION("out_region",2,eDIV_OUT_REGION) ;

	/* Draws */
	FUNCTION("draw",8,eDIV_DRAW) ;
	FUNCTION("move_draw",7,eDIV_MOVE_DRAW) ;
	FUNCTION("delete_draw",1,eDIV_DELETE_DRAW) ;

	/* Otras */
	FUNCTION("set_mode",1,eDIV_SET_MODE);
	FUNCTION("set_mode",3,eDIV_SET_MODE);
	FUNCTION("set_mode",4,eDIV_SET_MODE);


	/* Entrypoints */
	ENTRYPOINT( frame ) ;
	ENTRYPOINT( first_load ) ;


	return TRUE;
}

FILE * fichero ;
FILE * memo ;

/*! \brief Guarda una captura de la pantalla en un .bmp
 *
 * Esta función es llamada cuando se pulsa ALT+P. Primero busca un nombre de
 * fichero válido del tipo nombre_programa####.bmp, donde #### es el primer
 * número que haya disponible, para ir numerando las capturas automáticamente.
 * Luego simplemente usa SDL_SaveBMP() para guardar el backbuffer con ese
 * nombre.
 * @param nombre_program Nombre del programa, obtenido de fp->nombre_program
 */
void guarda_pantallazo(char* nombre_program)
{
	char capturef[256];
	int c=0;
	FILE* f;

	sprintf(capturef,"%s%04d.bmp",nombre_program,c);
	while(f=fopen(capturef,"rb")) {
		fclose(f);
		c++;
		sprintf(capturef,"%s%04d.bmp",nombre_program,c);
		if(c==0)
			break;
	}
	SDL_SaveBMP(screen,capturef);
}

/*! \brief Función para usar con qsort() para ordenar los blits por su Z
 *
 * Esta función se pasa a qsort() para que la graphics (en el entrypoint frame)
 * ordene los blits (del tipo struct _blits) según su Z, para que se dibujen
 * en el backbuffer en el orden correcto.
 * @param a Primer blit a ordenar
 * @param b Segundo blit a ordenar
 * @return -1 si a está detrás de b, 1 si a está delante de b, 0 si tienen la misma Z
 * @see frame(), struct _blits
 */
int ordena_por_z(const void* a, const void* b)
{
	struct _blits* aa=(struct _blits*)a;
	struct _blits* bb=(struct _blits*)b;
	return (aa->z>bb->z)?-1:((aa->z<bb->z)?1:0);
}
	

/*
 * Entrypoints
 */

/*! \brief Entrypoint frame
 *
 * Este entrypoint se ejecuta una vez todos los procesos han ejecutado su código
 * correspondiente al frame actual. Se encarga de procesar todos los gráficos
 * en pantalla (fondo, draws, procesos y los creados por otras DLL's mediante
 * fp->Dibuja()), blitearlos en el backbuffer y actualizar la pantalla.
 * @see Dibuja(), draw.c
 */
void frame(FUNCTION_PARAMS)
{
	static int una_vez = 1 ;
	int i ,  id , f , g , r , z , trans,angle,size,resolution,flags;
	SDL_Rect dstrect , srcrect ;
//	Uint32 rmask , gmask , bmask , amask ;
	SDL_Surface* temp;
	int noevent;
	SDL_Event event;
	byte* teclas;

/*		rmask = 0x00ff0000;
		gmask = 0x0000ff00;
		bmask = 0x000000ff;
		amask = 0x00000000;
*/

	if(primer_frame) {
		primer_frame=FALSE;

		/*
		 * TODO: Añadir comprobacion de errores en los 2 if siguientes (Daijo)
		 */
		if (SDL_Init(SDL_INIT_VIDEO)) {
			fp->Critical_Error(7); /* No se pudo inicializar SDL */
			return;
		}
		
		temp = SDL_SetVideoMode(fp->graphics->ancho,fp->graphics->alto,fp->graphics->bpp,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_HWACCEL|((fp->graphics->flags&GR_FULLSCREEN)?SDL_FULLSCREEN:0)|((fp->graphics->bpp==8)?SDL_HWPALETTE:0));
		if(fp->graphics->bpp==8)
			//PaletteCopy(temp,screen);
			SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL,(SDL_Color*)default_palette,0,256);

		if(fp->graphics->resflags&GR_CHANGED) {
			SDL_FreeSurface(screen);
			SDL_FreeSurface(fondo);
			screen=temp;
			fp->graphics->buffer=screen->pixels;
			fondo=SDL_CreateRGBSurface(SDL_HWSURFACE,fp->graphics->ancho,fp->graphics->alto,fp->graphics->bpp,0,0,0,0);
			if(fp->graphics->bpp==8)
				PaletteCopy(fondo,screen);
			fp->graphics->background=fondo->pixels;
			fp->graphics->resflags^=GR_CHANGED;
		}
		else {
			SDL_BlitSurface(screen,NULL,temp,NULL);
			SDL_FreeSurface(screen);
			screen=temp;
			fp->graphics->buffer=screen->pixels;
		}

		if ( screen == NULL || fondo==NULL) {
			fp->Critical_Error(7); /* No se pudo inicializar SDL */
			return;
		}

		if(fp->graphics->bpp==8) {
			fp->graphics->palette=(struct _palette*)screen->format->palette->colors;
			fp->graphics->activepal=(struct _palette*)activepal;
			memcpy(activepal,fp->graphics->palette,256*4);
		}

		SDL_WM_SetCaption(fp->nombre_program, NULL);
		SDL_ShowCursor(0);
	}
	else {
		if(fp->graphics->resflags&GR_CHANGED)
			fp->graphics->resflags^=GR_CHANGED;
	}

	/* si no gestionamos un poco los eventos la cosa se cuelga :p */

	noevent=0;
	while ( SDL_PollEvent(&event) && !noevent )
	{
		switch( event.type )
		{
		case SDL_QUIT:
			//assert(0);
			fp->Stub_Quit(0);
			break ;
		case SDL_NOEVENT:
			noevent=1;
			break;
		}
	}

	teclas=SDL_GetKeyState(NULL);
	if(teclas[SDLK_x] && (teclas[SDLK_RALT] || teclas[SDLK_LALT]))
		fp->Stub_Quit(0);
	if(teclas[SDLK_p] && (teclas[SDLK_RALT] || teclas[SDLK_LALT]))
		guarda_pantallazo(fp->nombre_program);

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
						
/*			if ( draws[i].x + draws[i].Surface->w >= fp->regions[0].x && draws[i].x < fp->regions[0].x + fp->regions[0].w &&
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
					dstrect.w = 0 ; /* Se ignora *//*
					dstrect.h = 0 ; /* Se ignora *//*
					Dibuja( draws[i].Surface , srcrect , dstrect , z , draws[i].t,100,0) ; 
				}
			}*/
			Dibuja(draws[i].Surface,draws[i].x,draws[i].y,0,0,draws[i].region,z,0,draws[i].t,100,0);
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
		flags=local("flags",id);
		resolution = local("resolution",id);
		if(resolution!=0) {
			dstrect.x = dstrect.x / resolution;
			dstrect.y = dstrect.y / resolution;
		}
		/*dstrect.w = 0 ;
		dstrect.h = 0 ;*/
		if ( files[f].mapa[g].existe )
		{
			/*srcrect.x = 0 ;
			srcrect.y = 0 ;
			srcrect.w = files[f].mapa[g].Surface->w ;
			srcrect.h = files[f].mapa[g].Surface->h ;
			dstrect.x -= files[f].mapa[g].cpoint[0].x ;
			dstrect.y -= files[f].mapa[g].cpoint[0].y ;*/
			if ( flags & 4 )
				if ( flags & 8 ) {
					trans = 255 - local("transparency",id);
					if(trans<0) trans=0; else if(trans>255) trans=255;
				}
				else
					trans = 128 ;
            else
				trans = SDL_ALPHA_OPAQUE ;
			//if ( r == 0 )
					//Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans , size , angle) ; 
					Dibuja(files[f].mapa[g].Surface,dstrect.x,dstrect.y,files[f].mapa[g].cpoint[0].x,files[f].mapa[g].cpoint[0].y,r,z,flags,trans,size,angle);
			/*else { //if ( define_region == 0 )
				if ( dstrect.x >= fp->regions[r].x && dstrect.x + dstrect.w <= fp->regions[r].x + fp->regions[r].w &&
					dstrect.y >= fp->regions[r].y && dstrect.y + dstrect.h <= fp->regions[r].y + fp->regions[r].h )
				{
					Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans , size , angle) ; 
				}
				else {
					if ( dstrect.x < fp->regions[r].x + fp->regions[r].w && dstrect.x + dstrect.w > fp->regions[r].x &&
						dstrect.y < fp->regions[r].y + fp->regions[r].h && dstrect.y + dstrect.h > fp->regions[r].y )
					{
						srcrect.x = 0 ;
						srcrect.y = 0 ;
						srcrect.w = dstrect.w ;
						srcrect.h = dstrect.h ;

						if ( dstrect.x < fp->regions[r].x ) {
							srcrect.x = fp->regions[r].x - dstrect.x ;
							srcrect.w -= fp->regions[r].x - dstrect.x ;
							dstrect.x = fp->regions[r].x ;
						}
						if ( dstrect.y < fp->regions[r].y ) {
							srcrect.y = fp->regions[r].y - dstrect.y ;
							srcrect.h -= fp->regions[r].y - dstrect.y ;
							dstrect.y = fp->regions[r].y ;
						}
						if ( dstrect.x + srcrect.w > fp->regions[r].x + fp->regions[r].w ) {
							srcrect.w -= dstrect.x + srcrect.w - fp->regions[r].x - fp->regions[r].w ;
						}
						if ( dstrect.y + srcrect.h > fp->regions[r].y + fp->regions[r].h ) {
							srcrect.h -= dstrect.y + srcrect.h - fp->regions[r].y - fp->regions[r].h ;
						}

	
						Dibuja( files[f].mapa[g].Surface , srcrect , dstrect , z , trans , size , angle) ; 
					}
				}
			}*/
		}
	}
	/*
	 * Volcamos la pila de bliteos
	 */
	qsort(blits,last_blit+1,sizeof(struct _blits),ordena_por_z);

	for ( i = 0 ; i <= last_blit ; i++ )
	{
		dstrect.x=regions[blits[i].region].x;
		dstrect.y=regions[blits[i].region].y;
		dstrect.w=regions[blits[i].region].w;
		dstrect.h=regions[blits[i].region].h;
		SDL_SetClipRect(screen,&dstrect);
		SDL_BlitSurface( blits[i].src , &blits[i].srcrect , screen , &blits[i].dstrect ) ;
	    SDL_FreeSurface (blits[i].src);
	}
	last_blit = -1 ;

	SDL_Flip(screen) ;
	fp->graphics->buffer=screen->pixels;
	fclose(fichero);

}

/*! \brief Entrypoint first_load
 *
 * Este entrypoint se ejecuta cuando la DLL se carga en el comienzo de la ejecución
 * del programa. Se usa para inicializar todos los datos internos de la DLL y
 * realizar alguna operación inicial si es preciso.
 */
void first_load(FUNCTION_PARAMS)
{
	int i ;
	Uint32 rmask , gmask , bmask , amask ;

	fp->Dibuja = Dibuja ;
	fp->files = files ;
	fp->existe.dibuja = 1 ;

	adaptar_paleta=FALSE;
	primer_frame=TRUE;
	smooth=0;

	for ( i = 0 ; i < 0xFF ; i++ )
		last_map[i] = 0 ;
	color_transparente = 0 ;

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

	for (i=1;i<MAX_REGIONS;i++) {
		regions[i].x = 0 ;
		regions[i].y = 0 ;
		regions[i].w = 0 ;
		regions[i].h = 0 ;
	}
	regions[0].x=0;
	regions[0].y=0;
	regions[0].w=320;
	regions[0].h=200;
	
	fp->regions = regions ;
	fp->existe.regions = 1 ;

	fp->graphics->ancho=320;
	fp->graphics->alto=200;
	fp->graphics->bpp=8;
	fp->graphics->flags=0;
	fp->graphics->resflags=GR_ACTIVE;

	rmask = 0;
	gmask = 0;
	bmask = 0;
	amask = 0;

	screen=SDL_CreateRGBSurface(SDL_HWSURFACE,fp->graphics->ancho,fp->graphics->alto,fp->graphics->bpp,rmask,gmask,bmask,amask);
	if(screen==NULL)
		fp->Custom_Error(_critical_error,"graphics: Error al crear buffer");

	fondo=SDL_CreateRGBSurface(SDL_HWSURFACE,fp->graphics->ancho,fp->graphics->alto,fp->graphics->bpp,rmask,gmask,bmask,amask);
	if(fondo==NULL)
		fp->Custom_Error(_critical_error,"graphics: Error al crear buffer");

	fp->graphics->buffer=screen->pixels;
	fp->graphics->background=fondo->pixels;

	graphics=fp->graphics;
}




/*
 * Funciones Internas de la DLL
 */

/*! \brief Agrega un gráfico a la lista de blits
 * 
 * Esta función puede usarse desde cualquier DLL (se exporta en la estructura fp)
 * y se encarga de meter un registro en la pila de bliteos, permitiendo especificar
 * ciertos parámetros describiendo cómo debe dibujarse el gráfico, incluyendo Z,
 * transparencia, etc.
 * La función hace "clipping" al gráfico automáticamente según la región que se le
 * indique.
 * \todo Hacer que no obligue a la DLL que la llama a usar SDL. Puede convertirse
 * esta función o usar una alternativa, esta se usaría de forma interna y la otra se
 * almacenaría en fp->Dibuja para que la usen las demás DLL's.
 * @param src Superficie donde se encuentra el gráfico a dibujar
 * @param x Coordenada X destino
 * @param y Coordenada Y destino
 * @param cx Y del centro del gráfico (relativo al mismo)
 * @param cy X del centro del gráfico
 * @param region Región de pantalla en la que se dibujará
 * @param z Profundidad del gráfico, permite que pueda dibujarse delante o detrás de otros gráficos
 * @param flags Flags de espejado horizontal(1)/vertical(2) (sólo se tendrán en cuenta estos dos bits)
 * @param trans Transparencia del gráfico (0..255)
 * @param size Tamaño (en porcentaje) al que debe escalarse el gráfico, 100% es el tamaño original
 * @param angle Ángulo (en milésimas de grado) para rotar el gráfico. (0 = sin rotación)
 * @return 1
 * @see frame(), xput(), ordena_por_z(), #blits
 */
int Dibuja(SDL_Surface *src,int x,int y,int cx,int cy,int region,int z,int flags,int trans,int size,int angle)
{
	float zoom;
	double angulo;
	register int i , j ;
	int u,v;
	SDL_Surface* temp=src;
	
	if(size<=0)
		return 1;

	last_blit++ ;

/*	if (size==0)
		size=100;*/
/*	if (size>100)
		size=100;*/
		
	zoom=size*0.01f;

	angle=angle%360000;
	if(angle<0) angle+=360000;
	angulo=angle/1000;
	
	/*! Ahora siempre se crea una copia de la surface, lo he puesto así porque
	 * tiene la ventaja de que con el smooth no kedan bordes negros y hace un
	 * perfecto antialiasing, pero vamos, esto hay ke optimizarlo bastante (habrá
	 * que guarrear bastante en el SDL_rotozoomer.c)
	 */
	temp=SDL_CreateRGBSurface(src->flags,src->w,src->h,src->format->BitsPerPixel,0,0,0,0);
	SDL_SetColorKey(temp,src->flags,color_transparente);

	if(flags&3) {
		/* el volteado vertical es más rápido */
		if((flags&3)==2) {
			for(i=0;i<src->h;i++)
				memcpy((byte*)temp->pixels+i*temp->pitch,(byte*)src->pixels+(src->h-i-1)*src->pitch,src->pitch);
		}
		else {
			for(i=0;i<src->w;i++)
				for(j=0;j<src->h;j++) {
					if(flags&1)
						u=src->w-i-1;
					else
						u=i;
					if(flags&2)
						v=src->h-j-1;
					else
						v=j;
					memcpy((byte*)temp->pixels+i*temp->format->BytesPerPixel+j*temp->pitch,
						(byte*)src->pixels+(u*src->format->BytesPerPixel)+(v*src->pitch),src->format->BytesPerPixel);
				}
		}
		if(flags&1)
			cx=src->w-cx-1;
		if(flags&2)
			cy=src->h-cy-1;
	}
	else {
		memcpy(temp->pixels,src->pixels,temp->h*temp->pitch);
	}

	/*! 
	 * Pequeño hack para arreglar transparency
	 * \todo Debería limpiarse y revisarse un poco :P
	 */
	if(src->flags & SDL_SRCALPHA) {
		for(i=0;i<temp->h*temp->w*temp->format->BytesPerPixel;i+=temp->format->BytesPerPixel) {
			if(*((int*)&((byte*)src->pixels)[i])!=color_transparente)
				((byte*)temp->pixels)[i+3]=trans;
		}
	}
	else {
		SDL_SetAlpha(temp,SDL_SRCALPHA,trans);
	}

	blits[last_blit].src = xput(temp,zoom,angulo);

//	if(temp!=src)
		SDL_FreeSurface(temp);

#define redondea(x) (int)(floor(x)+(((x)-(int)(x))<.5?0:1))

	if(size!=100) {
		if(angle!=0)
			i=j=sqrt(cx*cx+cy*cy)*zoom+1.0;
		else {
			i=(int)(cx*zoom);
			j=(int)(cy*zoom);
		}
	}
	else {
		if(angle!=0)
			i=j=ceil(sqrt(cx*cx+cy*cy)+1);
		else {
			i=cx;
			j=cy;
		}
	}

	blits[last_blit].srcrect.x = 0;
	blits[last_blit].srcrect.y = 0;
	blits[last_blit].srcrect.w = blits[last_blit].src->w;
	blits[last_blit].srcrect.h = blits[last_blit].src->h;
	blits[last_blit].dstrect.x = x-i;
	blits[last_blit].dstrect.y = y-j;
	blits[last_blit].dstrect.w = 0;
	blits[last_blit].dstrect.h = 0;
	blits[last_blit].z = z ;
	blits[last_blit].trans = trans ;
	blits[last_blit].region=region;

	return 1 ;
}

/*! \brief Escala y rota un gráfico
 *
 * Esta función recibe una superficie y la rota y escala según los valores deseados,
 * y devuelve una nueva superficie que contiene el gráfico transformado.
 * @param src La superficie con el gráfico que se desea transformar
 * @param size Tamaño al que se desea escalar el gráfico (1.0 es el tamaño original)
 * @param angle Ángulo (en grados) al que se desea rotar el gráfico (0.0 = sin rotación)
 * @return Una nueva superficie con el gráfico transformado
 * @see Dibuja(), SDL_rotozoom.c
 */
SDL_Surface *xput(SDL_Surface *src,double size,double angle)
{

	int s;
	SDL_Surface *dst;
	SDL_Surface *tmp;
	
    s=smooth&1;
	if((size==1 && angle==0) || graphics->bpp<16) s=0;
//	tmp= zoomSurface (src, size, size,s);
	dst=rotozoomSurface (src, angle, size,s);
	//SDL_FreeSurface (tmp);
	
	return dst;
}
