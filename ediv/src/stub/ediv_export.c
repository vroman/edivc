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

#ifdef _WIN32
#	include <windows.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "main.h"
#include "extern.h"
#include "parser.h"
#include "compiler.h"
#include "varindex.h"

#include "language.h"

//int len,ret,cod;
int len;
/*struct objeto * ob;
struct objeto * ob2;*/

int creaobj;

byte decl_struct=0;		// 1 si se está declarando un struct

byte slocal=0;			// para las structs. 0=global, 1=local
int nregs;
int offset;

// no creo que haga falta (sólo serviría para comprobar errores en las dll)
// -> nos ahorramos la tabla de objetos

/*int crea_objeto(byte * nombre)
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
		return 2;		// genera un error "el nombre no es nuevo"
	} else {
        *ptr=_ivnom; ptr_o=(void*)(_ivnom+4); *ptr_o=o=iobj++; // id nuevo
        (*o).name=(byte*)_ivnom+8;
        (*o).member=member;
        if (num_obj++==max_obj) return 3;	// error "demasiados objetos"
	}
	
	(*o).dll=numdlls;
//	(*o).usado_dll=0;
	
	return 0;
}*/


// Errores específicos en la ejecución de una DLL

void dll_error(int error, ...)
{
	va_list opparam;
	char mensaje[256];

	va_start(opparam,error);

	sprintf(mensaje,translate(3), fichdll);
	vsprintf(mensaje,translate_dll_error(error),opparam);

	#ifdef _WIN32
		MessageBox(0,mensaje,translate(4),MB_ICONERROR);
	#else
		printf("%s\n",mensaje);
	#endif

	/*printf("\n");
	n_errors++;*/

	exit(-1);
}



int EDIV_Export(char* cadena, int nparam, void* hfuncion)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

//	creaobj=crea_objeto((byte*)cadena);

    #ifdef DEBUG_DLL
		printf("export_function:\t-- ID FUNCION: %d\n"
			   "export_function:\tCadena exportada: %s\n"
			   "export_function:\tN. parametros: %d\n"
			   "export_function:\tOffset de la funcion: 0x%X\n",n_externs,cadena,nparam,(unsigned int)hfuncion);
	#endif

/*	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

    (*ob).tipo=tfext;
    (*ob).fext.codigo=n_externs;
    (*ob).fext.num_par=nparam; // es necesario?*/

	extfuncs[n_externs]=hfuncion;

/*    for(i=0;i<numdlls;i++)
    	for(j=0;j<dlls[i].nfuncs;j++)
			if(strcmp(cadena,dlls[i].ext_funcs[j].cadena)==0) {
				dll_error(4,cadena);
				return 0;
			}*/

/*#define actfunc dlls[numdlls-1].ext_funcs[dlls[numdlls-1].nfuncs]

	actfunc.cadena=(char*)e_malloc(strlen(cadena)+1);
	strcpy(actfunc.cadena,cadena);
	actfunc.nparam = nparam;
	actfunc.hfuncion = hfuncion;*/

//	dlls[numdlls-1].nfuncs++;
	n_externs++;

	return 1;
}


int EDIV_Export_Entrypoint(int ep, void* hfuncion)
{
	entrypoints[n_entrypoints].tipo=ep;
	entrypoints[n_entrypoints].hfuncion=hfuncion;
	entrypoints[n_entrypoints++].dll=numdlls;
	return 0;
}


int EDIV_Export_Priority(int priority)
{
	dlls[numdlls].prioridad=priority;
	return 0;
}


int EDIV_Export_Const(char* cadena, int valor)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

	// sólamente se tendrá que hacer algo en la versión con trazador

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_const:\t-- ID OBJETO: %d\n"
			   "export_const:\tCadena exportada: %s\n"
			   "export_const:\tValor: %d\n"
			   "export_const:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,valor,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	(*ob).tipo=tcons;
	(*ob).cons.valor=valor;
	(*ob).cons.literal=0;
*/
	return 1;
}


int EDIV_Export_Global(char* cadena, int valor)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_global:\t-- ID OBJETO: %d\n"
			   "export_global:\tCadena exportada: %s\n"
			   "export_global:\tValor: %d\n"
			   "export_global:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,valor,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	(*ob).tipo=tvglo;
	(*ob).vglo.offset=imem;
	mem[imem]=valor;
