/*
 * eDiv Compiler
 * Copyleft (C) 2000-2002 Sion Entertainment
 * http://www.sion-e.com
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

#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "shared.h"
#include "edivcfg/iniparser.h"

//#define MULTI_ERROR	// para activar el sistema multi-error (bugs a mansalva)
#define DEBUG_DLL		// para mostrar información al cargar las DLLs

#define swap(a,b) {(a)^=(b);(b)^=(a);(a)^=(b);}

unsigned int stub_size;			// tamaño del STUB.EXE (está almacenado en edivrun.lib)
//FILE *fout;						// stream del seudocodigo

byte *prog, *tprog;				// buffer donde se almacena el PRG
size_t progsize;				// tamaño del PRG
char nombreprog[256];			// nombre del PRG sin extensión
char *fichero_prg;				// nombre del archivo a compilar
char outfilename[256];			// archivo de salida
char edivrun_lib[256];			// archivo stub (distinto para cada sistema)

byte listados;					// +1 = genera listado EML; +2 = genera tabla de objetos
byte debug;						// 1 = Compilar en modo de depurado
byte noexe;						// 1 = no crea un ejecutable

int n_errors;
int n_warnings;
int linea;

// Información sobre los errores

/*int numero_error=-1;
int linea_error;
int columna_error;*/

word error_25;        // Para emitir "se esperaba una sentencia"


// Prototipos

void * e_malloc(size_t size);		// un malloc con mensaje de error
void errormem();					// mensaje "memoria insuficiente"
void save_error(word tipo);			// guarda una posición de error
void error(word tipo,word num, ...);// error de compilación
void warning(int num, ...);			// warning en el proceso
//void c_error(int num);

#endif // __MAIN_H