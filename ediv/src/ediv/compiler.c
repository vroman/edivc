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


#include <string.h>
#include <stdlib.h>

#include <zlib.h>

#include "main.h"
#include "compiler.h"
#include "parser.h"
#include "lower.h"
#include "expresion.h"
#include "extern.h"
#include "modulos.h"
#include "encrypt.h"
#include "listados.h"

#include "shared.h"
#include "varindex.h"
#include "language.h"

void analiza_ltlex(void);	/* en ltlex.c */


void prepara_compilacion()
{
	int n;

	vnom=NULL;

	mem_ory=NULL;
	frm=NULL;
	mem=NULL;
	loc=NULL;

	itbreak=0; itcont=0; itelseif=0;
	coment=0; cero=0; error_25=25;
	
	memset(vhash,0,256*sizeof(byte*));
	
	inicializa_lower();

	/* Inicializamos la tabla de objetos */
	memset(obj,0,sizeof(obj[0])*max_obj);
	iobj=obj;
	num_obj=0;

	/* Inicializamos la tabla de símbolos */
	memset(lex_simb,0,sizeof(lex_simb));
	ilex_simb=lex_simb;
	num_nodos=0;


	/* Inicializamos los caracteres básicos en lex_case */
	for (n=0;n<256;n++)
		if (lower[n]) {
			if (n>='0' && n<='9')
				lex_case[n]=(struct lex_ele *)l_num;
			else
				lex_case[n]=(struct lex_ele *)l_id;
		}
		else
			lex_case[n]=(struct lex_ele *)l_err;


	/* Inicializamos el vector de nombres */
	vnom=(byte *) e_malloc(max_obj*long_med_id+1024);
	ivnom.b=vnom;


	/* Leemos los símbolos y palabras reservadas de ltlex.def */
	analiza_ltlex();


	/* Terminamos de inicializar lex_case */
	lex_case[' ']=(struct lex_ele *)l_spc;
	lex_case[tab]=(struct lex_ele *)l_spc;
	lex_case[cr]=(struct lex_ele *)l_cr;
	lex_case[lf]=(struct lex_ele *)l_cr;
	lex_case[0]=(struct lex_ele *)l_eof;

	/* Buffer para el bytecode */
	imem_max=default_buffer; imem=0;
	mem_ory=mem=(int*)e_malloc(imem_max*sizeof(int));
	memset(mem,0,imem_max*sizeof(int));

	/* Buffer para variables locales y privadas */
	iloc_max=default_buffer/2; iloc=0; iloc_len=0;
	loc=(int*)e_malloc(iloc_max*sizeof(int));
	memset(loc,0,iloc_max*sizeof(int));

	/* ¿Que es esto? */
	ifrm_max=default_buffer/2;
	frm=(int*)e_malloc(ifrm_max*sizeof(int));
	memset(frm,0,ifrm_max*sizeof(int));

	imem=long_header;

	*(prog+progsize)=0;
	
	_source=NULL;

	linsize=0;
}

