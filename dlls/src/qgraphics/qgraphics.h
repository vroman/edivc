#ifndef __EDIV_QGRAPHICS_H_
#define __EDIV_QGRAPHICS_H_

#include "export.h"

/* Flags de modos de video */
#define _FULLSCREEN		0x01

typedef struct {
	byte* buffer;                // invisible buffer
	byte* colormap;              // 256 * VID_GRADES size
	byte* alphamap;              // 256 * 256 translucency map
	int	width;          
	int	height;
	int bpp;
	int flags;
} viddef_t;

viddef_t vid;

#endif /* __EDIV_QGRAPHICS_H_ */