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
#include <SDL/SDL.h>


#include "main.h"

//#define _KEY2(a) CONST("a",a)
#define _KEY(a) CONST(#a,a)


#define _esc 1
#define _f1 59
#define _f2 60
#define _f3 61
#define _f4 62
#define _f5 63
#define _f6 64
#define _f7 65
#define _f8 66
#define _f9 67
#define _f10 68
#define _f11 87
#define _f12 88
#define _prn_scr 55 //Impr Pant
#define _scroll_lock 70 //Bloq Despl

#define _wave 41 // "º"
#define _1 2
#define _2 3
#define _3 4
#define _4 5
#define _5 6
#define _6 7
#define _7 8
#define _8 9
#define _9 10
#define _0 11
#define _minus 12 //?
#define _plus 13 //¿

#define _backspace 14
#define _tab 15
#define _q 16
#define _w 17
#define _e 18
#define _r 19
#define _t 20
#define _y 21
#define _u 22
#define _i 23
#define _o 24
#define _p 25
#define _l_brachet 26 //"`" "^"
#define _r_brachet 27 //"+" "*"
#define _enter 28

#define _caps_lock 58
#define _a 30
#define _s 31
#define _d 32
#define _f 33
#define _g 34
#define _h 35
#define _j 36
#define _k 37
#define _l 38
#define _semicolon 39 //Ñ
#define _apostrophe 40 //{
#define _backslash 43 //}

#define _l_shift 42
#define _z 44
#define _x 45
#define _c 46
#define _v 47
#define _b 48
#define _n 49
#define _m 50
#define _comma 51
#define _point 52
#define _slash 53 //"-" "_"
#define _r_shift 54

#define _control 29
#define _alt 56
#define _space 57

#define _ins 82
#define _home 71
#define _pgup 73
#define _del 83
#define _end 79
#define _pgdn 81

#define _up 72
#define _down 80
#define _left 75
#define _right 77

#define _num_lock 69
#define _c_backslash 90
#define _c_asterisk 91
#define _c_minus 92
#define _c_home 93
#define _c_up 94
#define _c_pgup 95
#define _c_left 96
#define _c_center 97
#define _c_right 98
#define _c_end 99
#define _c_down 100
#define _c_pgdn 101
#define _c_ins 102
#define _c_del 103
#define _c_plus 104
#define _c_enter 105



Uint8 *keys ;

int SDLtoDIV[1024] ;


int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{
/*
	CONST("_esc",_esc) ;
	CONST("_f1",_f1) ;
	CONST("_f2",_f1) ;
	CONST("_f3",_f1) ;
	CONST("_f4",_f1) ;
	CONST("_f5",_f1) ;
	CONST("_f6",_f1) ;
	CONST("_f7",_f1) ;
	CONST("_f8",_f1) ;
	CONST("_f9",_f1) ;
	CONST("_f10",_f1) ;
	CONST("_f11"_f1) ;
	CONST("_f12",_f12) ;
*/
	_KEY(_esc) ;
	_KEY(_f1) ;
	_KEY(_f2) ;
	_KEY(_f3) ;
	_KEY(_f4) ;
	_KEY(_f5) ;
	_KEY(_f6) ;
	_KEY(_f7) ;
	_KEY(_f8) ;
	_KEY(_f9) ;
	_KEY(_f10) ;
	_KEY(_f11) ;
	_KEY(_f12) ;
	_KEY(_prn_scr ) ;
	_KEY(_scroll_lock ) ;
	_KEY(_wave) ;
	_KEY(_1) ;
	_KEY(_2) ;
	_KEY(_3) ;
	_KEY(_4) ;
	_KEY(_5) ;
	_KEY(_6) ;
	_KEY(_7) ;
	_KEY(_8) ;
	_KEY(_9) ;
	_KEY(_0) ;
	_KEY(_minus) ;
	_KEY(_plus) ;
	_KEY(_backspace) ;
	_KEY(_tab) ;
	_KEY(_q) ;
	_KEY(_w) ;
	_KEY(_e) ;
	_KEY(_r) ;
	_KEY(_t) ;
	_KEY(_y) ;
	_KEY(_u) ;
	_KEY(_i) ;
	_KEY(_o) ;
	_KEY(_p) ;
	_KEY(_l_brachet) ;
	_KEY(_r_brachet) ;
	_KEY(_enter) ;
	_KEY(_caps_lock ) ;
	_KEY(_a) ;
	_KEY(_s) ;
	_KEY(_d) ;
	_KEY(_f) ;
	_KEY(_g) ;
	_KEY(_h) ;
	_KEY(_j) ;
	_KEY(_k) ;
	_KEY(_l) ;
	_KEY(_semicolon) ;
	_KEY(_apostrophe) ;
	_KEY(_backslash) ;
	_KEY(_l_shift) ;
	_KEY(_z) ;
	_KEY(_x) ;
	_KEY(_c) ;
	_KEY(_v) ;
	_KEY(_b) ;
	_KEY(_n) ;
	_KEY(_m) ;
	_KEY(_comma) ;
	_KEY(_point) ;
	_KEY(_slash) ;
	_KEY(_r_shift) ;
	_KEY(_control) ;
	_KEY(_alt) ;
	_KEY(_space) ;
	_KEY(_ins) ;
	_KEY(_home) ;
	_KEY(_pgup) ;
	_KEY(_del) ;
	_KEY(_end) ;
	_KEY(_pgdn) ;
	_KEY(_up) ;
	_KEY(_down) ;
	_KEY(_left) ;
	_KEY(_right) ;
	_KEY(_num_lock) ;
	_KEY(_c_backslash) ;
	_KEY(_c_asterisk) ;
	_KEY(_c_minus) ;
	_KEY(_c_plus) ;
	_KEY(_c_enter) ;
	_KEY(_c_home) ;
	_KEY(_c_pgup) ;
	_KEY(_c_end) ;
	_KEY(_c_pgdn) ;
	_KEY(_c_up) ;
	_KEY(_c_down) ;
	_KEY(_c_left) ;
	_KEY(_c_right) ;
	_KEY(_c_center) ;
	_KEY(_c_ins) ;
	_KEY(_c_del) ;
	
	FUNCTION("key",1,eDiv_Key) ;
	ENTRYPOINT(first_load) ;
	ENTRYPOINT(frame) ;

	return TRUE;
}

