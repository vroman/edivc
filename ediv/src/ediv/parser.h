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

#ifndef __PARSER_H
#define __PARSER_H

#include "main.h"


/* A continuacion viene el asunto de los objetos. Para el que no se haya estudiado a fondo
 * el DIVC.CPP (ejem) diré que un objeto es cada una de las variables, constantes, tipos
 * de proceso (declarados con PROCESS), tipos de funcion (declarados con FUNCTION), funciones
 * de DLL, estructuras, etc. etc. etc.
 * Estudiaros la siguiente tabla para entenderlo mejor...
 * y familiarizaros con las abreviaturas... ;)
 */

#define max_obj 4096    //Limite maximo de objetos del compilador
#define long_med_id 16  //Longitud media de los identificadores (+4+4+1)

//-----------------------------------------------------------------------------

// Tabla de objetos - - - - - - - - - - - - - - - - - *iobj primer objeto libre

struct objeto {
  byte tipo;    // Tipo de objeto
  byte usado;   // Indica si el objeto ha sido usado antes de definirse
  byte * name;  // Puntero al nombre, para algún listado
  byte * ierror;// Puntero al código para determinar la columna si es necesario
  int linea;    // Línea de código fuente, para informar del error
  int param;    // Indica que es un objeto declarado en los parámetros
  int dll;	// En qué dll se declara el objeto, -1 = pertenece al prg
  struct objeto * anterior; // Anterior objeto de igual nombre
  struct objeto * bloque;   // Bloque de este proceso (0-global/local N-private)
  struct objeto * member;   // Indica a que struct pertenece (0-n/a)
  union {
    struct {                                            // Constante
      int valor;
      int literal;                                      // 0-no, 1-si
    } cons;
    struct {                                            // Variable global
      int offset;
    } vglo;
    struct {                                            // Tabla global
      int offset;
      int totalen;                                      // numero total de elementos
      int len1,len2,len3;                               // len2/3==-1 si n/a
    } tglo,pigl;
    struct {                                            // Byte global
      int offset;
      int totalen;                                      // numero total de elementos
      int len1,len2,len3;                               // len2/3==-1 si n/a
    } bglo,pbgl;
    struct {                                            // Word global
      int offset;
      int totalen;                                      // numero total de elementos
      int len1,len2,len3;                               // len2/3==-1 si n/a
    } wglo,pwgl;
    struct {                                            // Cadena (string) global
      int offset;
      int totalen;                                      // numero total de elementos
    } cglo,pcgl;
    struct {                                            // Struct
      int offset;
      int len_item;                                     // Número de campos
      int totalitems;                                   // Numero total de registros
      int items1,items2,items3;                         // -1 si n/a
    } sglo,sloc;
    struct {                                            // Struct pointer
      int offset;
      struct objeto * ostruct;                          // Puntero al struct
      int totalitems;                                   // Número total de registros
      int items1,items2,items3;                         // -1 si n/a
    } psgl,pslo;
    struct {                                            // Variable local
      int offset;
    } vloc;
    struct {                                            // Tabla local
      int offset;
      int totalen;
      int len1,len2,len3;
    } tloc,pilo;
    struct {                                            // Byte local
      int offset;
      int totalen;                                      // numero total de elementos
      int len1,len2,len3;                               // len2/3==-1 si n/a
    } bloc,pblo;
    struct {                                            // Word local
      int offset;
      int totalen;                                      // numero total de elementos
      int len1,len2,len3;                               // len2/3==-1 si n/a
    } wloc,pwlo;
    struct {                                            // Cadena (string) local
      int offset;
      int totalen;                                      // numero total de elementos
    } cloc,pclo;
    struct {                                            // Proceso
      struct objeto * bloque;
      int offset;
      int num_par;										// Número de parámetros
    } proc;

	  // qsort requiere como parametros un nombre de estructura y un campo
      // Ya le buscaremos una solución (p.ej. una función complementaria a EDIV_Export)

    struct {                                            // Funcion externa (DLL)
      int codigo;
      int num_par;
    } fext;
  };
} obj[max_obj], * iobj; // - - - - - - - - - - - - - - - - - - - - - - - - - -

//-----------------------------------------------------------------------------

#define tnone   0       // Tipos de objetos en obj[]
#define tcons   1		// Constantes
#define tvglo   2		// Int global
#define ttglo   3		// Tabla int global
#define tcglo   4		// Cadena global
#define tvloc   5		// Locales
#define ttloc   6
#define tcloc   7
#define tproc   8		// Proceso
#define tfunc   9
#define tsglo   10		// Structs
#define tsloc   11
#define tfext   12      // Función de una librer¡a externa

