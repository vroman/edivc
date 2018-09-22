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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "main.h"
#include "extern.h"
#include "parser.h"
#include "compiler.h"
#include "varindex.h"

#include "language.h"

int len;
struct objeto *ob;
struct objeto *ob2;

int creaobj;
int numparams;

byte decl_struct = 0;	 /* 1 si se está declarando un struct */
byte struct_reserved = 0; /* 1 si se está declarando la estructura reserved */

/* Errores específicos en la ejecución de una DLL */
void dll_error(int error, ...)
{
	va_list opparam;

	va_start(opparam, error);
	print_translate(35, fichdll);
	vprintf(translate_dll_error(error - 1), opparam);
	printf("\n");
	n_errors++;
}

int EDIV_Export(char *cadena, int nparam, void *hfuncion)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	if (nparam < 0)
	{
		dll_error(5, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, nparam);

#ifdef DEBUG_DLL
	printf("export_function:\t-- ID FUNCION: %d\n"
		   "export_function:\tCadena exportada: %s\n"
		   "export_function:\tN. parametros: %d\n"
		   "export_function:\tOffset de la funcion: 0x%X\n",
		   n_externs, cadena, nparam, (unsigned int)hfuncion);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	ob->tipo = tfext;
	ob->fext.codigo = n_externs;
	ob->fext.num_par = nparam;

	n_externs++;

	return 1;
}

int EDIV_Export_Entrypoint(int ep, void *hfuncion)
{
	entrypoints[n_entrypoints].tipo = ep;
	entrypoints[n_entrypoints].hfuncion = hfuncion;
	entrypoints[n_entrypoints].dll = numdlls;
	return 0;
}

int EDIV_Export_Priority(int priority)
{
	dlls[numdlls].prioridad = priority;
	return 0;
}

int EDIV_Export_Const(char *cadena, int valor)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_const:\t-- ID OBJETO: %d\n"
		   "export_const:\tCadena exportada: %s\n"
		   "export_const:\tValor: %d\n"
		   "export_const:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, valor, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	ob->tipo = tcons;
	ob->cons.valor = valor;
	ob->cons.literal = 0;

	return 1;
}

int EDIV_Export_Global(char *cadena, int valor)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_global:\t-- ID OBJETO: %d\n"
		   "export_global:\tCadena exportada: %s\n"
		   "export_global:\tValor: %d\n"
		   "export_global:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, valor, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	indexa_variable(v_global, cadena, imem);

	ob->tipo = tvglo;
	ob->vglo.offset = imem;
	mem[imem] = valor;

	return imem++;
}

int EDIV_Export_Global_Tab(char *cadena, int numregs)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_global_tab:\t-- ID OBJETO: %d\n"
		   "export_global_tab:\tCadena exportada: %s\n"
		   "export_global_tab:\tN.regs: %d\n"
		   "export_global_tab:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, numregs, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	indexa_variable(v_global, cadena, imem);

	ob->tipo = ttglo;
	ob->tglo.offset = imem;
	ob->tglo.len1 = len = numregs;
	ob->tglo.len2 = -1;
	ob->tglo.len3 = -1;
	ob->tglo.totalen = len + 1;

	/* Inicializamos la tabla a 0 */
	do
	{
		mem[imem++] = 0;
	} while (len--);

	return ob->tglo.offset;
}

int EDIV_Export_Global_Struct(char *cadena, int numregs)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_global_struct:\t-- ID OBJETO: %d\n"
		   "export_global_struct:\tCadena exportada: %s\n"
		   "export_global_struct:\tN.regs: %d\n"
		   "export_global_struct:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, numregs, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	indexa_variable(v_global, cadena, imem);

	decl_struct = 1;

	ob->tipo = tsglo;
	ob->sglo.offset = imem;
	ob->sglo.items1 = numregs;
	ob->sglo.items2 = ob->sglo.items3 = -1;
	ob->sglo.totalitems = numregs + 1;
	ob->sglo.len_item = 0;
	member = ob;
	len = 0;

	return imem;
}