void compila()
{
	struct objeto * i;
	FILE *f;
	int n,l;
	unsigned long m;
	byte * q, * p;
	int start_lin, start_dbg;
	byte* varptr;		/* variables indexadas */
	FILE* fvar;
	
	ultima_linea=prog;
	acceso_remoto=0; parametros=0; linea=1;
	linf=NULL;
	
	printf(translate(27));
	
	itxt=inicio_textos=imem;
	psintactico();		/* Para obtener "longitud_textos" */
	imem+=longitud_textos;
	#ifdef _DEBUG
		printf("dbg: longitud_textos: %d\n",longitud_textos);
	#endif

	test_buffer(&mem,&imem_max,imem);
	
	if(n_errors>0) return;

	num_obj_predefinidos=num_obj;

	ultima_linea=source; 
	acceso_remoto=0; parametros=0; linea=1;

	sintactico();

	i=obj;
	while (i<iobj) {
		if (i->usado) {
			linea=i->linea;
			ierror=i->ierror;
			error(0,34,i->name); /* nombre desconocido */
		}
		i++;
	}
	
	if(n_errors>0) return;
	
	/* Borra todo y comienza de nuevo :P */
	
	printf(translate(28));
	
	if (frm!=NULL) { free(frm); frm=NULL; }
	if (loc!=NULL) { free(loc); loc=NULL; }
	if (mem!=NULL) { free(mem); mem=mem_ory=NULL; }
	if (vnom!=NULL) { free(vnom); vnom=NULL; }
	
	prepara_compilacion();
	source=prog;
	
	inicializa_index();	/* ahora toca construir el indice de variables */
	dll_func2();		 /* recarga sólo las dlls necesarias */
	
	if(debug) {
		if((linf=tmpfile())==NULL) {
			printf(translate(29));
			exit(1);
		}
	}

	itxt=inicio_textos=imem;
	imem+=longitud_textos;
	test_buffer(&mem,&imem_max,imem);
	num_obj_predefinidos=num_obj;
	acceso_remoto=0; parametros=0; linea=1;
	
	sintactico();
	
	/* 
	 * Ahora que estamos en el final del bytecode, añadiremos la rutina que carga
	 * las DLLs. Lo primero es guardar las cadenas con los nombres:
	 */
	for(n=0;n<numdlls;n++)
		if(dlls[n].usado || dlls[n].prioridad>=P_SIEMPRE) {
			dlls[n].mem_nombre=imem;
			l=strlen(dlls[n].nombre);
			memcpy(&mem[imem],dlls[n].nombre,l+1);
			imem+=(l+4)/4;
			test_buffer(&mem,&imem_max,imem);
		}

	/* Si estamos compilando en modo debug, añadimos tambien la debug.dll */
	if(debug) {
		dlls[numdlls].mem_nombre=imem;
		memcpy(&mem[imem],"debug",6);
		imem+=2;
		test_buffer(&mem,&imem_max,imem);
	}
	
	/* 
	 * Ahora estamos en la posición donde comienza la rutina, por lo que tenemos
	 * que guardar la posición actual en el offset que guardamos en salto_import
	 */
	mem[salto_import]=imem;
	
	/* Escribimos la rutina de carga de DLLs */
	for(n=0;n<numdlls;n++)
		if(dlls[n].usado || dlls[n].prioridad>=P_SIEMPRE)
			g2(limp,dlls[n].mem_nombre);

	if(debug)
		g2(limp,dlls[numdlls].mem_nombre);

	g2(ljmp,salto_import+1);			
	
	/* Ya está !! :) */
	
	/* Preparamos la cabecera del bytecode */
	mem[2]=imem;
	mem[3]=max_process; /* Antes long_header, ahora no se utiliza */
	mem[4]=0; /* Antes mem[1]-mem[3] (long datos globales), ahora no se utiliza */
	mem[5]=iloc_len-iloc;
	mem[6]=iloc;
	mem[7]=0; /* Antes imem+iloc (inicio textos), ahora no se utiliza */
	mem[8]=imem+iloc; /* Número de elementos ocupados en mem[] */

	/*
	 * mem[0] se usa para almacenar flags para el ejecutable. En el caso de DIV 2,
	 * éstas eran:
	 * +1	= El programa es un setup de sonido (setup_program)
	 * +128	= El programa invoca al trazador nada más ejecutarse (compilado con F12)
	 * +512	= Se ignoran los errores de ejecución (ignore_errors)
	 * +1024= Modo DEMO (mensaje en el centro de la pantalla parpadeando diciendo
	 *		  "VERSIÓN DE DEMOSTRACIÓN")
	 */

	/*
	 * nosotros usaremos las siguientes:
	 * +16	= El exe lleva incluidas las DLL's
	 *        A ver como lo hacemos en Linux.. 1) con temporales o 2) hurgando en el codigo
	 *        de dlopen y demas y haciendonos una pekeña lib (esto es mas aconsejable)
	 * +32	= El exe lleva incluido el PAK
	 * +64	= Compilado en modo debug
	 * +128 = Igual que en DIV2, el programa invoca al trazador nada más ejecutarse (se
	 *        ha usado la orden "trazar programa" en el IDE)
	 * +512	= ignore_errors, igual que en DIV2
	 */
		
	mem[0]=0;
	if (debug) mem[0]+=64;
	if (ignore_errors) mem[0]+=512;

	/*
	 * Generamos los listados (debe hacerse ahora porque en el listado de objetos guardamos 
	 * los valores de mem[1..8])
	 */
	if(listados) {
		printf(translate(30));
		if(listados & 1) listado_ensamblador();
		if(listados & 2) listado_objetos();
	}

	if(noexe) return;

	printf(translate(31));

   	/* descomprime la edivrun.lib y guarda una copia con el nombre temp.dj! */
   	_encriptar(0,edivrun_lib,la_clave);
   	_comprimir(0,"temp.dj!");

   	/* si el archivo de salida ya existe, lo borra */
   	if((f = fopen(outfilename, "rb"))!=NULL) {
   		fclose(f);
   		remove(outfilename);
   	}

   	/* renombra temp.dj! al nombre del exe */
	if(rename("temp.dj!",outfilename)) {
		printf(translate(33));	/* error escribiendo ejecutable */
		remove("temp.dj!");
		exit(1);
	}

	/* ordenamos varindex */
	ordena_varindex();

	/* escribimos en un temporal todo el indice de variables */
	fvar=tmpfile();
	for(n=0;n<num_indexed_vars;n++) {
		fputc(varindex[n].hash,fvar);
		fwrite(varindex[n].nombre,1,strlen(varindex[n].nombre)+1,fvar);
		fwrite(&varindex[n].offset,1,4,fvar);
		fputc(varindex[n].tipo,fvar);
	}

	/* liberamos varindex */
	for(n=0;n<num_indexed_vars;n++)
		free(varindex[n].nombre);
	free(varindex);

	/* lo pasamos todo del temporal a la memoria */
	l=ftell(fvar);
	fseek(fvar,0,SEEK_SET);
	varptr=(byte*)e_malloc(l);
	fread(varptr,1,l,fvar);
	fclose(fvar);
	#ifdef _DEBUG
		if(fvar=fopen("varindex.out","wb")) {
			fwrite(varptr,1,l,fvar);
			fclose(fvar);
		}
	#endif

	if ((f=fopen(outfilename,"ab"))!=NULL) {
		fwrite(nombre_program,strlen((const char*)nombre_program)+1,1,f);
		p=(byte*)e_malloc((imem+iloc)*4);
		m=(imem+iloc)*4+1024;
		q=(byte*)e_malloc(m);
		if (p!=NULL && q!=NULL) {
			fwrite(mem,4,9,f); /* mem[0..8] */
			memcpy(p,&mem[9],(imem-9)*4);
			memcpy(p+(imem-9)*4,loc,iloc*4);
			n=(imem-9+iloc)*4;
			if (!compress(q,&m,p,n)) {
				fwrite(&n,1,4,f); /* mem[0]..mem[8],longitud_datos_descomp,longitud_datos_comp,datos comp... */
				fwrite(&m,1,sizeof(unsigned long),f);
				fwrite(q,1,m,f);
				free(q); free(p);
				m=l*2;
				q=(byte*)e_malloc(m);
				if(!compress(q,&m,varptr,l)) { /* nºvariables,longitud_datos_descomp,longitud_datos_comp,datos_comp... */
					fwrite(&num_indexed_vars,1,4,f);
					fwrite(&l,1,4,f);
					fwrite(&m,1,sizeof(unsigned long),f);
					fwrite(q,1,m,f);
					free(q);
					free(varptr);
					if(debug) {			/* formato de ejecutable de debug */
						printf(translate(32));
						start_lin=ftell(f);
						escribe_lin(f);
						start_dbg=ftell(f);
						escribe_dbg(f);
						fwrite(&start_lin,1,4,f);
						fwrite(&start_dbg,1,4,f);
					}			
					fwrite(&stub_size,1,4,f); /* Ultimos 4 bytes siempre son el tamaño del stub */
					fclose(f);
				} else {
					free(q);
					free(varptr);
					fclose(f);
					errormem();
				}
			} else {
				free(q); free(p);
				free(varptr);
				fclose(f);
				errormem();
			}
		} else {
			if (p!=NULL) free(p);
			if (q!=NULL) free(q);
			free(varptr);
			fclose(f);
			errormem();
		}
	}
	else {
		free(varptr);
		printf(translate(33));
		exit(1);
	}
	
	#ifndef _WIN32
		chmod(outfilename,493); /* -rwxr-xr-x */
	#endif

	printf(translate(34));
}


