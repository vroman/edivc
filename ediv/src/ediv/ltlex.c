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

//-----------------------------------------------------------------------------
//      Precarga de las estructuras léxicas, analiza el fichero ltlex.def
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "parser.h"
#include "lower.h"

#include "language.h"

void ltlex_error(int error)
{
	if(error==1)
		printf(translate(40),translate_ltlex_error(error));
	else
		printf(translate(41),"ltlex.def",linea,translate_ltlex_error(error));
	printf(translate(42));
	exit(1);
}

void analiza_ltlex(void){

    byte *_buf, * buf;		// Buffer del texto y puntero al carácter actual
	byte cont=1;			// 0 indica final del archivo
    int len;				// Tamaño del archivo
    struct lex_ele * e;		// Puntero al lex_case correspondiente
	FILE * def;				// Stream del fichero

    int t; //token (pieza)
    byte h; //hash (para id)
    byte * _ivnom;
    byte * * ptr;

    // Busca el archivo ltlex.def
	if ((def=fopen("system/ltlex.def","rb"))==NULL)
		ltlex_error(1);

	// Lo lee
    fseek(def,0,SEEK_END);
	len=ftell(def);
    _buf=buf=(byte *) e_malloc(len+1);
    fseek(def,0,SEEK_SET);
    len=fread(buf,1,len,def);

	// El carácter ASCII 0 indica final del fichero
    *(buf+len)=0;// *(buf+len+1)=cr;

	#ifdef _DEBUG
		printf("dbg: tamano ltlex: %d\n",len);
	#endif

    linea=1;

    do {
/*    printf("- %d\n",linea);
    if(linea==45)
    	printf("heyp\n");*/
    	
    switch (*buf++) {

    // Fin de fichero
    case 0:
    	cont=0; break;

	// Ignoramos los espacios
    case ' ': case tab:
		break;

	// Salta de linea
    case cr:
		if(*buf==lf) buf++;
		
	case lf:
        linea++;
        break;

	// Los ; indican línea de comentario
    case ';':
		while (*buf!=cr && *buf!=lf) buf++; break;

	// El símbolo & indica definición de token
    case '&':
		*buf=lower[*buf];
		if (*buf>='0' && *buf<='9')
			t=(*buf++-'0')<<4;
		else
			if (*buf>='a' && *buf<='f')
				t=(*buf++-'a'+10)<<4;
			else
				ltlex_error(2);
		*buf=lower[*buf];
		if (*buf>='0' && *buf<='9')
			t+=(*buf++-'0');
		else
			if (*buf>='a' && *buf<='f')
				t+=(*buf++-'a'+10);
			else
				ltlex_error(2);
		if (*buf==cr || *buf==lf || *buf==' ' || *buf==tab) break;
		else if (lower[*buf]) {           //Analiza una palabra reservada
			_ivnom=ivnom.b; *ivnom.p++=0; *ivnom.p++=(unsigned char*)t; h=0;
			while ((*ivnom.b=lower[*buf++])) h=((byte)(h<<1)+(h>>7))^(*ivnom.b++);
			ptr=&vhash[h]; while (*ptr) ptr=(unsigned char * *)*ptr; *ptr=_ivnom;
			buf--; ivnom.b++;
		} else if (t>=0x78 && t<=0x7b) {  //Analiza un delimitador de literal
			lex_case[*buf]=(struct lex_ele*)l_lit;
		} else {                          //Analiza un nuevo símbolo
			if ((e=lex_case[*buf])==0) {
				if (num_nodos++==max_nodos) ltlex_error(3);
				e=lex_case[*buf]=ilex_simb++; (*e).caracter=*buf++;
			} else buf++;
			while (*buf!=' ' && *buf!=tab && *buf!=cr) {
				if (lower[*buf]) ltlex_error(4);
				if ((*e).siguiente==0)
					if (num_nodos++==max_nodos) ltlex_error(3);
					else e=(*e).siguiente=ilex_simb++;
					else {
						e=(*e).siguiente;
						while ((*e).caracter!=*buf && (*e).alternativa)
							e=(*e).alternativa;
						if ((*e).caracter!=*buf) {
							if (num_nodos++==max_nodos) ltlex_error(3);
							else e=(*e).alternativa=ilex_simb++;
						}
					} (*e).caracter=*buf++;
			} (*e).token=t;
		} break;
    }} while (cont);

    free(_buf); _buf=NULL;
    fclose(def); def=NULL;

}
