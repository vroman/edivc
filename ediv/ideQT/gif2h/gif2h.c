#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int tamano;
	FILE *f;
	FILE *f2;
	int i;
	char h[256];
	char* a;
	unsigned char b;

	if(argc!=2) {
		printf("Usar: %s <fichero.ext>\n",argv[0]);
		exit(1);
	}

	if((f=fopen(argv[1],"rb"))==NULL) {
		printf("Error abriendo archivo\n");
		exit(1);
	}

	fseek(f,0,SEEK_END);
	tamano=ftell(f);
	fseek(f,0,SEEK_SET);

	strcpy(h,argv[1]);
	a=strchr(h,'.');
	*(a+1)='h';
	*(a+2)=0;

	if((f2=fopen(h,"w"))==NULL) {
		printf("Error escribiendo %s\n",h);
		fclose(f);
		exit(1);
	}

	*(strchr(argv[1],'.'))='_';

	fprintf(f2,"static unsigned char %s[]={\n",argv[1]);
		
	for(i=0;i<tamano;i++) {
		fread(&b,1,1,f);
		fprintf(f2,"%d",b);
		if(i<tamano-1) fputc(',',f2);
		if(i%20==19) fputc('\n',f2);
	}
	fprintf(f2,"\n};\n");

	fclose(f);
	fclose(f2);

	return 0;
}