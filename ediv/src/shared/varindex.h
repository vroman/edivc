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

#ifndef __VARINDEX_H
#define __VARINDEX_H

typedef enum { v_global, v_reserved, v_local } tipo_t;

/// PROTOTIPOS ///

// compilador
void inicializa_index();
void indexa_variable(tipo_t tipo, char* nombre, int ptr);
//void get_varptr(int** ptr, int* nptr);
void ordena_varindex();

// stub
int GetVarOffset(tipo_t tipo, char* nombre);

int num_indexed_vars;

typedef struct {
	unsigned char hash;
	tipo_t tipo;
	char* nombre;
	int offset;
} varindex_t;

varindex_t* varindex;

//#define var(a)		mem[varindex[a]]

#define global(nombre) mem[GetVarOffset(v_global,nombre)]
#define reserved(nombre,id) mem[(id)+GetVarOffset(v_reserved,nombre)]
#define local(nombre,id) mem[(id)+GetVarOffset(v_local,nombre)]
#define globalptr(nombre) GetVarOffset(v_global,nombre)
#define reservedptr(nombre) GetVarOffset(v_reserved,nombre)
#define localptr(nombre) GetVarOffset(v_local,nombre)


#endif // __VARINDEX_H