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
 *
 * $Id$
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "export.h"
#include "math.h"

int ExportaFuncs(EXPORTAFUNCS_PARAMS){
	CONST("pi",180000);

	FUNCTION("abs",1,eDIV_abs);
	FUNCTION("pow",2,eDIV_pow);
	FUNCTION("sqrt",1,eDIV_sqrt);
	FUNCTION("rand",2,eDIV_rand);
	FUNCTION("rand_seed",1,eDIV_rand_seed);
	FUNCTION("sin",1,eDIV_sin);
	FUNCTION("cos",1,eDIV_cos);
	FUNCTION("tan",1,eDIV_tan);
	FUNCTION("get_angle",1,eDIV_get_angle);
	FUNCTION("get_dist",1,eDIV_get_dist);
	FUNCTION("get_distx",2,eDIV_get_distx);
	FUNCTION("get_disty",2,eDIV_get_disty);
	FUNCTION("fget_angle",4,eDIV_fget_angle);
	FUNCTION("fget_dist",4,eDIV_fget_dist);
	FUNCTION("near_angle",3,eDIV_near_angle);
	FUNCTION("cpysign",2,eDIV_cpysign);
	/*
	FUNCTION("fcpysign",2,eDIV_fcpysign);
	FUNCTION("fixtof8",1,eDIV_fixtof8);
	FUNCTION("fixtof16",1,eDIV_fixtof16);
	*/
	FUNCTION("fixtoi8",1,eDIV_fixtoi8);
	FUNCTION("fixtoi16",1,eDIV_fixtoi16);
	/*
	FUNCTION("ftofix8",1,eDIV_ftofix8);
	FUNCTION("ftofix16",1,eDIV_ftofix16);
	*/
	FUNCTION("itofix8",1,eDIV_itofix8);
	FUNCTION("itofix16",1,eDIV_itofix16);
	/*
	FUNCTION("dtoradian",1,eDIV_dtoradian);
	FUNCTION("rtodegree",1,eDIV_rtodegree);
	FUNCTION("fmax",2,eDIV_fmax);
	FUNCTION("fmin",2,eDIV_fmin);
	FUNCTION("fxmax",3,eDIV_fxmax);	
	FUNCTION("fxmin",3,eDIV_fxmin);
	FUNCTION("fxmid",3,eDIV_fxmid);
	*/
	FUNCTION("max",2,eDIV_max);
	FUNCTION("min",2,eDIV_min);
	FUNCTION("xmax",3,eDIV_xmax);	
	FUNCTION("xmin",3,eDIV_xmin);
	FUNCTION("xmid",3,eDIV_xmid);
	
	ENTRYPOINT(first_load);

	return TRUE;
}

int eDIV_abs(FUNCTION_PARAMS){
	int n=getparm();
	return ((n<0)?(-n):(n));
}

int eDIV_pow(FUNCTION_PARAMS){
	int c,n,i;
	int b=getparm();
	int a=getparm();
	n=1;
	c=b;	
	if(c<0) c=-c;
	for(i=1; i<c; i++) n*=a;
	if(b<0) n=1/n;
	return n ;
}

int eDIV_sqrt(FUNCTION_PARAMS){
	int a=getparm();
	return (int)sqrt(a);   
}

int eDIV_rand(FUNCTION_PARAMS){
	int b=getparm();
	int a=getparm();
	return ((int)(rand()%(b-a+1))+a);
}

int eDIV_rand_seed(FUNCTION_PARAMS){
	int a=getparm();
	if(a==0) srand(time(NULL));
	else     srand(a);
	return 1;
}

int eDIV_sin(FUNCTION_PARAMS){
	int a=getparm();
	a=prepara_angulo(a);
	if(a<=90000)  return  seno[a];
	if(a<=180000) return  seno[180000-a];
	if(a<=270000) return -seno[a-180000];
	return -seno[360000-a];
}

