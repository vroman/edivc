#!/bin/bash

CCOPTS="-I. -I/usr/local/include -I../include -fPIC"

FILES="
SDL_rotozoom
advance
carga
collision
draw
graphics
map_ops
misc
paleta
regiones
"

OBJECTS=""

for FILE in $FILES; do
	gcc -c $FILE.c $CCOPTS -o $FILE.o
	OBJECTS+="$FILE.o "
done

gcc $OBJECTS -shared -o ../../bin/graphics.so -L/usr/local/lib -lSDL

rm $OBJECTS
