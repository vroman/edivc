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


#include <string.h>
#include "main.h"
#include "parser.h"
#include "compiler.h"
#include "lower.h"
#include "expresion.h"
#include "extern.h"
#include "modulos.h"

// Para los objetos predefinidos (DLL)
#ifdef _WIN32
#pragma warning(disable:4018) // Signed/Unsigned Mismatch :)
#endif

int crea_objeto(byte * nombre)
{
	struct objeto * * ptr_o;
	byte ** ptr, * _ivnom, h;

	_ivnom=ivnom.b; *ivnom.p++=0; *ivnom.p++=0; h=0;
	while (*ivnom.b=lower[*nombre++]) h=((byte)(h<<1)+(h>>7))^(*ivnom.b++);
	ivnom.b++; if (ivnom.b-vnom>max_obj*long_med_id) return 1; // error "sobrepasado buffer de nombres de objeto"
	ptr=&vhash[h];
	while (*ptr && strcmp((char *)(ptr+2),(char *)_ivnom+8)) ptr=(byte**)*ptr;
	if (!strcmp((byte *)(ptr+2),_ivnom+8)) { // id encontrado
        ivnom.b=_ivnom; // lo saca de vnom

		ptr_o=(void*)(ptr+1); o=*ptr_o; pieza=p_id;

		// TODO: TESTEAR!!!

		// busca si el id encontrado se encuentra en la misma struct
		while(o!=NULL && ((*o).member!=member)) o=(*o).anterior;
		if(o==NULL) { // ok, no hay problema
			o=iobj++; (*o).anterior=*ptr_o; *ptr_o=o;
			(*o).name=(byte*)(ptr_o+1);
			(*o).member=member;
			(*o).param=0;
			if (num_obj++==max_obj) return 3;	// error "demasiados objetos"
		} else {
			return 2;		// dos nombres iguales en el mismo nivel de struct -> "el nombre no es nuevo"
		}

	} else {
        *ptr=_ivnom; ptr_o=(void*)(_ivnom+4); *ptr_o=o=iobj++; // id nuevo
        (*o).name=(byte*)_ivnom+8;
        (*o).member=member;
        if (num_obj++==max_obj) return 3;	// error "demasiados objetos"
	}
	
	(*o).dll=numdlls;
//	(*o).usado_dll=0;
	
	return 0;
}


////////////////////
// PRECOMPILACIÓN //
////////////////////

void psintactico(void)
{
	byte * _ivnom=ivnom.b;

	longitud_textos=0;
	source=prog;

	do {
		plexico();
	} while (pieza!=p_ultima);

	ivnom.b=_ivnom;
	source=prog;
	_source=source;
	coment=0;
}

void plexico(void)
{
	byte ** ptr, * _ivnom, h, * _source=source;
	struct lex_ele * e;

	if (!coment) {
		old_linea=linea;
		old_ierror=ierror;
		old_ierror_end=ierror_end;
	}

lex_scan:

	ierror=_source;
	
	switch ((int)lex_case[*_source]) { 		// Puntero a un lex_ele o l_???

		case l_err:
			if (coment) { pieza=p_rem; _source++; }
			else error(0,5);	// carácter no reconocido
			_source++;
			break;
			
		case l_eof:
			pieza=p_ultima;
			if (coment) error(0,1);	// llegó el final dentro de un comentario
			break;

		case l_cr:
			linea++;	
			if (*_source==cr && *(_source+1)==lf) _source+=2;
			else _source++;
				
			ultima_linea=_source;
			goto lex_scan;

		case l_id :
			if (coment) { pieza=p_rem; _source++; break; }
			
			_ivnom=ivnom.b;
			*ivnom.p++=0;
			*ivnom.p++=0;
			h=0;
			
			while (*ivnom.b=lower[*_source++])
				h=((byte)(h<<1)+(h>>7))^(*ivnom.b++);

			ivnom.b++;
			_source--;
			if (ivnom.b-vnom>max_obj*long_med_id) error(0,2); // Excedida la capacidad del vector de nombres

			ptr=&vhash[h];
			while (*ptr && strcmp((byte *)(ptr+2),_ivnom+8))
				ptr=(void*)*ptr;
				
			if (!strcmp((byte *)(ptr+2),_ivnom+8)) { 		// id encontrado
				ivnom.b=_ivnom; 				// lo saca de vnom
				pieza=(int)*(ptr+1);
				if (pieza<256 && pieza>=0) { 	// palabra reservada (token)
					if (pieza==p_rem) {
						while (*_source!=cr && *_source!=lf) _source++;
						goto lex_scan;
					}
				} else {						// objeto (id anterior)
					pieza=p_id;
				}
			}
			else {											// id nuevo
				ivnom.b=_ivnom; 				// lo saca de vnom
				pieza=p_id;
			}
			
			break;

		case l_spc:
			while ((*++_source)==' ');
			goto lex_scan;

		case l_lit:
			if (coment) { pieza=p_rem; _source++; break; }

			pieza=p_lit;		// Literal entre dos h
			h=*_source;
			_ivnom=ivnom.b;
			
			do {
				_source++;
				
				if (*_source==cr || *_source==lf) {
					error(0,3);	// literal sin cerrar
					break;
				}
					
				if (*_source==h) {
					if (*(_source+1)==h) *_ivnom=*++_source;
					else *_ivnom=0;
				}
				else
					*_ivnom=*_source;

			} while (*_ivnom++);
			
			_source++;
			longitud_textos+=(strlen(ivnom.b)+4)/4;
			ivnom.b=_ivnom;		// lo saca de vnom
			break;

		case l_num:
			if (coment) { pieza=p_rem; _source++; break; }

			pieza=p_num;
			pieza_num=0;

			if (*_source=='0' && lower[*(_source+1)]=='x') {
				_source+=2;
				while ((int)lex_case[*_source]==l_num ||
 				 (lower[*_source]>='a' && lower[*_source]<='f')) {
					if ((int)lex_case[*_source]==l_num)
            			pieza_num=pieza_num*16+*_source++-0x30;
          			else
          				pieza_num=pieza_num*16+lower[*_source++]-'a'+10;
				}
			}
			else
				do {
					pieza_num=pieza_num*10+*_source++-0x30;
				} while ((int)lex_case[*_source]==l_num);
				
			break;

		default: 									// puntero a un lex_ele
			e=lex_case[*_source++];
			_ivnom=_source;
			pieza=(*e).token;

			while (e=(*e).siguiente) {
				while (*_source!=(*e).caracter && (*e).alternativa)
					e=(*e).alternativa;

				if (*_source++==(*e).caracter && (*e).token) {
					pieza=(*e).token;
					_ivnom=_source;
				}
			}
			_source=_ivnom;

			if (pieza==p_rem && !coment) {
				while (*_source!=cr && *_source!=lf) _source++;
				goto lex_scan;
			}

			if (pieza==p_ini_rem) {
				coment++;
				do {
 					source=_source;
 					plexico();
 					_source=source;
				} while (pieza!=p_end_rem);
				coment--;
				goto lex_scan;
			}
			
			if (pieza==p_ultima) {
				if (coment) error(0,1);	// llegó el final dentro de un comentario
				else error(0,4);		// símbolo no reconocido (¡¡creo!!)
			}
			
			break;

	}
	
	source=_source;
	ierror_end=_source-1;
}


//////////////////////////////////////
// ANÁLISIS SINTÁCTICO DEL PROGRAMA //
//////////////////////////////////////