/*
 * void test_buffer(int **bufer, int *maximo, int n);
 * Comprueba los límites de un buffer de generación de código
 */
void test_buffer(int **buffer,int *maximo,int n)
{
	int max;

	if (n+buffer_grow>2*1000*1000) errormem();

	if (n>*maximo-security_distance) {
		max=*maximo;
		*maximo=n+buffer_grow;
		max=*maximo-max;
		if (*buffer==mem) {
			if ((*buffer=mem_ory=(int *)realloc(*buffer,*maximo*sizeof(int)))==NULL)
				errormem();
		} else {
			if ((*buffer=(int *)realloc(*buffer,*maximo*sizeof(int)))==NULL)
				errormem();
		}
		memset((byte*)*buffer+(*maximo-max)*sizeof(int),0,max*sizeof(int));
	}
}


/*
 * void sentencia(void)
 * Analiza un bloque de sentencias
 */
void sentencia(void) {

  int im1,im2,im3,im4;
  int dir,from,to,step;

  while (pieza>=p_return) {
    test_buffer(&mem,&imem_max,imem);
    switch (pieza) {
      case p_return:
        inicio_sentencia();
        lexico(); if (pieza==p_abrir) {
          lexico(); if (pieza!=p_cerrar) {
            expresion(); if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
            g1(lrtf);
            } else {
            g1(lret);
            } lexico();
          } else {
          g1(lret);
        }
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
        final_sentencia(); grabar_sentencia(); break;
      case p_if:
        telseif[itelseif++]=0;
        inicio_sentencia();
        lexico();
        if (!free_sintax) if (pieza!=p_abrir) error(3,22); /* esperando '(' */
        if (pieza==p_abrir) lexico();
        condicion();
        if (!free_sintax) if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
        if (pieza==p_cerrar) lexico();
        g2(ljpf,0); im1=imem-1;
        final_sentencia(); grabar_sentencia();
        if1:
        sentencia();
        if (pieza==p_else) {
          inicio_sentencia();
          lexico();
          g2(ljmp,0); mem[im1]=imem; im1=imem-1;
          final_sentencia(); grabar_sentencia();
          sentencia();
        }else if (pieza==p_elseif) {
          if (itelseif==0) error(0,73); /* elseif fuera de bloque if */
          inicio_sentencia();
          g2(ljmp,0); telseif[itelseif++]=imem-1;
          mem[im1]=imem;
		  lexico();
          if (!free_sintax) if (pieza!=p_abrir) error(3,22); /* esperando '(' */
          if (pieza==p_abrir) lexico();
          condicion();
          if (!free_sintax) if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
          if (pieza==p_cerrar) lexico();
          g2(ljpf,0); im1=imem-1;
          final_sentencia(); grabar_sentencia();
          goto if1;
        }
        mem[im1]=imem; if (pieza!=p_end) error(0,57); lexico(); /* esperando END */
        while (telseif[--itelseif]!=0) mem[telseif[itelseif]]=imem;
        break;
      case p_loop:
        tbreak[itbreak++]=0; tcont[itcont++]=0;
        lexico();
        im1=imem; sentencia();
        if (pieza!=p_end) error(0,57); /* esperando END */
        inicio_sentencia(); lexico();
        g2(ljmp,im1);
        while (tbreak[--itbreak]!=0) mem[tbreak[itbreak]]=imem;
        while (tcont[--itcont]!=0) mem[tcont[itcont]]=im1;
        final_sentencia(); grabar_sentencia();
        break;
      case p_while:
        inicio_sentencia();
        tbreak[itbreak++]=0; tcont[itcont++]=0; im1=imem;
        lexico();
        if (!free_sintax) if (pieza!=p_abrir) error(3,22); /* esperando '(' */
        if (pieza==p_abrir) lexico();
        condicion();
        if (!free_sintax) if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
        if (pieza==p_cerrar) lexico();
        g2(ljpf,0); im2=imem-1;
        final_sentencia(); grabar_sentencia();
        sentencia();
        if (pieza!=p_end) error(0,57); inicio_sentencia(); /* esperando END */
        lexico();
        g2(ljmp,im1); mem[im2]=imem;
        while (tbreak[--itbreak]!=0) mem[tbreak[itbreak]]=imem;
        while (tcont[--itcont]!=0) mem[tcont[itcont]]=im1;
        final_sentencia(); grabar_sentencia();
        break;
      case p_repeat:
        tbreak[itbreak++]=0; tcont[itcont++]=0;
        lexico();
        im1=imem; sentencia();
        if (pieza!=p_until) error(0,58); /* esperando UNTIL */
        inicio_sentencia();
        lexico();
        if (!free_sintax) if (pieza!=p_abrir) error(3,22); /* esperando '(' */
        if (pieza==p_abrir) lexico();
        condicion();
        if (!free_sintax) if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
        if (pieza==p_cerrar) lexico();
        g2(ljpf,im1);
        while (tbreak[--itbreak]!=0) mem[tbreak[itbreak]]=imem;
        while (tcont[--itcont]!=0) mem[tcont[itcont]]=im1;
        final_sentencia(); grabar_sentencia();
        break;
      case p_from:
        inicio_sentencia();
        tbreak[itbreak++]=0; tcont[itcont++]=0;
        lexico();
        if (pieza!=p_id) error(0,59); /* esperando una variable */

        if ((*o).tipo==tvglo) {
          dir=(*o).vglo.offset; g2(lcar,dir);
        } else if ((*o).tipo==tvloc && (!(*o).bloque || (*o).bloque==bloque_actual)) {
          dir=-(*o).vloc.offset;
          g2(lcar,-dir); g1(laid);
        } else error(0,59); /* esperando una variable */

        lexico();
        if (pieza!=p_asig) error(3,7); lexico(); /* esperando '=' */
        from=constante();
        if (pieza!=p_to) error(1,60); lexico(); /* esperando TO */
        to=constante();
        if (from==to) error(4,61); /* sentencia FROM incorrecta */
        if (pieza==p_step) {
          lexico();
          step=constante();
          if (from<to && step<=0) error(4,62); /* el valor step no es válido */
          if (from>to && step>=0) error(4,62);
        } else {
          if (from<to) step=1; else step=-1;
        }
        g2(lcar,from); /* Asignación del from */
        g1(lasi); g1(lasp);

        im1=imem; /* Inicio del bucle */

        if (dir>=0) { /* Comparación de la condición de permanencia */
          g2(lcar,dir);
        } else {
          g2(lcar,-dir); g1(laid);
        }
        g1(lptr); g2(lcar,to);
        if (step>0) g1(lmei); else g1(lmai);
        g2(ljpf,0); im2=imem-1;

        final_sentencia(); grabar_sentencia();
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();

        sentencia();
        if (pieza!=p_end) error(0,57); inicio_sentencia(); /* esperando END */
        lexico();

        im3=imem; /* Posición del continue */

        if (dir>=0) { /* Incremento y vuelta al inicio del bucle */
          g2(lcar,dir);
        } else {
          g2(lcar,-dir); g1(laid);
        }
        g2(lcar,step);
        g1(lada); g1(lasp);
        g2(ljmp,im1); mem[im2]=imem;

        while (tbreak[--itbreak]!=0) mem[tbreak[itbreak]]=imem;
        while (tcont[--itcont]!=0) mem[tcont[itcont]]=im3;
        final_sentencia(); grabar_sentencia();

        break;

      case p_for:
        inicio_sentencia();
        tbreak[itbreak++]=0; tcont[itcont++]=0;
        lexico();
        if (pieza!=p_abrir) error(3,22); lexico(); /* esperando '(' */
        if (pieza!=p_ptocoma) {
          expresion(); g1(lasp);
          while (pieza==p_coma) { lexico(); expresion(); g1(lasp); }
        } im1=imem;
        if (pieza!=p_ptocoma) error(3,9); lexico(); /* esperando ';' */
        if (pieza==p_ptocoma) {
          g2(lcar,1);
        } else expresion();
        g2(ljpf,0); im2=imem-1;
        while (pieza==p_coma) { lexico(); expresion();
          g2(ljpf,im2); im2=imem-1; }
        g2(ljmp,0); im3=imem-1;
        if (pieza!=p_ptocoma) error(3,9); lexico(); /* esperando ';' */
        if (pieza!=p_cerrar) {
          expresion(); g1(lasp);
          while (pieza==p_coma) { lexico(); expresion(); g1(lasp); }
        }
        g2(ljmp,im1);
        if (pieza!=p_cerrar) error(3,18); lexico(); /* esperando ')' */
        final_sentencia(); grabar_sentencia();
        mem[im3++]=imem; sentencia();
        if (pieza!=p_end) error(0,57); /* esperando END */
        inicio_sentencia(); lexico();
          g2(ljmp,im3);
          do { im1=mem[im2]; mem[im2]=imem; im2=im1; } while(im2);
        while (tbreak[--itbreak]!=0) mem[tbreak[itbreak]]=imem;
          while (tcont[--itcont]!=0) mem[tcont[itcont]]=im3;
        final_sentencia(); grabar_sentencia();
        break;
      case p_switch:
        inicio_sentencia();
        lexico();
        if (!free_sintax) if (pieza!=p_abrir) error(3,22); /* esperando '(' */
        if (pieza==p_abrir) lexico();
        condicion();
        if (!free_sintax) if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
        if (pieza==p_cerrar) lexico();
        while (pieza==p_ptocoma) {
          lexico();
        }
        final_sentencia(); grabar_sentencia();
        im1=0; im2=0;
        while (pieza!=p_end) {
          inicio_sentencia();
          if (pieza==p_case) {
            im3=0; do {

              lexico();
              if (im1) mem[im1]=imem;
              expresion(); if (pieza!=p_rango) {
                g2(lcse,0); im1=imem-1;
              } else {
                lexico(); expresion();
                g2(lcsr,0); im1=imem-1;
              }

              if (pieza==p_coma) {
                g2(ljmp,im3); im3=imem-1;
              }

            } while (pieza==p_coma);

            while(im3) { im4=mem[im3]; mem[im3]=imem; im3=im4; }

          } else if (pieza==p_default) {
            lexico();
            if (im1) mem[im1]=imem; im1=0;
          } else error(0,63); /* esperando case, default o end */
          if (!free_sintax) if (pieza!=p_ptocoma) error(3,64); /* esperando ':' */
          while (pieza==p_ptocoma || pieza==p_coma) lexico();
          g1(lasp);
          final_sentencia(); grabar_sentencia();
          sentencia();
          if (pieza!=p_end) error(0,57); /* esperando END */
          inicio_sentencia();
            g2(ljmp,im2); im2=imem-1;
          pasa_ptocoma();
          final_sentencia(); grabar_sentencia();
        } inicio_sentencia();
        if (im1) mem[im1]=imem; g1(lasp);
        while(im2) { im1=mem[im2]; mem[im2]=imem; im2=im1; }
        lexico();
        final_sentencia(); grabar_sentencia(); break;
      case p_frame:
        inicio_sentencia();
        lexico(); if (pieza==p_abrir) {
          lexico(); if (pieza!=p_cerrar) {
            expresion(); if (pieza!=p_cerrar) error(3,18); /* esperando ')' */
            g1(lfrf);
          } else {
            g1(lfrm);
          } lexico();
        } else {
          g1(lfrm);
        }
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
        final_sentencia(); grabar_sentencia(); break;
      case p_debug:
        inicio_sentencia();
        g1(ldbg); lexico();
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
        final_sentencia(); grabar_sentencia();
        break;
      case p_break:
        inicio_sentencia();
        if (itbreak==0) error(0,65); lexico(); /* break fuera de un bucle */
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
        g2(ljmp,0); tbreak[itbreak++]=imem-1;
        final_sentencia(); grabar_sentencia(); break;
      case p_continue:
        inicio_sentencia();
        if (itcont==0) error(0,66); lexico(); /* continue fuera de un bucle */
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
        g2(ljmp,0); tcont[itcont++]=imem-1;
        final_sentencia(); grabar_sentencia(); break;
      case p_clone:
        inicio_sentencia();
        lexico(); g2(lclo,0); im1=imem-1;
        final_sentencia(); grabar_sentencia();
        sentencia();
        if (pieza!=p_end) error(0,57); lexico(); /* esperando END */
        mem[im1]=imem; break;
      case p_ptocoma:
        lexico(); break;
      default:
        inicio_sentencia();
        error_25=67;
        expresion();
        do {
          _exp--;
        } while ((*_exp).tipo==eoper && (*_exp).token==p_string);
        error_25=25;
        switch((*_exp).tipo) {
          case ecall: break;
          case efext: break;
          case eoper:
            switch((*_exp).token) {
              case p_asig: case p_inc: case p_suma: case p_dec: case p_resta:
              case p_add_asig: case p_sub_asig: case p_mul_asig:
              case p_div_asig: case p_mod_asig: case p_and_asig:
              case p_or_asig: case p_xor_asig: case p_shr_asig: case p_shl_asig:

              case p_asigword: case p_incword: case p_sumaword: case p_decword: case p_restaword:
              case p_add_asigword: case p_sub_asigword: case p_mul_asigword:
              case p_div_asigword: case p_mod_asigword: case p_and_asigword:
              case p_or_asigword: case p_xor_asigword: case p_shr_asigword: case p_shl_asigword:

              case p_asigchar: case p_incchar: case p_sumachar: case p_decchar: case p_restachar:
              case p_add_asigchar: case p_sub_asigchar: case p_mul_asigchar:
              case p_div_asigchar: case p_mod_asigchar: case p_and_asigchar:
              case p_or_asigchar: case p_xor_asigchar: case p_shr_asigchar: case p_shl_asigchar:

              case p_strcpy: case p_strcat: case p_strsub: break;
              default: error(4,68); break; /* expresion sin sentido */
            } break;
          default: error(4,68); /* expresion sin sentido */
        }
        if (!free_sintax) if (pieza!=p_ptocoma) error(3,9); /* esperando ';' */
        while (pieza==p_ptocoma || pieza==p_coma) lexico();
        g1(lasp);
        final_sentencia(); grabar_sentencia();
        break;
    }
  }
}


