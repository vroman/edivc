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


/**** ¡¡¡ OJO CON LAS COMAS !!! ****/


#ifdef _WIN32
#	include <windows.h>
#else
#	include <stdlib.h>
#	include <string.h>
#endif

#include "shared.h"
#include "language.h"
#include "edivcfg/iniparser.h"


int detecta_idioma_iso(char* lang)
{
	int i;
	static char getid[NUM_LANGUAGES][3] = {
		"es",	/* español   */
		"it",	/* italiano  */
		"pt",	/* portugués */
		"en",	/* inglés    */
		"ca",	/* catalán   */
		"eu"	/* euskera   */
	};
	if(lang==NULL) return DEFAULT_LANGUAGE;
	if(strlen(lang)>2) lang[2]=0;
	strlwr(lang);
	for(i=0;i<NUM_LANGUAGES;i++)
		if(lang[0]==getid[i][0])
			if(lang[1]==getid[i][1])
				break;
	if(i==NUM_LANGUAGES) i=DEFAULT_LANGUAGE;
	return i;
}


int detecta_idioma()
{
	char* langcfg=NULL;
	if (ini!=NULL) langcfg=iniparser_getstr(ini,"general:lang");
	if(langcfg==NULL) {
		#ifdef _WIN32
			int i;
			static int getid[NUM_LANGUAGES] = {
				0x0a,	/* español   */
				0x10,	/* italiano  */
				0x16,	/* portugués */
				0x09,	/* inglés    */
				0x03,	/* catalán   */
				0x2d	/* euskera   */
			};
			LANGID lang;

			lang=GetSystemDefaultLangID()&0xff;
			for(i=0;i<NUM_LANGUAGES;i++)
				if(lang==getid[i]) break;
			if(i==NUM_LANGUAGES) i=DEFAULT_LANGUAGE;
			return i;
		#else
			langcfg=getenv("LANG");
			if(langcfg==NULL) return DEFAULT_LANGUAGE;
			else return detecta_idioma_iso(langcfg);
		#endif
	}
	else return detecta_idioma_iso(langcfg);
}