void sintactico(void)
{
	struct objeto * ob, * member2;
	int _imem,_imem_old,num_par,n;
	byte *oimemptr;
	int _itxt,dup;
//	byte * old_source,*nombre_dll;
//	char cWork[256];
//	int num_extern;

	lexico();

	//
	// Opciones de compilacion
	//
	// p_compiler_options {opcion} {;}
	//

/*	max_process=0; // Valores de las opciones por defecto
	ignore_errors=0;
	free_sintax=0;
	extended_conditions=0;
	simple_conditions=0;
	comprueba_rango=1;
	comprueba_id=1;
	comprueba_null=1;
	hacer_strfix=1;
	optimizar=1;*/

	if(case_sensitive) {
		memcpy(lower+129,"üéâäàåçêëèïîìäåéææôöòûùÿöü¢£¥áíóú",35);
		memcpy(lower+'A',"ABCDEFGHIJKLMNOPQRSTUVWXYZ",26);
		lower['Ñ']='Ñ';
	}

	if (pieza==p_compiler_options) {
		lexico();
		do {
			if (pieza==p_ptocoma) { lexico(); break; }
			if (pieza==p_coma) lexico();
			if (pieza==p_id && (*o).tipo==tcons) {
				switch((*o).cons.valor) {
					case 0: // _max_process
						lexico();
						if (pieza!=p_asig) error(3,7);	// se esperaba '='
						lexico();
						max_process=constante();
						if (max_process<0) max_process=0;
						#ifdef _DEBUG
							printf("dbg: max_process=%d\n",max_process);
						#endif
						break;
					case 1: // _extended_conditions
						lexico();
						extended_conditions=1;
						break;
					case 2: // _simple_conditions
						lexico();
						simple_conditions=1;
						break;
					case 3: // _case_sensitive
						lexico();
						case_sensitive=1;
						memcpy(lower+129,"üéâäàåçêëèïîìäåéææôöòûùÿöü¢£¥áíóú",35);
						memcpy(lower+'A',"ABCDEFGHIJKLMNOPQRSTUVWXYZ",26);
						lower['Ñ']='Ñ';
						break;
					case 4: // _ignore_errors
						lexico();
						ignore_errors=1;
						break;
					case 5: // _free_sintax
						lexico();
						free_sintax=1;
						break;
					case 6: // _no_check
						lexico();
						comprueba_rango=0;
						comprueba_id=0;
						comprueba_null=0;
						break;
					case 7: // _no_strfix
						lexico();
						hacer_strfix=0;
						break;
					case 8: // _no_optimization
                        lexico();
                        optimizar=0;
                        break;
                    case 9: // _no_range_check
                        lexico();
                        comprueba_rango=0;
                        break;
                    case 10: // _no_id_check
                        lexico();
                        comprueba_id=0;
                        break;
                    case 11: // _no_null_check
                        lexico();
                        comprueba_null=0;
                        break;
                    default:
                        error(0,8); // se esperaba una opción de compilación
                        break;
                }
            } else {
                if (!free_sintax) {
                    if (pieza==p_program) {
                    	error(3,9); // se esperaba ';' (¡creo!)
                    	//lexico();
                    	break;
                    }
                    else {
                    	error(0,8); // se esperaba una opción de compilación
                    	//lexico();
                    	// puede ser peligroso
                    	while(pieza!=p_coma && pieza!=p_ptocoma && pieza!=p_program)
                    		lexico();
                    }
                } else break;
            }
        } while (1);
    }

	//
	// Cabecera
	//
	// p_program p_id {;}
	//

	save_error(0);
	if (pieza!=p_program && pieza!=p_setup_program) error(4,44); // esperando PROGRAM

	if (pieza==p_setup_program)
		warning(1); // característica desfasada

	//if ((lins=fopen(cWork,"wb"))==NULL) c_error(0,0);	// cWork="system\exec.ins"

	inicio_sentencia(); lexico();
	if (pieza!=p_id) error(1,45);	// esperando el nombre del programa
	
	ob=o;
	if ((*ob).tipo!=tnone) error(0,30); // el nombre no es nuevo
	(*ob).tipo=tproc;
	(*ob).proc.bloque=bloque_actual=ob;
	(*ob).proc.offset=0;
	(*ob).proc.num_par=0;
	lexico();
	if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
	while (pieza==p_ptocoma || pieza==p_coma) lexico();
	final_sentencia();

	//
	// Import (DESFASADO), se ignora
	//
	// {p_import p_lit {;}}
	//

	while (pieza==p_import) {
		warning(2);	// sintaxis antigua
		lexico();
		if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
			error(1,46); // se esperaba un literal
		lexico();
		if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
		while (pieza==p_ptocoma || pieza==p_coma) lexico();
	}

	//
	// Zona de constantes
	//
	// [ p_const {;} { p_id p_asig <exp> {; | ,} } ]
	//

	if (pieza==p_const) {
		pasa_ptocoma();
		while (pieza==p_id) {
			ob=o;
			if ((*ob).tipo!=tnone && (*ob).tipo!=tcons) error(0,30); // el nombre no es nuevo
			(*ob).tipo=tcons;
			lexico(); if (pieza!=p_asig) error(3,7); // esperando '='
			
			lexico();
			if (pieza==p_lit || (pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
				(*ob).cons.literal=1;
			else
				(*ob).cons.literal=0;
				
			(*ob).cons.valor=constante();
			if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
			while (pieza==p_ptocoma || pieza==p_coma) lexico();
		}
	}

    //
    // Variables globales, pueden usarse en expresiones constantes
    //
    // <variable> := p_id [ p_asig <exp> ] {;}
    //
    // <tabla> := p_id p_corab ( p_corce p_asig <init>
    //                       | <exp> p_corce [ p_asig <init> ] ) {;}
    //
    // <init>  := ( <string> | <exp> [ [p_dup] p_abrir <init> p_cerrar ] )
    //            [ p_coma <init>]
    //

    if (pieza==p_global) {
      pasa_ptocoma();
      while (pieza==p_id || pieza==p_int || pieza==p_pointer || pieza==p_struct || pieza==p_string || pieza==p_byte || pieza==p_word) {

        if (pieza==p_struct) {                            // Struct global

          lexico();

          if (pieza==p_pointer) { // Se define un puntero a struct

            lexico(); if (pieza!=p_id) error(1,27); ob=o; // esperando el nombre de la estructura
            if ((*ob).tipo==tnone) error(0,28); // No se define el pointer así
            if ((*ob).tipo!=tsglo && (*ob).tipo!=tsloc) error(0,28);
            lexico();
            puntero_a_struct:
            analiza_pointer_struct(tpsgl,imem,ob);
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,imem++);

            if (pieza==p_coma) {
              lexico();
              if (pieza==p_pointer) lexico();
              goto puntero_a_struct;
            } else {
              if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
              while (pieza==p_ptocoma || pieza==p_coma) lexico();
            }

          } else {

            if (pieza!=p_id) error(1,27); // esperando el nombre de la estructura
            ob=o; member=ob; lexico();

            if ((*ob).tipo!=tnone) error(2,30); // el nombre no es nuevo

            (*ob).tipo=tsglo; (*ob).sglo.offset=_imem=imem;
            if (pieza==p_corab) {
              member2=member; member=NULL; lexico();
              if (((*ob).sglo.items1=constante())<0) error(4,43); // estructura de longitud negativa
              if (pieza==p_coma) {
                lexico();
                if (((*ob).sglo.items2=constante())<0) error(4,43); // idem
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).sglo.items3=constante())<0) error(4,43); // reidem
                } else (*ob).sglo.items3=-1;
              } else { (*ob).sglo.items2=-1; (*ob).sglo.items3=-1; }
              member=member2;
              (*ob).sglo.totalitems=(*ob).sglo.items1+1;
              if ((*ob).sglo.items2>-1) (*ob).sglo.totalitems*=(*ob).sglo.items2+1;
              if ((*ob).sglo.items3>-1) (*ob).sglo.totalitems*=(*ob).sglo.items3+1;
              if (pieza!=p_corce) error(3,19); lexico(); // espernado ']'
            } else {
              (*ob).sglo.totalitems=1;
              (*ob).sglo.items1=0; (*ob).sglo.items2=-1; (*ob).sglo.items3=-1;
            }
            if (((*ob).sglo.len_item=analiza_struct(_imem))==0) error(0,47); // estructua vacia
            member=NULL; lexico();
            imem=(*ob).sglo.offset; dup=(*ob).sglo.totalitems+1;
            if (dup>1) {
              test_buffer(&mem,&imem_max,imem+(*ob).sglo.len_item*(*ob).sglo.totalitems);
              test_buffer(&frm,&ifrm_max,imem+(*ob).sglo.len_item*(*ob).sglo.totalitems);
              while (--dup) {
                memcpy(&mem[imem],&mem[_imem],(*ob).sglo.len_item<<2);
                memcpy(&frm[imem],&frm[_imem],(*ob).sglo.len_item<<2);
                imem+=(*ob).sglo.len_item;
              }
            } imem=_imem;
            if (pieza==p_asig) {
              save_error(1);
              lexico(); tglo_init(0);
              if (imem-_imem-1>=(*ob).sglo.len_item*(*ob).sglo.totalitems) error(4,55); // demasiados valores para la estructura
            } while (pieza==p_ptocoma) lexico();

            imem=_imem+(*ob).sglo.len_item*(*ob).sglo.totalitems;
            test_buffer(&mem,&imem_max,imem);
          }

        } else if (pieza==p_string) {                     // Cadena global

          lexico();

          if (pieza==p_pointer) { // Puntero a byte

            lexico(); puntero1:
            ob=analiza_pointer(tpcgl,imem);
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,++imem);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero1;
            }

          } else {

            if (pieza!=p_id) error(1,29); // esperando el nombre de la cadena
            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico();	// el nombre no es nuevo
            (*ob).tipo=tcglo;
            _imem=imem;
            (*ob).cglo.offset=_imem+1;
            if (pieza==p_corab) {
              lexico();
              if (pieza==p_corce) {
                lexico();
                (*ob).cglo.totalen=255;
              } else {
                if (((*ob).cglo.totalen=constante())<0) error(4,31); // cadena de long. negativa
                if ((*ob).cglo.totalen>0xFFFFF) error(4,32); // cadena demasiado larga
                if (pieza!=p_corce) error(3,19); // esperando ']'
                lexico();
              }
            } else (*ob).cglo.totalen=255;
            if (pieza==p_asig) {
              save_error(1);
              _itxt=itxt;
              lexico();
              if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
                error(3,46); // se esperaba un literal
              if (strlen((char*)&mem[pieza_num])>(*ob).cglo.totalen+1)
                error(4,49); // literal demasiado largo
              imem=_imem+1+((*ob).cglo.totalen+5)/4; // ej. c[32] -> c[0]..c[32],NUL
              test_buffer(&mem,&imem_max,imem);
              strcpy((char*)&mem[_imem+1],(char*)&mem[pieza_num]);
              itxt=_itxt; // Saca la cadena del segmento de textos
              lexico();
            } else {
              imem=_imem+1+((*ob).cglo.totalen+5)/4;
              test_buffer(&mem,&imem_max,imem);
            } mem[_imem]=0xDAD00000|(*ob).cglo.totalen;
          }

          if (pieza==p_coma) pieza=p_string; else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else if (pieza==p_byte) {                       // Tabla Byte global

          lexico();

          if (pieza==p_pointer) { // Puntero a byte

            lexico(); puntero2:
            ob=analiza_pointer(tpbgl,imem);
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,++imem);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero2;
            }

          } else {

            if (pieza!=p_id) error(1,23); // esperando un nombre
            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            (*ob).tipo=tbglo; (*ob).bglo.offset=_imem=imem;
            if (pieza==p_corab) {
              lexico();
              if (pieza==p_corce) {
                lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
                oimemptr=(byte*)&mem[imem];
                tglo_init(2);
                (*ob).bglo.len1=imemptr-oimemptr-1;
                (*ob).bglo.len2=-1;
                (*ob).bglo.len3=-1;
                (*ob).bglo.totalen=((*ob).bglo.len1+4)/4;
              } else {
                if (((*ob).bglo.len1=constante())<0) error(4,40); // tabla de longitud negativa
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).bglo.len2=constante())<0) error(4,40); // idem
                  if (pieza==p_coma) {
                    lexico();
                    if (((*ob).bglo.len3=constante())<0) error(4,40); // reidem
                  } else (*ob).bglo.len3=-1;
                } else { (*ob).bglo.len2=-1; (*ob).bglo.len3=-1; }

                (*ob).bglo.totalen=(*ob).bglo.len1+1;
                if ((*ob).bglo.len2>-1) (*ob).bglo.totalen*=(*ob).bglo.len2+1;
                if ((*ob).bglo.len3>-1) (*ob).bglo.totalen*=(*ob).bglo.len3+1;
                if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
                if (pieza==p_asig) {
                  save_error(1);
                  lexico();
                  oimemptr=(byte*)&mem[imem];
                  tglo_init(2);
                  if (imemptr-oimemptr>(*ob).bglo.totalen) error(4,48); // demasiados valores para la tabla
                } (*ob).bglo.totalen=((*ob).bglo.totalen+3)/4;
              }
            } else {                                          // Byte global
              (*ob).tipo=tbglo; (*ob).bglo.offset=imem;
              (*ob).bglo.len1=0;
              (*ob).bglo.len2=-1;
              (*ob).bglo.len3=-1;
              (*ob).bglo.totalen=1; // 1 int
              if (pieza==p_asig) {
                save_error(1);
                lexico();
                mem[imem]=constante();
                if (mem[imem]<0 || mem[imem]>255) error(4,50); // valor byte fuera de rango
              }
            } imem=_imem+(*ob).bglo.totalen; test_buffer(&mem,&imem_max,imem);
          }

          if (pieza==p_coma) pieza=p_byte; else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else if (pieza==p_word) {                       // Tabla Word global

          lexico();

          if (pieza==p_pointer) { // Puntero a word

            lexico(); puntero3:
            ob=analiza_pointer(tpwgl,imem);
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,++imem);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero3;
            }

          } else {

            if (pieza!=p_id) error(1,23); // esperando un nombre
            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            (*ob).tipo=twglo; (*ob).wglo.offset=_imem=imem;
            if (pieza==p_corab) {
              lexico();
              if (pieza==p_corce) {
                lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
                oimemptr=(byte*)&mem[imem];
                tglo_init(1);
                (*ob).wglo.len1=(imemptr-oimemptr)/2-1;
                (*ob).wglo.len2=-1;
                (*ob).wglo.len3=-1;
                (*ob).wglo.totalen=((*ob).wglo.len1+2)/2;
              } else {
                if (((*ob).wglo.len1=constante())<0) error(4,40); // tabla de longitud negativa
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).wglo.len2=constante())<0) error(4,40); // idem
                  if (pieza==p_coma) {
                    lexico();
                    if (((*ob).wglo.len3=constante())<0) error(4,40); // reidem
                  } else (*ob).wglo.len3=-1;
                } else { (*ob).wglo.len2=-1; (*ob).wglo.len3=-1; }

                (*ob).wglo.totalen=(*ob).wglo.len1+1;
                if ((*ob).wglo.len2>-1) (*ob).wglo.totalen*=(*ob).wglo.len2+1;
                if ((*ob).wglo.len3>-1) (*ob).wglo.totalen*=(*ob).wglo.len3+1;
                if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
                if (pieza==p_asig) {
                  save_error(1);
                  lexico();
                  oimemptr=(byte*)&mem[imem];
                  tglo_init(1);
                  if (imemptr-oimemptr>(*ob).wglo.totalen*2) error(4,48); // demasiados valores para la tabla
                } (*ob).wglo.totalen=((*ob).wglo.totalen+1)/2;
              }
            } else {                                          // Word global
              (*ob).tipo=twglo; (*ob).wglo.offset=imem;
              (*ob).wglo.len1=0;
              (*ob).wglo.len2=-1;
              (*ob).wglo.len3=-1;
              (*ob).wglo.totalen=1; // 1 int
              if (pieza==p_asig) {
                save_error(1);
                lexico();
                mem[imem]=constante();
                if (mem[imem]<0 || mem[imem]>65535) error(4,51); // valor word fuera de rango
              }
            } imem=_imem+(*ob).wglo.totalen; test_buffer(&mem,&imem_max,imem);
          }

          if (pieza==p_coma) pieza=p_word; else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else if (pieza==p_int || pieza==p_id || pieza==p_pointer) {

          if (pieza==p_int) {
            lexico();
            if (pieza!=p_id && pieza!=p_pointer) error(1,23); // esperando un nombre
          }

          if (pieza==p_pointer) { // Puntero a int

            lexico(); puntero4:
            ob=analiza_pointer(tpigl,imem);
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,++imem);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero4;
            }

          } else {

            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            if (pieza==p_corab) {                           // Tabla global
              lexico();
              (*ob).tipo=ttglo; (*ob).tglo.offset=_imem=imem;
              if (pieza==p_corce) {
                lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
                tglo_init(3);
                (*ob).tglo.len1=imem-_imem-1;
                (*ob).tglo.len2=-1;
                (*ob).tglo.len3=-1;
                (*ob).tglo.totalen=imem-_imem;
              } else {
                if (((*ob).tglo.len1=constante())<0) error(4,40); // tabla de longitud negativa
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).tglo.len2=constante())<0) error(4,40); // idem
                  if (pieza==p_coma) {
                    lexico();
                    if (((*ob).tglo.len3=constante())<0) error(4,40); // reidem
                  } else (*ob).tglo.len3=-1;
                } else { (*ob).tglo.len2=-1; (*ob).tglo.len3=-1; }
                (*ob).tglo.totalen=(*ob).tglo.len1+1;
                if ((*ob).tglo.len2>-1) (*ob).tglo.totalen*=(*ob).tglo.len2+1;
                if ((*ob).tglo.len3>-1) (*ob).tglo.totalen*=(*ob).tglo.len3+1;
                if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
                if (pieza==p_asig) {
                  save_error(1);
                  lexico(); tglo_init(3);
                  if (imem-_imem>(*ob).tglo.totalen) error(4,48); // demasiados valores para la tabla
                }
              }
              imem=_imem+(*ob).tglo.totalen;
              test_buffer(&mem,&imem_max,imem);

            } else {                                          // Variable global

              (*ob).tipo=tvglo; (*ob).vglo.offset=imem;
              if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
              test_buffer(&mem,&imem_max,++imem);

            }
          }
          if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }
      }
    }

    //
    // Variables locales, no se pueden usar en expresiones constantes
    //
    // <variable> := p_id [ p_asig <exp> ] {;}
    //
    // <tabla> := p_id p_corab ( p_corce p_asig <init>
    //                       | <exp> p_corce [ p_asig <init> ] ) {;}
    //

    if (pieza==p_local) {
      pasa_ptocoma();
      while (pieza==p_id || pieza==p_int || pieza==p_pointer || pieza==p_struct || pieza==p_string || pieza==p_byte || pieza==p_word) {

        if (pieza==p_struct) {

          lexico();

          if (pieza==p_pointer) { // Se define un puntero a struct

            lexico(); if (pieza!=p_id) error(1,27); ob=o; // esperando el nombre de la estructura
            if ((*ob).tipo==tnone) error(0,28); // No se define el pointer así
            if ((*ob).tipo!=tsglo && (*ob).tipo!=tsloc) error(0,28);
            lexico();
            puntero_a_struct_local:
            analiza_pointer_struct(tpslo,iloc,ob);
            if (pieza==p_asig) { lexico(); loc[iloc]=constante(); }
            test_buffer(&loc,&iloc_max,iloc++);

            if (pieza==p_coma) {
              lexico();
              if (pieza==p_pointer) lexico();
              goto puntero_a_struct_local;
            } else {
              if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
              while (pieza==p_ptocoma || pieza==p_coma) lexico();
            }

          } else {

            if (pieza!=p_id) error(1,27); // esperando el nombre de la estructura
            ob=o; member=ob; lexico();

            if ((*ob).tipo!=tnone) error(2,30); // el nombre no es nuevo

            (*ob).tipo=tsloc; (*ob).sloc.offset=_imem=iloc;
            if (pieza==p_corab) {
              member2=member; member=NULL; lexico();
              if (((*ob).sloc.items1=constante())<0) error(4,43); // estructura de longitud negativa
              if (pieza==p_coma) {
                lexico();
                if (((*ob).sloc.items2=constante())<0) error(4,43); // idem
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).sloc.items3=constante())<0) error(4,43); // reidem
                } else (*ob).sloc.items3=-1;
              } else { (*ob).sloc.items2=-1; (*ob).sloc.items3=-1; }
              member=member2;
              (*ob).sloc.totalitems=(*ob).sloc.items1+1;
              if ((*ob).sloc.items2>-1) (*ob).sloc.totalitems*=(*ob).sloc.items2+1;
              if ((*ob).sloc.items3>-1) (*ob).sloc.totalitems*=(*ob).sloc.items3+1;
              if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
            } else {
              (*ob).sloc.totalitems=1;
              (*ob).sloc.items1=0; (*ob).sloc.items2=-1; (*ob).sloc.items3=-1;
            }
            if (((*ob).sloc.len_item=analiza_struct_local(_imem))==0) error(0,47); // estructura vacia
            member=NULL; lexico();
            iloc=(*ob).sloc.offset; dup=(*ob).sloc.totalitems+1;
            if (dup>1) {
              test_buffer(&loc,&iloc_max,iloc+(*ob).sloc.len_item*(*ob).sloc.totalitems);
              test_buffer(&frm,&ifrm_max,imem+(*ob).sloc.len_item*(*ob).sloc.totalitems);
              while (--dup) {
                memcpy(&loc[iloc],&loc[_imem],(*ob).sloc.len_item<<2);
                memcpy(&frm[iloc],&frm[_imem],(*ob).sloc.len_item<<2);
                iloc+=(*ob).sloc.len_item;
              }
            } iloc=_imem;
            if (pieza==p_asig) {
              save_error(1); lexico(); tloc_init(0);
              if (iloc-_imem-1>=(*ob).sloc.len_item*(*ob).sloc.totalitems) error(4,55); // demasiados valores para la estructura
            } while (pieza==p_ptocoma) lexico();
            iloc=_imem+(*ob).sloc.len_item*(*ob).sloc.totalitems;
            test_buffer(&loc,&iloc_max,iloc);
          }

        } else if (pieza==p_string) {                     // Cadena local

          lexico();

          if (pieza==p_pointer) { // Puntero a byte

            lexico(); puntero5:
            ob=analiza_pointer(tpclo,iloc);
            if (pieza==p_asig) { lexico(); loc[iloc]=constante(); }
            test_buffer(&loc,&iloc_max,++iloc);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero5;
            }

          } else {

            if (pieza!=p_id) error(1,29); // esperando el nombre de la cadena
            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            (*ob).tipo=tcloc;
            _imem=iloc;
            (*ob).cloc.offset=_imem+1;
            if (pieza==p_corab) {
              lexico();
              if (pieza==p_corce) {
                lexico();
                (*ob).cloc.totalen=255;
              } else {
                if (((*ob).cloc.totalen=constante())<0) error(4,31); // cadena de long. negativa
                if ((*ob).cloc.totalen>0xFFFFF) error(4,32); // cadena demasiado larga
                if (pieza!=p_corce) error(3,19); // esperando ']'
                lexico();
              }
            } else (*ob).cloc.totalen=255;
            if (pieza==p_asig) {
              save_error(1);
              _itxt=itxt;
              lexico();
              if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
                error(3,46); // se esperaba un literal
              if (strlen((char*)&mem[pieza_num])>(*ob).cloc.totalen+1)
                error(4,49); // literal demasiado largo
              iloc=_imem+1+((*ob).cloc.totalen+5)/4; // ej. c[32] -> c[0]..c[32],NUL
              test_buffer(&loc,&iloc_max,iloc);
              strcpy((char*)&loc[_imem+1],(char*)&mem[pieza_num]);
              itxt=_itxt; // Saca la cadena del segmento de textos
              lexico();
            } else {
              iloc=_imem+1+((*ob).cloc.totalen+5)/4;
              test_buffer(&loc,&iloc_max,iloc);
            } loc[_imem]=0xDAD00000|(*ob).cloc.totalen;
          }

          if (pieza==p_coma) pieza=p_string; else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else if (pieza==p_byte) {                       // Tabla Byte local

          lexico();

          if (pieza==p_pointer) { // Puntero a byte

            lexico(); puntero6:
            ob=analiza_pointer(tpblo,iloc);
            if (pieza==p_asig) { lexico(); loc[iloc]=constante(); }
            test_buffer(&loc,&iloc_max,++iloc);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero6;
            }

          } else {

            if (pieza!=p_id) error(1,23); // esperando un nombre
            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            (*ob).tipo=tbloc; (*ob).bloc.offset=_imem=iloc;
            if (pieza==p_corab) {
              lexico();
              if (pieza==p_corce) {
                lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
                oimemptr=(byte*)&loc[iloc];
                tloc_init(2);
                (*ob).bloc.len1=imemptr-oimemptr-1;
                (*ob).bloc.len2=-1;
                (*ob).bloc.len3=-1;
                (*ob).bloc.totalen=((*ob).bloc.len1+4)/4;
              } else {
                if (((*ob).bloc.len1=constante())<0) error(4,40); // tabla de long. negativa
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).bloc.len2=constante())<0) error(4,40); // idem
                  if (pieza==p_coma) {
                    lexico();
                    if (((*ob).bloc.len3=constante())<0) error(4,40); // reidem
                  } else (*ob).bloc.len3=-1;
                } else { (*ob).bloc.len2=-1; (*ob).bloc.len3=-1; }

                (*ob).bloc.totalen=(*ob).bloc.len1+1;
                if ((*ob).bloc.len2>-1) (*ob).bloc.totalen*=(*ob).bloc.len2+1;
                if ((*ob).bloc.len3>-1) (*ob).bloc.totalen*=(*ob).bloc.len3+1;
                if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
                if (pieza==p_asig) {
                  save_error(1);
                  lexico();
                  oimemptr=(byte*)&loc[iloc];
                  tloc_init(2);
                  if (imemptr-oimemptr>(*ob).bloc.totalen) error(4,48); // demasiados valores para la tabla
                } (*ob).bloc.totalen=((*ob).bloc.totalen+3)/4;
              }
            } else {                                          // Byte local
              (*ob).tipo=tbloc; (*ob).bloc.offset=iloc;
              (*ob).bloc.len1=0;
              (*ob).bloc.len2=-1;
              (*ob).bloc.len3=-1;
              (*ob).bloc.totalen=1; // 1 int
              if (pieza==p_asig) {
                save_error(1);
                lexico();
                loc[iloc]=constante();
                if (loc[iloc]<0 || loc[iloc]>255) error(4,50); // valor byte fuera de rango
              }
            } iloc=_imem+(*ob).bloc.totalen; test_buffer(&loc,&iloc_max,iloc);
          }

          if (pieza==p_coma) pieza=p_byte; else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else if (pieza==p_word) {                       // Tabla Word local

          lexico();

          if (pieza==p_pointer) { // Puntero a byte

            lexico(); puntero7:
            ob=analiza_pointer(tpwlo,iloc);
            if (pieza==p_asig) { lexico(); loc[iloc]=constante(); }
            test_buffer(&loc,&iloc_max,++iloc);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero7;
            }

          } else {

            if (pieza!=p_id) error(1,23); // esperando un nombre
            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            (*ob).tipo=twloc; (*ob).wloc.offset=_imem=iloc;
            if (pieza==p_corab) {
              lexico();
              if (pieza==p_corce) {
                lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
                oimemptr=(byte*)&loc[iloc];
                tloc_init(1);
                (*ob).wloc.len1=(imemptr-oimemptr)/2-1;
                (*ob).wloc.len2=-1;
                (*ob).wloc.len3=-1;
                (*ob).wloc.totalen=((*ob).wloc.len1+2)/2;
              } else {
                if (((*ob).wloc.len1=constante())<0) error(4,40); // tabla de long. negativa
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).wloc.len2=constante())<0) error(4,40); // idem
                  if (pieza==p_coma) {
                    lexico();
                    if (((*ob).wloc.len3=constante())<0) error(4,40); // reidem
                  } else (*ob).wloc.len3=-1;
                } else { (*ob).wloc.len2=-1; (*ob).wloc.len3=-1; }

                (*ob).wloc.totalen=(*ob).wloc.len1+1;
                if ((*ob).wloc.len2>-1) (*ob).wloc.totalen*=(*ob).wloc.len2+1;
                if ((*ob).wloc.len3>-1) (*ob).wloc.totalen*=(*ob).wloc.len3+1;
                if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
                if (pieza==p_asig) {
                  save_error(1);
                  lexico();
                  oimemptr=(byte*)&loc[iloc];
                  tloc_init(1);
                  if (imemptr-oimemptr>(*ob).wloc.totalen*2) error(4,48); // demasiados valores para la tabla
                } (*ob).wloc.totalen=((*ob).wloc.totalen+1)/2;
              }
            } else {                                          // Word local
              (*ob).tipo=twloc; (*ob).wloc.offset=iloc;
              (*ob).wloc.len1=0;
              (*ob).wloc.len2=-1;
              (*ob).wloc.len3=-1;
              (*ob).wloc.totalen=1; // 1 int
              if (pieza==p_asig) {
                save_error(1);
                lexico();
                loc[iloc]=constante();
                if (loc[iloc]<0 || loc[iloc]>65535) error(4,51); // valor word fuera de rango
              }
            } iloc=_imem+(*ob).wloc.totalen; test_buffer(&loc,&iloc_max,iloc);
          }

          if (pieza==p_coma) pieza=p_word; else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else if (pieza==p_int || pieza==p_id || pieza==p_pointer) {

          if (pieza==p_int) {
            lexico();
            if (pieza!=p_id && pieza!=p_pointer) error(1,23); // esperando un nombre
          }

          if (pieza==p_pointer) { // Miembro puntero a int

            lexico(); puntero8:
            ob=analiza_pointer(tpilo,iloc);
            if (pieza==p_asig) { lexico(); loc[iloc]=constante(); }
            test_buffer(&loc,&iloc_max,++iloc);
            if (pieza==p_coma) {
              lexico(); if (pieza==p_pointer) lexico();
              goto puntero8;
            }

          } else {

            ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
            if (pieza==p_corab) {                             // Tabla local
              lexico();
              (*ob).tipo=ttloc; (*ob).tloc.offset=_imem=iloc;
              if (pieza==p_corce) {
                lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
                tloc_init(3);
                (*ob).tloc.len1=iloc-_imem-1;
                (*ob).tloc.len2=-1;
                (*ob).tloc.len3=-1;
                (*ob).tloc.totalen=iloc-_imem;
              } else {
                if (((*ob).tloc.len1=constante())<0) error(4,40); // tabla de long. negativa
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).tloc.len2=constante())<0) error(4,40); // idem
                  if (pieza==p_coma) {
                    lexico();
                    if (((*ob).tloc.len3=constante())<0) error(4,40); // reidem
                  } else (*ob).tloc.len3=-1;
                } else { (*ob).tloc.len2=-1; (*ob).tloc.len3=-1; }
                (*ob).tloc.totalen=(*ob).tloc.len1+1;
                if ((*ob).tloc.len2>-1) (*ob).tloc.totalen*=(*ob).tloc.len2+1;
                if ((*ob).tloc.len3>-1) (*ob).tloc.totalen*=(*ob).tloc.len3+1;
                if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
                if (pieza==p_asig) {
                  save_error(1);
                  lexico(); tloc_init(3);
                  if (iloc-_imem>(*ob).tloc.totalen) error(4,48); } // demasiados valores para la tabla
              }
              iloc=_imem+(*ob).tloc.totalen;
              test_buffer(&loc,&iloc_max,iloc);

            } else {                                          // Variable local

              (*ob).tipo=tvloc; (*ob).vloc.offset=iloc;
              if (pieza==p_asig) { lexico(); loc[iloc]=constante(); }
              test_buffer(&loc,&iloc_max,++iloc);

            }
          }
          if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }
      }
    }

    //
    // Código principal
    //

    // Genera el salto al inicio del código (long_header)

    mem[0]=0; mem[1]=imem; iloc_len=iloc;

    g2(ltyp,(int)bloque_actual);
    g2(lcbp,0);
    inicio=0; final=imem-1;
    grabar_sentencia();

    // otra vez import... se ha usado sintaxis de DIV 1.
    // de nuevo le decimos k se lo meta por las narices...

    if (pieza==p_import) {
		warning(2);	// sintaxis antigua
		lexico();
		if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
			error(1,46); // se esperaba un literal
		lexico();
		if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
		while (pieza==p_ptocoma || pieza==p_coma) lexico();
	}

    // esta instrucción realiza un salto a la rutina donde están los limp
    // (importación de DLLs). Como esta la colocaremos al final (porque, hasta
    // que lleguemos al final no sabremos qué dlls hay que importar), dejamos
    // el parámetro de ljmp de momento a 0 y guardamos el offset en
    // salto_import, donde al final sustituiremos el 0 por el offset adecuado.

    salto_import=imem+1;
    g2(ljmp,0);

	// El primer FRAME, y la carga de variables PRIVATE, se ejecutan
	// conjuntamente en el BEGIN del programa principal.

	final=imem;

    parametros=-1;
    bloque_lexico=bloque_actual;
    n=iloc;
    analiza_private();
    if (iloc>iloc_len) iloc_len=iloc;
    iloc=n;
    parametros=0;

	g1(lfrm);

    if (pieza!=p_begin) error(0,56);
    inicio_sentencia(); inicio=final;
    lexico(); final_sentencia(); grabar_sentencia();

    sentencia();

    if (pieza!=p_end) error(0,57); inicio_sentencia();
    g1(lret);
    pasa_ptocoma(); final_sentencia(); grabar_sentencia();


    //
    // Procesos
    //
    // p_process p_id p_abrir [ id { , id } ] p_cerrar { ; } p_begin ... p_end
    //

    save_error(0);
    while (pieza==p_process || pieza==p_function) {
      n=pieza; inicio_sentencia(); lexico();
      if (pieza!=p_id) error(1,70); // esperando el nombre del proceso o funcion
      ob=o; lexico();
      if ((*ob).tipo==tproc && (*ob).usado) {
        num_par=(*ob).proc.num_par; bloque_lexico=bloque_actual=ob;
        _imem=(*ob).proc.offset; while(_imem) {
           _imem_old=mem[_imem]; mem[_imem]=imem; _imem=_imem_old; }
      } else if ((*ob).tipo==tnone) {
        (*ob).usado=0; (*ob).tipo=tproc;
        (*ob).proc.bloque=bloque_lexico=bloque_actual=ob;
      } else error(2,30); // el nombre no es nuevo
      (*ob).proc.offset=imem; (*ob).proc.num_par=0;

      g2(ltyp,(int)bloque_actual);
      if (n==p_function) g1(lnop);
      g2(lcbp,0); _imem=imem-1;
      if (pieza!=p_abrir) error(3,22); // esperando '('

      parametros=1; n=iloc; lexico();

      while (pieza!=p_cerrar) {
        (*ob).proc.num_par++; expresion_cpa();
        if (pieza!=p_cerrar) if (pieza!=p_coma) error(3,35); // se esperaba una coma
          else { lexico(); if (pieza==p_cerrar) error(3,36); } // se esperaba otro parámetro
      }
      if ((*ob).usado) {
        if (num_par==(*ob).proc.num_par) (*ob).usado=0; else error(0,38); // nº de parametros incorrecto
      }
      pasa_ptocoma(); final_sentencia();

      if (parametros>1) {
        g2(lpar,parametros-1);
      }

      parametros=-1; // Para que los parámetros se puedan repetir como PRIVATE

      num_par=mem[_imem]=(*ob).proc.num_par;

      analiza_private(); parametros=0;

      if (iloc>iloc_len) iloc_len=iloc; iloc=n;

      if (pieza!=p_begin) error(0,56); lexico(); // esperando BEGIN
      final=imem-1; grabar_sentencia();
      sentencia();
      if (pieza!=p_end) error(0,57); // esperando END
      inicio_sentencia();
      g1(lret);
      pasa_ptocoma(); final_sentencia(); grabar_sentencia();

      save_error(0);
    }

    if (pieza!=p_ultima) error(4,71); // esperando PROCESS o FUNCTION

}

