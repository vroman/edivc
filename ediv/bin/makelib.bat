@echo off

:: Esto crea EDIVRUN.LIB, a partir de STUB.EXE

if [%1]==[] goto err3
if exist temp.dj! del temp.dj!
if not exist stub.exe goto err1
if not exist encrypt.exe goto err2
if exist %1 del %1
encrypt.exe %1
if not exist %1 echo algo ha pasao, porque no se ha creado la %1...
goto fin
:err1
echo no encuentro STUB.EXE ¨lo has compilado?
goto fin
:err2
echo no encuentro ENCRYPT.EXE ¨lo has compilado?
goto fin
:err3
echo debes indicar el nombre de la libreria a crear (p.ej. WIN32.REL)
:fin
