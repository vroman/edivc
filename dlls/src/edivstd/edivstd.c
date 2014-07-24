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
 *EDIVSTD.DLL
 * Esta librería contiene los datos más básicos que puede requerir un programa
 * DIV, tal como las opciones de compilación, estructura reserved, etc.
 */
 
 #include <time.h>
 #include <sys/timeb.h>
 unsigned int tiempo;
 unsigned int ultimo_tiempo;
#include <limits.h>
#include <assert.h>

#include <stdio.h>

#include "export.h"
#include "edivstd.h"

int last_type ;
int last_proc ;

#ifdef _DEBUG
int DEBUG_PROCESOS(FUNCTION_PARAMS)
{
	int i,n;
	FILE* f;
	f=fopen("procs.txt","a");
	fprintf(f,"\n--- COMIENZO ---\n");
	for(i=0;i<*fp->num_procs;i++) {
		n=fp->procs_s[fp->proc_orden[i]].id;
		fprintf(f,"(%d) ID: %d  TYPE: %d  TIPO: %d  STATUS: %d  ORDEN: %d\n",i,n,reserved("process_type",n),fp->procs_s[fp->proc_orden[i]].tipo,reserved("status",n),fp->procs_s[fp->proc_orden[i]].orden);
	}
	fprintf(f,"\n--- FIN ---\n");
	fclose(f);
	return 0;
}
#endif

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	/* CONSTANTES PREDEFINIDAS */
	
	/* Opciones de compilación */
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

	/* Indicadores de ventanas (cnumber) */
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
	
	/* True y false */
	CONST("true",1);
	CONST("false",0);
	
	/* Rango de INT */
	CONST("max_int",INT_MAX);
	CONST("min_int",INT_MIN);
	
	/* Señales entre procesos */
	CONST("s_kill",0);
	CONST("s_wakeup",1);
	CONST("s_sleep",2);
	CONST("s_freeze",3);
	CONST("s_kill_tree",100);
	CONST("s_wakeup_tree",101);
	CONST("s_sleep_tree",102);
	CONST("s_freeze_tree",103);
	
	
	/* DATOS GLOBALES PREDEFINIDOS */
	
	/* Contadores de tiempo */
	GLOBAL_ARRAY("timer",9);
	
	/* Tiempo máximo de ejecución para un proceso */
	GLOBAL("max_process_time",500);
	
	/* Argumentos de línea de comando */
	GLOBAL("argc",0);
	GLOBAL_ARRAY("argv",9);
	
	
	/* DATOS LOCALES PREDEFINIDOS */
	
	/* Estructura reserved - POR HACER: ¿preservar compatibilidad con DIV? -> si
	 * pero ¿Qué hacer con distance_1 y distance_2 (sinónimos de m8_object y old_ctype,
	 * respectivamente)? ya nos toparemos con ellas cuando hagamos el modo7. 
	 */

	LOCAL_STRUCT("reserved",0);
		_INT("process_id",0);
		_INT("id_scan",0);
		_INT("process_type",0);		/* usado por VisualDIV ;) */
		_INT("type_scan",0);
		_INT("status",2);
		_INT("parameters",0);
		_INT("param_offset",0);
		_INT("program_index",0);
		_INT("stack_pointer",0);
		_INT("is_executed",0);
		_INT("is_painted",0);
		_INT("m8_object",-1);		/* usado por VPE en DIV2 */
		_INT("old_ctype",0);
		_INT("frame_percent",0);
		_INT("box_x0",0);
		_INT("box_y0",0);
		_INT("box_x1",0);
		_INT("box_y1",0);
		_INT("f_count",0);
		_INT("caller_id",0);
	END_STRUCT;
	
	/* Jerarquía de procesos */
	LOCAL("father",0);
	LOCAL("son",0);
	LOCAL("smallbro",0);
	LOCAL("bigbro",0);
	
	/* Variables locales varias */
	LOCAL("priority",0);
	LOCAL("ctype",0);
	LOCAL("height",1);
	LOCAL("cnumber",0);

	FUNCTION("exit",2,eDIV_Exit);
	FUNCTION("get_id",1,eDiv_GetId) ;
	FUNCTION("signal",2,eDIV_Signal);
	FUNCTION("let_me_alone",0,eDIV_Let_Me_Alone);

