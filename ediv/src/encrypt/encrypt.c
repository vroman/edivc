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

// ¡ojo! versión capada, no devuelve errores

#ifdef WIN32
#  define el_stub "stub.exe"
#else
#  define el_stub "stub"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "shared.h"

int stub_size;

void _encriptar(int encode, char * fichero, char * clave);
void _comprimir(int encode, char *fichero);

int main(int argc, char * argv[])
{
	if(argc<2) {
		printf("Modo de uso: %s <libreria> [flag_encriptar]\n",argv[0]);
		exit(1);
	}
	if(argc==2) {
		_comprimir(1,el_stub);
		_encriptar(1,el_stub,la_clave);
		rename(el_stub,argv[1]);
	}
	else {
		_encriptar(0,argv[1],la_clave);
		_comprimir(0,argv[1]);
		rename(argv[1],el_stub);
		#if defined(__linux__) || defined(__APPLE__)
			chmod(el_stub,755);		// -rwxr-xr-x
		#endif
	}
 	printf("STUB_SIZE: %d\n",stub_size);
	return 0;
}


union {
	byte b[128];
	int d[32];
} seed_coder; // Seed aleatorio (127 bytes + PTR)

byte rndb(void) 
{
	byte ptr;
	ptr=seed_coder.b[127];
	
	if ((seed_coder.b[127]=(ptr+seed_coder.b[ptr])&127)==127)
		seed_coder.b[127]=0;
	return(seed_coder.b[seed_coder.b[127]]+=++ptr);
}

int rnd_coder(void) 
{
	return((((rndb()&127)*256+rndb())*256+rndb())*256+rndb());
}

int rnd2(int min,int max) {
	if (max<min) {
		max^=min; 
		min^=max; 
		max^=min; 
	}
  return((rnd_coder()%(max-min+1))+min);
}

void init_rnd_coder(int n, char * clave)
{
	register a;
	
	for (a=0;a<32;a++) 
		seed_coder.d[a]=n;
	
	if (clave!=NULL) {
		for (a=0;a<127;a++) {
			if (!clave[a]) 
				break;
			seed_coder.b[a]^=clave[a];
		}
	}
	
	seed_coder.b[127]&=127;

	for (a=0;a<2048;a++) 
		rndb();
}

//ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
//  Funci¢n de encriptaci¢n/desencriptaci¢n
//ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

void coder(byte * ptr, int len, char * clave) 
{
	int n;
	
	init_rnd_coder(len,clave);
	
	for (n=0;n<len;n++) {
		ptr[n]^=rndb();
	}
}


void _encriptar(int encode, char * fichero, char * clave) 
{
	int n,size;
	byte *ptr,*p;
	FILE *f;
	
	if ((f=fopen(fichero,"rb"))!=NULL) {
		fseek(f,0,SEEK_END); 
		size=ftell(f);
		if ((ptr=malloc(size))!=NULL) {
			fseek(f,0,SEEK_SET);
			if(fread(ptr,1,size,f) ==(unsigned int) size) {
				fclose(f);
			} else {
				fclose(f);
				free(ptr); 
				return;
			}
		} else {
			fclose(f);
			return; 
		}
	} else {
		return;
	}

	if (encode) {
		if (!strcmp(ptr,magic)) 
			return;
		else
			p=ptr;
	} else {
		if (strcmp(ptr,magic)) 
			return; 
		else { 
			p=ptr+12;
			size-=12;
		}
	}
	
	init_rnd_coder(size+1133,clave);
	
	for (n=0;n<size;n++) p[n]^=rndb();
	
	if (rename(fichero,"temp.dj!")) {
		free(ptr); 
		return;
	}

	if ((f=fopen(fichero,"wb"))==NULL) {
		rename("temp.dj!",fichero);
		free(ptr);
		return;
	}

	if (encode) {
		if(fwrite(magic,1,14,f)!=14) {
			fclose(f);
			remove(fichero);
			rename("temp.dj!",fichero); 
			free(ptr); 
			return;
		}
		
		if(fwrite(&stub_size,1,4,f)!=4) {
			fclose(f);
			remove(fichero);
			rename("temp.dj!",fichero);
			free(ptr);
			return;
		}
	}

	if(fwrite(p,1,size,f)!=(unsigned int)size) {
		fclose(f);
		remove(fichero);
		rename("temp.dj!",fichero); 
		free(ptr);
		return;
	}

	// Si todo ha ido bien ...
	fclose(f);
	free(ptr);
	remove("temp.dj!");
}

void _comprimir(int encode, char *fichero) 
{
	unsigned long size,size2;
	byte *ptr,*ptr_dest;
	FILE *f;

	if ((f=fopen(fichero,"rb"))!=NULL) {
		fseek(f,0,SEEK_END); 
		size=ftell(f);
		if ((ptr=malloc(size))!=NULL) {
			fseek(f,0,SEEK_SET);
			if(fread(ptr,1,size,f) == size) {
				fclose(f);
			} else {
				fclose(f);
				free(ptr);
				return; 
			}
		} else { 
			fclose(f);
			return; 
		}
	} else 
		return;

	if (encode) {
		if (!strcmp(ptr,"zx!\x1a\x0d\x0a\xff")) 
			return;
		stub_size=(int)size;
		size2=size+size/100+256;
		if ((ptr_dest=malloc(size2))==NULL) {
			free(ptr);
			return;
		}
		
		if (compress(ptr_dest, &size2, ptr, size)) {
			free(ptr_dest);
			free(ptr);
			return;
		}

		// Si no se gana espacio, se deja el fichero sin comprimir

		if (size2>=size-12) { 
			free(ptr_dest);
			free(ptr);
			return;
		}
	} else {
		if (strcmp(ptr,"zx!\x1a\x0d\x0a\xff")) 
			return;
		size2=*(int*)(ptr+8);
		stub_size=(int)size2;
		if ((ptr_dest=malloc(size2))==NULL) {
			free(ptr); 
			return;
		}
		
		if (uncompress(ptr_dest, &size2, ptr+12, size-12)) {
			free(ptr_dest);
			free(ptr);
			return;
		} 
		size2=*(int*)(ptr+8);
	}

	free(ptr);

	if (rename(fichero,"temp.ZX!")) {
		free(ptr_dest);
		return;
	}

	if ((f=fopen(fichero,"wb"))==NULL) {
		rename("temp.ZX!",fichero); 
		free(ptr_dest);
		return;
	}

	if (encode) {
		if(fwrite("zx!\x1a\x0d\x0a\xff",1,8,f)!=8) {
			fclose(f);
			remove(fichero);
			rename("temp.ZX!",fichero); 
			free(ptr_dest);
			return;
		}
		if(fwrite(&size,1,4,f)!=4) {
			fclose(f);
			remove(fichero);
			rename("temp.ZX!",fichero);
			free(ptr_dest);
			return;
		}
	}
	
	if(fwrite(ptr_dest,1,size2,f)!=size2) {
		fclose(f);
		remove(fichero);
		rename("temp.ZX!",fichero);
		free(ptr_dest);
		return;
	}

	// Si todo ha ido bien ...

	fclose(f);
	free(ptr_dest);
	remove("temp.ZX!");
}
