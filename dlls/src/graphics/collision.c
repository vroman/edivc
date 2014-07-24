/*
** collision.c
** Incluye las rutinas de detección de colisiones entre sprites
*/

#include <SDL/SDL.h>

#include "export.h"
#include "graphics.h"

/*
** int IntersectionRR(int rc1left,int rc1top,int rc1right,int rc1bottom,int rc2left,int rc2top,int rc2right,int rc2bottom)
** Comprueba si hay colisión entre dos regiones rectangulares.
**
** Devuelve:
** 0 - No hay colisión
** 1 - Hay colisión
*/
int IntersectionRR(int rc1left,int rc1top,int rc1right,int rc1bottom,int rc2left,int rc2top,int rc2right,int rc2bottom)
{
	if(rc1left==rc2left || rc1top==rc2top || rc1right==rc2right || rc1bottom==rc2bottom)
		return 1;

	return ((Miedzy(rc1left,rc2left,rc2right) || Miedzy(rc1right,rc2left,rc2right) ||
		Miedzy(rc2left,rc1left,rc1right) || Miedzy(rc2right,rc1left,rc1right)) &&
		(Miedzy(rc1top,rc2top,rc2bottom) || Miedzy(rc1bottom,rc2top,rc2bottom) ||
		Miedzy(rc2top,rc1top,rc1bottom) || Miedzy(rc2bottom,rc1top,rc1bottom)));
}

/*
** eDIV_COLLISION (id)
** 
** Detecta si el proceso actual está colisionando con el del id indicado, o, si
** se le indica un type, si colisiona con algún proceso de ese tipo
**
** Devuelve: - el ID del proceso con el que se está colisionando (si se indica
**             un type y se está colisionando a la vez con varios procesos de
**             ese type, en sucesivas llamadas se irán devolviendo sus respec-
**             tivos IDs, hasta devolver 0 cuando ya no queden más).
**           - 0 si no se colisiona con ningún proceso del ID o type indicado.
*/
int eDIV_COLLISION(FUNCTION_PARAMS)
{
	int g1 , g2 ;
	int f1 , f2 ;
	int id1, id2 ;
	int a, i ;
	SDL_Rect r1 , r2 ;
	int _status=reservedptr("status");
	a = getparm() ;

	id1 = fp->procs_s[ fp->proc_orden[ *fp->proceso_actual ] ].id ;

	g1 = local("graph",id1)  ;
	f1 = local("file",id1) ;
	if ( files[f1].existe == 0 || files[f1].mapa[g1].existe == 0 )
		return 0;

	/* Si se le pasa un ID */
	if (a<fp->imem_max)
	{
		if(a==id1 || (fp->mem[id1+_status]!=2 && fp->mem[id1+_status]!=4))
			return 0;
		r1.x = local("x",id1) ;
		r1.y = local("y",id1) ;
		r1.w = files[f1].mapa[g1].Surface->w ;
		r1.h = files[f1].mapa[g1].Surface->h ;
		id2 = a ;
		g2 = local("graph",id2) ;
		f2 = local("file",id2);
		if ( files[f2].existe == 0 || files[f2].mapa[g2].existe == 0 )
			return 0;
		r2.x = local("x",id2) ;
		r2.y = local("y",id2) ;
		r2.w = files[f2].mapa[g2].Surface->w ;
		r2.h = files[f2].mapa[g1].Surface->h ;
		/* Colision barata :P */
		if(IntersectionRR(r1.x,r1.y,r1.x+r1.w-1,r1.x+r1.h-1,r2.x,r2.y,r2.x+r2.w-1,r2.y+r2.h-1))
			return id2;
	}
	else {
		int* type_scan=&reserved("type_scan",id1);
		int* id_scan=&reserved("id_scan",id1);
		/* Si se le pasa un type */

		if(*type_scan!=a) {
			*id_scan=0;
			*type_scan=a;
		}

		for ( i = *id_scan+1 ; i < *fp->num_procs ; i++ )
		{
			id2 = fp->procs_s[ fp->proc_orden[ i ] ].id;
			if(id2==id1 || (fp->mem[id1+_status]!=2 && fp->mem[id1+_status]!=4))
				continue;

			/* Si el proceso se corresponde con el type */
			if ( reserved("process_type",id2) == a )
			{

				r1.x = local("x",id1) ;
				r1.y = local("y",id1) ;
				r1.w = files[f1].mapa[g1].Surface->w ;
				r1.h = files[f1].mapa[g1].Surface->h ;
				g2 = local("graph",id2) ;
				f2 = local("file",id2) ;
				if ( files[f2].existe == 0 || files[f2].mapa[g2].existe == 0 )
					continue;
				r2.x = local("x",id2) ;
				r2.y = local("y",id2) ;
				r2.w = files[f2].mapa[g2].Surface->w ;
				r2.h = files[f2].mapa[g1].Surface->h ;
				/* Colision barata :P */
				if(IntersectionRR(r1.x,r1.y,r1.x+r1.w-1,r1.x+r1.h-1,r2.x,r2.y,r2.x+r2.w-1,r2.y+r2.h-1)) {
					*id_scan=i;

					return id2;
				}
			}
		}
		*type_scan=0;
	}

	return 0 ;
}
