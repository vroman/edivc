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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
 * esta es la tabla donde se almacenan los nombres de las directivas y los textos
 * por los que deben ser sustituidas, definidos en template.txt
 */
struct _sust {
	char de[20];
	char a[1024];
} sust[20];

/* 
 * esta funcion recibe un nombre de fichero (actual) y, consultando el index.dok,
 * devuelve el documento anterior, el siguiente y el superior. Evidentemente esto
 * se usa para generar los enlaces de navegacion.
 */
void situame(char* actual, char* ant, char* sig, char* sup);

/* esta funcion abre el fichero indicado en fixero1 y busca en el el texto
 * encerrado entre las directivas <%title%> y <%/title%>, y lo devuelve en
 * el puntero "titulo"
 */
void lee_titulo(char* fixero1, char* titulo);

/*
 * esta es llamada cuando nos encontramos con la directiva <%index%>. Parsea
 * el arbolito ese con los signos '+' y genera una unordered list mu potita.
 */
void procesa_indice();

/*
 * esta es llamada cuando encontramos <%subindex%>. Se le indica el fichero
 * actual y busca la entrada correspondiente en el index.dok. Entonces parsea
 * la porcion de arbol que engloba y genera un subíndice como los de SGML.
 */
void procesa_subindice(char* actual);


FILE *f;
int i,tamano;
char* buffer;

int main(int argc, char* argv[])
{
	int c,num=-1,j,sw;
	char nomfich[256];
	char tag[20];
	int flag=0;

	if(argc!=2) {
		printf("Modo de uso: %s <fichero>\n",argv[0]);
		exit(1);
	}

	for(i=0;i<strlen(argv[1]);i++)
		if(argv[1][i]=='.') {
			argv[1][i]=0;
			break;
		}

	if((f=fopen("template.txt","r"))==NULL) {
		printf("Error abriendo fichero template.txt\n");
		exit(2);
	}

	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);
	if((buffer=(char*)malloc(tamano))==NULL) {
		printf("Error: memoria insuficiente\n");
		exit(3);
	}

	fread(buffer,1,tamano,f);
	fclose(f);

	for(i=0;i<tamano;i++) {
		if(buffer[i]=='<' && buffer[i+1]=='%') {
			if(num!=-1) sust[num].a[c]=0;
			num++; i+=2; c=0;
			while(buffer[i]==' ') i++;			
			while(buffer[i]!=' ' && buffer[i]!='%')
				sust[num].de[c++]=buffer[i++];
			sust[num].de[c]=0;
			while(buffer[i]==' ') i++;			
			if(buffer[i]=='%' && buffer[i+1]=='>') i+=2;
			c=0;
			if(!strcmp(sust[num].de,"index") || !strcmp(sust[num].de,"/index") ||
				!strcmp(sust[num].de,"subindex")) {
				printf("Error en template.txt: \"%s\" es una directiva reservada\n",sust[num].de);
				exit(10);
			}
		}
		if(buffer[i]=='"') {
			i++;
			while(1) {
				if(buffer[i]=='"' && buffer[i-1]!='\\') break;
				if(buffer[i]=='\\' && buffer[i+1]=='n')
					buffer[++i]='\n';
				else if(buffer[i]=='\\' && buffer[i+1]=='"')
					buffer[++i]='"';
				sust[num].a[c++]=buffer[i++];
			}
		}
	}

	free(buffer);

	strcpy(nomfich,argv[1]);
	strcat(nomfich,".dok");
	
	if((f=fopen(nomfich,"r"))==NULL) {
		printf("Error abriendo fichero %s\n",nomfich);
		exit(4);
	}

	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);

	if((buffer=(char*)malloc(tamano+1))==NULL) {
		printf("Error: memoria insuficiente\n");
		exit(5);
	}

	tamano=fread(buffer,1,tamano,f);
	fclose(f);

	buffer[tamano]=0;

	strcpy(nomfich,argv[1]);
	strcat(nomfich,".html");

	if((f=fopen(nomfich,"w"))==NULL) {
		printf("Error creando fichero %s\n",nomfich);
		exit(7);
	}

	printf("Procesando...\n");

	fprintf(f,"<!-- Generado con Sion Doku - http://www.sionhq.com -->\n");
	for(i=0;i<tamano;i++) {
		if(buffer[i]=='<' && buffer[i+1]=='%') {
			buffer[i]=0;
			fwrite(&buffer[flag],1,strlen(&buffer[flag]),f);
			c=0; i+=2;
			while(buffer[i]==' ') i++;			
			while(buffer[i]!=' ' && buffer[i]!='%')
				tag[c++]=buffer[i++];
			tag[c]=0;
			while(buffer[i]==' ') i++;			
			if(buffer[i]=='%' && buffer[i+1]=='>') i+=2;
			else {
				fclose(f);
				printf("Error en tag %s, byte %d\n",tag,i);
				exit(8);
			}
			flag=i;
			sw=0;
			for(j=0;j<=num;j++)
				if(!strcmp(sust[j].de,tag)) {
					sw=1;
					fwrite(sust[j].a,1,strlen(sust[j].a),f);
					break;
				}
			if(!sw) {
				if(!strcmp(tag,"index")) {
					procesa_indice();
					flag=i;
				}
				else if(!strcmp(tag,"subindex")) {
					procesa_subindice(argv[1]);
				}
				else {
					fclose(f);
					printf("Error: tag no reconocido %s en byte %d\n",tag,i);
					exit(9);
				}
			}
			if(!strcmp(tag,"/title") && strcmp(argv[1],"index")) {
				char ant[256], sig[256], sup[256];
				char tant[512], tsig[512], tsup[512], tidx[512];
				situame(argv[1],ant,sig,sup);
				lee_titulo(ant,tant);
				lee_titulo(sig,tsig);
				lee_titulo(sup,tsup);
				lee_titulo("index",tidx);
				fprintf(f,"<table border=\"0\" width=\"100%%\">\n<tr><td align=\"left\" width=\"25%%\">");
				if(*ant!=0) fprintf(f,"<a href=\"%s.html\">Anterior</a><br>%s",ant,tant);
				else fprintf(f,"Anterior<br>&nbsp;");
				fprintf(f,"</td>\n<td align=\"center\" width=\"25%%\">");
				if(*sup!=0) fprintf(f,"<a href=\"%s.html\">Subir</a><br>%s",sup,tsup);
				else fprintf(f,"<a href=\"index.html\">Subir</a><br>%s",tidx);
				fprintf(f,"</td>\n<td align=\"center\" width=\"25%%\"><a href=\"index.html\">Contenido</a>"
					"<br>%s</td>\n<td align=\"right\" width=\"25%%\">",tidx);
				if(*sig!=0) fprintf(f,"<a href=\"%s.html\">Siguiente</a><br>%s",sig,tsig);
				else fprintf(f,"Siguiente<br>&nbsp;");
				fprintf(f,"</td></tr>\n</table><hr>\n");
			}
		}
	}

	fwrite(&buffer[flag],1,strlen(&buffer[flag]),f);

	fclose(f);

	printf("Guardado %s\n",nomfich);

	return 0;
}

