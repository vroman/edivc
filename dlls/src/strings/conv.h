#ifndef __EDIV_CONV_H_
#define __EDIV_CONV_H_

#ifndef byte
 #define byte unsigned char
#endif

byte dos_to_win[256];
byte win_to_dos[256];

byte lower[256];
byte upper[256];

byte c_convert(byte c);

void set_c_lower (const byte *from, const byte *to);
void inicializa_lower();

void set_c_upper (const byte *from, const byte *to);
void inicializa_upper();

#endif __CONV_H_