*/
	return dimem++;
}


int EDIV_Export_Global_Tab(char* cadena, int numregs)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_global_tab:\t-- ID OBJETO: %d\n"
			   "export_global_tab:\tCadena exportada: %s\n"
			   "export_global_tab:\tN.regs: %d\n"
			   "export_global_tab:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,numregs,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	(*ob).tipo=ttglo;
	(*ob).tglo.offset=imem;
	(*ob).tglo.len1=len=numregs;
	(*ob).tglo.len2=-1;
	(*ob).tglo.len3=-1;
	(*ob).tglo.totalen=len+1;

	// Inicializamos la tabla a 0
	do {
		mem[imem++]=0;
	} while (len--);
*/
	offset=dimem;
	dimem+=numregs;

	return offset;
}


int EDIV_Export_Global_Struct(char* cadena, int numregs)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

	nregs=numregs;
	slocal=0;
/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_global_struct:\t-- ID OBJETO: %d\n"
			   "export_global_struct:\tCadena exportada: %s\n"
			   "export_global_struct:\tN.regs: %d\n"
			   "export_global_struct:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,numregs,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	decl_struct=1;

	(*ob).tipo=tsglo;
	(*ob).sglo.offset=imem;
	(*ob).sglo.items1=numregs;
	(*ob).sglo.items2=(*ob).sglo.items3=-1;
	(*ob).sglo.totalitems=numregs+1;
	(*ob).sglo.len_item=0;
	member=ob;
	len=0;
*/
	decl_struct=1;
	len=0;
	return dimem;
}