int eDIV_cos(FUNCTION_PARAMS){
	int a=getparm();
	a=prepara_angulo(a);
	if(a<=90000)  return  seno[90000-a];
	if(a<=180000) return -seno[a-90000];
	if(a<=270000) return -seno[270000-a];
	return seno[a-270000];
}

int eDIV_tan(FUNCTION_PARAMS){
	int b,c;
	int a=getparm();
	a=prepara_angulo(a);
	if(a<=90000)      { b= seno[a];        c= seno[90000-a];  }
	else if(a<=180000){	b= seno[180000-a]; c=-seno[a-90000];  }
	else if(a<=270000){	b=-seno[a-180000]; c=-seno[270000-a]; }
	else              { b=-seno[360000-a]; c= seno[a-270000]; }	
	return ((c==0)?(0x7FFFFFFF):((int)(b/c)));
}

int eDIV_get_angle(FUNCTION_PARAMS){
	int id1,id2;
	int x1,x2,y1,y2;
	int a=getparm();
	id1=fp->procs_s[fp->proc_orden[*fp->proceso_actual]].id;
	id2=a;
	x1=local("x",id1);
	y1=local("y",id1);
	x2=local("x",id2);
	y2=local("y",id2);
	if((x2-x1)!=0){
		a=(int)(atan((double)((y2-y1)/(x2-x1)))/PIOVER180);
		if(x2<x1) a+=180;
	}
	else
		if(y2>y1)	a=90; else a=270;
	a*=1000;
	a=prepara_angulo(a);
  return a;
}

