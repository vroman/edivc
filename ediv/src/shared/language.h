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
 * IDs de idioma:
 *   0  - ES - Español
 *   1  - IT - Italiano (CicTec)
 *   2  - PT - Portugués (tareco)
 *   3  - EN - Inglés (de momento Er_Makina :P)
 *   4  - CA - Catalán (BenKo)
 *   5  - EU - Euskera (Jorge Urreta)
 */

#ifndef __EDIV_LANGUAGE_H_
#define __EDIV_LANGUAGE_H_

#define NUM_LANGUAGES 6

#define DEFAULT_LANGUAGE 3

#ifdef TRANSLATE
#undef TRANSLATE
#endif

#define TRANSLATE_ERROR				1
#define TRANSLATE_WARNING			2
#define TRANSLATE_DLL_ERROR			3
#define TRANSLATE_LTLEX_ERROR		4
#define TRANSLATE_COMPILER			5
#define TRANSLATE_STUB				6
#define TRANSLATE_RUNTIME_ERROR		7
#define TRANSLATE_CRITICAL_ERROR	8


int idioma;

/* comunes */
int detecta_idioma();
char* translate(int num);
char* translate_dll_error(int num);

/* compilador */
char* translate_error(int num);
char* translate_warning(int num);
char* translate_ltlex_error(int num);

/* stub */
char* translate_runtime_error(int num);
char* translate_critical_error(int num);

#endif /* __EDIV_LANGUAGE_H_ */
