#!/bin/sh

# Esto es igual que MAKELIB.BAT, solo que para Linux

# OBSOLETO # ACTUALIZAR !

if ! test -f "stub"; then
	echo "no encuentro el fichero \"stub\" ¿lo has compilado?";
	exit 1;
fi

if ! test -f "encrypt"; then
	echo "no encuentro el programa \"encrypt\" ¿lo has compilado?";
	exit 1;
fi

./encrypt

if ! test -f "edivrun.a"; then
	echo "algo ha pasao, porque no se ha creado el \"edivrun.a\"...";
	exit 1;
fi
