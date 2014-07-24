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

/**
 * @file builtinfont.c
 * Convierte una fuente en bmp (8bpp) de 6x8 en un .h para poderse usar como
 * fuente de sistema (nº 0) en el stub.
 * NOTA: Este programa es menos indulgente que el de Riseven leyendo los .bmp :p
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>

typedef unsigned char byte;

void ayuda()
{
	printf("Modo de uso: builtinfont <fuente.bmp>\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	FILE *fp;
	byte trans;
	byte *px;
	SDL_Surface *imagen ;
	int i, x, y, pos=0;
	unsigned char fuente[256*8];
	char nombre[256];
	char nombreh[256];
	
	if(argc<2)
		ayuda();

	imagen = SDL_LoadBMP(argv[1]);
	if(imagen==NULL) {
		printf("No se ha podido abrir: %s\n",argv[1]);
		ayuda();
	}

	SDL_LockSurface(imagen);
        
	px = (byte*)imagen->pixels+imagen->pitch+1;

	// Color transparente ( Fila 1, Columna1, corresponde al caracter 0 )
	trans = *px;

	for(i=0;i<256;i++) {
		for(y=0;y<8;y++) {
			fuente[pos]=0;
			for(x=0;x<6;x++) {
				if(*px!=trans)
					fuente[pos]|=1<<x;
				px++;
			}
			px+=imagen->pitch-6;
			pos++;
		}
		px+=(-imagen->pitch*8)+7;
	}
	SDL_UnlockSurface( imagen );
	SDL_FreeSurface(imagen);

	// Descomentarizar esto para ver el resultado en la consola
	/*pos=0;
	for(i=0;i<256;i++) {
		for(y=0;y<8;y++) {
			for(x=0;x<6;x++) {
				if(fuente[pos]&(1<<x))
					printf("X");
				else
					printf(".");
			}
			printf("\n");
			pos++;
		}
		getchar();
	}
	exit(EXIT_SUCCESS);*/

	// Hace un bin2h de fuente[]
	sscanf(argv[1],"%[^.]",nombre);
	strcpy(nombreh,nombre);
	strcat(nombreh,".h");

	fp=fopen(nombreh,"w");
	if(fp==NULL) {
		printf("Error al crear %s\n",nombreh);
		exit(EXIT_FAILURE);
	}
	fprintf(fp,"/**\n * @file %s\n * Definición de la fuente %s (generado por builtinfont)\n */\n\n",nombreh,nombre);
	fprintf(fp,"#ifndef __EDIV__%s_h__\n#define __EDIV__%s_h__\n\n",nombre,nombre);
	fprintf(fp,"static unsigned char %s[256*8] = {\n",nombre);

	x=0;
	for(i=0;i<256*8;i++) {
		if(i<256*8-1)
			fprintf(fp,"%d,",fuente[i]);
		else
			fprintf(fp,"%d",fuente[i]);
		x++;
		if(x==30) {
			fprintf(fp,"\n");
			x=0;
		}
	}

	fprintf(fp,"\n};\n\n#endif /*__EDIV__%s_h__*/\n",nombre);
	fclose(fp);

	printf("Creado %s\n",nombreh);

	exit (EXIT_SUCCESS);
}


