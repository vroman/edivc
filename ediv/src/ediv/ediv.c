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

#ifdef _WIN32
#	include <winsock2.h>
#	include "dll_load.h"

struct in_addr iahost;
//struct sockaddr_in sin;
struct hostent *htent;
struct servent *svent;
int status;
char buffer[1024];
SOCKET sock;

#define sock_error(s) fprintf(stderr,"\nerror: %d\n", WSAGetLastError())

void muestra_motd();
void conecta(char *servidor, char *archivo);

#else
#	include <dlfcn.h>		// ¿será igual en BeOS?
#	include <dirent.h>
//#	include <sys/stat.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "main.h"
#include "compiler.h"
#include "parser.h"
#include "extern.h"
#include "modulos.h"
#include "shared.h"
#include "language.h"
#include "edivcfg/iniparser.h"

int main(int argc, char *argv[])
{
	FILE *fp;			// stream del PRG
	char *p;
	char libmagic[14];	// cabecera "magic" de EDIVRUN.LIB
	
	int i,j;
	byte hayprog=0;

	char sistema[20];

	n_errors=0;
	n_warnings=0;
	debug=0;
	listados=0;
	noexe=0;

	// Sistemas

	#ifdef _WIN32
		strcpy(sistema,"win32");
	#endif

	#ifdef __linux__
		strcpy(sistema,"linux");
	#endif

	ini=iniparser_load("ediv.cfg");

	idioma=detecta_idioma();

	// mensaje de cabecera
	printf(translate(0));
	printf(translate(1));
	printf("\n");

    if(argc<2) goto ayuda;

    if(strcmp(argv[1],"--version")==0 || strcmp(argv[1],"-v")==0)
        exit(0);

    if(strcmp(argv[1],"--help")==0 || strcmp(argv[1],"-h")==0) {
        ayuda:
		// pantalla de ayuda
        printf(translate(2));
        printf(translate(3),argv[0]);
		printf(translate(4));
		printf(translate(5));
		printf(translate(6));
		printf(translate(7));
		printf(translate(8));
		printf(translate(9));
		printf(translate(10),sistema);
		printf(translate(48));
		printf(translate(11));
        exit(0);
    }

    for(i=1;i<argc;i++) {
        if(argv[i][0]=='-') {
            if(argv[i][1]=='-') {
                if(strcmp(argv[i]+2,"debug")==0)
                    debug=1;
                else if(strcmp(argv[i]+2,"list")==0)
                    listados|=1;
                else if(strcmp(argv[i]+2,"table")==0)
                    listados|=2;
				else if(strcmp(argv[i]+2,"check")==0)
					noexe=1;
				else if(strcmp(argv[i]+2,"system")==0)
					if(++i<argc) {
						strcpy(sistema,argv[i]);
					} else {
						printf(translate(12)); // se debe indicar un sistema para --system
						exit(1);
					}
#ifdef _WIN32
				else if(strcmp(argv[i]+2,"news")==0) {
					muestra_motd();
				}
#endif
                else {
                    printf(translate(13),argv[i]); // parámetro erróneo
                    exit(1);
                }
            }
            else {
                j=1;
                while(argv[i][j]!=0) {
                    switch(argv[i][j]) {
                        case 'd':
                            debug=1;
                            break;
                        case 'l':
                            listados|=1;
                            break;
                        case 't':
                            listados|=2;
                            break;
						case 'c':
							noexe=1;
							break;
						case 's':
							if(argv[i][j+1]!=0) {
								strcpy(sistema,argv[i]+j+1);
								j=strlen(argv[i])-1;
							}
							else {
								if(++i<argc) {
									strcpy(sistema,argv[i]);
									j=strlen(argv[i])-1;
								}
								else {
									printf(translate(14)); // se debe indicar un sistema para -s
									exit(1);
								}
							}
							break;
#ifdef _WIN32
						case 'n':
							muestra_motd();
							exit(0);
#endif
                        default:
                            printf(translate(15),argv[i][j]); // parámetro erróneo
                            exit(1);
                    }
                    j++;
                }
            }
        }
        else switch (hayprog) {
			case 0:
				fichero_prg=argv[i];
				hayprog=1;
				break;
			case 1:
				strcpy(outfilename,argv[i]);
				hayprog=2;
				break;
			default:        	
                printf(translate(13),argv[i]); // parámetro erróneo
                exit(1);
		}
    }

    if(hayprog==0) {
        printf(translate(16)); // no se ha especificado un archivo
        exit(1);
    }
	
	for (i=0;i<=strlen(fichero_prg);i++) {
		if (*(fichero_prg+i) == '.')
			break;
		nombreprog[i] = *(fichero_prg+i);
	}
	nombreprog[i]=0;
	
	if(!(fp = fopen(fichero_prg, "rb"))) {
		if(!strcmp(nombreprog,fichero_prg)) {
			strcpy(fichero_prg,nombreprog);
			strcat(fichero_prg,".prg");
			if(!(fp = fopen(fichero_prg, "rb"))) {
				strcpy(fichero_prg,nombreprog);
				strcat(fichero_prg,".PRG");
				if(!(fp = fopen(fichero_prg, "rb"))) {
					printf(translate(17),nombreprog); // error al abrir archivo
					exit(1);
				}
			}
		}
		else {
			printf(translate(17),argv[1]); // error al abrir archivo
			exit(1);
		}
	}
	
	// Componemos el nombre del ejecutable
	if(hayprog<2) {
		strcpy(outfilename,nombreprog);
		if(!strcmp(sistema,"win32"))
			strcat(outfilename,".exe");
	}
	
	printf(translate(18),fichero_prg); // compilando...

	/*fichero_prg=(char*)e_malloc(strlen(argv[1])+1);
	strcpy(fichero_prg,argv[1]);		// fichero_prg="xxxxx.prg"*/

	// mete el PRG en el buffer prog
	fseek(fp,0,SEEK_END);
	progsize=ftell(fp);
	prog = (char *)e_malloc(progsize+1);
	fseek(fp,0,SEEK_SET);
	p=prog;
	do {
		*p = getc(fp);
		p++;
	} while(!feof(fp));
	fclose(fp);

	// Comprueba si existe la edivrun.lib
	if(!noexe) {
		strcpy(edivrun_lib,sistema);
		if(debug)
			strcat(edivrun_lib,".dbg");
		else
			strcat(edivrun_lib,".rel");

		if((fp = fopen(edivrun_lib, "rb"))==NULL) {
			printf(translate(19),edivrun_lib); // no se encuentra el fichero
			exit(1);
		}
		else {
			fread(libmagic,1,14,fp);
			if(strcmp(libmagic,magic)) {
				printf(translate(20),edivrun_lib); // formato incorrecto de edivrun.lib
				fclose(fp);
				exit(1);
			}
			else {
				fread(&stub_size,1,4,fp);
				fclose(fp);
				#ifdef _DEBUG
					printf("dbg: STUB_SIZE: %d\n",stub_size);
				#endif
			}
		}
	}

	// El stream de salida para el seudocódigo
//	fout=tmpfile();

	if(!ini) {
		max_process=0; // Valores de las opciones por defecto
		ignore_errors=0;
		free_sintax=0;
		extended_conditions=0;
		simple_conditions=0;
		comprueba_rango=1;
		comprueba_id=1;
		comprueba_null=1;
		hacer_strfix=1;
		optimizar=1;
		case_sensitive=0;
	} else {
		max_process=iniparser_getint(ini,"default_compiler_options:max_process",0);
		ignore_errors=iniparser_getboolean(ini,"default_compiler_options:ignore_errors",0);
		free_sintax=iniparser_getboolean(ini,"default_compiler_options:free_sintax",0);
		extended_conditions=iniparser_getboolean(ini,"default_compiler_options:extended_conditions",0);
		simple_conditions=iniparser_getboolean(ini,"default_compiler_options:simple_conditions",0);
		case_sensitive=iniparser_getboolean(ini,"default_compiler_options:case_sensitive",0);
		comprueba_rango=iniparser_getboolean(ini,"default_compiler_options:no_range_check",0)?0:1;
		comprueba_id=iniparser_getboolean(ini,"default_compiler_options:no_id_check",0)?0:1;
		comprueba_null=iniparser_getboolean(ini,"default_compiler_options:no_null_check",0)?0:1;
		hacer_strfix=iniparser_getboolean(ini,"default_compiler_options:no_strfix",0)?0:1;
		optimizar=iniparser_getboolean(ini,"default_compiler_options:no_optimization",0)?0:1;
		case_sensitive=iniparser_getboolean(ini,"default_compiler_options:case_sensitive",0);
		if(iniparser_getboolean(ini,"default_compiler_options:no_check",0))
			comprueba_rango=comprueba_id=comprueba_null=0;

	}

	prepara_compilacion();
	dll_func();

	//**************************

	//###############//
	// NO PONER NADA //
	//loggea("prg: %s",argv[1]);
	//@@@@@@@@@@@@@@@//

	compila();
	
#ifdef MULTI_ERROR	
	printf("edivc: Finalizado - %d advertencias, %d errores\n",n_warnings,n_errors);
#else
	if(n_errors)
		printf(translate(21),n_warnings);
	else
		printf(translate(22),n_warnings);
#endif

	if(n_errors==0)
		return 0;
	else {
		printf(translate(23));
		return 2;
	}

}