void situame(char* actual, char* ant, char* sig, char* sup)
{
	FILE* fi;
	char* buf,tag[20],fixero[256];
	int tam,p,c;

	int nivel=0,ni;
	char anterior[256],superior[20][256];
	const char fin[]="<%/index%>";
	int j,sw=0,pillasig=0;

	*ant=0; *sig=0; *sup=0;

	if((fi=fopen("index.dok","r"))==NULL) {
		printf("Error: no puedo abrir index.dok\n");
		exit(13);
	}

	fseek(fi,0,SEEK_END);
	tam=ftell(fi);
	fseek(fi,0,SEEK_SET);

	if((buf=(char*)malloc(tam+1))==NULL) {
		printf("Error: memoria insuficiente\n");
		exit(14);
	}

	tam=fread(buf,1,tam,fi);
	fclose(fi);

	buf[tam]=0;

	for(p=0;p<tam;p++) {
		if(buf[p]=='<' && buf[p+1]=='%') {
			c=0; p+=2;
			while(buf[p]==' ') p++;			
			while(buf[p]!=' ' && buf[p]!='%')
				tag[c++]=buf[p++];
			tag[c]=0;
			while(buf[p]==' ') p++;			
			if(buf[p]=='%' && buf[p+1]=='>') p+=2;
			else {
				printf("Error en fichero index.dok, tag %s, byte %d\n",tag,p);
				exit(15);
			}
			if(!strcmp(tag,"index")) {
				sw=1;
				break;
			}
		}
	}

	if(!sw) {
		printf("Error: no se encuentra directiva <%%index%%> en index.dok\n");
		exit(16);
	}

	memset(fixero,0,256);
	memset(anterior,0,256);
	memset(superior,0,256*20);

	for(;p<tam;p++) {
		while(buf[p]=='<')
			while(buf[p-1]!='>' && p<tam) p++;
		while(buf[p]<33) p++;
		ni=0;
		for(j=0;j<10;j++)
			if(buf[p+j]!=fin[j]) break;
		if(j==10) {
			break;
		}
		if(buf[p]==';') {
			while(buf[p]>31 && p<tam) p++;
			continue;
		}
		while(buf[p]=='+') { ni++; p++; }
		if(ni>nivel) {
			if(ni==20) {
				printf("Error: el indice no puede tener mas de 20 niveles\n");
				exit(17);
			}
			
			if(!pillasig) { nivel=ni; strcpy(superior[nivel],fixero); }
		}
		if(ni<nivel) {
			if(!pillasig) nivel=ni;
		}
		c=0;
		while(buf[p]>31 && buf[p]!='<')
			fixero[c++]=buf[p++];
		fixero[c]=0;
		if(buf[p]=='<') p--;
		if(pillasig) {
			strcpy(ant,anterior);
			strcpy(sig,fixero);
			strcpy(sup,superior[nivel]);
			fclose(fi);
			free(buf);
			return;
		}
		if(!strcmp(actual,fixero)) pillasig=1;
		else strcpy(anterior,fixero);
	}
	if(pillasig) {
		strcpy(ant,anterior);
		strcpy(sup,superior[nivel]);
		fclose(fi);
		free(buf);
		return;
	}
	printf("Error: el documento no esta listado en el indice\n");
	exit(18);
}