int EDIV_Export_Member_Int(char *cadena, int valor)
{
	if (!decl_struct)
	{
		dll_error(7, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_member_int:\t-- ID OBJETO: %d\n"
		   "export_member_int:\tCadena exportada: %s\n"
		   "export_member_int:\tValor: %d\n"
		   "export_member_int:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, valor, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob2 = o;

	if ((*ob2).tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	if (ob->tipo == tsglo)
	{ /* int miembro de struct global */
		(*ob2).tipo = tvglo;
		(*ob2).vglo.offset = len++;
		mem[imem] = valor;
		ob->sglo.len_item++;
		return imem++;
	}
	else
	{ /* int miembro de struct local */
		if (struct_reserved)
			indexa_variable(v_reserved, cadena, iloc);
		(*ob2).tipo = tvloc;
		(*ob2).vloc.offset = len++;
		loc[iloc] = valor;
		ob->sloc.len_item++;
		return iloc++;
	}
}

int EDIV_Export_Member_Str(char *cadena, int tamano)
{
	if (!decl_struct)
	{
		dll_error(7, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_member_str:\t-- ID OBJETO: %d\n"
		   "export_member_str:\tCadena exportada: %s\n"
		   "export_member_str:\tTamano: %d\n"
		   "export_member_str:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, tamano, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob2 = o;

	if ((*ob2).tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	if (ob->tipo == tsglo)
	{ /* string miembro de struct global */

		(*ob2).tipo = tcglo;
		(*ob2).cglo.offset = len + 1;

		if (((*ob2).cglo.totalen = tamano) < 0)
		{
			dll_error(8, cadena);
			return 0;
		}
		if ((*ob2).cglo.totalen > 0xFFFFF)
		{
			dll_error(8, cadena);
			return 0;
		}

		mem[imem] = 0xDAD00000 | (*ob2).cglo.totalen;
		len += 1 + ((*ob2).cglo.totalen + 5) / 4;
		mem[imem + 1] = 0;
		imem += 1 + ((*ob2).cglo.totalen + 5) / 4;
		ob->sglo.len_item += 1 + ((*ob2).cglo.totalen + 5) / 4;

		return (*ob2).cglo.offset;
	}
	else
	{ /* string miembro de struct local */

		(*ob2).tipo = tcloc;
		(*ob2).cloc.offset = len + 1;

		if (((*ob2).cloc.totalen = tamano) < 0)
		{
			dll_error(8, cadena);
			return 0;
		}
		if ((*ob2).cloc.totalen > 0xFFFFF)
		{
			dll_error(8, cadena);
			return 0;
		}

		loc[iloc] = 0xDAD00000 | (*ob2).cloc.totalen;
		len += 1 + ((*ob2).cloc.totalen + 5) / 4;
		loc[iloc + 1] = 0;
		iloc += 1 + ((*ob2).cloc.totalen + 5) / 4;
		ob->sloc.len_item += 1 + ((*ob2).cloc.totalen + 5) / 4;

		return (*ob2).cloc.offset;
	}
}

int EDIV_Export_Member_Tab(char *cadena, int numregs)
{
	if (!decl_struct)
	{
		dll_error(7, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_member_tab:\t-- ID OBJETO: %d\n"
		   "export_member_tab:\tCadena exportada: %s\n"
		   "export_member_tab:\tN.regs: %d\n"
		   "export_member_tab:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, numregs, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob2 = o;

	if ((*ob2).tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	if (ob->tipo == tsglo)
	{ /* array miembro de struct global */
		(*ob2).tipo = ttglo;
		(*ob2).tglo.offset = len;
		(*ob2).tglo.len1 = numregs;
		(*ob2).tglo.len2 = -1;
		(*ob2).tglo.len3 = -1;
		(*ob2).tglo.totalen = numregs + 1;
		len += numregs + 1;
		memset(&mem[imem], 0, (numregs + 1) * 4);
		imem += numregs + 1;
		ob->sglo.len_item += numregs + 1;

		return (*ob2).tglo.offset;
	}
	else
	{ /* array miembro de struct local */
		if (struct_reserved)
			indexa_variable(v_reserved, cadena, iloc);
		(*ob2).tipo = ttloc;
		(*ob2).tloc.offset = len;
		(*ob2).tloc.len1 = numregs;
		(*ob2).tloc.len2 = -1;
		(*ob2).tloc.len3 = -1;
		(*ob2).tloc.totalen = numregs + 1;
		len += numregs + 1;
		memset(&loc[iloc], 0, (numregs + 1) * 4);
		iloc += numregs + 1;
		ob->sloc.len_item += numregs + 1;

		return (*ob2).tloc.offset;
	}
}

int EDIV_Export_EndStruct()
{
	if (!decl_struct)
	{
		dll_error(9);
		return 0;
	}

#ifdef DEBUG_DLL
	printf("export_endstruct:\t--- estructura cerrada\n");
#endif

	member = NULL;

	if (ob->tipo == tsglo)
	{ /* cierra estructura global */
		if (ob->sglo.len_item == 0)
		{
			dll_error(10);
			return 0;
		}
		if (ob->sglo.totalitems > 1)
		{
			len = (ob->sglo.totalitems - 1) * ob->sglo.len_item - 1;
			do
			{
				mem[imem] = mem[imem - ob->sglo.len_item];
				imem++;
			} while (len--);
		}
	}
	else
	{ /* cierra estructura local */
		if (ob->sloc.len_item == 0)
		{
			dll_error(10);
			return 0;
		}
		if (ob->sloc.totalitems > 1)
		{
			len = (ob->sloc.totalitems - 1) * ob->sloc.len_item - 1;
			do
			{
				loc[iloc] = loc[iloc - ob->sloc.len_item];
				iloc++;
			} while (len--);
		}
	}

	decl_struct = 0;
	struct_reserved = 0;

	return 1;
}

int EDIV_Export_Local(char *cadena, int valor)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_local:\t-- ID OBJETO: %d\n"
		   "export_local:\tCadena exportada: %s\n"
		   "export_local:\tValor: %d\n"
		   "export_local:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, valor, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	indexa_variable(v_local, cadena, iloc);

	ob->tipo = tvloc;
	ob->vloc.offset = iloc;
	loc[iloc] = valor;

	return iloc++;
}

int EDIV_Export_Local_Tab(char *cadena, int numregs)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_local_tab:\t-- ID OBJETO: %d\n"
		   "export_local_tab:\tCadena exportada: %s\n"
		   "export_local_tab:\tN.regs: %d\n"
		   "export_local_tab:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, numregs, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	indexa_variable(v_local, cadena, iloc);

	ob->tipo = ttloc;
	ob->tloc.offset = iloc;
	ob->tloc.len1 = len = numregs;
	ob->tloc.len2 = -1;
	ob->tloc.len3 = -1;
	ob->tloc.totalen = len + 1;

	/* Inicializamos la tabla a 0 */
	do
	{
		loc[iloc++] = 0;
	} while (len--);

	return ob->tloc.offset;
}

int EDIV_Export_Local_Struct(char *cadena, int numregs)
{
	if (decl_struct)
	{
		dll_error(6, cadena);
		return 0;
	}

	creaobj = crea_objeto((byte *)cadena, -1);

#ifdef DEBUG_DLL
	printf("export_local_struct:\t-- ID OBJETO: %d\n"
		   "export_local_struct:\tCadena exportada: %s\n"
		   "export_local_struct:\tN.regs: %d\n"
		   "export_local_struct:\tOffset del objeto: 0x%X\n",
		   num_obj - 1, cadena, numregs, (unsigned int)o);
#endif

	if (creaobj)
	{
		dll_error(creaobj, cadena);
		return 0;
	}

	ob = o;

	if (ob->tipo != tnone)
	{
		dll_error(5, cadena);
		return 0;
	}

	decl_struct = 1;
	if (!strcmp(cadena, "reserved"))
	{
		struct_reserved = 1;
	}
	else
	{
		indexa_variable(v_local, cadena, iloc);
	}

	ob->tipo = tsloc;
	ob->sloc.offset = iloc;
	ob->sloc.items1 = numregs;
	ob->sloc.items2 = ob->sloc.items3 = -1;
	ob->sloc.totalitems = numregs + 1;
	ob->sloc.len_item = 0;
	member = ob;
	len = 0;

	return iloc;
}
