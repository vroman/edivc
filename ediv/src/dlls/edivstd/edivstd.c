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

/* EDIVSTD.DLL
 * Esta librería contiene los datos más básicos que puede requerir un programa
 * DIV, tal como las opciones de compilación, estructura reserved, etc.
 */
 
#include <export.h>
#include "../../shared/varindex.h"

#include "main.h"

int last_type ;
int last_proc ;

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	// CONSTANTES PREDEFINIDAS
	
	// Opciones de compilación
	CONST("_max_process",0);
	CONST("_extended_conditions",1);
	CONST("_simple_conditions",2);
	CONST("_case_sensitive",3);
	CONST("_ignore_errors",4);
	CONST("_free_sintax",5);
	CONST("_no_check",6);
	CONST("_no_strfix",7);
	CONST("_no_optimization",8);
	CONST("_no_range_check",9);
	CONST("_no_id_check",10);
	CONST("_no_null_check",11);

	// Indicadores de ventanas (cnumber)
	CONST("c_0",1);
	CONST("c_1",2);
	CONST("c_2",4);
	CONST("c_3",8);
	CONST("c_4",16);
	CONST("c_5",32);
	CONST("c_6",64);
	CONST("c_7",128);
	CONST("c_8",256);
	CONST("c_9",512);
	
	// True y false
	CONST("true",1);
	CONST("false",0);
	
	// Rango de INT
	CONST("max_int",2147483647);
	CONST("min_int",-2147483648);
	
	// Señales entre procesos
	CONST("s_kill",0);
	CONST("s_wakeup",1);
	CONST("s_sleep",2);
	CONST("s_freeze",3);
	CONST("s_kill_tree",100);
	CONST("s_wakeup_tree",101);
	CONST("s_sleep_tree",102);
	CONST("s_freeze_tree",103);
	
	
	// DATOS GLOBALES PREDEFINIDOS
	
	// Contadores de tiempo
	GLOBAL_ARRAY("timer",9);
	
	// Tiempo máximo de ejecución para un proceso
	GLOBAL("max_process_time",500);
	
	// Argumentos de línea de comando
	GLOBAL("argc",0);
	GLOBAL_ARRAY("argv",9);
	
	
	// DATOS LOCALES PREDEFINIDOS
	
	// Estructura reserved - POR HACER: ¿preservar compatibilidad con DIV?
	LOCAL_STRUCT("reserved",0);
		_INT("process_id",0);
		_INT("id_scan",0);
		_INT("process_type",0);		// usado por VisualDIV ;)
		_INT("type_scan",0);
		_INT("status",2);
		_INT("parameters",0);
		_INT("param_offset",0);
		_INT("program_index",0);
		_INT("stack_pointer",0);
		_INT("is_executed",0);
		_INT("is_painted",0);
		_INT("m8_object",-1);		// usado por VPE en DIV2
		_INT("old_ctype",0);
		_INT("frame_percent",0);
		_INT("box_x0",0);
		_INT("box_y0",0);
		_INT("box_x1",0);
		_INT("box_y1",0);
		_INT("f_count",0);
		_INT("caller_id",0);
	END_STRUCT;
	
	// Jerarquía de procesos
	LOCAL("father",0);
	LOCAL("son",0);
	LOCAL("smallbro",0);
	LOCAL("bigbro",0);
	
	// Variables locales varias
	LOCAL("priority",0);
	LOCAL("ctype",0);
	LOCAL("x",0);
	LOCAL("y",0);
	LOCAL("z",0);
	LOCAL("graph",0);
	LOCAL("size",0);
	LOCAL("angle",0);
	LOCAL("region",0);
	LOCAL("file",0);
	LOCAL("xgraph",0);
	LOCAL("height",1);
	LOCAL("cnumber",0);
	LOCAL("resolution",0);
	LOCAL("flags",0);
	LOCAL("transparency",128) ;

	FUNCTION("get_id",1,eDiv_GetId) ;
	FUNCTION("define_region",5,eDiv_DefineRegion) ;

	ENTRYPOINT( first_load ) ;

	// POR HACER: funciones signal, get_id, system, exit, ignore_error...
	
	return TRUE;
}

int eDiv_GetId(FUNCTION_PARAMS)
{
	int i ,id1 ;
	int a = getparm() ;
	if ( last_type == a )
		i = last_proc+1 ;
	else
		i = 0 ;
	last_type = a ;
	for ( ; i < *fp->num_procs ; i++ )
	{
		last_proc = i ;
		id1 = fp->procs_s[ fp->proc_orden[ i ] ].id ;
		//Si el proceso se corresponde con el type
		if ( fp->mem[ id1 + fp->varindex[ _res_process_type ] ] == a )
		{
			return ( fp->procs_s[ fp->proc_orden[ i ] ].id ) ;
		}
	}
	last_proc = -1 ;
	return 0 ;
}

int eDiv_DefineRegion(FUNCTION_PARAMS)
{
	int n , x , y , w , h ;
	h = getparm() ;
	w = getparm() ;
	y = getparm() ;
	x = getparm() ;
	n = getparm() ;

	if ( n > 31 || n < 1 )
		return -1 ;

	regions[n].x = x ;
	regions[n].y = y ;
	regions[n].w = w ;
	regions[n].h = h ;


	return 1 ;
}




//---------------------------- ENTRYPOINTS -----------------------------------------------



void first_load(FUNCTION_PARAMS)
{
	int i ;

	for ( i = 0 ; i < 32 ; i++ )
	{
		regions[i].x = 0 ;
		regions[i].y = 0 ;
		regions[i].w = 0 ;
		regions[i].h = 0 ;
	}

	
	fp->regions = regions ;
	fp->existe.regions = 1 ;

	
}


