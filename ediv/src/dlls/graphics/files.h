/* Fenix - Compilador/intérprete de videojuegos
 * Copyright (C) 1999 José Luis Cebrián Pagüe
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

#ifndef __FILES_H
#define __FILES_H

/* Funciones de acceso a ficheros */
/* ------------------------------ */

	/* Ahora mismo son casi wrappers de stdio.h, pero en el futuro
	 * el tipo "file" puede ser una estructura y las funciones,
	 * ofrecer soporte transparente para ficheros PAK, etc. */

#define F_XFILE  1
#define F_FILE	 2
#define F_GZFILE 3

#include <zlib.h>

typedef struct
{
	int	type ;
	FILE *  fp ;
	gzFile  gz ;
	int     n ;
        int     error ;
}
file ;

extern file * file_open  (const char * filename, char * mode) ;
extern int    file_read  (file * fp, void * buffer, int len) ;
extern int    file_write (file * fp, void * buffer, int len) ;
extern int    file_gets  (file * fp, char * buffer, int len) ;
extern int    file_puts  (file * fp, const char * buffer) ;
extern int    file_size  (file * fp) ;
extern int    file_pos   (file * fp) ;
extern int    file_seek  (file * fp, int pos, int where) ;
extern void   file_addp  (const char * path) ;
extern void   file_close (file * fp) ;
extern int    file_exists (char * filename) ;
extern void   file_add_xfile (file * fp, long offset, char * name, int size) ;
extern int    file_eof   (file * fp) ;
extern FILE * file_fp    (file * fp) ;

#endif
