/*
** carga.c
** Incluye las funciones de carga y descarga de recursos gráficos
** (bmp, fpg, map, pcx, pal, png...)
*/

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"

/*
**
** eDIV_LOAD_BMP(filename);
**
** Carga un BMP donde 'filename' es el fichero a cargar.
**
** Retorna: El numero de Mapa.
**          -1 : Si no se ha podido cargar el mapa.
**
*/
int eDIV_LOAD_BMP(FUNCTION_PARAMS)
{
	int i ;
	const char *filename=getstrparm();    /* Fichero a cargar */
	for ( i = 1000 ; i < files[0].num ; i++ ) 
	{
		if ( files[0].mapa[i].existe == 0 )
		{
			files[0].mapa[i].Surface = SDL_LoadBMP( filename ) ;
			if(files[0].mapa[i].Surface == NULL)
			fp->Runtime_Error(143); /* "No se pudo cargar el mapa, archivo no encontrado." */			
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

/*
**
** eDIV_LOAD_FPG(filename);
**
** Carga un FPG, de cualquier bpp.
**
** Retorna: El numero de FPG.
**
*/

int eDIV_LOAD_FPG(FUNCTION_PARAMS)
{
	char * archivo ;
	FILE *f;
	FPGHEADER8 cabecera8;
	FPGHEADER cabecera;
	FPGMAPINFO infomapa;
	int tamano;
	int cont=0,num,i;
	int bpp;
	char* graphic;

	SDL_Color p[256];

	archivo = getstrparm() ;

	f=fopen(archivo,"rb");
	if(f==NULL) {
		fp->Runtime_Error(ERR_FILENOTFOUND);
	}
	
	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);

	fread(&cabecera,1,sizeof(FPGHEADER),f);
	
	/*
	 * TODO: optimizar esto ligeramente (comprobar primero los bytes comunes y luego
	 *       leer "pg","16","24","32")
	 */

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
			printf("\n 16 bits");
		}
	}
	else {
		bpp=8;
		printf("\n 8 bits");
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

		graphic = malloc(infomapa.wide*infomapa.height*bpp/8);
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
