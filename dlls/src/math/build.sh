#!/bin/bash
NAME="math"
CCOPTS="-I. -I/usr/local/include -I../include -fPIC"

FILES="math"

OBJECTS=""

for FILE in $FILES; do
	gcc -c $FILE.c $CCOPTS -o $FILE.o
	OBJECTS+="$FILE.o "
done

gcc $OBJECTS -shared -o ../../bin/${NAME}.so -L/usr/local/lib -lSDL -lm

rm $OBJECTS