/*
 * Funciones de generación de código
 */

void g1(int op) {
  if (optimizar) gen(0,op,0); else mem[imem++]=op;
}

void g2(int op, int pa) {
  if (optimizar) gen(1,op,pa); else { mem[imem++]=op; mem[imem++]=pa; }
}

/*
 *  Optimización peephole de código intermedio EML
 */

void gen(int param, int op, int pa) 
{
  optimizado=0;

  switch(op) {
    case lcar:
      if (code[15].op==lcar) {
        code[15].op=mem[imem-2]=lcar2;
        code[15].param=2;
        mem[imem++]=pa; optimizado=1;
      } else if (code[15].op==lcar2) {
        code[15].op=mem[imem-3]=lcar3;
        code[15].param=3;
        mem[imem++]=pa; optimizado=1;
      } else if (code[15].op==lcar3) {
        code[15].op=mem[imem-4]=lcar4;
        code[15].param=4;
        mem[imem++]=pa; optimizado=1;
      } break;
    case lasp:
      if (code[15].op==lasi) {
        if (code[14].op==lcar) {
          remove_code(1);
          code[15].op=mem[imem-2]=lcarasiasp;
        } else {
          code[15].op=mem[imem-1]=lasiasp;
          optimizado=1;
        }
      } else if (code[15].op==lfun) {
        code[15].op=mem[imem-2]=lfunasp;
        optimizado=1;
      } break;
    case laid:
      if (code[15].op==lcar) {
        code[15].op=mem[imem-2]=lcaraid;
        optimizado=1;
      } break;
    case lptr:
      if (code[15].op==lcar) {
        code[15].op=mem[imem-2]=lcarptr;
        optimizado=1;
      } else if (code[15].op==lcaradd) {
        code[15].op=mem[imem-2]=lcaraddptr;
        optimizado=1;
      } else if (code[15].op==laid) {
        code[15].op=mem[imem-1]=laidptr;
        optimizado=1;
      } else if (code[15].op==ladd) {
        code[15].op=mem[imem-1]=laddptr;
        optimizado=1;
      } else if (code[15].op==lcaraid) {
        code[15].op=mem[imem-2]=lcaraidptr;
        optimizado=1;
      } break;
    case lcpa:
      if (code[15].op==lcaraid) {
        code[15].op=mem[imem-2]=lcaraidcpa;
        optimizado=1;
      } break;
    case ladd:
      if (code[15].op==lcar2){
        code[15].op=mem[imem-3]=lcar;
        code[15].param=1;
        mem[imem-2]+=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar3){
        code[15].op=mem[imem-4]=lcar2;
        code[15].param=2;
        mem[imem-2]+=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar4){
        code[15].op=mem[imem-5]=lcar3;
        code[15].param=3;
        mem[imem-2]+=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar){
        if (mem[imem-1]==0) remove_code(1); else {
          code[15].op=mem[imem-2]=lcaradd;
          optimizado=1;
        }
      } else if (code[15].op==lcarmul){
        code[15].op=mem[imem-2]=lcarmuladd;
        optimizado=1;
      } break;
    case lmul:
      if (code[15].op==lcar2){
        code[15].op=mem[imem-3]=lcar;
        code[15].param=1;
        mem[imem-2]*=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar3){
        code[15].op=mem[imem-4]=lcar2;
        code[15].param=2;
        mem[imem-2]*=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar4){
        code[15].op=mem[imem-5]=lcar3;
        code[15].param=3;
        mem[imem-2]*=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar){
        if (mem[imem-1]==1) remove_code(1); else {
          code[15].op=mem[imem-2]=lcarmul;
          optimizado=1;
        }
      } break;
    case lsub:
      if (code[15].op==lcar2){
        code[15].op=mem[imem-3]=lcar;
        code[15].param=1;
        mem[imem-2]-=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar3){
        code[15].op=mem[imem-4]=lcar2;
        code[15].param=2;
        mem[imem-2]-=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar4){
        code[15].op=mem[imem-5]=lcar3;
        code[15].param=3;
        mem[imem-2]-=mem[imem-1];
        imem--; optimizado=1;
      } else if (code[15].op==lcar){
        if (mem[imem-1]==0) remove_code(1); else {
          code[15].op=mem[imem-2]=lcarsub;
          optimizado=1;
        }
      } break;
    case ldiv:
      if (code[15].op==lcar2){
        if (mem[imem-1]!=0) {
          code[15].op=mem[imem-3]=lcar;
          code[15].param=1;
          mem[imem-2]/=mem[imem-1];
          imem--; optimizado=1;
        }
      } else if (code[15].op==lcar3){
        if (mem[imem-1]!=0) {
          code[15].op=mem[imem-4]=lcar2;
          code[15].param=2;
          mem[imem-2]/=mem[imem-1];
          imem--; optimizado=1;
        }
      } else if (code[15].op==lcar4){
        if (mem[imem-1]!=0) {
          code[15].op=mem[imem-5]=lcar3;
          code[15].param=3;
          mem[imem-2]/=mem[imem-1];
          imem--; optimizado=1;
        }
      } else if (code[15].op==lcar){
        if (mem[imem-1]==1) remove_code(1);
        else if (mem[imem-1]!=0) {
          code[15].op=mem[imem-2]=lcardiv; /* Un cardiv nunca será "cardiv 0" */
          optimizado=1;
        }
      } break;
    case lneg:
      if (code[15].op==lcar || code[15].op==lcar2 || code[15].op==lcar3 || code[15].op==lcar4) {
        mem[imem-1]=-mem[imem-1]; optimizado=1;
      }
      break;
    case lnot:
      if (code[15].op==lcar || code[15].op==lcar2 || code[15].op==lcar3 || code[15].op==lcar4) {
        mem[imem-1]=mem[imem-1]^-1; optimizado=1;
      }
      break;
  }

  if (!optimizado) {
    if (imem-code[15].dir>2) delete_code();
    add_code(imem,param,op);
    if (param) { mem[imem++]=op; mem[imem++]=pa; } else mem[imem++]=op;
  }

}