//-----------------------------------------------------------------------------
//      Lexico (lee una nueva pieza del *source)
//-----------------------------------------------------------------------------

void lexico(void)
{
	struct objeto * * ptr_o;
	byte ** ptr, * _ivnom, h, * _source=source;
	struct lex_ele * e;

	int n,i;

	if (!coment) {
		old_linea=linea;
		old_ierror=ierror;
		old_ierror_end=ierror_end;
	}

lex_scan:

	ierror=_source;

	switch ((int)lex_case[*_source]) { // Puntero a un lex_ele o l_???

		case l_err:
			if (coment) { pieza=p_rem; _source++; }
			else error(0,5);	// carácter no reconocido
			_source++;
			break;
			
		case l_eof:
			pieza=p_ultima;
			break;

		case l_cr:
			linea++;	
			if (*_source==cr && *(_source+1)==lf) _source+=2;
			else _source++;
				
			ultima_linea=_source;
			goto lex_scan;

		case l_id :
			if (coment) { pieza=p_rem; _source++; break; }

			_ivnom=ivnom.b;
			*ivnom.p++=0;
			*ivnom.p++=0;
			h=0;

			while (*ivnom.b=lower[*_source++])
				h=((byte)(h<<1)+(h>>7))^(*ivnom.b++);

			ivnom.b++;
			_source--;
			if (ivnom.b-vnom>max_obj*long_med_id) error(0,2); // Excedida la capacidad del vector de nombres

			ptr=&vhash[h];
			
			while (*ptr && strcmp((char *)(ptr+2),(char *)_ivnom+8))
				ptr=(byte**)*ptr;

			if (!strcmp((byte *)(ptr+2),_ivnom+8)) { // id encontrado
				ivnom.b=_ivnom; // lo saca de vnom
				pieza=(int)*(ptr+1);
				
				if (pieza<256 && pieza>=0) { // palabra reservada (token)

					if (pieza==p_rem) {
						while (*_source!=cr && *_source!=lf) _source++;
						goto lex_scan;
					}

				}
				else { // objeto (id anterior)
					
					ptr_o=(void*)(ptr+1);
					o=*ptr_o;
					pieza=p_id;
					
					while(o!=NULL && ( ((*o).bloque && bloque_lexico!=(*o).bloque) ||
					 ((*o).member!=member) ))
					 	o=(*o).anterior;

   					if(o==NULL) { // No encontrado
   						o=iobj++; (*o).anterior=*ptr_o; *ptr_o=o;
   						(*o).name=(byte*)(ptr_o+1);
   						(*o).member=member;
   						(*o).param=0;
						(*o).dll=-1;
   						if (parametros) (*o).bloque=bloque_actual;
   						if (num_obj++==max_obj)
   							error(0,6); // excedida la capacidad de la tabla de objetos
   					}
   					else {
						//printf("->->-> %s\n",(char*)_ivnom+8);
   						if ((*o).tipo==tcons)
   							pieza_num=(*o).cons.valor;
						if((*o).dll!=-1) {
							for(i=0;i<numdlls;i++) {
								if(nuevo_orden[i]==(*o).dll) {
   									dlls[i].usado=1;
									break;
								}
							}
						}
   					}
   				}
			}
			else {						// id nuevo
				*ptr=_ivnom;
				ptr_o=(void*)(_ivnom+4);
				*ptr_o=o=iobj++;
				pieza=p_id;
				
				(*o).name=(byte*)_ivnom+8;
				(*o).member=member;
				(*o).dll=-1;
				if (parametros) (*o).bloque=bloque_actual;
				if (num_obj++==max_obj)
					error(0,6);	// excedida la capacidad de la tabla de objetos
			}
			
			break;

		case l_spc:
			while ((*++_source)==' ');
			goto lex_scan;

		case l_lit:
			if (coment) { pieza=p_rem; _source++; break; }
			
			pieza=p_lit;	// Literal entre dos h
			h=*_source;
			_ivnom=ivnom.b;

			do {
				_source++;
				if (*_source==cr || *_source==lf) {
					error(0,3);	// literal sin cerrar
					break;
				}

				if (*_source==h) {
					if (*(_source+1)==h) *_ivnom=*++_source;
					else *_ivnom=0;
				} else *_ivnom=*_source;

			} while (*_ivnom++);
			
			_source++;
			next_lexico(_source,0,0);

			while (next_pieza==p_lit) {
				next_lexico(_source,0,linea);
				linea=next_linea;
				_source=next_source;
				h=*_source; _ivnom--;
				do {
					_source++;
					if (*_source==cr || *_source==lf) {
						error(0,3);	// literal sin cerrar
						break;
					}
						
					if (*_source==h) {
						if (*(_source+1)==h) *_ivnom=*++_source;
						else *_ivnom=0;
					} else *_ivnom=*_source;
					
				} while (*_ivnom++);
				
				_source++;
				next_lexico(_source,0,0);
			}

			n=(strlen(ivnom.b)+4)/4;
			memcpy(&mem_ory[itxt],ivnom.b,strlen(ivnom.b)+1);

			pieza_num=itxt;
			itxt+=n;
			
			break;

		case l_num:
		
			if (coment) { pieza=p_rem; _source++; break; }

			pieza=p_num;
			pieza_num=0;

			// Número hexadecimal
			if (*_source=='0' && lower[*(_source+1)]=='x') {
				_source+=2;
				while ((int)lex_case[*_source]==l_num ||
				 (lower[*_source]>='a' && lower[*_source]<='f')) {
					if ((int)lex_case[*_source]==l_num)
						pieza_num=pieza_num*16+*_source++-0x30;
					else
						pieza_num=pieza_num*16+lower[*_source++]-'a'+10;
				}
			}
			else do {
				pieza_num=pieza_num*10+*_source++-0x30;
			} while ((int)lex_case[*_source]==l_num);

			break;

		default: // puntero a un lex_ele

			e=lex_case[*_source++];
			_ivnom=_source;
			pieza=(*e).token;
			
			while (e=(*e).siguiente) {
			
				while (*_source!=(*e).caracter && (*e).alternativa)
					e=(*e).alternativa;
					
				if (*_source++==(*e).caracter && (*e).token) {
					pieza=(*e).token;
					_ivnom=_source;
				}
			}
			
			_source=_ivnom;

			if (pieza==p_rem && !coment) {
				while (*_source!=cr && *_source!=lf) _source++;
				goto lex_scan;
			}
			
			if (pieza==p_ini_rem) {
				coment++;
				do {
					source=_source;
					lexico();
					_source=source;
				} while (pieza!=p_end_rem);
				coment--;
				goto lex_scan;
			}

			if (pieza==p_ultima) {
				if (coment) error(0,1);	// llegó el final dentro de un comentario
				else error(0,4);		// símbolo no reconocido (¡¡creo!!)
			}

			break;

	}
	
	source=_source;
	ierror_end=_source-1;
}


