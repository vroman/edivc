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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "files.h"

char * possible_paths[32] =
{
	"",
	0
} ;

#define MAX_X_FILES 256

typedef struct
{
	char name[40] ;
	int  offset ;
	long pos ;
	int  size ;
	int  eof ;
	FILE * fp ;
}
XFILE ;

XFILE x_file[MAX_X_FILES] ;

int x_files_count = 0 ;

/* Añade un nuevo archivo al PATH */

void file_add_xfile (file * fp, long offset, char * name, int size)
{
	assert (x_files_count < MAX_X_FILES) ;
	assert (fp->type == F_FILE) ;

	x_file[x_files_count].fp = fp->fp ;
	x_file[x_files_count].offset = offset ;
	x_file[x_files_count].size = size ;
	x_file[x_files_count].pos = offset ;
	x_file[x_files_count].eof = 0 ;
	strncpy (x_file[x_files_count].name, name, 39) ;
	x_file[x_files_count].name[39] = 0 ;

	x_files_count++ ;
}

/* Lee un bloque de datos del fichero */


int file_read (file * fp, void * buffer, int len)
{
	assert (len != 0);

	if (fp->type == F_XFILE)
	{
		XFILE * xf ;
		int result ;

		xf = &x_file[fp->n] ;

		if (len + xf->pos > xf->offset + xf->size) 
		{
			xf->eof = 1 ;
			len = xf->size + xf->offset - xf->pos ;
		}
		fseek (xf->fp, xf->pos, SEEK_SET) ;
		result = fread (buffer, 1, len, xf->fp) ;
		xf->pos = ftell (xf->fp) ;
		return result ;
	}
	if (fp->type == F_GZFILE)
	{
		int result = gzread (fp->gz, buffer, len) ;
		if ((fp->error = result < 0) != 0) 
                        result = 0 ;
		return result ;
	}

	return fread (buffer, 1, len, fp->fp) ;
}

/* Guarda una cadena "cuoteada" al disco */

int file_puts (file * fp, const char * buffer)
{
	char dest[1024], * optr ;
	const char * ptr ;

	ptr = buffer ;
	optr = dest ;
	while (*ptr)
	{
		if (optr > dest+1000)
		{
			*optr++ = '\\' ;
			*optr++ = '\n' ;
			*optr   = 0 ;
			file_write (fp, dest, optr-dest) ;
			optr = dest ;
		}
		if (*ptr == '\n')
		{
			*optr++ = '\\' ;
			*optr++ = 'n' ;
			ptr++ ;
			continue ;
		}
		if (*ptr == '\\')
		{
			*optr++ = '\\' ;
			*optr++ = *ptr++ ;
			continue ;
		}
		*optr++ = *ptr++ ;
	}
	*optr++ = '\n' ;
	return file_write (fp, dest, optr-dest) ;
}

/* Recupera una cadena de un fichero y la "descuotea" */

int file_gets (file * fp, char * buffer, int len)
{
	char * ptr, * result ;

	if (fp->type == F_XFILE)
	{
		XFILE * xf ;
		int l = 0, result = 0 ;
                char * ptr = buffer ;

		xf = &x_file[fp->n] ;

		fseek (xf->fp, xf->pos, SEEK_SET) ;
                while (l < len)
                {
                        if (xf->pos >= xf->offset + xf->size)
                        {
                                xf->eof = 1 ;
                                break ;
                        }
                        fread (ptr, 1, 1, xf->fp) ;
                        result++ ;
                        xf->pos++ ;
                        if (*ptr++ == '\n') 
                                break ;
                }
                *ptr = 0 ;
                xf->pos = ftell(xf->fp) ;
	}
	else if (fp->type == F_GZFILE)
	{
		result = gzgets (fp->gz, buffer, len) ;
	}
	else 
	{
		result = fgets(buffer, len, fp->fp);
	}

	if (result == 0) { buffer[0] = 0 ; return 0 ; }

	ptr = buffer ;
	while (*ptr)
	{
		if (*ptr == '\\')
		{
			if (ptr[1] == 'n') ptr[1] = '\n' ;
			strcpy (ptr, ptr+1) ;
			ptr++ ;
			continue ;
		}
		if (*ptr == '\n')
		{
			*ptr = 0 ;
			break ;
		}
		ptr++ ;
	}
	return strlen(buffer) ;
}

/* Escribe un bloque de datos en el fichero */

int file_write (file * fp, void * buffer, int len)
{
	if (fp->type == F_XFILE)
	{
		XFILE * xf ;
		int result ;

		xf = &x_file[fp->n] ;

		if (len + xf->pos > xf->offset + xf->size) 
		{
			xf->eof = 1 ;
			len = xf->size + xf->offset - xf->pos ;
		}
		fseek (xf->fp, xf->pos, SEEK_SET) ;
		result = fwrite (buffer, 1, len, xf->fp) ;
		xf->pos = ftell (xf->fp) ;
		return result ;
	}
	if (fp->type == F_GZFILE)
	{
		int result = gzwrite (fp->gz, buffer, len) ;
		if ((fp->error = result < 0) != 0) 
                        result = 0 ;
		return result < len ? 0 : 1 ;
	}


	return fwrite (buffer, 1, len, fp->fp) ;
}

/* Devuelve el tamaño de un fichero */

int file_size (file * fp)
{
	long pos, size ;

	if (fp->type == F_XFILE)
		return x_file[fp->n].size ;
	if (fp->type == F_GZFILE)
	{
		fprintf (stderr, "file_size: inválida en ficheros comprimidos\n") ;
		return 0 ;
	}

	pos = ftell(fp->fp) ;
	fseek (fp->fp, 0, SEEK_END) ;
	size = ftell(fp->fp) ;
	fseek (fp->fp, pos, SEEK_SET) ;
	return size ;
}