char* translate_error(int num)
{

	static char *e[NUM_LANGUAGES][74] = {

	/* 0 - ESPAÑOL */

	/* 0	*/	"",
	/* 1	*/	"Llegó el final dentro de un comentario",
	/* 2	*/	"Excedida la capacidad del vector de nombres",
	/* 3	*/	"Literal sin cerrar",
	/* 4	*/	"Símbolo no reconocido",
	/* 5	*/	"Carácter no reconocido",
	/* 6	*/	"Excedida la capacidad de la tabla de objetos",
	/* 7	*/	"Esperando '='",
	/* 8	*/	"Esperando una opción de compilación",
	/* 9	*/	"Esperando ';'",
	/* 10	*/	"Se esperaba un operando",
	/* 11	*/	"Expresión incorrecta",
	/* 12	*/	"Asignación a cadena incorrecta",
	/* 13	*/	"Asignación incorrecta",
	/* 14	*/	"No se puede calcular el offset del dato",
	/* 15	*/	"No se puede calcular el offset",
	/* 16	*/	"No se puede realizar el incremento",
	/* 17	*/	"Local no se puede acceder",
	/* 18	*/	"Esperando ')'",
	/* 19	*/	"Esperando ']'",
	/* 20	*/	"No es un tipo de proceso",
	/* 21	*/	"Esperando el nombre de un proceso",
	/* 22	*/	"Esperando '('",
	/* 23	*/	"Esperando un nombre",
	/* 24	*/	"No se puede calcular el tamaño",
	/* 25	*/	"Expresión incorrecta",
	/* 26	*/	"No se puede recibir una estructura como parámetro",
	/* 27	*/	"Esperando el nombre de la estructura",
	/* 28	*/	"Declaración incorrecta de puntero a una estructura",
	/* 29	*/	"Esperando el nombre de la cadena",
	/* 30	*/	"El nombre no es nuevo",
	/* 31	*/	"Cadena de texto de longitud negativa",
	/* 32	*/	"Cadena de texto demasiado larga",
	/* 33	*/	"No se puede especificar una tabla como parámetro",
	/* 34	*/	"Nombre desconocido: %s",
	/* 35	*/	"Se esperaba una coma",
	/* 36	*/	"Se esperaba otro parámetro",
	/* 37	*/	"No se puede acceder a datos PRIVATE externos",
	/* 38	*/	"Número de parámetros incorrecto",
	/* 39	*/	"Esperando un elemento de la estructura",
	/* 40	*/	"Tabla de longitud negativa",
	/* 41	*/	"La longitud no coincide con la declaración anterior",
	/* 42	*/	"No se puede inicializar un parámetro",
	/* 43	*/	"Estructura de longitud negativa",
	/* 44	*/	"Esperando PROGRAM",
	/* 45	*/	"Esperando el nombre del programa",
	/* 46	*/	"Se esperaba un literal",
	/* 47	*/	"Una estructura debe contener, al menos, un elemento",
	/* 48	*/	"Demasiados valores para la tabla",
	/* 49	*/	"Literal demasiado largo",
	/* 50	*/	"Valor BYTE fuera del rango (0...255)",
	/* 51	*/	"Valor WORD fuera del rango (0...65535)",
	/* 52	*/	"No se puede duplicar 0 o menos veces",
	/* 53	*/	"Inicialización incorrecta",
	/* 54	*/	"Se esperaba una expresión constante",
	/* 55	*/	"Demasiados valores para la estructura",
	/* 56	*/	"Esperando BEGIN",
	/* 57	*/	"Esperando END",
	/* 58	*/	"Esperando UNTIL",
	/* 59	*/	"Esperando una variable", /* (para el FROM)	*/
	/* 60	*/	"Esperando TO",
	/* 61	*/	"Sentencia FROM incorrecta", /* (from==to) */
	/* 62	*/	"El valor STEP no es válido",
	/* 63	*/	"Esperando CASE, DEFAULT o END",
	/* 64	*/	"Esperando ':'",
	/* 65	*/	"BREAK no aparece dentro de un bucle",
	/* 66	*/	"CONTINUE no aparece dentro de un bucle",
	/* 67	*/	"Se esperaba una sentencia",
	/* 68	*/	"Expresión sin sentido",
	/* 69	*/	"Asignación dentro de una condición",
	/* 70	*/	"Esperando el nombre del proceso o función",
	/* 71	*/	"Esperando PROCESS o FUNCTION",
	/* 72	*/	"Parámetro no válido",
	/* 73	*/	"ELSEIF no aparece dentro de un bloque IF",

	/* 1 - ITALIANO */

	/* 0	*/	"",
	/* 1	*/	"Si aspettava '*/'",
	/* 2	*/	"Capacità vettore nomi oltrepassata",
	/* 3	*/	"Si aspettava '\"'",
	/* 4	*/	"Simbolo non riconosciuto",
	/* 5	*/	"Carattere non riconosciuto",
	/* 6	*/	"Capacità tabella oggetti oltrepassata",
	/* 7	*/	"Aspettando '='",
	/* 8	*/	"Aspettando un'opzione di compilazione",
	/* 9	*/	"Aspettando ';'",
	/* 10	*/	"Si aspettava un operando",
	/* 11	*/	"Espressione incorretta",
	/* 12	*/	"Assegnazione a catena incorretta",
	/* 13	*/	"Assegnazione incorretta",
	/* 14	*/	"Impossibile calcolare l'offset del dato",
	/* 15	*/	"Impossibile calcolare l'offset",
	/* 16	*/	"Non è possibile realizzare l'incremento",
	/* 17	*/	"Impossibile accedere a local",
	/* 18	*/	"Aspettando ')'",
	/* 19	*/	"Aspettando ']'",
	/* 20	*/	"Non è un tipo di proceso",
	/* 21	*/	"Aspettando il nome di un processo",
	/* 22	*/	"Aspettando '('",
	/* 23	*/	"Aspettando un nome",
	/* 24	*/	"Impossibile calcolare la grandezza",
	/* 25	*/	"Espressione incorretta",
	/* 26	*/	"Non è possibile ricevere una struttura come parametro",
	/* 27	*/	"Aspettando il nome della struttura",
	/* 28	*/	"Dichiarazione incorretta di puntatore a struttura",
	/* 29	*/	"Aspettando il nome della catena",
	/* 30	*/	"Il nome non è nuovo",
	/* 31	*/	"Catena di testo di lunghezza negativa",
	/* 32	*/	"Catena di testo troppo lunga",
	/* 33	*/	"Non si può specificare una tabella come parametro",
	/* 34	*/	"Nome sconosciuto: %s",
	/* 35	*/	"Si aspettava una virgola",
	/* 36	*/	"Si aspettava un altro parametro",
	/* 37	*/	"Non è possibile accedere a dati PRIVATE esterni",
	/* 38	*/	"Numero di parametri incorretto",
	/* 39	*/	"Aspettando un elemento della struttura",
	/* 40	*/	"Tabella di lunghezza negativa",
	/* 41	*/	"La lunghezza non coincide con la dichiarazione precedente",
	/* 42	*/	"Impossibile inizializzare un parametro",
	/* 43	*/	"Struttura di lunghezza negativa",
	/* 44	*/	"Aspettando PROGRAM",
	/* 45	*/	"Aspettando il nome del programma",
	/* 46	*/	"Si aspettava un stringa",
	/* 47	*/	"Una struttura deve contenere almeno un elemento",
	/* 48	*/	"Troppi valori per la tabella",
	/* 49	*/	"Stringa troppo lunga",
	/* 50	*/	"Valore BYTE fuori rango (0...255)",
	/* 51	*/	"Valore WORD fuori rango (0...65535)",
	/* 52	*/	"Non si può duplicare 0 o meno volte",
	/* 53	*/	"Inizializzazione incorretta",
	/* 54	*/	"Si aspettava un'espressione costante",
	/* 55	*/	"Troppi valori per la struttura",
	/* 56	*/	"Aspettando BEGIN",
	/* 57	*/	"Aspettando END",
	/* 58	*/	"Aspettando UNTIL",
	/* 59	*/	"Aspettando una variabile",
	/* 60	*/	"Aspettando TO",
	/* 61	*/	"Sentenza FROM incorretta",
	/* 62	*/	"Il valore STEP non è valido",
	/* 63	*/	"Aspettando CASE, DEFAULT o END",
	/* 64	*/	"Aspettando ':'",
	/* 65	*/	"BREAK non appare dentro un bugle",
	/* 66	*/	"CONTINUE non appare dentro un bugle",
	/* 67	*/	"Si aspettava una sentenza",
	/* 68	*/	"Espressione insensata",
	/* 69	*/	"Assegnazione dentro una condizione",
	/* 70	*/	"Aspettando un nome di processo o funzione",
	/* 71	*/	"Aspettando PROCESS o FUNCTION",
	/* 72	*/	"Parametro non valido",
	/* 73	*/	"ELSEIF non appare dentro un blocco IF",

	/* 2 - PORTUGUÉS */

	/* 0	*/	"",
    /* 1    */  "Esperava-se '*/'",
	/* 2	*/	"Capacidade do vector de nomes excedida",
    /* 3    */  "Esperava-se '\"'",
    /* 4    */  "Símbolo desconhecido",
    /* 5    */  "Caracter desconhecido",
	/* 6	*/	"Capacidade da tabela de objectos excedida",
    /* 7    */  "Esperando '='",
    /* 8    */  "Esperando uma opção de compilação",
    /* 9    */  "Esperando ';'",
	/* 10   */  "Esperando um operando",
	/* 11   */  "Expressão incorrecta",
	/* 12   */  "Asignação a cadeia incorrecta",
	/* 13   */  "Asignação incorrecta",
	/* 14   */  "Não se pode calcular o offset do dado",
	/* 15   */  "Não se pode calcular o offset",
	/* 16   */  "Não se pode realizar o incremento",
	/* 17   */  "Local não se pode aceder",
	/* 18   */  "Esperando ')'",
	/* 19   */  "Esperando ']'",
	/* 20   */  "Não é um tipo de processo",
	/* 21   */  "Esperando o nome de um processo",
	/* 22   */  "Esperando '('",
	/* 23   */  "Esperando um nome",
	/* 24   */  "Não se pode calcular o tamanho",
	/* 25   */  "Expressão incorrecta",
	/* 26   */  "Não se pode receber uma estructura como parametro",
	/* 27   */  "Esperando o nome da estructura",
	/* 28   */  "Declaração incorrecta de ponteiro a uma estructura",
	/* 29   */  "Esperando o nome da cadeia",
	/* 30   */  "O nome não é novo",
	/* 31   */  "Cadeia de texto de longitude negativa",
	/* 32   */  "Cadeia de texto demasiado larga",
	/* 33   */  "Não se pode especificar uma tabela como parametro",
	/* 34   */  "Nome desconhecido: %s",
	/* 35   */  "Esperava-se uma virgula",
	/* 36   */  "Esperava-se outro parametro",
	/* 37   */  "Não se pode aceder a dados PRIVATE externos",
	/* 38   */  "Número de parametros incorrecto",
	/* 39   */  "Esperando um elemento da estructura",
	/* 40   */  "Tabela de longitude negativa",
	/* 41   */  "A longitude não coincide con a declaração anterior",
	/* 42   */  "Não se pode inicializar un parametro",
	/* 43   */  "Estructura de longitude negativa",
	/* 44	*/	"Esperando PROGRAM",
	/* 45   */  "Esperando o nome do programa",
	/* 46   */  "Esperava-se um literal",
	/* 47   */  "Uma estructura deve conter, pelo menos, um elemento",
	/* 48   */  "Demasiados valores para a tabela",
	/* 49	*/	"Literal demasiado largo",
	/* 50   */  "Valor BYTE fora de alcance (0...255)",
	/* 51   */  "Valor WORD fora de alcance (0...65535)",
	/* 52   */  "Não se pode duplicar 0 o menos veces",
	/* 53   */  "Inicialização incorrecta",
	/* 54   */  "Esperava-se uma expressão constante",
	/* 55   */  "Demasiados valores para a estructura",
	/* 56	*/	"Esperando BEGIN",
	/* 57	*/	"Esperando END",
	/* 58	*/	"Esperando UNTIL",
	/* 59   */  "Esperando uma variable", 
	/* 60	*/	"Esperando TO",
	/* 61   */  "Sentença FROM incorrecta", 
	/* 62   */  "O valor STEP não é válido",
	/* 63   */  "Esperando CASE, DEFAULT ou END",
	/* 64	*/	"Esperando ':'",
	/* 65   */  "BREAK não aparece dentro de um ciclo",
	/* 66   */  "CONTINUE não aparece dentro de um ciclo",
	/* 67   */  "Esperava-se uma sentença",
	/* 68   */  "Expressão sem sentido",
	/* 69   */  "Asignação dentro de uma condição",
	/* 70   */  "Esperando o nome do processo ou função",
	/* 71   */  "Esperando PROCESS ou FUNCTION",
	/* 72   */  "Parametro não válido",
	/* 73   */  "ELSEIF não aparece dentro de um bloco IF",

	/* 3 - INGLÉS */

	/* 0	*/	"",
	/* 1	*/	"End Of File reached inside a comment",
	/* 2	*/	"Name vector out of bounds",
	/* 3	*/	"Literal not closed",
	/* 4	*/	"Symbol not recognized",
	/* 5	*/	"Character not recognized",
	/* 6	*/	"Object table out of bounds",
	/* 7	*/	"Expecting '='",
	/* 8	*/	"Expecting a compiler option",
	/* 9	*/	"Expecting ';'",
	/* 10	*/	"Expecting operand",
	/* 11	*/	"Incorrect expression",
	/* 12	*/	"Incorrect assignment to a string",
	/* 13	*/	"Incorrect assignment",
	/* 14	*/	"Can't calculate data's offset",
	/* 15	*/	"Can't calculate offset",
	/* 16	*/	"Can't do the increment",
	/* 17	*/	"Can't access local",
	/* 18	*/	"Expecting ')'",
	/* 19	*/	"Expecting ']'",
	/* 20	*/	"It isn't a process type",
	/* 21	*/	"Expecting a process' name",
	/* 22	*/	"Expecting '('",
	/* 23	*/	"Expecting name",
	/* 24	*/	"Can't calculate size",
	/* 25	*/	"Incorrect expression",
	/* 26	*/	"Can't receive a struct as a parameter",
	/* 27	*/	"Expecting the name for the struct",
	/* 28	*/	"Incorrect declaration of struct pointer",
	/* 29	*/	"Expecting the name for the string",
	/* 30	*/	"The name isn't new",
	/* 31	*/	"Negative-lenght string",
	/* 32	*/	"String too large",
	/* 33	*/	"Can't receive an array as a parameter",
	/* 34	*/	"Unknown name: %s",
	/* 35	*/	"Expecting ','",
	/* 36	*/	"Expecting another parameter",
	/* 37	*/	"Can't access external PRIVATE data",
	/* 38	*/	"Incorrect number of parameters",
	/* 39	*/	"Expecting a struct item",
	/* 40	*/	"Negative-lenght array",
	/* 41	*/	"The lenght doesn't match with previous declaration",
	/* 42	*/	"Can't initialize a parameter",
	/* 43	*/	"Negative-lenght struct",
	/* 44	*/	"Expecting PROGRAM",
	/* 45	*/	"Expecting program's name",
	/* 46	*/	"Expecting literal",
	/* 47	*/	"A struct must contain, at least, one item",
	/* 48	*/	"Too many values for the array",
	/* 49	*/	"Literal too long",
	/* 50	*/	"BYTE value out of bounds (0...255)",
	/* 51	*/	"WORD value out of bounds (0...65535)",
	/* 52	*/	"Can't duplicate 0 or less times",
	/* 53	*/	"Incorrect initialization",
	/* 54	*/	"Expecting a constant expression",
	/* 55	*/	"Too many values for the struct",
	/* 56	*/	"Expecting BEGIN",
	/* 57	*/	"Expecting END",
	/* 58	*/	"Expecting UNTIL",
	/* 59	*/	"Expecting variable",
	/* 60	*/	"Expecting TO",
	/* 61	*/	"Incorrect FROM statement", 
	/* 62	*/	"Incorrect STEP value",
	/* 63	*/	"Expecting CASE, DEFAULT or END",
	/* 64	*/	"Expecting ':'",
	/* 65	*/	"BREAK does not appear in a loop",
	/* 66	*/	"CONTINUE does not appear in a loop",
	/* 67	*/	"Expecting statement",
	/* 68	*/	"Expression without sense",
	/* 69	*/	"Assignment within a condition",
	/* 70	*/	"Expecting name for the process or function",
	/* 71	*/	"Expecting PROCESS or FUNCTION",
	/* 72	*/	"Parameter not valid",
	/* 73	*/	"ELSEIF does not appear in a IF block",

	/* 4 - CATALÁN */

	/* 0	*/ "",
	/* 1	*/ "Ha arribat el final dins d' un comentari",
	/* 2	*/ "Excedida la capacitat del vector de noms",
	/* 3	*/ "Literal sense tancar",
	/* 4	*/ "Símbol no reconegut",
	/* 5	*/ "Caràcter no reconegut",
	/* 6	*/ "Excedida la capacitat de la tabla d' objectes",
	/* 7	*/ "Esperant '='",
	/* 8	*/ "Esperant una opció de compilació",
	/* 9	*/ "Esperant ';'",
	/* 10	*/ "S' esperava un operand",
	/* 11	*/ "Expresió incorrecta",
	/* 12	*/ "Asignació a cadena incorrecta",
	/* 13	*/ "Asignació incorrecta",
	/* 14	*/ "No es pot calcular l' offset de la dada",
	/* 15	*/ "No es pot calcular l' offset",
	/* 16	*/ "No es pot realitzar l' increment",
	/* 17	*/ "Local no es pot accedir",
	/* 18	*/ "Esperant ')'",
	/* 19	*/ "Esperant ']'",
	/* 20	*/ "No és un tipus de procés",
	/* 21	*/ "Esperant el nom del procés",
	/* 22	*/ "Esperant '('",
	/* 23	*/ "Esperant un nom",
	/* 24	*/ "No es pot calcular el tamany",
	/* 25	*/ "Expresió incorrecta",
	/* 26	*/ "No es pot rebre una estructura com a paràmetre",
	/* 27	*/ "Esperant el nom de la estructura",
	/* 28	*/ "Declaració incorrecta de punter a una estructura",
	/* 29	*/ "Esperant el nom de la cadena",
	/* 30	*/ "El nom no és nou",
	/* 31	*/ "Cadena de text de longitud negativa",
	/* 32	*/ "Cadena de text massa llarga",
	/* 33	*/ "No es pot especificar una tabla com a paràmetre",
	/* 34	*/ "Nombre desconegut: %s",
	/* 35	*/ "S' esperava una coma",
	/* 36	*/ "S' esperaba altre paràmetre",
	/* 37	*/ "No es pot accedir a dades PRIVATE externes",
	/* 38	*/ "Nombre de paràmetres incorrecte",
	/* 39	*/ "Esperant un element de la estructura",
	/* 40	*/ "Tabla de longitud negativa",
	/* 41	*/ "La longitud no coincideix amb la declaració anterior",
	/* 42	*/ "No es pot inicialitzar un paràmetre",
	/* 43	*/ "Estructura de longitud negativa",
	/* 44	*/ "Esperant PROGRAM",
	/* 45	*/ "Esperant el nom del programa",
	/* 46	*/ "S' esperava un literal",
	/* 47	*/ "Una estructura ha de contenir, almenys, un element",
	/* 48	*/ "Massa valors per a la tabla",
	/* 49	*/ "Literal massa llarg",
	/* 50	*/ "Valor BYTE fora del rang (0...255)",
	/* 51	*/ "Valor WORD fora del rang (0...65535)",
	/* 52	*/ "No es pot duplicar 0 o menys vegades",
	/* 53	*/ "Inicialització incorrecta",
	/* 54	*/ "S' esperava una expresió constant",
	/* 55	*/ "Massa valors per a la estructura",
	/* 56	*/ "Esperant BEGIN",
	/* 57	*/ "Esperant END",
	/* 58	*/ "Esperant UNTIL",
	/* 59	*/ "Esperant una variable",
	/* 60	*/ "Esperant TO",
	/* 61	*/ "Sentència FROM incorrecta",
	/* 62	*/ "El valor STEP no es vàlid",
	/* 63	*/ "Esperant CASE, DEFAULT o END",
	/* 64	*/ "Esperant ':'",
	/* 65	*/ "BREAK no apareix dins d' un bucle",
	/* 66	*/ "CONTINUE no apareix dins d' un bucle",
	/* 67	*/ "S' esperava una sentència",
	/* 68	*/ "Expresió sense sentit",
	/* 69	*/ "Asignació dins d' una condició",
	/* 70	*/ "Esperant el nomb del procés o funció",
	/* 71	*/ "Esperant PROCESS o FUNCTION",
	/* 72	*/ "Paràmetre no vàlid",
	/* 73	*/ "ELSEIF no apareix dins d' un bloc IF",

	/* 5 - EUSKERA */

	/* 0	*/ "",
	/* 1	*/ "Bukaerara heldu izan da azalpen baten barruan",
	/* 2	*/ "Gainditu da izenen bektorearen kapazitatea",
	/* 3	*/ "Itxi gabeko literala",
	/* 4	*/ "Ezesaguneko ikurra",
	/* 5	*/ "Ezesaguneko karakterra",
	/* 6	*/ "Gainditu da objektuen taularen kapazitatea",
	/* 7	*/ "'='-ren zai...",
	/* 8	*/ "konpilazio aukera baten zai...",
	/* 9	*/ "';'-ren zai...",
	/* 10	*/ "Eragigai bat espero zen",
	/* 11	*/ "Okerreko espresioa",
	/* 12	*/ "Okerreko kateko esleipena",
	/* 13	*/ "Okerreko esleipena",
	/* 14	*/ "Ezin da datuaren offset-a kalkulatu",
	/* 15	*/ "Ezin da offset-a kalkulatu",
	/* 16	*/ "Ezin da gehikuntza gauzatu",
	/* 17	*/ "Ezin da lokala atzitu",
	/* 18	*/ "')'-ren zai...",
	/* 19	*/ "']'-ren zai...",
	/* 20	*/ "Ez da prozesu mota bat",
	/* 21	*/ "Prozesu baten izenaren zai...",
	/* 22	*/ "'('-ren zai...",
	/* 23	*/ "Izen baten zai...",
	/* 24	*/ "Ezin da neurria kalkulatu",
	/* 25	*/ "Okerreko espresioa",
	/* 26	*/ "Estruktura bat ezin da parametro moduan jaso",
	/* 27	*/ "Estruktura baten izenaren zai...",
	/* 28	*/ "Okerreko estrukturako eragile deklarazioa",
	/* 29	*/ "Kate baten izenaren zai...",
	/* 30	*/ "Izena ez da berria",
	/* 31	*/ "Neurri negatiboko testu katea",
	/* 32	*/ "Luzeera gehiehizko testu katea",
	/* 33	*/ "Ezin da taula bat parametro moduan espezifikatu",
	/* 34	*/ "Izen ezesaguna: %s",
	/* 35	*/ "Koma bat espero zen",
	/* 36	*/ "Beste parametro bat espero zen",
	/* 37	*/ "Ezin da kanpoko PRIVATE daturik atzitu",
	/* 38	*/ "Okerreko parametro kopurua",
	/* 39	*/ "Estrukturaren atal baten zai...",
	/* 40	*/ "Neurri negatiboko taula",
	/* 41	*/ "Neurria ez dator bat lehenagoko deklarazioarekin",
	/* 42	*/ "Ezin da parametro bat hasieratu",
	/* 43	*/ "Neurri negatiboko estruktura",
	/* 44	*/ "PROGRAM-ren zai...",
	/* 45	*/ "Programaren izenaren zai...",
	/* 46	*/ "Literal bat espero zen",
	/* 47	*/ "Estruktura batek atal bat gutxienez eduki behar du",
	/* 48	*/ "Balio gehiegi taularentzat",
	/* 49	*/ "Gehiegizko luzeerako literala",
	/* 50	*/ "BYTE balioa barrutiaz kanpo (0...255)",
	/* 51	*/ "WORD balioa barrutiaz kanpo  (0...65535)",
	/* 52	*/ "Ezin da bikoiztu 0 bider edo gutxiago",
	/* 53	*/ "Okerreko hasieraketa",
	/* 54	*/ "Espresio konstantea espero zen",
	/* 55	*/ "Bailo gehiegi estrukturarentzat",
	/* 56	*/ "BEGIN-en zai...",
	/* 57	*/ "END-en zai...",
	/* 58	*/ "UNTIL-en zai..",
	/* 59	*/ "Aldagai baten zai...",
	/* 60	*/ "TO-ren zai...",
	/* 61	*/ "Okerreko FROM Sententzia",
	/* 62	*/ "Okerreko STEP balioa",
	/* 63	*/ "CASE, DEFAULT edo END-en zai...",
	/* 64	*/ "':'-ren zai...",
	/* 65	*/ "BREAK ez dago begizta baten barruan",
	/* 66	*/ "CONTINUE ez dago begizta baten barruan",
	/* 67	*/ "Sententzia bat espero zen",
	/* 68	*/ "Zentzurik gabeko espresioa",
	/* 69	*/ "Esleipena baldintza baten barruan",
	/* 70	*/ "Prozesu edo funtzio baten izenaren zai...",
	/* 71	*/ "PROCESS edo FUNCTION-en zai",
	/* 72	*/ "Okerreko parametroa",
	/* 73	*/ "ELSEIF ez dago IF barruan"

	};

	return e[idioma][num];
}

