#include <stdio.h>
#include <ctype.h>

#include <export.h>
#include "conv.h"

/************************/
/* char()       *       */
/* lower()      * HECHA */
/* strcat()     * HECHA */
/* strchr()     * HECHA */
/* strcmp()     * HECHA */
/* strcpy()     * HECHA */
/* strdel()     *       */
/* strlen()     * HECHA */
/* strset()     * HECHA */
/* strstr()     * HECHA */
/* upper()      * HECHA */
/* calculate()  * HECHA */
/* fcalculate() * HECHA */ 
/* itoa()       *       */
/* asc()        *       */ /* [<CicTec> text=ASC(65); // test="A";] (Una peticion suya :). */
/* ftoa()       *       */ /* Para los nuevos FLOAT :) */
/************************/
  
/* Rutinas eDiv! :) */
char *DIV_strcpy(FUNCTION_PARAMS);
size_t DIV_strlen(FUNCTION_PARAMS);
int DIV_strcmp(FUNCTION_PARAMS);
char *DIV_strchr(FUNCTION_PARAMS);
char *DIV_strcat(FUNCTION_PARAMS);
char *DIV_strstr(FUNCTION_PARAMS);
int DIV_atoi(FUNCTION_PARAMS);
float DIV_atof(FUNCTION_PARAMS);
char *DIV_lower(FUNCTION_PARAMS);
char *DIV_upper(FUNCTION_PARAMS);
char *DIV_strset(FUNCTION_PARAMS);
char *DIV_itoa(FUNCTION_PARAMS);

/* Rutinas internas */
static int hexval(char c);

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

	/* Estas dos son iguales, lo sengundo es por una standarizacion :) */
	FUNCTION("calculate",1,DIV_atoi);
	FUNCTION("atoi",1,DIV_atoi);
	/* Idem a lo anterior, pero para los datos float :) */
	FUNCTION("fcalculate",1,DIV_atof);
	FUNCTION("atof",1,DIV_atof);

	FUNCTION("lower",1,DIV_lower);
	FUNCTION("upper",1,DIV_upper);

	return TRUE;
}

char *DIV_strcpy(FUNCTION_PARAMS)
{
	char *src=(char *)getparm();
	char *dest=(char *)getparm();

	char *tmp = dest;

	while ((*dest++ = *src++) != '\0');
	return tmp;
}

size_t DIV_strlen(FUNCTION_PARAMS)
{
	char *s = (char *)getparm();
	const char *sc;

	for(sc = s; *sc != '\0'; ++sc);
	return sc - s;
}

char *DIV_strset(FUNCTION_PARAMS)
{
	int valor=getparm();
	char *cadena=(char *)getparm();

    char *inicio=cadena;
	
	while (*cadena)
		*cadena++ = (char)valor;
	
	return inicio;
}

int DIV_strcmp(FUNCTION_PARAMS)
{
	const char *ct = (const char *)getparm();
	const char *cs = (const char *)getparm();

	signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

char *DIV_strchr(FUNCTION_PARAMS)
{
	int c = getparm();
	const char *s = (const char *)getparm();

	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}

char *DIV_strcat(FUNCTION_PARAMS)
{
	const char *src = (const char *)getparm();
	char *dest = (char *)getparm();

	char *tmp = dest;

	while(*dest)
		dest++;
	while((*dest++ = *src++) != '\0')
		;
	return tmp;
}

char *DIV_strstr(FUNCTION_PARAMS)
{
	const char *s2 = (const char*)getparm();
	const char *s1 = (const char*)getparm();

	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	l1 = strlen(s1);
	while(l1 >= l2) {
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *)s1;
		s1++;
	}
	return NULL;
}

int DIV_atoi(FUNCTION_PARAMS)
{
	const char *num = (const char*)getparm();
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

float DIV_atof(FUNCTION_PARAMS)
{
	const char *num = (const char*)getparm();
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

char *DIV_lower(FUNCTION_PARAMS)
{
	int i;
	char *str=(char*)getparm();

	if (!i_lower)
		inicializa_lower();

	for (i=0;str[i]!=0;i++) {
		if (lower[str[i]]!=0) str[i]=lower[str[i]]; 
	}
	
	return str;
}

char *DIV_upper(FUNCTION_PARAMS)
{
	int i;
	char *str=(char*)getparm();

	if (!i_upper)
		inicializa_upper();

	for (i=0;str[i]!=0;i++) {
		if (upper[str[i]]!=0) str[i]=upper[str[i]]; 
	}
	
	return str;
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

