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

/*
 * Este fichero contien el codigo del interprete del eDiv
 *   by Riseven
 *  
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>

#include "extern.h"
#include "edivfont.h"
#include "kernel.h"
#include "inte.h"
#include "main.h"
#include "dll.h"
#include "varindex.h"


int first_loaded ;

int ini_interprete()
{
	int i ;
	first_loaded = 1 ;

	pila_max = 1024 ;
	if ((pila=(int*)malloc(4*pila_max))==NULL)
		critical_error(4); // no se pudo reservar memoria para la pila

	sp = 0 ; //por si acaso


	// Posicion de inicio del code
	iloc=mem[2]; // Inicio de la imagen de las variables locales
	iloc_len=mem[6]+mem[5]; // Longitud de las locales (p£blicas y privadas)
	iloc_pub_len=mem[6]; 	// Longitud de las variables locales p£blicas
	inicio_privadas=iloc_pub_len;
	imem=mem[1]; // Principio del codigo
/*
	for ( i = 0 ; i < 10 ; i++ )
		printf( " : %i \n" , mem[i] ) ;
*/
	

	procs_s[1].imem = imem ;
	procs_s[1].tipo = 0 ;
	lista_mete( 1 ) ;
	
	// Preparamos la estructura de parametros
	fp.Call_Entrypoint = Call_Entrypoint ;
	fp.mem = mem ;
	fp.pila = pila ;
	fp.procs_s = &procs_s ;
	fp.sp = &sp ;
	fp.varindex = varindex ;
	fp.num_procs = &num_proc_orden ;
	fp.proc_orden = proc_orden ;
	fp.proceso_actual = &proceso_actual ;
	fp.existe.regions = 0 ;
	fp.existe.dibuja = 0 ;
	fp.Runtime_Error=error;
	fp.Critical_Error=critical_error;


	return 1 ;
}

int busca_proc_libre()
{
	int i ;
	for ( i = 1 ; i < 4096 ; i++ )
	{
		if ( procs_s[i].tipo == 0 )
			return i ;
	}

	return -1 ;
}

int lista_mete( int num ) 
{
	// Esto esta mal, habra que buscar su posicion segun priority
	procs_s[num].orden = num_proc_orden ;
	proc_orden[ num_proc_orden++ ] = num ;
	

	return num_proc_orden-1 ;
}


int lista_quita( int num )
{
	int i ;

	for ( i = procs_s[num].orden+1 ; i < num_proc_orden ; i++ )
	{
		procs_s[ proc_orden[ i ] ].orden-- ;
		proc_orden[ i - 1 ] = proc_orden[ i ] ;
	}
	num_proc_orden-- ;

	return 1 ;
	
}



int interprete()
{
	
	int i ;

	// PROVISIONAL. Se tendra que cambiar para k soporte frame(int) y prioritys
	if ( num_proc_orden > 0 )
	{
		for ( proceso_actual = 0 ; proceso_actual < num_proc_orden ; proceso_actual++ )
		{
			proceso( proc_orden[ proceso_actual ] ) ;
		}
		Call_Entrypoint(EDIV_frame);
	}else
	{
		stub_quit() ;
	}
	
	
	return 1 ;
}


