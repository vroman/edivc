/*
 * eDiv Compiler
 * Copyright (C) 2000-2003 Sion, Ltd.
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

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <string.h>
#endif

#include "shared.h"
#include "language.h"
#include "edivcfg/iniparser.h"

static char *ediv_strlwr(char *str);

int detecta_idioma_iso(char *lang)
{
	int i;
	static char getid[NUM_LANGUAGES][3] = {
		"es", /* español   */
		"it", /* italiano  */
		"pt", /* portugués */
		"en", /* inglés    */
		"ca", /* catalán   */
		"eu"  /* euskera   */
	};
	if (lang == NULL)
		return DEFAULT_LANGUAGE;
	if (strlen(lang) > 2)
		lang[2] = 0;
	ediv_strlwr(lang);
	for (i = 0; i < NUM_LANGUAGES; i++)
		if (lang[0] == getid[i][0])
			if (lang[1] == getid[i][1])
				break;
	if (i == NUM_LANGUAGES)
		i = DEFAULT_LANGUAGE;
	return i;
}

int detecta_idioma()
{
	char *langcfg = NULL;
	if (ini != NULL)
		langcfg = iniparser_getstr(ini, "general:lang");
	if (langcfg == NULL)
	{
#ifdef _WIN32
		int i;
		static int getid[NUM_LANGUAGES] = {
			0x0a, /* español   */
			0x10, /* italiano  */
			0x16, /* portugués */
			0x09, /* inglés    */
			0x03, /* catalán   */
			0x2d  /* euskera   */
		};
		LANGID lang;

		lang = GetSystemDefaultLangID() & 0xff;
		for (i = 0; i < NUM_LANGUAGES; i++)
			if (lang == getid[i])
				break;
		if (i == NUM_LANGUAGES)
			i = DEFAULT_LANGUAGE;
		return i;
#else
		langcfg = getenv("LANG");
		if (langcfg == NULL)
			return DEFAULT_LANGUAGE;
		else
			return detecta_idioma_iso(langcfg);
#endif
	}
	else
		return detecta_idioma_iso(langcfg);
}

char *translate_error(int num)
{

	static char *e[NUM_LANGUAGES][74] = {

#define TRANSLATE TRANSLATE_ERROR
#include "language_aux.h"

	};

	return e[idioma][num];
}

char *translate_warning(int num)
{

	static char *e[NUM_LANGUAGES][3] = {

#define TRANSLATE TRANSLATE_WARNING
#include "language_aux.h"

	};

	return e[idioma][num];
}

char *translate_dll_error(int num)
{
	static char *e[NUM_LANGUAGES][10] = {

#define TRANSLATE TRANSLATE_DLL_ERROR
#include "language_aux.h"

	};
	return e[idioma][num];
}

char *translate_ltlex_error(int num)
{
	static char *e[NUM_LANGUAGES][5] = {

#define TRANSLATE TRANSLATE_LTLEX_ERROR
#include "language_aux.h"

	};
	return e[idioma][num];
}

char *translate(int num)
{
	static char *e[NUM_LANGUAGES][74] = {

#define TRANSLATE TRANSLATE_COMPILER
#include "language_aux.h"

	};
	return e[idioma][num];
}

static char *ediv_strlwr(char *str)
{
	unsigned char *p = (unsigned char *)str;

	while (*p)
	{
		*p = tolower((unsigned char)*p);
		p++;
	}

	return str;
}