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

/* Errores en tiempo de ejecución */

/* POR HACER: En Windows se muestra un MsgBox para los errores, pero en Linux se hace
 * un simple printf. Esto no es problema en los errores no críticos de la versión debug
 * (ya que los errores los muestra la DLL de Delphi), o si siempre ejecutamos los juegos
 * desde una consola, pero el usuario final los ejecutará haciendo doble clic sobre el
 * programa. Por tanto en Linux también tenemos que mostrar una MsgBox, y no podemos usar
 * el propio buffer de SDL para imprimirlos (ya que uno de los errores no críticos es la
 * falta de memoria), así que habrá que tirar de X a lo basto, recomiendo mirar el código
 * fuente de XBill ya que implementa MsgBoxes de una forma reducida usando Motif o Athena
 * (basada en Xaw). XBill también puede usar GTK pero esa librería ya es demasiado grande
 * (la librería a usar debe ser linkada de forma estática)
 */

#ifdef _WIN32
#	include <windows.h>
#endif
#include "main.h"
#include "language.h"

/* Error ( numero, parametros [...] )
 *
 * Genera un error en tiempo de ejecución, no crítico (en debug se llama al trazador)
 * ATENCIÓN: Ahora mismo está en forma de RELEASE, para la versión DEBUG del stub se deberá
 * invocar al trazador (llamando a la función adecuada, que aún no está hecha, en debug.dll)
 * Otra nota: he puesto exactamente los mismos errores de DIV2, ya que tenemos que mantener
 * la compatibilidad con el ignore_error... 100% compatible, recordad.. :P
 */

// POR HACER: ignore_errors, ignore_error()

void error(int num, ...)
{
	va_list opparam;
	char mensaje[256];

	va_start(opparam,num);
	sprintf(mensaje,translate(0), num);
	vsprintf(mensaje,translate_runtime_error(num-100),opparam);

	#ifdef _WIN32
		MessageBox(0,mensaje,translate(1),MB_ICONERROR);
	#else
		printf("%s\n",mensaje);
	#endif

	exit(num);
}


/* critical_error ( numero, parametros [...] )
 *
 * Interrumpe el programa con un mensaje de error. No es posible trazar.
 * Esta función es igual para release que para debug, ya que no se puede trazar en el punto
 * en que se produce un error de este tipo.
 */

void critical_error(int num, ...)
{
	va_list opparam;
	char mensaje[256];


	va_start(opparam,num);
	sprintf(mensaje,translate(0), num);
	vsprintf(mensaje,translate_critical_error(num),opparam);

	#ifdef WIN32
		MessageBox(0,mensaje,translate(2),MB_ICONERROR);
	#else
		printf("%s\n",mensaje);
	#endif

	exit(num);
}