#define tbglo   13      // Byte global
#define twglo   14		// Word global
#define tbloc   15		// Byte local
#define twloc   16		// Word local

#define tpigl   17      // Puntero a ints global (un ttglo direccionable)
#define tpilo   18      // Puntero a ints local (idem)

#define tpwgl   19      // Punteros a word
#define tpwlo   20
#define tpbgl   21      // Punteros a byte
#define tpblo   22

#define tpcgl   23      // Punteros a string
#define tpclo   24
#define tpsgl   25      // Punteros a struct
#define tpslo   26

//-----------------------------------------------------------------------------

byte * vnom;			// Vector de nombres (cad_hash:int, pieza (o iobj):int, asciiz)

union {
	byte *b;
	byte **p;
} ivnom;

byte * inicio_objetos;	// Para el crear listado de la tabla de objetos

byte * vhash[256];		// Punteros al vector de nombres;

//-----------------------------------------------------------------------------


/* Ahora viene todo lo referente al parser en si, es decir, al analizador léxico y sintáctico.
 * También se incluye aquí al evaluador de expresiones (por cierto, usa notación polaca xD)
 * El parser de DIV es realmente bueno, y además muy personalizable. Que yo sepa, sólo existe
 * un bug en el evaluador de expresiones, al parecer los operadores lógicos no funcionan de
 * igual forma en sus distintas sintaxis, al menos al trabajar con cadenas. Es decir, que un
 * AND no devuelve lo mismo que un && (pablo-bug nº1). Pero todo se arreglará con un vistazo
 * al listado EML ;)
 */

#define max_nodos 128   //Máximo número de nodos del léxico para símbolos

#define cr 13           //Retorno de carro
#define lf 10           //Salto de linea
#define tab 9           //Tabulación

//Valores de lex_case, si no son punteros a lex_simb
#define l_err 0         //Caracter desconocido
#define l_cr  1         //Fin de linea (l_err Carácter no esperado)
#define l_id  2         //Identificador o palabra reservada
#define l_spc 3         //Espacios y tabulaciones
#define l_lit 4         //Literal
#define l_num 5         //Constante numérica
#define l_eof 6			//Fin de fichero


//-----------------------------------------------------------------------------
//      Valores sintácticos o tokens (pieza)
//-----------------------------------------------------------------------------

#define p_ultima        0x00 //Fin de fichero <EOF>

#define p_program       0x01
#define p_const         0x02
#define p_global        0x03
#define p_local         0x04
#define p_begin         0x05
#define p_end           0x06
#define p_process       0x07
#define p_private       0x08
#define p_struct        0x09
#define p_import        0x0A // sólo para mostrar el warning de que ya no se usa
#define p_setup_program 0x0B // idem

#define p_string        0x0C
#define p_byte          0x0D
#define p_word          0x0E
#define p_int           0x0F

#define p_compiler_options 0x10
#define p_function      0x11

#define p_until         0x15 // antes 0x16
#define p_else          0x16 // antes 0x17
#define p_elseif		0x17 // NUEVO!

#define p_return        0x18

#define p_from          0x19
#define p_to            0x1a
#define p_step          0x1b
#define p_if            0x20
#define p_loop          0x21
#define p_while         0x22
#define p_repeat        0x23
#define p_for           0x24
#define p_switch        0x25
#define p_case          0x26
#define p_default       0x27

#define p_frame         0x28

#define p_break         0x29
#define p_continue      0x2A

#define p_clone         0x2B

#define p_debug         0x2C

#define p_ptocoma       0x30
#define p_coma          0x31
#define p_corab         0x32
#define p_corce         0x33

#define p_asig          0x38

#define p_dup           0x3C

#define p_abrir         0x40
#define p_cerrar        0x41

#define p_suma          0x44
#define p_resta         0x45

#define p_shr           0x46
#define p_shl           0x47

#define p_or            0x48
#define p_xor           0x49
#define p_and           0x4a

#define p_xorptr        0x4b
#define p_andofs        0x4c

#define p_neg           0x4e
#define p_pointer       0x4f

#define p_offset        0x50
#define p_not           0x51

#define p_add           0x52
#define p_sub           0x53

#define p_mul           0x54
#define p_div           0x55
#define p_mod           0x56
#define p_multi         0x57

#define p_inc           0x58
#define p_dec           0x59

#define p_igu           0x5b
#define p_dis           0x5c

#define p_may           0x5d
#define p_men           0x5e
#define p_mei           0x5f
#define p_mai           0x60

#define p_sizeof        0x66

#define p_type          0x67

#define p_whoami        0x68

#define p_punto         0x69

#define p_rango         0x6a

