LDFLAGS = -ggdb
CFLAGS = -Wall
CC = gcc

TARGET = ../../../bin/so/edivstd.so

$(TARGET):	edivstd.c
	$(CC) $(LDFLAGS) $(CFLAGS) -I../ -shared -Wl,-soname,edivstd.so -o $@ $<