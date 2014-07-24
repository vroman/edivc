#!/bin/bash
NAME="input"
CCOPTS="-I. -I/usr/local/include -I../include"

FILES="input"

OBJECTS=""

for FILE in $FILES; do
	gcc -c $FILE.c $CCOPTS -o $FILE.o
	OBJECTS+="$FILE.o "
done

gcc $OBJECTS -shared -o ../../bin/${NAME}.so -L/usr/local/lib -lSDL

rm $OBJECTS
