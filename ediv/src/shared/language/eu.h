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
 * 5 - EUSKERA (eu)
 * Autor: Jorge Urreta
 */


/*
 * Errores de compilación
 */

#if TRANSLATE == TRANSLATE_ERROR

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
/* 73	*/ "ELSEIF ez dago IF barruan" /* SIN COMA */


/*
 * Advertencias
 */

#elif TRANSLATE == TRANSLATE_WARNING

/* 0	*/ "",
/* 1	*/ "SETUP_PROGRAM: Zaharkituriko karakteristika",
/* 2	*/ "IMPORT: Antzinako sintaxia, alde batean utziko da" /* SIN COMA */


/*
 * Errores de DLL
 */

#elif TRANSLATE == TRANSLATE_DLL_ERROR

/* 1	*/ "Objektu izenen buffer-aren kapazitatea gainditurik",
/* 2	*/ "'%s'objektua: Izena ez da berria",
/* 3	*/ "Gehiegizko deklaraturiko objektuak",
/* 4	*/ "'%s' funtzioa: izena ez da berria",
/* 5	*/ "Akatz bat sortu da '%s' objektua deklaratzean",
/* 6	*/ "'%s'objektua: Debekaturiko deklarazioa struct barruan",
/* 7	*/ "'%s'objektua: Atal baten deklarazioan struct kanpo",
/* 8	*/ "'%s' String-eko okerreko neurria",
/* 9	*/ "Struct gabeko EndStruct",
/* 10	*/ "Estruktura batek atal bat gutxienez eduki behar du" /* SIN COMA */


/*
 * Errores de ltlex.def
 */

#elif TRANSLATE == TRANSLATE_LTLEX_ERROR

/* 0	*/ "",
/* 1	*/ "Ezin da ltlex.def fitxategia aurkitu",
/* 2	*/ "Balio hamaseitar bat espero zen (&00..&FF)",
/* 3	*/ "Deklraturiko ikur gehiegi",
/* 4	*/ "Okerreko ikurra" /* SIN COMA */


/*
 * Mensajes del compilador
 */

#elif TRANSLATE == TRANSLATE_COMPILER

/* 0	*/ " **** eDIV konpiladorea " VERSION " ****\n" COPYRIGHT "\n" WEBSITE "\n",
/* 1	*/ "KONTUZ: eDIV bertsio hau ez dago bukaturik.\nMesedez eman berri edozein akatzi buruz helbide honetan: ediv@divsite.net\n",
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
/* 50	*/ "Akatza: jasotako fitxategia ez da itxarotako motakoa\n" /* SIN COMA */


/*
 * Mensajes del stub
 */

#elif TRANSLATE == TRANSLATE_STUB

/* FIXME: traducir esto */
/* 0	*/	"ERROR %d: ",
/* 1	*/	"eDIV: Error en ejecución",
/* 2	*/	"eDIV: Error crítico",
/* 3	*/	"Error en %s: ",
/* 4	*/	"eDIV: Error en librería dinámica" /* SIN COMA */


/*
 * Errores de tiempo de ejecución del stub
 */

#elif TRANSLATE == TRANSLATE_RUNTIME_ERROR

