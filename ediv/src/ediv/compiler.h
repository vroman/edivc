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

#ifndef __EDIV_COMPILER_H_
#define __EDIV_COMPILER_H_

#include "main.h"

/* "Truco" para que MSVC no de problemas 'raros' */
#ifdef _WINBASE_
#	undef lstrcpy
#	undef lstrcat
#	undef lstrlen
#endif

#define default_buffer    (16384*8)
#define security_distance (4096*8)
#define buffer_grow       (16384*8)

int * mem_ory, * frm;
int * mem, * loc;
int imem, imem_max;
int iloc, iloc_max;
int itxt, ifrm_max;

byte * imemptr;

int iloc_len; /* final de las variables locales incluyendo las privadas */

/*
 * mem[]        vector de memoria destino
 * imem         puntero para la generación de datos y código
 * imem_max     fin de la memoria destino (indice máximo de mem[])
 * iloc         inicio en mem[] de las variables locales (inicializadas)
 * iloc_len     longitud de las variables locales
 */

/* 
 * Inserción en la tabla de expresiones, acceso a variables:
 *
 * vglo           &vglo ptr
 * tglo[_exp]     &tglo <_exp> add rng ptr
 * vloc           &vloc aid ptr
 * tloc[_exp]      &tloc <_exp> add rng aid ptr
 * proc.vloc      &proc ptr &vloc add ptr
 * proc.tloc[_exp] &proc ptr &tloc <_exp> add rng add ptr
 */


FILE * fin, * fout;
byte * _source;
byte * source, * ierror, * ierror_end;

int longitud_textos;
int inicio_textos;
int max_process;
int ignore_errors;
int free_sintax;
int extended_conditions;
int simple_conditions;
int comprueba_rango;
int comprueba_id;
int comprueba_null;
int hacer_strfix;
int optimizar;
int case_sensitive;


/*
 *  MNEMÓNICOS BYTECODE EML
 */
#define lnop  0 /* *            No operación											*/
#define lcar  1 /* valor        Carga una constante en pila								*/
#define lasi  2 /*              Saca valor, offset y mete el valor en [offset]			*/
#define lori  3 /*              Or lógico												*/
#define lxor  4 /*              Xor, or exclusivo										*/
#define land  5 /*              And lógico, operador sobre condiciones					*/
#define ligu  6 /*              Igual, operador logico de comparación					*/
#define ldis  7 /*              Distinto, true si los 2 valores son diferentes			*/
#define lmay  8 /*              Mayor, comparación con signo							*/
#define lmen  9 /*              Menor, idem												*/
#define lmei 10 /*              Menor o igual											*/
#define lmai 11 /*              Mayor o igual											*/
#define ladd 12 /*              Suma dos constantes										*/
#define lsub 13 /*              Resta, operación binaria								*/
#define lmul 14 /*              Multiplicación											*/
#define ldiv 15 /*              División de enteros										*/
#define lmod 16 /*              Módulo, resto de la división							*/
#define lneg 17 /*              Negación, cambia de signo una constante					*/
#define lptr 18 /*              Pointer, saca offset y mete [offset]					*/
#define lnot 19 /*              Negación binaria, bit a bit								*/
#define laid 20 /*              Suma id a la constante de la pila						*/
#define lcid 21 /*              Carga id en la pila										*/
#define lrng 22 /* rango        Realiza una comparación de rango						*/
#define ljmp 23 /* offset       Salta a una dirección de mem[]							*/
#define ljpf 24 /* offset       Salta si un valor es falso a una dirección				*/
#define lfun 25 /* código       Llamada a un proceso interno, ej. signal() (obsoleto, ver lext)	*/
#define lcal 26 /* offset       Crea un nuevo proceso en el programa					*/
#define lret 27 /*              Auto-eliminación del proceso							*/
#define lasp 28 /*              Desecha un valor apilado								*/
#define lfrm 29 /*              Detiene por este frame la ejecución del proceso			*/
#define lcbp 30 /* num_par      Inicializa el puntero a los parámetros locales			*/
#define lcpa 31 /*              Saca offset, lee parámetro [offset] y bp++				*/
#define ltyp 32 /* bloque       Define el tipo de proceso actual (colisiones)			*/
#define lpri 33 /* offset       Salta a la dirección, y carga var. privadas				*/
#define lcse 34 /* offset       Si switch <> expresión, salta al offset					*/
#define lcsr 35 /* offset       Si switch no esta en el rango, salta al offset			*/
#define lshr 36 /*              Rotacion a la derecha (modo C, >>)						*/
#define lshl 37 /*              Rotacion a la izquierda (modo C, <<)					*/
#define lipt 38 /*              Incremento y pointer									*/
#define lpti 39 /*              Pointer e incremento									*/
#define ldpt 40 /*              Decremento y pointer									*/
#define lptd 41 /*              Pointer y decremento									*/
#define lada 42 /*              Add-asignación											*/
#define lsua 43 /*              Sub-asignación											*/
#define lmua 44 /*              Mul-asignación											*/
#define ldia 45 /*              Div-asignación											*/
#define lmoa 46 /*              Mod-asignación											*/
#define lana 47 /*              And-asignación											*/
#define lora 48 /*              Or-asignación											*/
#define lxoa 49 /*              Xor-asignación											*/
#define lsra 50 /*              Shr-asignación											*/
#define lsla 51 /*              Shl-asignación											*/
#define lpar 52 /* num_par_pri  Define el número de parámetros privados					*/
#define lrtf 53 /*              Auto-eliminación del proceso, devuelve un valor			*/
#define lclo 54 /* offset       Crea un clon del proceso actual							*/
#define lfrf 55 /*              Pseudo-Frame (frame a un porcentaje)					*/
#define limp 56 /* offset text  Importa una DLL externa									*/	
#define lext 57 /* código       Llama a una función externa								*/
#define lchk 58 /*              Comprueba la validez de un identificador				*/
#define ldbg 59 /*              Invoca al debugger										*/