int eDIV_get_dist(FUNCTION_PARAMS){
	int id1,id2;
	int x1,x2,y1,y2;
	int a=getparm();
	id1=fp->procs_s[fp->proc_orden[*fp->proceso_actual]].id;
	id2=a;
	x1=local("x",id1);
	y1=local("y",id1);
	x2=local("x",id2);
	y2=local("y",id2);
	return ((int)sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
}

int eDIV_get_distx(FUNCTION_PARAMS){
	int d=getparm(); /* distancia */
	int a=getparm(); /* angulo */
	return ((int)(d*cos((a/1000)*PIOVER180)));
}

int eDIV_get_disty(FUNCTION_PARAMS){
	int d=getparm(); /* distancia */
	int a=getparm(); /* angulo */
	return ((int)(d*sin((a/1000)*PIOVER180)));
}

int eDIV_fget_angle(FUNCTION_PARAMS){
	int a;
	int y2=getparm();
	int x2=getparm();
	int y1=getparm();
	int x1=getparm();
	if((x2-x1)!=0){
		a=(int)(atan((double)((y2-y1)/(x2-x1)))/PIOVER180);
		if(x2<x1) a+=180;
	}
	else
		if(y2>y1)	a=90; else a=270;
	a*=1000;
	a=prepara_angulo(a);
  return a;
}	

int eDIV_fget_dist(FUNCTION_PARAMS){
	int y2=getparm();
	int x2=getparm();
	int y1=getparm();
	int x1=getparm();
	return ((int)sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
}

int eDIV_near_angle(FUNCTION_PARAMS){
	int c=getparm();
	int b=getparm();
	int a=getparm();
	a=prepara_angulo(a);
	b=prepara_angulo(b);
	c=prepara_angulo(c);
	if(abs(a-b)<=c)
		return b;
	if(a<b){
		if(abs(b-a)<180000)
			a=prepara_angulo(a+c);
		else
			a=prepara_angulo(a-c);
		return a ;
	}
	else{
		if(abs(b-a)<180000)
			a=prepara_angulo(a-c);
		else
			a=prepara_angulo(a+c);
		return a;
	}
	return 0;
}

int eDIV_cpysign(FUNCTION_PARAMS){
	int s=getparm();
	int n=getparm();
	return ((s<0)?((n<0)?(n):(-n)):((n>=0)?(n):(-n)));
}

/*
float eDIV_fcpysign(FUNCTION_PARAMS){
	float s=getparm();
	float n=getparm();
	return ((s<0)?((n<0)?(n):(-n)):((n>=0)?(n):(-n)));
}

float eDIV_fixtof8(FUNCTION_PARAMS){
	int n=getparm();
	return ((float)(n/255.0));
}

float eDIV_fixtof16(FUNCTION_PARAMS){
	int n=getparm();
	return ((float)(n/65535.0));
}
*/

int eDIV_fixtoi8(FUNCTION_PARAMS){
	int n=getparm();
	return (n>>8);
}

int eDIV_fixtoi16(FUNCTION_PARAMS){
	int n=getparm();
	return (n>>16);
}

/*
int eDIV_ftofix8(FUNCTION_PARAMS){
	float n=getparm();
	return ((int)(n*255.0));
}

int eDIV_ftofix16(FUNCTION_PARAMS){
	float n=getparm();
	return ((int)(n*65535.0));
}
*/

int eDIV_itofix8(FUNCTION_PARAMS){
	int n=getparm();
	return (n<<8);
}

int eDIV_itofix16(FUNCTION_PARAMS){
	int n=getparm();
	return (n<<16);
}

/*
float eDIV_dtoradian(FUNCTION_PARAMS){
	float d=getparm();
	return (d*(PI/180.0f));
}

float eDIV_rtodegree(FUNCTION_PARAMS){
	float r=getparm();
	return (r*(180.0f/PI));
}
*/

int eDIV_max(FUNCTION_PARAMS){
	int n2=getparm();
	int n1=getparm();
	return ((n1>n2)?(n1):(n2));
}

int eDIV_min(FUNCTION_PARAMS){
	int n2=getparm();
	int n1=getparm();
	return ((n1<n2)?(n1):(n2));
}

int eDIV_xmax(FUNCTION_PARAMS){
	int n3=getparm();
	int n2=getparm();
	int n1=getparm();
	int m=n1;
	if(n2>m) m=n2;
	if(n3>m) m=n3;	
	return m;
}

int eDIV_xmin(FUNCTION_PARAMS){
	int n3=getparm();
	int n2=getparm();
	int n1=getparm();
	int m=n1;
	if(n2<m) m=n2;
	if(n3<m) m=n3;	
	return m;
}

int eDIV_xmid(FUNCTION_PARAMS){
	int n3=getparm();
	int n2=getparm();
	int n1=getparm();
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

/*
float eDIV_fmax(FUNCTION_PARAMS){
	float n2=getparm();
	float n1=getparm();
	return ((n1>n2)?(n1):(n2));
}

float eDIV_fmin(FUNCTION_PARAMS){
	float n2=getparm();
	float n1=getparm();
	return ((n1<n2)?(n1):(n2));
}

float eDIV_fxmax(FUNCTION_PARAMS){
	float n3=getparm();
	float n2=getparm();
	float n1=getparm();
	float m=n1;
	if(n2>m) m=n2;
	if(n3>m) m=n3;	
	return m;
}

float eDIV_fxmin(FUNCTION_PARAMS){
	float n3=getparm();
	float n2=getparm();
	float n1=getparm();
	float m=n1;
	if(n2<m) m=n2;
	if(n3<m) m=n3;	
	return m;
}

float eDIV_fxmid(FUNCTION_PARAMS){
	float n3=getparm();
	float n2=getparm();
	float n1=getparm();
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
*/

/* Entrypoints */
void first_load(FUNCTION_PARAMS){
	int i;
	/* Calculamos los senos del 0 al 90000C */
	for(i=0; i<90000; i++)
		seno[i]=ftomil((float)sin((i/1000)*PIOVER180));
	seno[90000]=1000;
	srand(time(NULL));
}
	
/* Funciones internas de la DLL */
int ftomil(float n){
	return ((int)(n*1000));
}

int prepara_angulo(int n){
	n=n%360000;
	if(n<0) n=n+360000;
	return n;
}