char* translate_warning(int num)
{

	static char *e[NUM_LANGUAGES][3] = {

	/* 0 - ESPAÑOL */

	/* 0	*/	"",
	/* 1	*/	"SETUP_PROGRAM: Característica desfasada",
	/* 2	*/	"IMPORT: Sintaxis antigua, se ignora",

	/* 1 - ITALIANO */

	/* 0	*/	"",
	/* 1	*/	"SETUP_PROGRAM: Caratteristica sfasata",
	/* 2	*/	"IMPORT: Sintassi antica, si ignora",

	/* 2 - PORTUGUÉS */

	/* 0	*/	"",
	/* 1    */	"SETUP_PROGRAM: Caracteristica ambígua",
    /* 2    */  "IMPORT: Sintaxe antiga, ignorado",

	/* 3 - INGLÉS */

	/* 0	*/	"",
	/* 1	*/	"SETUP_PROGRAM: Ambiguous feature",
	/* 2	*/	"IMPORT: Old syntax, ignored",

	/* 4 - CATALÁN */

	/* 0	*/ "",
	/* 1	*/ "SETUP_PROGRAM: Característica desfasada",
	/* 2	*/ "IMPORT: Sintaxi antiga, s' ignora",

	/* 5 - EUSKERA */

	/* 0	*/ "",
	/* 1	*/ "SETUP_PROGRAM: Zaharkituriko karakteristika",
	/* 2	*/ "IMPORT: Antzinako sintaxia, alde batean utziko da"

	};

	return e[idioma][num];
}
	
