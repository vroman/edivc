#ifndef __EDIV_MATH_H
#define __EDIV_MATH_H

#define PIOVER180	0.017453292519943295769236907684886


int eDiv_Abs(FUNCTION_PARAMS) ;
int eDiv_Pow(FUNCTION_PARAMS) ;
int eDiv_Sqrt(FUNCTION_PARAMS) ;
int eDiv_Rand(FUNCTION_PARAMS) ;
int eDiv_RandSeed(FUNCTION_PARAMS) ;
int eDiv_Sin(FUNCTION_PARAMS) ;
int eDiv_Cos(FUNCTION_PARAMS) ;
int eDiv_Tan(FUNCTION_PARAMS) ;
int eDiv_GetAngle(FUNCTION_PARAMS) ;
int eDiv_GetDist(FUNCTION_PARAMS) ;
int eDiv_GetDistX(FUNCTION_PARAMS) ;
int eDiv_GetDistY(FUNCTION_PARAMS) ;
int eDiv_FgetAngle(FUNCTION_PARAMS) ;
int eDiv_FgetDist(FUNCTION_PARAMS) ;
int eDiv_NearAngle(FUNCTION_PARAMS) ;

float eDiv_cpysign(float n,float s);
float eDiv_max(float n1,float n2);
float eDiv_min(float n1,float n2);
float eDiv_xmax(float n1,float n2,float n3);
float eDiv_xmin(float n1,float n2,float n3);
float eDiv_xmid(float n1,float n2,float n3);

int ftomil(float) ;
int prepara_angulo( int ) ;

#endif