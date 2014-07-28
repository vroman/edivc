#include <stdio.h>
#include <ctype.h>

#include "export.h"
#include "conv.h"

/************************/
/* char()       * HECHA */
/* lower()      * HECHA */
/* strcat()     * HECHA */
/* strchr()     * HECHA */
/* strcmp()     * HECHA */
/* strcpy()     * HECHA */
/* strdel()     * HECHA */
/* strlen()     * HECHA */
/* strset()     * HECHA */
/* strstr()     * HECHA */
/* upper()      * HECHA */
/* calculate()  *       */
/* fcalculate() *       */ 
/* itoa()       * HECHA */
/* asc()        *       */ /* [<CicTec> text=ASC(65); // test="A";] (Una peticion suya :). */
/* ftoa()       *       */ /* Para los nuevos FLOAT :) */
/************************/
  
/* Rutinas eDiv! :) */
int DIV_strcpy(FUNCTION_PARAMS);
int DIV_strlen(FUNCTION_PARAMS);
int DIV_strset(FUNCTION_PARAMS);
int DIV_strcmp(FUNCTION_PARAMS);
int DIV_strchr(FUNCTION_PARAMS);
int DIV_strcat(FUNCTION_PARAMS);
int DIV_strstr(FUNCTION_PARAMS);
int DIV_atoi(FUNCTION_PARAMS);
float DIV_atof(FUNCTION_PARAMS);
int DIV_lower(FUNCTION_PARAMS);
int DIV_upper(FUNCTION_PARAMS);
int DIV_char(FUNCTION_PARAMS);
int DIV_strdel(FUNCTION_PARAMS);
int DIV_itoa(FUNCTION_PARAMS);

/* Rutinas internas */
static int hexval(char c);
void strdelbeg(char * s,int n);
void strdelend(char * s,int n);

/* Variables */
byte i_lower=0; /* lower ya esta inicializado? */
byte i_upper=0; /* upper ya esta inicializado? */

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	FUNCTION("strcpy",2,DIV_strcpy);
	FUNCTION("strlen",1,DIV_strlen);
	FUNCTION("strcmp",2,DIV_strcmp);
	FUNCTION("strchr",2,DIV_strchr);
	FUNCTION("strcat",2,DIV_strcat);
	FUNCTION("strstr",2,DIV_strstr);
	FUNCTION("strset",2,DIV_strset);
	FUNCTION("strdel",3,DIV_strdel);

//	FUNCTION("calculate",1,DIV_calculate);
	FUNCTION("atoi",1,DIV_atoi);

//	FUNCTION("fcalculate",1,DIV_fcalculate);
	FUNCTION("atof",1,DIV_atof);

	FUNCTION("itoa",1,DIV_itoa);

	FUNCTION("lower",1,DIV_lower);
	FUNCTION("upper",1,DIV_upper);

	FUNCTION("char",1,DIV_char);

	return TRUE;
}

int DIV_strcpy(FUNCTION_PARAMS)
{
	int src=getparm();
	int dest=getparm();

	if ((fp->mem[dest-1]&0xFFF00000)!=0xDAD00000) {
		fp->Runtime_Error(164);	/* Cadena destino inválida */
		return 0;		
	}
	if ((unsigned)src>255) {
		if ((unsigned)(fp->mem[dest-1]&0xFFFFF)+1<strlen((char*)&fp->mem[src])) {
			fp->Runtime_Error(140); /* Acceso fuera de rango */
			return 0;
		}
		strcpy((char*)&fp->mem[dest],(char*)&fp->mem[src]);
	}
	else
		fp->mem[dest]=src;

	return dest;
}

int DIV_strlen(FUNCTION_PARAMS)
{
	int s = getparm();

	if ((unsigned)s>255)
		return (int)strlen((char*)&fp->mem[s]);
	else
		return 1;
}

int DIV_strset(FUNCTION_PARAMS)
{
	int valor=getparm();
	int cadena=getparm();
	int n;

	if ((fp->mem[cadena-1]&0xFFF00000)!=0xDAD00000) {
		fp->Runtime_Error(164); /* Cadena destino inválida */
		return 0;
	}
	n=(fp->mem[cadena-1]&0xFFFFF)+1;
	if ((unsigned)valor>255)
		memset((char*)&fp->mem[cadena],(char)fp->mem[valor],n);
	else
		memset((char*)&fp->mem[cadena],(char)valor,n);

	return cadena;
}

int DIV_strcmp(FUNCTION_PARAMS)
{
	int s1 = getparm();
	int s2 = getparm();

	if ((unsigned)s2>255) {
		if ((unsigned)s1>255) {
			return strcmp((char*)&fp->mem[s2],(char*)&fp->mem[s1]);
		} else {
			return strcmp((char*)&fp->mem[s2],(char*)&s1);
		}
	} else {
		if ((unsigned)s1>255) {
			return strcmp((char*)&s2,(char*)&fp->mem[s1]);
		} else {
			return strcmp((char*)&s2,(char*)&s1);
		}
	}
}

int DIV_strchr(FUNCTION_PARAMS)
{
	int c = getparm();
	int s = getparm();

	char * p;

	if ((unsigned)c>255)
		p=strpbrk((char*)&fp->mem[s],(char*)&fp->mem[c]);
	else
		p=strchr((char*)&fp->mem[s],(char)c);

	if (p!=NULL)
		return (int)(p-(char*)&fp->mem[s]);
	else
		return -1;

}