/* Devuelve la posición actual de un fichero */

int file_pos (file * fp)
{
	if (fp->type == F_XFILE)
		return x_file[fp->n].pos - x_file[fp->n].offset ;
	if (fp->type == F_GZFILE)
		return gztell(fp->gz) ;
	
	return ftell (fp->fp) ;
}

/* Posiciona el puntero de lectura/escritura dentro de un fichero */

int file_seek (file * fp, int pos, int where)
{
	assert(fp);
	if (fp->type == F_XFILE)
	{
		if (where == SEEK_END)
			pos = x_file[fp->n].size - pos + 1 ;
		else if (where == SEEK_CUR)
			pos += x_file[fp->n].pos - x_file[fp->n].offset ;

		if (x_file[fp->n].size < pos)
			pos = x_file[fp->n].size ;
		if (pos < 0) pos = 0 ;

		x_file[fp->n].pos = pos + x_file[fp->n].offset ;
		return x_file[fp->n].pos - x_file[fp->n].offset ;
	}
	if (fp->type == F_GZFILE)
	{
		assert(fp->gz);
		return gzseek (fp->gz, pos, where) ;
	}

	assert(fp->fp);
	return fseek (fp->fp, pos, where) ;
}

/* Abre un fichero */

static int open_raw (file * f, const char * filename, const char * mode)
{
	if (!strchr(mode,'0'))
	{
		f->type = F_GZFILE ;
		f->gz = gzopen (filename, mode) ;
		if (f->gz) return 1 ;
	}
	f->type = F_FILE ;
	f->fp = fopen (filename, mode) ;
	if (f->fp) return 1 ;
	return 0 ;
}

file * file_open (const char * filename, char * mode)
{
	char name [256] ;
	char path [256] ;
	char here [256] ;

	const char * c, * n ;
	int i ;
#ifdef TARGET_linux
	int j ;
#endif

	file * f ;

	f = (file *) malloc(sizeof(file)) ;
	assert (f) ;
	memset (f, 0, sizeof(f)) ;

	c = filename ;
	for (n = c+strlen(c) ; n >= c ; n--)
	{
		if (*n == '/' || *n == '\\')
		{
			c = n+1 ;
			break ;
		}
	}
	strcpy (name, c) ;
	strcpy (path, filename) ;
	path[c-filename] = 0 ;
#ifdef TARGET_linux
	for (j = 0 ; path[j] ; j++)
		if (path[j] == '\\') path[j] = '/' ;
#endif

	if (strchr(mode,'r') && strchr(mode,'b'))
	for (i = 0 ; i < x_files_count ; i++)
	{
		if (strcmp(name, x_file[i].name) == 0)
		{
			x_file[i].pos = x_file[i].offset ;
			f->type = F_XFILE ;
			f->n = i ;
			return f ;
		}
	}

	strcpy (here, path) ;
	strcat (here, name) ;
	if (open_raw (f, here, mode)) return f ;

	/* Busca por el directorio de la extensión (directorio FPG para FPG) */
	if (strchr(name,'.'))
	{
		strcpy (here, strchr(name,'.') + 1) ;
#ifdef TARGET_linux
		strcat (here, "/") ;
#else
		strcat (here, "\\") ;
#endif
		strcat (here, path) ;
		strcat (here, name) ;
		if (open_raw (f, here, mode)) return f ;
	}

	for (i = 0 ; possible_paths[i] ; i++)
	{
		strcpy (here, possible_paths[i]) ;
		strcat (here, name) ;
#ifdef TARGET_linux
		for (j = 0 ; here[j] ; j++)
			if (here[j] == '\\') here[j] = '/' ;
#endif
		if (open_raw (f, here, mode)) return f ;
	}

	free (f) ;
	return 0 ;
}

/* Cierra un fichero */

void file_close (file * fp)
{
	if (fp->type == F_XFILE)
                x_file[fp->n].pos = x_file[fp->n].offset ;
	if (fp->type == F_FILE)
		fclose(fp->fp) ;
	if (fp->type == F_GZFILE)
		gzclose(fp->gz) ;
	free(fp) ;
}

/* Añade un nuevo directorio al PATH */

void file_addp  (const char * path)
{
	char truepath[256];
	int n ;

	strcpy (truepath, path) ;
	for (n = 0 ; truepath[n] ; n++)
		if (truepath[n] == '/')
			truepath[n] = '\\' ;
	if (truepath[strlen(truepath)-1] != '\\') 
		strcat (truepath, "\\") ;

	for (n = 0 ; n < 31 && possible_paths[n] ; n++) ;

	//possible_paths[n] = strdup (truepath) ;
	possible_paths[n+1] = 0 ;
}

/* Devuelve cierto si existe el fichero */

int file_exists (char * filename)
{
	file * fp ;

	fp = file_open (filename, "rb") ;
	if (fp)
	{
		file_close (fp) ;
		return 1 ;
	}
	return 0 ;
}

/* Devuelve cierto si se leyó más allá del fin del fichero */

int file_eof (file * fp)
{
	if (fp->type == F_XFILE)
	{
		XFILE * f = &x_file[fp->n] ;
		return f->eof ;
	}
	if (fp->type == F_GZFILE)
        {
                if (fp->error) return 1 ;
		return gzeof(fp->gz) ;
        }

	return feof(fp->fp) ;
}

/* Devuelve el FILE * correspondiente al fichero */

FILE * file_fp (file * f)
{
	if (f->type == F_XFILE)
	{
		XFILE * xf = &x_file[f->n] ;
		fseek (xf->fp, xf->pos, SEEK_SET) ;
		return xf->fp ;
	}
	return f->fp ;
}