// idéntico a malloc, pero con errormsg utomático
void* e_malloc(size_t size)
{
	void* ret;

	if(!(ret = malloc(size))) {
		printf(translate(24)); // memoria insuficiente
		exit(1);
	}

	return ret;
}

void errormem()
{
	printf(translate(24));
	exit(1);
}


//-----------------------------------------------------------------------------
//      Gestión de errores
//-----------------------------------------------------------------------------

int _le,_t;
byte *_ie;

void save_error(word tipo) { // Guarda una posicion de error (de 0 .. 3)
  switch(tipo) {
    case 0: _le=linea; _ie=ierror; break;
    case 1: _le=old_linea; _ie=old_ierror_end; break;
    case 2: _le=old_linea; _ie=old_ierror; break;
    case 3: _le=old_linea; _ie=old_ierror_end; break;
  } _t=tipo;
}

// 0 - Inicio de la pieza actual
// 1 - Final de la pieza anterior mas un espacio (si se puede)
// 2 - Inicio de la pieza anterior
// 3 - Final de la pieza anterior
// 4 - Error guardado con save_error(0..3)

void error(word tipo, word num, ...)
{
	int columna=0;
	int linea_error;
	byte *_p,*p;
	va_list opparam;

	switch(tipo) {
		case 0: linea_error=linea; p=ierror; break;
		case 1: linea_error=old_linea; _p=p=old_ierror_end; break;
		case 2: linea_error=old_linea; p=old_ierror; break;
		case 3: linea_error=old_linea; _p=p=old_ierror_end; break;
		case 4: linea_error=_le; _p=p=_ie; tipo=_t; break;
	}

	if (num!=2 && num!=6) {
		while (--p>=prog) {
			if (*p==lf || (*p==cr && *(p+1)==lf)) break;
			columna++;
		}
		if (p<prog) columna++;
	}

	if (tipo==1) {
		if (*(_p+1)==' ' || *(_p+1)==cr || *(_p+1)==lf) columna++; columna++;
	} else if (tipo==3) columna++;

	va_start(opparam,num);
	printf(translate(25), fichero_prg, linea_error, columna, num);
	vprintf(translate_error(num),opparam);
	printf("\n");

	n_errors++;

#ifdef MULTI_ERROR		
	if(n_errors>20)
		printf("Demasiados errores. Compilación interrumpida.\n");
				
	if(num==1 || num==2 || num==6 || n_errors>20) {
		printf("edivc: Finalizado - %d advertencias, %d errores\n",n_warnings,n_errors);
#else
		printf(translate(21),n_warnings);	
#endif		
		printf(translate(23));
		exit(2);
#ifdef MULTI_ERROR
	}
#endif
	
	//lexico();
}