int eDiv_Key(FUNCTION_PARAMS)
{
	//FILE *fichero ;
	int a ;
	
	//fichero = fopen("input.txt" , "a+" ) ;
	a = getparm() ;
	//fprintf( fichero , "key: %i , keys[30]: %i \n" , a , keys[30] ) ;
	//fclose( fichero ) ;
	
	if ( a == _control )
		if ( keys[ SDLK_RCTRL ] )
			return 1 ;
	if ( a == _alt )
		if ( keys[ SDLK_RALT ] )
			return 1 ;

	return keys[ SDLtoDIV[a] ] ;
}
//*************************** ENTRYPOINTS **********************************************

void first_load()
{
	int i ;
	SDL_Init( SDL_INIT_EVENTTHREAD ) ;

	for ( i = 0 ; i < 1024 ; i++ )
	{
		SDLtoDIV[ i ] = 0 ;
	}
/*
	SDLtoDIV[ SDLK_ESCAPE ] = _esc ;
	SDLtoDIV[ SDLK_1 ] = _1 ;
	SDLtoDIV[ SDLK_2 ] = _2 ;
	SDLtoDIV[ SDLK_3 ] = _3 ;
	SDLtoDIV[ SDLK_4 ] = _4 ;
	SDLtoDIV[ SDLK_5 ] = _5 ;
	SDLtoDIV[ SDLK_6 ] = _6 ;
	SDLtoDIV[ SDLK_7 ] = _7 ;
	SDLtoDIV[ SDLK_8 ] = _8 ;
	SDLtoDIV[ SDLK_9 ] = _9 ;
	SDLtoDIV[ SDLK_0 ] = _0 ;
	SDLtoDIV[ SDLK_MINUS ] = _minus ;
	SDLtoDIV[ SDLK_EQUALS ] = _plus ;
	SDLtoDIV[ SDLK_BACKSPACE ] = _backspace ;
	SDLtoDIV[ SDLK_TAB ] = _tab ;
	SDLtoDIV[ SDLK_p ] = _p ;
	SDLtoDIV[ SDLK_w ] = _w ;
	SDLtoDIV[ SDLK_e ] = _e ;
	SDLtoDIV[ SDLK_r ] = _r ;
	SDLtoDIV[ SDLK_t ] = _t ;
	SDLtoDIV[ SDLK_y ] = _y ;
	SDLtoDIV[ SDLK_u ] = _u ;
	SDLtoDIV[ SDLK_i ] = _i ;
	SDLtoDIV[ SDLK_o ] = _o ;
	SDLtoDIV[ SDLK_p ] = _p ;
	SDLtoDIV[ SDLK_LEFTBRACKET ] = _l_brachet ;
	SDLtoDIV[ SDLK_RIGHTBRACKET ] = _r_brachet ;
	SDLtoDIV[ SDLK_RETURN ] = _enter ;
	SDLtoDIV[ SDLK_LCTRL ] = _control ;
	SDLtoDIV[ SDLK_RCTRL ] = _control ;
	SDLtoDIV[ SDLK_a ] = _a ;
	SDLtoDIV[ SDLK_s ] = _s ;
	SDLtoDIV[ SDLK_d ] = _d ;
	SDLtoDIV[ SDLK_f ] = _f ;
	SDLtoDIV[ SDLK_g ] = _g ;
	SDLtoDIV[ SDLK_h ] = _h ;
	SDLtoDIV[ SDLK_j ] = _j ;
	SDLtoDIV[ SDLK_k ] = _k ;
	SDLtoDIV[ SDLK_l ] = _l ;
	SDLtoDIV[ SDLK_SEMICOLON ] = _semicolon ;
	SDLtoDIV[ SDLK_QUOTE ] = _apostrophe ;
	SDLtoDIV[ SDLK_BACKQUOTE ] = _wave ;
	SDLtoDIV[ SDLK_LSHIFT ] = _l_shift ;
	SDLtoDIV[ SDLK_BACKSLASH ] = _backslash ;
	SDLtoDIV[ SDLK_z ] = _z ;
	SDLtoDIV[ SDLK_x ] = _x ;
	SDLtoDIV[ SDLK_c ] = _c ;
	SDLtoDIV[ SDLK_v ] = _v ;
	SDLtoDIV[ SDLK_b ] = _b ;
	SDLtoDIV[ SDLK_n ] = _n ;
	SDLtoDIV[ SDLK_m ] = _m ;
	SDLtoDIV[ SDLK_COMMA ] = _comma ;
	SDLtoDIV[ SDLK_PERIOD ] = _point ;
	SDLtoDIV[ SDLK_SLASH ] = _slash ;
	SDLtoDIV[ SDLK_RSHIFT ] = _r_shift ;
	SDLtoDIV[ SDLK_SYSREQ ] = _prn_scr ;
	SDLtoDIV[ SDLK_LALT ] = _alt ;
	SDLtoDIV[ SDLK_RALT ] = _alt ;
	SDLtoDIV[ SDLK_SPACE ] = _space ;
	SDLtoDIV[ SDLK_CAPSLOCK ] = _caps_lock ;
	SDLtoDIV[ SDLK_F1 ] = _f1 ;
	SDLtoDIV[ SDLK_F2 ] = _f2 ;
	SDLtoDIV[ SDLK_F3 ] = _f3 ;
	SDLtoDIV[ SDLK_F4 ] = _f4 ;
	SDLtoDIV[ SDLK_F5 ] = _f5 ;
	SDLtoDIV[ SDLK_F6 ] = _f6 ;
	SDLtoDIV[ SDLK_F7 ] = _f7 ;
	SDLtoDIV[ SDLK_F8 ] = _f8 ;
	SDLtoDIV[ SDLK_F9 ] = _f9 ;
	SDLtoDIV[ SDLK_F10 ] = _f10 ;
	SDLtoDIV[ SDLK_F11 ] = _f11 ;
	SDLtoDIV[ SDLK_F12 ] = _f12 ;
	SDLtoDIV[ SDLK_NUMLOCK ] = _num_lock ;
	SDLtoDIV[ SDLK_SCROLLOCK ] = _scroll_lock ;
	SDLtoDIV[ SDLK_HOME ] = _home ;
	SDLtoDIV[ SDLK_UP ] = _up ;
	SDLtoDIV[ SDLK_PAGEUP ] = _pgup ;
	SDLtoDIV[ SDLK_END ] = _end ;
	SDLtoDIV[ SDLK_DOWN ] = _down ;
	SDLtoDIV[ SDLK_PAGEDOWN ] = _pgdn ;
	SDLtoDIV[ SDLK_INSERT ] = _ins ;
	SDLtoDIV[ SDLK_KP_PERIOD ] = _c_del ;
	SDLtoDIV[ SDLK_KP_DIVIDE ] = _c_backslash ;
	SDLtoDIV[ SDLK_KP_MULTIPLY ] = _c_asterisk ;
	SDLtoDIV[ SDLK_KP_MINUS ] = _c_minus ;
	SDLtoDIV[ SDLK_KP_PLUS ] = _c_plus ;
	SDLtoDIV[ SDLK_KP_ENTER ] = _c_enter ;
	SDLtoDIV[ SDLK_KP0 ] = _c_ins ;
	SDLtoDIV[ SDLK_KP1 ] = _c_end ;
	SDLtoDIV[ SDLK_KP2 ] = _c_down ;
	SDLtoDIV[ SDLK_KP3 ] = _c_pgdn ;
	SDLtoDIV[ SDLK_KP4 ] = _c_left ;
	SDLtoDIV[ SDLK_KP5 ] = _c_center ;
	SDLtoDIV[ SDLK_KP6 ] = _c_right ;
	SDLtoDIV[ SDLK_KP7 ] = _c_home ;
	SDLtoDIV[ SDLK_KP8 ] = _c_up ;
	SDLtoDIV[ SDLK_KP9 ] = _c_pgup ;
*/
	SDLtoDIV[ _esc ] = SDLK_ESCAPE ;
	SDLtoDIV[ _1 ] = SDLK_1 ;
	SDLtoDIV[ _2 ] = SDLK_2 ;
	SDLtoDIV[ _3 ] = SDLK_3 ;
	SDLtoDIV[ _4 ] = SDLK_4 ;
	SDLtoDIV[ _5 ] = SDLK_5 ;
	SDLtoDIV[ _6 ] = SDLK_6 ;
	SDLtoDIV[ _7 ] = SDLK_7 ;
	SDLtoDIV[ _8 ] = SDLK_8 ;
	SDLtoDIV[ _9 ] = SDLK_9 ;
	SDLtoDIV[ _0 ] = SDLK_0 ;
	SDLtoDIV[ _minus ] = SDLK_MINUS ;
	SDLtoDIV[ _plus ] =SDLK_EQUALS  ;
	SDLtoDIV[ _backspace ] =SDLK_BACKSPACE  ;
	SDLtoDIV[ _tab ] =SDLK_TAB  ;
	SDLtoDIV[ _p ] =SDLK_p  ;
	SDLtoDIV[ _w ] =SDLK_w  ;
	SDLtoDIV[ _e ] =SDLK_e  ;
	SDLtoDIV[ _r ] =SDLK_r  ;
	SDLtoDIV[ _t ] =SDLK_t  ;
	SDLtoDIV[ _y ] =SDLK_y  ;
	SDLtoDIV[ _u ] =SDLK_u  ;
	SDLtoDIV[ _i ] =SDLK_i  ;
	SDLtoDIV[ _o ] =SDLK_o  ;
	SDLtoDIV[ _p ] =SDLK_p  ;
	SDLtoDIV[ _l_brachet ] =SDLK_LEFTBRACKET  ;
	SDLtoDIV[ _r_brachet ] =SDLK_RIGHTBRACKET  ;
	SDLtoDIV[ _enter ] =SDLK_RETURN  ;
	SDLtoDIV[ _control ] =SDLK_LCTRL  ;
	//SDLtoDIV[ _control ] =SDLK_RCTRL  ;
	SDLtoDIV[ _a ] =SDLK_a  ;
	SDLtoDIV[ _s ] =SDLK_s  ;
	SDLtoDIV[ _d ] =SDLK_d  ;
	SDLtoDIV[ _f ] =SDLK_f  ;
	SDLtoDIV[ _g ] =SDLK_g  ;
	SDLtoDIV[ _h ] =SDLK_h  ;
	SDLtoDIV[ _j ] =SDLK_j  ;
	SDLtoDIV[ _k ] =SDLK_k  ;
	SDLtoDIV[ _l ] =SDLK_l  ;
	SDLtoDIV[ _semicolon ] =SDLK_SEMICOLON  ;
	SDLtoDIV[ _apostrophe ] =SDLK_QUOTE  ;
	SDLtoDIV[ _wave ] =SDLK_BACKQUOTE  ;
	SDLtoDIV[ _l_shift ] =SDLK_LSHIFT  ;
	SDLtoDIV[ _backslash ] =SDLK_BACKSLASH  ;
	SDLtoDIV[ _z ] =SDLK_z  ;
	SDLtoDIV[ _x ] =SDLK_x  ;
	SDLtoDIV[ _c ] =SDLK_c  ;
	SDLtoDIV[ _v ] =SDLK_v  ;
	SDLtoDIV[ _b ] =SDLK_b  ;
	SDLtoDIV[ _n ] =SDLK_n  ;
	SDLtoDIV[ _m ] =SDLK_m  ;
	SDLtoDIV[ _comma ] =SDLK_COMMA  ;
	SDLtoDIV[ _point ] =SDLK_PERIOD  ;
	SDLtoDIV[ _slash ] =SDLK_SLASH  ;
	SDLtoDIV[ _r_shift ] =SDLK_RSHIFT  ;
	SDLtoDIV[ _prn_scr ] =SDLK_SYSREQ  ;
	SDLtoDIV[ _alt ] =SDLK_LALT  ;
	//SDLtoDIV[ _alt ] =SDLK_RALT  ;
	SDLtoDIV[ _space ] =SDLK_SPACE  ;
	SDLtoDIV[ _caps_lock ] =SDLK_CAPSLOCK  ;
	SDLtoDIV[ _f1 ] =SDLK_F1  ;
	SDLtoDIV[ _f2 ] =SDLK_F2  ;
	SDLtoDIV[ _f3 ] =SDLK_F3  ;
	SDLtoDIV[ _f4 ] =SDLK_F4  ;
	SDLtoDIV[ _f5 ] =SDLK_F5  ;
	SDLtoDIV[ _f6 ] =SDLK_F6  ;
	SDLtoDIV[ _f7 ] =SDLK_F7  ;
	SDLtoDIV[ _f8 ] =SDLK_F8  ;
	SDLtoDIV[ _f9 ] =SDLK_F9  ;
	SDLtoDIV[ _f10 ] =SDLK_F10  ;
	SDLtoDIV[ _f11 ] =SDLK_F11  ;
	SDLtoDIV[ _f12 ] =SDLK_F12  ;
	SDLtoDIV[ _num_lock ] =SDLK_NUMLOCK  ;
	SDLtoDIV[ _scroll_lock ] =SDLK_SCROLLOCK  ;
	SDLtoDIV[ _home ] =SDLK_HOME  ;
	SDLtoDIV[ _up ] =SDLK_UP  ;
	SDLtoDIV[ _pgup ] =SDLK_PAGEUP  ;
	SDLtoDIV[ _end ] =SDLK_END  ;
	SDLtoDIV[ _down ] =SDLK_DOWN  ;
	SDLtoDIV[ _left ] =SDLK_LEFT ;
	SDLtoDIV[ _right ] =SDLK_RIGHT ;
	SDLtoDIV[ _pgdn ] =SDLK_PAGEDOWN  ;
	SDLtoDIV[ _ins ] =SDLK_INSERT  ;
	SDLtoDIV[ _c_del ] =SDLK_KP_PERIOD  ;
	SDLtoDIV[ _c_backslash ] =SDLK_KP_DIVIDE  ;
	SDLtoDIV[ _c_asterisk ] =SDLK_KP_MULTIPLY  ;
	SDLtoDIV[ _c_minus ] =SDLK_KP_MINUS  ;
	SDLtoDIV[ _c_plus ] =SDLK_KP_PLUS  ;
	SDLtoDIV[ _c_enter ] =SDLK_KP_ENTER  ;
	SDLtoDIV[ _c_ins ] =SDLK_KP0  ;
	SDLtoDIV[ _c_end ] =SDLK_KP1  ;
	SDLtoDIV[ _c_down ] =SDLK_KP2  ;
	SDLtoDIV[ _c_pgdn ] =SDLK_KP3  ;
	SDLtoDIV[ _c_left ] =SDLK_KP4  ;
	SDLtoDIV[ _c_center ] =SDLK_KP5  ;
	SDLtoDIV[ _c_right ] =SDLK_KP6  ;
	SDLtoDIV[ _c_home ] =SDLK_KP7  ;
	SDLtoDIV[ _c_up ] =SDLK_KP8  ;
	SDLtoDIV[ _c_pgup ] =SDLK_KP9  ;

}