//-----------------------------------------------------------------------------
//  Adivina cual será la siguiente pieza lexica leida (y donde estará)
//-----------------------------------------------------------------------------

// No genera nunca errores

byte * next_lexico(byte * _source, int coment, int linea)
{
	byte ** ptr, * _ivnom, h;
	struct lex_ele * e;

	if (!coment && linea) {
		old_linea=linea;
		old_ierror=ierror;
		old_ierror_end=ierror_end;
	}

lex_scan:

	if (linea) ierror=_source;
	next_source=_source;

	switch ((int)lex_case[*_source]) {

		case l_err:
			if (coment) { _source++; goto lex_scan; }
			next_pieza=0;
			break;

		case l_cr :
			if (linea) {
				
				linea++;
				if (*_source==cr && *(_source+1)==lf) _source+=2;
				else _source++;
				
				ultima_linea=_source;
				goto lex_scan;
			
			}
			else {

				if (*_source==cr && *(_source+1)==lf) _source+=2;
				else _source++;
				
				goto lex_scan;

			}
			
		case l_eof:
			next_pieza=0;
			break;

		case l_id :		
			if (coment) { _source++; goto lex_scan; }
			
			_ivnom=ivnom.b;
			*ivnom.p++=0;
			*ivnom.p++=0;
			h=0;
			
			while (*ivnom.b=lower[*_source++])
				h=((byte)(h<<1)+(h>>7))^(*ivnom.b++);

			ivnom.b++;
			_source--;
			ptr=&vhash[h];

			while (*ptr && strcmp((char *)(ptr+2),(char *)_ivnom+8))
				ptr=(unsigned char **)*ptr;

			if (!strcmp((char *)(ptr+2),(char *)_ivnom+8)) { // id encontrado
				ivnom.b=_ivnom; // lo saca de vnom
				next_pieza=(int)*(ptr+1);
				if (next_pieza<256 && next_pieza>=0) { // palabra reservada (token)
					if (next_pieza==p_rem) {
						while (*_source!=cr) _source++;
						goto lex_scan;
					}
				} else { // objeto (id anterior)
					next_pieza=p_id;
				}
			} else {
				ivnom.b=_ivnom; // lo saca de vnom
				next_pieza=p_id; // id nuevo
			}
			
			break;

		case l_spc:
			while ((*++_source)==' ');
			goto lex_scan;

		case l_lit:
			if (coment) { _source++; goto lex_scan; }
			next_pieza=p_lit;
			break;

		case l_num:
			if (coment) { _source++; goto lex_scan; }
			next_pieza=p_num;
			break;

		default: // puntero a un lex_ele
			e=lex_case[*_source++];
			next_pieza=(*e).token;
			_ivnom=_source;

			while (e=(*e).siguiente) {
				while (*_source!=(*e).caracter && (*e).alternativa)
					e=(*e).alternativa;
					
				if (*_source++==(*e).caracter && (*e).token) {
					next_pieza=(*e).token;
					_ivnom=_source;
				}
			}
			
			_source=_ivnom;

			if (next_pieza==p_rem && !coment) {
				while (*_source!=cr && *_source!=lf) _source++;
				goto lex_scan;
			}

			if (next_pieza==p_ini_rem) {
				coment++;
				do {
					_source=next_lexico(_source,coment,linea);
					linea=next_linea;
				} while (next_pieza!=p_end_rem);
				coment--;
				goto lex_scan;
			}
			
			break;

	}
	
	next_linea=linea;
	return(_source);
}


//-----------------------------------------------------------------------------
//      Pasa varias comas o puntos y coma seguidos
//-----------------------------------------------------------------------------

void pasa_ptocoma(void)
{
  do {
    lexico();
  } while (pieza==p_ptocoma);
}


  //
  // Analiza las variables privadas de un bloque, como las locales
  //

    // *** OJO *** No se debe permitir #id.tvpri
    //             pues fallaría a no ser que #id fuera del mismo tipo que el
    //             proceso actual (hermano)

