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
 * 0 - ESPAÑOL (es)
 * Autores: eDIV Team, Hammer Technologies
 */

/*
 * Este archivo puede usarse como archivo de plantilla para hacer nuevas
 * traducciones. Si quieres que eDIV soporte un nuevo idioma, traduce este
 * archivo y envíanoslo por email.
 * De momento eDIV no soporta Unicode, pero si tu idioma lo requiere,
 * avísanos y nos ocuparemos de que lo soporte :)
 *
 * Si editas este archivo, por favor:
 * - Puedes utilizar un editor de Linux, Windows o Mac, pero NO uses un
 *   editor de MS-DOS (como EDIT.COM), ya que destrozarás todos los acentos
 *   y caracteres especiales.
 * - Respeta la numeración de los mensajes.
 * - Respeta las comillas, los símbolos reservados de C (%s, \n, etc),
 *   las instrucciones de preprocesador (#ifdef, #else, etc)...
 * - Ten especial cuidado con las comas que aparecen al final de todas las
 *   líneas, EXCEPTO en la última línea de cada bloque.
 */

/*
 * Errores de compilación
 */

#if TRANSLATE == TRANSLATE_ERROR

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
/* 73	*/	"ELSEIF no aparece dentro de un bloque IF" /* SIN COMA */


/*
 * Advertencias
 */

#elif TRANSLATE == TRANSLATE_WARNING

/* 0	*/	"",
/* 1	*/	"SETUP_PROGRAM: Sentencia obsoleta",
/* 2	*/	"IMPORT: Sintaxis antigua, se ignora" /* SIN COMA */


/*
 * Errores de DLL
 */

#elif TRANSLATE == TRANSLATE_DLL_ERROR

/* 1	*/ "Sobrepasado buffer de nombres de objeto",
/* 2	*/ "Objeto '%s': El nombre no es nuevo",
/* 3	*/ "Demasiados objetos declarados",
/* 4	*/ "Función '%s': El nombre no es nuevo",
/* 5	*/ "Error declarando objeto '%s'",
/* 6	*/ "Objeto '%s': Declaración ilegal dentro de struct",
/* 7	*/ "Objeto '%s': Declaración de un miembro fuera de struct",
/* 8	*/ "Tamaño ilegal del string '%s'",
/* 9	*/ "EndStruct sin Struct",
/* 10	*/ "Una estructura debe contener al menos un elemento" /* SIN COMA */


/*
 * Errores de ltlex.def
 */

#elif TRANSLATE == TRANSLATE_LTLEX_ERROR

/* 0	*/ "",
/* 1	*/ "Archivo ltlex.def no encontrado",
/* 2	*/ "Se esperaba un valor hexadecimal (&00..&FF)",
/* 3	*/ "Demasiados símbolos definidos",
/* 4	*/ "Símbolo no válido" /* SIN COMA */


/*
 * Mensajes del compilador
 */

#elif TRANSLATE == TRANSLATE_COMPILER

/* 0	*/ " **** Compilador eDIV " VERSION " ****\nCopyleft (c) 2000-2003 Sion, Ltd.\nhttp://ediv.divsite.net\n",
/* 1	*/ "ATENCION: Esta version de eDIV esta inacabada.\nPor favor informe de cualquier bug a: ediv@divsite.net\n",
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
/* 50	*/ "Error: el fichero recibido no es del tipo esperado\n" /* SIN COMA */


/*
 * Mensajes del stub
 */

#elif TRANSLATE == TRANSLATE_STUB

/* 0	*/	"ERROR %d: ",
/* 1	*/	"eDIV: Error en ejecución",
/* 2	*/	"eDIV: Error crítico",
/* 3	*/	"Error en %s: ",
/* 4	*/	"eDIV: Error en librería dinámica" /* SIN COMA */


/*
 * Errores de tiempo de ejecución del stub
 */

#elif TRANSLATE == TRANSLATE_RUNTIME_ERROR

