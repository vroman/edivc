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

/* EDIVSTD.DLL
 * Esta librería contiene los datos más básicos que puede requerir un programa
 * DIV, tal como las opciones de compilación, estructura reserved, etc.
 */
#include <stdio.h>
#include <export.h>
#include "math.h"

#include "main.h"

int seno[90001] ;

/*int DllMain()
{
	//printf("Cargada DLL: SAMPLE\n");

	// Si se quiere que la DLL se cargue, hay que devolver true
	return TRUE;
}*/

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{

	FUNCTION("abs",1,eDiv_Abs);
	FUNCTION("pow",2,eDiv_Pow);
	FUNCTION("sqrt",1,eDiv_Sqrt);
	FUNCTION("rand",2,eDiv_Rand);
	FUNCTION("rand_seed",1,eDiv_RandSeed) ;
	FUNCTION("sin",1,eDiv_Sin) ;
	FUNCTION("cos",1,eDiv_Cos) ;
	FUNCTION("tan",1,eDiv_Tan) ;
	FUNCTION("get_angle",1,eDiv_GetAngle) ;
	FUNCTION("get_dist",1,eDiv_GetDist) ;
	FUNCTION("get_distx",2,eDiv_GetDistX) ;
	FUNCTION("get_disty",2,eDiv_GetDistY) ;
	FUNCTION("fget_angle",4,eDiv_FgetAngle) ;
	FUNCTION("fget_dist",4,eDiv_FgetDist) ;
	FUNCTION("near_angle",3,eDiv_NearAngle) ;
	FUNCTION("cpysign",2,eDiv_cpysign) ;
	FUNCTION("max",2,eDiv_max) ;
	FUNCTION("min",2,eDiv_min) ;
	FUNCTION("xmax",3,eDiv_xmax) ;
	FUNCTION("xmin",3,eDiv_xmin) ;
	FUNCTION("xmid",3,eDiv_xmid) ;
	ENTRYPOINT(first_load) ;

	return TRUE;
}


    

int eDiv_Abs(FUNCTION_PARAMS)
{
	int a = getparm() ;
	if ( a < 0 ) a = -a ;
	return a ;
}

int eDiv_Pow(FUNCTION_PARAMS)
{
	int a,b,c,n,i ;
	b = getparm() ;
	a = getparm() ;
	n = 1 ;
	c = b ;	
	if ( c < 0 ) c = -c ;
	for ( i = 1 ; i < c ; i++ )
	{
		n *= a ;
	}
	if ( b < 0 )
		n = 1 / n ;
	return n ;
}

int eDiv_Sqrt(FUNCTION_PARAMS)
{
	int a ;
	a = getparm() ;
	return sqrt( a ) ;   
}

int eDiv_Rand(FUNCTION_PARAMS)
{
	int a,b,n,i ;
	b = getparm() ;
	a = getparm() ;
	n = (int)(rand()%(b-a+1))+a ;
	return n ;
}

int eDiv_RandSeed(FUNCTION_PARAMS)
{
	int a ;
	a = getparm() ;
	if ( a == 0 )
		srand(time(NULL)) ;
	else
		srand(a) ;
	return 1 ;
}

int eDiv_Sin(FUNCTION_PARAMS)
{
	int a ;
	a = getparm() ;
	a = prepara_angulo(a) ;
	if ( a <= 90000 )
		return seno[a];
	if ( a <= 180000 )
		return seno[180000-a] ;
	if ( a <= 270000 )
		return -seno[a-180000] ;
	return -seno[360000-a] ;
}

int eDiv_Cos(FUNCTION_PARAMS)
{
	int a ;
	a = getparm() ;
	a = prepara_angulo(a) ;
	if ( a <= 90000 )
		return seno[90000-a];
	if ( a <= 180000 )
		return -seno[a-90000] ;
	if ( a <= 270000 )
		return -seno[270000-a] ;
	return seno[a-270000] ;
}

int eDiv_Tan(FUNCTION_PARAMS)
{
	int a , b , c;
	a = getparm() ;
	a = prepara_angulo(a) ;
	if ( a <= 90000 )
		b = seno[a];
	else
		if ( a <= 180000 )
			b = seno[180000-a] ;
		else
			if ( a <= 270000 )
				b = -seno[a-180000] ;
			else
				b = -seno[360000-a] ;
	if ( a <= 90000 )
		c = seno[90000-a];
	else
		if ( a <= 180000 )
			c = -seno[a-90000] ;
		else
			if ( a <= 270000 )
				c = -seno[270000-a] ;
			else
				c = seno[a-270000] ;

	if ( c == 0 )
		return 0x7FFFFFFF ;
	else
		return ( (int)(b/c) ) ;
	return 0 ;
}

	

int eDiv_GetAngle(FUNCTION_PARAMS)
{
	int id1 , id2 ;
	int x1 , x2 , y1, y2 ;
	int a = getparm() ;
	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;
	id2 = a ;
	x1 = local("x",id1) ;
	y1 = local("y",id1) ;
	x2 = local("x",id2) ;
	y2 = local("y",id2) ;
	if ( (x2-x1) != 0 )
	{
		a = atan( (double)((y2-y1) / (x2-x1)) ) / PIOVER180 ;
		if ( x2 < x1 )
			a+= 180 ;
	}else
		if ( y2 > y1 )
			a = 90 ;
		else
			a = 270 ;
	a *= 1000 ;
	a = prepara_angulo(a) ;
    return a ;
}