#ifdef _DEBUG
	FUNCTION("debug_procesos",0,DEBUG_PROCESOS);
#endif

	ENTRYPOINT(first_load) ;
	ENTRYPOINT(frame);

	/* 
	 * POR HACER: funciones signal, system, ignore_error... 
	 */
	
	return TRUE;
}

int eDIV_Exit(FUNCTION_PARAMS)
{
	int codigo=getparm();
	char* mensaje=getstrparm();
	
	/* 
	 * FIXME: ¿Qué hacemos con el mensaje? ¿Lo mostramos en un msgbox si no es ""? 
	 */
	if(*mensaje) {
		#ifdef _WIN32
			MessageBox(0,mensaje,fp->nombre_program,0);
		#else
			fprintf(stdout,"%s\n",mensaje);
		#endif
	}
	#ifdef _DEBUG
		printf("dbg:\texit(): código de retorno: %d\n",codigo);
	#endif
	fp->Stub_Quit(codigo);

	/* Por si acaso :P */
	return codigo;
}

int eDiv_GetId(FUNCTION_PARAMS)
{
	int i ,id1 ;
	int a = getparm() ;
	int* last_type=&reserved("type_scan",fp->procs_s[fp->proc_orden[*fp->proceso_actual]].id);
	int* last_proc=&reserved("id_scan",fp->procs_s[fp->proc_orden[*fp->proceso_actual]].id);

	if ( *last_type == a )
		i = *last_proc+1 ;
	else {
		i = 0 ;
		*last_type = a ;
	}
	for ( ; i < *fp->num_procs ; i++ )
	{
		*last_proc = i ;
		id1 = fp->procs_s[ fp->proc_orden[ i ] ].id ;
		/* Si el proceso se corresponde con el type */
		if ( reserved("process_type",id1) == a )
		{
			return ( fp->procs_s[ fp->proc_orden[ i ] ].id ) ;
		}
	} 
	*last_type = 0 ;
	return 0 ;
}

void signal_tree(int proc, int signal, FUNCTION_PARAMS)
{
	int id2;
	reserved("status",proc)=signal;
	if(id2=local("son",proc)) {
		signal_tree(id2,signal,fp);
		//reserved("status",id2)=signal;
	}
	while(local("bigbro",id2)) {
		proc=local("bigbro",id2);
		reserved("status",id2)=signal;
	}
}

int eDIV_Signal(FUNCTION_PARAMS)
{
	int signal=getparm();
	int proc=getparm();
	char tree=FALSE;
	
//	assert(0);
	if(signal>=100) {
		signal-=100;
		tree=TRUE;
	}

	signal++;
	/* Si se le pasa un ID */
	if(proc<fp->imem_max)
	{
		reserved("status",proc)=signal;
		if(tree && (proc=local("son",proc)))
			signal_tree(proc,signal,fp);
	}
	/* Si se le pasa un type */
	else {
		int i,p;
		for(i=0;i<*fp->num_procs;i++) {
			if(fp->procs_s[fp->proc_orden[i]].tipo==proc) {
				reserved("status",fp->procs_s[fp->proc_orden[i]].id)=signal;
				if(tree && (p=local("son",fp->procs_s[fp->proc_orden[i]].id)))
					signal_tree(fp->procs_s[fp->proc_orden[i]].id,signal,fp);
			}
		}
	}
	return 0;
}

int eDIV_Let_Me_Alone(FUNCTION_PARAMS)
{
	int i;
	int _status=reservedptr("status");

	for(i=0;i<*fp->num_procs;i++) {
		if(i!=*fp->proceso_actual) {
			fp->mem[fp->procs_s[fp->proc_orden[i]].id+_status]=1; /* s_kill */
		}
	}
	return 0;
}



/*
 * Entrypoints
 */
void first_load(FUNCTION_PARAMS)
{
	struct timeb tiempob;
	ftime(&tiempob);
	ultimo_tiempo=tiempob.time*100+tiempob.millitm/10;
}

void frame(FUNCTION_PARAMS)
{
	int i;
	int timer;
	struct timeb tiempob;
	ftime(&tiempob);
	tiempo=tiempob.time*100+tiempob.millitm/10;
	timer=globalptr("timer");
	for(i=0;i<10;i++)
		fp->mem[timer+i]+=tiempo-ultimo_tiempo;
	ultimo_tiempo=tiempo;
}



