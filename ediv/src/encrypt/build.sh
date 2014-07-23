#!/bin/bash

CCOPTS="-I. -I../shared -I/usr/local/include -DDBG"

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
