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

#ifndef __EDIV_KEYWORDS_H_
#define __EDIV_KEYWORDS_H_

/* tabla de keywords */
extern struct keywords {
	char  keyword[20];	/* Max. 20 caracteres por keyword */
    char  code;			/* Code del keyword */
	void* hfuncion;		/* Puntero a la función correspondiente (para no usar SWITCH) */
} table[];

extern struct prioridades {
	char keyword[20];
	char prioridad;
} prioridad[];

/* Defines de los keywords con su code */

/* CM = TIPO DE COMANDO */
#define CM_SEPARATE		0		/* Se pone entre linea y linea        */
#define CM_STATEMENT	1		/* Statement                          */
#define CM_CALL			2		/* Llamada a una función o un proceso */
#define CM_SIGNO		3		/* Asignación                         */
#define CM_DATA			4		/* Cualquier dato                     */

/* ST = STATEMENTS */
#define ST_COMPILER_OPT 1
#define ST_PROGRAM		2
#define ST_CONST		3
#define ST_GLOBAL		4
#define ST_LOCAL		5
#define ST_PRIVATE		6
#define ST_BEGIN		7
#define ST_END			8		/* Este end vale para todos los end */
#define ST_PROCESS		9
#define ST_FUNCTION		10

#define ST_BYTE			11
#define ST_WORD			12
#define ST_INTEGER		13
#define ST_STRING		14
#define ST_POINTER		15
#define ST_STRUCT		16
#define ST_FLOAT		17

#define ST_IF			18
#define ST_ELSE			19
#define ST_ELSEIF		20
#define ST_SWITCH		21
#define ST_CASE			22
#define ST_DEFAULT		23
#define ST_GOTO			24

#define ST_LOOP			25
#define ST_FROM			26
#define ST_REPEAT		27
#define ST_UNTIL		28
#define ST_WHILE		29
#define ST_FOR			30

#define ST_BREAK		31
#define ST_CONTINUE		32
#define ST_RETURN		33

#define ST_FRAME		34
#define ST_CLONE		35
#define ST_DEBUG		36

/* DT = DATA TYPES */
#define DT_BYTE			0
#define DT_WORD			1
#define DT_INTEGER		2
#define DT_STRING		3
#define DT_POINTER		4
#define DT_STRUCT		5
#define DT_FLOAT		7 /* Tipo de dato FLOAT (nuevo) */
#define DT_UNDECLARED	8

#define DT_GLOBAL		20 /* Variable global */
#define DT_LOCAL		21
#define DT_PRIVATE		22

/* SG = SIGNOS */
#define	SG_CALL_OPEN			0
#define	SG_COMA					1
#define	SG_PARENTESIS_OPEN		2
#define	SG_PARENTESIS_CLOSE		3
#define	SG_PUNTO				4
#define	SG_NEGACION				5
#define	SG_OFFSET				6
#define	SG_PUNTERO				7
#define	SG_CORCHETE_OPEN		8
#define	SG_CORCHETE_CLOSE		9

#define	SG_NEGACION_SIGNO		10
#define	SG_INCREMENTO			11
#define	SG_DECREMENTO			12
#define	SG_MULTIPLICACION		13
#define	SG_DIVISION				14
#define	SG_MODULO				15
#define	SG_SUMA					16
#define	SG_RESTA				17
#define	SG_DESP_DERECHA			18
#define	SG_DESP_IZQUIERDA		19

#define	SG_AND					20
#define	SG_OR					21
#define	SG_XOR					22
#define	SG_IGUALDAD				23	/* Comparacion */
#define	SG_DISTINTO				24
#define	SG_MAYOR				25
#define	SG_MAYOR_IGUAL			26
#define	SG_MENOR				27
#define	SG_MENOR_IGUAL			28
#define	SG_IGUAL				29	/* Asignacion */

#define	SG_SUMA_IGUAL			30
#define	SG_RESTA_IGUAL			31
#define	SG_MULTIPLICACION_IGUAL	32
#define	SG_DIVISION_IGUAL		33
#define	SG_MODULO_IGUAL			34
#define	SG_AND_IGUAL			35
#define	SG_OR_IGUAL				36
#define	SG_XOR_IGUAL			37
#define	SG_DESP_DERECHA_IGUAL	38
#define	SG_DESP_IZQUIERDA_IGUAL	39

/* CL = Tipos de call */
#define CL_USER					0
#define CL_DLL					1


/*
 * PROTOTIPOS DE FUNCIONES
 */

typedef void (TYPEOF_hfuncion)();

/* h = HANDLERS de los statements */
void h_compiler_options();
void h_program();
void h_const();
void h_global();
void h_local();
void h_private();
void h_begin();
void h_end();
void h_process();
void h_function();

void h_byte();
void h_word();
void h_integer();
void h_float();

void h_if();
void h_else();
void h_elseif();
void h_switch();
void h_case();
void h_default();
void h_goto();

void h_loop();
void h_from();
void h_repeat();
void h_until();
void h_while();
void h_for();

void h_break();
void h_continue();
void h_return();

void h_frame();
void h_clone();
void h_debug();

#endif