void lee_titulo(char* fixero1, char* titulo)
{
	FILE* fi;
	char* buf,tag[20],fixero[256];
	int tam,p,c;

	strcpy(fixero,fixero1);
	strcat(fixero,".dok");

	if((fi=fopen(fixero,"rb"))==NULL) {
		sprintf(titulo,"[Error al abrir \"%s\"]",fixero);
		return;
	}

	fseek(fi,0,SEEK_END);
	tam=ftell(fi);
	fseek(fi,0,SEEK_SET);

	if((buf=(char*)malloc(tam+1))==NULL) {
		printf("Error: memoria insuficiente\n");
		exit(11);
	}

	tam=fread(buf,1,tam,fi);
	fclose(fi);

	buf[tam]=0;

	for(p=0;p<tam;p++) {
		if(buf[p]=='<' && buf[p+1]=='%') {
			c=0; p+=2;
			while(buf[p]==' ') p++;			
			while(buf[p]!=' ' && buf[p]!='%')
				tag[c++]=buf[p++];
			tag[c]=0;
			while(buf[p]==' ') p++;			
			if(buf[p]=='%' && buf[p+1]=='>') p+=2;
			else {
				printf("Error en fichero %s, tag %s, byte %d\n",fixero,tag,p);
				exit(13);
			}
			if(!strcmp(tag,"title")) {
				c=0;
				while(p<tam) {
					if(buf[p]=='<' && buf[p+1]=='%') break;
					titulo[c++]=buf[p++];
				}
				titulo[c]=0;
				free(buf);
				return;
			}
		}
	}

}