/* 100 */ "No hay memoria suficiente para ejecutar el programa.",
/* 101 */ "Llamada a key() con un código de tecla inexistente.",
/* 102 */ "No se pudo cargar la paleta, archivo no encontrado.",
/* 103 */ "Se intentó cargar un archivo de paleta inválido.",
/* 104 */ "Demasiados ficheros FPG abiertos a la vez.",
/* 105 */ "No se pudo cargar el fichero, archivo no encontrado.",
/* 106 */ "Se intentó cargar un fichero FPG no válido.",
/* 107 */ "Número de scroll inválido (debe estar entre 0 y 9).",
/* 108 */ "Número de región inválido (fuera de rango).",
/* 109 */ "Código de fichero inválido.",
/* 110 */ "Código de gráfico inválido.",
/* 111 */ "Se intentó usar un fichero FPG antes de cargarlo.",
/* 112 */ "Se necesita un código de gráfico para el scroll.",
/* 113 */ "Se han cargado demasiados fuentes de letras.",
/* 114 */ "No se pudo cargar el fuente, archivo no encontrado.",
/* 115 */ "Se intentó cargar un archivo de fuente no válido.",
/* 116 */ "Código identificador de fuente de letras inválido.",
/* 117 */ "Código de centrado de texto inválido.",
/* 118 */ "Hay demasiados textos activos en el programa.",
/* 119 */ "Indentificador de texto no válido.",
/* 120 */ "Región de pantalla definida incorrectamente.",
/* 121 */ "Se intentó utilizar un gráfico que no existe.",
/* 122 */ "Se intentó escribir un bloque fuera de la memoria.",
/* 123 */ "No se pudo abrir el archivo para escritura.",
/* 124 */ "No se pudo escribir el archivo (ver espacio en disco).",
/* 125 */ "Se intentó leer un bloque fuera de la memoria.",
/* 126 */ "No se pudo abrir el archivo para lectura.",
/* 127 */ "No se pudo leer el archivo.",
/* 128 */ "No se pudo cargar el efecto de sonido.",
/* 129 */ "No se pudo tocar el efecto de sonido solicitado.",
/* 130 */ "No se pudo iniciar la animación solicitada.",
/* 131 */ "Número de modo 7 inválido (debe estar entre 0 y 9).",
/* 132 */ "Se necesita un código de gráfico para el modo 7.",
/* 133 */ "Número de punto de control inválido.",
/* 134 */ "Número de botón incorrecto (debe estar entre 0 y 3).",
/* 135 */ "Número de eje incorrecto (debe estar entre 0 y 3).",
/* 136 */ "Dirección de la tabla de paleta fuera de la memoria.",
/* 137 */ "No se pueden comprobar regiones en gráficos de m-7.",
/* 138 */ "Código de información gráfica incorrecto.",
/* 139 */ "No se pueden comprobar colisiones en gráficos de m-7.",
/* 140 */ "Acceso fuera de rango.",
/* 141 */ "Se intentó acceder a un proceso inválido.",
/* 142 */ "El proceso parece bloqueado, esperando FRAME.",
/* 143 */ "No se pudo cargar el mapa, archivo no encontrado.",
/* 144 */ "Se intentó cargar un archivo de mapa inválido.",
/* 145 */ "Se ha intentado hacer una división entre cero.",
/* 146 */ "El tamaño de la región es incorrecto.",
/* 147 */ "No se pudo cargar FLI/FLC, archivo no encontrado.",
/* 148 */ "Valor de timeout incorrecto (debe ser entre 1 y 60).",
/* 149 */ "Número de jugadores incorrecto (debe ser entre 2 y 8).",
/* 150 */ "Sólo se pueden enviar datos globales.",
/* 151 */ "Tamaño de casilla inválido (debe ser entre 1 y 256).",
/* 152 */ "Mapa de búsqueda no válido (desde 1x1 hasta 128x128).",
/* 153 */ "Tamaño de mapa incorrecto (de 1x1 a 32768x32768).",
/* 154 */ "Número de color incorrecto (debe ser de 0 a 255).",
/* 155 */ "El centro está fuera de los límites del gráfico.",
/* 156 */ "El objeto se encuentra fuera del mapa y sera eliminado.",
/* 157 */ "Textura del mapa no encontrada.",
/* 158 */ "El tamaño de la textura es incorrecto.",
/* 159 */ "No se pudo cargar el mundo, archivo no encontrado.",
/* 160 */ "No se pudo cargar el mundo, archivo no válido.",
/* 161 */ "El número de bandera no es válido.",
/* 162 */ "Dispositivo inválido (sólo 1-IPX,2-SERIAL,3-MODEM).",
/* 163 */ "Tamaño de paquete inválido (sólo de 1 a 199).",
/* 164 */ "La cadena de texto destino no es válida.",
/* 165 */ "Se intentó acceder a un puntero nulo.",
/* 166 */ "El modo de acceso al archivo no es válido.",
/* 167 */ "No se pudo cargar el módulo.",
/* 168 */ "Demasiados errores ignorados.",
/* 169 */ "Demasiados archivos abiertos.",
/* 170 */ "Identificador de archivo (handle) no válido.",
/* 171 */ "Valor fuera de rango.",
/* 172 */ "Demasiados objetos gráficos visualizados.",
/* 173 */ "Tipo de objeto gráfico no conocido.",
/* 174 */ "Porcentaje de tinta fuera de rango (0..15).",
/* 175 */ "Número de objeto gráfico no válido.",
/* 176 */ "Error variable de vpe: Textura de mapa [a] no encontrada en fichero [b].",
/* 177 */ "Textura de mapa.",
/* 178 */ "no encontrada en fichero.",
/* 179 */ "Se realizaron demasiadas peticiones de memoria dinámica.",
/* 180 */ "El puntero no se corresponde con ninguno de los bloques reservados.",
/* 181 */ "No se pueden reservar bloques de menos de una posición.",
/* 182 */ "Se intentó (des)encriptar un bloque fuera de la memoria.",
/* 183 */ "No se puede cargar otra paleta cuando está forzada la actual.",
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
/* 200 */ "Archivo con datos corruptos, no se pudo cargar." /* SIN COMA */


/*
 * Errores críticos del stub
 */

#elif TRANSLATE == TRANSLATE_CRITICAL_ERROR

/* 0	*/	"",
/* 1	*/	"Error leyendo el código del programa",
/* 2	*/	"Error en el formato de código",
/* 3	*/	"Redefinicion del tipo de proceso",
/* 4	*/	"No se pudo reservar memoria para la pila",
/* 5	*/	"Librería no encontrada: %s",
/* 6	*/	"%s no es una librería válida de eDIV",
/* 7	*/	"No se pudo inicializar SDL",
/* 8	*/	"Demasiados procesos en ejecución",
/* 9	*/	"Se requiere SDL versión %d.%d.%d\nVersión detectada en el sistema: %d.%d.%d" /* SIN COMA */


#endif

/* Fin de fichero */
