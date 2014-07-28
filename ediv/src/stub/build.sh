#!/bin/bash

CCOPTS="-I. -I../shared -I/usr/local/include -DDBG"

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
