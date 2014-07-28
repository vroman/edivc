#!/bin/bash
DEBUG="-g -ggdb3 -D_DEBUG -DDBG -DDEBUG"
CCOPTS="-I. -I../shared -I/usr/local/include ${DEBUG} -fPIC"

FILES="
encrypt
"

OBJECTS=""

for FILE in $FILES; do
	gcc -c $FILE.c $CCOPTS -o $FILE.o
	OBJECTS+="$FILE.o "
done

gcc $OBJECTS -o ../../bin/encrypt -lz

rm $OBJECTS
