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
 * EDIV LANGUAGE FILE
 * 3 - ENGLISH (en)
 * Authors: eDIV Team, FastTrak
 */

/*
 * This file can be used as a template to make new translation files. If you
 * want eDIV to support a new language, translate this file and send it to us
 * by email.
 * eDIV doesn't support Unicode yet, but if your language requires it, tell us
 * and we will implement it :)
 *
 * If you're going to edit this file, please:
 * - You can use a Linux, Windows or Mac editor, but DON'T use a MS-DOS
 *   program (like EDIT.COM), because it would mess up all accents and other
 *   special characters.
 * - Respect the line numbering.
 * - Respect the quotation, C special symbols (%s, \n, etc), preprocessor
 *   statements (#ifdef, #else, etc)...
 * - Be specially careful with commas - they are at the end of every line
 *   EXCEPT the last line of each block.
 */

/*
 * Compile time errors
 */

#if TRANSLATE == TRANSLATE_ERROR

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
/* 30	*/	"Name already in use",
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
/* 73	*/	"ELSEIF does not appear in a IF block" /* NO COMMA */


/*
 * Warnings
 */

#elif TRANSLATE == TRANSLATE_WARNING

/* 0	*/	"",
/* 1	*/	"SETUP_PROGRAM: Obsolete statement",
/* 2	*/	"IMPORT: Old syntax, ignored" /* NO COMMA */


/*
 * DLL errors
 */

#elif TRANSLATE == TRANSLATE_DLL_ERROR

/* 1	*/ "Object name buffer out of bounds",
/* 2	*/ "Object '%s': Name already in use",
/* 3	*/ "Too many objects declared",
/* 4	*/ "Function '%s': Name already in use",
/* 5	*/ "Error declaring object '%s'",
/* 6	*/ "Object '%s': Illegal declaration inside a struct",
/* 7	*/ "Object '%s': Member declaration outside a struct",
/* 8	*/ "Illegal size for string '%s'",
/* 9	*/ "EndStruct without Struct",
/* 10	*/ "A struct must contain at least one item" /* NO COMMA */


/*
 * ltlex.def errors
 */

#elif TRANSLATE == TRANSLATE_LTLEX_ERROR

/* 0	*/ "",
/* 1	*/ "File ltlex.def not found",
/* 2	*/ "Expecting an hexadecimal value (&00..&FF)",
/* 3	*/ "Too many symbols defined",
/* 4	*/ "Symbol not valid" /* NO COMMA */


/*
 * Compiler messages
 */

#elif TRANSLATE == TRANSLATE_COMPILER

/* 0	*/ " **** eDIV Compiler " VERSION " ****\n" COPYRIGHT "\n" WEBSITE "\n",
/* 1	*/ "WARNING: This version of eDIV is unfinished.\nPlease report any bug to: ediv@divsite.net\n",
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
/* 50	*/ "Error: received file if of an unexpected format\n" /* NO COMMA */


/*
 * Stub messages
 */

#elif TRANSLATE == TRANSLATE_STUB

/* 0	*/	"ERROR %d: ",
/* 1	*/	"eDIV: Runtime error",
/* 2	*/	"eDIV: Critical error",
/* 3	*/	"Error in %s: ",
/* 4	*/	"eDIV: Error in dynamic library" /* NO COMMA */


/*
 * Stub runtime errors
 */

#elif TRANSLATE == TRANSLATE_RUNTIME_ERROR