void analiza_private(void) {
  struct objeto * ob, * member2;
  int _imem,_imem_old,_itxt,dup;
  byte *oimemptr;

  if (pieza==p_private) {
    pasa_ptocoma();

    g2(lpri,0); _imem_old=imem-1;

    while (pieza==p_id || pieza==p_int || pieza==p_pointer || pieza==p_struct || pieza==p_string || pieza==p_byte || pieza==p_word) {

      if (pieza==p_struct) {

        lexico();

        if (pieza==p_pointer) { // Se define un puntero a struct

          lexico(); if (pieza!=p_id) error(1,27); ob=o; // esperando el nombre de la estructura
          if ((*ob).tipo==tnone) error(0,28); // No se define el pointer así
          if ((*ob).tipo!=tsglo && (*ob).tipo!=tsloc) error(0,28);
          lexico();
          puntero_a_struct:
          if (analiza_pointer_struct(tpslo,iloc,ob)==1) {
            iloc++;
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,++imem);
          }

          if (pieza==p_coma) {
            lexico();
            if (pieza==p_pointer) lexico();
            goto puntero_a_struct;
          } else {
            if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
            while (pieza==p_ptocoma || pieza==p_coma) lexico();
          }

        } else {

          if (pieza!=p_id) error(1,27); // esperando el nombre de la estructura
          ob=o; member=ob; lexico();

          if ((*ob).tipo!=tnone) error(2,30); // el nombre no es nuevo

          (*ob).tipo=tsloc; (*ob).sloc.offset=_imem=imem;
          if (pieza==p_corab) {
            member2=member; member=NULL; lexico();
            if (((*ob).sloc.items1=constante())<0) error(4,43); // estructura de long. negativa
            if (pieza==p_coma) {
              lexico();
              if (((*ob).sloc.items2=constante())<0) error(4,43); // idem
              if (pieza==p_coma) {
                lexico();
                if (((*ob).sloc.items3=constante())<0) error(4,43); // reidem
              } else (*ob).sloc.items3=-1;
            } else { (*ob).sloc.items2=-1; (*ob).sloc.items3=-1; }
            member=member2;
            (*ob).sloc.totalitems=(*ob).sloc.items1+1;
            if ((*ob).sloc.items2>-1) (*ob).sloc.totalitems*=(*ob).sloc.items2+1;
            if ((*ob).sloc.items3>-1) (*ob).sloc.totalitems*=(*ob).sloc.items3+1;
            if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
          } else {
            (*ob).sloc.totalitems=1;
            (*ob).sloc.items1=0; (*ob).sloc.items2=-1; (*ob).sloc.items3=-1;
          }
          if (((*ob).sloc.len_item=analiza_struct_private(_imem))==0) error(0,47); // estructura vacia

          member=NULL; lexico();

          imem=(*ob).sloc.offset; dup=(*ob).sloc.totalitems+1;
          if (dup>1) {
            test_buffer(&mem,&imem_max,imem+(*ob).sloc.len_item*(*ob).sloc.totalitems);
            test_buffer(&frm,&ifrm_max,imem+(*ob).sloc.len_item*(*ob).sloc.totalitems);
            while (--dup) {
              memcpy(&mem[imem],&mem[_imem],(*ob).sloc.len_item<<2);
              memcpy(&frm[imem],&frm[_imem],(*ob).sloc.len_item<<2);
              imem+=(*ob).sloc.len_item;
            }
          } imem=_imem;

          if (pieza==p_asig) {
            save_error(1); lexico(); tglo_init(0);
            if (imem-_imem-1>=(*ob).sloc.len_item*(*ob).sloc.totalitems) error(4,55); // demasiados valores para la estructura
          } while (pieza==p_ptocoma) lexico();

          imem=_imem+(*ob).sloc.len_item*(*ob).sloc.totalitems;
          (*ob).sloc.offset=iloc; iloc+=(*ob).sloc.len_item*(*ob).sloc.totalitems;
          test_buffer(&mem,&imem_max,imem);
        }

      } else if (pieza==p_string) {                     // Cadena privada

        lexico();

        if (pieza==p_pointer) { // Puntero a byte

          lexico(); puntero1:
          ob=analiza_pointer(tpclo,iloc);
          if (ob==NULL) continue; else iloc++;
          if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
          test_buffer(&mem,&imem_max,++imem);
          if (pieza==p_coma) {
            lexico(); if (pieza==p_pointer) lexico();
            goto puntero1;
          }

        } else {

          if (pieza!=p_id) error(1,29); // esperando el nombre de la cadena
          ob=o; if ((*ob).tipo!=tnone) { // Mira si se repite un parámetro ...
            if (parametros==-1 && (*ob).param==1 && (*ob).bloque==bloque_actual) {
              if ((*ob).tipo==tcloc) { // Se repite un string
                save_error(0);
                lexico();
                if (pieza==p_corab) {
                  lexico();
                  if (pieza==p_corce) {
                    lexico();
                    dup=255;
                  } else {
                    dup=constante();
                    if (pieza!=p_corce) error(3,19); // esperando ']'
                    lexico();
                  }
                } else dup=255;
                if (dup!=(*ob).cloc.totalen) error(4,41); // la longitud no coincide con la declaración anterior
                else if (pieza==p_asig) error(0,42); // no se puede inicializar un parámetro
                else {
                  while (pieza==p_ptocoma || pieza==p_coma) lexico();
                  (*ob).param++;
                  continue;
                }
              } else error(0,30); // el nombre no es nuevo
            } else error(0,30);
          } else lexico();

          (*ob).tipo=tcloc;
          _imem=imem;
          if (pieza==p_corab) {
            lexico();
            if (pieza==p_corce) {
              lexico();
              (*ob).cloc.totalen=255;
            } else {
              if (((*ob).cloc.totalen=constante())<0) error(4,31); // cadena de long. negativa
              if ((*ob).cloc.totalen>0xFFFFF) error(4,32); // cadena demasiado larga
              if (pieza!=p_corce) error(3,19); // esperando ']'
              lexico();
            }
          } else (*ob).cloc.totalen=255;
          if (pieza==p_asig) {
            save_error(1);
            _itxt=itxt;
            lexico();
            if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
              error(3,46); // se esperaba un literal
            if (strlen((char*)&mem[pieza_num])>(*ob).cloc.totalen+1)
              error(4,49); // literal demasiado largo
            imem=_imem+1+((*ob).cloc.totalen+5)/4; // ej. c[32] -> c[0]..c[32],NUL
            test_buffer(&mem,&imem_max,imem);
            strcpy((char*)&mem[_imem+1],(char*)&mem[pieza_num]);
            itxt=_itxt; // Saca la cadena del segmento de textos
            lexico();
          } else {
            imem=_imem+1+((*ob).cloc.totalen+5)/4;
            test_buffer(&mem,&imem_max,imem);
          } mem[_imem]=0xDAD00000|(*ob).cloc.totalen;
          (*ob).cloc.offset=iloc+1;
          iloc+=1+((*ob).cloc.totalen+5)/4;
        }

        if (pieza==p_coma) pieza=p_string; else {
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }

      } else if (pieza==p_byte) {                       // Tabla Byte privado

        lexico();

        if (pieza==p_pointer) { // Puntero a byte

          lexico(); puntero2:
          ob=analiza_pointer(tpblo,iloc);
          if (ob==NULL) continue; else iloc++;
          if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
          test_buffer(&mem,&imem_max,++imem);
          if (pieza==p_coma) {
            lexico(); if (pieza==p_pointer) lexico();
            goto puntero2;
          }

        } else {

          if (pieza!=p_id) error(1,23); // esperando un nombre
          ob=o; if ((*ob).tipo!=tnone) {
            if (parametros==-1 && (*ob).param==1 && (*ob).bloque==bloque_actual) {
              if ((*ob).tipo==tbloc) { // Se repite un byte parámetro
                lexico();
                if (pieza==p_corab) error(2,33); // no se puede pasar una tabla como parámetro
                else if (pieza==p_asig) error(0,42); // no se puede inicializar un parámetro
                else {
                  while (pieza==p_ptocoma || pieza==p_coma) {
                    lexico();
                  }
                  (*ob).param++;
                  continue;
                }
              } else error(0,30); // el nombre no es nuevo
            } else error(0,30);
          } else lexico();

          (*ob).tipo=tbloc; _imem=imem;
          if (pieza==p_corab) {
            lexico();
            if (pieza==p_corce) {
              lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
              oimemptr=(byte*)&mem[imem];
              tglo_init(2);
              (*ob).bloc.len1=imemptr-oimemptr-1;
              (*ob).bloc.len2=-1;
              (*ob).bloc.len3=-1;
              (*ob).bloc.totalen=((*ob).bloc.len1+4)/4;
            } else {
              if (((*ob).bloc.len1=constante())<0) error(4,40); // tabla de long. negativa
              if (pieza==p_coma) {
                lexico();
                if (((*ob).bloc.len2=constante())<0) error(4,40); // idem
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).bloc.len3=constante())<0) error(4,40); // reidem
                } else (*ob).bloc.len3=-1;
              } else { (*ob).bloc.len2=-1; (*ob).bloc.len3=-1; }

              (*ob).bloc.totalen=(*ob).bloc.len1+1;
              if ((*ob).bloc.len2>-1) (*ob).bloc.totalen*=(*ob).bloc.len2+1;
              if ((*ob).bloc.len3>-1) (*ob).bloc.totalen*=(*ob).bloc.len3+1;
              if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
              if (pieza==p_asig) {
                save_error(1);
                lexico();
                oimemptr=(byte*)&mem[imem];
                tglo_init(2);
                if (imemptr-oimemptr>(*ob).bloc.totalen) error(4,48); // demasiados valores para la tabla
              } (*ob).bloc.totalen=((*ob).bloc.totalen+3)/4;
            }
          } else {                                          // Byte privado
            (*ob).tipo=tbloc; (*ob).bloc.offset=imem;
            (*ob).bloc.len1=0;
            (*ob).bloc.len2=-1;
            (*ob).bloc.len3=-1;
            (*ob).bloc.totalen=1; // 1 int
            if (pieza==p_asig) {
              save_error(1);
              lexico();
              mem[imem]=constante();
              if (mem[imem]<0 || mem[imem]>255) error(4,50); // valor byte fuera de rango
            }
          }
          imem=_imem+(*ob).bloc.totalen; test_buffer(&mem,&imem_max,imem);
          (*ob).bloc.offset=iloc; iloc+=(*ob).bloc.totalen;
        }

        if (pieza==p_coma) pieza=p_byte; else {
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }

      } else if (pieza==p_word) {                       // Tabla Word privada

        lexico();

        if (pieza==p_pointer) { // Puntero a word

          lexico(); puntero3:
          ob=analiza_pointer(tpwlo,iloc);
          if (ob==NULL) continue; else iloc++;
          if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
          test_buffer(&mem,&imem_max,++imem);
          if (pieza==p_coma) {
            lexico(); if (pieza==p_pointer) lexico();
            goto puntero3;
          }

        } else {

          if (pieza!=p_id) error(1,23); // esperando un nombre
          ob=o; if ((*ob).tipo!=tnone) {
            if (parametros==-1 && (*ob).param==1 && (*ob).bloque==bloque_actual) {
              if ((*ob).tipo==twloc) { // Se repite un word parámetro
                lexico();
                if (pieza==p_corab) error(2,33); // no se puede pasar una tabla como parámetro
                else if (pieza==p_asig) error(0,42); // no se puede inicializar un parámetro
                else {
                  while (pieza==p_ptocoma || pieza==p_coma) {
                    lexico();
                  }
                  (*ob).param++;
                  continue;
                }
              } else error(0,30); // el nombre no es nuevo
            } else error(0,30);
          } else lexico();

          (*ob).tipo=twloc; _imem=imem;
          if (pieza==p_corab) {
            lexico();
            if (pieza==p_corce) {
              lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
              oimemptr=(byte*)&mem[imem];
              tglo_init(1);
              (*ob).wloc.len1=(imemptr-oimemptr)/2-1;
              (*ob).wloc.len2=-1;
              (*ob).wloc.len3=-1;
              (*ob).wloc.totalen=((*ob).wloc.len1+2)/2;
            } else {
              if (((*ob).wloc.len1=constante())<0) error(4,40); // tabla de long. negativa
              if (pieza==p_coma) {
                lexico();
                if (((*ob).wloc.len2=constante())<0) error(4,40); // idem
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).wloc.len3=constante())<0) error(4,40); // reidem
                } else (*ob).wloc.len3=-1;
              } else { (*ob).wloc.len2=-1; (*ob).wloc.len3=-1; }

              (*ob).wloc.totalen=(*ob).wloc.len1+1;
              if ((*ob).wloc.len2>-1) (*ob).wloc.totalen*=(*ob).wloc.len2+1;
              if ((*ob).wloc.len3>-1) (*ob).wloc.totalen*=(*ob).wloc.len3+1;
              if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
              if (pieza==p_asig) {
                save_error(1);
                lexico();
                oimemptr=(byte*)&mem[imem];
                tglo_init(1);
                if (imemptr-oimemptr>(*ob).wloc.totalen*2) error(4,48); // demasiados valores para la tabla
              } (*ob).wloc.totalen=((*ob).wloc.totalen+1)/2;
            }
          } else {                                          // Word privado
            (*ob).tipo=twloc; (*ob).wloc.offset=imem;
            (*ob).wloc.len1=0;
            (*ob).wloc.len2=-1;
            (*ob).wloc.len3=-1;
            (*ob).wloc.totalen=1; // 1 int
            if (pieza==p_asig) {
              save_error(1);
              lexico();
              mem[imem]=constante();
              if (mem[imem]<0 || mem[imem]>65535) error(4,51); // valor word fuera de rango
            }
          }
          imem=_imem+(*ob).wloc.totalen; test_buffer(&mem,&imem_max,imem);
          (*ob).wloc.offset=iloc; iloc+=(*ob).wloc.totalen;
        }

        if (pieza==p_coma) pieza=p_word; else {
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }

      } else if (pieza==p_int || pieza==p_id || pieza==p_pointer) {

        if (pieza==p_int) {
          lexico();
          if (pieza!=p_id && pieza!=p_pointer) error(1,23); // esperando un nombre
        }

        if (pieza==p_pointer) { // Miembro puntero a int

          lexico(); puntero4:
          ob=analiza_pointer(tpilo,iloc);
          if (ob==NULL) continue; else iloc++;
          if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
          test_buffer(&mem,&imem_max,++imem);
          if (pieza==p_coma) {
            lexico(); if (pieza==p_pointer) lexico();
            goto puntero4;
          }

        } else {

          // Si el objeto no es tnone, se repite un parámetro o bien es un error

          ob=o; if ((*ob).tipo!=tnone) {
            if (parametros==-1 && (*ob).param==1 && (*ob).bloque==bloque_actual) {
              if ((*ob).tipo==tvloc) { // Se repite una variable local
                lexico();
                if (pieza==p_corab) error(2,33); // no se puede pasar una tabla como parámetro
                else if (pieza==p_asig) error(0,42); // no se puede inicializar un parámetro
                else {
                  while (pieza==p_ptocoma || pieza==p_coma) {
                    lexico();
                  }
                  (*ob).param++;
                  continue;
                }
              } else error(0,30); // el nombre no es nuevo
            } else error(0,30);
          } else lexico();

          if (pieza==p_corab) {                             // Tabla privada
            lexico();
            (*ob).tipo=ttloc; _imem=imem;
            if (pieza==p_corce) {
              lexico(); if (pieza!=p_asig) error(3,7); lexico(); // esperando '='
              tglo_init(3);
              (*ob).tloc.len1=imem-_imem-1;
              (*ob).tloc.len2=-1;
              (*ob).tloc.len3=-1;
              (*ob).tloc.totalen=imem-_imem;
            } else {
              if (((*ob).tloc.len1=constante())<0) error(4,40); // tabla de long. negativa
              if (pieza==p_coma) {
                lexico();
                if (((*ob).tloc.len2=constante())<0) error(4,40); // idem
                if (pieza==p_coma) {
                  lexico();
                  if (((*ob).tloc.len3=constante())<0) error(4,40); // reidem
                } else (*ob).tloc.len3=-1;
              } else { (*ob).tloc.len2=-1; (*ob).tloc.len3=-1; }
              (*ob).tloc.totalen=(*ob).tloc.len1+1;
              if ((*ob).tloc.len2>-1) (*ob).tloc.totalen*=(*ob).tloc.len2+1;
              if ((*ob).tloc.len3>-1) (*ob).tloc.totalen*=(*ob).tloc.len3+1;
              if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
              if (pieza==p_asig) {
                save_error(1); lexico(); tglo_init(3);
                if (imem-_imem>(*ob).tloc.totalen) error(4,48); } // demasiados valores para la tabla
            }
            imem=_imem+(*ob).tloc.totalen;
            test_buffer(&mem,&imem_max,imem);
            (*ob).tloc.offset=iloc; iloc+=(*ob).tloc.totalen;

          } else {                                          // Variable privada

            (*ob).tipo=tvloc; (*ob).vloc.offset=iloc++;
            if (pieza==p_asig) { lexico(); mem[imem]=constante(); }
            test_buffer(&mem,&imem_max,++imem);
          }
        }
        if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      } (*ob).bloque=bloque_actual;
    }

    mem[_imem_old]=imem;

  };

}

//-----------------------------------------------------------------------------
//      Análisis de una declaración pointer (int, word o byte)
//-----------------------------------------------------------------------------

struct objeto * analiza_pointer(int tipo, int offset)
{
  struct objeto * ob;
  int len1,len2,len3;

  if (pieza!=p_id) error(1,23);	// esperando un nombre
  ob=o; if ((*ob).tipo!=tnone) {
    if (parametros==-1 && (*ob).param==1 && (*ob).bloque==bloque_actual) {
      if ((*ob).tipo==tipo) { // Se repite un pointer parámetro como private
        save_error(0); lexico();
        len1=-1; len2=-1; len3=-1;
        if (pieza==p_corab) { lexico();
          if ((len1=constante())<0) error(4,40); // tabla de longitud negativa
          if (pieza==p_coma) {
            lexico();
            if ((len2=constante())<0) error(4,40); // tabla de longitud negativa
            if (pieza==p_coma) {
              lexico();
              if ((len3=constante())<0) error(4,40); // tabla de longitud negativa
            }
          } if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
        }
        if (len1!=(*ob).pilo.len1 || len2!=(*ob).pilo.len2 || len3!=(*ob).pilo.len3) error(4,41); // la longitud no coincide con la declaración anterior
        else if (pieza==p_asig) error(0,42); // no se puede inicializar un parámetro
        else {
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
          (*ob).param++; // No permite volver a redeclararlo
          return(NULL);
        }
      } else error(0,30); // el nombre no es nuevo
    } else error(0,30);
  } else lexico();
  if (parametros==-1) (*ob).bloque=bloque_actual;
  (*ob).pilo.len1=-1; (*ob).pilo.len2=-1; (*ob).pilo.len3=-1;
  if (pieza==p_corab) { lexico();
    if (((*ob).pilo.len1=constante())<0) error(4,40); // tabla de longitud negativa
    if (pieza==p_coma) {
      lexico();
      if (((*ob).pilo.len2=constante())<0) error(4,40); // tabla de longitud negativa
      if (pieza==p_coma) {
        lexico();
        if (((*ob).pilo.len3=constante())<0) error(4,40); // tabla de longitud negativa
      }
    }
    if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
  } (*ob).pilo.totalen=0;
  if ((*ob).pilo.len1>-1) (*ob).pilo.totalen=(*ob).pilo.len1+1;
  if ((*ob).pilo.len2>-1) (*ob).pilo.totalen*=(*ob).pilo.len2+1;
  if ((*ob).pilo.len3>-1) (*ob).pilo.totalen*=(*ob).pilo.len3+1;

  (*ob).tipo=tipo; (*ob).pilo.offset=offset;

  return(ob);
}

//-----------------------------------------------------------------------------

