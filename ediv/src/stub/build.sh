#!/bin/bash

DEBUG="-g -ggdb3 -D_DEBUG -DDBG -DDEBUG"
CCOPTS="-I. -I../shared -I/usr/local/include ${DEBUG} -fPIC"

FILES="
dll
ediv_export
error
fatal
inte
language
lower
modulos
stub
varindex
../shared/utils
"

OBJECTS=""

for FILE in $FILES; do
	gcc -c $FILE.c $CCOPTS -o $FILE.o
	OBJECTS+="$FILE.o "
done

gcc $OBJECTS -o ../../bin/stub -lz -ldl

rm $OBJECTS