void procesa_indice()
{
	int nivel=0,c,ni;
	char fixero[256];
	char titulo[512];
	const char fin[]="<%/index%>";
	int j;

	fprintf(f,"\n");
	for(;i<tamano;i++) {
		while(buffer[i]=='<' && buffer[i+1]!='%')
			while(buffer[i-1]!='>' && i<tamano) i++;
		while(buffer[i]<33) i++;
		ni=0;
		for(j=0;j<10;j++)
			if(buffer[i+j]!=fin[j]) break;
		if(j==10) {
			for(j=0;j<nivel;j++)
				fprintf(f,"</ul>");
			fprintf(f,"\n");
			i+=10;
			return;
		}
		if(buffer[i]==';') {
			while(buffer[i]>31 && i<tamano) i++;
			continue;
		}
		while(buffer[i]=='+') { ni++; i++; }
		if(ni>nivel) {
			if(ni==20) {
				printf("Error: el indice no puede tener mas de 20 niveles\n");
				exit(25);
			}			
			for(j=nivel;j<ni;j++)
				fprintf(f,"<ul>");
			fprintf(f,"\n");
			nivel=ni;
		}
		if(ni<nivel) {
			for(j=ni;j<nivel;j++)
				fprintf(f,"</ul>");
			fprintf(f,"\n");
			nivel=ni;
		}
		c=0;
		while(buffer[i]>31 && buffer[i]!='<')
			fixero[c++]=buffer[i++];
		fixero[c]=0;
		if(buffer[i]=='<') i--;
		lee_titulo(fixero,titulo);
		if(nivel==0)
			fprintf(f,"<p class=\"indexheader\"><a href=\"%s.html\">%s</a></p>\n",fixero,titulo);
		else
			fprintf(f,"<li><a href=\"%s.html\">%s</a></li>\n",fixero,titulo);
	}
}

void procesa_subindice(char* actual)
{
	FILE* fi;
	char* buf,tag[20],fixero[256],titulo[512];
	int tam,p,c;

	int nivel=0,ni,iniv;
	const char fin[]="<%/index%>";
	int j,sw=0,pilla=0;

	if((fi=fopen("index.dok","r"))==NULL) {
		printf("Error: no puedo abrir index.dok\n");
		exit(19);
	}

	fseek(fi,0,SEEK_END);
	tam=ftell(fi);
	fseek(fi,0,SEEK_SET);

	if((buf=(char*)malloc(tam+1))==NULL) {
		printf("Error: memoria insuficiente\n");
		exit(20);
	}

	tam=fread(buf,1,tam,fi);
	fclose(fi);

	buf[tam]=0;

	for(p=0;p<tam;p++) {
		if(buf[p]=='<' && buf[p+1]=='%') {
			c=0; p+=2;
			while(buf[p]==' ') p++;			
			while(buf[p]!=' ' && buf[p]!='%')
				tag[c++]=buf[p++];
			tag[c]=0;
			while(buf[p]==' ') p++;			
			if(buf[p]=='%' && buf[p+1]=='>') p+=2;
			else {
				printf("Error en fichero index.dok, tag %s, byte %d\n",tag,p);
				exit(21);
			}
			if(!strcmp(tag,"index")) {
				sw=1;
				break;
			}
		}
	}

	if(!sw) {
		printf("Error: no se encuentra directiva <%%index%%> en index.dok\n");
		exit(22);
	}

	for(;p<tam;p++) {
		while(buf[p]=='<')
			while(buf[p-1]!='>' && p<tam) p++;
		while(buf[p]<33) p++;
		ni=0;
		for(j=0;j<10;j++)
			if(buf[p+j]!=fin[j]) break;
		if(j==10) {
			break;
		}
		if(buf[p]==';') {
			while(buf[p]>31 && p<tam) p++;
			continue;
		}
		while(buf[p]=='+') { ni++; p++; }
		if(ni>nivel) {
			if(ni==20) {
				printf("Error: el indice no puede tener mas de 20 niveles\n");
				exit(23);
			}
			if(pilla && ni==iniv+1) {
					fprintf(f,"<ul>");
				fprintf(f,"\n");
			}
			nivel=ni;
		}
		if(ni<nivel) {
			nivel=ni;
			if(pilla && nivel<=iniv) 
				break;
		}
		c=0;
		while(buf[p]>31 && buf[p]!='<')
			fixero[c++]=buf[p++];
		fixero[c]=0;
		if(buf[p]=='<') p--;
		if(!strcmp(actual,fixero)) {
			pilla=1;
			fprintf(f,"<ul>");
			iniv=nivel;
		}
		if(pilla && nivel<=iniv+1) {
			lee_titulo(fixero,titulo);
			fprintf(f,"<li><a href=\"%s.html\">%s</a></li>\n",fixero,titulo);
		}
	}
	if(pilla) {
			fprintf(f,"</ul>");
			fprintf(f,"</ul>");
		fprintf(f,"\n");
		return;
	}
	printf("Error: el documento no esta listado en el indice\n");
	exit(24);
}