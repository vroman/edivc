/*
 * Este fichero contiene rutinas específicas para una plataforma.
 * En caso de hacer un port de la librería, deben implementarse las siguientes
 * funciones:
 *
 * gr_Init
 */

#include "gr_win32.h"

/*
 * Inicializa cosillas de la libreria
 */
int gr_Init( void *hInstance, void *wndProc )
{
	sww_state.hInstance = ( HINSTANCE ) hInstance;
	sww_state.wndproc = wndProc;

	return TRUE;
}

/*
** SWimp_InitGraphics
**
** This initializes the software refresh's implementation specific
** graphics subsystem.  In the case of Windows it creates DIB or
** DDRAW surfaces.
**
** The necessary width and height parameters are grabbed from
** vid.width and vid.height.
*/
static bool SWimp_InitGraphics( int vid_width, int vid_height, bool fullscreen )
{
	// free resources in use
	gr_Shutdown ();

	// create a new window
	VID_CreateWindow (vid_width, vid_height, WINDOW_STYLE);

	// initialize the appropriate subsystem
	if ( !fullscreen )
	{
		if ( !DIB_Init( &vid.buffer, &vid.rowbytes ) )
		{
			vid.buffer = 0;
			vid.rowbytes = 0;

			return false;
		}
	}
	else
	{
		if ( !DDRAW_Init( &vid.buffer, &vid.rowbytes ) )
		{
			vid.buffer = 0;
			vid.rowbytes = 0;

			return false;
		}
	}

	return true;
}
