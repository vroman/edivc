/*
KPLIB.H: Ken's Picture LIBrary written by Ken Silverman
Copyright (c) 1998-2004 Ken Silverman
Ken Silverman's official web site: http://www.advsys.net/ken
*/

#ifndef __KPLIB_H__
#define __KPLIB_H__

	//High-level (easy) picture loading function:
extern void kpzload (const char *, long *, long *, long *, long *);
	//Low-level PNG/JPG functions:
extern void kpgetdim (const char *, long, long *, long *);
extern long kprender (const char *, long, long, long, long, long, long, long);

	//ZIP functions:
extern void kzaddstack (const char *);
extern void kzuninit ();
extern long kzopen (const char *);
extern long kzread (void *, long);
extern long kzfilelength ();
extern void kzseek (long, long);
extern long kztell ();
extern long kzgetc ();
extern long kzeof ();
extern void kzclose ();

extern void kzfindfilestart (const char *); //pass wildcard string
extern long kzfindfile (char *); //you alloc buf, returns 1:found,0:~found

#endif //__KPLIB_H__
