/*
 * eDiv Compiler
 * Copyright (C) 2000-2002 Sion Entertainment
 * http://www.sionhq.com
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "compiler.h"
#include "lower.h"
#include "language.h"

FILE *f;
int i,tamano;
char* buffer;
int linea;
char cfg[256];

void cfg_error(int error)
{
	printf(translate(41),cfg,linea,translate_cfg_error(error));
	printf(translate(42));
	exit(1);
}

void salta_spc() {
	while(lower[buffer[i]]==0 && i<tamano) {
		if(buffer[i]=='\n') linea++;
		else if(buffer[i]=='\'') {
			while(buffer[i]!='\n' && i<tamano) i++;
			linea++;
		}
		i++;
	}
}

void lee_ediv_cfg(char* ediv_cfg)
{
	int c;
	char tag[20];

	strcpy(cfg,ediv_cfg);

	/* Valores de las opciones por defecto */
	max_process=0; 
	ignore_errors=0;
	free_sintax=0;
	extended_conditions=0;
	simple_conditions=0;
	comprueba_rango=1;
	comprueba_id=1;
	comprueba_null=1;
	hacer_strfix=1;
	optimizar=1;

	if((f=fopen(ediv_cfg,"r"))==NULL) {
		return;
	}

	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);
	buffer=(char*)e_malloc(tamano);

	fread(buffer,1,tamano,f);
	fclose(f);

	for(i=0;i<tamano;i++) {
		salta_spc();
		c=0;
		while(lower[buffer[i]]!=0 && c<20 && i<tamano)
			tag[c++]=lower[buffer[i++]];
		tag[c]=0;

		if(!strcmp(tag,"max_process")) {
			salta_spc();
			if(buffer[i]!='=') cfg_error(1); /* se esperaba '=' */
			salta_spc();
			if(buffer[i]<'0' || buffer[i]>'9') cfg_error(2); /* se esperaba un dato numerico */
			c=0;
			while(buffer[i]>='0' && buffer[i]<='9' && i<tamano)
				tag[c++]=buffer[i++];
			tag[c]=0;
			itoa(

						max_process=constante();
						if (max_process<0) max_process=0;
						#ifdef _DEBUG
							printf("dbg: max_process=%d\n",max_process);
						#endif
						break;
					case 1: /* _extended_conditions */
						lexico();
						extended_conditions=1;
						break;
					case 2: /* _simple_conditions */
						lexico();
						simple_conditions=1;
						break;
					case 3: /* _case_sensitive */
						lexico();
						memcpy(lower+129,"üéâäàåçêëèïîìäåéææôöòûùÿöü¢£¥áíóú",35);
						memcpy(lower+'A',"ABCDEFGHIJKLMNOPQRSTUVWXYZ",26);
						lower['Ñ']='Ñ';
						break;
					case 4: /* _ignore_errors */
						lexico();
						ignore_errors=1;
						break;
					case 5: /* _free_sintax */
						lexico();
						free_sintax=1;
						break;
					case 6: /* _no_check */
						lexico();
						comprueba_rango=0;
						comprueba_id=0;
						comprueba_null=0;
						break;
					case 7: /* _no_strfix */
						lexico();
						hacer_strfix=0;
						break;
					case 8: /* _no_optimization */
                        lexico();
                        optimizar=0;
                        break;
                    case 9: /* _no_range_check */
                        lexico();
                        comprueba_rango=0;
                        break;
                    case 10: /* _no_id_check */
                        lexico();
                        comprueba_id=0;
                        break;
                    case 11: /* _no_null_check */
                        lexico();
                        comprueba_null=0;
                        break;
				
	}

	free(buffer);
}