int analiza_struct(int offstruct) { // tras " struct id [ <const> ] " // idðmember
  int len=0,dup,i,_itxt,_imem;
  struct objeto * ob;
  struct objeto * old_member,* member2;
  byte * oimemptr;

  while (pieza==p_ptocoma) lexico();

  while (pieza!=p_end) {

    if (pieza==p_struct) {                              // Miembro struct

      lexico();

      if (pieza==p_pointer) { // Se define un puntero a struct

        old_member=member; member=NULL; lexico(); member=old_member;
        if (pieza!=p_id) error(1,27); ob=o; // esperando el nombre de la estructura

        if ((*ob).tipo==tnone) error(0,28); // No se define el pointer así
        if ((*ob).tipo!=tsglo && (*ob).tipo!=tsloc) error(0,28);
        lexico();
        puntero_a_struct:
        analiza_pointer_struct(tpsgl,len,ob);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;

        if (pieza==p_coma) {
          lexico();
          if (pieza==p_pointer) lexico();
          goto puntero_a_struct;
        } else {
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }

      } else {

        if (pieza!=p_id) error(1,27); // esperando el nombre de la estructura
        ob=o;
        old_member=member; member=ob; lexico();
        if ((*ob).tipo!=tnone) error(2,30); // el nombre no es nuevo

        (*ob).tipo=tsglo; (*ob).sglo.offset=len;
        if (pieza==p_corab) {
          member2=member; member=NULL; lexico();
          if (((*ob).sglo.items1=constante())<0) error(4,43); // estructura de longitud negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).sglo.items2=constante())<0) error(4,43); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).sglo.items3=constante())<0) error(4,43); // reidem
            } else (*ob).sglo.items3=-1;
          } else { (*ob).sglo.items2=-1; (*ob).sglo.items3=-1; }
          member=member2;
          (*ob).sglo.totalitems=(*ob).sglo.items1+1;
          if ((*ob).sglo.items2>-1) (*ob).sglo.totalitems*=(*ob).sglo.items2+1;
          if ((*ob).sglo.items3>-1) (*ob).sglo.totalitems*=(*ob).sglo.items3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
        } else {
          (*ob).sglo.totalitems=1;
          (*ob).sglo.items1=0; (*ob).sglo.items2=-1; (*ob).sglo.items3=-1;
        }
        if (((*ob).sglo.len_item=analiza_struct(offstruct+len))==0) error(0,47); // estructura vacia
        i=offstruct+len; dup=(*ob).sglo.totalitems+1;
        if (dup>1) {
          test_buffer(&mem,&imem_max,i+(*ob).sglo.len_item*(*ob).sglo.totalitems);
          test_buffer(&frm,&ifrm_max,i+(*ob).sglo.len_item*(*ob).sglo.totalitems);
          while (--dup) {
            memcpy(&mem[i],&mem[offstruct+len],(*ob).sglo.len_item<<2);
            memcpy(&frm[i],&frm[offstruct+len],(*ob).sglo.len_item<<2);
            i+=(*ob).sglo.len_item;
          }
        }
        len+=(*ob).sglo.len_item*(*ob).sglo.totalitems;
        member=old_member; lexico();
        while (pieza==p_ptocoma) lexico();
      }
    } else if (pieza==p_string) {                     // Miembro cadena

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a byte

        lexico(); puntero1:
        ob=analiza_pointer(tpcgl,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;

        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero1;
        }

      } else {

        if (pieza!=p_id) error(1,29); // esperando el nombre de la cadena
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=tcglo; (*ob).cglo.offset=len+1;
        if (pieza==p_corab) {
          lexico();
          if (pieza==p_corce) {
            lexico();
            (*ob).cglo.totalen=255;
          } else {
            if (((*ob).cglo.totalen=constante())<0) error(4,31); // cadena de longitud negativa
            if ((*ob).cglo.totalen>0xFFFFF) error(4,32); // cadena demasiado larga
            if (pieza!=p_corce) error(3,19); // esperando ']'
            lexico();
          }
        } else (*ob).cglo.totalen=255;

        test_buffer(&frm,&ifrm_max,offstruct+len);
        frm[offstruct+len]=0xDAD00000;

        if (pieza==p_asig) {
          save_error(1);
          _itxt=itxt;
          lexico();
          if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
            error(3,46); // se esperaba un literal
          if (strlen((char*)&mem[pieza_num])>(*ob).cglo.totalen+1)
            error(4,49); // literal demasiado largo
          test_buffer(&mem,&imem_max,offstruct+len);
          mem[offstruct+len]=0xDAD00000|(*ob).cglo.totalen;
          strcpy((char*)&mem[offstruct+len+1],(char*)&mem[pieza_num]);
          len+=1+((*ob).cglo.totalen+5)/4;
          itxt=_itxt; // Saca la cadena del segmento de textos
          lexico();
        } else {
          test_buffer(&mem,&imem_max,offstruct+len);
          mem[offstruct+len]=0xDAD00000|(*ob).cglo.totalen;
          len+=1+((*ob).cglo.totalen+5)/4;
        }
      }

      if (pieza==p_coma) pieza=p_string; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_byte) {                         // Miembro byte

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a byte

        lexico(); puntero2:
        ob=analiza_pointer(tpbgl,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;

        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero2;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=tbglo; (*ob).bglo.offset=len;
        if (pieza==p_corab) { lexico();
          if (((*ob).bglo.len1=constante())<0) error(4,40); // tabla de longitud negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).bglo.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).bglo.len3=constante())<0) error(4,40); // reidem
            } else (*ob).bglo.len3=-1;
          } else { (*ob).bglo.len2=-1; (*ob).bglo.len3=-1; }
          (*ob).bglo.totalen=(*ob).bglo.len1+1;
          if ((*ob).bglo.len2>-1) (*ob).bglo.totalen*=(*ob).bglo.len2+1;
          if ((*ob).bglo.len3>-1) (*ob).bglo.totalen*=(*ob).bglo.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
          test_buffer(&mem,&imem_max,offstruct+len+((*ob).bglo.totalen+3)/4);

          test_buffer(&frm,&ifrm_max,offstruct+len+((*ob).bglo.totalen+3)/4);
          memset(&frm[offstruct+len],2,(*ob).bglo.totalen);

          if (pieza==p_asig) {
            _imem=imem; imem=offstruct+len;
            save_error(1); lexico();
            oimemptr=(byte*)&mem[imem];
            tglo_init(2);
            if (imemptr-oimemptr>(*ob).bglo.totalen) error(4,48); // demasiados valores para la tabla
            imem=_imem;
          } (*ob).bglo.totalen=((*ob).bglo.totalen+3)/4;
        } else {
          (*ob).tipo=tbglo; (*ob).bglo.offset=len;
          (*ob).bglo.len1=0;
          (*ob).bglo.len2=-1;
          (*ob).bglo.len3=-1;
          (*ob).bglo.totalen=1; // 1 int

          test_buffer(&frm,&ifrm_max,offstruct+len);
          frm[offstruct+len]=2;

          if (pieza==p_asig) {
            save_error(1);
            lexico();
            test_buffer(&mem,&imem_max,offstruct+len);
            mem[offstruct+len]=constante();
            if (mem[offstruct+len]<0 || mem[offstruct+len]>255) error(4,50); // valor byte fuera de rango
          }
        } len+=(*ob).bglo.totalen;
      }

      if (pieza==p_coma) pieza=p_byte; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_word) {                         // Miembro word

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a word

        lexico(); puntero3:
        ob=analiza_pointer(tpwgl,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero3;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=twglo; (*ob).wglo.offset=len;
        if (pieza==p_corab) { lexico();
          if (((*ob).wglo.len1=constante())<0) error(4,40); // tabla de longitud negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).wglo.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).wglo.len3=constante())<0) error(4,40); // idem
            } else (*ob).wglo.len3=-1;
          } else { (*ob).wglo.len2=-1; (*ob).wglo.len3=-1; }
          (*ob).wglo.totalen=(*ob).wglo.len1+1;
          if ((*ob).wglo.len2>-1) (*ob).wglo.totalen*=(*ob).wglo.len2+1;
          if ((*ob).wglo.len3>-1) (*ob).wglo.totalen*=(*ob).wglo.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
          test_buffer(&mem,&imem_max,offstruct+len+((*ob).wglo.totalen+1)/2);

          test_buffer(&frm,&ifrm_max,offstruct+len+((*ob).wglo.totalen+1)/2);
          memset(&frm[offstruct+len],1,(*ob).wglo.totalen*2);

          if (pieza==p_asig) {
            _imem=imem; imem=offstruct+len;
            save_error(1); lexico();
            oimemptr=(byte*)&mem[imem];
            tglo_init(1);
            if (imemptr-oimemptr>(*ob).wglo.totalen*2) error(4,48); // demasiados valores para la tabla
            imem=_imem;
          } (*ob).wglo.totalen=((*ob).wglo.totalen+1)/2;
        } else {
          (*ob).tipo=twglo; (*ob).wglo.offset=len;
          (*ob).wglo.len1=0;
          (*ob).wglo.len2=-1;
          (*ob).wglo.len3=-1;
          (*ob).wglo.totalen=1; // 1 int

          test_buffer(&frm,&ifrm_max,offstruct+len);
          frm[offstruct+len]=1;

          if (pieza==p_asig) {
            save_error(1);
            lexico();
            test_buffer(&mem,&imem_max,offstruct+len);
            mem[offstruct+len]=constante();
            if (mem[offstruct+len]<0 || mem[offstruct+len]>65535) error(4,51); // valor word fuera de rango
          }
        } len+=(*ob).wglo.totalen;
      }

      if (pieza==p_coma) pieza=p_word; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_int || pieza==p_id || pieza==p_pointer) {

      if (pieza==p_int) {
        lexico();
        if (pieza!=p_id && pieza!=p_pointer) error(1,23); // esperando un nombre
      }

      if (pieza==p_pointer) { // Miembro puntero a int

        lexico(); puntero4:
        ob=analiza_pointer(tpigl,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero4;
        }

      } else {

        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        if (pieza==p_corab) { lexico();                   // Miembro tabla
          (*ob).tipo=ttglo; (*ob).tglo.offset=len;
          if (((*ob).tglo.len1=constante())<0) error(4,40); // tabla de longitud negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).tglo.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).tglo.len3=constante())<0) error(4,40); // idem
            } else (*ob).tglo.len3=-1;
          } else { (*ob).tglo.len2=-1; (*ob).tglo.len3=-1; }
          (*ob).tglo.totalen=(*ob).tglo.len1+1;
          if ((*ob).tglo.len2>-1) (*ob).tglo.totalen*=(*ob).tglo.len2+1;
          if ((*ob).tglo.len3>-1) (*ob).tglo.totalen*=(*ob).tglo.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'

          test_buffer(&mem,&imem_max,offstruct+len+(*ob).tglo.totalen);
          if (pieza==p_asig) {
            _imem=imem; imem=offstruct+len;
            save_error(1); lexico(); tglo_init(0);
            if (imem-(offstruct+len)>(*ob).tglo.totalen) error(4,48); // demasiados valores para la tabla
            imem=_imem;
          } len+=(*ob).tglo.totalen;

        } else {                                          // Miembro variable

          (*ob).tipo=tvglo; (*ob).vglo.offset=len;
          test_buffer(&mem,&imem_max,offstruct+len);
          if (pieza==p_asig) {
            lexico(); mem[offstruct+len]=constante();
          } len+=1;

        }
      }
      if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
      while (pieza==p_ptocoma || pieza==p_coma) lexico();
    } else { error(0,39); do lexico(); while (pieza==p_ptocoma); } // esperando un elemento de la estructura
  } return(len);
}

//

int analiza_struct_local(int offstruct) { // tras " struct id [ <const> ] " // idðmember
  int len=0,dup,i,_itxt,_iloc;
  struct objeto * ob;
  struct objeto * old_member,* member2;
  byte * oimemptr;

  while (pieza==p_ptocoma) lexico();

  while (pieza!=p_end) {

    if (pieza==p_struct) {                              // Miembro struct

      lexico();

      if (pieza==p_pointer) { // Se define un puntero a struct

        old_member=member; member=NULL; lexico(); member=old_member;
        if (pieza!=p_id) error(1,27); ob=o; // esperando el nombre de la estructura

        if ((*ob).tipo==tnone) error(0,28); // No se define el pointer así
        if ((*ob).tipo!=tsglo && (*ob).tipo!=tsloc) error(0,28);
        lexico();
        puntero_a_struct:
        analiza_pointer_struct(tpslo,len,ob);
        test_buffer(&loc,&iloc_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); loc[offstruct+len]=constante();
        } len+=1;

        if (pieza==p_coma) {
          lexico();
          if (pieza==p_pointer) lexico();
          goto puntero_a_struct;
        } else {
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }

      } else {

        if (pieza!=p_id) error(1,27); // esperando el nombre de la estructura
        ob=o;

        old_member=member; member=ob; lexico();
        if ((*ob).tipo!=tnone) error(2,30); // el nombre no es nuevo

        (*ob).tipo=tsloc; (*ob).sloc.offset=len;
        if (pieza==p_corab) {
          member2=member; member=NULL; lexico();
          if (((*ob).sloc.items1=constante())<0) error(4,43); // estructura de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).sloc.items2=constante())<0) error(4,43); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).sloc.items3=constante())<0) error(4,43); // reidem
            } else (*ob).sloc.items3=-1;
          } else { (*ob).sloc.items2=-1; (*ob).sloc.items3=-1; }
          member=member2;
          (*ob).sloc.totalitems=(*ob).sloc.items1+1;
          if ((*ob).sloc.items2>-1) (*ob).sloc.totalitems*=(*ob).sloc.items2+1;
          if ((*ob).sloc.items3>-1) (*ob).sloc.totalitems*=(*ob).sloc.items3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
        } else {
          (*ob).sloc.totalitems=1;
          (*ob).sloc.items1=0; (*ob).sloc.items2=-1; (*ob).sloc.items3=-1;
        }
        if (((*ob).sloc.len_item=analiza_struct_local(offstruct+len))==0) error(0,47); // estructura vacia
        i=offstruct+len; dup=(*ob).sloc.totalitems+1;
        if (dup>1) {
          test_buffer(&loc,&iloc_max,i+(*ob).sloc.len_item*(*ob).sloc.totalitems);
          test_buffer(&frm,&ifrm_max,i+(*ob).sloc.len_item*(*ob).sloc.totalitems);
          while (--dup) {
            memcpy(&loc[i],&loc[offstruct+len],(*ob).sloc.len_item<<2);
            memcpy(&frm[i],&frm[offstruct+len],(*ob).sloc.len_item<<2);
            i+=(*ob).sloc.len_item;
          }
        }
        len+=(*ob).sloc.len_item*(*ob).sloc.totalitems;
        member=old_member; lexico();
        while (pieza==p_ptocoma) lexico();
      }

    } else if (pieza==p_string) {                     // Miembro cadena

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a byte

        lexico(); puntero1:
        ob=analiza_pointer(tpclo,len);
        test_buffer(&loc,&iloc_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); loc[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero1;
        }

      } else {

        if (pieza!=p_id) error(1,29); // esperando el nombre de la cadena
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=tcloc; (*ob).cloc.offset=len+1;
        if (pieza==p_corab) {
          lexico();
          if (pieza==p_corce) {
            lexico();
            (*ob).cloc.totalen=255;
          } else {
            if (((*ob).cloc.totalen=constante())<0) error(4,31); // cadena de long. negativa
            if ((*ob).cloc.totalen>0xFFFFF) error(4,32); // cadena demasiado larga
            if (pieza!=p_corce) error(3,19); // esperando ']'
            lexico();
          }
        } else (*ob).cloc.totalen=255;

        test_buffer(&frm,&ifrm_max,offstruct+len);
        frm[offstruct+len]=0xDAD00000;

        if (pieza==p_asig) {
          save_error(1);
          _itxt=itxt;
          lexico();
          if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
            error(3,46); // se esperaba un literal
          if (strlen((char*)&mem[pieza_num])>(*ob).cloc.totalen+1)
            error(4,49); // literal demasiado largo
          test_buffer(&loc,&iloc_max,offstruct+len);
          loc[offstruct+len]=0xDAD00000|(*ob).cloc.totalen;
          strcpy((char*)&loc[offstruct+len+1],(char*)&mem[pieza_num]);
          len+=1+((*ob).cloc.totalen+5)/4;
          itxt=_itxt; // Saca la cadena del segmento de textos
          lexico();
        } else {
          test_buffer(&loc,&iloc_max,offstruct+len);
          loc[offstruct+len]=0xDAD00000|(*ob).cloc.totalen;
          len+=1+((*ob).cloc.totalen+5)/4;
        }
      }

      if (pieza==p_coma) pieza=p_string; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_byte) {                         // Miembro byte

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a byte

        lexico(); puntero2:
        ob=analiza_pointer(tpblo,len);
        test_buffer(&loc,&iloc_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); loc[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero2;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=tbloc; (*ob).bloc.offset=len;
        if (pieza==p_corab) { lexico();
          if (((*ob).bloc.len1=constante())<0) error(4,40); // tabla de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).bloc.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).bloc.len3=constante())<0) error(4,40); // reidem
            } else (*ob).bloc.len3=-1;
          } else { (*ob).bloc.len2=-1; (*ob).bloc.len3=-1; }
          (*ob).bloc.totalen=(*ob).bloc.len1+1;
          if ((*ob).bloc.len2>-1) (*ob).bloc.totalen*=(*ob).bloc.len2+1;
          if ((*ob).bloc.len3>-1) (*ob).bloc.totalen*=(*ob).bloc.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'

          test_buffer(&loc,&iloc_max,offstruct+len+((*ob).bloc.totalen+3)/4);

          test_buffer(&frm,&ifrm_max,offstruct+len+((*ob).bloc.totalen+3)/4);
          memset(&frm[offstruct+len],2,(*ob).bloc.totalen);

          if (pieza==p_asig) {
            _iloc=iloc; iloc=offstruct+len;
            save_error(1); lexico();
            oimemptr=(byte*)&loc[iloc];
            tloc_init(2);
            if (imemptr-oimemptr>(*ob).bloc.totalen) error(4,48); // demasiados valores para la tabla
            iloc=_iloc;
          } (*ob).bloc.totalen=((*ob).bloc.totalen+3)/4;
        } else {
          (*ob).tipo=tbloc; (*ob).bloc.offset=len;
          (*ob).bloc.len1=0;
          (*ob).bloc.len2=-1;
          (*ob).bloc.len3=-1;
          (*ob).bloc.totalen=1; // 1 int

          test_buffer(&frm,&ifrm_max,offstruct+len);
          frm[offstruct+len]=2;

          if (pieza==p_asig) {
            save_error(1);
            lexico();
            test_buffer(&loc,&iloc_max,offstruct+len);
            loc[offstruct+len]=constante();
            if (loc[offstruct+len]<0 || loc[offstruct+len]>255) error(4,51); // valor byte fuera de rango
          }
        } len+=(*ob).bloc.totalen;
      }

      if (pieza==p_coma) pieza=p_byte; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ;
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_word) {                         // Miembro word

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a word

        lexico(); puntero3:
        ob=analiza_pointer(tpwlo,len);
        test_buffer(&loc,&iloc_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); loc[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero3;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=twloc; (*ob).wloc.offset=len;
        if (pieza==p_corab) { lexico();
          if (((*ob).wloc.len1=constante())<0) error(4,40); // tabla de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).wloc.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).wloc.len3=constante())<0) error(4,40); // reidem
            } else (*ob).wloc.len3=-1;
          } else { (*ob).wloc.len2=-1; (*ob).wloc.len3=-1; }
          (*ob).wloc.totalen=(*ob).wloc.len1+1;
          if ((*ob).wloc.len2>-1) (*ob).wloc.totalen*=(*ob).wloc.len2+1;
          if ((*ob).wloc.len3>-1) (*ob).wloc.totalen*=(*ob).wloc.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'

          test_buffer(&loc,&iloc_max,offstruct+len+((*ob).wloc.totalen+1)/2);

          test_buffer(&frm,&ifrm_max,offstruct+len+((*ob).wloc.totalen+1)/2);
          memset(&frm[offstruct+len],1,(*ob).wloc.totalen*2);

          if (pieza==p_asig) {
            _iloc=iloc; iloc=offstruct+len;
            save_error(1); lexico();
            oimemptr=(byte*)&loc[iloc];
            tloc_init(1);
            if (imemptr-oimemptr>(*ob).wloc.totalen*2) error(4,48); // demasiados valores para la tabla
            iloc=_iloc;
          } (*ob).wloc.totalen=((*ob).wloc.totalen+1)/2;
        } else {
          (*ob).tipo=twloc; (*ob).wloc.offset=len;
          (*ob).wloc.len1=0;
          (*ob).wloc.len2=-1;
          (*ob).wloc.len3=-1;
          (*ob).wloc.totalen=1; // 1 int

          test_buffer(&frm,&ifrm_max,offstruct+len);
          frm[offstruct+len]=1;

          if (pieza==p_asig) {
            save_error(1);
            lexico();
            test_buffer(&loc,&iloc_max,offstruct+len);
            loc[offstruct+len]=constante();
            if (loc[offstruct+len]<0 || loc[offstruct+len]>65535) error(4,51); // valor word fuera de rango
          }
        } len+=(*ob).wloc.totalen;
      }

      if (pieza==p_coma) pieza=p_word; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_int || pieza==p_id || pieza==p_pointer) {

      if (pieza==p_int) {
        lexico();
        if (pieza!=p_id && pieza!=p_pointer) error(1,23); // esperando un nombre
      }

      if (pieza==p_pointer) { // Miembro puntero a int

        lexico(); puntero4:
        ob=analiza_pointer(tpilo,len);
        test_buffer(&loc,&iloc_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); loc[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero4;
        }

      } else {

        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        if (pieza==p_corab) { lexico();    // Miembro tabla
          (*ob).tipo=ttloc; (*ob).tloc.offset=len;
          if (((*ob).tloc.len1=constante())<0) error(4,40); // tabla de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).tloc.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).tloc.len3=constante())<0) error(4,40); // reidem
            } else (*ob).tloc.len3=-1;
          } else { (*ob).tloc.len2=-1; (*ob).tloc.len3=-1; }
          (*ob).tloc.totalen=(*ob).tloc.len1+1;
          if ((*ob).tloc.len2>-1) (*ob).tloc.totalen*=(*ob).tloc.len2+1;
          if ((*ob).tloc.len3>-1) (*ob).tloc.totalen*=(*ob).tloc.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'

          test_buffer(&loc,&iloc_max,offstruct+len+(*ob).tloc.totalen);
          if (pieza==p_asig) {
            _iloc=iloc; iloc=offstruct+len;
            save_error(1); lexico(); tloc_init(0);
            if (iloc-(offstruct+len)>(*ob).tloc.totalen) error(4,48); // demasiados valores para la tabla
            iloc=_iloc;
          } len+=(*ob).tloc.totalen;

        } else {                                          // Miembro variable

          (*ob).tipo=tvloc; (*ob).vloc.offset=len;
          test_buffer(&loc,&iloc_max,offstruct+len);
          if (pieza==p_asig) {
            lexico(); loc[offstruct+len]=constante();
          } len+=1;

        }
      }
      if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
      while (pieza==p_ptocoma || pieza==p_coma) lexico();
    } else { error(0,39); do lexico(); while (pieza==p_ptocoma); } // esperando un elemento de la estructura
    (*ob).bloque=bloque_lexico;
  } return(len);
}


