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

#ifndef __VARINDEX_H
#define __VARINDEX_H

typedef enum { global, reserved, local } tipo_t;

/// PROTOTIPOS ///

// compilador
void inicializa_index();
void indexa_variable(tipo_t tipo, char* nombre, int ptr);
void get_varptr(int** ptr, int* nptr);


int num_indexed_vars;
int *varindex;

#define var(a)		mem[varindex[a]]

/*
//////////////////////////////
///        GLOBALES        ///
//////////////////////////////

#define _glo_timer				0
#define _glo_max_process_time	1
#define _glo_argc				2
#define _glo_argv				3


//////////////////////////////
///        LOCALES         ///
//////////////////////////////

// Estructura reserved

#define _res_process_id			4
#define _res_id_scan			5
#define _res_process_type		6
#define _res_type_scan			7
#define _res_status				8
#define _res_parameters			9
#define _res_param_offset		10
#define _res_program_index		11
#define _res_stack_pointer		12
#define _res_is_executed		13
#define _res_is_painted			14
#define _res_m8_object			15
#define _res_old_ctype			16
#define _res_frame_percent		17
#define _res_box_x0				18
#define _res_box_y0				19
#define _res_box_x1				20
#define _res_box_y1				21
#define _res_f_count			22
#define _res_caller_id			23

// Jerarquía de procesos
#define _loc_father				24
#define _loc_son				25
#define _loc_smallbro			26
#define _loc_bigbro				27
	
// Variables locales varias
#define _loc_priority			28
#define _loc_ctype				29
#define _loc_x					30
#define _loc_y					31
#define _loc_z					32
#define _loc_graph				33
#define _loc_size				34
#define _loc_angle				35
#define _loc_region				36
#define _loc_file				37
#define _loc_xgraph				38
#define _loc_height				39
#define _loc_cnumber			40
#define _loc_resolution			41
#define _loc_flags				42

#define _glo_draw_z				43
#define _glo_scroll				44
#define _glo_fps				45
*/

enum {

//////////////////////////////
///        GLOBALES        ///
//////////////////////////////

	_glo_timer,
	_glo_max_process_time,
	_glo_argc,
	_glo_argv,
	_glo_draw_z,
	_glo_scroll,
	_glo_fps,


//////////////////////////////
///        LOCALES         ///
//////////////////////////////

// Estructura reserved

	_res_process_id,
	_res_id_scan,
	_res_process_type,
	_res_type_scan,
	_res_status,
	_res_parameters,
	_res_param_offset,
	_res_program_index,
	_res_stack_pointer,
	_res_is_executed,
	_res_is_painted,
	_res_m8_object,
	_res_old_ctype,
	_res_frame_percent,
	_res_box_x0,
	_res_box_y0,
	_res_box_x1,
	_res_box_y1,
	_res_f_count,
	_res_caller_id,

// Jerarquía de procesos
	_loc_father,
	_loc_son,
	_loc_smallbro,
	_loc_bigbro,
	
// Variables locales varias
	_loc_priority,
	_loc_ctype,
	_loc_x,
	_loc_y,
	_loc_z,
	_loc_graph,
	_loc_size,
	_loc_angle,
	_loc_region,
	_loc_file,
	_loc_xgraph,
	_loc_height,
	_loc_cnumber,
	_loc_resolution,
	_loc_flags,
	_loc_transparency,


	___last		// por si acaso

};

#endif // __VARINDEX_H