int EDIV_Export_Member_Int(char* cadena, int valor)
{
	if(!decl_struct) {
		dll_error(7,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_member_int:\t-- ID OBJETO: %d\n"
			   "export_member_int:\tCadena exportada: %s\n"
			   "export_member_int:\tValor: %d\n"
			   "export_member_int:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,valor,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob2=o;

	if ((*ob2).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	if((*ob).tipo==tsglo) {			// int miembro de struct global
		(*ob2).tipo=tvglo;
		(*ob2).vglo.offset=len++;
		mem[imem]=valor;
		(*ob).sglo.len_item++;
		return imem++;
	}
	else {							// int miembro de struct local
        (*ob2).tipo=tvloc;
        (*ob2).vloc.offset=len++;
        loc[iloc]=valor;
        (*ob).sloc.len_item++;
        return iloc++;
	}
	*/

	return len++;
}


int EDIV_Export_Member_Str(char* cadena, int tamano)
{
	if(!decl_struct) {
		dll_error(7,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_member_str:\t-- ID OBJETO: %d\n"
			   "export_member_str:\tCadena exportada: %s\n"
			   "export_member_str:\tTamano: %d\n"
			   "export_member_str:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,tamano,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob2=o;

	if ((*ob2).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	if((*ob).tipo==tsglo) {			// string miembro de struct global

		(*ob2).tipo=tcglo;
		(*ob2).cglo.offset=len+1;

		if (((*ob2).cglo.totalen=tamano)<0) {
			dll_error(8,cadena);
			return 0;
		}
		if ((*ob2).cglo.totalen>0xFFFFF) {
			dll_error(8,cadena);
			return 0;
		}

		mem[imem]=0xDAD00000|(*ob2).cglo.totalen;
		len+=1+((*ob2).cglo.totalen+5)/4;
		mem[imem+1]=0;
		imem+=1+((*ob2).cglo.totalen+5)/4;
		(*ob).sglo.len_item+=1+((*ob2).cglo.totalen+5)/4;
		
		return (*ob2).cglo.offset;
	}
	else {							// string miembro de struct local

        (*ob2).tipo=tcloc;
        (*ob2).cloc.offset=len+1;

		if (((*ob2).cloc.totalen=tamano)<0) {
			dll_error(8,cadena);
			return 0;
		}
		if ((*ob2).cloc.totalen>0xFFFFF) {
			dll_error(8,cadena);
			return 0;
		}

		loc[iloc]=0xDAD00000|(*ob2).cloc.totalen;
		len+=1+((*ob2).cloc.totalen+5)/4;
		loc[iloc+1]=0;
		iloc+=1+((*ob2).cloc.totalen+5)/4;
		(*ob).sloc.len_item+=1+((*ob2).cloc.totalen+5)/4;
		
		return (*ob2).cloc.offset;
	}*/

	offset=len;
	len+=tamano;

	return offset;

}


int EDIV_Export_Member_Tab(char* cadena, int numregs)
{
	if(!decl_struct) {
		dll_error(7,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_member_tab:\t-- ID OBJETO: %d\n"
			   "export_member_tab:\tCadena exportada: %s\n"
			   "export_member_tab:\tN.regs: %d\n"
			   "export_member_tab:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,numregs,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob2=o;

	if ((*ob2).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	if((*ob).tipo==tsglo) {			// array miembro de struct global
		(*ob2).tipo=ttglo;
		(*ob2).tglo.offset=len;
		(*ob2).tglo.len1=numregs;
		(*ob2).tglo.len2=-1;
		(*ob2).tglo.len3=-1;
		(*ob2).tglo.totalen=numregs+1;
		len+=numregs+1;
		memset(&mem[imem],0,(numregs+1)*4);
		imem+=numregs+1;
		(*ob).sglo.len_item+=numregs+1;
		
		return (*ob2).tglo.offset;
	}
	else {							// array miembro de struct local
		(*ob2).tipo=ttloc;
		(*ob2).tloc.offset=len;
		(*ob2).tloc.len1=numregs;
		(*ob2).tloc.len2=-1;
		(*ob2).tloc.len3=-1;
		(*ob2).tloc.totalen=numregs+1;
		len+=numregs+1;
		memset(&loc[iloc],0,(numregs+1)*4);
		iloc+=numregs+1;
		(*ob).sloc.len_item+=numregs+1;
		
		return (*ob2).tloc.offset;
	}*/

	offset=len;
	len+=numregs;
	return offset;
}


int EDIV_Export_EndStruct()
{
	if(!decl_struct) {
		dll_error(9);
		return 0;
	}

/*	#ifdef DEBUG_DLL
		printf("export_endstruct:\t--- estructura cerrada\n");
	#endif

	member=NULL;

	if((*ob).tipo==tsglo) {			// cierra estructura global
		if ((*ob).sglo.len_item==0) {
			dll_error(10);
			return 0;
		}
		if ((*ob).sglo.totalitems>1) {
			len=((*ob).sglo.totalitems-1)*(*ob).sglo.len_item-1;
			do {
				mem[imem]=mem[imem-(*ob).sglo.len_item];
				imem++;
			} while (len--);
		}
	}
	else {							// cierra estructura local
		if ((*ob).sloc.len_item==0) {
			dll_error(10);
			return 0;
		}
		if ((*ob).sloc.totalitems>1) {
			len=((*ob).sloc.totalitems-1)*(*ob).sloc.len_item-1;
			do {
				loc[iloc]=loc[iloc-(*ob).sloc.len_item];
				iloc++;
			} while (len--);
		}
//        iloc+=(*ob).sloc.totalitems*(*ob).sloc.len_item;
	}
*/
	if(len==0) dll_error(10);

	if(slocal) diloc+=len*nregs;
	else dimem+=len*nregs;

	decl_struct=0;

	return 1;
}


int EDIV_Export_Local(char* cadena, int valor)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_local:\t-- ID OBJETO: %d\n"
			   "export_local:\tCadena exportada: %s\n"
			   "export_local:\tValor: %d\n"
			   "export_local:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,valor,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

    (*ob).tipo=tvloc;
    (*ob).vloc.offset=iloc;
    loc[iloc]=valor;
*/
    return diloc++;
}


int EDIV_Export_Local_Tab(char* cadena, int numregs)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_local_tab:\t-- ID OBJETO: %d\n"
			   "export_local_tab:\tCadena exportada: %s\n"
			   "export_local_tab:\tN.regs: %d\n"
			   "export_local_tab:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,numregs,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	(*ob).tipo=ttloc;
	(*ob).tloc.offset=iloc;
	(*ob).tloc.len1=len=numregs;
	(*ob).tloc.len2=-1;
	(*ob).tloc.len3=-1;
	(*ob).tloc.totalen=len+1;

	// Inicializamos la tabla a 0
	do {
		loc[iloc++]=0;
	} while (len--);

	return (*ob).tloc.offset;*/

	offset=diloc;
	diloc+=numregs;
	return offset;
}


int EDIV_Export_Local_Struct(char* cadena, int numregs)
{
	if(decl_struct) {
		dll_error(6,cadena);
		return 0;
	}

	nregs=numregs;
	slocal=1;

/*	creaobj=crea_objeto((byte*)cadena);

	#ifdef DEBUG_DLL
		printf("export_local_struct:\t-- ID OBJETO: %d\n"
			   "export_local_struct:\tCadena exportada: %s\n"
			   "export_local_struct:\tN.regs: %d\n"
			   "export_local_struct:\tOffset del objeto: 0x%X\n",num_obj-1,cadena,numregs,(unsigned int)o);
	#endif

	if(creaobj) {
		dll_error(creaobj,cadena);
		return 0;
	}

	ob=o;

	if ((*ob).tipo!=tnone) {
		dll_error(5,cadena);
		return 0;
	}

	decl_struct=1;

	(*ob).tipo=tsloc;
	(*ob).sloc.offset=iloc;
	(*ob).sloc.items1=numregs;
	(*ob).sloc.items2=(*ob).sloc.items3=-1;
	(*ob).sloc.totalitems=numregs+1;
	(*ob).sloc.len_item=0;
	member=ob;
	len=0;
*/
	decl_struct=1;
	len=0;
    return diloc;
}


////////////////
/// Call_Entrypoint
//////////////

int Call_Entrypoint(int ep, ...)
{
	va_list opparam;
	int i,c=0;
	for(i=0;i<n_entrypoints;i++) {
		if(entrypoints[i].tipo==ep) {
			c++;
			va_start(opparam,ep);
			switch(ep) {

				// void funcion(void)
				case EDIV_set_video_mode:
				case EDIV_process_palette:
				case EDIV_process_active_palette:
				case EDIV_background_to_buffer:
				case EDIV_buffer_to_video:
				case EDIV_post_process_scroll:
				case EDIV_post_process_m7:
				case EDIV_post_process_m8:
				case EDIV_post_process_buffer:
				case EDIV_post_process:
				case EDIV_ss_init:
				case EDIV_ss_frame:
				case EDIV_ss_end:
					{
						void (*funcion_ep)(void);
						funcion_ep=entrypoints[i].hfuncion;
						funcion_ep();
					}
					break;
				case EDIV_frame:
				case EDIV_first_load:
					{
						void (*funcion_ep)(struct _fun_params* fp);
						funcion_ep=entrypoints[i].hfuncion;
						funcion_ep(&fp) ;
					}
					break;
				// void funcion(char*,int)
				case EDIV_process_sound:
				case EDIV_process_fpg:
				case EDIV_process_map:
				case EDIV_process_fnt:
					{
						void (*funcion_ep)(char *ptr,int lenght);
						char* _ptr=va_arg(opparam,char*);
						int _lenght=va_arg(opparam,int);
						funcion_ep=entrypoints[i].hfuncion;
						funcion_ep(_ptr,_lenght);
					}
					break;

				case EDIV_put_sprite:
					{
						void (*funcion_ep)(unsigned char * si, int x,int y, int an, int al,
							int xg, int yg, int ang, int size, int flags);
						unsigned char* _si=va_arg(opparam,unsigned char*);
						int _x=va_arg(opparam,int);
						int _y=va_arg(opparam,int);
						int _an=va_arg(opparam,int);
						int _al=va_arg(opparam,int);
						int _xg=va_arg(opparam,int);
						int _yg=va_arg(opparam,int);
						int _ang=va_arg(opparam,int);
						int _size=va_arg(opparam,int);
						int _flags=va_arg(opparam,int);
						funcion_ep=entrypoints[i].hfuncion;
						funcion_ep(_si,_x,_y,_an,_al,_xg,_yg,_ang,_size,_flags);
					}
#ifdef DBG
				// entrypoints del trazador
				// void funcion(int,char*,int*,void*);
				case EDIV_trace:
				case EDIV_debug:
					{
						void (*funcion_ep)(int imem, char* nombreprg, int* lin, void* dbg);
						int _imem=va_arg(opparam,int);
						char* _nombreprg=va_arg(opparam,char*);
						void* _lin=va_arg(opparam,void*);
						void* _dbg=va_arg(opparam,void*);
						//assert(0);
						funcion_ep=entrypoints[i].hfuncion;
						funcion_ep(_imem,_nombreprg,_lin,_dbg);
					}
#endif

			}
		}
	}
	return c;
}