//

int analiza_struct_private(int offstruct) { // tras " struct id [ <const> ] " // idðmember
  int len=0,dup,i,_itxt,_imem;
  struct objeto * ob;
  struct objeto * old_member,* member2;
  byte * oimemptr;

  while (pieza==p_ptocoma) lexico();

  while (pieza!=p_end) {

    if (pieza==p_struct) {                              // Miembro struct

      lexico();

      if (pieza==p_pointer) { // Se define un puntero a struct

        old_member=member; member=NULL; lexico(); member=old_member;
        if (pieza!=p_id) error(1,27); ob=o; // esperando el nombre de la estructura

        if ((*ob).tipo==tnone) error(0,28); // No se define el pointer así
        if ((*ob).tipo!=tsglo && (*ob).tipo!=tsloc) error(0,28);
        lexico();
        puntero_a_struct:
        analiza_pointer_struct(tpslo,len,ob);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;

        if (pieza==p_coma) {
          lexico();
          if (pieza==p_pointer) lexico();
          goto puntero_a_struct;
        } else {
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
        }

      } else {

        if (pieza!=p_id) error(1,27); // esperando el nombre de la estructura
        ob=o;

        old_member=member; member=ob; lexico();
        if ((*ob).tipo!=tnone) error(2,30); // el nombre no es nuevo

        (*ob).tipo=tsloc; (*ob).sloc.offset=len;
        if (pieza==p_corab) {
          member2=member; member=NULL; lexico();
          if (((*ob).sloc.items1=constante())<0) error(4,43); // estructura de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).sloc.items2=constante())<0) error(4,43); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).sloc.items3=constante())<0) error(4,43); // reidem
            } else (*ob).sloc.items3=-1;
          } else { (*ob).sloc.items2=-1; (*ob).sloc.items3=-1; }
          member=member2;
          (*ob).sloc.totalitems=(*ob).sloc.items1+1;
          if ((*ob).sloc.items2>-1) (*ob).sloc.totalitems*=(*ob).sloc.items2+1;
          if ((*ob).sloc.items3>-1) (*ob).sloc.totalitems*=(*ob).sloc.items3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
        } else {
          (*ob).sloc.totalitems=1;
          (*ob).sloc.items1=0; (*ob).sloc.items2=-1; (*ob).sloc.items3=-1;
        }
        if (((*ob).sloc.len_item=analiza_struct_private(offstruct+len))==0) error(0,55); // demasiados valores para la estructura
        i=offstruct+len; dup=(*ob).sloc.totalitems+1;
        if (dup>1) {
          test_buffer(&mem,&imem_max,i+(*ob).sloc.len_item*(*ob).sloc.totalitems);
          test_buffer(&frm,&ifrm_max,i+(*ob).sloc.len_item*(*ob).sloc.totalitems);
          while (--dup) {
            memcpy(&mem[i],&mem[offstruct+len],(*ob).sloc.len_item<<2);
            memcpy(&frm[i],&frm[offstruct+len],(*ob).sloc.len_item<<2);
            i+=(*ob).sloc.len_item;
          }
        }
        len+=(*ob).sloc.len_item*(*ob).sloc.totalitems;
        member=old_member; lexico();
        while (pieza==p_ptocoma) lexico();
      }

    } else if (pieza==p_string) {                     // Miembro cadena

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a byte

        lexico(); puntero1:
        ob=analiza_pointer(tpclo,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero1;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=tcloc; (*ob).cloc.offset=len+1;
        if (pieza==p_corab) {
          lexico();
          if (pieza==p_corce) {
            lexico();
            (*ob).cloc.totalen=255;
          } else {
            if (((*ob).cloc.totalen=constante())<0) error(4,31); // cadena de long. negativa
            if ((*ob).cloc.totalen>0xFFFFF) error(4,32); // cadena demasiado larga
            if (pieza!=p_corce) error(3,19); // esperando ']'
            lexico();
          }
        } else (*ob).cloc.totalen=255;

        test_buffer(&frm,&ifrm_max,offstruct+len);
        frm[offstruct+len]=0xDAD00000;

        if (pieza==p_asig) {
          save_error(1);
          _itxt=itxt;
          lexico();
          if (pieza!=p_lit && !(pieza==p_id && (*o).tipo==tcons && (*o).cons.literal))
            error(3,46); // se esperaba un literal
          if (strlen((char*)&mem[pieza_num])>(*ob).cloc.totalen+1)
            error(4,49); // literal demasiado largo
          test_buffer(&mem,&imem_max,offstruct+len);
          mem[offstruct+len]=0xDAD00000|(*ob).cloc.totalen;
          strcpy((char*)&mem[offstruct+len+1],(char*)&mem[pieza_num]);
          len+=1+((*ob).cloc.totalen+5)/4;
          itxt=_itxt; // Saca la cadena del segmento de textos
          lexico();
        } else {
          test_buffer(&mem,&imem_max,offstruct+len);
          mem[offstruct+len]=0xDAD00000|(*ob).cloc.totalen;
          len+=1+((*ob).cloc.totalen+5)/4;
        }
      }

      if (pieza==p_coma) pieza=p_string; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_byte) {                         // Miembro byte

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a byte

        lexico(); puntero2:
        ob=analiza_pointer(tpblo,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero2;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=tbloc; (*ob).bloc.offset=len;
        if (pieza==p_corab) { lexico();
          if (((*ob).bloc.len1=constante())<0) error(4,40); // tabla de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).bloc.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).bloc.len3=constante())<0) error(4,40); // reidem
            } else (*ob).bloc.len3=-1;
          } else { (*ob).bloc.len2=-1; (*ob).bloc.len3=-1; }
          (*ob).bloc.totalen=(*ob).bloc.len1+1;
          if ((*ob).bloc.len2>-1) (*ob).bloc.totalen*=(*ob).bloc.len2+1;
          if ((*ob).bloc.len3>-1) (*ob).bloc.totalen*=(*ob).bloc.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
          test_buffer(&mem,&imem_max,offstruct+len+((*ob).bloc.totalen+3)/4);

          test_buffer(&frm,&ifrm_max,offstruct+len+((*ob).bloc.totalen+3)/4);
          memset(&frm[offstruct+len],2,(*ob).bloc.totalen);

          if (pieza==p_asig) {
            _imem=imem; imem=offstruct+len;
            save_error(1); lexico();
            oimemptr=(byte*)&mem[imem];
            tglo_init(2);
            if (imemptr-oimemptr>(*ob).bloc.totalen) error(4,48); // demasiados valores para la tabla
            imem=_imem;
          } (*ob).bloc.totalen=((*ob).bloc.totalen+3)/4;
        } else {
          (*ob).tipo=tbloc; (*ob).bloc.offset=len;
          (*ob).bloc.len1=0;
          (*ob).bloc.len2=-1;
          (*ob).bloc.len3=-1;
          (*ob).bloc.totalen=1; // 1 int

          test_buffer(&frm,&ifrm_max,offstruct+len);
          frm[offstruct+len]=2;

          if (pieza==p_asig) {
            save_error(1);
            lexico();
            test_buffer(&mem,&imem_max,offstruct+len);
            mem[offstruct+len]=constante();
            if (mem[offstruct+len]<0 || mem[offstruct+len]>255) error(4,50); // valor byte fuera de rango
          }
        } len+=(*ob).bloc.totalen;
      }

      if (pieza==p_coma) pieza=p_byte; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_word) {                         // Miembro word

      lexico();

      if (pieza==p_pointer) { // Miembro puntero a word

        lexico(); puntero3:
        ob=analiza_pointer(tpwlo,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero3;
        }

      } else {

        if (pieza!=p_id) error(1,23); // esperando un nombre
        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        (*ob).tipo=twloc; (*ob).wloc.offset=len;
        if (pieza==p_corab) { lexico();
          if (((*ob).wloc.len1=constante())<0) error(4,40); // tabla de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).wloc.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).wloc.len3=constante())<0) error(4,40); // reidem
            } else (*ob).wloc.len3=-1;
          } else { (*ob).wloc.len2=-1; (*ob).wloc.len3=-1; }
          (*ob).wloc.totalen=(*ob).wloc.len1+1;
          if ((*ob).wloc.len2>-1) (*ob).wloc.totalen*=(*ob).wloc.len2+1;
          if ((*ob).wloc.len3>-1) (*ob).wloc.totalen*=(*ob).wloc.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
          test_buffer(&mem,&imem_max,offstruct+len+((*ob).wloc.totalen+1)/2);

          test_buffer(&frm,&ifrm_max,offstruct+len+((*ob).wloc.totalen+1)/2);
          memset(&frm[offstruct+len],1,(*ob).wloc.totalen*2);

          if (pieza==p_asig) {
            _imem=imem; imem=offstruct+len;
            save_error(1); lexico();
            oimemptr=(byte*)&mem[imem];
            tglo_init(1);
            if (imemptr-oimemptr>(*ob).wloc.totalen*2) error(4,48); // demasiados valores para la tabla
            imem=_imem;
          } (*ob).wloc.totalen=((*ob).wloc.totalen+1)/2;
        } else {
          (*ob).tipo=twloc; (*ob).wloc.offset=len;
          (*ob).wloc.len1=0;
          (*ob).wloc.len2=-1;
          (*ob).wloc.len3=-1;
          (*ob).wloc.totalen=1; // 1 int

          test_buffer(&frm,&ifrm_max,offstruct+len);
          frm[offstruct+len]=1;

          if (pieza==p_asig) {
            save_error(1);
            lexico();
            test_buffer(&mem,&imem_max,offstruct+len);
            mem[offstruct+len]=constante();
            if (mem[offstruct+len]<0 || mem[offstruct+len]>65535) error(4,51); // valor word fuera de rango
          }
        } len+=(*ob).wloc.totalen;
      }

      if (pieza==p_coma) pieza=p_word; else {
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
      }

    } else if (pieza==p_int || pieza==p_id || pieza==p_pointer) {

      if (pieza==p_int) {
        lexico();
        if (pieza!=p_id && pieza!=p_pointer) error(1,23); // esperando un nombre
      }

      if (pieza==p_pointer) { // Miembro puntero a int

        lexico(); puntero4:
        ob=analiza_pointer(tpilo,len);
        test_buffer(&mem,&imem_max,offstruct+len);
        if (pieza==p_asig) {
          lexico(); mem[offstruct+len]=constante();
        } len+=1;
        if (pieza==p_coma) {
          lexico(); if (pieza==p_pointer) lexico();
          goto puntero4;
        }

      } else {

        ob=o; if ((*ob).tipo!=tnone) error(0,30); lexico(); // el nombre no es nuevo
        if (pieza==p_corab) { lexico();    // Miembro tabla
          (*ob).tipo=ttloc; (*ob).tloc.offset=len;
          if (((*ob).tloc.len1=constante())<0) error(4,40); // tabla de long. negativa
          if (pieza==p_coma) {
            lexico();
            if (((*ob).tloc.len2=constante())<0) error(4,40); // idem
            if (pieza==p_coma) {
              lexico();
              if (((*ob).tloc.len3=constante())<0) error(4,40); // reidem
            } else (*ob).tloc.len3=-1;
          } else { (*ob).tloc.len2=-1; (*ob).tloc.len3=-1; }
          (*ob).tloc.totalen=(*ob).tloc.len1+1;
          if ((*ob).tloc.len2>-1) (*ob).tloc.totalen*=(*ob).tloc.len2+1;
          if ((*ob).tloc.len3>-1) (*ob).tloc.totalen*=(*ob).tloc.len3+1;
          if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'

          test_buffer(&mem,&imem_max,offstruct+len+(*ob).tloc.totalen);
          if (pieza==p_asig) {
            _imem=imem; imem=offstruct+len;
            save_error(1); lexico(); tglo_init(0);
            if (imem-(offstruct+len)>(*ob).tloc.totalen) error(4,48); // demasiados valores para la tabla
            imem=_imem;
          } len+=(*ob).tloc.totalen;

        } else {                                          // Miembro variable

          (*ob).tipo=tvloc; (*ob).vloc.offset=len;

          test_buffer(&mem,&imem_max,offstruct+len);
          if (pieza==p_asig) {
            lexico(); mem[offstruct+len]=constante();
          } len+=1;

        }
      }

      if (!free_sintax) if (pieza!=p_ptocoma && pieza!=p_coma) error(3,9); // esperando ';'
      while (pieza==p_ptocoma || pieza==p_coma) lexico();

    } else { error(0,39); do lexico(); while (pieza==p_ptocoma); } // esperando un elemento de la estructura
    (*ob).bloque=bloque_lexico;
  } return(len);
}