void remove_code(int i) {
  int n;
  optimizado=1;
  while (i--) {
    imem-=code[15].param+1;
    for (n=15;n>0;n--) {
      code[n].dir=code[n-1].dir;
      code[n].param=code[n-1].param;
      code[n].op=code[n-1].op;
    } code[0].dir=0; code[0].param=0; code[0].op=0;
  }
}

void delete_code(void) {
  int n;
  for (n=0;n<16;n++) {
    code[n].dir=0; code[n].param=0; code[n].op=0;
  }
}

void add_code(int dir, int param, int op) {
  int n;
  for (n=0;n<15;n++) {
    code[n].dir=code[n+1].dir;
    code[n].param=code[n+1].param;
    code[n].op=code[n+1].op;
  } code[15].dir=dir; code[15].param=param; code[15].op=op;
}


/*
 * Con el primer token leido guarda el inicio de una sentencia
 */
void inicio_sentencia(void)
{
	inicio=imem;
	inicio_prg=(int)(ierror-prog);
}

/*
 * Con el primer token que no es de la sentecia guarda el fin de una sentencia
 */
void final_sentencia(void)
{
	final=imem-1;
	final_prg=(int)(old_ierror_end-prog);
}

/*
 * Guarda un registro (inicio,final,linea1,...) en el temporal (linf)
 */

