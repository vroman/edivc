/* Copyright (C) 1999 José Luis Cebrián Pagüe
 * Copyright (C) 2000-2002 Sion, Ltd
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

#include <memory.h>

#include "main.h"
#include "lower.h"

/* Tabla de conversión de caracteres MS-DOS a Windows */

int dos_chars=0;

byte dos_to_win[256] = {
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
199, 252, 233, 226, 228, 224, 229, 231, 234, 235, 232, 239, 238, 236, 196, 197,
201, 230, 198, 244, 246, 242, 251, 249, 255, 214, 220, 248, 163, 216, 215, 131,
225, 237, 243, 250, 241, 209, 170, 186, 191, 174, 172, 189, 188, 161, 171, 187,
166, 166, 166, 166, 166, 193, 194, 192, 169, 166, 166,  43,  43, 162, 165,  43,
 43,  45,  45,  43,  45,  43, 227, 195,  43,  43,  45,  45, 166,  45,  43, 164,
240, 208, 202, 203, 200, 105, 205, 206, 207,  43,  43, 166,  95, 166, 204, 175,
211, 223, 212, 210, 245, 213, 181, 254, 222, 218, 219, 217, 253, 221, 175, 180,
173, 177,  61, 190, 182, 167, 247, 184, 176, 168, 183, 185, 179, 178, 166, 160
};

/* Tabla de conversión de caracteres Windows a MS-DOS */

byte win_to_dos[256] =
{
   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
 128, 129, 130, 159, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
 255, 173, 189, 156, 207, 190, 254, 245, 249, 184, 166, 174, 170, 240, 169, 238,
 248, 241, 253, 252, 239, 230, 244, 250, 247, 251, 167, 175, 172, 171, 243, 168,
 183, 181, 182, 199, 142, 143, 146, 128, 212, 144, 210, 211, 222, 214, 215, 216,
 209, 165, 227, 224, 226, 229, 153, 158, 157, 235, 233, 234, 154, 237, 232, 225,
 133, 160, 131, 198, 132, 134, 145, 135, 138, 130, 136, 137, 141, 161, 140, 139,
 208, 164, 149, 162, 147, 228, 148, 246, 155, 151, 163, 150, 129, 236, 231, 152
};

byte lower[256];

byte c_convert (byte c)
{
	return dos_chars ? c : win_to_dos[c];
}


void set_c_lower (const byte * from, const byte * to)
{
	if (dos_chars)
		while (*from) lower[win_to_dos[*from++]] = win_to_dos[*to++] ;
	else
		while (*from) lower[*from++] = *to++ ;
}

void inicializa_lower()
{
	memset (lower, 0, 256);
	set_c_lower  ("ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		      "abcdefghijklmnopqrstuvwxyz");
	set_c_lower  ("abcdefghijklmnopqrstuvwxyz",
		      "abcdefghijklmnopqrstuvwxyz");

#ifdef RESPETAR_ACENTOS
	set_c_lower  ("ÁÉÍÓÚÑÇÀÈÌÒÙÄËÏÖÜİÂÊÎÔÛÆÃÅÕ",
		      "áéíóúñçàèìòùäëïöüıâêîôûæãåõ");
	set_c_lower("áéíóúñçàèìòùäëïöüıâêîôûæãåõ",
				"áéíóúñçàèìòùäëïöüıâêîôûæãåõ");
#else
	set_c_lower  ("ÁÉÍÓÚÑÇÀÈÌÒÙÄËÏÖÜİÂÊÎÔÛÆÃÅÕ",
				  "aeiouncaeiouaeiouyaeiouæaao");
	set_c_lower  ("áéíóúñÑçÇàèìòùäëïöüıâêîôûæãåõÁÉÍÓÚ",
				"aeiounnccaeiouaeiouyaeiouÆaaoaeiou");
#endif

	set_c_lower("1234567890#$_ºª","1234567890#$_ºª");
}