//

int analiza_pointer_struct(int tipo, int offset, struct objeto * estructura)
{
  struct objeto * ob;
  int items1,items2,items3;

  if (pieza!=p_id) error(1,23); // esperando un nombre
  ob=o; if ((*ob).tipo!=tnone) {
    if (parametros==-1 && (*ob).param==1 && (*ob).bloque==bloque_actual) {
      if ((*ob).tipo==tipo) { // Se repite un pointer parámetro como private
        save_error(0); lexico();
        items1=-1; items2=-1; items3=-1;
        if (pieza==p_corab) { lexico();
          if ((items1=constante())<0) error(4,43); // estructura de longitud negativa
          if (pieza==p_coma) {
            lexico();
            if ((items2=constante())<0) error(4,43); // estructura de longitud negativa
            if (pieza==p_coma) {
              lexico();
              if ((items3=constante())<0) error(4,43); // estructura de longitud negativa
            }
          } if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
        }
        if (items1!=(*ob).psgl.items1 || items2!=(*ob).psgl.items2 || items3!=(*ob).psgl.items3)  error(4,41); // la longitud no coincide con la declaración anterior
        else if (pieza==p_asig) error(0,42); // no se puede inicializar un parámetro
        else {
          (*ob).param++; return(0); // No permite volver a redeclararlo
        }
      } else error(0,30); // el nombre no es nuevo
    } else error(0,30);
  } else lexico();
  if (parametros==-1) (*ob).bloque=bloque_actual;
  (*ob).psgl.items1=-1; (*ob).psgl.items2=-1; (*ob).psgl.items3=-1;
  if (pieza==p_corab) { lexico();
    if (((*ob).psgl.items1=constante())<0) error(4,43); // estructura de longitud negativa
    if (pieza==p_coma) {
      lexico();
      if (((*ob).psgl.items2=constante())<0) error(4,43); // estructura de longitud negativa
      if (pieza==p_coma) {
        lexico();
        if (((*ob).psgl.items3=constante())<0) error(4,43); // estructura de longitud negativa
      }
    }
    if (pieza!=p_corce) error(3,19); lexico(); // esperando ']'
  } (*ob).psgl.totalitems=0;
  if ((*ob).psgl.items1>-1) (*ob).psgl.totalitems=(*ob).psgl.items1+1;
  if ((*ob).psgl.items2>-1) (*ob).psgl.totalitems*=(*ob).psgl.items2+1;
  if ((*ob).psgl.items3>-1) (*ob).psgl.totalitems*=(*ob).psgl.items3+1;

  (*ob).tipo=tipo;                // tpsgl o tpslo
  (*ob).psgl.offset=offset;       // del pointer
  (*ob).psgl.ostruct=estructura;  // struct original

  // (*ob).psgl.len_item ð (*((*ob).psgl.ostruct)).len_item;

  return(1);
}

  //
  // Inicialización de tablas (a mem[imem++])
  //
  // <init>  := ( <string> | <exp> [ [p_dup] p_abrir <init> p_cerrar ] )
  //            [ p_coma <init>]
  //


void tglo_init(int tipo) { // Tipo: 0-Int o variado (struct), 1-Word, 2-Byte
  if (tipo==3) { memset(frm,0,ifrm_max*sizeof(int)); tipo=0; }
  imemptr=(byte*)&mem[imem];
  tglo_init2(tipo);
  if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
}

void tloc_init(int tipo) {
  int * ptr;

  if (tipo==3) { memset(frm,0,ifrm_max*sizeof(int)); tipo=0; }

  ptr=mem; mem=loc; loc=ptr;
  swap(imem,iloc); swap(imem_max,iloc_max);

  imemptr=(byte*)&mem[imem];
  tglo_init2(tipo);

  ptr=mem; mem=loc; loc=ptr;
  swap(imem,iloc); swap(imem_max,iloc_max);

  if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); // esperando ';'
}

// OJO!!! tglo_init no puede llamar a test_buffer !!!

void tglo_init2(int tipo) {
  int valor,dup,_imem,len,n;
  byte *oimemptr;

  while (1) {

    // Mira si finaliza la inicialización de datos

    if (pieza==p_cerrar || pieza==p_ptocoma) {
      if (*(imemptr-(int)mem+(int)frm)==1 || tipo==1) {
        imemptr+=2;
        imem=((int)imemptr-(int)mem+3)/4;
        if (*(imemptr-(int)mem+(int)frm)!=1 && tipo!=1) imemptr=(byte*)&mem[imem];
      } else if (*(imemptr-(int)mem+(int)frm)==2 || tipo==2) {
        imemptr++;
        imem=((int)imemptr-(int)mem+3)/4;
        if (*(imemptr-(int)mem+(int)frm)!=2 && tipo!=2) imemptr=(byte*)&mem[imem];
      } else {
        imemptr+=4;
        imem=((int)imemptr-(int)mem+3)/4;
      }
//      test_buffer(&mem,&imem_max,imem);
      return;
    }

    // Una coma sola, como "3,,4", avanza una posición (y define un 0)

    if (pieza==p_coma) {
      if (*(imemptr-(int)mem+(int)frm)==1 || tipo==1) {
        (*(word*)imemptr)=0;
        imemptr+=2;
        imem=((int)imemptr-(int)mem+3)/4;
        if (*(imemptr-(int)mem+(int)frm)!=1 && tipo!=1) imemptr=(byte*)&mem[imem];
      } else if (*(imemptr-(int)mem+(int)frm)==2 || tipo==2) {
        *imemptr++=0;
        imem=((int)imemptr-(int)mem+3)/4;
        if (*(imemptr-(int)mem+(int)frm)!=2 && tipo!=2) imemptr=(byte*)&mem[imem];
      } else {
        imemptr+=4;
        imem=((int)imemptr-(int)mem+3)/4;
      }
//      test_buffer(&mem,&imem_max,imem);
      lexico(); continue;
    }

    if (pieza==p_abrir) { // Dup x 1

      dup=1;

    } else if (pieza==p_dup) { // Dup x 2

      dup=2; lexico();
      if (pieza!=p_abrir) error(3,22); // esperando '('

    } else if (pieza==p_lit || (pieza==p_id && (*o).tipo==tcons && (*o).cons.literal)) {

      valor=pieza_num; lexico();
      if (pieza==p_abrir || pieza==p_dup) error(2,52); // no se puede duplicar 0 o menos veces (se ha indicado un literal como número de veces)

      // Mete un literal en un dato de tipo string

      if ((((int)imemptr-(int)mem)%4)==0) {
        imem=((int)imemptr-(int)mem+3)/4;
        if (frm[imem]==0xdad00000) {
          if (strlen((char*)&mem_ory[valor])>(mem[imem]&0xFFFFF)+1) error(2,49); // literal demasiado largo
          _imem=imem;
          imem+=1+((mem[imem]&0xFFFFF)+5)/4;
//          test_buffer(&mem,&imem_max,imem);
          imemptr=(byte*)&mem[imem];
          strcpy((char*)&mem[_imem+1],(char*)&mem_ory[valor]);
          if (pieza!=p_coma) return;
          lexico(); continue;
        }
      }

      // Mete un valor literal en otro tipo de dato (en lugar de un string)

      if (*(imemptr-(int)mem+(int)frm)==1 || tipo==1) { // Un string en words (el puntero short)

        imemptr=(byte*)(((int)imemptr+1)&-2); // Lo hace par
        *((word*)imemptr)=valor;
        imemptr+=2;
        imem=((int)imemptr-(int)mem+3)/4;
        if (*(imemptr-(int)mem+(int)frm)!=1 && tipo!=1) imemptr=(byte*)&mem[imem];
        if (pieza!=p_coma) return;
        lexico(); continue;

      } else if (*(imemptr-(int)mem+(int)frm)==2 || tipo==2) { // Un string en bytes

        oimemptr=imemptr;
        while (*(oimemptr-(int)mem+(int)frm)==2) oimemptr++;
        if (tipo==0) if (strlen((char*)&mem_ory[valor])>(int)(oimemptr-imemptr)) error(2,48); // demasiados valores para la tabla
        strcpy(imemptr,(char*)&mem_ory[valor]);
        imemptr+=strlen(imemptr);
        imem=((int)imemptr-(int)mem+3)/4;
        if (*(imemptr-(int)mem+(int)frm)!=2 && tipo!=2) imemptr=(byte*)&mem[imem];
        if (pieza!=p_coma) return;
        lexico(); continue;

      } else { // Un string en un int (el puntero long, como en DIV 1)

        imem=((int)imemptr-(int)mem+3)/4;
        mem[imem++]=valor;
//        test_buffer(&mem,&imem_max,imem);
        imemptr=(byte*)&mem[imem];
        if (pieza!=p_coma) return;
        lexico(); continue;

      }

    } else {
      valor=constante();
      if (pieza!=p_abrir) {
        if (pieza!=p_dup) {

          if ((((int)imemptr-(int)mem)%4)==0) {
            imem=((int)imemptr-(int)mem+3)/4;
            if (frm[imem]==0xdad00000) error(2,46); // se esperaba un literal
          }

          // Mete un valor numérico en la memoria

          if (*(imemptr-(int)mem+(int)frm)==1 || tipo==1) { // En un word

            if (valor<0 || valor>65535) error(2,51); // valor word fuera de rango
            imemptr=(byte*)(((int)imemptr+1)&-2); // Lo hace par
            *((word*)imemptr)=valor;
            imemptr+=2;
            imem=((int)imemptr-(int)mem+3)/4;
            if (*(imemptr-(int)mem+(int)frm)!=1 && tipo!=1) imemptr=(byte*)&mem[imem];
            if (pieza!=p_coma) return;
            lexico(); continue;

          } else if (*(imemptr-(int)mem+(int)frm)==2 || tipo==2) { // En un byte

            if (valor<0 || valor>255) error(2,50); // valor byte fuera de rango
            *imemptr++=valor;
            imem=((int)imemptr-(int)mem+3)/4;
            if (*(imemptr-(int)mem+(int)frm)!=2 && tipo!=2) imemptr=(byte*)&mem[imem];
            if (pieza!=p_coma) return;
            lexico(); continue;

          } else { // En un int (lo normal en DIV 1)

            imem=((int)imemptr-(int)mem+3)/4;
            mem[imem++]=valor;
//            test_buffer(&mem,&imem_max,imem);
            imemptr=(byte*)&mem[imem];
            if (pieza!=p_coma) return;
            lexico(); continue;

          }

        } else {
          dup=valor; lexico();
          if (pieza!=p_abrir) error(3,22); // esperando '('
        }
      } else dup=valor;
    }

    // Duplica una secuencia de valores

    if (dup<1) error(2,52); // no se puede duplicar 0 o menos veces
    lexico();

    // Analiza la secuencia como otra inicialización ...

    oimemptr=imemptr;
    tglo_init2(tipo);
    if (pieza!=p_cerrar) error(3,18); // esperando ')'

    // Y la duplica el número de veces indicado

    if (dup>1) {
      len=imemptr-oimemptr;
//      test_buffer(&mem,&imem_max,(((int)oimemptr-(int)mem)+len*dup+3)/4);
      while (--dup) {
        for (n=0;n<len;n++) {

          // Comprueba que las cadenas caigan sobre si mismas en los dup()...
          if ((((int)imemptr-(int)mem)%4)==0) {
            imem=((int)imemptr-(int)mem+3)/4;
            _imem=((int)oimemptr+n-(int)mem+3)/4;
            if (frm[imem]==0xdad00000) {
              if (frm[_imem]!=0xdad00000) error(3,53); // inicialización incorrecta
              else if (mem[imem]!=mem[_imem]) error(3,53);
            } else if (frm[_imem]==0xdad00000) error(3,53);
          }

          // Comprueba que los words caigan sobre words
          if (*(byte*)((int)imemptr-(int)mem+(int)frm)==1) {
            if (*(byte*)((int)oimemptr+n-(int)mem+(int)frm)!=1) error(3,53); // idem
          } else if (*(byte*)((int)oimemptr+n-(int)mem+(int)frm)==1) error(3,53);

          // Comprueba que los bytes caigan sobre bytes
          if (*(byte*)((int)imemptr-(int)mem+(int)frm)==2) {
            if (*(byte*)((int)oimemptr+n-(int)mem+(int)frm)!=2) error(3,53); // reidem
          } else if (*(byte*)((int)oimemptr+n-(int)mem+(int)frm)==2) error(3,53);

          *imemptr++=*(oimemptr+n);
        }
      }
    }

    lexico();
    if (pieza!=p_coma) {
      imem=((int)imemptr-(int)mem+3)/4;
      return;
    }
    lexico();
  }
}
