# eDIV Compiler
# Copyright (C) 2001 Sion Entertainment
# http://www.sion-e.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

# makefile chorizado descaradamente del fenix

debug = true
target = linux

CFLAGS += -Wall

# Usar debug=true para activar versión de depurado

ifeq ($(debug), true)
CFLAGS += -DDEBUG -g
else
CFLAGS += -O2 
endif

# Usar efence=true para activar la librería Electric Fence

ifeq ($(efence), true)
LIBS += -lefence
endif

# Usar target=win32 para compilar el .EXE de Windows

ifeq ($(target), win32)
TARGET = ../../bin/encrypt.exe
else
TARGET = ../../bin/encrypt
endif

CFLAGS += -DTARGET_$(target)

# Target

$(TARGET): encrypt.o
	$(CC) -o $@ $^ -lz

encrypt.o: encrypt.c $(HEADERS)


# Limpieza/empaquetado/etc.

clean:
	rm -f *.o core

