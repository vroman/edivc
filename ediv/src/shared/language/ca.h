/*
 * eDiv Compiler
 * Copyleft (C) 2000-2003 Sion Ltd.
 * Copyleft (C) 2005 eDIV Team
 * http://ediv.divsite.net
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
 * ARCHIVO DE IDIOMA DE EDIV
 * 4 - CATALÁN (ca)
 * Autora: Belén Albeza (BenKo)
 */


/*
 * Errores de compilación
 */

#if TRANSLATE == TRANSLATE_ERROR

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
/* 73	*/ "ELSEIF no apareix dins d' un bloc IF" /* SIN COMA */


/*
 * Advertencias
 */

#elif TRANSLATE == TRANSLATE_WARNING

/* 0	*/ "",
/* 1	*/ "SETUP_PROGRAM: Característica desfasada",
/* 2	*/ "IMPORT: Sintaxi antiga, s' ignora" /* SIN COMA */


/*
 * Errores de DLL
 */

#elif TRANSLATE == TRANSLATE_DLL_ERROR

/* 1	*/ "Sobrepassat buffer de noms d' objecte",
/* 2	*/ "Objecte '%s': El nom no és nou",
/* 3	*/ "Massa objectes declarats",
/* 4	*/ "Funció '%s': El nom no és nou",
/* 5	*/ "Error declarant objecte '%s'",
/* 6	*/ "Objecte '%s': Declaració il·legal dins d' struct",
/* 7	*/ "Objecte '%s': Declaració d' un membre fora d' struct",
/* 8	*/ "Tamany il·legal de l' string '%s'",
/* 9	*/ "EndStruct sense Struct",
/* 10	*/ "Una estructura ha de contenir almenys un element" /* SIN COMA */


/*
 * Errores de ltlex.def
 */

#elif TRANSLATE == TRANSLATE_LTLEX_ERROR

/* 0	*/ "",
/* 1	*/ "Arxiu ltlex.def no trobat",
/* 2	*/ "S' esperava un valor hexadecimal (&00..&FF)",
/* 3	*/ "Massa símbols definits",
/* 4	*/ "Símbol no vàlid" /* SIN COMA */


/*
 * Mensajes del compilador
 */

#elif TRANSLATE == TRANSLATE_COMPILER

/* 0	*/ " **** Compilador eDIV " VERSION " ****\n" COPYRIGHT "\n" WEBSITE "\n",
/* 1	*/ "ATENCIÓ: Aquesta versió d' eDIV està sense acabar.\nSi us plau, informi de qualsevol bug a: ediv@divsite.net\n",
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
/* 50	*/ "Error: l' arxiu rebut no és del tipus esperat\n" /* SIN COMA */


/*
 * Mensajes del stub
 */

#elif TRANSLATE == TRANSLATE_STUB

/* FIXMI: traducir esto */
/* 0	*/	"ERROR %d: ",
/* 1	*/	"eDIV: Error en ejecución",
/* 2	*/	"eDIV: Error crítico",
/* 3	*/	"Error en %s: ",
/* 4	*/	"eDIV: Error en librería dinámica" /* SIN COMA */


/*
 * Errores de tiempo de ejecución del stub
 */

#elif TRANSLATE == TRANSLATE_RUNTIME_ERROR

