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

#ifdef _WINBASE_
#	undef lstrcpy
#	undef lstrcat
#	undef lstrlen
#endif

#define long_header 9   //Longitud de la cabecera al inicio de los programas

#define default_buffer    (16384*8)
#define security_distance (4096*8)
#define buffer_grow       (16384*8)

int * mem_ory, * frm;
int * mem, * loc;
int imem, imem_max;
int iloc, iloc_max;
int itxt, ifrm_max;

byte * imemptr;

int iloc_len; // final de las variables locales incluyendo las privadas

