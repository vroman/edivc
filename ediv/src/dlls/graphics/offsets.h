
/* ------------------------------------------ */
/* Offsets de los datos globales predefinidos */
/* ------------------------------------------ */

#define MOUSEX          4*0
#define MOUSEY          4*1
#define MOUSEGRAPH      4*2
#define MOUSEFILE       4*3
#define MOUSEZ          4*4
#define MOUSEANGLE      4*5
#define MOUSESIZE       4*6
#define MOUSEFLAGS      4*7
#define MOUSEREGION     4*8
#define MOUSELEFT       4*9
#define MOUSEMIDDLE     4*10
#define MOUSERIGHT      4*11

#define SCROLLS		4*12

			/* Siguen 10 estructuras scroll de tamaño 20 */

#define JOYLEFT		4*212
#define JOYRIGHT	4*213
#define JOYUP		4*214
#define JOYDOWN		4*215
#define JOYBUTTON1	4*216
#define JOYBUTTON2	4*217
#define JOYBUTTON3	4*218
#define JOYBUTTON4	4*219
#define SETUPCARD	4*220
#define SETUPPORT	4*221
#define	SETUPIRQ	4*222
#define SETUPDMA	4*223
#define SETUPDMA2	4*224
#define SETUPMASTER	4*225
#define SETUPSOUNDFX	4*226
#define SETUPCDAUDIO	4*227
#define TIMER 		4*228

			/* Siguen 9 temporizadores adicionales */

#define TEXTZ		4*238
#define FADING		4*239
#define SHIFTSTATUS	4*240
#define ASCII		4*241
#define SCANCODE	4*242
#define JOYFILTER	4*243
#define JOYSTATUS	4*244
#define RESTORETYPE	4*245
#define DUMPTYPE	4*246
#define MAXPROCESSTIME	4*247

#define M7STRUCTS	4*248

			/* Siguen 10 estructuras de tamaño 10 */

/* Extras */

#define FPS             4*348
#define TEXT_FLAGS      4*349
#define PANSEP		4*350
#define REVERB		4*351
#define VOLUME		4*352
#define SOUND_FREQ	4*353
#define SOUND_MODE	4*354
#define GRAPH_MODE	4*355
#define ARGC            4*356
#define ARGV_TABLE	4*357	/* 32+1 cadenas */
#define SPEED_GAUGE	4*390

/* ----------------------------------------- */
/* Offsets de los datos locales predefinidos */
/* ----------------------------------------- */

#define PROCESS_ID	4*0
#define ID_SCAN		4*1
#define PROCESS_TYPE	4*2
#define TYPE_SCAN	4*3
#define STATUS		4*4
#define PARAM_OFFSET	4*5
#define PROGRAM_INDEX	4*6
#define IS_EXECUTED	4*7
#define IS_PAINTED	4*8
#define DISTANCE_1	4*9
#define DISTANCE_2	4*10
#define FRAME_PERCENT	4*11
#define BOX_X0		4*12
#define BOX_Y0		4*13
#define BOX_X1		4*14
#define BOX_Y1		4*15
#define FATHER		4*16
#define SON		4*17
#define SMALLBRO	4*18
#define BIGBRO		4*19
#define PRIORITY	4*20
#define CTYPE   	4*21
#define CFLAGS  	4*22
#define COORDX       	4*23
#define COORDY       	4*24
#define COORDZ       	4*25
#define GRAPHID 	4*26
#define FLAGS   	4*27
#define GRAPHSIZE    	4*28
#define ANGLE   	4*29
#define REGIONID    	4*30
#define FILEID  	4*31
#define XGRAPH  	4*32
#define HEIGHT  	4*33
#define CNUMBER 	4*34
#define RESOLUTION	4*35

/* Algunas definiciones comunes */

#define STATUS_DEAD	0
#define STATUS_KILLED	1
#define STATUS_RUNNING	2
#define STATUS_SLEEPING	3
#define STATUS_FROZEN 	4

