/* 
 * eDiv Executable Stub
 * Copyleft (C) 2000-2002 Sion Ltd.
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

#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "varindex.h"

int GetVarOffset(tipo_t tipo, char* nombre)
{
	byte *c, h=0;
	int bajo, alto, medio, ok=0;

	for(c=(byte*)nombre;*c;c++)
		h=((byte)(h<<1)+(h>>7))^*c;

	bajo=0;
	alto=num_indexed_vars-1;

	while(bajo<=alto) {
		medio=(bajo+alto)/2;
		if(h<varindex[medio].hash) alto=medio-1;
		else if(h>varindex[medio].hash) bajo=medio+1;
		else { ok=1; break; }
	}

	if(!ok) {
		#ifdef _DEBUG
			printf("varindex: GetVarOffset: Hash %d no encontrado (%s %s)\n",
				h,tipo==v_global?"global":tipo==v_reserved?"reserved":"local",nombre);
		#endif
		return 0;
	}

	if(medio>0) while(varindex[medio-1].hash==h) if(--medio==0) break;

	while(varindex[medio].hash==h) {
		if(varindex[medio].tipo==tipo) {
			if(!strcmp(varindex[medio].nombre,nombre)) {
				return varindex[medio].offset;
			}
		}
		medio++;
	}

	#ifdef _DEBUG
		printf("varindex: GetVarOffset: Nombre no encontrado (%s %s) hash=%d\n",
			tipo==v_global?"global":tipo==v_reserved?"reserved":"local",nombre,h);
	#endif
	return 0;
}