/* Instrucciones añadidas para la optimización (DIV 2.0) */
#define lcar2 60		/* car + car */
#define lcar3 61		/* car + car + car */
#define lcar4 62		/* car + car + car + car */
#define lasiasp 63		/* asi + asp */
#define lcaraid 64		/* car + aid */
#define lcarptr 65		/* car + ptr */
#define laidptr 66		/* aid + ptr */
#define lcaraidptr 67	/* car + aid + ptr */
#define lcaraidcpa 68	/* car + aid + cpa */
#define laddptr 69		/* add + ptr */
#define lfunasp 70		/* fun + asp (obsoleto, ver lextasp) */
#define lcaradd 71		/* car + add */
#define lcaraddptr 72	/* car + add + ptr */
#define lcarmul 73		/* car + mul */
#define lcarmuladd 74	/* car + mul + add */
#define lcarasiasp 75	/* car + asi + asp */
#define lcarsub 76		/* car + sub */
#define lcardiv 77		/* car + div */

/* Instrucciones añadidas para el manejo de caracteres */
#define lptrchr 78  /* Pointer, saca (index, offset) y mete [offset+byte index]				*/
#define lasichr 79  /* Saca (valor, index, offset) y mete el valor en [offset+byte index]	*/
#define liptchr 80  /* Incremento y pointer													*/
#define lptichr 81  /* Pointer e incremento													*/
#define ldptchr 82  /* Decremento y pointer													*/
#define lptdchr 83  /* Pointer y decremento													*/
#define ladachr 84  /* Add-asignación														*/
#define lsuachr 85  /* Sub-asignación														*/
#define lmuachr 86  /* Mul-asignación														*/
#define ldiachr 87  /* Div-asignación														*/
#define lmoachr 88  /* Mod-asignación														*/
#define lanachr 89  /* And-asignación														*/
#define lorachr 90  /* Or-asignación														*/
#define lxoachr 91  /* Xor-asignación														*/
#define lsrachr 92  /* Shr-asignación														*/
#define lslachr 93  /* Shl-asignación														*/
#define lcpachr 94  /* Saca offset, lee parámetro [offset] y bp++							*/