void grabar_sentencia(void)
{
	/*if(debug && linf!=NULL) {
		fwrite(&inicio,4,1,linf);
		fwrite(&final,4,1,linf);
		fwrite(&linea1,4,1,linf);
		fwrite(&columna1,4,1,linf);
		fwrite(&linea2,4,1,linf);
		fwrite(&columna2,4,1,linf);
	}*/
	if(debug && linf!=NULL) {
		fwrite(&inicio,4,1,linf);
		fwrite(&final,4,1,linf);
		fwrite(&inicio_prg,4,1,linf);
		fwrite(&final_prg,4,1,linf);
		linsize++;
	}
}

/*
 * Agrega información de depurado al ejecutable
 */

void escribe_lin(FILE* f)
{
	int b=0;
	int l;
	byte* progcomp;
#ifdef _DEBUG
	FILE* ediv_lin;
	int j;
#endif

	/* comprimimos el codigo fuente */
	while(prog[b]!=0) b++;
	b++;
	progcomp=e_malloc(b*2);
	l=b*2;
	if(compress(progcomp,(unsigned long*)&l,prog,b)) {
		fclose(linf);
		fclose(f);
		errormem();
	}
	/* escribe el tamaño del codigo descomprimido */
	fwrite(&b,1,4,f);
	/* escribe el tamaño del codigo comprimido */
	fwrite(&l,1,4,f);
	/* escribe el codigo comprimido */
	fwrite(progcomp,1,l,f);
	free(progcomp);
	
	/* Escribe el tamaño del LIN */
	#ifdef _DEBUG
		printf("dbg: linsize: %d\n",linsize);
	#endif
	fwrite(&linsize,1,4,f);
	#ifdef _DEBUG
		ediv_lin=fopen("ediv.lin","w");
		fprintf(ediv_lin,"linsize: %d\n",linsize);
	#endif
	/* Escribe la información LIN (offset de cada sentencia en el prg y en el bytecode) */
	fseek(linf,0,SEEK_SET);
	while((b=fgetc(linf))!=EOF) {
		fputc(b,f);
	}
	#ifdef _DEBUG
		j=0;
		fseek(linf,0,SEEK_SET);
		while(!feof(linf)) {
			fread(&b,1,4,linf);
			fprintf(ediv_lin,"[%d] eml_start\t%d\n",j,b);
			fread(&b,1,4,linf);
			fprintf(ediv_lin,"[%d] eml_end  \t%d\n",j,b);
			fread(&b,1,4,linf);
			fprintf(ediv_lin,"[%d] prg_start\t\t%d\n",j,b);
			fread(&b,1,4,linf);
			fprintf(ediv_lin,"[%d] prg_end  \t\t%d\n",j++,b);
		}
		fclose(ediv_lin);
	#endif
	
	fclose(linf);
}		


