/*! \file advance.c
 * \brief Contiene las funciones advance() y xadvance() para el movimiento en 2D.
 * \todo Hacer que les afecte la local resolution
 */

#include <math.h>

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"

/*! \brief Avanza 'a' unidades según el ángulo del proceso.
 * @param a Distancia que se quiere avanzar
 * @return 1
 * @see eDIV_XADVANCE()
 */
int eDIV_ADVANCE(FUNCTION_PARAMS)
{
	int a , id1 , x , y , angulo ;
	a = getparm() ;
	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;
	angulo = local("angle",id1) ;
	x = (int) ((double)a * cos( (angulo/1000) * PIOVER180 )) ;
	y = (int) ((double)a * sin( (angulo/1000) * PIOVER180 )) ;
	local("x",id1) += x;
	local("y",id1) += y ;
	return 1 ;
}

/*! \brief Avanza 'b' unidades según el ángulo 'a'
 * @param a Ángulo
 * @param b Distancia que se quiere avanzar
 * @return 1
 * @see eDIV_ADVANCE()
 */
int eDIV_XADVANCE(FUNCTION_PARAMS)
{
	int a , b , id1 , x , y , angulo ;
	b = getparm() ;
	a = getparm() ;
	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;
	angulo = a ;
	x = (int) ((double)b * cos( (angulo/1000) * PIOVER180 )) ;
	y = (int) ((double)b * sin( (angulo/1000) * PIOVER180 )) ;
	local("x",id1) += x ;
	local("y",id1) += y ;
	return 1 ;
}
