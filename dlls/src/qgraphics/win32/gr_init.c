/*
 * Este fichero contiene rutinas específicas para una plataforma.
 * En caso de hacer un port de la librería, deben implementarse las siguientes
 * funciones:
 *
 * gr_Init
 * gr_InitVideoMode
 * gr_Shutdown
 */

#include "qgraphics.h"
#include "gr_win32.h"
#include "dib.h"

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
** gr_InitVideoMode
**
** This initializes the software refresh's implementation specific
** graphics subsystem.  In the case of Windows it creates DIB or
** DDRAW surfaces.
**
** The necessary width and height parameters are grabbed from
** vid.width and vid.height.
*/
static bool gr_InitVideoMode( int vid_width, int vid_height, int bpp, int flags )
{
	// free resources in use
	gr_Shutdown ();

	vid.width=vid_width;
	vid.height=vid_height;
	vid.bpp=bpp;
	vid.flags=flags;

	// create a new window
	Win32_CreateWindow (vid_width, vid_height, WINDOW_STYLE);

	// initialize the appropriate subsystem
	if ( !(flags & _FULLSCREEN) )
	{
		if ( !DIB_Init( &vid.buffer ) )
		{
			vid.buffer = 0;

			return FALSE;
		}
	}
	else 
	{
		/* de momento, solo DIB (sin fullscreen) */
		return FALSE;

		/*if ( !DDRAW_Init( &vid.buffer, &vid.rowbytes ) )
		{
			vid.buffer = 0;
			vid.rowbytes = 0;

			return false;
		}*/
	}

	return FALSE;
}

/*
** Win32_CreateWindow
*/
void Win32_CreateWindow( int width, int height, int stylebits )
{
	WNDCLASS		wc;
	RECT			r;
	int				x, y, w, h;
	int				exstyle;

	if ( vid.flags & _FULLSCREEN )
		exstyle = WS_EX_TOPMOST;
	else
		exstyle = 0;

	/* Register the frame class */
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)sww_state.wndproc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = sww_state.hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
	wc.hbrBackground = (void *)COLOR_GRAYTEXT;
    wc.lpszMenuName  = 0;
    wc.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClass (&wc) )
		fp->CustomError(_critical_error,"qgraphics: No puedo registrar clase de ventana");

	r.left = 0;
	r.top = 0;
	r.right  = width;
	r.bottom = height;

	AdjustWindowRect (&r, stylebits, FALSE);

	w = r.right - r.left;
	h = r.bottom - r.top;
	x = 0;
	y = 0;

	sww_state.hWnd = CreateWindowEx (
		exstyle,
		 WINDOW_CLASS_NAME,
		 "qgraphics",
		 stylebits,
		 x, y, w, h,
		 NULL,
		 NULL,
		 sww_state.hInstance,
		 NULL);

	if (!sww_state.hWnd)
		fp->CustomError(_critical_error."qgraphics: No puedo crear ventana");
	
	ShowWindow( sww_state.hWnd, SW_SHOWNORMAL );
	UpdateWindow( sww_state.hWnd );
	SetForegroundWindow( sww_state.hWnd );
	SetFocus( sww_state.hWnd );

	// let the sound and input subsystems know about the new window
	//ri.Vid_NewWindow (width, height);
}