char* translate_dll_error(int num)
{
	static char *e[NUM_LANGUAGES][10] = {

		/* 0 - ESPAÑOL */

		/* 1	*/ "Sobrepasado buffer de nombres de objeto",
		/* 2	*/ "Objeto '%s': El nombre no es nuevo",
		/* 3	*/ "Demasiados objetos declarados",
		/* 4	*/ "Funcion '%s': El nombre no es nuevo",
		/* 5	*/ "Error declarando objeto '%s'",
		/* 6	*/ "Objeto '%s': Declaración ilegal dentro de struct",
		/* 7	*/ "Objeto '%s': Declaración de un miembro fuera de struct",
		/* 8	*/ "Tamaño ilegal del string '%s'",
		/* 9	*/ "EndStruct sin Struct",
		/* 10	*/ "Una estructura debe contener al menos un elemento",

		/* 1 - ITALIANO */

		/* 1	*/ "Limite buffer nomi oggetto oltrepassato",
		/* 2	*/ "Oggetto '%s': Il nome non è nuovo",
		/* 3	*/ "Troppi oggetti dichiarati",
		/* 4	*/ "Funzione '%s': Il nome non è nuovo",
		/* 5	*/ "Errore dichiarando oggetto '%s'",
		/* 6	*/ "Oggetto '%s': Dichiarazione illegale dentro struct",
		/* 7	*/ "Oggetto '%s': Dichiarazione di un membro fuori struct",
		/* 8	*/ "Grandezza illegale di string '%s'",
		/* 9	*/ "EndStruct senza Struct",
		/* 10	*/ "Una struttura deve contenere almeno un elemento",

		/* 2 - PORTUGUÉS */

		/* 1	*/ "Buffer de nomes de objecto ultrapassado",
		/* 2	*/ "Objecto '%s': O nome não é novo",
		/* 3	*/ "Demasiados objectos declarados",
		/* 4	*/ "Função '%s': O nome não é novo",
		/* 5	*/ "Erro declarando objecto '%s'",
		/* 6	*/ "Objecto '%s': Declaração ilegal dentro de struct",
		/* 7	*/ "Objecto '%s': Declaração de um membro fora de struct",
		/* 8	*/ "Tamanho ilegal da string '%s'",
		/* 9	*/ "EndStruct sem Struct",
		/* 10	*/ "Uma estructura deve conter ao menos um elemento",

		/* 3 - INGLÉS */

		/* 1	*/ "Object name buffer out of bounds",
		/* 2	*/ "Object '%s': The name isn't new",
		/* 3	*/ "Too many objects declared",
		/* 4	*/ "Function '%s': The name isn't new",
		/* 5	*/ "Error declaring object '%s'",
		/* 6	*/ "Object '%s': Illegal declaration inside a struct",
		/* 7	*/ "Object '%s': Member declaration outside a struct",
		/* 8	*/ "Illegal size for string '%s'",
		/* 9	*/ "EndStruct without Struct",
		/* 10	*/ "A struct must contain at least one item",

		/* 4 - CATALÁN */

		/* 1	*/ "Sobrepassat buffer de noms d' objecte",
		/* 2	*/ "Objecte '%s': El nom no és nou",
		/* 3	*/ "Massa objectes declarats",
		/* 4	*/ "Funció '%s': El nom no és nou",
		/* 5	*/ "Error declarant objecte '%s'",
		/* 6	*/ "Objecte '%s': Declaració il·legal dins d' struct",
		/* 7	*/ "Objecte '%s': Declaració d' un membre fora d' struct",
		/* 8	*/ "Tamany il·legal de l' string '%s'",
		/* 9	*/ "EndStruct sense Struct",
		/* 10	*/ "Una estructura ha de contenir almenys un element",

		/* 5 - EUSKERA */

		/* 1	*/ "Objektu izenen buffer-aren kapazitatea gainditurik",
		/* 2	*/ "'%s'objektua: Izena ez da berria",
		/* 3	*/ "Gehiegizko deklaraturiko objektuak",
		/* 4	*/ "'%s' funtzioa: izena ez da berria",
		/* 5	*/ "Akatz bat sortu da '%s' objektua deklaratzean",
		/* 6	*/ "'%s'objektua: Debekaturiko deklarazioa struct barruan",
		/* 7	*/ "'%s'objektua: Atal baten deklarazioan struct kanpo",
		/* 8	*/ "'%s' String-eko okerreko neurria",
		/* 9	*/ "Struct gabeko EndStruct",
		/* 10	*/ "Estruktura batek atal bat gutxienez eduki behar du"

	};
	return e[idioma][num];
}