void frame(int mem[], int * varindex, struct _procs_s *procs_s)
{
	int numkeys ;
	
	keys = SDL_GetKeyState(&numkeys ) ;
	
	//FILE* fichero ;
	//fichero = fopen( "input.txt" , "w+" ) ;
	//for ( i = 0 ; i < 256 ; i++ )
	//	keys[i] = 0 ;
/*
	for ( i = 0 ; i < 256 ; i++ )
	{
		if ( event[i].type == SDL_KEYDOWN )
		{
			keys[ SDLtoDIV[ event[i].key.keysym.sym ] ] = 1 ;
		}
		if ( event[i].type == SDL_KEYUP )
		{
			keys[ SDLtoDIV[ event[i].key.keysym.sym ] ] = 0 ;
		}
	}*/
	/*
	while(SDL_PollEvent(&tecla))
	{
		if(tecla.type == SDL_KEYDOWN)
		{
			if ( 
			keys[tecla.key.keysym.sym] = 1 ;
			//i=(int)tecla.key.keysym.sym;
		}
	}
	*/
/*	for ( i = 0 ; i < 256 ; i++ )
	{
		fprintf( fichero , "%i " , keys[i] ) ;
	}
	fprintf( fichero , "\n" ) ;
*/	
	//fclose( fichero ) ;

}
    


//**************************** FUNCIONES INTERNAS DE LA DLL ***************************


