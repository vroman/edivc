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

#ifndef __FLIC_H
#define __FLIC_H

#include <SDL/SDL_types.h>

#ifndef __GNUC__
#define __PACKED
#define inline __inline
#else
#define __PACKED __attribute__ ((packed))
#endif
/* Reproducción de FLI */
/* ------------------- */

typedef struct {
  Uint32  size;          /* Size of FLIC including this header */
  Uint16  type;          /* File type 0xAF11, 0xAF12, 0xAF30, 0xAF44, ... */
  Uint16  frames;        /* Number of frames in first segment */
  Uint16  width;         /* FLIC width in pixels */
  Uint16  height;        /* FLIC height in pixels */
  Uint16  depth;         /* Bits per pixel (usually 8) */
  Uint16  flags;         /* Set to zero or to three */
  Uint32  speed;         /* Delay between frames */
  Uint16  reserved1;     /* Set to zero */
  Uint32  created;       /* Date of FLIC creation (FLC only) */
  Uint32  creator;       /* Serial number or compiler id (FLC only) */
  Uint32  updated;       /* Date of FLIC update (FLC only) */
  Uint32  updater;       /* Serial number (FLC only), see creator */
  Uint16  aspect_dx;     /* Width of square rectangle (FLC only) */
  Uint16  aspect_dy;     /* Height of square rectangle (FLC only) */
  Uint16  ext_flags;     /* EGI: flags for specific EGI extensions */
  Uint16  keyframes;     /* EGI: key-image frequency */
  Uint16  totalframes;   /* EGI: total number of frames (segments) */
  Uint32  req_memory;    /* EGI: maximum chunk size (uncompressed) */
  Uint16  max_regions;   /* EGI: max. number of regions in a CHK_REGION chunk */
  Uint16  transp_num;    /* EGI: number of transparent levels */
  Uint8   reserved2[24]; /* Set to zero */
  Uint32  oframe1;       /* Offset to frame 1 (FLC only) */
  Uint32  oframe2;       /* Offset to frame 2 (FLC only) */
  Uint8   reserved3[40]; /* Set to zero */
}  
__PACKED
FLIC_HEADER;

typedef union
{
	struct {
	  Uint32 size;           /* Size of the chunk */
	  Uint16 type;           /* Chunk type */
	}
	header ;

	struct {
	  Uint32 size;           /* Size of the chunk */
	  Uint16 type;           /* Chunk type: 12 */
	  Uint16 first_line;     /* First line */
	  Uint16 line_count;     /* Number of lines in the chunk */
	  Uint8  data[0];
	}
	delta_fli ;

	struct {
	  Uint32 size;           /* Size of the chunk  */
	  Uint16 type;           /* Chunk type: 15, 12, etc. */
	  Uint8  data[0];
	}
	raw ;
}
FLIC_CHUNK ;

typedef struct
{
  Uint32 size;		/* Size of the frame, including subchunks */
  Uint16 type;		/* Chunk type: 0xF1FA */
  Uint16 chunks;	/* Number of subchunks */
  Uint8  expand[8];
}
FLIC_FRAME ;

typedef struct
{
	FLIC_HEADER	header ;
	FLIC_CHUNK	* chunk ;
	FLIC_FRAME	* frame ;
	Uint32		frame_reserved ;

	GRAPH          * bitmap ;

	file		* fp ;

	int		current_frame ;
	int		speed_ms ;
	int		last_frame_ms ;
	int		finished ;
}
FLIC ;

/* Tipos de chunk */

#define	CHUNK_COLOR_256		4
#define	CHUNK_COLOR_64 		11
#define CHUNK_DELTA_FLI		12
#define CHUNK_DELTA_FLC		7
#define CHUNK_BLACK		13
#define CHUNK_BYTE_RUN		15
#define CHUNK_FLI_COPY		16
#define CHUNK_STAMP   		18
#define CHUNK_PREFIX		0xF100
#define CHUNK_FRAME 		0xF1FA

extern FLIC * flic_open     (const char * filename) ;
extern void   flic_destroy  (FLIC * flic) ;
extern FLIC * flic_do_frame (FLIC * flic) ;
extern void   flic_reset    (FLIC * flic) ;

extern FLIC * current_fli ;
extern int    current_fli_x ;
extern int    current_fli_y ;

#endif