int DIV_strcat(FUNCTION_PARAMS)
{
	int src = getparm();
	int dest = getparm();

	int n;

	if ((fp->mem[dest-1]&0xFFF00000)!=0xDAD00000) {
		fp->Runtime_Error(164); /* Cadena destino inválida */
		return 0;
	}

	if ((unsigned)src>255)
		n=strlen((char*)&fp->mem[src]);
	else
		n=1;

	if ((fp->mem[dest-1]&0xFFFFF)+1<strlen((char*)&fp->mem[dest])+n) {
		fp->Runtime_Error(140); /* Acceso fuera de rango */
		return 0;
	}

	if ((unsigned)src>255)
		strcat((char*)&fp->mem[dest],(char*)&fp->mem[src]);
	else
		sprintf((char*)&fp->mem[dest],"%s%c\0",(char*)&fp->mem[dest],src);

	return dest;
}

int DIV_strstr(FUNCTION_PARAMS)
{
	int s2 = getparm();
	int s1 = getparm();

	char * p;

	if ((unsigned)s2>255)
		p=strstr((char*)&fp->mem[s1],(char*)&fp->mem[s2]);
	else
		p=strchr((char*)&fp->mem[s1],(char)s2);

	if (p!=NULL)
		return (int)(p-(char*)&fp->mem[s1]);
	else
		return -1;
}

/* NUEVA */
int DIV_atoi(FUNCTION_PARAMS)
{
	char *num = getstrparm();
	int value = 0;
	if (num[0] == '0' && num[1] == 'x') {
		/* hexadecimal */
		num += 2;
		while (*num && isxdigit(*num))
			value = value * 16 + hexval(*num++);
	} else {
		/* decimal */
		while (*num && isdigit(*num))
			value = value * 10 + *num++  - '0';
	}

	return value;
}

/* NUEVA */
float DIV_atof(FUNCTION_PARAMS)
{
	char *num = getstrparm();
	float value = 0;
	if (num[0] == '0' && num[1] == 'x') {
		/* hexadecimal */
		num += 2;
		while (*num && isxdigit(*num))
			value = value * 16 + hexval(*num++);
	} else {
		/* decimal */
		while (*num && isdigit(*num))
			value = value * 10 + *num++  - '0';
	}

	return value;
}

int DIV_lower(FUNCTION_PARAMS)
{
	int str=getparm();
	int n;

	byte* memb=(byte*)fp->mem;

	if (!i_lower)
		inicializa_lower();

	if ((unsigned)str>255) {
		n=strlen((char*)&fp->mem[str]);
		while (n--) {
			if (lower[memb[str*4+n]]!=0)
				memb[str*4+n]=lower[memb[str*4+n]];
		}
	}
	else {
		if (lower[(char)str]!=0)
			return (int)lower[(char)str];
	}
	return str;
}

int DIV_upper(FUNCTION_PARAMS)
{
	int str=getparm();
	int n;

	byte* memb=(byte*)fp->mem;

	if (!i_upper)
		inicializa_upper();

	if ((unsigned)str>255) {
		n=strlen((char*)&fp->mem[str]);
		while (n--) {
			if (upper[memb[str*4+n]]!=0)
				memb[str*4+n]=upper[memb[str*4+n]];
		}
	}
	else {
		if (upper[(char)str]!=0)
			return (int)upper[(char)str];
	}
	return str;
}

int DIV_char(FUNCTION_PARAMS)
{
	int s=getparm();
	char* memb=(char*)fp->mem;

	if ((unsigned)s>255)
		return (int)memb[s*4];
	else
		return s;
}

int DIV_strdel(FUNCTION_PARAMS)
{
	int m=getparm();
	int n=getparm();
	int str=getparm();

	if ((fp->mem[str-1]&0xFFF00000)!=0xDAD00000) {
		fp->Runtime_Error(164); /* Cadena destino inválida */
		return str;
	}

	if ((fp->mem[str-1]&0xFFFFF)+1<strlen((char*)&fp->mem[str])-n-m) {
		fp->Runtime_Error(140); /* Acceso fuera de rango */
		return str;
	}

	if (n>m) {
		/* Borra primero del inicio */
		strdelbeg((char*)&fp->mem[str],n);
		strdelend((char*)&fp->mem[str],m);
	}
	else {
		/* Borra primero del final */
		strdelend((char*)&fp->mem[str],m);
		strdelbeg((char*)&fp->mem[str],n);
	}

	return str;
}

int DIV_itoa(FUNCTION_PARAMS)
{
	int n=getparm();
	sprintf((char*)&fp->mem[fp->nullstring[*fp->nstring]], "%d", n);
	n=fp->nullstring[*fp->nstring];
	*fp->nstring=(((*fp->nstring)+1)&3);
	return n;
}

/* Rutinas internas */
static int hexval(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
		
	return 0;
}

void strdelbeg(char * s,int n)
{
	int len=strlen(s);
	if (n>0) {
		if (n>=len)
			*s=0;
		else
			memmove(s,s+n,len+1-n);
	}
	else if (n<0) {
		memmove(s-n,s,len+1);
		memset(s,' ',-n);
	}
}

void strdelend(char * s,int n)
{
	int len=strlen(s);
	if (n>0) {
		if (n>=len)
			*s=0;
		else
			s[len-n]=0;
	}
	else if (n<0) {
		n=len-n;
		for (;len<n;len++)
			s[len]=' ';
		s[len]=0;
	}
}

