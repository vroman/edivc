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

<<<<<<< HEAD
gcc $OBJECTS -o ../../bin/stub -lz
=======
gcc $OBJECTS -o ../../bin/stub -lz -ldl
>>>>>>> 6f1419a31008aa9314171e0f90b309fe54da51b0

rm $OBJECTS
