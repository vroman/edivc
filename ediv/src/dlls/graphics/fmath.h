
/* Rutinas matemáticas de punto fijo, basadas en Allegro */

typedef long int fixed ;

extern fixed cos_table[90001] ;
extern int   cos_table_initialized ;
extern void  init_cos_tables() ;


#ifndef __GNUC__

#define inline __inline

#endif


#ifdef DEBUG
# define __INLINE static
#else
# define __INLINE static inline
#endif

#define FIXED_PREC 12

__INLINE fixed ftofix(double x)
{
	if (x > (1 << (31-FIXED_PREC))) {
		return 0x7FFFFFFF;
	}

	if (x < -(1 << (31-FIXED_PREC))) {
		return -(int)0x7FFFFFFF;
	}

	return (long)(x * (1 << FIXED_PREC) + (x < 0 ? -0.5 : 0.5));
}

__INLINE float fixtof (fixed x)
{
	return (float)x / (1 << FIXED_PREC) ;
}
  
__INLINE fixed itofix(int x)
{
	return x << FIXED_PREC ;
}

__INLINE int fixtoi(fixed x)
{
	return (x >> FIXED_PREC) ;
}

__INLINE fixed fcos(int x)
{
	if (x < 0) x = -x ;
	if (x > 360000) x %= 360000 ;

	if (x > 270000) return  cos_table[360000 - x] ;
	if (x > 180000) return -cos_table[x - 180000] ;
	if (x > 90000)  return -cos_table[180000 - x] ;
	return cos_table[x] ;
}

__INLINE fixed fsin(int x)
{
	if (x < 0) return -fsin(-x) ;
	if (x > 360000) x %= 360000 ;
	if (x > 270000) return -cos_table[x - 270000] ;
	if (x > 180000) return -cos_table[270000 - x] ;
	if (x > 90000)  return  cos_table[x - 90000 ] ;
	return cos_table[90000 - x] ;
}


__INLINE fixed fmul(int x, int y)
{
	return ftofix (fixtof(x) * fixtof(y)) ;
}

__INLINE fixed fdiv(int x, int y)
{
	return ftofix (fixtof(x) / fixtof(y)) ;
}