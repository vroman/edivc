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
  short dll;	// En qué dll se declara el objeto, -1 = pertenece al prg
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


struct objeto * o; // Cuando pieza=p_id, objeto de tipo (**o).tipo
struct objeto * bloque_actual; // Bloque que esta siendo analizado
struct objeto * bloque_lexico; // Es 0 hasta las privadas del program

struct objeto * member; // !=0 al declarar/acceder un miembro de un struct

// número de nodos, número de objetos
int num_nodos, num_obj;

int num_obj_predefinidos;

