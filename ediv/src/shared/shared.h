/*
 * eDiv Compiler
 * Copyright (C) 2000-2002 Sion, Ltd.
 * http://www.sionhq.com
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
 *	Definiciones compartidas de eDiv - Usar en eDiv, encrypt y stub :)
 */

#ifndef __EDIV_SHARED_H_
#define __EDIV_SHARED_H_

#define VERSION "0.1.3b (Rigel)" /* Version y codename */
#define COPYRIGHT "Copyright (C) 2000-2003 Sion Ltd. Copyright (C) 2005 eDIV Team"
#define WEBSITE "http://ediv.divsite.net"

#define la_clave "81ab50e9c20d"    /* Clave de cifrado   */

#define uchar char
#define byte unsigned char
#define ushort unsigned short
#define word unsigned short
#define ulong unsigned int
#define dword unsigned int

#define magic "eDIVstub0\x1a\x0d\x0a\xff"

#define long_header 9 /* Longitud de la cabecera al inicio de los programas */

#endif /* __EDIV_SHARED_H_ */