#define p_add_asig      0x6b
#define p_sub_asig      0x6c
#define p_mul_asig      0x6d
#define p_div_asig      0x6e
#define p_mod_asig      0x6f
#define p_and_asig      0x70
#define p_or_asig       0x71
#define p_xor_asig      0x72
#define p_shr_asig      0x73
#define p_shl_asig      0x74

#define p_ini_rem       0x7d //Inicio comentario
#define p_end_rem       0x7e //Fin comentario
#define p_rem           0x7f //Comentario de una linea

#define p_strigu        0xc0
#define p_strdis        0xc1
#define p_strmay        0xc2
#define p_strmen        0xc3
#define p_strmei        0xc4
#define p_strmai        0xc5

#define p_pointerword   0xd0
#define p_sumaword      0xd1
#define p_restaword     0xd2
#define p_incword       0xd3
#define p_decword       0xd4
#define p_asigword      0xd5
#define p_add_asigword  0xd6
#define p_sub_asigword  0xd7
#define p_mul_asigword  0xd8
#define p_div_asigword  0xd9
#define p_mod_asigword  0xda
#define p_and_asigword  0xdb
#define p_or_asigword   0xdc
#define p_xor_asigword  0xdd
#define p_shr_asigword  0xde
#define p_shl_asigword  0xdf

#define p_pointerchar   0xe0
#define p_sumachar      0xe1
#define p_restachar     0xe2
#define p_incchar       0xe3
#define p_decchar       0xe4
#define p_asigchar      0xe5
#define p_add_asigchar  0xe6
#define p_sub_asigchar  0xe7
#define p_mul_asigchar  0xe8
#define p_div_asigchar  0xe9
#define p_mod_asigchar  0xea
#define p_and_asigchar  0xeb
#define p_or_asigchar   0xec
#define p_xor_asigchar  0xed
#define p_shr_asigchar  0xee
#define p_shl_asigchar  0xef

#define p_strcpy        0xf0
#define p_strfix        0xf1
#define p_strcat        0xf2
#define p_stradd        0xf3
#define p_strdec        0xf4
#define p_strsub        0xf5
#define p_strlen        0xf6

#define p_pointerbyte   0xf7

#define p_lit           0xfc //Puntero al literal (txt) en pieza_num
#define p_id            0xfd //o es un ptr a vnom (a un ptr al objeto)
#define p_num           0xfe //Número en pieza_num


//-----------------------------------------------------------------------------

// Tabla de elementos léxicos (distintos tipos de token)

struct lex_ele { byte caracter;
         byte token;
                 struct lex_ele * alternativa;
                 struct lex_ele * siguiente; }
       lex_simb[max_nodos], * ilex_simb, * lex_case[256];

// Pieza (token) que se está leyendo (ver constantes p_xxxx)
int pieza, pieza_num;

struct objeto * o; // Cuando pieza=p_id, objeto de tipo (**o).tipo
struct objeto * bloque_actual; // Bloque que esta siendo analizado
struct objeto * bloque_lexico; // Es 0 hasta las privadas del program

struct objeto * member; // !=0 al declarar/acceder un miembro de un struct

// número de nodos, número de objetos
int num_nodos, num_obj;

int num_obj_predefinidos;


//-----------------------------------------------------------------------------
//      Variables relacionadas con el listado formateado
//-----------------------------------------------------------------------------

int coment;           // 0-Código, 1-Dentro de comentario, 2-Anidado, ...
//int convert;          // Indica si se deben generar ya los token en ls


//-----------------------------------------------------------------------------

int old_linea; // Situación en el fichero del token anterior al último leido
byte * old_ierror, * old_ierror_end;

// El último token leido está en (linea,ierror,ierror_end)

byte * ultima_linea,cero;

// Siguiente token -> next_lexico()
int next_pieza;
int next_linea;
byte * next_source;

//-----------------------------------------------------------------------------

int parametros;

int acceso_remoto; // Para no permitir id.private

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  PROTOTIPOS
//-----------------------------------------------------------------------------

int crea_objeto(byte * nombre, int nparam);
void psintactico(void);
void plexico(void);
void sintactico(void);
void lexico(void);
byte * next_lexico(byte * _source, int coment, int linea);
void pasa_ptocoma(void);

void analiza_private(void);
struct objeto * analiza_pointer(int tipo, int offset);
int analiza_struct(int offstruct);
int analiza_struct_local(int offstruct);
int analiza_struct_private(int offstruct);
int analiza_pointer_struct(int tipo, int offset, struct objeto * estructura);
void tglo_init(int tipo);
void tloc_init(int tipo);
void tglo_init2(int tipo);

#endif // __PARSER_H