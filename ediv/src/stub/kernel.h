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

#ifndef __KERNEL_H
#define __KERNEL_H

// CODIGOS DE ERROR DEL KERNEL
#define K_ERR_SDL_INIT					0
#define K_ERR_SDL_SET_VIDEO_MODE		1
#define K_ERR_KERNEL					2
#define K_ERR_INTERPRETE				3
#define K_ERR_INI_INTERPRETE			4

// NIVEL CRITICO DE ERROR DEL KERNEL
#define K_ERROR_EXIT	500

struct _k_error_s {
	char cadena[64];
	int nivel;
};

struct _lista_draw_proc_s{
	SDL_Surface *imagen;
	int x,y;
} lista_draw_proc_s[512];


int num_draw_proc;

struct _textos_s {
	SDL_Surface *cadena;
	int x,y;
	int existe;
} textos_s[256];


int ini_kernel();
int kernel();
void k_error(int codigo);

#endif /* __KERNEL_H */