/*
 * Escribe información sobre los objetos
 */
void escribe_dbg(FILE* f)
{
	int n;
	
	/* Datos comunes de cada objeto */
	struct {
		int tipo;
		int nombre;
		int bloque;
		int miembro;
		int v0,v1,v2,v3,v4,v5;
	} ob;
	
	/* Cabecera de sección DBG */
	fwrite(&num_obj,4,1,f);
	fwrite(&num_obj_predefinidos,4,1,f);
	n=(int)&obj[0];
	fwrite(&n,4,1,f);
	n=sizeof(struct objeto);
	fwrite(&n,4,1,f);
	
	/* Escribe los datos de cada objeto */
	for (n=0;n<num_obj;n++) {
		ob.tipo=(int)obj[n].tipo;
		ob.nombre=(int)obj[n].name-(int)vnom; /* offset que apunta al nombre en vnom */
		ob.bloque=(int)obj[n].bloque;
		ob.miembro=(int)obj[n].member;
		ob.v0=(int)obj[n].sglo.offset;
		ob.v1=(int)obj[n].sglo.len_item;
		ob.v2=(int)obj[n].sglo.totalitems;
		ob.v3=(int)obj[n].sglo.items1;
		ob.v4=(int)obj[n].sglo.items2;
		ob.v5=(int)obj[n].sglo.items3;
		if (obj[n].tipo==tpsgl || obj[n].tipo==tpslo) ob.v1=(ob.v1-(int)&obj[0])/sizeof(struct objeto);

		/* OJO ! que no se pueden añadir objetos aquí (ver uso de &obj[0] y sizeof(struct objeto)) */
		fwrite(&ob,sizeof(ob),1,f);
	}
	
	/* Escribe el tamaño del vector de nombres */
	n=(int)ivnom.b-(int)vnom;
	fwrite(&n,4,1,f);
	
	/* Escribe el vector de nombres */
	fwrite(vnom,1,n,f);
}