char* translate_ltlex_error(int num)
{
	static char *e[NUM_LANGUAGES][5] = {

		/* 0 - ESPAÑOL */

		/* 0	*/ "",
		/* 1	*/ "Archivo ltlex.def no encontrado",
		/* 2	*/ "Se esperaba un valor hexadecimal (&00..&FF)",
		/* 3	*/ "Demasiados símbolos definidos",
		/* 4	*/ "Símbolo no válido",

		/* 1 - ITALIANO */

		/* 0	*/ "",
		/* 1	*/ "Archivio ltlex.def non trovato",
		/* 2	*/ "Si aspettava un valore esadecimale (&00..&FF)",
		/* 3	*/ "Troppi simboli definiti",
		/* 4	*/ "Simbolo non valido",

		/* 2 - PORTUGUÉS */

		/* 0	*/ "",
		/* 1	*/ "Arquivo ltlex.def não encontrado",
		/* 2	*/ "Esperava-se um valor hexadecimal (&00..&FF)",
		/* 3	*/ "Demasiados símbolos definidos",
		/* 4	*/ "Símbolo inválido",

		/* 3 - INGLÉS */

		/* 0	*/ "",
		/* 1	*/ "File ltlex.def not found",
		/* 2	*/ "Expecting an hexadecimal value (&00..&FF)",
		/* 3	*/ "Too many symbols defined",
		/* 4	*/ "Symbol not valid",

		/* 4 - CATALÁN */

		/* 0	*/ "",
		/* 1	*/ "Arxiu ltlex.def no trobat",
		/* 2	*/ "S' esperava un valor hexadecimal (&00..&FF)",
		/* 3	*/ "Massa símbols definits",
		/* 4	*/ "Símbol no vàlid",

		/* 5 - EUSKERA */

		/* 0	*/ "",
		/* 1	*/ "Ezin da ltlex.def fitxategia aurkitu",
		/* 2	*/ "Balio hamaseitar bat espero zen (&00..&FF)",
		/* 3	*/ "Deklraturiko ikur gehiegi",
		/* 4	*/ "Okerreko ikurra"

	};
	return e[idioma][num];
}