/* Instrucciones añadidas para el manejo de cadenas */
#define lstrcpy 95  /* Saca si, di, y hace strcpy(mem[di],[si]) (deja di en pila)			*/
#define lstrfix 96  /* Amplia una cadena antes de meter un char en ella						*/
#define lstrcat 97  /* Concatena dos cadenas (opera como strcpy)							*/
#define lstradd 98  /* Suma dos strings "en el aire" y deja en pila el puntero al aire		*/
#define lstrdec 99  /* Añade o quita caracteres a una cadena								*/
#define lstrsub 100 /* Quita caracteres a una cadena (-=)									*/
#define lstrlen 101 /* Sustituye una cadena por su longitud									*/
#define lstrigu 102 /* Comparacion de igualdad de dos cadenas								*/
#define lstrdis 103 /* Cadenas distintas													*/
#define lstrmay 104 /* Cadena mayor															*/
#define lstrmen 105 /* Cadena menor															*/
#define lstrmei 106 /* Cadena mayor o igual													*/
#define lstrmai 107 /* Cadena menor o igual													*/
#define lcpastr 108 /* Carga un parámetro en una cadena										*/

/* Instrucciones añadidas para el manejo de Words */
#define lptrwor 109 /* Pointer, saca (index, offset) y mete [offset+byte index]				*/
#define lasiwor 110 /* Saca (valor, index, offset) y mete el valor en [offset+byte index]	*/
#define liptwor 111 /* Incremento y pointer													*/
#define lptiwor 112 /* Pointer e incremento													*/
#define ldptwor 113 /* Decremento y pointer													*/
#define lptdwor 114 /* Pointer y decremento													*/
#define ladawor 115 /* Add-asignación														*/
#define lsuawor 116 /* Sub-asignación														*/
#define lmuawor 117 /* Mul-asignación														*/
#define ldiawor 118 /* Div-asignación														*/
#define lmoawor 119 /* Mod-asignación														*/
#define lanawor 120 /* And-asignación														*/
#define lorawor 121 /* Or-asignación														*/
#define lxoawor 122 /* Xor-asignación														*/
#define lsrawor 123 /* Shr-asignación														*/
#define lslawor 124 /* Shl-asignación														*/
#define lcpawor 125 /* Saca offset, lee parámetro [offset] y bp++							*/

/* Miscelánea */
#define lnul    126 /* Comprueba que un puntero no sea NULL									*/

/* Instrucciones añadidas en eDIV */
#define lextasp 127	/* Combinación de ext y asp, sustituye a funasp				*/


struct {      /* Peephole, "mirilla" para el optimizador                        */
  int dir;    /* Dirección                                                      */
  int param;  /* Indica si ese opcode tiene parámetro                           */
  int op;     /* Opcode                                                         */
} code[16];   /* En code[15] debe quedar siempre la última instrucción generada */

/* Informacion para temporal de debug (posicion en mem y fuente de cada sentencia) */
FILE * linf;	/* En este temporal guardamos la info y luego lo agregamos al exe */

int inicio,final;     /* Direcciones inicial y final de mem[] de la sentencia */
int inicio_prg,final_prg; /* Direcciones inicial y final en el PRG de la sentencia */
int linsize;

void inicio_sentencia(void);  /* Fija inicio, linea1, columna1 */
void final_sentencia(void);   /* Fija final, linea2, columna2 */
void grabar_sentencia(void);  /* Guarda las seis variables en el fichero */

void escribe_lin(FILE* f);
void escribe_dbg(FILE* f);


int salto_import;	/* offset donde guardaremos el offset del ljmp que salta
					 * a la rutina de carga de DLLs.
					 */
					

#define max_break 512
int tbreak[max_break];
int itbreak;

#define max_cont 256
int tcont[max_cont];
int itcont;

#define max_elseif 256
int telseif[max_elseif];
int itelseif;


/*
 *  PROTOTIPOS
 */

void gen(int param, int op, int pa);
void remove_code(int i);
void delete_code(void);
void add_code(int dir, int param, int op);
int optimizado;

void prepara_compilacion();
void compila();
void test_buffer(int * * ,int * ,int);
void sentencia();

void g1(int op);
void g2(int op, int pa);

#endif /* __EDIV_COMPILER_H_ */