/* 100 */ "No hi ha memòria suficient per a executar el programa.",
/* 101 */ "Crida a key() amb un codi de tecla inexistent.",
/* 102 */ "No es va poder carregar la paleta, arxiu no trobat.",
/* 103 */ "Es va intentar carregar un arxiu de paleta invàlid.",
/* 104 */ "Massa arxius FPG oberts al mateix temps.",
/* 105 */ "No es va poder carregar l' arxiu, arxiu no trobat.",
/* 106 */ "Es va intentar carregar un arxiu FPG no vàlid.",
/* 107 */ "Número d' scroll invàlid (ha d' estar entre 0 y 9).",
/* 108 */ "Número de regió invàlid (fora de rang).",
/* 109 */ "Codi d' arxiu invàlido.",
/* 110 */ "Codi de gràfic invàlid.",
/* 111 */ "Es va intentar usar un arxiu FPG abans de carregar-lo.",
/* 112 */ "Es necesita un codi de gràfic per a l' scroll.",
/* 113 */ "S' han carregat massa fonts de lletres.",
/* 114 */ "No es va poder carregar la font, arxiu no trobat.",
/* 115 */ "Es va intentar carregar un arxiu de font no vàlid.",
/* 116 */ "Codi identificador de font de lletres invàlid.",
/* 117 */ "Codi de centrat de text invàlid.",
/* 118 */ "Hi ha massa textos actius al programa.",
/* 119 */ "Indentificador de text no vàlid.",
/* 120 */ "Regió de pantalla definida incorrectament.",
/* 121 */ "Es va intentar utilitzar un gràfic que no existeix.",
/* 122 */ "Es va intentar escriure un bloc fora de la memòria.",
/* 123 */ "No es va poder obrir l' arxiu per a escritura.",
/* 124 */ "No es va poder escriure l' arxiu (veure espai al disc).",
/* 125 */ "Es va intentar llegir un bloc fora de la memòria.",
/* 126 */ "No es va poder obrir l' arxiu per a lectura.",
/* 127 */ "No es va poder llegir l' arxiu.",
/* 128 */ "No es va poder carregar l' efecte de so.",
/* 129 */ "No es va poder reproduir l' efecte de so sol·licitat.",
/* 130 */ "No es va poder iniciar l' animació sol·licitada.",
/* 131 */ "Número de mode 7 invàlid (ha d' estar entre 0 y 9).",
/* 132 */ "Es necessita un codi de gràfic per al mode 7.",
/* 133 */ "Número de punt de control invàlid.",
/* 134 */ "Número de botó incorrecte (ha d' estar entre 0 y 3).",
/* 135 */ "Número d' eix incorrecte (ha d' estar entre 0 y 3).",
/* 136 */ "Direcció de la tabla de paleta fora de la memòria.",
/* 137 */ "No es poden comprovar regions en gràfics de mode 7.",
/* 138 */ "Codi d' informació gràfica incorrecte.",
/* 139 */ "No es poden comprovar col·lisions en gràfics de mode 7.",
/* 140 */ "Accés fora de rang.",
/* 141 */ "Es va intentar accedir a un procés invàlid.",
/* 142 */ "El procés sembla bloquejat, esperant FRAME.",
/* 143 */ "No es va poder carregar el mapa, arxiu no trobat.",
/* 144 */ "Es va intentar carregar un arxiu de mapa invàlid.",
/* 145 */ "S' ha intentar fer una divisió entre zero.",
/* 146 */ "El tamany de la regió es incorrecte.",
/* 147 */ "No es va poder carregar FLI/FLC, arxiu no trobat.",
/* 148 */ "Valor de timeout incorrecte (ha de ser entre 1 y 60).",
/* 149 */ "Nombre de jugadors incorrecte (ha de ser entre 2 y 8).",
/* 150 */ "Només es poden enviar dades globals.",
/* 151 */ "Tamany de casella invàlid (ha de ser entre 1 y 256).",
/* 152 */ "Map de búsqueda no vàlid (des d' 1x1 fins a 128x128).",
/* 153 */ "Tamany de mapa incorrecte (des d' 1x1 fins a 32768x32768).",
/* 154 */ "Número de color incorrecte (ha de ser ser des de 0 fins a 255).",
/* 155 */ "El centre es troba fora dels límits del gràfic.",
/* 156 */ "L' objecte es troba fora del mapa i serà eliminat.",
/* 157 */ "Textura del mapa no trobada.",
/* 158 */ "El tamany de la textura és incorrecte.",
/* 159 */ "No es va poder carregar el món, arxiu no trobat.",
/* 160 */ "No es va poder carregar el món, arxiu no vàlid.",
/* 161 */ "El número de bandera no és vàlid.",
/* 162 */ "Dispositiu invàlid (només 1-IPX,2-SERIAL,3-MODEM).",
/* 163 */ "Tamany de paquet invàlid (només des d' 1 fins a 199).",
/* 164 */ "La cadena de text destí no és vàlida.",
/* 165 */ "Es va intentar accedir a un punter nul.",
/* 166 */ "El mode d' accés a l' arxiu no és vàlid.",
/* 167 */ "No es va poder carregar el mòdul.",
/* 168 */ "Massa errors ignorats.",
/* 169 */ "Massa arxius oberts.",
/* 170 */ "Identificador d' arxiu (handle) no vàlid.",
/* 171 */ "Valor fora de rang.",
/* 172 */ "Massa objectes gràfics visualitzats.",
/* 173 */ "Tipus d' objecte gràfic no conegut.",
/* 174 */ "Percentatge de tinta fora de rang (0..15).",
/* 175 */ "Número d' objecte gràfic no vàlid.",
/* 176 */ "Error variable de mode 8: Textura de mapa [a] no trobada en arxiu [b].",
/* 177 */ "Textura de map.",
/* 178 */ "no trobada en arxiu.",
/* 179 */ "Es van realitzar massa peticions de memòria dinàmica.",
/* 180 */ "El punter no es correspón amb ningú dels blocs reservats.",
/* 181 */ "No es poden reservar blocs de menys d' una posició.",
/* 182 */ "Es va intentar (des)encriptar un bloc fora de la memòria.",
/* 183 */ "No es pot carregar altra paleta quan està forçada l' actual.",
/* 184 */ "",
/* 185 */ "",
/* 186 */ "",
/* 187 */ "",
/* 188 */ "",
/* 189 */ "",
/* 190 */ "",
/* 191 */ "",
/* 192 */ "",
/* 193 */ "",
/* 194 */ "",
/* 195 */ "",
/* 196 */ "",
/* 197 */ "",
/* 198 */ "",
/* 199 */ "",
/* 200 */ "Arxiu amb dades corrompudes, no es va poder carregar." /* SIN COMA */


/*
 * Errores críticos del stub
 */

#elif TRANSLATE == TRANSLATE_CRITICAL_ERROR

/* 0	*/	"",
/* 1	*/	"Error llegint el codi del programa",
/* 2	*/	"Error en el format de codi",
/* 3	*/	"Redefinició del tipus de procés",
/* 4	*/	"No es va poder reservar memòria per a la pila",
/* 5	*/	"Llibreria no trobada: %s",
/* 6	*/	"%s no és una llibreria vàlida d' eDIV",
/* 7	*/	"No se pudo inicializar SDL",	// FIXME: traducir esto
/* 8	*/	"Demasiados procesos en ejecución", // FIXME: traducir esto
/* 9	*/	"Se requiere SDL versión %d.%d.%d\nVersión detectada en el sistema: %d.%d.%d" /* SIN COMA */ // FIXME: traducir esto


#endif

/* Fin de fichero */