void warning(int num, ...)
{
	va_list opparam;

	va_start(opparam,num);
    printf(translate(26), fichero_prg, linea);
	vprintf(translate_warning(num),opparam);
	printf("\n");
	n_warnings++;
}

//#define DEBUG_HTTP
#ifdef _WIN32
void muestra_motd()
{
	char* motdserver=NULL;
	char* motdpath=NULL;

	if(ini) {
		motdserver=iniparser_getstr(ini,"general:motd_host");
		motdpath=iniparser_getstr(ini,"general:motd_path");
		if(!motdserver) {
			printf(translate(49)); // el servidor de motd debe estar en ediv.cfg
			exit(1);
		}
		if(!motdpath) motdpath="/ediv/actual/motd.txt";
		conecta(motdserver,motdpath);
		exit(0);
	} else {
		printf(translate(49)); // el servidor de motd debe estar en ediv.cfg
		exit(1);
	}
}

void conecta(char *servidor, char *archivo)
{
	struct sockaddr_in sin;

	char *size, *content,*ras;
	int tam,inc;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	
	iahost.s_addr=inet_addr(servidor);

	if (iahost.s_addr==INADDR_NONE)
		htent=gethostbyname(servidor);
	else
		htent=gethostbyaddr((const char *)&iahost,sizeof(struct in_addr),AF_INET);
	
	if (htent==NULL) {
		sock_error("gethostbyname()");
		return;
	}

	sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if (sock==INVALID_SOCKET) {
		sock_error("socket()"); 
		return;
	}

	svent=getservbyname("http", "tcp");
	if (svent==NULL)
		sin.sin_port=htons((u_short)80);
	else
		sin.sin_port=svent->s_port;

	sin.sin_family=AF_INET;
	sin.sin_addr=*(struct in_addr *)*htent->h_addr_list;

	status=connect(sock,(struct sockaddr *)&sin,sizeof(sin));
	if (status==SOCKET_ERROR) {
		sock_error("connect()");
		closesocket(sock);
		return;
	}

	//sprintf(buffer,"GET /%s\n",archivo);
	sprintf(buffer,"GET %s HTTP/1.1\n"
		"User-Agent: Mozilla/4.76 (Windows 2000; U) Opera 5.12  [es]\n"
		"Host: %s\n"
		"Accept: text/html, image/png, image/jpeg, image/gif, image/x-xbitmap, */*\n"
		"Accept-Language: es\nAccept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\n"
		"Connection: Keep-Alive\n"
		"\n"
		,archivo,servidor);

#ifdef DEBUG_HTTP
	printf("GET %s HTTP/1.1\n"
		"User-Agent: Mozilla/4.76 (Windows 2000; U) Opera 5.12  [es]\n"
		"Host: %s\n"
		"Accept: text/html, image/png, image/jpeg, image/gif, image/x-xbitmap, */*\n"
		"Accept-Language: es\nAccept-Encoding: deflate, gzip, x-gzip, identity, *;q=0\n"
		"Connection: Keep-Alive\n"
		"\n"
		,archivo,servidor);
#endif

	status=send(sock,buffer,strlen(buffer),0);
	if (status==SOCKET_ERROR) {
		sock_error("send()");
		closesocket(sock);	
		return;
	}

	status=recv(sock,buffer,sizeof(buffer),0);
	if (status == SOCKET_ERROR) {
		sock_error("recv()");
	}

	closesocket(sock);

	if(*(buffer+9)!='2' || *(buffer+10)!='0' || *(buffer+10)!='0') {
		*strchr(buffer,0x0d)=0;
		printf("%s\n",buffer);
		exit(1);
	}
	size=strstr(buffer,"Content-Type: ");
	if(!size) {
		printf(translate(50));
		exit(1);
	}
	size+=14;
	*(ras=strchr(size,0x0d))=0;
	if(strcmp(size,"text/plain")) {
		printf(translate(50));
		exit(1);
	}
	*ras=0x0d;

	size=strstr(buffer,"Content-Length: ");
	if(!size) {
		printf(translate(50));
		exit(1);
	}
	size+=16;
	ras=strchr(size,0x0d)-1;
	content=strstr(size,"\x0d\x0a\x0d\x0a")+4;
	tam=0; inc=1;
	for(;ras>=size;ras--) {
		if (*ras>='0' && *ras<='9')
			tam+=(*ras-'0')*inc;
		else
			break;
		inc*=10;
	}
	if(tam>1024) tam=1024;

    fwrite(content, tam, 1, stdout);
}
#endif