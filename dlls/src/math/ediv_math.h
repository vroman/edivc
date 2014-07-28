#ifndef __EDIV_MATH_H
#define __EDIV_MATH_H


#define PIOVER180	0.017453292519943295769236907684886
#define PI        3.141592654f

int seno[90001];

int   eDIV_abs(FUNCTION_PARAMS);
int   eDIV_pow(FUNCTION_PARAMS);
int   eDIV_sqrt(FUNCTION_PARAMS);
int   eDIV_rand(FUNCTION_PARAMS);
int   eDIV_rand_seed(FUNCTION_PARAMS);
int   eDIV_sin(FUNCTION_PARAMS);
int   eDIV_cos(FUNCTION_PARAMS);
int   eDIV_tan(FUNCTION_PARAMS);
int   eDIV_get_angle(FUNCTION_PARAMS);
int   eDIV_get_dist(FUNCTION_PARAMS);
int   eDIV_get_distx(FUNCTION_PARAMS);
int   eDIV_get_disty(FUNCTION_PARAMS);
int   eDIV_fget_angle(FUNCTION_PARAMS);
int   eDIV_fget_dist(FUNCTION_PARAMS);
int   eDIV_near_angle(FUNCTION_PARAMS);
int   eDIV_cpysign(FUNCTION_PARAMS);
/*
int   eDIV_fcpysign(FUNCTION_PARAMS);
float eDIV_fixtof8(FUNCTION_PARAMS);
float eDIV_fixtof16(FUNCTION_PARAMS);
*/
int   eDIV_fixtoi8(FUNCTION_PARAMS);
int   eDIV_fixtoi16(FUNCTION_PARAMS);
/*
int   eDIV_ftofix8(FUNCTION_PARAMS);
int   eDIV_ftofix16(FUNCTION_PARAMS);
*/
int   eDIV_itofix8(FUNCTION_PARAMS);
int   eDIV_itofix16(FUNCTION_PARAMS);
/*
float eDIV_dtoradian(FUNCTION_PARAMS);
float eDIV_rtodegree(FUNCTION_PARAMS);
int   eDIV_fmax(FUNCTION_PARAMS);
int   eDIV_fmin(FUNCTION_PARAMS);
int   eDIV_fxmax(FUNCTION_PARAMS);
int   eDIV_fxmin(FUNCTION_PARAMS);
int   eDIV_fxmid(FUNCTION_PARAMS);
*/
int   eDIV_max(FUNCTION_PARAMS);
int   eDIV_min(FUNCTION_PARAMS);
int   eDIV_xmax(FUNCTION_PARAMS);
int   eDIV_xmin(FUNCTION_PARAMS);
int   eDIV_xmid(FUNCTION_PARAMS);


int ftomil(float);
int prepara_angulo(int);

#endif