int eDiv_GetDist(FUNCTION_PARAMS)
{
	int id1 , id2 ;
	int x1 , x2 , y1, y2 ;
	int a = getparm() ;
	id1 = fp->procs_s[ fp->proc_orden[*fp->proceso_actual] ].id ;
	id2 = a ;
	x1 = local("x",id1) ;
	y1 = local("y",id1) ;
	x2 = local("x",id2) ;
	y2 = local("y",id2) ;
	a = sqrt( pow( x2-x1 , 2 ) + pow( y2-y1 , 2 ) ) ;
	return a ;
}

int eDiv_GetDistX(FUNCTION_PARAMS)
{
	int a , b ;
	b = getparm() ; //distancia
	a = getparm() ; //angulo
	a = (int) b* cos( (a/1000)*PIOVER180 ) ;
	return a ;
}

int eDiv_GetDistY(FUNCTION_PARAMS)
{
	int a , b ;
	b = getparm() ; //distancia
	a = getparm() ; //angulo
	a = (int) b* sin( (a/1000)*PIOVER180 ) ;
	return a ;
}

int eDiv_FgetAngle(FUNCTION_PARAMS)
{
	int x1 , y1 , x2 , y2 , a ;
	y2 = getparm() ;
	x2 = getparm() ;
	y1 = getparm() ;
	x1 = getparm() ;
	if ( (x2-x1) != 0 )
	{
		a = atan( (double)((y2-y1) / (x2-x1)) ) / PIOVER180 ;
		if ( x2 < x1 )
			a+= 180 ;
	}else
		if ( y2 > y1 )
			a = 90 ;
		else
			a = 270 ;
	a *= 1000 ;
	a = prepara_angulo(a) ;
    return a ;
}	

int eDiv_FgetDist(FUNCTION_PARAMS)
{
	int x1 , y1 , x2 , y2 , a;
	y2 = getparm() ;
	x2 = getparm() ;
	y1 = getparm() ;
	x1 = getparm() ;
	a = sqrt( pow( x2-x1 , 2 ) + pow( y2-y1 , 2 ) ) ;
	return a ;
}

int eDiv_NearAngle(FUNCTION_PARAMS)
{
	int a , b , c ;
	c = getparm() ;
	b = getparm() ;
	a = getparm() ;
	a = prepara_angulo(a) ;
	b = prepara_angulo(b) ;
	c = prepara_angulo(c) ;
	if ( abs(a - b) <= c )
		return b ;
	if ( a < b )
	{
		if ( abs( b-a ) < 180000 )
		{
			a = prepara_angulo( a+c ) ;
		}else
		{
			a = prepara_angulo( a-c ) ;
		}
		return a ;
	}else
	{
		if ( abs( b-a ) < 180000 )
		{
			a = prepara_angulo( a-c ) ;
		}else
		{
			a = prepara_angulo( a+c ) ;
		}
		return a ;
	}
	return 0 ;
}

int eDiv_cpysign(FUNCTION_PARAMS)
{
	int s = getparm() ;
	int n = getparm() ;
	return ((s<0)?((n<0)?(n):(-n)):((n>=0)?(n):(-n)));
}

int eDiv_max(FUNCTION_PARAMS)
{
	int n2 = getparm() ;
	int n1 = getparm() ;
	return ((n1>n2)?(n1):(n2));
}

int eDiv_min(FUNCTION_PARAMS)
{
	int n2 = getparm() ;
	int n1 = getparm() ;
	return ((n1<n2)?(n1):(n2));
}

int eDiv_xmax(FUNCTION_PARAMS)
{
	int n3 = getparm() ;
	int n2 = getparm() ;
	int n1 = getparm() ;

	int m=n1;
	if(n2>m) m=n2;
	if(n3>m) m=n3;	
	return m;
}

int eDiv_xmin(FUNCTION_PARAMS)
{
	int n3 = getparm() ;
	int n2 = getparm() ;
	int n1 = getparm() ;

	int m=n1;
	if(n2<m) m=n2;
	if(n3<m) m=n3;	
	return m;
}

int eDiv_xmid(float n1,float n2,float n3){
	if((n1<n2 && n1<n3  && n2<n3)  ||
	   (n1>n2 && n1>n3  && n2>=n3) || 
	   (n1>n2 && n1==n3 && n2<n3)) 
		return n2;	
	if((n1<n2  && n1<n3 && n2>n3) || 
	   (n1>n2  && n1>n3 && n2<n3) ||
	   (n1==n2 && n1>n3 && n2>n3)) 
		return n3;
	return n1;
}


//------------------------------ ENTRYPOINTS -------------------------------------------

void first_load()
{
	int i ;
	//calculamos los senos del 0 al 90000
	for ( i = 0 ; i < 90000 ; i++ )
	{
		seno[i] = ftomil( sin( (i / 1000)*PIOVER180 ) );
	}
	seno[90000] = 0x7FFFFFFF ;
}
	

//**************************** FUNCIONES INTERNAS DE LA DLL ***************************

int ftomil( float n )
{

	return ((int)(n*1000)) ;

}

int prepara_angulo( int n )
{
	n = n % 360000 ;
	if ( n < 0 ) n = n + 360000 ;
	return n ;
}