/* TODO: Creo que esta traducción es un poco patatera, voy a tener que revisarla un poco. */
/* 100 */ "There is not enough memory to execute the program.",
/* 101 */ "Call to a key() with a nonexistent key code.",
/* 102 */ "The palette was not loaded, archive not found.",
/* 103 */ "A not valid archive of palette was loaded.",
/* 104 */ "Too many files FPG opened at the same time.",
/* 105 */ "The file was not loaded, archive not found.",
/* 106 */ "A not valid FPG file was loaded.",
/* 107 */ "Not valid number of scroll (must be between 0 and 9).",
/* 108 */ "Not valid number of region (out of rank).",
/* 109 */ "Not valid code of file.",
/* 110 */ "Not valid code of graphic.",
/* 111 */ "A file FPG was used before load it.",
/* 112 */ "A code of graphic is needed for the scroll.",
/* 113 */ "Too many letter font have been loaded.",
/* 114 */ "The font can't be loaded, archive not found.",
/* 115 */ "A not valid archive of font was loaded.",
/* 116 */ "Not valid identifier code of letter fonts.",
/* 117 */ "Not valid text's code of centrate.",
/* 118 */ "Too many active texts in the program.",
/* 119 */ "Not valid text identifier.",
/* 120 */ "Region of screen defined incorrectly.",
/* 121 */ "A nonexistent graphic was used.",
/* 122 */ "A block out of memory was written.",
/* 123 */ "An archive of writeness can't be opened.",
/* 124 */ "The archive can't be written (see space in disk).",
/* 125 */ "The block out of the memory was readen.",
/* 126 */ "The readness archive can't be opened.",
/* 127 */ "The archive can't be readen.",
/* 128 */ "The sound effect can't be loaded.",
/* 129 */ "The asked sound effect can't be played.",
/* 130 */ "The asked animation can't be started.",
/* 131 */ "Not valid number of mode 7 (must be between 0 and 9).",
/* 132 */ "The code of graphic for mode 7 is needed.",
/* 133 */ "Not valid checkpoint number.",
/* 134 */ "Incorrect botton number (must be between 0 and 3).",
/* 135 */ "Incorrect axis number (must be between 0 and 3).",
/* 136 */ "Adress of palette board out of the memory.",
/* 137 */ "Regions in graphics of m-7 can't be checked.",
/* 138 */ "Incorrect code of graphic information.",
/* 139 */ "Collisions in graphics of m-7 can't be checked.",
/* 140 */ "Access out of range.",
/* 141 */ "A not valid process was accessed.",
/* 142 */ "The process seems to be blocked, waiting FRAME.",
/* 143 */ "The map can't be loaded, archive not found.",
/* 144 */ "A not valid archive of map was loaded.",
/* 145 */ "A division by zero was made.",
/* 146 */ "The size of the region is incorrect.",
/* 147 */ "Can't be loaded FLI/FLC, archive not found.",
/* 148 */ "Incorrect value of timeout (must be between 1 and 60).",
/* 149 */ "Incorrect number of players (must be between 2 and 8).",
/* 150 */ "Only global data can be send.",
/* 151 */ "Not valid size of square (must be between 1 and 256).",
/* 152 */ "Not valid map of search (from 1x1 to 128x128).",
/* 153 */ "Incorrect size of map (from 1x1 to 32768x32768).",
/* 154 */ "Incorrect number of color (must be between 0 and 255).",
/* 155 */ "The center is outside of the graphic's limits.",
/* 156 */ "The object is outside of the map and will be eliminated.",
/* 157 */ "Texture of the map not found.",
/* 158 */ "The size of the texture is incorrect.",
/* 159 */ "The world can't be loaded, archive not found.",
/* 160 */ "The world can't be loaded, archive not valid.",
/* 161 */ "Not valid flag number.",
/* 162 */ "Not valid dispositive (only 1-IPX,2-SERIAL,3-MODEM).",
/* 163 */ "Not valid size of box (only from 1 to 199).",
/* 164 */ "The string of destination text is not valid.",
/* 165 */ "A wrong cursor has been accessed.",
/* 166 */ "The mode of access to the archive is not valid.",
/* 167 */ "The module can't be loaded.",
/* 168 */ "Too many ignored errors.",
/* 169 */ "Too many open archives.",
/* 170 */ "Identifier of archive (handle) not valid.",
/* 171 */ "Value out of rank.",
/* 172 */ "Too many graphic objects visualizated.",
/* 173 */ "Type of graphic object unknown.",
/* 174 */ "Percentage of ink out of rank (0..15).",
/* 175 */ "Number of graphic object not valid.",
/* 176 */ "Variable error of vpe: Texture of map [a] not found in the file [b]",
/* 177 */ "Texture of map",
/* 178 */ "Doesn't found in file",
/* 179 */ "Too many petitions of dynamic memory has been made.",
/* 180 */ "The cursor doesn't relate with any of the reserved blocks.",
/* 181 */ "No blocks of less than one position can be reserved.",
/* 182 */ "A block out of memory was (des)encript.",
/* 183 */ "Can't be loaded another palette when the current one is forced.",
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
/* 200 */ "Archive with corrupted data, can't be loaded." /* NO COMMA */


/*
 * Stub critical errors
 */

#elif TRANSLATE == TRANSLATE_CRITICAL_ERROR

/* 0	*/	"",
/* 1	*/	"Error reading program bytecode",
/* 2	*/	"Erroneous bytecode format",
/* 3	*/	"Process type redefinition",
/* 4	*/	"Can't allocate stack memory",
/* 5	*/	"Library not found: %s",
/* 6	*/	"%s is not a valid eDIV library",
/* 7	*/	"Cannot initialize SDL",
/* 8	*/	"Too many processes in execution",
/* 9	*/	"This program requires SDL version %d.%d.%d\nSDL version detected in this system: %d.%d.%d" /* NO COMMA */


#endif

/* End Of File */
