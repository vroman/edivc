/*
 * eDiv Compiler
 * Copyright (C) 2000-2002 Sion, Ltd.
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

#ifdef DEBUG_DLL
#	include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "shared.h"
#include "varindex.h"
#include "main.h"

/*
struct _vars {
	tipo_t tipo;
	char* nombre;
} vars[] = {
	global, "timer",
	global, "max_process_time",
	global, "argc",
	global, "argv",
	global, "draw_z",
	global, "scroll",
	global, "fps",
	reserved, "process_id",
	reserved, "id_scan",
	reserved, "process_type",
	reserved, "type_scan",
	reserved, "status",
	reserved, "parameters",
	reserved, "param_offset",
	reserved, "program_index",
	reserved, "stack_pointer",
	reserved, "is_executed",
	reserved, "is_painted",
	reserved, "m8_object",
	reserved, "old_ctype",
	reserved, "frame_percent",
	reserved, "box_x0",
	reserved, "box_y0",
	reserved, "box_x1",
	reserved, "box_y1",
	reserved, "f_count",
	reserved, "caller_id",
	local, "father",
	local, "son",
	local, "smallbro",
	local, "bigbro",
	local, "priority",
	local, "ctype",
	local, "x",
	local, "y",
	local, "z",
	local, "graph",
	local, "size",
	local, "angle",
	local, "region",
	local, "file",
	local, "xgraph",
	local, "height",
	local, "cnumber",
	local, "resolution",
	local, "flags",
	local, "transparency"
};

#define NUM_INDEXED_VARS (sizeof(vars)/sizeof(struct _vars))

byte hash[NUM_INDEXED_VARS];
static int varptr[NUM_INDEXED_VARS];
*/

int res_pos=0, loc_pos=0;

byte index_inicializado=0;


void inicializa_index()
{
//	int i;
//	byte *c, h;

	num_indexed_vars=0;
	varindex=NULL;
	index_inicializado=1;

/*	memset(varptr,0,NUM_INDEXED_VARS*4);

	for(i=0;i<NUM_INDEXED_VARS;i++) {
		h=0;
		for(c=(byte*)vars[i].nombre;*c;c++)
			h=((byte)(h<<1)+(h>>7))^*c;
		hash[i]=h;
		if(!res_pos && vars[i].tipo==reserved)
			res_pos=i;
		else if (!loc_pos && vars[i].tipo==local)
			loc_pos=i;
	}
	index_inicializado=1;
	return;*/
}


void indexa_variable(tipo_t tipo, char* nombre, int ptr)
{
//	int i;
	byte *c, h=0;

	if(!index_inicializado) return;

	for(c=(byte*)nombre;*c;c++)
		h=((byte)(h<<1)+(h>>7))^*c;
	
	#ifdef DEBUG_DLL
		printf("-- Indexando %s \"%s\" (hash: %d) ptr=%d\n",
			tipo==v_global?"global":tipo==v_reserved?"reserved":"local",
			nombre,h,ptr);
	#endif

	if(varindex) {
		varindex=realloc(varindex,sizeof(varindex_t)*(num_indexed_vars+1));
	}
	else {
		varindex=e_malloc(sizeof(varindex_t)*(num_indexed_vars+1));
	}
	varindex[num_indexed_vars].hash=h;
	varindex[num_indexed_vars].tipo=tipo;
	varindex[num_indexed_vars].offset=ptr;
	varindex[num_indexed_vars].nombre=strdup(nombre);

	num_indexed_vars++;

/*	#ifdef DEBUG_DLL
		printf("id asignado: %d\n",num_indexed_vars);
	#endif

	i=(tipo==reserved)?res_pos:((tipo==local)?loc_pos:0);
	for(;i<NUM_INDEXED_VARS && vars[i].tipo==tipo;i++) {
		if(h==hash[i]) if(!strcmp(nombre,vars[i].nombre)) break;
	}

	if(i==NUM_INDEXED_VARS || vars[i].tipo!=tipo) {
		#ifdef DEBUG_DLL
			printf("no encontrado\n");
		#endif
		return;
	}

	#ifdef DEBUG_DLL
		printf("id=%d encontrado!! :D\n",i);
	#endif

	varptr[i]=ptr;*/
	return;
}

int varindex_compara(const void* a, const void* b)
{
	const varindex_t *aa=(const varindex_t*)a, *bb=(const varindex_t*)b;
	return aa->hash-bb->hash;
}

void ordena_varindex()
{
	qsort(varindex,num_indexed_vars,sizeof(varindex_t),varindex_compara);
}

/*void get_varptr(int** ptr, int* nptr)
{
	*ptr=varptr;
	*nptr=NUM_INDEXED_VARS;
}
*/
