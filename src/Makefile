
OUT = exe
APP = main.c
LIBMENU = ../lib/menu.a
SRC = ../lib/menu.c
CC = gcc

all: static 

static: $(SRC)
		$(CC) -o $(OUT) $(APP) -I../include/ $(LIBMENU) 
