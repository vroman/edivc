edivc
=====

eDivc was an attempt to make a DIV language compiler started in 2000 and publicly released in 2002. After many years and an unstable 0.1 version, it was left on [SourceForge](https://sf.net/projects/edivc) mostly abandoned. The code was written while all of us were about between 14-17 y/o and it had parts of the DIV Games Studio original compiler written in C for MS-DOS using Watcom C/C++. The quality is poor.

I took the work we (the eDiv Team at that time) did and moved into GitHub just for historical and nostalgic tracking, keeping the CVS commit history. At sometimes I may send some changes for fun, maybe I release a working (at that time state) version which can run the included test programs, but this is not a promise nor a thing I may do soon.

I don't have plans to maintain this, so feel free to fork and send pull requests if you want your changes merged.

You must know this thing won't compile nowadays with current versions of GCC, LLVM or Visual C++. I have sent some changes to build it on latest macOS builds, but it fails at runtime and needs many fixes. The code is not commented, and variables/functions are in Spanish. You have been warned :-).

textshot
========
```
macbookair:bin vroman$ ./edivc 
 **** Compilador eDIV 0.1.3b (Rigel) ****
Copyright (C) 2000-2005 eDIV Team
http://ediv.divsite.net
ATENCION: Esta version de eDIV esta inacabada.
Por favor informe de cualquier bug a: ediv@divsite.net

Ayuda de opciones de comando:

./edivc [opciones] archivo.prg [ejecutable]

Lista de opciones:
    -h, --help      Muestra esta ayuda y sale
    -d, --debug     Genera información adicional de depurado
    -l, --list      Guarda el listado EML como <programa.eml>
    -t, --table     Guarda la tabla de objetos como <programa.tab>
    -c, --check     No genera un ejecutable (para comprobar sintaxis, etc.)
    -s, --system    Sistema Operativo destino (por defecto p<?Q?)
    -n, --news      Muestra las últimas noticias de eDivCentral.com y sale
    -v, --version   Muestra versión y sale

macbookair:bin vroman$ 
```