char* translate(int id_cadena)
{
	switch(idioma) {

	case 0:	/* español */
		{
			static char* msg[] = {
				/* 0	*/ " **** Compilador eDIV " VERSION " ****\nCopyleft (c) 2000-2002 Sion, Ltd.\nhttp://www.edivcentral.com\n",
				/* 1	*/ "ATENCION: Esta version de eDIV esta inacabada.\nPor favor informe de cualquier bug a: bugs@edivcentral.com\n",
				/* 2	*/ "Ayuda de opciones de comando:\n\n",
				/* 3	*/ "%s [opciones] archivo.prg [ejecutable]\n\n",
				/* 4	*/ "Lista de opciones:\n",
				/* 5	*/ "    -h, --help      Muestra esta ayuda y sale\n",
				/* 6	*/ "    -d, --debug     Genera información adicional de depurado\n",
				/* 7	*/ "    -l, --list      Guarda el listado EML como <programa.eml>\n",
				/* 8	*/ "    -t, --table     Guarda la tabla de objetos como <programa.tab>\n",
				/* 9	*/ "    -c, --check     No genera un ejecutable (para comprobar sintaxis, etc.)\n",
				/* 10	*/ "    -s, --system    Sistema Operativo destino (por defecto %s)\n",
				/* 11	*/ "    -v, --version   Muestra versión y sale\n\n",
				/* 12	*/ "Parámetro --system: debe indicar un sistema destino\nConsulte la documentación para obtener ayuda\n",
				/* 13	*/ "Parámetro erróneo: %s\nUse la opción -h para ver la ayuda\n",
				/* 14	*/ "Parámetro -s: debe indicar un sistema destino\nConsulte la documentación para obtener ayuda\n",
				/* 15	*/ "Parámetro erróneo: -%c\nUse la opción -h para ver la ayuda\n",
				/* 16	*/ "No se ha especificado un archivo\nUse la opción -h para ver la ayuda\n",
				/* 17	*/ "edivc: Error al abrir archivo %s.\n",
				/* 18	*/ "edivc: Compilando: %s\n",
				/* 19	*/ "edivc: ERROR: No se encuentra %s\n",
				/* 20	*/ "edivc: ERROR: Formato incorrecto de %s\n",
				/* 21	*/ "edivc: Interrumpido (%d advertencias)\n",
				/* 22	*/ "edivc: Finalizado con éxito (%d advertencias)\n",
				/* 23	*/ "edivc: Salida con errores\n",
				/* 24	*/ "edivc: ERROR: Memoria insuficiente\n",
				/* 25	*/ "%s:%d:%d: ERROR %d: ",
				/* 26	*/ "%s:%d: Advertencia: ",
				/* 27	*/ "Precompilando...\n",
				/* 28	*/ "Compilando...\n",
				/* 29	*/ "Error creando archivo de intercambio\n",
				/* 30	*/ "Generando listados...\n",
				/* 31	*/ "Guardando ejecutable...\n",
				/* 32	*/ "Guardando información de depurado...\n",
				/* 33	*/ "Error escribiendo ejecutable\n",
				/* 34	*/ "Compilación finalizada.\n",
				/* 35	*/ "%s: ERROR: ",
				/* 36	*/ "Error creando archivo %s\n",
				/* 37	*/ "Listado de objetos de %s - eDIV versión " VERSION "\n\n",
				/* 38	*/ "\tbloque(%u), anterior(%u)\n",
				/* 39	*/ "Código EML de %s - eDIV versión " VERSION "\n",
				/* 40	*/ "edivc: ERROR: %s\n\n",
				/* 41	*/ "edivc: %s (linea %d) ERROR: %s\n\n",
				/* 42	*/ "edivc: Interrumpido - se han encontrado errores\n",
				#ifdef _WIN32
				/* 43	*/ "No se encuentra ninguna DLL\n",
				/* 44	*/ "",
				#else
				/* 43	*/ "No se encuentra ninguna librería\n",
				/* 44	*/ "Directorio %s/ no encontrado\n",
				#endif
				/* 45	*/ "Error al cargar librería %s\n",
				#ifdef _WIN32
				/* 46	*/ "No puedo cargar %s\n",
				/* 47	*/ "No se encuentra ExportaFuncs en %s - DLL no valida\n",
				#else
				/* 46	*/ "No puedo cargar %s (%s)\n",
				/* 47	*/ "No se encuentra ExportaFuncs en %s - Librería no válida (%s)\n",
				#endif
				/* 48	*/ "    -n, --news      Muestra las últimas noticias de eDivCentral.com y sale\n",
				/* 49	*/ "Error: no se ha configurado un servidor de noticias en ediv.cfg\n",
				/* 50	*/ "Error: el fichero recibido no es del tipo esperado\n"
			};
			return msg[id_cadena];
		}

	case 1:	/* italiano */
		{
			static char* msg[] = {
				/* 0	*/ " **** Compilatore eDIV " VERSION " ****\nCopyleft (c) 2000-2002 Sion, Ltd.\nhttp://www.edivcentral.com\n",
				/* 1	*/ "ATTENZIONE: Questa versione di eDIV è incompleta.\nPer favore informa su qualunque bug a: bugs@edivcentral.com\n",
				/* 2	*/ "Guida opzioni di comando:\n\n",
				/* 3	*/ "%s [opzioni] archivio.prg [eseguibile]\n\n",
				/* 4	*/ "Lista opzioni:\n",
				/* 5	*/ "    -h, --help      Mostra questa guida ed esce\n",
				/* 6	*/ "    -d, --debug     Genera informazioni addizionali di depurazione\n",
				/* 7	*/ "    -l, --list      Salva il listato EML come <programma.eml>\n",
				/* 8	*/ "    -t, --table     Salva la tabella oggetti come <programma.tab>\n",
				/* 9	*/ "    -c, --check     Non genera l'eseguibile (per comprovare sintassi, etc.)\n",
				/* 10	*/ "    -s, --system    Sistema Operativo di destinazione (per difetto %s)\n",
				/* 11	*/ "    -v, --version   Mostra la versione ed esce\n\n",
				/* 12	*/ "Parametro --system: deve indicare il sistema di destinazione\nConsulta la documentazione per ottenere aiuto\n",
				/* 13	*/ "Parametro errato: %s\nUsa l'opzione -h per visualizzare la guida\n",
				/* 14	*/ "Parametro -s: deve indicare il sistema di destinazione\nConsulta la documentazione per ottenere aiuto\n",
				/* 15	*/ "Parametro errato: -%c\nUsa l'opzione -h per visualizzare la guida\n",
				/* 16	*/ "Non è stato specificato l'archivio\nUsa l'opzione -h per visualizzare la guida\n",
				/* 17	*/ "edivc: Errore aprendo l'archivio %s.\n",
				/* 18	*/ "edivc: Compilando: %s\n",
				/* 19	*/ "edivc: ERRORE: Non si incontra %s\n",
				/* 20	*/ "edivc: ERRORE: Formato incorretto di %s\n",
				/* 21	*/ "edivc: Interrotto (%d avvertenze)\n",
				/* 22	*/ "edivc: Completato con successo (%d avvertenze)\n",
				/* 23	*/ "edivc: Uscita con errori\n",
				/* 24	*/ "edivc: ERRORE: Memoria insufficiente\n",
				/* 25	*/ "%s:%d:%d: ERRORE %d: ",
				/* 26	*/ "%s:%d: Avvertenza: ",
				/* 27	*/ "Precompilando...\n",
				/* 28	*/ "Compilando...\n",
				/* 29	*/ "Errore creando l'archivio d'intercambio\n",
				/* 30	*/ "Generando listati...\n",
				/* 31	*/ "Salvando eseguibile...\n",
				/* 32	*/ "Salvando informazioni di depurazione...\n",
				/* 33	*/ "Errore scrivendo l'eseguibile\n",
				/* 34	*/ "Compilazione completata.\n",
				/* 35	*/ "%s: ERRORE: ",
				/* 36	*/ "Errore creando l'archivio %s\n",
				/* 37	*/ "Listato oggetti di %s - eDIV versione " VERSION "\n\n",
				/* 38	*/ "\tblocco(%u), precedente(%u)\n",
				/* 39	*/ "Codice EML di %s - eDIV versione " VERSION "\n",
				/* 40	*/ "edivc: ERRORE: %s\n\n",
				/* 41	*/ "edivc: %s (linea %d) ERRORE: %s\n\n",
				/* 42	*/ "edivc: Interrotto - Sono stati trovati errori\n",
				#ifdef _WIN32
				/* 43	*/ "Non si trova nessuna DLL\n",
				/* 44	*/ "",
				#else
				/* 43	*/ "Non si trova nenssuna libreria\n",
				/* 44	*/ "Percorso %s/ non trovato\n",
				#endif
				/* 45	*/ "Errore nel caricare la libreria %s\n",
				#ifdef _WIN32
				/* 46	*/ "Non posso caricare %s\n",
				/* 47	*/ "Non si trova ExportaFuncs in %s - DLL non valida\n",
				#else
				/* 46	*/ "Non posso caricare %s (%s)\n",
				/* 47	*/ "Non si trova ExportaFuncs in %s - Libreria non valida (%s)\n",
				#endif
				/* 48	*/ "    -n, --news      Mostra le ultime notizie di eDivCentral.com ed esce\n",
				/* 49	*/ "Errore: non è stato configurato un server di notizie in ediv.cfg\n",
				/* 50	*/ "Errore: il file ricevuto non è del tipo atteso\n"
			};	
			return msg[id_cadena];
		}

	case 2:	/* portugués */
		{
			static char* msg[] = {
				/* 0	*/ " **** Compilador eDIV " VERSION " ****\nCopyleft (c) 2000-2002 Sion, Ltd.\nhttp://www.edivcentral.com\n",
				/* 1    */ "ATENÇÃO: Esta versão de eDIV está inacabada.\nPor favor informe-nos de qualquer bug a: bugs@edivcentral.com\n",
				/* 2    */ "Ajuda de opções de comando:\n\n",
				/* 3    */ "%s [opções] arquivo.prg [executavel]\n\n",
				/* 4    */ "Lista de opções:\n",
				/* 5    */ "    -h, --help      Mostra esta ajuda e sai\n",
				/* 6    */ "    -d, --debug     Gera informação adicional de depuração\n",
				/* 7    */ "    -l, --list      Guarda o listado EML como <programa.eml>\n",
				/* 8    */ "    -t, --table     Guarda a tabela de objectos como <programa.tab>\n",
				/* 9    */ "    -c, --check     Não gera um executavel (para comprovar sintaxe, etc.)\n",
				/* 10   */ "    -s, --system    Sistema Operativo de destino (por defeito %s)\n",
				/* 11   */ "    -v, --version   Mostra versão e sai\n\n",
				/* 12   */ "Parametro --system: deve indicar um sistema de destino\nConsulte a documentação para obter ajuda\n",
				/* 13   */ "Parametro erróneo: %s\nUse a opção -h para ver a ajuda\n",
				/* 14   */ "Parametro -s: deve indicar um sistema de destino\nConsulte a documentação para obter ajuda\n",
				/* 15   */ "Parametro erróneo: -%c\nUse a opção -h para ver a ajuda\n",
				/* 16   */ "Não foi especificado um arquivo\nUse a opção -h para ver a ajuda\n",
				/* 17   */ "edivc: Erro ao abrir arquivo %s.\n",
				/* 18	*/ "edivc: Compilando: %s\n",
				/* 19   */ "edivc: ERROR: Não se enconntra %s\n",
				/* 20	*/ "edivc: ERROR: Formato incorrecto de %s\n",
				/* 21   */ "edivc: Interrompido (%d advertencias)\n",
				/* 22   */ "edivc: Finalizado con exito (%d advertencias)\n",
				/* 23   */ "edivc: Saida com erros\n",
				/* 24   */ "edivc: ERROR: Memória insuficiente\n",
				/* 25	*/ "%s:%d:%d: ERROR %d: ",
				/* 26   */ "%s:%d: Advertencia: ",
				/* 27	*/ "Precompilando...\n",
				/* 28	*/ "Compilando...\n",
				/* 29	*/ "Erro ao criar arquivo de intercambio\n",
				/* 30	*/ "Generando listados...\n",
				/* 31	*/ "Guardando executavel...\n",
				/* 32	*/ "Guardando informação de depuração...\n",
				/* 33	*/ "Erro ao escrever executavel\n",
				/* 34	*/ "Compilação finalizada.\n",
				/* 35	*/ "%s: ERRO: ",
				/* 36	*/ "Erro ao criar arquivo %s\n",
				/* 37	*/ "Listado de objectos de %s - eDIV versão " VERSION "\n\n",
				/* 38	*/ "\tbloco(%u), anterior(%u)\n",
				/* 39	*/ "Código EML de %s - eDIV versão " VERSION "\n",
				/* 40	*/ "edivc: ERRO: %s\n\n",
				/* 41	*/ "edivc: %s (linha %d) ERRO: %s\n\n",
				/* 42	*/ "edivc: Interrompido - encontraram-se erros\n",
				#ifdef _WIN32
				/* 43	*/ "Não se encontra nenhuma DLL\n",
				/* 44	*/ "",
				#else
				/* 43	*/ "Não se encontra nenhuma biblioteca\n",
				/* 44	*/ "Directorio %s/ não foi encontrado\n",
				#endif
				/* 45	*/ "Erro ao carregar biblioteca %s\n",
				#ifdef _WIN32
				/* 46	*/ "Não podo carregar %s\n",
				/* 47	*/ "Não se encontra ExportaFuncs em %s - DLL invalida\n",
				#else
				/* 46	*/ "Não se pode carregar %s (%s)\n",
				/* 47	*/ "Não se encontra ExportaFuncs em %s - Biblioteca invalida (%s)\n",
				#endif

				/* FIXME: revisar/traducir esto */
				/* 48	*/ "    -n, --news      Mostra a última notícia de eDivCentral.com e sai\n",
				/* 49	*/ "Error: no se ha configurado un servidor de noticias en ediv.cfg\n",
				/* 50	*/ "Error: el fichero recibido no es del tipo esperado\n"
			};
			return msg[id_cadena];
		}

	case 3:	/* inglés */
		{
			static char* msg[] = {
				/* 0	*/ " **** eDIV Compiler " VERSION " ****\nCopyleft (c) 2000-2002Sion, Ltd.\nhttp://www.edivcentral.com\n",
				/* 1	*/ "WARNING: This version of eDIV is unfinished.\nPlease report any bug to: bugs@edivcentral.com\n",
				/* 2	*/ "Command line help:\n\n",
				/* 3	*/ "%s [options] file.prg [executable]\n\n",
				/* 4	*/ "Options list:\n",
				/* 5	*/ "    -h, --help      Shows this help and exits\n",
				/* 6	*/ "    -d, --debug     Generates additional debug information\n",
				/* 7	*/ "    -l, --list      Saves the EML listing as <program.eml>\n",
				/* 8	*/ "    -t, --table     Saves the object table as <program.tab>\n",
				/* 9	*/ "    -c, --check     Doesn't create an executable (for syntax checking, etc)\n",
				/* 10	*/ "    -s, --system    Target Operating System (by default %s)\n",
				/* 11	*/ "    -v, --version   Shows program version and exits\n\n",
				/* 12	*/ "Parameter --system: you must indicate a target system\nConsult the documentation in order to obtain help\n",
				/* 13	*/ "Erroneous parameter: %s\nUse -h command-line option to view the help screen\n",
				/* 14	*/ "Parameter -s: you must indicate a target system\nConsult the documentation in order to obtain help\n",
				/* 15	*/ "Erroneous parameter: -%c\nUse -h command-line option to view the help screen\n",
				/* 16	*/ "You didn't specified an input file\nUse -h command-line option to view the help screen\n",
				/* 17	*/ "edivc: Error opening file %s.\n",
				/* 18	*/ "edivc: Compiling: %s\n",
				/* 19	*/ "edivc: ERROR: File %s not found\n",
				/* 20	*/ "edivc: ERROR: Incorrect file format of %s\n",
				/* 21	*/ "edivc: Break (%d warnings)\n",
				/* 22	*/ "edivc: Finished succesfully (%d warnings)\n",
				/* 23	*/ "edivc: Exit with errors\n",
				/* 24	*/ "edivc: ERROR: Out of memory\n",
				/* 25	*/ "%s:%d:%d: ERROR %d: ",
				/* 26	*/ "%s:%d: Warning: ",
				/* 27	*/ "Precompiling...\n",
				/* 28	*/ "Compiling...\n",
				/* 29	*/ "Error creating swap file\n",
				/* 30	*/ "Generating listings...\n",
				/* 31	*/ "Writing executable...\n",
				/* 32	*/ "Writing debug information...\n",
				/* 33	*/ "Error writing executable\n",
				/* 34	*/ "Compilation finished.\n",
				/* 35	*/ "%s: ERROR: ",
				/* 36	*/ "Error creating file %s\n",
				/* 37	*/ "Object listing of %s - eDIV version " VERSION "\n\n",
				/* 38	*/ "\tblock(%u), previous(%u)\n",
				/* 39	*/ "EML code of %s - eDIV version " VERSION "\n",
				/* 40	*/ "edivc: ERROR: %s\n\n",
				/* 41	*/ "edivc: %s (line %d) ERROR: %s\n\n",
				/* 42	*/ "edivc: Break - errors found\n",
				#ifdef _WIN32
				/* 43	*/ "No DLL's found\n",
				/* 44	*/ "",
				#else
				/* 43	*/ "No libraries found\n",
				/* 44	*/ "Directoriy %s/ not found\n",
				#endif
				/* 45	*/ "Error loading library %s\n",
				#ifdef _WIN32
				/* 46	*/ "Can't load %s\n",
				/* 47	*/ "ExportaFuncs not found in %s - DLL not valid\n",
				#else
				/* 46	*/ "Can't load %s (%s)\n",
				/* 47	*/ "ExportaFuncs not found in %s - Library not valid (%s)\n",
				#endif
				/* 48	*/ "    -n, --news      Shows last news from eDivCentral.com and exits\n",
				/* 49	*/ "Error: news server is not set up in ediv.cfg\n",
				/* 50	*/ "Error: received file if of an unexpected format\n"
			};
			return msg[id_cadena];
		}

	case 4: /* catalán */
		{
			static char* msg[] = {
				/* 0	*/ " **** Compilador eDIV " VERSION " ****\nCopyleft (c) 2000-2002 Sion, Ltd.\nhttp://www.edivcentral.com\n",
				/* 1	*/ "ATENCIÓ: Aquesta versió d' eDIV està sense acabar.\nSi us plau, informi de qualsevol bug a: bugs@edivcentral.com\n",
				/* 2	*/ "Ajuda d' opcions de comand:\n\n",
				/* 3	*/ "%s [opcions] arxiu.prg [executable]\n\n",
				/* 4	*/ "Llista d' opcions:\n",
				/* 5	*/ "    -h, --help      Mostra aquesta ajuda i en sort\n",
				/* 6	*/ "    -d, --debug     Genera informació adicional de depurat\n",
				/* 7	*/ "    -l, --list      Guarda el llistat EML com a <programa.eml>\n",
				/* 8	*/ "    -t, --table     Guarda la tabla d' objectes com a <programa.tab>\n",
				/* 9	*/ "    -c, --check     No genera un executable (per comprovar sintaxi, etc.)\n",
				/* 10	*/ "    -s, --system    Sistema Operatiu destí (per defecte %s)\n",
				/* 11	*/ "    -v, --version   Mostra versió i en sort\n\n",
				/* 12	*/ "Paràmetre --system: ha d' indicar un sistema destí\nConsulti la documentació per a obtenir ajuda\n",
				/* 13	*/ "Paràmetre erroni: %s\nUtilitze la opció -h per a veure l' ajuda\n",
				/* 14	*/ "Paràmetre -s: ha d' indicar un sistema destí\nConsulti la documentació per a obtenir ajuda\n",
				/* 15	*/ "Paràmetre erroni: -%c\nUtilitze la opció -h per a veure l' ajuda\n",
				/* 16	*/ "No s'ha especificat un arxiu\nUtilitze la opció -h per a veure l' ajuda\n",
				/* 17	*/ "edivc: Error obrint l' arxiu %s.\n",
				/* 18	*/ "edivc: Compilant: %s\n",
				/* 19	*/ "edivc: ERROR: No es troba %s\n",
				/* 20	*/ "edivc: ERROR: Format incorrecte de %s\n",
				/* 21	*/ "edivc: Interromput (%d advertències)\n",
				/* 22	*/ "edivc: Finalitzat amb èxit (%d advertències)\n",
				/* 23	*/ "edivc: Sortida amb errors\n",
				/* 24	*/ "edivc: ERROR: Memòria insuficient\n",
				/* 25	*/ "%s:%d:%d: ERROR %d: ",
				/* 26	*/ "%s:%d: Advertència: ",
				/* 27	*/ "Precompilant...\n",
				/* 28	*/ "Compilant...\n",
				/* 29	*/ "Error creant arxiu d' intercanvi\n",
				/* 30	*/ "Generant llistats...\n",
				/* 31	*/ "Guardant executable...\n",
				/* 32	*/ "Guardant informació de depurat...\n",
				/* 33	*/ "Error escriguent executable\n",
				/* 34	*/ "Compilació finalitzada.\n",
				/* 35	*/ "%s: ERROR: ",
				/* 36	*/ "Error creant arxiu %s\n",
				/* 37	*/ "Llistado d' objectes de %s - eDIV versió " VERSION "\n\n",
				/* 38	*/ "\tbloque(%u), anterior(%u)\n",
				/* 39	*/ "Codi EML de %s - eDIV versió " VERSION "\n",
				/* 40	*/ "edivc: ERROR: %s\n\n",
				/* 41	*/ "edivc: %s (línia %d) ERROR: %s\n\n",
				/* 42	*/ "edivc: Interromput - s' han trobat errors\n",
				#ifdef _WIN32
				/* 43	*/ "No es troba cap DLL\n",
				/* 44	*/ "",
				#else
				/* 43	*/ "No es troba cap llibreria\n",
				/* 44	*/ "Directori %s/ no trobat\n",
				#endif
				/* 45	*/ "Error cargant llibrería %s\n",
				#ifdef _WIN32
				/* 46	*/ "No es pot cargar %s\n",
				/* 47	*/ "No es troba ExportaFuncs en %s - DLL no vàlida\n",
				#else
				/* 46	*/ "No es pot cargar %s (%s)\n",
				/* 47	*/ "No es troba ExportaFuncs en %s - Llibreria no vàlida (%s)\n",
				#endif
				/* 48	*/ "    -n, --news Mostra les últimes notícies d' eDIVCentral i en surt\n",
				/* 49	*/ "Error: No s' ha configurat un servidor de notícies en ediv.cfg\n",
				/* 50	*/ "Error: l' arxiu rebut no és del tipus esperat\n"
			};
			return msg[id_cadena];
		}
	case 5: /* euskera */
		{
			static char* msg[] = {
				/* 0	*/ " **** eDIV konpiladorea " VERSION " ****\nCopyleft (c) 2000-2002 Sion, Ltd.\nhttp://www.edivcentral.com\n",
				/* 1	*/ "KONTUZ: eDIV bertsio hau ez dago bukaturik.\nMesedez eman berri edozein akatzi buruz helbide honetan: bugs@edivcentral.com\n",
				/* 2	*/ "Komandoei buruzko laguntza:\n\n",
				/* 3	*/ "%s [aukerak] archivo.prg [exekutagarria]\n\n",
				/* 4	*/ "Aukeren zerrenda:\n",
				/* 5	*/ "    -h, --help      Laguntza hau erakuste du eta irteten da\n",
				/* 6	*/ "    -d, --debug     Depurazioari buruzko gehigarrizko informazioa erakusten du\n",
				/* 7	*/ "    -l, --list      EML listatua <programa.eml> moduan grabatzen du\n",
				/* 8	*/ "    -t, --table     Objektuen taula <programa.tab> moduan grabatzen du\n",
				/* 9	*/ "    -c, --check     Ez du exekutagarririk egiten (sintaxia zihurtatzeko eta abar.)\n",
				/* 10	*/ "    -s, --system    Helburuko sistema eragilea (balio lehentsia %s)\n",
				/* 11	*/ "    -v, --version   Bertsioa erakusten du eta irteten da\n\n",
				/* 12	*/ "--system parametroa: sistema bat adierazi behar duzu\nkontsultatu dokumentazioa laguntza jasotzeko\n",
				/* 13	*/ "Okerreko parametroa: %s\nerabili -h aukera laguntza jasotzeko\n",
				/* 14	*/ "-s parametroa: sistema bat adierazi behar duzu\nkontsultatu dokumentazioa laguntza jasotzeko\n",
				/* 15	*/ "Okerreko parametroa: %s\nerabili -h aukera laguntza jasotzeko\n",
				/* 16	*/ "Ez duzu fitxategi bat adierazi\nErabili -h aukera laguntza jasotzeko\n",
				/* 17	*/ "edivc: Akatz bat sortu da %s fitxategia irekitzerakoan.\n",
				/* 18	*/ "edivc: Konpilatzen: %s\n",
				/* 19	*/ "edivc: AKATZA: Ezin da %s aurkitu\n",
				/* 20	*/ "edivc: AKATZA: %s-ko okerreko formatua\n",
				/* 21	*/ "edivc: Gelditurki (%d abisu)\n",
				/* 22	*/ "edivc: Ondo bukatuta (%d abisu)\n",
				/* 23	*/ "edivc: Akaztuneko irteera\n",
				/* 24	*/ "edivc: AKATZA: Ez dago behar beste memoria\n",
				/* 25	*/ "%s:%d:%d: AKATZA %d: ",
				/* 26	*/ "%s:%d: Abisua: ",
				/* 27	*/ "Prekonpilatzen...\n",
				/* 28	*/ "Konpilatzen...\n",
				/* 29	*/ "Akatz bat sortu da memoria virtualeko fitxategia sortzerakoan\n",
				/* 30	*/ "Listatuak sortzen...\n",
				/* 31	*/ "Exekutagarria sortzen...\n",
				/* 32	*/ "Depurazio informazioa grabatzen...\n",
				/* 33	*/ "Akatz bar sortu da exekutagarria sortzerakoan\n",
				/* 34	*/ "Konpilazioa bukaturik.\n",
				/* 35	*/ "%s: AKATZA: ",
				/* 36	*/ "Akatz bar sortu da %s fitxategia sortzerakoan\n",
				/* 37	*/ "%s-ren objektu zerrenda - eDIV bertsio " VERSION "\n\n",
				/* 38	*/ "\tbloque(%u), anterior(%u)\n",
				/* 39	*/ "%s-ko EML kodea - eDIV bertsio " VERSION "\n",
				/* 40	*/ "edivc: ERROR: %s\n\n",
				/* 41	*/ "edivc: %s (%d lerro) AKATZA: %s\n\n",
				/* 42	*/ "edivc: Gelditurik - akatzak aurkitu izan dira\n",
				#ifdef _WIN32
				/* 43	*/ "Ezin da DLL-rik aurkitu\n",
				/* 44	*/ "",
				#else
				/* 43	*/ "Ezin da liburutegirik aurkitu\n",
				/* 44	*/ "Ezin da %s/ direktorioa aurkitu\n",
				#endif
				/* 45	*/ "Akatz bat sortu da %s liburutegia kargatzerakoan\n",
				#ifdef _WIN32
				/* 46	*/ "Ezin dut %s kargatu\n",
				/* 47	*/ "%s-en ez da ExportaFuncs aurkitzen - Okerreko DLL\n",
				#else
				/* 46	*/ "Ezin da %s (%s) kargatu\n",
				/* 47	*/ "%s-en ez da ExportaFuncs aurkitzen - Okerreko DLL\n",
				#endif
				/* 48	*/ "    -n, --news      eDivCentral.com-eko asken berriak erakusten du eta irteten da\n",
				/* 49	*/ "Akatza: ez da berri zerbitzaririk prestatu ediv.cfg fitxategian\n",
				/* 50	*/ "Akatza: jasotako fitxategia ez da itxarotako motakoa\n"
			};
			return msg[id_cadena];
		}
	}
	return "<please translate this>";
}
