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

/* OJITO: _encriptar conserva el fichero original y crea temp.dj! desencriptado
 *        _comprimir descomprime (y reemplaza) el archivo indicado (temp.dj!)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "encrypt.h"

#include "shared.h" // Contiene la clave de cifrado

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

int rnd2(int min,int max) 
{
	if (max<min) {
	  max^=min; 
	  min^=max; 
	  max^=min; 
	}
	
	return((rnd_coder()%(max-min+1))+min);
}

void init_rnd_coder(int n, char * clave)
{
	register int a;
	
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

//อออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//  Funciขn de encriptaciขn/desencriptaciขn
//อออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

void coder(byte * ptr, int len, char * clave) 
{
	int n;

	init_rnd_coder(len,clave);
	
	for (n=0;n<len;n++)
		ptr[n]^=rndb();
}


void _encriptar(int encode, char * fichero, char * clave) 
{
	int n,size;
	byte *ptr,*p;
	FILE *f;
	
	if ((f=fopen(fichero,"rb"))!=NULL) {
		fseek(f,0,SEEK_END); 
		size=ftell(f);
    
		if ((ptr=(byte *)malloc(size))!=NULL) {
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
			p=ptr+18; 
			size-=18;
		}
	}

  init_rnd_coder(size+1133,clave);
  
  for (n=0;n<size;n++)
	  p[n]^=rndb();


  if ((f=fopen("temp.dj!","wb"))==NULL) {
	  free(ptr);
	  return;
  }

  if (encode) {
    if(fwrite(magic,1,14,f)!=14) {
      fclose(f);
      remove(fichero);
      rename("temp.dj!",fichero); free(ptr); return;
    }
  }

  if(fwrite(p,1,size,f)!=(unsigned int)size) {
    fclose(f);
	free(ptr); return;
  }

  // Si todo ha ido bien ...

  fclose(f);
  free(ptr);

}

void _comprimir(int encode, char *fichero) {

	unsigned long size,size2;
	byte *ptr,*ptr_dest;
	FILE *f;

	if ((f=fopen(fichero,"rb"))!=NULL) {
		fseek(f,0,SEEK_END); 
		size=ftell(f);
	
		if ((ptr=(byte *)malloc(size))!=NULL) {
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
		size2=size+size/100+256;
		
		if ((ptr_dest=(byte *)malloc(size2))==NULL) {
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
		
		if ((ptr_dest=(byte *)malloc(size2))==NULL) {
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