int proceso( int num )
{
	int v1 ;
	int retcode = 0;
	int no_devuelve = 0 ;
	int (*externa)(struct _fun_params*);
	int temp ;
	
	num_proc = num ;
	imem = procs_s[num_proc].imem ;
	

	
	//assert(0);

	while( retcode == 0 )
	{
	
		//printf("imem: %i\tnum_proc: %i\tsp: %i\tpila[sp]: %i \n" , imem , num_proc , sp , pila[sp]) ;
		
		switch ( mem[ imem++ ] )
		{
		case lnop://0
			break;
		case lcar://1
			pila[++sp]=mem[imem++] ;
			break;
		case lasi://2
			mem[ pila[sp-1] ] = pila[sp--] ;
			//printf("Cargado %i en %i\n" , pila[sp+1], pila[sp] ) ;
			break ;
		case lori://3
			pila[sp-1]|=pila[sp];
			sp--;
			break;
		case lxor://4
			pila[sp-1]^=pila[sp];
			sp--;
			break;
		case land://5
			pila[sp-1]&=pila[sp];
			sp--;
			break;
		case ligu://6
			pila[sp-1] = ( pila[sp-1] == pila[sp] ) ;
			sp--;
			break;
		case ldis://7
			pila[sp-1] = ( pila[sp-1] != pila[sp] ) ;
			sp--;
			break;
		case lmay://8
			pila[sp-1] = ( pila[sp-1] > pila[sp] ) ;
			sp--;
			break;
		case lmen://9
			pila[sp-1] = ( pila[sp-1] < pila[sp] ) ;
			sp--;
			break;
		case lmei://10
			pila[sp-1] = ( pila[sp-1] <= pila[sp] ) ;
			sp--;
			break;
		case lmai://11
			pila[sp-1] = ( pila[sp-1] >= pila[sp] ) ;
			sp--;
			break;
		case ladd://12
			pila[sp-1] += pila[sp] ;
			sp--;
			break;
		case lsub://13
			pila[sp-1] -= pila[sp] ;
			sp--;
			break;
		case lmul://14
			pila[sp-1] *= pila[sp] ;
			sp--;
			break;
		case ldiv://15
			if(pila[sp]==0) error(145); // división entre cero
			pila[sp-1] /= pila[sp] ;
			sp--;
			break;
		case lmod://16
			if(pila[sp]==0) error(145); // división entre cero
			pila[sp-1] %= pila[sp] ;
			sp--;
			break;
		case lneg://17
			pila[sp] = -pila[sp] ;
			break ;
		case lptr://18
			pila[sp] = mem[ pila[sp] ] ;
			break ;
		case lnot://19
			pila[sp]^=-1; 
			break ;
		case laid://20
			pila[sp]+= procs_s[num_proc].id ;
			break ;
		case lcid://21
			pila[++sp] = procs_s[num_proc].id ;
			break ;
		case lrng://22
			imem++;
			break ;
		case ljmp://23
			imem = mem[ imem ] ;
			//printf("Nueva posicion: %i\n", imem ) ;
			break ;
		case ljpf://24 
			if (!(pila[sp]&1))
				imem = mem[ imem ] ;
			sp--; 
			break ;
		case lfun://25 NO USADO
			break ;
		case lcal://26
			assert(num_proc_orden<153);
			devolver++ ;
			v1 = busca_proc_libre() ;
			procs_s[v1].imem = mem[imem++] ;
			temp = proceso_actual ;
			proceso_actual = lista_mete(v1) ;
			procs_s[num_proc].imem = imem ;
			proceso(v1) ;
			proceso_actual = temp ;
			num_proc = num ;
			imem = procs_s[num_proc].imem ;
			break ;
		case lret://27
			lista_quita(num_proc) ;
			//printf("Ret: %i\n" , num_proc) ;
			retcode = 1 ;
			break ;
		case lasp://28
			sp--;
			break;
		case lfrm://29
			procs_s[num_proc].imem = imem ;
			if ( first_loaded )
			{
				Call_Entrypoint(EDIV_first_load);
				first_loaded = 0 ;
			}
			//printf("Frame en: %i\n" , imem ) ;
			retcode = 1 ;
			break ;
		case lcbp://30
			mem[procs_s[num_proc].id+var(_res_parameters)]=mem[imem++];
			mem[procs_s[num_proc].id+var(_res_param_offset)]=sp-mem[procs_s[num_proc].id+var(_res_parameters)]+1;
			/**/procs_s[num_proc].num_params = mem[ imem-1 ] ;
			break;
		case lcpa://31 POR HACER #-#
			mem[pila[sp--]]=pila[mem[procs_s[num_proc].id+var(_res_param_offset)]++];
			break;
		case ltyp://32
			procs_s[num_proc].id = mem[2] + ( num_proc * iloc_len ) ; 
			if ( procs_s[num_proc].tipo != 0 )
				critical_error(3); // redefinición del tipo de proceso
			procs_s[num_proc].tipo = mem[ imem++ ] ;
			mem[ procs_s[num_proc].id + varindex[_res_process_type] ] = procs_s[num_proc].tipo ;
			inicio_privadas=mem[6];
			break ;
		case lpri://33 POR HACER
			memcpy(&mem[procs_s[num_proc].id+inicio_privadas],&mem[imem+1],(mem[imem]-imem-1)<<2);
			inicio_privadas+=(mem[imem]-imem-1); imem=mem[imem];
			break ;
		case lcse://34
			if ( pila[sp-1] == pila[sp] ) 
				imem++; 
			else 
				imem=mem[imem];
			sp--;
			break ;
		case lcsr://35
			if ( pila[sp-2] >= pila[sp-1] && pila[sp-2] <= pila[sp] )
				imem++ ;
			else 
				imem = mem[imem]; 
			sp-=2; 
			break ;
		case lshr://36
			pila[sp-1] >>= pila[sp] ; 
			sp-- ;
			break ;
		case lshl://37
			pila[sp-1] <<= pila[sp] ; 
			sp-- ;
			break ;
		case lipt://38
			pila[sp] = ++mem[ pila[sp] ] ;
			break ;
		case lpti://39 COMPORTAMIENTO EXTRAÑO SI SE PONE EN UNA LINEA
			mem[ pila[sp] ]++;
			pila[sp] = mem[ pila[sp] ]-1 ;
			break ;
		case ldpt://40
			pila[sp] = --mem[ pila[sp] ] ;
			break ;
		case lptd://41 COMPORTAMIENTO EXTRAÑO SI SE PONE EN UNA LINEA
			mem[ pila[sp] ]--;
			pila[sp] = mem[ pila[sp] ]+1 ;
			break ;
		case lada://42
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] += pila[sp] ;
			sp--;
			break;
		case lsua://43
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] -= pila[sp] ;
			sp--;
			break;
		case lmua://44
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] *= pila[sp] ;
			sp--;
			break;
		case ldia://45
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] /= pila[sp] ;
			sp--;
			break;
		case lmoa://46
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] %= pila[sp] ;
			sp--;
			break;
		case lana://47
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] &= pila[sp] ;
			sp--;
			break;
		case lora://48
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] |= pila[sp] ;
			sp--;
			break;
		case lxoa://49
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] ^= pila[sp] ;
			sp--;
			break;
		case lsra://50
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] >>= pila[sp] ;
			sp--;
			break;
		case lsla://51
			pila[ sp-1 ] = mem[ pila[ sp-1 ] ] <<= pila[sp] ;
			sp--;
			break;
		case lpar://52 POR HACER
			//imem++;
			inicio_privadas+=mem[imem++];
			break;
		case lrtf://53
			lista_quita(num_proc) ;
			no_devuelve = 1 ;
			//printf("Return: %i\n" , num_proc) ;
			retcode = 1 ;
			break ;
		case lclo://54 POR HACER
			break ;
		case lfrf://55 POR HACER
			sp--;
			break ;
		case limp://56
			carga_dll((char*)&mem[mem[imem++]]);
			break ;
		case lext://57
			//assert(0);
			externa=extfuncs[mem[imem++]];
			// corresponder con FUNCTION_PARAMS
			//temp = externa(pila,&sp,mem,varindex,&procs_s,Call_Entrypoint);
			temp = externa(&fp) ;
			pila[++sp]= temp ;
			break ;
		case ldbg://58
			#ifdef DBG
				//Call_Entrypoint(EDIV_debug,imem,nombreprg,lin,0);
			#endif
			break ;
		}
		#ifdef DBG
			//Call_Entrypoint(EDIV_trace,imem,nombreprg,lin,0);
		#endif
	}

	if ( devolver > 0 && no_devuelve == 0 )
	{
		pila[++sp] = 0 ;// AQUI SE DEBERA DEVOLVER EL ID
		devolver-- ;
	}

	Call_Entrypoint(EDIV_post_process);

	return 1 ;
}