/* 100 */ "Ez dago behar beste memoria programa egikaritzeko.",
/* 101 */ "Key() deitu duzu existitzen ez den tekla kode batekin.",
/* 102 */ "Ezin da paleta kargatu, ez bait da fitxategia aurkitu.",
/* 103 */ "Okerreko paleta fitxategi bat kargatzen saiatu zara.",
/* 104 */ "FPG fitxategi gehiegi aldi berean kargaturik.",
/* 105 */ "Ezin da fitxategia kargatu, ez bait da aurkitzen.",
/* 106 */ "Okerreko FPG fitxategi bat kargatzen saiatu zara.",
/* 107 */ "Okerreko scroll zenbakia (0 eta 9 artean egon behar du).",
/* 108 */ "Okerreko erregio zenbakia (Barrutiaren kanpo dago).",
/* 109 */ "Okerreko fitxategi zenbakia.",
/* 110 */ "Okerreko irudi zenbakia.",
/* 111 */ "FPG fitxategi bat erabiltzen saiatu zara kargatu baino lehen.",
/* 112 */ "Irudi kode bat behar da scroll-arentzat.",
/* 113 */ "Letra-tipo gehiegi kargatu dira.",
/* 114 */ "Ezin da letra-tipoa kargatu, ez bait da aurkitzen.",
/* 115 */ "Okerreko letra-tipoa kargatzen saiatu zara.",
/* 116 */ "Okerreko letra-tipo kode identifidorea.",
/* 117 */ "Okerreko testua zentratzeko kodea.",
/* 118 */ "Testu gehiegi aktibaturik programan.",
/* 119 */ "Okerreko testu identifikadorea.",
/* 120 */ "Pantaila erregio txarto mugatua.",
/* 121 */ "Existitzen ez den irudi bat erabiltzen saiatu zara.",
/* 122 */ "Multzo bat memoriaz kanpo idazten saiatu zara.",
/* 123 */ "Ezin izan da fitxategia idazteko ireki.",
/* 124 */ "Ezin izan da fitxategia idatzi (begiratu diskoan dagoen lekuari).",
/* 125 */ "Multzo bat memoriaz kanpotik irakurtzen saiatu zara.",
/* 126 */ "Ezin izan da fitxategia irakurtzeko ireki.",
/* 127 */ "Ezin izan da fitxategia irakurri.",
/* 128 */ "Ezin izan da soinu efektua kargatu.",
/* 129 */ "Ezin izan da aukeratutako soinu efektua jo.",
/* 130 */ "Ezin izan da aukeratutako animazioa iniziatu.",
/* 131 */ "Okerreko 7 modu zenbakia (0 eta 9 artean egon behar du).",
/* 132 */ "Irudi kode bat behar da 7 moduarentzat.",
/* 133 */ "Okerreko kotroleko puntua.",
/* 134 */ "Okerreko botoi zenbakia (0 eta 3 artean egon behar du).",
/* 135 */ "Okerreko ardatz zenbakia (0 eta 3 artean egon behar du).",
/* 136 */ "Paletaren taularen helbidea memoriaz kanpo dago.",
/* 137 */ "7 moduan ezin da irudi erregiorik egiaztatu.",
/* 138 */ "Okerreko irudi informazio kodea.",
/* 139 */ "7 moduan ezin da irudien arteko talkarik egiaztatu.",
/* 140 */ "Barrutiaren kanpoko atzipena.",
/* 141 */ "Okerreko prozesu bat atziten saiatu zara.",
/* 142 */ "Prozesuak blokeaturik dirudi, FRAME-aren zai.",
/* 143 */ "Ezin izan da mapa kargatu, ez bait da aurkitzen.",
/* 144 */ "Okerreko mapa fitxategi bat kargatzen saiatu zara.",
/* 145 */ "Zeroz zatitzen saiatu zara.",
/* 146 */ "Okerreko erregio neurria.",
/* 147 */ "Ezin da FLI/FLC fitxategia kargatu, ez bait da aurkitzen.",
/* 148 */ "Okerreko timeout zenbakia (1 eta 60 artean egon behar du).",
/* 149 */ "Okerreko jokalari zenbaia (2 eta 8 artean egon behar du).",
/* 150 */ "Datu orrokorrak bakarrik bidali daitezke.",
/* 151 */ "Okerreko lauki neurria (1 eta 256 artean egon behar du).",
/* 152 */ "Okerreko bilaketa mapa zenbakia (1x1 eta 128x128 artean egon behar du).",
/* 153 */ "Okerreko mapa neurria (1x1 eta 32768x32768 artean egon behar du).",
/* 154 */ "Okerreko kolore zenbakia (0 eta 255 artean egon behar du).",
/* 155 */ "Erdia iruditik kanpo dago.",
/* 156 */ "Objektua mapatik kanpo dago eta ezabaturik izango da.",
/* 157 */ "Ezin da aurkitu maparen egitura.",
/* 158 */ "Egituraren neurria okerrekoa da.",
/* 159 */ "Ezin izan da mundua kargatu, ez bait da aurkitzen.",
/* 160 */ "Ezin izan da mundua kargatu, okerreko fitxategia da.",
/* 161 */ "Okerreko bandera zenbakia.",
/* 162 */ "Okerreko dispositiboa (1-IPX,2-SERIAL edo 3-MODEM soilik).",
/* 163 */ "Okerreko pakete neurria (1 eta 199 artean egon behar du).",
/* 164 */ "Okerreko helburuko testua.",
/* 165 */ "Balio gabeko erakusle bat atziten saiatu zara.",
/* 166 */ "Okerreko fitxategi atzipen modua.",
/* 167 */ "Ezin izan da modulua kargatu.",
/* 168 */ "Alde batera utzitako akatz gehiegi.",
/* 169 */ "Irekitako fitxategi gehiegi.",
/* 170 */ "Okerreko fitxategi identifikadorea (handle).",
/* 171 */ "Barrutiaren kanpoko datua.",
/* 172 */ "Erakutsitako irudi objektu gehiegi.",
/* 173 */ "Irudi objektu mota ezesaguna.",
/* 174 */ "Barrutiaren kanpoko tinta porzentaia (0..15).",
/* 175 */ "Okerreko irudi objektu zenbakia.",
/* 176 */ "Vpe akatz aldakorra: ezin da [a] maparen egitura [b] fitxategian aurkitu.",
/* 177 */ "Maparen egitura.",
/* 178 */ "ezin da fitxategian aurkitu.",
/* 179 */ "Memoria dinamiko gehiegi eskatu da.",
/* 180 */ "Erakuslea ez dator bat erreserbaturiko multzo bakar batekin.",
/* 181 */ "Ezin da posizio bat baino gutxiko multzorik erreserbatu.",
/* 182 */ "Memoriarik gabeko multzo bat (des)enkriptatzen saiatu zara.",
/* 183 */ "Ezin da beste paleta bat kargatu oraingokoa derrigorturik dagoenean.",
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
/* 200 */ "Hondaturiko fitxategia, ezin da kargatu." /* SIN COMA */


/*
 * Errores críticos del stub
 */

#elif TRANSLATE == TRANSLATE_CRITICAL_ERROR

/* 0	*/	"",
/* 1	*/	"Akatz bat sortu da programaren kodea irakurtzean",
/* 2	*/	"Akatz bat dago kodearen formatuan",
/* 3	*/	"Prozesu motaren berdefinizioa",
/* 4	*/	"Ezin izan da memoriarik erreserbatu pilarentzat",
/* 5	*/	"Ezin da liburutegia aurkitu: %s",
/* 6	*/	"%s ez da eDIV-en liburutegi zuzen bat",
/* 7	*/	"No se pudo inicializar SDL",	// FIXME: traducir esto
/* 8	*/	"Demasiados procesos en ejecución",	// FIXME: traducir esto
/* 9	*/	"Se requiere SDL versión %d.%d.%d\nVersión detectada en el sistema: %d.%d.%d" /* SIN COMA */ // FIXME: traducir esto


#endif

/* Fin de fichero */
